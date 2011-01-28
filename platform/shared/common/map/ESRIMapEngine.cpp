#include "common/map/ESRIMapEngine.h"
#include "common/map/GoogleMapEngine.h"
#include "common/RhoMath.h"
#include "common/RhoConf.h"
#include "common/IRhoClassFactory.h"

#include <list>

namespace rho
{
namespace common
{
namespace map
{

// DON'T CHANGE THIS CONSTANT!!!
// This is maximum absolute value of sine ( == sin(85*PI/180) ) allowed by Merkator projection
static double MAX_SIN = 0.99627207622;

static int const MIN_ZOOM = 0;
static int const MAX_ZOOM = 19;

static int const TILE_SIZE = 256;

uint64 degreesToPixelsX(int degrees, int zoom);
uint64 degreesToPixelsY(int degrees, int zoom);

static uint64 const MIN_LATITUDE = degreesToPixelsY(90, MAX_ZOOM);
static uint64 const MAX_LATITUDE = degreesToPixelsY(-90, MAX_ZOOM);
static uint64 const MIN_LONGITUDE = degreesToPixelsX(-180, MAX_ZOOM);
static uint64 const MAX_LONGITUDE = degreesToPixelsX(180, MAX_ZOOM);

static uint64 degreesToPixelsX(double n, int zoom)
{
    while (n < -180) n += 360;
    while (n > 180) n -= 360;
    double angleRatio = 360.0/rho_math_pow2(zoom);
    double val = TILE_SIZE * (n + 180)/angleRatio;
    return (uint64)val;
}

static uint64 degreesToPixelsY(double n, int zoom)
{
    // Merkator projection
    double sin_phi = rho_math_sin(n*M_PI/180);
    // MAX_SIN - maximum value of sine allowed by Merkator projection
    // (~85.0 degrees of north latitude)
    if (sin_phi < -MAX_SIN) sin_phi = -MAX_SIN;
    if (sin_phi > MAX_SIN) sin_phi = MAX_SIN;
    double ath = rho_math_atanh(sin_phi);
    double val = TILE_SIZE * rho_math_pow2(zoom) * (1 - ath/M_PI)/2;
    return (uint64)val;
}

static double pixelsToDegreesX(uint64 n, int zoom)
{
    while (n < 0) n += MAX_LONGITUDE;
    while (n > MAX_LONGITUDE) n -= MAX_LONGITUDE;
    double angleRatio = 360.0/rho_math_pow2(zoom);
    double val = n * angleRatio/TILE_SIZE - 180;
    return val;
}

static double pixelsToDegreesY(uint64 n, int zoom)
{
    // Revert calculation of Merkator projection
    double ath = M_PI - 2*M_PI*n/(TILE_SIZE * rho_math_pow2(zoom));
    double th = rho_math_tanh(ath);
    double val = 180*rho_math_asin(th)/M_PI;
    return val;
}

static uint64 toMaxZoom(uint64 n, int zoom)
{
    if (n == 0) return 0;
    uint64 pow = rho_math_pow2(MAX_ZOOM - zoom);
    return n*pow;
}

static uint64 toCurrentZoom(uint64 n, int zoom)
{
    if (n == 0) return 0;
    uint64 pow = rho_math_pow2(MAX_ZOOM - zoom);
    return n/pow;
}

//=============================================================================================
// Helper classes

struct CachedTile
{};

class TilesCache
{
public:
    typedef std::list<CachedTile>::const_iterator iterator;

public:
    TilesCache();
    ~TilesCache();

    TilesCache clone() const;

    iterator begin() const;
    iterator end() const;

private:
    std::list<CachedTile> m_tiles;
};

ESRIMapView::MapFetch::MapFetch(IMapView *view)
    :CRhoThread(rho_impl_createClassFactory()), m_mapview(view)
{
    start(epNormal);
}

ESRIMapView::MapFetch::~MapFetch()
{
    stop(1000);
}

ESRIMapView::CacheUpdate::CacheUpdate(IMapView *view)
    :CRhoThread(rho_impl_createClassFactory()), m_mapview(view)
{
    start(epNormal);
}

ESRIMapView::CacheUpdate::~CacheUpdate()
{
    stop(1000);
}

class ESRIGeoCodingCallback : public GeoCodingCallback
{
public:
    ESRIGeoCodingCallback(IMapView *view) :m_mapview(view) {}

    void onError(String const &error)
    {
        // TODO:
    }

    void onSuccess(double latitude, double longitude)
    {
        m_mapview->moveTo(latitude, longitude);
    }

private:
    IMapView *m_mapview;
};

// ESRIMapView implementation

ESRIMapView *ESRIMapEngine::createMapView(IDrawingDevice *device)
{
    return new ESRIMapView(device);
}

ESRIMapView::ESRIMapView(IDrawingDevice *device)
    :m_drawing_device(device), m_geo_coding(new GoogleGeoCoding())
{
    String url = RHOCONF().getString("esri_map_url_roadmap");
    if (url.empty())
        url = "http://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/";
    m_map_urls.put("roadmap", url);

    url = RHOCONF().getString("esri_map_url_satellite");
    if (url.empty())
        url = "http://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/";
    m_map_urls.put("satellite", url);
}

void ESRIMapView::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
}

String ESRIMapView::getMapUrl()
{
    String type = m_maptype;
    if (!m_map_urls.containsKey(type))
        type = "roadmap";
    return m_map_urls.get(type);
}

int ESRIMapView::minZoom() const
{
    return MIN_ZOOM;
}

int ESRIMapView::maxZoom() const
{
    return MAX_ZOOM;
}

void ESRIMapView::setZoom(int zoom)
{
    m_zoom = zoom;
    if (m_zoom < MIN_ZOOM) m_zoom = MIN_ZOOM;
    if (m_zoom > MAX_ZOOM) m_zoom = MAX_ZOOM;
}

static int calc_zoom(double degrees, int pixels, int tile_size)
{
    double angleRatio = degrees*tile_size/pixels;
    double twoInZoomExp = 360/angleRatio;
    int zoom = (int)rho_math_log2(twoInZoomExp);
    return zoom;
}

void ESRIMapView::setZoom(double latDelta, double lonDelta)
{
    int zoom1 = calc_zoom(latDelta, m_width, TILE_SIZE);
    int zoom2 = calc_zoom(lonDelta, m_height, TILE_SIZE);
    setZoom(std::min(zoom1, zoom2));
}

double ESRIMapView::latitude() const
{
    return pixelsToDegreesY(m_latitude, MAX_ZOOM);
}

double ESRIMapView::longitude() const
{
    return pixelsToDegreesX(m_longitude, MAX_ZOOM);
}

void ESRIMapView::setCoordinates(uint64 latitude, uint64 longitude)
{
    if (latitude < MIN_LATITUDE) latitude = MIN_LATITUDE;
    if (latitude > MAX_LATITUDE) latitude = MAX_LATITUDE;
    if (longitude > MIN_LONGITUDE) longitude = MIN_LONGITUDE;
    if (longitude < MAX_LONGITUDE) longitude = MAX_LONGITUDE;
    m_latitude = latitude;
    m_longitude = longitude;
}

void ESRIMapView::moveTo(double latitude, double longitude)
{
    uint64 lat = degreesToPixelsY(latitude, MAX_ZOOM);
    uint64 lon = degreesToPixelsX(longitude, MAX_ZOOM);
    setCoordinates(lat, lon);
}

void ESRIMapView::moveTo(String const &address)
{
    m_geo_coding->resolve(address, new ESRIGeoCodingCallback(this));
}

void ESRIMapView::move(int dx, int dy)
{
    uint64 lat = m_latitude;
    lat += toMaxZoom(dy, m_zoom);
    uint64 lon = m_longitude;
    lon += toMaxZoom(dx, m_zoom);
    setCoordinates(lat, lon);
}

void ESRIMapView::addAnnotation(Annotation const &ann)
{
    // TODO:
}

bool ESRIMapView::handleClick(int x, int y)
{
    // TODO:
    return false;
}

void ESRIMapView::paint(IDrawingContext *context)
{
    // TODO:
}

} // namespace map
} // namespace common
} // namespace rho
