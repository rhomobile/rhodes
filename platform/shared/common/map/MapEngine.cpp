#include "common/map/MapEngine.h"
#include "common/map/GoogleMapEngine.h"
#include "common/map/ESRIMapEngine.h"
#include "ruby.h"

#include <algorithm>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MapEngine"

namespace rho
{
namespace common
{
namespace map
{

MapProvider &MapProvider::getInstance()
{
    static MapProvider instance;
    return instance;
}

MapProvider::MapProvider()
{
    // Initialize predefined engines
    //registerMapEngine("google", new GoogleMapEngine());
    registerMapEngine("esri", new ESRIMapEngine());
}

void MapProvider::registerMapEngine(String const &id, IMapEngine *engine)
{
    RAWTRACE2("Register map engine: id=%s, engine=%p", id.c_str(), engine);
    IMapEngine *old = m_engines.get(id);
    if (old)
        delete old;
    m_engines.put(id, engine);
}

void MapProvider::unregisterMapEngine(String const &id)
{
    RAWTRACE1("Unregister map engine: id=%s", id.c_str());
    IMapEngine *engine = m_engines.get(id);
    if (engine)
        delete engine;
    m_engines.remove(id);
}

IMapView *MapProvider::createMapView(String const &id, IDrawingDevice *device)
{
    IMapEngine *engine = m_engines.get(id);
    if (!engine)
        return 0;
    IMapView *view = engine->createMapView(device);
    if (!view)
        return 0;
    m_cache.put(view, engine);
    return view;
}

void MapProvider::destroyMapView(IMapView *view)
{
    if (!view)
        return;
    IMapEngine *engine = m_cache.get(view);
    if (!engine)
        return;
    m_cache.remove(view);
    engine->destroyMapView(view);
}

String Annotation::make_address(double latitude, double longitude)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "%.5lf,%.5lf", latitude, longitude);
    return buf;
}

} // namespace map
} // namespace common
} // namespace rho

namespace rhomap = rho::common::map;

rhomap::IMapView *rho_map_create(rho_param *p, rhomap::IDrawingDevice *device, int width, int height)
{
    if (!p || !p->type == RHO_PARAM_HASH)
        rb_raise(rb_eArgError, "Wrong input parameter (expect Hash)");

    rho_param *provider = NULL;
    rho_param *settings = NULL;
    rho_param *annotations = NULL;
    for (int i = 0, lim = p->v.hash->size; i < lim; ++i)
    {
        char *name = p->v.hash->name[i];
        rho_param *value = p->v.hash->value[i];
        if (!name || !value)
            continue;

        if (strcasecmp(name, "provider") == 0)
            provider = value;
        else if (strcasecmp(name, "settings") == 0)
            settings = value;
        else if (strcasecmp(name, "annotations") == 0)
            annotations = value;
    }

    std::string providerId = "google";
    if (provider)
    {
        if (provider->type != RHO_PARAM_STRING)
            rb_raise(rb_eArgError, "Wrong 'provider' value (expect String)");
        providerId = provider->v.string;
    }
    std::transform(providerId.begin(), providerId.end(), providerId.begin(), &::tolower);

    char *map_type = NULL;
    bool use_center_radius = false;
    double latitude = 0;
    double longitude = 0;
    double latitudeSpan = 0;
    double longitudeSpan = 0;
    char *center = NULL;
    double radius = 0;
    bool zoom_enabled = true;
    bool scroll_enabled = true;

    if (settings)
    {
        if (settings->type != RHO_PARAM_HASH)
            rb_raise(rb_eArgError, "Wrong 'settings' value (expect Hash)");

        for (int i = 0, lim = settings->v.hash->size; i < lim; ++i)
        {
            char *name = settings->v.hash->name[i];
            rho_param *value = settings->v.hash->value[i];
            if (!name || !value)
                continue;

            if (strcasecmp(name, "map_type") == 0)
            {
                if (value->type != RHO_PARAM_STRING)
                    rb_raise(rb_eArgError, "Wrong 'map_type' value (expect String)");
                map_type = value->v.string;
            }
            else if (strcasecmp(name, "region") == 0)
            {
                if (value->type == RHO_PARAM_ARRAY)
                {
                    if (value->v.array->size != 4)
                        rb_raise(rb_eArgError, "'region' array should contain exactly 4 items");

                    rho_param *lat = value->v.array->value[0];
                    if (!lat) continue;
                    rho_param *lon = value->v.array->value[1];
                    if (!lon) continue;
                    rho_param *latSpan = value->v.array->value[2];
                    if (!latSpan) continue;
                    rho_param *lonSpan = value->v.array->value[3];
                    if (!lonSpan) continue;

                    latitude = lat->type == RHO_PARAM_STRING ? strtod(lat->v.string, NULL) : 0;
                    longitude = lon->type == RHO_PARAM_STRING ? strtod(lon->v.string, NULL) : 0;
                    latitudeSpan = latSpan->type == RHO_PARAM_STRING ? strtod(latSpan->v.string, NULL) : 0;
                    longitudeSpan = lonSpan->type == RHO_PARAM_STRING ? strtod(lonSpan->v.string, NULL) : 0;

                    use_center_radius = false;
                }
                else if (value->type == RHO_PARAM_HASH)
                {
                    for (int j = 0, limm = value->v.hash->size; j < limm; ++j)
                    {
                        char *rname = value->v.hash->name[i];
                        rho_param *rvalue = value->v.hash->value[i];
                        if (!rname || !rvalue)
                            continue;
                        if (strcasecmp(rname, "center") == 0)
                        {
                            if (rvalue->type != RHO_PARAM_STRING)
                                rb_raise(rb_eArgError, "Wrong 'center' value (expect String)");
                            center = rvalue->v.string;
                        }
                        else if (strcasecmp(rname, "radius") == 0)
                        {
                            if (rvalue->type != RHO_PARAM_STRING)
                                rb_raise(rb_eArgError, "Wrong 'radius' value (expect String or Float)");
                            radius = strtod(rvalue->v.string, NULL);
                        }
                    }

                    use_center_radius = true;
                }
                else
                    rb_raise(rb_eArgError, "Wrong 'region' value (expect Array or Hash");
            }
            else if (strcasecmp(name, "zoom_enabled") == 0)
            {
                if (value->type != RHO_PARAM_STRING)
                    rb_raise(rb_eArgError, "Wrong 'zoom_enabled' value (expect boolean)");
                zoom_enabled = strcasecmp(value->v.string, "true") == 0;
            }
            else if (strcasecmp(name, "scroll_enabled") == 0)
            {
                if (value->type != RHO_PARAM_STRING)
                    rb_raise(rb_eArgError, "Wrong 'scroll_enabled' value (expect boolean)");
                scroll_enabled = strcasecmp(value->v.string, "true") == 0;
            }
        }
    }

    typedef rhomap::Annotation ann_t;
    typedef std::vector<ann_t> ann_list_t;
    ann_list_t ann_list;
    if (annotations)
    {
        if (annotations->type != RHO_PARAM_ARRAY)
            rb_raise(rb_eArgError, "Wrong 'annotations' value (expect Array)");
        for (int i = 0, lim = annotations->v.array->size; i < lim; ++i)
        {
            rho_param *ann = annotations->v.array->value[i];
            if (!ann)
                continue;
            if (ann->type != RHO_PARAM_HASH)
                rb_raise(rb_eArgError, "Wrong annotation value found (expect Hash)");

            bool latitude_set = false;
            double latitude = 0;
            bool longitude_set = false;
            double longitude = 0;
            char const *address = "";
            char const *title = "";
            char const *subtitle = "";
            char const *url = "";

            for (int j = 0, limm = ann->v.hash->size; j < limm; ++j)
            {
                char *name = ann->v.hash->name[i];
                rho_param *value = ann->v.hash->value[i];
                if (!name || !value)
                    continue;

                if (value->type != RHO_PARAM_STRING)
                    rb_raise(rb_eArgError, "Wrong annotation value");

                char *v = value->v.string;
                if (strcasecmp(v, "latitude") == 0)
                {
                    latitude = strtod(v, NULL);
                    latitude_set = true;
                }
                else if (strcasecmp(v, "longitude") == 0)
                {
                    longitude = strtod(v, NULL);
                    longitude_set = true;
                }
                else if (strcasecmp(v, "street_address") == 0)
                    address = v;
                else if (strcasecmp(v, "title") == 0)
                    title = v;
                else if (strcasecmp(v, "subtitle") == 0)
                    subtitle = v;
                else if (strcasecmp(v, "url") == 0)
                    url = v;
            }

            if (latitude_set && longitude_set)
                ann_list.push_back(ann_t(title, subtitle, latitude, longitude, url));
            else
                ann_list.push_back(ann_t(title, subtitle, address, url));
        }
    }

    rhomap::IMapView *mapview = RHOMAPPROVIDER().createMapView(providerId, device);
    if (!mapview)
        return NULL;

    mapview->setSize(width, height);

    if (map_type)
        mapview->setMapType(map_type);

    if (use_center_radius)
    {
        mapview->moveTo(center);
        mapview->setZoom(radius, radius);
    }
    else
    {
        mapview->moveTo(latitude, longitude);
        mapview->setZoom(latitudeSpan, longitudeSpan);
    }

    mapview->setZoomEnabled(zoom_enabled);
    mapview->setScrollEnabled(scroll_enabled);

    for (ann_list_t::const_iterator it = ann_list.begin(), lim = ann_list.end(); it != lim; ++it)
        mapview->addAnnotation(*it);

    return mapview;
}

void rho_map_destroy(rho::common::map::IMapView *mapview)
{
    RHOMAPPROVIDER().destroyMapView(mapview);
}
