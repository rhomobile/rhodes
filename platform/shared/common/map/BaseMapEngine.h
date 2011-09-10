/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#ifndef _RHOBASEMAPENGINE_H_
#define _RHOBASEMAPENGINE_H_

#include "common/map/MapEngine.h"
#include "common/ThreadQueue.h"
#include "net/INetRequest.h"

#include "common/RhoThread.h"

#include <list>




namespace rho
{
namespace common
{
namespace map
{

    
#define TILE_SIZE 256
  
    
    
    uint64 degreesToPixelsX(double n, int zoom);    
    uint64 degreesToPixelsY(double n, int zoom);    
    uint64 normalize_latitude(int64 latitude);    
    uint64 normalize_longitude(int64 longitude);    
    double pixelsToDegreesX(uint64 n, int zoom);    
    double pixelsToDegreesY(uint64 n, int zoom);    
    int64 toMaxZoom(int64 n, int zoom);
    int64 toCurrentZoom(int64 n, int zoom);    
    
    
    
    
    class BaseMapView : public IMapView
    {
    private:
        // Non-copyable class
        BaseMapView(BaseMapView const &);
        BaseMapView &operator=(BaseMapView const &);
        
    public:
        bool fetchData(String const &url, void **data, size_t *datasize);
        virtual int getMapTile(uint64 p_zoom, uint64 p_row, uint64 p_column, void** p_data, size_t* p_size) = 0;
        
        
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
            typedef std::list<Tile> list;
            typedef std::list<Tile>::const_iterator iterator;
            
        public:
            list clone() const;
            
            bool empty() const {return m_tiles.empty();}
            
            void put(Tile const &tile);
            Tile const *get(int zoom, uint64 latitude, uint64 longitude) const;
            std::map<String, Tile *>& get_by_coordinates(){ return m_by_coordinates; }
            std::list<Tile>& get_tiles(){ return m_tiles; }
            
        private:
            static String makeKey(int zoom, uint64 latitude, uint64 longitude);
            
        private:
            std::list<Tile> m_tiles;
            std::map<String, Tile *> m_by_coordinates;
        };
        
        class MapFetch : public CThreadQueue
        {
            DEFINE_LOGCLASS;
            
        private:
            MapFetch(MapFetch const &);
            MapFetch &operator=(MapFetch const &);
            
            struct Command : public IQueueCommand
            {
                Command(String const &u, int z, uint64 lat, uint64 lon)
                :baseUrl(u), zoom(z), latitude(lat), longitude(lon), row(0), column(0)
                {}

                Command(int z, uint64 _row, uint64 _column)
                :baseUrl(""), zoom(z), latitude(0), longitude(0), row(_row), column(_column)
                {}
                
                String baseUrl;
                int zoom;
                uint64 latitude;
                uint64 longitude;
                uint64 row;
                uint64 column;
                
                bool equals(IQueueCommand const &) {return false;}
                String toString();
            };
            
        public:
            MapFetch(BaseMapView *view);
            ~MapFetch();
            
            void fetchTile(String const &baseUrl, int zoom, uint64 latitude, uint64 longitude);
            void fetchTile(int zoom, uint64 row, uint64 column);
            
            virtual void cancel()
            {
                m_NetRequest.cancel();
            }
            
        private:
            int getMapTile(uint64 p_zoom, uint64 p_row, uint64 p_column, void** p_data, size_t* p_size);
            void processCommand(IQueueCommand *cmd);
            bool fetchData(String const &url, void **data, size_t *datasize);
        public:    
            net::CNetRequestWrapper getNet(){ return getNetRequest(&m_NetRequest); }
            
        private:
            BaseMapView *m_mapview;
            NetRequest   m_NetRequest;
        };
        
        friend class CacheUpdate;
        class CacheUpdate : public CThreadQueue
        {
            DEFINE_LOGCLASS;
        private:
            CacheUpdate(CacheUpdate const &);
            CacheUpdate &operator=(CacheUpdate const &);
            
            struct Command : public IQueueCommand
            {
                Command() {}
                
                bool equals(IQueueCommand const &) {return false;}
                String toString() {return "CACHE-UPDATE";}
            };
            
        public:
            CacheUpdate(BaseMapView *);
            ~CacheUpdate();
            
            void updateCache();
            
        private:
            void processCommand(IQueueCommand *cmd);
            
        private:
            BaseMapView *m_mapview;
        };
        
        
    public:    
        
        bool m_isJustDownloadMode;
        
        BaseMapView(IDrawingDevice *device, const char* name);
        virtual ~BaseMapView();
                    
        String const &getName() const {return m_name;}            
        
        void setSize(int width, int height);
        int width() const {return m_width;}
        int height() const {return m_height;}
        
        void setZoomEnabled(bool v) {m_zoom_enabled = v;}
        bool zoomEnabled() const {return m_zoom_enabled;}
        
        void setScrollEnabled(bool v) {m_scroll_enabled = v;}
        bool scrollEnabled() const {return m_scroll_enabled;}
        
        void setMapType(String const &type);
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
        uint64 latitudeInt() const {return m_latitude;}
        double longitude() const;
        uint64 longitudeInt() const {return m_longitude;}
        
        void addAnnotation(Annotation &ann);
        
        bool handleClick(int x, int y);
        
        void paint(IDrawingContext *context);
        
        void setPinImage(IDrawingImage *pin, PIN_INFO pin_info);
        void setPinCalloutImage(IDrawingImage *pinCallout, PIN_INFO pin_callout_info);
        void setPinCalloutLinkImage(IDrawingImage *pinCallout, PIN_INFO pin_callout_info);
        virtual void setESRILogoImage(IDrawingImage *ESRILogoImg);
        virtual void setGoogleLogoImage(IDrawingImage *GoogleLogoImg);
        
        void set_file_caching_enable(int enable);

        int mEnableFileCaching;

        String const &getMapUrl();
        
        virtual int preloadMapTiles(double top_latitude, double left_longitude, double bottom_latitude, double right_longitude, int min_zoom, int max_zoom);
        virtual int getCountOfTilesToDownload();
        
    private:
        void setCoordinates(int64 latitude, int64 longitude);
        
        IDrawingDevice *drawingDevice() const {return m_drawing_device;}
        void redraw();
        
        void fetchTile(int zoom, uint64 latitude, uint64 longitude);
        void updateCache();
        
        void paintBackground(IDrawingContext *context);
        void paintTile(IDrawingContext *context, Tile const &tile);
        void paintAnnotation(IDrawingContext *context, Annotation &ann);
        void paintCallout(IDrawingContext *context, Annotation const &ann);
        
        CMutex &tilesCacheLock() {return m_tiles_cache_mtx;}
        TilesCache &tilesCache() {return m_tiles_cache;}
        
        int getAnnotation(int x, int y);
        
        int64 toScreenCoordinateX(double n);
        int64 toScreenCoordinateY(double n);
        
        bool isClickOnCallout(int x, int y, Annotation const &ann);
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
                    
        String m_name;
        
        int m_zoom;
        uint64 m_latitude;
        uint64 m_longitude;
        
        typedef Vector<Annotation> annotations_list_t;
        CMutex m_annotations_mtx;
        annotations_list_t m_annotations;
        int m_selected_annotation_index;
        
        CMutex m_tiles_cache_mtx;
        TilesCache m_tiles_cache;
        
        IDrawingImage *m_pin;
        PIN_INFO m_pin_info;
        IDrawingImage *m_pinCallout;
        PIN_INFO m_pin_callout_info;
        IDrawingImage *m_pinCalloutLink;
        PIN_INFO m_pin_calloutlink_info;
        IDrawingImage *m_ESRILogo;
        IDrawingImage *m_GoogleLogo;
        
        IDrawingImage *m_Callout;
        PIN_INFO m_CalloutInfo;
        Annotation* m_CalloutAnnotation;
        
    };
    
/*
    class GoogleMapEngine : public IMapEngine
    {
    public:
        GoogleMapView *createMapView(IDrawingDevice *device);
        void destroyMapView(IMapView *view);
    };
*/    
    
    
    
    
    
    
    

} // namespace map
} // namespace common
} // namespace rho

#endif
