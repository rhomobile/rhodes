#ifndef _RHOMAPVIEW_H_
#define _RHOMAPVIEW_H_

#include "logging/RhoLog.h"

namespace rho
{
namespace map
{

class IDrawingImage
{
public:
    virtual ~IDrawingImage() {}
};

class IDrawingDevice
{
public:
    virtual ~IDrawingDevice(){}

    virtual IDrawingImage* createImage(String path) = 0;
    virtual IDrawingImage* createImage(void* p, size_t s) = 0;
    virtual void destroyImage(IDrawingImage* image) = 0;

    virtual void requestRedraw() = 0;
};

class IDrawingContext
{
public:
    virtual ~IDrawingContext(){}

    virtual void drawImage(int x, int y, IDrawingImage* image) = 0;
    virtual void drawText(int x, int y, String &text, int color) = 0;
};

class Annotation
{
public:
    Annotation(String const &title, String const &subtitle,
        String const &address, String const &url)
        :m_title(title), m_subtitle(subtitle), m_address(address), m_url(url)
    {}

    String const &title() {return m_title;}
    String const &subtitle() {return m_subtitle;}
    String const &address() {return m_address;}
    String const &url() {return m_url;}

private:
    String m_title;
    String m_subtitle;
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

    virtual void stop() = 0;

    virtual void resolve(String const &address, GeoCodingCallback *callback) = 0;
};

class IMapEngine
{
public:
    virtual IMapView *createMapView(IDrawingDevice *drawingDevice) = 0;
};

class MapProvider
{
public:
    MapProvider();

    void registerMapEngine(String const &id, IMapEngine *engine);
    void unregisterMapEngine(String const &id);

    IMapView *createMapView(String const &id, IDrawingDevice *device);

private:
    Hashtable<String, IMapEngine *> m_engines;
};

} // namespace map
} // namespace rho

#endif

