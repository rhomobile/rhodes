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

namespace rho
{
namespace common
{
namespace map
{

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
    virtual ~IDrawingDevice(){}

    virtual IDrawingImage* createImage(String const &path, bool useAlpha) = 0;
    virtual IDrawingImage* createImage(void const *p, size_t s, bool useAlpha) = 0;
    virtual IDrawingImage* cloneImage(IDrawingImage *image) = 0;
    virtual void destroyImage(IDrawingImage* image) = 0;

    virtual void requestRedraw() = 0;
};

class IDrawingContext
{
public:
    virtual ~IDrawingContext(){}

    virtual void drawImage(int x, int y, IDrawingImage *image) = 0;
    virtual void drawText(int x, int y, String const &text, int color) = 0;
	virtual void fillRect(int x, int y, int width, int height, int color) = 0;
};

typedef struct {
	int x_offset;
	int y_offset;
	int click_rect_x;
	int click_rect_y;
	int click_rect_width;
	int click_rect_height;
}PIN_INFO;


class Annotation
{
public:
    Annotation(String const &title, String const &subtitle,
        double latitude, double longitude, String const &address,
        String const &url)
        :m_title(title), m_subtitle(subtitle), m_resolved(true),
        m_latitude(latitude), m_longitude(longitude),
        m_address(address), m_url(url)
    {}

    Annotation(String const &title, String const &subtitle,
        double latitude, double longitude, String const &url)
        :m_title(title), m_subtitle(subtitle), m_resolved(true),
        m_latitude(latitude), m_longitude(longitude),
        m_address(make_address(latitude, longitude)), m_url(url)
    {}

    Annotation(String const &title, String const &subtitle,
        String const &address, String const &url)
        :m_title(title), m_subtitle(subtitle), m_resolved(false),
        m_latitude(0), m_longitude(0),
        m_address(address), m_url(url)
    {}

    String const &title() const {return m_title;}
    String const &subtitle() const {return m_subtitle;}
    bool resolved() const {return m_resolved;}
    double latitude() const {return m_latitude;}
    double longitude() const {return m_longitude;}
    String const &address() const {return m_address;}
    String const &url() const {return m_url;}

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

    virtual void addAnnotation(Annotation const &ann) = 0;

    // Return true if this MapView need to be closed
    virtual bool handleClick(int x, int y) = 0;

    virtual void paint(IDrawingContext *device) = 0;

    virtual void setPinImage(IDrawingImage *pin, PIN_INFO pin_info) = 0;
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

    IMapView *createMapView(String const &id, IDrawingDevice *device);
    void destroyMapView(IMapView *view);

private:
    Hashtable<String, IMapEngine *> m_engines;
    Hashtable<IMapView *, IMapEngine *> m_cache;
};

} // namespace map
} // namespace common
} // namespace rho

inline rho::common::map::MapProvider &RHOMAPPROVIDER() {return rho::common::map::MapProvider::getInstance();}

rho::common::map::IMapView *rho_map_create(rho_param *p, rho::common::map::IDrawingDevice *device, int width, int height);
void rho_map_destroy(rho::common::map::IMapView *mapview);

#endif

