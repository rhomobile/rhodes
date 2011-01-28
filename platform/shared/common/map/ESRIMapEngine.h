#ifndef _RHOESRIMAPENGINE_H_
#define _RHOESRIMAPENGINE_H_

#include "common/map/MapEngine.h"
#include "common/RhoThread.h"

namespace rho
{
namespace common
{
namespace map
{

class ESRIMapView : public IMapView
{
private:
    // Non-copyable class
    ESRIMapView(ESRIMapView const &);
    ESRIMapView &operator=(ESRIMapView const &);

    class MapFetch : public CRhoThread
    {
    private:
        MapFetch(MapFetch const &);
        MapFetch &operator=(MapFetch const &);

    public:
        MapFetch(IMapView *view);
        ~MapFetch();

        void fetch();

    private:
        IMapView *m_mapview;
    };

    class CacheUpdate : public CRhoThread
    {
    private:
        CacheUpdate(CacheUpdate const &);
        CacheUpdate &operator=(CacheUpdate const &);

    public:
        CacheUpdate(IMapView *);
        ~CacheUpdate();

    private:
        IMapView *m_mapview;
    };

public:
    ESRIMapView(IDrawingDevice *device);

    void setSize(int width, int height);
    int width() const {return m_width;}
    int height() const {return m_height;}

    void setMapType(String const &type) {m_maptype = type;}
    String const &mapType() const {return m_maptype;}

    void setZoom(int zoom);
    void setZoom(double latDelta, double lonDelta);
    int zoom() const {return m_zoom;}
    int minZoom() const;
    int maxZoom() const;

    void moveTo(double latitude, double longitude);
    void moveTo(String const &address);
    void move(int dx, int dy);
    double latitude() const;
    double longitude() const;

    void addAnnotation(Annotation const &ann);

    bool handleClick(int x, int y);

    void paint(IDrawingContext *context);

private:
    String getMapUrl();
    void setCoordinates(uint64 latitude, uint64 longitude);

private:
    IDrawingDevice *m_drawing_device;

    Hashtable<String, String> m_map_urls;

    std::auto_ptr<IGeoCoding> m_geo_coding;

    std::auto_ptr<MapFetch> m_map_fetch;
    std::auto_ptr<CacheUpdate> m_cache_update;

    int m_width;
    int m_height;
    String m_maptype;

    int m_zoom;
    uint64 m_latitude;
    uint64 m_longitude;
};

class ESRIMapEngine : public IMapEngine
{
public:
    ESRIMapView *createMapView(IDrawingDevice *device);
};

} // namespace map
} // namespace common
} // namespace rho

#endif
