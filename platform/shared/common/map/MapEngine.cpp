#include "common/map/MapEngine.h"
#include "common/map/GoogleMapEngine.h"
#include "common/map/ESRIMapEngine.h"

namespace rho
{
namespace common
{
namespace map
{

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
    return engine->createMapView(device);
}

} // namespace map
} // namespace common
} // namespace rho

