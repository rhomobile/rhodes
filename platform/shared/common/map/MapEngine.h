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

#ifndef _RHOMAPVIEW_H_
#define _RHOMAPVIEW_H_

#include "logging/RhoLog.h"
#include "common/rhoparams.h"

//#define RHO_MAP_DEBUG

#ifdef RHO_MAP_DEBUG
#   define RHO_MAP_TRACE(fmt) RAWTRACE(fmt)
#   define RHO_MAP_TRACE1(fmt, arg1) RAWTRACE1(fmt, arg1)
#   define RHO_MAP_TRACE2(fmt, arg1, arg2) RAWTRACE2(fmt, arg1, arg2)
#   define RHO_MAP_TRACE3(fmt, arg1, arg2, arg3) RAWTRACE3(fmt, arg1, arg2, arg3)
#   define RHO_MAP_TRACE4(fmt, arg1, arg2, arg3, arg4) RAWTRACE4(fmt, arg1, arg2, arg3, arg4)
#else
#   define RHO_MAP_TRACE(fmt)
#   define RHO_MAP_TRACE1(fmt, arg1)
#   define RHO_MAP_TRACE2(fmt, arg1, arg2)
#   define RHO_MAP_TRACE3(fmt, arg1, arg2, arg3)
#   define RHO_MAP_TRACE4(fmt, arg1, arg2, arg3, arg4)
#endif

namespace rho { namespace common { namespace map {

class IDrawingImage
{
public:
    virtual ~IDrawingImage() {}

    virtual int width() const = 0;
    virtual int height() const = 0;
};

class IDrawingDevice
{
public:
    virtual ~IDrawingDevice() {}

    virtual IDrawingImage* createImage(String const &path, bool useAlpha) = 0;
    virtual IDrawingImage* createImage(void const *p, size_t s, bool useAlpha) = 0;
    virtual IDrawingImage* createImageEx(void const *p, size_t s, int x, int y, int w, int h, bool useAlpha) = 0;
    virtual IDrawingImage* cloneImage(IDrawingImage *image) = 0;
    virtual void destroyImage(IDrawingImage* image) = 0;

    virtual IDrawingImage* createCalloutImage(String const &title, String const &subtitle, String const& url, int* x_offset, int* y_offset) = 0;

    virtual void requestRedraw() = 0;
};

class IDrawingContext
{
public:
    virtual ~IDrawingContext() {}

    virtual void drawImage(int x, int y, IDrawingImage *image) = 0;
    virtual void drawText(int x, int y, int width, int height, String const &text, int color) = 0;
    virtual void fillRect(int x, int y, int width, int height, int color) = 0;
};

typedef struct
{
    int x_offset;
    int y_offset;
    int click_rect_x;
    int click_rect_y;
    int click_rect_width;
    int click_rect_height;
} PIN_INFO;


typedef enum
{
    SIMPLE,
    MY_LOCATION
} ANNOTATION_TYPE;

class Annotation
{
public:

    virtual ~Annotation() {}

    Annotation(String const &str_title, String const &str_subtitle,
               double lat, double lon, String const &addr,
               String const &str_url)
        :m_title(str_title), m_subtitle(str_subtitle), m_resolved(true),
         m_latitude(lat), m_longitude(lon),
         m_address(addr), m_url(str_url), m_x_off(0), m_y_off(0), m_data(NULL), m_type(SIMPLE)
    {
        if (m_title.empty())
            m_title = m_address;
    }

    Annotation(String const &str_title, String const &str_subtitle,
               double lat, double lon, String const &addr,
               String const &str_url, ANNOTATION_TYPE _type)
    :m_title(str_title), m_subtitle(str_subtitle), m_resolved(true),
    m_latitude(lat), m_longitude(lon),
    m_address(addr), m_url(str_url), m_x_off(0), m_y_off(0), m_data(NULL), m_type(_type)
    {
        if (m_title.empty())
            m_title = m_address;
    }


    Annotation(String const &str_title, String const &str_subtitle,
               double lat, double lon, String const &str_url)
        :m_title(str_title), m_subtitle(str_subtitle), m_resolved(true),
         m_latitude(lat), m_longitude(lon),
         m_address(make_address(lat, lon)), m_url(str_url), m_x_off(0), m_y_off(0), m_data(NULL), m_type(SIMPLE)
    {
        if (m_title.empty())
            m_title = m_address;
    }

    Annotation(String const &str_title, String const &str_subtitle,
               String const &addr, String const &str_url)
        :m_title(str_title), m_subtitle(str_subtitle), m_resolved(false),
         m_latitude(0), m_longitude(0),
         m_address(addr), m_url(str_url), m_x_off(0), m_y_off(0), m_data(NULL), m_type(SIMPLE)
    {
        if (m_title.empty())
            m_title = m_address;
    }

    void setImageFileName(String const &filename, int x_off, int y_off)
    {
        mImageFileName = filename;
        m_x_off = x_off;
        m_y_off = y_off;
    }

    String const &title() const
    {
        return m_title;
    }
    String const &subtitle() const
    {
        return m_subtitle;
    }
    bool resolved() const
    {
        return m_resolved;
    }
    double latitude() const
    {
        return m_latitude;
    }
    double longitude() const
    {
        return m_longitude;
    }
    String const &address() const
    {
        return m_address;
    }
    String const &url() const
    {
        return m_url;
    }
    String const &imageFileName() const
    {
        return mImageFileName;
    }
    ANNOTATION_TYPE type() const
    {
        return m_type;
    }
    int x_offset() const
    {
        return m_x_off;
    }
    int y_offset() const
    {
        return m_y_off;
    }
    void* data() const
    {
        return m_data;
    }
    void setData(void* p)
    {
        m_data = p;
    }
private:
    static String make_address(double latitude, double longitude);

private:
    String m_title;
    String m_subtitle;
    bool m_resolved;
    double m_latitude;
    double m_longitude;
    String m_address;
    String m_url;
    String mImageFileName;
    int m_x_off;
    int m_y_off;
    void* m_data;
    ANNOTATION_TYPE m_type;
};

class IMapView
{
public:
    virtual ~IMapView() {}

    virtual void setSize(int width, int height) = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual void setZoomEnabled(bool v) = 0;
    virtual bool zoomEnabled() const = 0;

    virtual void setScrollEnabled(bool v) = 0;
    virtual bool scrollEnabled() const = 0;

    virtual void setMapType(String const &type) = 0;
    virtual String const &mapType() const = 0;

    virtual void setZoom(int zoom) = 0;
    virtual void setZoom(double latDelta, double lonDelta) = 0;
    virtual int zoom() const = 0;
    virtual int minZoom() const = 0;
    virtual int maxZoom() const = 0;

    virtual void moveTo(double latitude, double longitude) = 0;
    virtual void moveTo(String const &address) = 0;
    virtual void move(int dx, int dy) = 0;
    virtual double latitude() const = 0;
    virtual double longitude() const = 0;

    virtual void addAnnotation(Annotation &ann) = 0;
    virtual void setMyLocation(double latitude, double longitude) = 0;

    // Return true if this MapView need to be closed
    virtual bool handleClick(int x, int y) = 0;

    virtual void paint(IDrawingContext *device) = 0;

    virtual void setPinImage(IDrawingImage *pin, PIN_INFO pin_info) = 0;
    virtual void setPinCalloutImage(IDrawingImage *pinCallout, PIN_INFO pin_callout_info) = 0;
    virtual void setPinCalloutLinkImage(IDrawingImage *pinCallout, PIN_INFO pin_callout_info) = 0;
    virtual void setESRILogoImage(IDrawingImage *esriLogoImg) = 0;
    virtual void setGoogleLogoImage(IDrawingImage *googleLogoImg) = 0;
    virtual void setPinMyLocationImage(IDrawingImage *img, PIN_INFO pin_info) = 0;

    virtual void set_file_caching_enable(int enable) = 0;

    // return count of tiles to download
    virtual int preloadMapTiles(double top_latitude, double left_longitude, double bottom_latitude, double right_longitude, int min_zoom, int max_zoom) = 0;
    virtual int getCountOfTilesToDownload() = 0;

};

class GeoCodingCallback
{
public:
    virtual void onError(String const &description) = 0;
    virtual void onSuccess(double latitude, double longitude) = 0;
};

class IGeoCoding
{
public:
    virtual ~IGeoCoding() {}

    //virtual void stop() = 0;

    virtual void resolve(String const &address, GeoCodingCallback *callback) = 0;
};

class IMapEngine
{
public:
    virtual IMapView *createMapView(IDrawingDevice *drawingDevice) = 0;
    virtual void destroyMapView(IMapView *view) = 0;
};

class MapProvider
{
private:
    MapProvider();

public:
    static MapProvider &getInstance();

    void registerMapEngine(String const &id, IMapEngine *engine);
    void unregisterMapEngine(String const &id);

    bool isRegisteredMapEngine(String const &id);

    IMapView *createMapView(String const &id, IDrawingDevice *device);
    void destroyMapView(IMapView *view);

private:
    Hashtable<String, IMapEngine *> m_engines;
    Hashtable<IMapView *, IMapEngine *> m_cache;
};

} // namespace map
} // namespace common
} // namespace rho

inline rho::common::map::MapProvider &RHOMAPPROVIDER()
{
    return rho::common::map::MapProvider::getInstance();
}

rho::common::map::IMapView *rho_map_create(rho_param *p, rho::common::map::IDrawingDevice *device, int width, int height);
void rho_map_destroy(rho::common::map::IMapView *mapview);
// check param for valid - now check only :provider (must be supported)
bool rho_map_check_param(rho_param *p);

#ifdef __cplusplus
extern "C"
#endif //__cplusplus
int mapview_preload_map_tiles(rho_param* p, const char* callback);

#endif

