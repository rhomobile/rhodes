#include "common/map/ESRIMapEngine.h"
#include "common/map/GoogleMapEngine.h"
#include "common/RhoMath.h"
#include "common/RhoConf.h"
#include "common/IRhoClassFactory.h"
#include "common/RhodesApp.h"
#include "net/INetRequest.h"

//TODO:
//0. create rho::graphics namespace, move all draw interfaces there, create Rect and Point class
//1. move classes to files, raname namespace to rho::map
//2. image cache: remove only unvisible images
//3. see TODO inside
//4. when stop thread - cancel current net request. Add cancelCurrentCommand to ThreadQueue and call it from stop
//5. move all platform code to appropriate folders

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifdef _MSC_VER
// Disable warnings about using "this" in member initializater list
#pragma warning(disable: 4355)
#define PRINTF_UINT64	"%I64u"
#define PRINTF_INT64	"%I64d"

#else

#define PRINTF_UINT64	"%llu"
#define PRINTF_INT64	"%lld"

#endif

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

#ifdef OS_WINCE
static int const MAX_TILES_CACHE_SIZE = 70;
#else
static int const MAX_TILES_CACHE_SIZE = 100;
#endif

static int const ANNOTATION_SENSITIVITY_AREA_RADIUS = 16;

static int const BACKGROUND_COLOR = 0x7F7F7F;
static int const CALLOUT_TEXT_COLOR = 0xFFFFFFFF;

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

static uint64 normalize_latitude(int64 latitude)
{
    while (latitude < 0) latitude += MAX_LATITUDE;
    while (latitude > (int64)MAX_LATITUDE) latitude -= MAX_LATITUDE;
    return (uint64)latitude;
}

static uint64 normalize_longitude(int64 longitude)
{
    while (longitude < 0) longitude += MAX_LONGITUDE;
    while (longitude > (int64)MAX_LONGITUDE) longitude -= MAX_LONGITUDE;
    return (uint64)longitude;
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
    m_image(m_device->createImage(data, datasize, false))
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

IMPLEMENT_LOGCLASS(ESRIMapView::MapFetch,"MapFetch");
ESRIMapView::MapFetch::MapFetch(ESRIMapView *view)
    :CThreadQueue(),
    m_mapview(view), m_net_request(rho_get_RhoClassFactory()->createNetRequest())
{
    CThreadQueue::setLogCategory(getLogCategory());

    start(epNormal);
}

ESRIMapView::MapFetch::~MapFetch()
{
    //stop(1000);
}

void ESRIMapView::MapFetch::fetchTile(String const &baseUrl, int zoom, uint64 latitude, uint64 longitude)
{
    RHO_MAP_TRACE3("fetchTile: zoom=%d, latitude="PRINTF_UINT64", longitude="PRINTF_UINT64"", zoom, latitude, longitude);
    addQueueCommandToFront(new Command(baseUrl, zoom, latitude, longitude));
}

bool ESRIMapView::MapFetch::fetchData(String const &url, void **data, size_t *datasize)
{
    RHO_MAP_TRACE1("fetchData: url=%s", url.c_str());
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
    RHO_MAP_TRACE1("processCommand: cmd=%s", cmd->toString().c_str());
    String url = cmd->baseUrl;
    if (url.empty())
    {
        RAWLOG_ERROR("MapFetch::processCommand: passed empty base url");
        return;
    }
    // Append last '/' if not exist
    if (url[url.size() - 1] != '/')
        url.push_back('/');
    int zoom = cmd->zoom;
    uint64 latitude = cmd->latitude;
    uint64 longitude = cmd->longitude;

    uint64 ts = toMaxZoom(TILE_SIZE, zoom);
    unsigned row = (unsigned)((latitude - ts/2)/ts);
    unsigned column = (unsigned)((longitude - ts/2)/ts);

    char buf[128];
    snprintf(buf, sizeof(buf), "MapServer/tile/%d/%d/%d", zoom, row, column);
    url += buf;

    void *data;
    size_t datasize;
    if (!fetchData(url, &data, &datasize))
        return;

    IDrawingDevice *device = m_mapview->drawingDevice();

    {
        synchronized (m_mapview->tilesCacheLock());
        m_mapview->tilesCache().put(Tile(device, zoom, latitude, longitude, data, datasize));
    }

    free(data);
    m_mapview->redraw();
}

String ESRIMapView::MapFetch::Command::toString()
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%d/"PRINTF_INT64"/"PRINTF_INT64"", zoom, latitude, longitude);
	RHO_MAP_TRACE1("ESRIMapView::MapFetch::Command::toString(): %s", buf);
    return String(&buf[0]);
}

IMPLEMENT_LOGCLASS(ESRIMapView::CacheUpdate,"CacheUpdate");
ESRIMapView::CacheUpdate::CacheUpdate(ESRIMapView *view)
    :CThreadQueue(), m_mapview(view)
{
    CThreadQueue::setLogCategory(getLogCategory());
    start(epNormal);
}

ESRIMapView::CacheUpdate::~CacheUpdate()
{
    //stop(200);
}

void ESRIMapView::CacheUpdate::updateCache()
{
    addQueueCommand(new Command());
}

void ESRIMapView::CacheUpdate::processCommand(IQueueCommand *c)
{
    RHO_MAP_TRACE("CacheUpdate: start");

    CMutex &lock = m_mapview->tilesCacheLock();
    TilesCache &cache = m_mapview->tilesCache();

    IDrawingDevice *drawingDevice = m_mapview->drawingDevice();

    int zoom = m_mapview->zoom();
    uint64 latitude = m_mapview->latitudeInt();
    uint64 longitude = m_mapview->longitudeInt();
    RHO_MAP_TRACE3("CacheUpdate: zoom=%d, latitude="PRINTF_UINT64", longitude="PRINTF_UINT64"", zoom, latitude, longitude);

    uint64 ts = toMaxZoom(TILE_SIZE, zoom);
    uint64 w = toMaxZoom(m_mapview->width(), zoom);
    uint64 h = toMaxZoom(m_mapview->height(), zoom);
    RHO_MAP_TRACE3("CacheUpdate: ts="PRINTF_UINT64", w="PRINTF_UINT64", h="PRINTF_UINT64"", ts, w, h);

    // Coordinates of the most top-left tile visible on the screen
    uint64 tileIndex = latitude <= h/2 ? 0 : (latitude - h/2)/ts;
    int64 latBegin = tileIndex*ts + ts/2;
    tileIndex = longitude <= w/2 ? 0 : (longitude - w/2)/ts;
    int64 lonBegin = tileIndex*ts + ts/2;
    RHO_MAP_TRACE2("CacheUpdate: latBegin="PRINTF_INT64", lonBegin="PRINTF_INT64"", latBegin, lonBegin);

    // Coordinates of the most bottom-right tile visible on the screen
    int64 latEnd = latBegin;
    while (latEnd < latBegin + (int64)h) latEnd += ts;
    if (latEnd - ts/2 < latitude + h/2) latEnd += ts;
    int64 lonEnd = lonBegin;
    while (lonEnd < lonBegin + (int64)w) lonEnd += ts;
    if (lonEnd - ts/2 < longitude + w/2) lonEnd += ts;
    RHO_MAP_TRACE2("CacheUpdate: latEnd="PRINTF_INT64", lonEnd="PRINTF_INT64"", latEnd, lonEnd);

    for (int64 lat = latEnd; lat >= latBegin; lat -= ts)
        for (int64 lon = lonEnd; lon >= lonBegin; lon -= ts)
        {
            uint64 latitude = normalize_latitude(lat);
            uint64 longitude = normalize_longitude(lon);

            synchronized (lock);
            Tile const *tile = cache.get(zoom, latitude, longitude);
            if (tile)
                continue;

            cache.put(Tile(drawingDevice, zoom, latitude, longitude));

            RHO_MAP_TRACE3("CacheUpdate: fetch tile: zoom=%d, latitude="PRINTF_UINT64", longitude="PRINTF_UINT64"", zoom, latitude, longitude);
            m_mapview->fetchTile(zoom, latitude, longitude);
        }

    RHO_MAP_TRACE("CacheUpdate: stop");
}

ESRIMapView::Tile const *ESRIMapView::TilesCache::get(int zoom, uint64 latitude, uint64 longitude) const
{
    //RHO_MAP_TRACE3("TilesCache::get: zoom=%d, latitude=%llu, longitude=%llu", zoom, latitude, longitude);
    String const &key = makeKey(zoom, latitude, longitude);
    std::map<String, Tile *>::const_iterator it = m_by_coordinates.find(key);
    Tile const *ptile = it == m_by_coordinates.end() ? NULL : it->second;
    RHO_MAP_TRACE3("TilesCache::get: key=%s, ptile=%p, cache size=%lu", key.c_str(), ptile, (unsigned long)m_tiles.size());
    return ptile;
}

void ESRIMapView::TilesCache::put(Tile const &tile)
{
    int zoom = tile.zoom();
    uint64 latitude = tile.latitude();
    uint64 longitude = tile.longitude();
    //RHO_MAP_TRACE3("TilesCache::put: zoom=%d, latitude=%llu, longitude=%llu", zoom, latitude, longitude);
    String const &key = makeKey(zoom, latitude, longitude);
    m_tiles.push_back(tile);
    Tile *ptile = &m_tiles.back();
    RHO_MAP_TRACE3("TilesCache::put: key=%s, ptile=%p, cache size=%lu", key.c_str(), ptile, (unsigned long)m_tiles.size());
    m_by_coordinates.insert(std::make_pair(key, ptile));

    // Throw away oldest tiles if limit reached
    //TODO: remove only unvisible images
    for (int i = MAX_TILES_CACHE_SIZE, lim = (int)m_tiles.size(); i < lim; ++i)
    {
        Tile const &tile = m_tiles.front();
        String const &key = makeKey(tile.zoom(), tile.latitude(), tile.longitude());
        RHO_MAP_TRACE1("TilesCache::put: limit reached, remove oldest tile (%s)", key.c_str());
        m_by_coordinates.erase(key);
        m_tiles.pop_front();
    }
}

ESRIMapView::TilesCache::list ESRIMapView::TilesCache::clone() const
{
    return m_tiles;
}

String ESRIMapView::TilesCache::makeKey(int zoom, uint64 latitude, uint64 longitude)
{
    char key[128];
    snprintf(key, sizeof(key), "%d/"PRINTF_UINT64"/"PRINTF_UINT64"", zoom, latitude, longitude);
    return key;
}

class CenterResolved : public GeoCodingCallback
{
public:
    CenterResolved(IMapView *view) :m_mapview(view) {}

    void onError(String const &) {}

    void onSuccess(double latitude, double longitude)
    {
        RHO_MAP_TRACE2("CenterResolved: latitude=%lf, longitude=%lf", latitude, longitude);
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
        RHO_MAP_TRACE2("AnnotationResolved: latitude=%lf, longitude=%lf", latitude, longitude);
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
    m_selected_annotation_index(-1),
    m_pinCallout(0), m_pinCalloutLink(0), m_pin(0)
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

ESRIMapView::~ESRIMapView()
{
    if ( m_map_fetch.get() != 0)
        m_map_fetch->stop(2000);

    if ( m_cache_update.get() != 0)
        m_cache_update->stop(2000);
}

void ESRIMapView::setSize(int width, int height)
{
    RHO_MAP_TRACE2("setSize: width=%d, height=%d", width, height);

    m_width = width;
    m_height = height;
}

void ESRIMapView::setMapType(String const &type)
{
    m_maptype = type;
    updateCache();
    redraw();
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

#if 0
static double calc_delta(int zoom, int pixels, int tile_size)
{
    RHO_MAP_TRACE3("calc_delta: zoom=%d, pixels=%d, tile_size=%d", zoom, pixels, tile_size);
    if (tile_size == 0)
        return 0;
    double twoInZoomExp = (double)rho_math_pow2(zoom);
    double angleRatio = 360/twoInZoomExp;
    double degrees = angleRatio*pixels/tile_size;
    RHO_MAP_TRACE1("calc_delta: degrees=%lf", degrees);
    return degrees;
}
#endif // #if 0

static int calc_zoom(double degrees, int pixels, int tile_size)
{
    RHO_MAP_TRACE3("calc_zoom: degrees=%lf, pixels=%d, tile_size=%d", degrees, pixels, tile_size);
    if (pixels == 0)
        return 0;
    double angleRatio = degrees*tile_size/pixels;
    double twoInZoomExp = 360/angleRatio;
    RHO_MAP_TRACE2("calc_zoom: angleRatio=%lf, twoInZoomExp=%lf", angleRatio, twoInZoomExp);
    int zoom = (int)rho_math_log2(twoInZoomExp);
    RHO_MAP_TRACE1("calc_zoom: zoom=%d", zoom);
    return zoom;
}

void ESRIMapView::setZoom(int zoom)
{
    RHO_MAP_TRACE1("setZoom: zoom=%d", zoom);
    m_zoom = zoom;
    if (m_zoom < MIN_ZOOM) m_zoom = MIN_ZOOM;
    if (m_zoom > MAX_ZOOM) m_zoom = MAX_ZOOM;
    updateCache();
    redraw();
}

void ESRIMapView::setZoom(double latDelta, double lonDelta)
{
    RHO_MAP_TRACE2("setZoom: latDelta=%lf, lonDelta=%lf", latDelta, lonDelta);
    int zoom1 = calc_zoom(latDelta, m_height, TILE_SIZE);
    int zoom2 = calc_zoom(lonDelta, m_width, TILE_SIZE);
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

void ESRIMapView::setCoordinates(int64 latitude, int64 longitude)
{
    RHO_MAP_TRACE2("setCoordinates: latitude="PRINTF_INT64", longitude="PRINTF_INT64"", latitude, longitude);
    m_latitude = normalize_latitude(latitude);
    m_longitude = normalize_longitude(longitude);
    updateCache();
    redraw();
}

void ESRIMapView::moveTo(double latitude, double longitude)
{
    RHO_MAP_TRACE2("moveTo: latitude=%lf, longitude=%lf", latitude, longitude);
    uint64 lat = degreesToPixelsY(latitude, MAX_ZOOM);
    uint64 lon = degreesToPixelsX(longitude, MAX_ZOOM);
    setCoordinates(lat, lon);
}

void ESRIMapView::moveTo(String const &address)
{
    RHO_MAP_TRACE1("moveTo: address=%s", address.c_str());
    m_geo_coding->resolve(address, new CenterResolved(this));
}

void ESRIMapView::move(int dx, int dy)
{
    RHO_MAP_TRACE2("move: dx=%d, dy=%d", dx, dy);
    if (dx == 0 && dy == 0)
        return;
    int64 lat = m_latitude;
    lat += toMaxZoom(dy, m_zoom);
    int64 lon = m_longitude;
    lon += toMaxZoom(dx, m_zoom);
    setCoordinates(lat, lon);
}

void ESRIMapView::addAnnotation(Annotation const &ann)
{
    if (ann.resolved())
    {
        RHO_MAP_TRACE("Add resolved annotation");
        {
            synchronized(m_annotations_mtx);
            m_annotations.addElement(ann);
        }
        redraw();
    }
    else if (!ann.address().empty())
    {
        RHO_MAP_TRACE1("Resolve annotation: address=%s", ann.address().c_str());
        m_geo_coding->resolve(ann.address(), new AnnotationResolved(this, ann));
    }
    else
        RAWLOG_ERROR("Attempt to add annotation with empty address");
}

int ESRIMapView::getAnnotation(int x, int y)
{
    synchronized(m_annotations_mtx);
    for ( int i = 0; i < (int)m_annotations.size(); i++ )
    {
        Annotation const &ann = m_annotations.elementAt(i);

        int64 ann_lon = (int64)degreesToPixelsX(ann.longitude(), m_zoom);
        int64 topleft_lon = toCurrentZoom(m_longitude, m_zoom) - m_width/2;
        int64 ann_lat = (int64)degreesToPixelsY(ann.latitude(), m_zoom);
        int64 topleft_lat = toCurrentZoom(m_latitude, m_zoom) - m_height/2;

        int annX = (int)(ann_lon - topleft_lon);
        int annY = (int)(ann_lat - topleft_lat);

		int click_rect_left = annX+m_pin_info.click_rect_x;
		int click_rect_top = annY+m_pin_info.click_rect_y;
		int click_rect_right = click_rect_left + m_pin_info.click_rect_width;
		int click_rect_bottom = click_rect_top + m_pin_info.click_rect_height;

		if ((x < click_rect_left) || (click_rect_right < x) || (y < click_rect_top) || (click_rect_bottom < y)) {
			continue;
		}
		/*
        int deltaX = x - annX;
        int deltaY = y - annY;

        double distance = rho_math_sqrt(deltaX*deltaX + deltaY*deltaY);
        if ((int)distance > ANNOTATION_SENSITIVITY_AREA_RADIUS)
            continue;
		*/
        return i;
    }

    return -1;
}

bool ESRIMapView::handleClick(int x, int y)
{
    synchronized(m_annotations_mtx);

    int old_selected = m_selected_annotation_index;

    //TODO: use m_rcCallout, remove isClickOnCallout
    if ( m_selected_annotation_index >=0 && isClickOnCallout(x,y,m_annotations.elementAt(m_selected_annotation_index) ))
    {
    }else
        m_selected_annotation_index = getAnnotation(x, y);

    if (m_selected_annotation_index>=0 && m_selected_annotation_index == old_selected )
    {
        Annotation& ann = m_annotations.elementAt(m_selected_annotation_index);

        // We have clicked on annotation - go to URL
		if (ann.url().size() > 0) {
			RHODESAPP().navigateToUrl(ann.url());
			m_selected_annotation_index = -1;
			return true;
		}
    }
    redraw();
    return false;
}

void ESRIMapView::setPinImage(IDrawingImage *pin, PIN_INFO pin_info)
{
    m_pin = pin;
	m_pin_info = pin_info;
}

void ESRIMapView::setPinCalloutImage(IDrawingImage *pinCallout, PIN_INFO pin_callout_info)
{
    m_pinCallout = pinCallout;
    m_pin_callout_info = pin_callout_info;
}

void ESRIMapView::setPinCalloutLinkImage(IDrawingImage *pinCallout, PIN_INFO pin_callout_info)
{
    m_pinCalloutLink = pinCallout;
    m_pin_calloutlink_info = pin_callout_info;
}

void ESRIMapView::fetchTile(int zoom, uint64 latitude, uint64 longitude)
{
    m_map_fetch->fetchTile(getMapUrl(), zoom, latitude, longitude);
}

void ESRIMapView::updateCache()
{
    m_cache_update->updateCache();
}

void ESRIMapView::redraw()
{
    m_drawing_device->requestRedraw();
}

int64 ESRIMapView::toScreenCoordinateX(double n)
{
    int64 v = (int64)degreesToPixelsX(n, m_zoom);
    int64 center = toCurrentZoom(m_longitude, m_zoom);
    int64 begin = center - m_width/2;
    return v - begin;
}

int64 ESRIMapView::toScreenCoordinateY(double n)
{
    int64 v = (int64)degreesToPixelsY(n, m_zoom);
    int64 center = toCurrentZoom(m_latitude, m_zoom);
    int64 begin = center - m_height/2;
    return v - begin;
}

void ESRIMapView::paint(IDrawingContext *context)
{
    paintBackground(context);

    {
        synchronized(m_tiles_cache_mtx);

        TilesCache::list mapTiles = m_tiles_cache.get_tiles();
        for (TilesCache::list::iterator it = mapTiles.begin(), lim = mapTiles.end(); it != lim; ++it)
            paintTile(context, *it);
    }

    {
        synchronized(m_annotations_mtx);
        for (annotations_list_t::iterator it = m_annotations.begin(), lim = m_annotations.end(); it != lim; ++it)
            paintAnnotation(context, *it);

        if (m_selected_annotation_index >= 0)
            paintCallout(context, m_annotations.elementAt(m_selected_annotation_index));
    }

/*
    TilesCache::list cache;
    {
        synchronized(m_tiles_cache_mtx);
        cache = m_tiles_cache.clone();
    }

    annotations_list_t annotations;
    int selected_annotation_index = -1;
    {
        synchronized(m_annotations_mtx);
        annotations = m_annotations;
        selected_annotation_index = m_selected_annotation_index;
    }

    for (TilesCache::iterator it = cache.begin(), lim = cache.end(); it != lim; ++it)
        paintTile(context, *it);

    for (annotations_list_t::iterator it = annotations.begin(), lim = annotations.end(); it != lim; ++it)
        paintAnnotation(context, *it);

    if (selected_annotation_index >= 0)
        paintCallout(context, annotations.elementAt(selected_annotation_index));  */
}

void ESRIMapView::paintBackground(IDrawingContext *context)
{
    // TODO:
}

void ESRIMapView::paintTile(IDrawingContext *context, Tile const &tile)
{
    IDrawingImage *image = tile.image();
	if (tile.zoom() != m_zoom) {
        return;
	}

    //RHO_MAP_TRACE3("=== paintTile: tile.zoom=%d, tile.latitude=%llu, tile.longitude=%llu",
    //    tile.zoom(), tile.latitude(), tile.longitude());
    //RHO_MAP_TRACE3("+++ paintTile: zoom=%d, latitude=%llu, longitude=%llu",
    //    m_zoom, m_latitude, m_longitude);

    // Difference between center of screen and center of tile (in screen pixels)
    int64 deltaX = toCurrentZoom(m_longitude - tile.longitude(), m_zoom);
    int64 deltaY = toCurrentZoom(m_latitude - tile.latitude(), m_zoom);
    //RHO_MAP_TRACE2("paintTile: deltaX=%lld, deltaY=%lld", deltaX, deltaY);

    int imgWidth = TILE_SIZE;
	if (image != NULL) image->width();
    int imgHeight = TILE_SIZE;
	if (image != NULL) image->height();

    //RHO_MAP_TRACE2("paintTile: imgWidth=%d, imgHeight=%d", imgWidth, imgHeight);
    //RHO_MAP_TRACE2("paintTile: m_width=%d, m_height=%d", m_width, m_height);

    int64 centerX = (int64)m_width/2 - deltaX;
    int64 centerY = (int64)m_height/2 - deltaY;
    //RHO_MAP_TRACE2("paintTile: centerX=%lld, centerY=%lld", centerX, centerY);

    int64 left = centerX - imgWidth/2;
    int64 top = centerY - imgHeight/2;
    //RHO_MAP_TRACE2("paintTile: left=%lld, top=%lld", left, top);

    // Convert MAX_LATITUDE/MAX_LONGITUDE to screen pixels on current zoom level
    int64 maxLat = toCurrentZoom(MAX_LATITUDE, m_zoom);
    int64 maxLon = toCurrentZoom(MAX_LONGITUDE, m_zoom);

    // Adjust image coordinates to be visible on the screen
    while (left + imgWidth > 0)
        left -= maxLon;
    while (top + imgHeight > 0)
        top -= maxLat;

    for (int64 x = left, limX = (int64)m_width; x < m_width; x += maxLon)
    {
        if (x + imgWidth < 0)
            continue;

        for (int64 y = top, limY = (int64)m_height; y < m_height; y += maxLat)
        {
            if (y + imgHeight < 0)
                continue;

			if (image != NULL) {
				context->drawImage((int)x, (int)y, image);
			}
			else {
                //TODO: draw grey image
				//context->fillRect((int)x, (int)y, imgWidth, imgHeight, BACKGROUND_COLOR);
			}
        }
    }
}

void ESRIMapView::paintAnnotation(IDrawingContext *context, Annotation const &ann)
{
    if (!ann.resolved())
        return;
    if (!m_pin)
        return;

    int pinWidth = m_pin->width();
    int pinHeight = m_pin->height();

	int64 x = toScreenCoordinateX(ann.longitude()) + m_pin_info.x_offset;
    if (((x + pinWidth) < 0) || (x > m_width))
        return;
	int64 y = toScreenCoordinateY(ann.latitude()) + m_pin_info.y_offset;
    if (((y + pinHeight) < 0) || (y > m_height))
        return;

    context->drawImage((int)x, (int)y, m_pin);
}

bool ESRIMapView::isClickOnCallout(int x, int y, Annotation const &ann)
{
    //TODO: remove this method. use m_rcCallout
    if (!m_pinCallout || !m_pin)
        return false;

    int pinCalloutWidth = m_pinCallout->width();
    int pinCalloutHeight = m_pinCallout->height();

	int64 xLoc = toScreenCoordinateX(ann.longitude());
	int64 yLoc = toScreenCoordinateY(ann.latitude());

    int64 xCallout = xLoc - pinCalloutWidth/2;
    //int64 yCallout = yLoc - pinCalloutHeight + m_pin_info.y_offset;
    int64 yCallout = yLoc + m_pin_info.y_offset - pinCalloutHeight;//- m_pin_info.y_offset;
#ifndef OS_ANDROID
	//yCallout -= m_pin_info.y_offset;
#endif

    return x > xCallout && x < xCallout + pinCalloutWidth && y > yCallout && y < yCallout + pinCalloutHeight;
}

void ESRIMapView::paintCallout(IDrawingContext *context, Annotation const &ann)
{
    if (!m_pinCallout || !m_pin)
        return;

    //TODO: create m_rcCallout
    int pinCalloutWidth = m_pinCallout->width();
    int pinCalloutHeight = m_pinCallout->height();

	int64 xLoc = toScreenCoordinateX(ann.longitude());
	int64 yLoc = toScreenCoordinateY(ann.latitude());

    int64 xCallout = xLoc - pinCalloutWidth/2;
    int64 yCallout = yLoc + m_pin_info.y_offset - pinCalloutHeight;//- m_pin->height() - pinCalloutHeight;
#ifndef OS_ANDROID
	//yCallout -= m_pin_info.y_offset;
#endif

    int reduceTextWidth = 0;
    if (ann.url().size() > 0) {
        context->drawImage((int)xCallout, (int)yCallout, m_pinCalloutLink);
        reduceTextWidth = 32;
    }
    else {
        context->drawImage((int)xCallout, (int)yCallout, m_pinCallout);
    }
    String strText;
    if ( ann.title().length() > 0 )
        strText += ann.title();

    if ( ann.subtitle().length() > 0 )
    {
        if ( strText.length() > 0 )
            strText += "\r\n";

        strText += ann.subtitle();
    }

    int nTextX = (int)xCallout + m_pin_callout_info.x_offset;
    int nTextY = (int)yCallout + m_pin_callout_info.y_offset;
    int nTextWidth = pinCalloutWidth - m_pin_callout_info.x_offset*2 - reduceTextWidth;
    int nTextHeight = pinCalloutHeight - m_pin_callout_info.y_offset*2;

    if ( strText.length() > 0 )
        context->drawText( nTextX, nTextY, nTextWidth, nTextHeight, strText, CALLOUT_TEXT_COLOR);
}

} // namespace map
} // namespace common
} // namespace rho
