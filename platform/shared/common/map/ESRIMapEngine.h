#ifndef _RHOESRIMAPENGINE_H_
#define _RHOESRIMAPENGINE_H_

#include "common/map/MapEngine.h"
#include "common/RhoThread.h"
#include "common/ThreadQueue.h"

#include <list>

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

    // Helper classes
    class Tile
    {
    public:
        Tile(IDrawingDevice *device, int z, uint64 lat, uint64 lon);
        Tile(IDrawingDevice *device, int z, uint64 lat, uint64 lon, void *data, size_t datasize);
        Tile(Tile const &c);
        ~Tile();

        Tile &operator=(Tile const &c);

        int zoom() const {return m_zoom;}
        uint64 latitude() const {return m_latitude;}
        uint64 longitude() const {return m_longitude;}
        IDrawingImage *image() const {return m_image;}

    private:
        void swap(Tile &tile);

    private:
        IDrawingDevice *m_device;
        int m_zoom;
        uint64 m_latitude;
        uint64 m_longitude;
        IDrawingImage *m_image;
    };

    class TilesCache
    {
    public:
        typedef std::list<Tile>::const_iterator iterator;

    public:
        TilesCache clone() const;

        iterator begin() const {return m_tiles.begin();}
        iterator end() const {return m_tiles.end();}

        bool empty() const {return m_tiles.empty();}

        void put(Tile const &tile);
        Tile const *get(int zoom, uint64 latitude, uint64 longitude) const;

    private:
        static String makeKey(int zoom, uint64 latitude, uint64 longitude);

    private:
        std::list<Tile> m_tiles;
        std::map<String, iterator> m_by_coordinates;
        std::map<uint64, iterator> m_by_time;
    };

    class MapFetch : public CThreadQueue
    {
    private:
        MapFetch(MapFetch const &);
        MapFetch &operator=(MapFetch const &);

        struct Command : public IQueueCommand
        {
            Command(String const &u, int z, uint64 lat, uint64 lon)
                :baseUrl(u), zoom(z), latitude(lat), longitude(lon)
            {}

            String baseUrl;
            int zoom;
            uint64 latitude;
            uint64 longitude;

            bool equals(IQueueCommand const &) {return false;}
            String toString();
        };

    public:
        MapFetch(ESRIMapView *view);
        ~MapFetch();

        void fetchTile(String const &baseUrl, int zoom, uint64 latitude, uint64 longitude);

    private:
        void processCommand(IQueueCommand *cmd);
        bool fetchData(String const &url, void **data, size_t *datasize);

    private:
        ESRIMapView *m_mapview;
        net::INetRequest *m_net_request;
    };

    friend class CacheUpdate;
    class CacheUpdate : public CRhoThread
    {
    private:
        CacheUpdate(CacheUpdate const &);
        CacheUpdate &operator=(CacheUpdate const &);

    public:
        CacheUpdate(ESRIMapView *);
        ~CacheUpdate();

    private:
        void run();

    private:
        ESRIMapView *m_mapview;
    };

public:
    ESRIMapView(IDrawingDevice *device);

    void setSize(int width, int height);
    int width() const {return m_width;}
    int height() const {return m_height;}

    void setZoomEnabled(bool v) {m_zoom_enabled = v;}
    bool zoomEnabled() const {return m_zoom_enabled;}

    void setScrollEnabled(bool v) {m_scroll_enabled = v;}
    bool scrollEnabled() const {return m_scroll_enabled;}

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
    String const &getMapUrl();
    void setCoordinates(uint64 latitude, uint64 longitude);

    IDrawingDevice *drawingDevice() const {return m_drawing_device;}

    void fetchTile(int zoom, uint64 latitude, uint64 longitude);

    void paintBackground(IDrawingContext *context);
    void paintTile(IDrawingContext *context, Tile const &tile);

    CMutex &tilesCacheLock() {return m_tiles_cache_mtx;}
    TilesCache &tilesCache() {return m_tiles_cache;}

private:
    IDrawingDevice *m_drawing_device;

    Hashtable<String, String> m_map_urls;

    std::auto_ptr<IGeoCoding> m_geo_coding;

    std::auto_ptr<MapFetch> m_map_fetch;
    std::auto_ptr<CacheUpdate> m_cache_update;

    int m_width;
    int m_height;

    bool m_zoom_enabled;
    bool m_scroll_enabled;
    String m_maptype;

    int m_zoom;
    uint64 m_latitude;
    uint64 m_longitude;

    Vector<Annotation> m_annotations;

    CMutex m_tiles_cache_mtx;
    TilesCache m_tiles_cache;
};

class ESRIMapEngine : public IMapEngine
{
public:
    ESRIMapView *createMapView(IDrawingDevice *device);
    void destroyMapView(IMapView *view);
};

} // namespace map
} // namespace common
} // namespace rho

#endif
