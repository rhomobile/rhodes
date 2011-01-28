#include "common/map/MapEngine.h"
#include "common/map/GoogleMapEngine.h"
#include "common/map/ESRIMapEngine.h"
#include "ruby.h"

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
    IMapEngine *old = m_engines.get(id);
    if (old)
        delete old;
    m_engines.put(id, engine);
}

void MapProvider::unregisterMapEngine(String const &id)
{
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
    m_cache.put(view, engine);
    return view;
}

void MapProvider::destroyMapView(IMapView *view)
{
    IMapEngine *engine = m_cache.get(view);
    if (!engine)
        return;
    m_cache.remove(view);
    engine->destroyMapView(view);
}

} // namespace map
} // namespace common
} // namespace rho

rho::common::map::IMapView *rho_map_create(rho_param *p, rho::common::map::IDrawingDevice *device)
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

    char const *providerId = "google";
    if (provider)
    {
        if (provider->type != RHO_PARAM_STRING)
            rb_raise(rb_eArgError, "Wrong 'provider' value (expect String)");
        providerId = provider->v.string;
    }

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

    rho::common::map::IMapView *mapview = RHOMAPPROVIDER().createMapView(providerId, device);
    if (!mapview)
        return NULL;

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

    return mapview;
}
