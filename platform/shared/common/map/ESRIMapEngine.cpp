#include "common/map/ESRIMapEngine.h"
#include "common/map/GoogleMapEngine.h"
#include "common/RhoMath.h"
#include "common/RhoConf.h"
#include "common/IRhoClassFactory.h"
#include "common/RhodesApp.h"
#include "net/INetRequest.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "ESRIMapEngine"

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

static int const CACHE_UPDATE_INTERVAL = 500;

static int const ANNOTATION_SENSITIVITY_AREA_RADIUS = 16;

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

//static uint64 const MIN_LATITUDE = degreesToPixelsY(90, MAX_ZOOM);
static uint64 const MAX_LATITUDE = degreesToPixelsY(-90, MAX_ZOOM);
//static uint64 const MIN_LONGITUDE = degreesToPixelsX(-180, MAX_ZOOM);
static uint64 const MAX_LONGITUDE = degreesToPixelsX(180, MAX_ZOOM);

static uint64 normalize_latitude(uint64 latitude)
{
    while (latitude < 0) latitude += MAX_LATITUDE;
    while (latitude > MAX_LATITUDE) latitude -= MAX_LATITUDE;
    return latitude;
}

static uint64 normalize_longitude(uint64 longitude)
{
    while (longitude < 0) longitude += MAX_LONGITUDE;
    while (longitude > MAX_LONGITUDE) longitude -= MAX_LONGITUDE;
    return longitude;
}

static double pixelsToDegreesX(uint64 n, int zoom)
{
    n = normalize_longitude(n);
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

static int64 toMaxZoom(int64 n, int zoom)
{
    if (n == 0) return 0;
    int64 pow = rho_math_pow2(MAX_ZOOM - zoom);
    return n*pow;
}

static int64 toCurrentZoom(int64 n, int zoom)
{
    if (n == 0) return 0;
    int64 pow = rho_math_pow2(MAX_ZOOM - zoom);
    return n/pow;
}

//=============================================================================================
// Helper classes

ESRIMapView::Tile::Tile(IDrawingDevice *device, int z, uint64 lat, uint64 lon)
    :m_device(device), m_zoom(z), m_latitude(lat), m_longitude(lon), m_image(0)
{}

ESRIMapView::Tile::Tile(IDrawingDevice *device, int z, uint64 lat, uint64 lon, void *data, size_t datasize)
    :m_device(device), m_zoom(z), m_latitude(lat), m_longitude(lon),
    m_image(m_device->createImage(data, datasize))
{}

ESRIMapView::Tile::Tile(ESRIMapView::Tile const &c)
    :m_device(c.m_device), m_zoom(c.m_zoom), m_latitude(c.m_latitude), m_longitude(c.m_longitude),
    m_image(m_device->cloneImage(c.m_image))
{}

ESRIMapView::Tile::~Tile()
{
    m_device->destroyImage(m_image);
}

ESRIMapView::Tile &ESRIMapView::Tile::operator=(ESRIMapView::Tile const &c)
{
    Tile copy(c);
    swap(copy);
    return *this;
}

void ESRIMapView::Tile::swap(ESRIMapView::Tile &tile)
{
    std::swap(m_zoom, tile.m_zoom);
    std::swap(m_latitude, tile.m_latitude);
    std::swap(m_longitude, tile.m_longitude);
    std::swap(m_image, tile.m_image);
}

ESRIMapView::MapFetch::MapFetch(ESRIMapView *view)
    :CThreadQueue(rho_impl_createClassFactory()),
    m_mapview(view), m_net_request(getFactory()->createNetRequest())
{
    RAWTRACE(__PRETTY_FUNCTION__);
    start(epNormal);
}

ESRIMapView::MapFetch::~MapFetch()
{
    RAWTRACE(__PRETTY_FUNCTION__);
    stop(1000);
    delete m_net_request;
}

void ESRIMapView::MapFetch::fetchTile(String const &baseUrl, int zoom, uint64 latitude, uint64 longitude)
{
    RAWTRACE3("fetchTile: zoom=%d, latitude=%llu, longitude=%llu", zoom, latitude, longitude);
    addQueueCommand(new Command(baseUrl, zoom, latitude, longitude));
}

bool ESRIMapView::MapFetch::fetchData(String const &url, void **data, size_t *datasize)
{
    RAWTRACE1("fetchData: url=%s", url.c_str());
    NetResponse(resp, m_net_request->doRequest("GET", url, "", 0, 0));
    if (!resp.isOK())
        return false;
    *datasize = resp.getDataSize();
    *data = malloc(*datasize);
    if (!*data)
        return false;
    memcpy(*data, resp.getCharData(), *datasize);
    return true;
}

void ESRIMapView::MapFetch::processCommand(IQueueCommand *c)
{
    Command *cmd = (Command *)c;
    RAWTRACE1("processCommand: cmd=%s", cmd->toString().c_str());
    String url = cmd->baseUrl;
    int zoom = cmd->zoom;
    uint64 latitude = normalize_latitude(cmd->latitude);
    uint64 longitude = normalize_longitude(cmd->longitude);

    uint64 ts = toMaxZoom(TILE_SIZE, zoom);
    unsigned row = (unsigned)(latitude/ts);
    unsigned column = (unsigned)(longitude/ts);

    char buf[128];
    snprintf(buf, sizeof(buf), "/MapServer/tile/%d/%d/%d", zoom, row, column);
    url += buf;

    void *data;
    size_t datasize;
    if (fetchData(url, &data, &datasize))
    {
        IDrawingDevice *device = m_mapview->drawingDevice();

        uint64 lat = ts*row + ts/2;
        uint64 lon = ts*column + ts/2;

        synchronized (m_mapview->tilesCacheLock());
        m_mapview->tilesCache().put(Tile(device, zoom, lat, lon, data, datasize));
        free(data);
        device->requestRedraw();
    }

    RAWTRACE("processCommand done");
}

String ESRIMapView::MapFetch::Command::toString()
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%d/%llu/%llu", zoom, latitude, longitude);
    return String(&buf[0]);
}

ESRIMapView::CacheUpdate::CacheUpdate(ESRIMapView *view)
    :CRhoThread(rho_impl_createClassFactory()), m_mapview(view)
{
    RAWTRACE(__PRETTY_FUNCTION__);
    start(epNormal);
}

ESRIMapView::CacheUpdate::~CacheUpdate()
{
    RAWTRACE(__PRETTY_FUNCTION__);
    stop(1000);
}

void ESRIMapView::CacheUpdate::run()
{
    RAWTRACE("CacheUpdate: start");

    CMutex &lock = m_mapview->tilesCacheLock();
    TilesCache &cache = m_mapview->tilesCache();
    while (!isStopping())
    {
        //RAWTRACE1("CacheUpdate: wait for %dms", CACHE_UPDATE_INTERVAL);
        //wait(CACHE_UPDATE_INTERVAL);
        ::usleep(CACHE_UPDATE_INTERVAL*1000);

        IDrawingDevice *drawingDevice = m_mapview->drawingDevice();

        int zoom = m_mapview->zoom();
        uint64 latitude = m_mapview->latitudeInt();
        uint64 longitude = m_mapview->longitudeInt();
        //uint64 latitude = toCurrentZoom(m_mapview->latitudeInt(), zoom);
        //uint64 longitude = toCurrentZoom(m_mapview->longitudeInt(), zoom);
        RAWTRACE3("CacheUpdate: zoom=%d, latitude=%llu, longitude=%llu", zoom, latitude, longitude);

        uint64 ts = toMaxZoom(TILE_SIZE, zoom);
        uint64 w = toMaxZoom(m_mapview->width(), zoom);
        uint64 h = toMaxZoom(m_mapview->height(), zoom);
        //uint64 ts = TILE_SIZE;
        //uint64 w = m_mapview->width();
        //uint64 h = m_mapview->height();
        RAWTRACE3("CacheUpdate: ts=%llu, w=%llu, h=%llu", ts, w, h);

        uint64 totalTiles = rho_math_pow2(zoom);
        RAWTRACE1("CacheUpdate: totalTiles=%llu", totalTiles);

        // Coordinates of the most top-left tile visible on the screen
        uint64 latBegin = ((latitude < h/2 ? 0 : latitude - h/2)/ts)*ts + ts/2;
        uint64 lonBegin = ((longitude < w/2 ? 0 : longitude - w/2)/ts)*ts + ts/2;
        RAWTRACE2("CacheUpdate: latBegin=%llu, lonBegin=%llu", latBegin, lonBegin);

        // Coordinates of the most bottom-right tile visible on the screen
        //uint64 latEnd = std::min(((latBegin - ts/2 + h)/ts + 1)*ts, ts*totalTiles - ts/2);
        //uint64 lonEnd = std::min(((lonBegin - ts/2 + w)/ts + 1)*ts, ts*totalTiles - ts/2);
        uint64 latEnd = ((latBegin - ts/2 + h)/ts + 1)*ts;
        uint64 lonEnd = ((lonBegin - ts/2 + w)/ts + 1)*ts;
        RAWTRACE2("CacheUpdate: latEnd=%llu, lonEnd=%llu", latEnd, lonEnd);

        for (uint64 lat = latBegin; lat <= latEnd; lat += ts)
            for (uint64 lon = lonBegin; lon <= lonEnd; lon += ts)
            {
                //uint64 latitude = toMaxZoom(lat, zoom);
                //uint64 longitude = toMaxZoom(lon, zoom);
                uint64 latitude = lat;
                uint64 longitude = lon;
                {
                    synchronized (lock);
                    Tile const *tile = cache.get(zoom, latitude, longitude);
                    if (tile)
                        continue;

                    cache.put(Tile(drawingDevice, zoom, latitude, longitude));
                }

                RAWTRACE3("CacheUpdate: fetch tile: zoom=%d, lat=%llu, lon=%llu", zoom, lat, lon);
                m_mapview->fetchTile(zoom, latitude, longitude);
            }
    }

    RAWTRACE("CacheUpdate: stop");
}

ESRIMapView::Tile const *ESRIMapView::TilesCache::get(int zoom, uint64 latitude, uint64 longitude) const
{
    //RAWTRACE3("TilesCache::get: zoom=%d, latitude=%llu, longitude=%llu", zoom, latitude, longitude);
    String const &key = makeKey(zoom, latitude, longitude);
    std::map<String, iterator>::const_iterator it = m_by_coordinates.find(key);
    if (it == m_by_coordinates.end())
        return 0;
    return &(*(it->second));
}

void ESRIMapView::TilesCache::put(Tile const &tile)
{
    int zoom = tile.zoom();
    uint64 latitude = tile.latitude();
    uint64 longitude = tile.longitude();
    //RAWTRACE3("TilesCache::put: zoom=%d, latitude=%llu, longitude=%llu", zoom, latitude, longitude);
    String const &key = makeKey(zoom, latitude, longitude);
    m_tiles.insert(m_tiles.begin(), tile);
    m_by_coordinates.insert(std::make_pair(key, m_tiles.begin()));
    // TODO: support Windows
#if !defined(OS_WINDOWS) && !defined(OS_WINCE)
    timeval tv;
    gettimeofday(&tv, NULL);
    uint64 now = (uint64)tv.tv_sec*1000000 + tv.tv_usec;
    m_by_time.insert(std::make_pair(now, m_tiles.begin()));
#endif
    // TODO: throw away oldest tiles if limit reached
}

ESRIMapView::TilesCache ESRIMapView::TilesCache::clone() const
{
    TilesCache copy;
    for (iterator it = begin(), lim = end(); it != lim; ++it)
        copy.put(*it);
    return copy;
}

String ESRIMapView::TilesCache::makeKey(int zoom, uint64 latitude, uint64 longitude)
{
    char key[64];
    snprintf(key, sizeof(key), "%d/%llu/%llu", zoom, latitude, longitude);
    return key;
}

class CenterResolved : public GeoCodingCallback
{
public:
    CenterResolved(IMapView *view) :m_mapview(view) {}

    void onError(String const &) {}

    void onSuccess(double latitude, double longitude)
    {
        RAWTRACE2("CenterResolved: latitude=%lf, longitude=%lf", latitude, longitude);
        m_mapview->moveTo(latitude, longitude);
    }

private:
    IMapView *m_mapview;
};

class AnnotationResolved : public GeoCodingCallback
{
public:
    AnnotationResolved(IMapView *view, Annotation const &ann)
        :m_mapview(view), m_ann(ann)
    {}

    void onError(String const &) {}

    void onSuccess(double latitude, double longitude)
    {
        RAWTRACE2("AnnotationResolved: latitude=%lf, longitude=%lf", latitude, longitude);
        Annotation ann(m_ann.title(), m_ann.subtitle(), latitude, longitude, m_ann.url());
        m_mapview->addAnnotation(ann);
    }

private:
    IMapView *m_mapview;
    Annotation m_ann;
};

// ESRIMapView implementation

ESRIMapView *ESRIMapEngine::createMapView(IDrawingDevice *device)
{
    return new ESRIMapView(device);
}

void ESRIMapEngine::destroyMapView(IMapView *view)
{
    delete view;
}

ESRIMapView::ESRIMapView(IDrawingDevice *device)
    :m_drawing_device(device), m_geo_coding(new GoogleGeoCoding()),
    m_map_fetch(new MapFetch(this)), m_cache_update(new CacheUpdate(this)),
    m_width(0), m_height(0),
    m_zoom_enabled(true), m_scroll_enabled(true), m_maptype("roadmap"),
    m_zoom(MIN_ZOOM), m_latitude(degreesToPixelsY(0, MAX_ZOOM)), m_longitude(degreesToPixelsX(0, MAX_ZOOM)),
    m_selected_annotation(NULL)
{
    RAWTRACE(__PRETTY_FUNCTION__);
    String url = RHOCONF().getString("esri_map_url_roadmap");
    if (url.empty())
        url = "http://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/";
    m_map_urls.put("roadmap", url);

    url = RHOCONF().getString("esri_map_url_satellite");
    if (url.empty())
        url = "http://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/";
    m_map_urls.put("satellite", url);
}

ESRIMapView::~ESRIMapView()
{
    RAWTRACE(__PRETTY_FUNCTION__);
}

void ESRIMapView::setSize(int width, int height)
{
    RAWTRACE2("setSize: width=%d, height=%d", width, height);

    m_width = width;
    m_height = height;
}

String const &ESRIMapView::getMapUrl()
{
    String type = m_maptype;
    if (!m_map_urls.containsKey(type))
        type = "roadmap";
    return m_map_urls[type];
}

int ESRIMapView::minZoom() const
{
    return MIN_ZOOM;
}

int ESRIMapView::maxZoom() const
{
    return MAX_ZOOM;
}

static double calc_delta(int zoom, int pixels, int tile_size)
{
    RAWTRACE3("calc_delta: zoom=%d, pixels=%d, tile_size=%d", zoom, pixels, tile_size);
    if (tile_size == 0)
        return 0;
    double twoInZoomExp = rho_math_pow2(zoom);
    double angleRatio = 360/twoInZoomExp;
    double degrees = angleRatio*pixels/tile_size;
    RAWTRACE1("calc_delta: degrees=%lf", degrees);
    return degrees;
}

static int calc_zoom(double degrees, int pixels, int tile_size)
{
    RAWTRACE3("calc_zoom: degrees=%lf, pixels=%d, tile_size=%d", degrees, pixels, tile_size);
    if (pixels == 0)
        return 0;
    double angleRatio = degrees*tile_size/pixels;
    double twoInZoomExp = 360/angleRatio;
    RAWTRACE2("calc_zoom: angleRatio=%lf, twoInZoomExp=%lf", angleRatio, twoInZoomExp);
    int zoom = (int)rho_math_log2(twoInZoomExp);
    RAWTRACE1("calc_zoom: zoom=%d", zoom);
    return zoom;
}

void ESRIMapView::setZoom(int zoom)
{
    RAWTRACE1("setZoom: zoom=%d", zoom);
    m_zoom = zoom;
    if (m_zoom < MIN_ZOOM) m_zoom = MIN_ZOOM;
    if (m_zoom > MAX_ZOOM) m_zoom = MAX_ZOOM;
}

void ESRIMapView::setZoom(double latDelta, double lonDelta)
{
    RAWTRACE2("setZoom: latDelta=%lf, lonDelta=%lf", latDelta, lonDelta);
    int zoom1 = calc_zoom(latDelta, m_height, TILE_SIZE);
    int zoom2 = calc_zoom(lonDelta, m_width, TILE_SIZE);
    m_zoom = std::min(zoom1, zoom2);
    if (m_zoom < MIN_ZOOM) m_zoom = MIN_ZOOM;
    if (m_zoom > MAX_ZOOM) m_zoom = MAX_ZOOM;
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
    RAWTRACE2("setCoordinates: latitude=%llu, longitude=%llu", latitude, longitude);
    m_latitude = normalize_latitude(latitude);
    m_longitude = normalize_longitude(longitude);
}

void ESRIMapView::moveTo(double latitude, double longitude)
{
    RAWTRACE2("moveTo: latitude=%lf, longitude=%lf", latitude, longitude);
    uint64 lat = degreesToPixelsY(latitude, MAX_ZOOM);
    uint64 lon = degreesToPixelsX(longitude, MAX_ZOOM);
    setCoordinates(lat, lon);
}

void ESRIMapView::moveTo(String const &address)
{
    RAWTRACE1("moveTo: address=%s", address.c_str());
    m_geo_coding->resolve(address, new CenterResolved(this));
}

void ESRIMapView::move(int dx, int dy)
{
    RAWTRACE2("move: dx=%d, dy=%d", dx, dy);
    uint64 lat = m_latitude;
    lat += toMaxZoom(dy, m_zoom);
    uint64 lon = m_longitude;
    lon += toMaxZoom(dx, m_zoom);
    setCoordinates(lat, lon);
}

void ESRIMapView::addAnnotation(Annotation const &ann)
{
    if (ann.resolved())
        m_annotations.addElement(ann);
    else
        m_geo_coding->resolve(ann.address(), new AnnotationResolved(this, ann));
}

Annotation const *ESRIMapView::getAnnotation(int x, int y)
{
    for (annotations_list_t::const_iterator it = m_annotations.begin(), lim = m_annotations.end();
        it != lim; ++it)
    {
        Annotation const &ann = *it;

        int64 ann_lon = (int64)degreesToPixelsX(ann.longitude(), m_zoom);
        int64 topleft_lon = toCurrentZoom(m_longitude, m_zoom) - m_width/2;
        int64 ann_lat = (int64)degreesToPixelsY(ann.latitude(), m_zoom);
        int64 topleft_lat = toCurrentZoom(m_latitude, m_zoom) - m_height/2;

        int annX = (int)(ann_lon - topleft_lon);
        int annY = (int)(ann_lat - topleft_lat);

        int deltaX = x - annX;
        int deltaY = y - annY;

        double distance = rho_math_sqrt(deltaX*deltaX + deltaY*deltaY);
        if ((int)distance > ANNOTATION_SENSITIVITY_AREA_RADIUS)
            continue;

        return &ann;
    }

    return NULL;
}

bool ESRIMapView::handleClick(int x, int y)
{
    Annotation const *ann = getAnnotation(x, y);
    Annotation const *selected = m_selected_annotation;
    m_selected_annotation = ann;
    if (ann && selected == ann)
    {
        // We have clicked already selected annotation
        RHODESAPP().navigateToUrl(ann->url());
        m_selected_annotation = NULL;
        return true;
    }
    m_drawing_device->requestRedraw();
    return false;
}

void ESRIMapView::paint(IDrawingContext *context)
{
    RAWTRACE(__PRETTY_FUNCTION__);

    paintBackground(context);

    TilesCache cache;
    {
        synchronized(m_tiles_cache_mtx);
        cache = m_tiles_cache.clone();
    }

    for (TilesCache::iterator it = cache.begin(), lim = cache.end(); it != lim; ++it)
        paintTile(context, *it);
}

void ESRIMapView::paintBackground(IDrawingContext *context)
{
    // TODO:
}

void ESRIMapView::paintTile(IDrawingContext *context, Tile const &tile)
{
    IDrawingImage *image = tile.image();
    if (!image)
        return;
    if (tile.zoom() != m_zoom)
        return;

    RAWTRACE3("=== paintTile: tile.zoom=%d, tile.latitude=%llu, tile.longitude=%llu",
        tile.zoom(), tile.latitude(), tile.longitude());
    RAWTRACE3("+++ paintTile: zoom=%d, latitude=%llu, longitude=%llu",
        m_zoom, m_latitude, m_longitude);

    // Difference between center of screen and center of tile (in screen pixels)
    int64 deltaX = toCurrentZoom(m_longitude - tile.longitude(), m_zoom);
    int64 deltaY = toCurrentZoom(m_latitude - tile.latitude(), m_zoom);
    RAWTRACE2("paintTile: deltaX=%lld, deltaY=%lld", deltaX, deltaY);

    int imgWidth = image->width();
    int imgHeight = image->height();

    RAWTRACE2("paintTile: imgWidth=%d, imgHeight=%d", imgWidth, imgHeight);
    RAWTRACE2("paintTile: m_width=%d, m_height=%d", m_width, m_height);

    int64 centerX = (int64)m_width/2 - deltaX;
    int64 centerY = (int64)m_height/2 - deltaY;
    RAWTRACE2("paintTile: centerX=%lld, centerY=%lld", centerX, centerY);

    int64 left = centerX - imgWidth/2;
    int64 top = centerY - imgHeight/2;
    RAWTRACE2("paintTile: left=%lld, top=%lld", left, top);

    // Convert MAX_LATITUDE/MAX_LONGITUDE to screen pixels on current zoom level
    int64 maxLat = toCurrentZoom(MAX_LATITUDE, m_zoom);
    int64 maxLon = toCurrentZoom(MAX_LONGITUDE, m_zoom);

    // Adjust image coordinates to be visible on the screen
    while (left > (int64)m_width)
    {
        left -= maxLon;
        if (left + imgWidth < 0)
            return;
    }
    while (left + imgWidth < 0)
    {
        left += maxLon;
        if (left > (int64)m_width)
            return;
    }
    while (top > (int64)m_height)
    {
        top -= maxLat;
        if (top + imgHeight < 0)
            return;
    }
    while (top + imgHeight < 0)
    {
        top += maxLat;
        if (top > (int64)m_height)
            return;
    }

    context->drawImage((int)left, (int)top, image);
}

void ESRIMapView::fetchTile(int zoom, uint64 latitude, uint64 longitude)
{
    m_map_fetch->fetchTile(getMapUrl(), zoom, latitude, longitude);
}

} // namespace map
} // namespace common
} // namespace rho
