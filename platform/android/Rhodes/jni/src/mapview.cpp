#include "rhodes/JNIRhodes.h"

#include <common/rhoparams.h>
#include <common/map/MapEngine.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "AndroidMapEngine"

#define RHO_MAP_DISABLE_TRACE
#ifdef RHO_MAP_DISABLE_TRACE

#   ifdef RHO_MAP_TRACE
#   undef RHO_MAP_TRACE
#   endif
#   define RHO_MAP_TRACE(...)

#   ifdef RHO_MAP_TRACE1
#   undef RHO_MAP_TRACE1
#   endif
#   define RHO_MAP_TRACE1(...)

#   ifdef RHO_MAP_TRACE2
#   undef RHO_MAP_TRACE2
#   endif
#   define RHO_MAP_TRACE2(...)

#   ifdef RHO_MAP_TRACE3
#   undef RHO_MAP_TRACE3
#   endif
#   define RHO_MAP_TRACE3(...)

#   ifdef RHO_MAP_TRACE4
#   undef RHO_MAP_TRACE4
#   endif
#   define RHO_MAP_TRACE4(...)

#endif /* #ifdef RHO_MAP_DISABLE_TRACE */

namespace rho
{
namespace common
{
namespace map
{

class AndroidImage : public IDrawingImage
{
private:
    AndroidImage(AndroidImage const &);
    AndroidImage &operator=(AndroidImage const &);

    AndroidImage(int *count, jobject *bitmap);

    void init(JNIEnv *env);

public:
    AndroidImage(jobject bitmap);
    ~AndroidImage();

    AndroidImage *clone();

    jobject bitmap() const {return *m_bitmap;}

    int width() const {return m_width;}
    int height() const {return m_height;}

private:
    int *m_count;
    jobject *m_bitmap;
    int m_width;
    int m_height;
};

class AndroidDrawingContext : public IDrawingContext
{
public:
    AndroidDrawingContext(jobject d, jobject canvas)
        :m_device(d), m_canvas(canvas)
    {}

    void drawImage(int x, int y, IDrawingImage *image);
    void drawText(int x, int y, int width, int height, String const &text, int color);
    void fillRect(int x, int y, int width, int height, int color) {}
private:
    jobject m_device;
    jobject m_canvas;
};

class AndroidMapDevice : public IDrawingDevice
{
private:
    AndroidMapDevice(AndroidMapDevice const &);
    AndroidMapDevice &operator=(AndroidMapDevice const &);

public:
    AndroidMapDevice(rho_param *p);
    ~AndroidMapDevice();

    void attach(JNIEnv *env, jobject jDevice);
    void setPinImage(JNIEnv *env, jobject bitmap);
    void setPinCalloutImage(JNIEnv *env, jobject bitmap);
    void setPinCalloutLinkImage(JNIEnv *env, jobject bitmap);
    void setESRILogoImage(JNIEnv *env, jobject bitmap);
 
	rho_param *params() const {return m_params;}

    void setMapView(IMapView *mv);
    IMapView *mapView() const {return m_mapview;}

    IDrawingImage* createImage(String const &path, bool useAlpha);
    IDrawingImage* createImage(void const *p, size_t s, bool useAlpha);
    IDrawingImage* cloneImage(IDrawingImage *image);
    void destroyImage(IDrawingImage* image);

    void requestRedraw();

    void paint(jobject canvas);

private:
    rho_param *m_params;
    IMapView *m_mapview;
    jobject m_jdevice;
    std::auto_ptr<IDrawingImage> m_pin_image;
    std::auto_ptr<IDrawingImage> m_pin_calloutimage;
    std::auto_ptr<IDrawingImage> m_pin_calloutlinkimage;
    std::auto_ptr<IDrawingImage> m_esriLogo_image;
};

AndroidImage::AndroidImage(jobject bitmap)
    :m_count(new int(1)), m_bitmap(new jobject(bitmap))
{
    RHO_MAP_TRACE("AndroidImage: ctor start");
    JNIEnv *env = jnienv();
    env->NewGlobalRef(bitmap);
    env->DeleteLocalRef(bitmap);
    init(env);
    RHO_MAP_TRACE("AndroidImage: ctor finish");
}

AndroidImage::AndroidImage(int *count, jobject *bitmap)
    :m_count(count), m_bitmap(bitmap)
{
    RHO_MAP_TRACE("AndroidImage: private ctor start");
    ++*m_count;
    init(jnienv());
    RHO_MAP_TRACE("AndroidImage: private ctor finish");
}

void AndroidImage::init(JNIEnv *env)
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_BITMAP);
    if (!cls) return;
    jmethodID midWidth = getJNIClassMethod(env, cls, "getWidth", "()I");
    if (!midWidth) return;
    jmethodID midHeight = getJNIClassMethod(env, cls, "getHeight", "()I");
    if (!midHeight) return;

    m_width = env->CallIntMethod(*m_bitmap, midWidth);
    m_height = env->CallIntMethod(*m_bitmap, midHeight);
}

AndroidImage::~AndroidImage()
{
    RHO_MAP_TRACE("AndroidImage: dtor start");
    if (--*m_count == 0)
    {
        JNIEnv *env = jnienv();
        jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
        jmethodID mid = getJNIClassStaticMethod(env, cls, "destroyImage", "(Landroid/graphics/Bitmap;)V");

        if (cls && mid)
            env->CallStaticVoidMethod(cls, mid, *m_bitmap);

        env->DeleteGlobalRef(*m_bitmap);

        delete m_bitmap;
        delete m_count;
    }
    RHO_MAP_TRACE("AndroidImage: dtor finish");
}

AndroidImage *AndroidImage::clone()
{
    return new AndroidImage(m_count, m_bitmap);
}

void AndroidDrawingContext::drawImage(int x, int y, IDrawingImage *image)
{
    if (!image)
        return;

    RHO_MAP_TRACE3("drawImage: x=%d, y=%d, image=%p", x, y, image);

    jobject bitmap = ((AndroidImage*)image)->bitmap();

    JNIEnv *env = jnienv();
    jclass cls = env->GetObjectClass(m_device);
    if (!cls) return;
    jmethodID mid = getJNIClassMethod(env, cls, "drawImage", "(Landroid/graphics/Canvas;IILandroid/graphics/Bitmap;)V");
    if (!mid) return;

    env->CallVoidMethod(m_device, mid, m_canvas, x, y, bitmap);

    RHO_MAP_TRACE("drawImage done");
}

void AndroidDrawingContext::drawText(int x, int y, int width, int height, String const &text, int color)
{
    RHO_MAP_TRACE4("drawText: x=%d, y=%d, text=%s, color=%d", x, y, text.c_str(), color);

    JNIEnv *env = jnienv();
    jclass cls = env->GetObjectClass(m_device);
    if (!cls) return;
    jmethodID mid = getJNIClassMethod(env, cls, "drawText", "(Landroid/graphics/Canvas;IIIILjava/lang/String;I)V");
    if (!mid) return;

    env->CallVoidMethod(m_device, mid, m_canvas, x, y, width, height,
        rho_cast<jhstring>(text).get(), color);

    RHO_MAP_TRACE("drawText done");
}

AndroidMapDevice::AndroidMapDevice(rho_param *p)
    :m_params(rho_param_dup(p)), m_mapview(NULL), m_jdevice(NULL)
{
    RHO_MAP_TRACE("AndroidMapDevice: ctor start");
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "create", "(J)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, (jlong)this);
    RHO_MAP_TRACE("AndroidMapDevice: ctor finish");
}

AndroidMapDevice::~AndroidMapDevice()
{
    RHO_MAP_TRACE("AndroidMapDevice: dtor start");
    rho_param_free(m_params);
    JNIEnv *env = jnienv();
    if (m_jdevice)
        env->DeleteGlobalRef(m_jdevice);
    RHO_MAP_TRACE("AndroidMapDevice: dtor finish");
}

void AndroidMapDevice::attach(JNIEnv *env, jobject jDevice)
{
    RHO_MAP_TRACE2("AndroidMapDevice: attach m_jdevice=%p, jDevice=%p", m_jdevice, jDevice);
    m_jdevice = env->NewGlobalRef(jDevice);
    RHO_MAP_TRACE("AndroidMapDevice: attach done");
}

void AndroidMapDevice::setMapView(IMapView *mv)
{
    RHO_MAP_TRACE("AndroidMapDevice: setMapView: start");
    m_mapview = mv;
    if (m_mapview && m_pin_image.get()) {

	PIN_INFO pin_info;
	pin_info.x_offset = -10;
	pin_info.y_offset = -35;
	pin_info.click_rect_x = -10;
	pin_info.click_rect_y = -35;
	pin_info.click_rect_width = 20;
	pin_info.click_rect_height = 30;

        m_mapview->setPinImage(m_pin_image.get(), pin_info);

		PIN_INFO pin_info1 = {0};
		pin_info1.x_offset = 5;

        mv->setPinCalloutImage(m_pin_calloutimage.get(), pin_info1);
        mv->setPinCalloutLinkImage(m_pin_calloutlinkimage.get(), pin_info1);
		mv->setESRILogoImage(m_esriLogo_image.get());

    }
    RHO_MAP_TRACE("AndroidMapDevice: setMapView: finish");
}

void AndroidMapDevice::setPinImage(JNIEnv *env, jobject bitmap)
{
    RHO_MAP_TRACE("AndroidMapDevice: setPinImage: start");
    m_pin_image.reset(new AndroidImage(bitmap));
    IMapView *mv = mapView();
    if (mv) {

	PIN_INFO pin_info;
	pin_info.x_offset = -10;
	pin_info.y_offset = -35;
	pin_info.click_rect_x = -10;
	pin_info.click_rect_y = -35;
	pin_info.click_rect_width = 20;
	pin_info.click_rect_height = 30;

        mv->setPinImage(m_pin_image.get(), pin_info);
    }
    RHO_MAP_TRACE("AndroidMapDevice: setPinImage: finish");
}

void AndroidMapDevice::setPinCalloutImage(JNIEnv *env, jobject bitmap)
{
    RHO_MAP_TRACE("AndroidMapDevice: setPinCalloutImage: start");
    m_pin_calloutimage.reset(new AndroidImage(bitmap));
    IMapView *mv = mapView();
    if (mv) {

		PIN_INFO pin_info = {0};
		pin_info.x_offset = 5;

        mv->setPinCalloutImage(m_pin_calloutimage.get(), pin_info);
    }
    RHO_MAP_TRACE("AndroidMapDevice: setPinCalloutImage: finish");
}

void AndroidMapDevice::setPinCalloutLinkImage(JNIEnv *env, jobject bitmap)
{
    RHO_MAP_TRACE("AndroidMapDevice: setPinCalloutLinkImage: start");
    m_pin_calloutlinkimage.reset(new AndroidImage(bitmap));
    IMapView *mv = mapView();
    if (mv) {

		PIN_INFO pin_info = {0};
		pin_info.x_offset = 5;

        mv->setPinCalloutLinkImage(m_pin_calloutlinkimage.get(), pin_info);
    }
    RHO_MAP_TRACE("AndroidMapDevice: setPinCalloutLinkImage: finish");
}

void AndroidMapDevice::setESRILogoImage(JNIEnv *env, jobject bitmap) {
    RHO_MAP_TRACE("AndroidMapDevice: setESRILogoImage: start");
    m_esriLogo_image.reset(new AndroidImage(bitmap));
    IMapView *mv = mapView();
    if (mv) {
        mv->setESRILogoImage(m_esriLogo_image.get());
    }
    RHO_MAP_TRACE("AndroidMapDevice: setESRILogoImage: finish");
}



IDrawingImage *AndroidMapDevice::createImage(String const &path, bool useAlpha)
{
    RHO_MAP_TRACE1("createImage: %s", path.c_str());

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "createImage", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
    if (!mid) return NULL;

    jobject bitmap = env->CallStaticObjectMethod(cls, mid, rho_cast<jhstring>(path).get());
    IDrawingImage *image = new AndroidImage(bitmap);

    RHO_MAP_TRACE1("createImage: return image=%p", image);
    return image;
}

IDrawingImage *AndroidMapDevice::createImage(void const *p, size_t size, bool useAlpha)
{
    RHO_MAP_TRACE2("createImage: p=%p, size=%llu", p, (unsigned long long)size);

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "createImage", "([B)Landroid/graphics/Bitmap;");
    if (!mid) return NULL;

    jholder<jbyteArray> data = jholder<jbyteArray>(env->NewByteArray(size));
    if (!data) return NULL;
    env->SetByteArrayRegion(data.get(), 0, size, (jbyte const *)p);

    jobject bitmap = env->CallStaticObjectMethod(cls, mid, data.get());
    IDrawingImage *image = new AndroidImage(bitmap);

    RHO_MAP_TRACE1("createImage: return image=%p", image);
    return image;
}

IDrawingImage *AndroidMapDevice::cloneImage(IDrawingImage *image)
{
    RHO_MAP_TRACE1("cloneImage: image=%p", image);
    IDrawingImage *cloned = image ? ((AndroidImage *)image)->clone() : NULL;
    RHO_MAP_TRACE1("cloneImage: return image=%p", cloned);
    return cloned;
}

void AndroidMapDevice::destroyImage(IDrawingImage *image)
{
    RHO_MAP_TRACE1("destroyImage: image=%p", image);
    delete image;
    RHO_MAP_TRACE("destroyImage done");
}

void AndroidMapDevice::requestRedraw()
{
    RHO_MAP_TRACE1("requestRedraw: m_jdevice=%p", m_jdevice);

    if (m_jdevice)
    {
        JNIEnv *env = jnienv();
        jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
        if (!cls) return;
        jmethodID mid = getJNIClassMethod(env, cls, "redraw", "()V");
        if (!mid) return;
        env->CallVoidMethod(m_jdevice, mid);
    }

    RHO_MAP_TRACE("requestRedraw done");
}

void AndroidMapDevice::paint(jobject canvas)
{
    RHO_MAP_TRACE1("paint: m_jdevice=%p", m_jdevice);

    if (m_mapview)
    {
        std::auto_ptr<AndroidDrawingContext> context(new AndroidDrawingContext(m_jdevice, canvas));
        m_mapview->paint(context.get());
    }

    RHO_MAP_TRACE("paint done");
}

} // namespace map
} // namespace common
} // namespace rho

namespace rhomap = rho::common::map;
static rhomap::AndroidMapDevice *s_mapdevice = NULL;

static rhomap::AndroidMapDevice *device(JNIEnv *env, jlong nativeDevice)
{
    rhomap::AndroidMapDevice *d = (rhomap::AndroidMapDevice*)nativeDevice;
    if (!d)
    {
        env->ThrowNew(getJNIClass(RHODES_JAVA_CLASS_RUNTIME_EXCEPTION), "Specified wrong native device");
        return NULL;
    }

    return d;
}

static rhomap::IMapView *mapview(JNIEnv *env, jlong nativeDevice)
{
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    if (!d)
        return NULL;
    rhomap::IMapView *mv = d->mapView();
    if (!mv)
    {
        env->ThrowNew(getJNIClass(RHODES_JAVA_CLASS_RUNTIME_EXCEPTION), "Specified native device does not have mapview");
        return NULL;
    }

    return mv;
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_setSize
  (JNIEnv *env, jobject, jobject jDevice, jlong nativeDevice, jint width, jint height)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setSize: start");
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    rhomap::IMapView *mv = d->mapView();
    if (!mv) {
        mv = rho_map_create(d->params(), d, width, height);
        d->setMapView(mv);
        d->attach(env, jDevice);
    }
    if (mv)
        mv->setSize(width, height);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setSize: finish");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_setPinImage
  (JNIEnv *env, jobject, jlong nativeDevice, jobject bitmap)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setPinImage: start");
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    d->setPinImage(env, bitmap);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setPinImage: finish");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_setPinCalloutImage
  (JNIEnv *env, jobject, jlong nativeDevice, jobject bitmap)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setPinCalloutImage: start");
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    d->setPinCalloutImage(env, bitmap);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setPinCalloutImage: finish");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_setPinCalloutLinkImage
  (JNIEnv *env, jobject, jlong nativeDevice, jobject bitmap)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setPinCalloutLinkImage: start");
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    d->setPinCalloutLinkImage(env, bitmap);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setPinCalloutLinkImage: finish");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_setESRILogoImage
  (JNIEnv *env, jobject, jlong nativeDevice, jobject bitmap)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setESRILogoImage: start");
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    d->setESRILogoImage(env, bitmap);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setESRILogoImage: finish");
}


RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_mapview_MapView_minZoom
  (JNIEnv *env, jobject, jlong nativeDevice)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_minZoom: start");
    jint ret = 0;
    rhomap::IMapView *mv = mapview(env, nativeDevice);
    if (mv)
        ret = mv->minZoom();

    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_minZoom: finish");
    return ret;
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_mapview_MapView_maxZoom
  (JNIEnv *env, jobject, jlong nativeDevice)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_maxZoom: start");
    jint ret = 0;
    rhomap::IMapView *mv = mapview(env, nativeDevice);
    if (mv)
        ret = mv->maxZoom();

    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_maxZoom: finish");
    return ret;
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_mapview_MapView_zoom
  (JNIEnv *env, jobject, jlong nativeDevice)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_zoom: start");
    jint ret = 0;
    rhomap::IMapView *mv = mapview(env, nativeDevice);
    if (mv)
        ret = mv->zoom();

    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_zoom: finish");
    return ret;
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_setZoom
  (JNIEnv *env, jobject, jlong nativeDevice, jint zoom)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setZoom: start");
    rhomap::IMapView *mv = mapview(env, nativeDevice);
    if (mv)
        mv->setZoom(zoom);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setZoom: finish");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_move
  (JNIEnv *env, jobject, jlong nativeDevice, jint dx, jint dy)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_move: start");
    rhomap::IMapView *mv = mapview(env, nativeDevice);
    if (mv)
        mv->move(dx, dy);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_move: finish");
}


RHO_GLOBAL void mapview_close();



RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_click
  (JNIEnv *env, jobject, jlong nativeDevice, jint x, jint y)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_click: start");
    rhomap::IMapView *mv = mapview(env, nativeDevice);
    if (mv) {
        if (mv->handleClick(x, y) ) {
            // close
            mapview_close();
	}
    }
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_click: finish");
}



RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_paint
  (JNIEnv *env, jobject, jlong nativeDevice, jobject canvas)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_paint: start");
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    if (d)
        d->paint(canvas);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_paint: finish");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_destroy
  (JNIEnv *, jobject, jlong nativeDevice)
{
    mapview_close();
}







//#if 0
RHO_GLOBAL void google_mapview_create(rho_param *p)
{
#ifdef RHO_GOOGLE_API_KEY
    JNIEnv *env = jnienv();
    jclass clsMapView = getJNIClass(RHODES_JAVA_CLASS_GOOGLEMAPVIEW);
    if (!clsMapView) return;
    jmethodID midCreate = getJNIClassStaticMethod(env, clsMapView, "create", "(Ljava/lang/String;Ljava/util/Map;)V");
    if (!midCreate) return;

    if (p->type != RHO_PARAM_HASH) {
        RAWLOG_ERROR("create: wrong input parameter (expect Hash)");
        return;
    }

    jhobject paramsObj = jhobject(RhoValueConverter(env).createObject(p));
    jhstring keyObj = rho_cast<jhstring>(RHO_GOOGLE_API_KEY);
    env->CallStaticVoidMethod(clsMapView, midCreate, keyObj.get(), paramsObj.get());
#else
    RAWLOG_ERROR("MapView disabled at build time");
#endif
}

RHO_GLOBAL void google_mapview_close()
{
#ifdef RHO_GOOGLE_API_KEY
    JNIEnv *env = jnienv();
    jclass clsMapView = getJNIClass(RHODES_JAVA_CLASS_GOOGLEMAPVIEW);
    if (!clsMapView) return;
    jmethodID midClose = getJNIClassStaticMethod(env, clsMapView, "close", "()V");
    if (!midClose) return;

    env->CallStaticVoidMethod(clsMapView, midClose);
#endif
}

RHO_GLOBAL VALUE google_mapview_state_started()
{
#ifdef RHO_GOOGLE_API_KEY
    VALUE nil = rho_ruby_get_NIL();
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GOOGLEMAPVIEW);
    if (!cls) return nil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "isStarted", "()Z");
    if (!mid) return nil;

    return rho_ruby_create_boolean(env->CallStaticBooleanMethod(cls, mid));
#else
    return rho_ruby_create_boolean(0);
#endif
}

RHO_GLOBAL double google_mapview_state_center_lat()
{
#ifdef RHO_GOOGLE_API_KEY
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GOOGLEMAPVIEW);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getCenterLatitude", "()D");
    if (!mid) return 0;

    return env->CallStaticDoubleMethod(cls, mid);
#else
    return 0;
#endif
}

RHO_GLOBAL double google_mapview_state_center_lon()
{
#ifdef RHO_GOOGLE_API_KEY
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_GOOGLEMAPVIEW);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getCenterLongitude", "()D");
    if (!mid) return 0;

    return env->CallStaticDoubleMethod(cls, mid);
#else
    return 0;
#endif
}
//#endif




static bool ourIsOldGoogleEngineUsed = false;



RHO_GLOBAL void mapview_close()
{
    if (ourIsOldGoogleEngineUsed) {
        google_mapview_close();
    }
    else {
		if (s_mapdevice)
		{
			rhomap::IMapView *mv = s_mapdevice->mapView();
			rho_map_destroy(mv);
		}
		delete s_mapdevice;
		s_mapdevice = NULL;

	JNIEnv *env = jnienv();
    	jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    	if (!cls) return;
    	jmethodID mid = getJNIClassStaticMethod(env, cls, "destroy", "()V");
    	if (!mid) return;
    	env->CallStaticVoidMethod(cls, mid);

	}
}

RHO_GLOBAL void mapview_create(rho_param *p)
{
    mapview_close();

    ourIsOldGoogleEngineUsed = false;

    // detect engine
    char* engine = "Google";
    if (p && p->type == RHO_PARAM_HASH) {
	rho_param *eng_p = NULL;
        for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
            char *name = p->v.hash->name[i];
            rho_param *value = p->v.hash->value[i];
            if (strcasecmp(name, "provider") == 0)
                eng_p = value;
        }
	if (eng_p && eng_p->type == RHO_PARAM_STRING) {
	   engine = eng_p->v.string;
	}
    }
    if (strcasecmp(engine, "Google") == 0) {
        ourIsOldGoogleEngineUsed = true;
    }

    if (ourIsOldGoogleEngineUsed) {
        google_mapview_create(p);
    }
    else {
        s_mapdevice = new rhomap::AndroidMapDevice(p);
    }
}

RHO_GLOBAL VALUE mapview_state_started()
{
    if (ourIsOldGoogleEngineUsed) {
        return google_mapview_state_started();
    }
    else {
	    return rho_ruby_create_boolean(!!s_mapdevice);
	}
}

RHO_GLOBAL double mapview_state_center_lat()
{
    if (ourIsOldGoogleEngineUsed) {
        return google_mapview_state_center_lat();
    }
    else {
		if (!s_mapdevice)
			return 0;
		return s_mapdevice->mapView()->latitude();
	}
}

RHO_GLOBAL double mapview_state_center_lon()
{
    if (ourIsOldGoogleEngineUsed) {
        return google_mapview_state_center_lon();
    }
    else {
		if (!s_mapdevice)
			return 0;
		return s_mapdevice->mapView()->longitude();
	}
}

