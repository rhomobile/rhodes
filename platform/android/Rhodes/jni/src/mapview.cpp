#include "rhodes/JNIRhodes.h"

#include <common/rhoparams.h>
#include <common/map/MapEngine.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "AndroidMapEngine"

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
    void drawText(int x, int y, String const &text, int color);

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

    void attach(jobject jDevice) {m_jdevice = jDevice;}

    rho_param *params() const {return m_params;}

    void setMapView(IMapView *mv) {m_mapview = mv;}
    IMapView *mapView() const {return m_mapview;}

    IDrawingImage* createImage(String const &path);
    IDrawingImage* createImage(void const *p, size_t s);
    IDrawingImage* cloneImage(IDrawingImage *image);
    void destroyImage(IDrawingImage* image);

    void requestRedraw();

    void paint(jobject canvas);

private:
    rho_param *m_params;
    IMapView *m_mapview;
    jobject m_jdevice;
};

AndroidImage::AndroidImage(jobject bitmap)
    :m_count(new int(1)), m_bitmap(new jobject(bitmap))
{
    JNIEnv *env = jnienv();
    env->NewGlobalRef(bitmap);
    env->DeleteLocalRef(bitmap);
    init(env);
}

AndroidImage::AndroidImage(int *count, jobject *bitmap)
    :m_count(count), m_bitmap(bitmap)
{
    ++*m_count;
    init(jnienv());
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
}

AndroidImage *AndroidImage::clone()
{
    return new AndroidImage(m_count, m_bitmap);
}

void AndroidDrawingContext::drawImage(int x, int y, IDrawingImage *image)
{
    if (!image)
        return;

    //RHO_MAP_TRACE3("drawImage: x=%d, y=%d, image=%p", x, y, image);

    jobject bitmap = ((AndroidImage*)image)->bitmap();

    JNIEnv *env = jnienv();
    jclass cls = env->GetObjectClass(m_device);
    if (!cls) return;
    jmethodID mid = getJNIClassMethod(env, cls, "drawImage", "(Landroid/graphics/Canvas;IILandroid/graphics/Bitmap;)V");
    if (!mid) return;

    env->CallVoidMethod(m_device, mid, m_canvas, x, y, bitmap);
}

void AndroidDrawingContext::drawText(int x, int y, String const &text, int color)
{
    //RHO_MAP_TRACE4("drawText: x=%d, y=%d, text=%s, color=%d", x, y, text.c_str(), color);

    JNIEnv *env = jnienv();
    jclass cls = env->GetObjectClass(m_device);
    if (!cls) return;
    jmethodID mid = getJNIClassMethod(env, cls, "drawText", "(Landroid/graphics/Canvas;IILjava/lang/String;I)V");
    if (!mid) return;

    env->CallVoidMethod(m_device, mid, m_canvas, x, y,
        rho_cast<jhstring>(text).get(), color);
}

AndroidMapDevice::AndroidMapDevice(rho_param *p)
    :m_params(rho_param_dup(p)), m_mapview(NULL), m_jdevice(NULL)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "create", "(J)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, (jlong)this);
}

AndroidMapDevice::~AndroidMapDevice()
{
    rho_param_free(m_params);
}

IDrawingImage *AndroidMapDevice::createImage(String const &path)
{
    //RHO_MAP_TRACE1("createImage: %s", path.c_str());

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "createImage", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
    if (!mid) return NULL;

    jobject bitmap = env->CallStaticObjectMethod(cls, mid, rho_cast<jhstring>(path).get());
    IDrawingImage *image = new AndroidImage(bitmap);

    //RHO_MAP_TRACE1("createImage: return image=%p", image);
    return image;
}

IDrawingImage *AndroidMapDevice::createImage(void const *p, size_t size)
{
    //RHO_MAP_TRACE2("createImage: p=%p, size=%llu", p, (unsigned long long)size);

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

    //RHO_MAP_TRACE1("createImage: return image=%p", image);
    return image;
}

IDrawingImage *AndroidMapDevice::cloneImage(IDrawingImage *image)
{
    //RHO_MAP_TRACE1("cloneImage: image=%p", image);
    IDrawingImage *cloned = image ? ((AndroidImage *)image)->clone() : NULL;
    //RHO_MAP_TRACE1("cloneImage: return image=%p", cloned);
    return cloned;
}

void AndroidMapDevice::destroyImage(IDrawingImage *image)
{
    //RHO_MAP_TRACE1("destroyImage: image=%p", image);
    delete image;
    //RHO_MAP_TRACE("destroyImage done");
}

void AndroidMapDevice::requestRedraw()
{
    //RHO_MAP_TRACE("requestRedraw");

    if (!m_jdevice)
        return;

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassMethod(env, cls, "redraw", "()V");
    if (!mid) return;
    env->CallVoidMethod(m_jdevice, mid);

    //RHO_MAP_TRACE("requestRedraw done");
}

void AndroidMapDevice::paint(jobject canvas)
{
    //RHO_MAP_TRACE("paint");

    if (!m_mapview)
        return;

    std::auto_ptr<AndroidDrawingContext> context(new AndroidDrawingContext(m_jdevice, canvas));
    m_mapview->paint(context.get());

    //RHO_MAP_TRACE("paint done");
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
  (JNIEnv *env, jobject jDevice, jlong nativeDevice, jint width, jint height)
{
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    rhomap::IMapView *mv = d->mapView();
    if (!mv) {
        mv = rho_map_create(d->params(), d, width, height);
        d->setMapView(mv);
        d->attach(jDevice);
    }
    if (mv)
        mv->setSize(width, height);
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_mapview_MapView_minZoom
  (JNIEnv *env, jobject, jlong nativeDevice)
{
    rhomap::IMapView *mv = mapview(env, nativeDevice);
    if (!mv)
        return 0;

    return mv->minZoom();
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_mapview_MapView_maxZoom
  (JNIEnv *env, jobject, jlong nativeDevice)
{
    rhomap::IMapView *mv = mapview(env, nativeDevice);
    if (!mv)
        return 0;

    return mv->maxZoom();
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_mapview_MapView_zoom
  (JNIEnv *env, jobject, jlong nativeDevice)
{
    rhomap::IMapView *mv = mapview(env, nativeDevice);
    if (!mv)
        return 0;

    return mv->zoom();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_setZoom
  (JNIEnv *env, jobject, jlong nativeDevice, jint zoom)
{
    rhomap::IMapView *mv = mapview(env, nativeDevice);
    if (!mv)
        return;

    mv->setZoom(zoom);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_move
  (JNIEnv *env, jobject, jlong nativeDevice, jint dx, jint dy)
{
    rhomap::IMapView *mv = mapview(env, nativeDevice);
    if (!mv)
        return;

    mv->move(dx, dy);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_paint
  (JNIEnv *env, jobject, jlong nativeDevice, jobject canvas)
{
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    if (!d)
        return;
    d->paint(canvas);
}

RHO_GLOBAL void mapview_close();

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_destroy
  (JNIEnv *, jobject, jlong nativeDevice)
{
    mapview_close();
}

RHO_GLOBAL void mapview_close()
{
    if (s_mapdevice)
    {
        rhomap::IMapView *mv = s_mapdevice->mapView();
        rho_map_destroy(mv);
    }
    delete s_mapdevice;
    s_mapdevice = NULL;
}

RHO_GLOBAL void mapview_create(rho_param *p)
{
    mapview_close();

    s_mapdevice = new rhomap::AndroidMapDevice(p);
}

RHO_GLOBAL VALUE mapview_state_started()
{
    return rho_ruby_create_boolean(!!s_mapdevice);
}

RHO_GLOBAL double mapview_state_center_lat()
{
    if (!s_mapdevice)
        return 0;
    return s_mapdevice->mapView()->latitude();
}

RHO_GLOBAL double mapview_state_center_lon()
{
    if (!s_mapdevice)
        return 0;
    return s_mapdevice->mapView()->longitude();
}

#if 0
RHO_GLOBAL void mapview_create(rho_param *p)
{
#ifdef RHO_GOOGLE_API_KEY
    JNIEnv *env = jnienv();
    jclass clsMapView = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
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

RHO_GLOBAL void mapview_close()
{
#ifdef RHO_GOOGLE_API_KEY
    JNIEnv *env = jnienv();
    jclass clsMapView = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!clsMapView) return;
    jmethodID midClose = getJNIClassStaticMethod(env, clsMapView, "close", "()V");
    if (!midClose) return;

    env->CallStaticVoidMethod(clsMapView, midClose);
#endif
}

RHO_GLOBAL VALUE mapview_state_started()
{
#ifdef RHO_GOOGLE_API_KEY
    VALUE nil = rho_ruby_get_NIL();
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return nil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "isStarted", "()Z");
    if (!mid) return nil;

    return rho_ruby_create_boolean(env->CallStaticBooleanMethod(cls, mid));
#else
    return rho_ruby_create_boolean(0);
#endif
}

RHO_GLOBAL double mapview_state_center_lat()
{
#ifdef RHO_GOOGLE_API_KEY
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getCenterLatitude", "()D");
    if (!mid) return 0;

    return env->CallStaticDoubleMethod(cls, mid);
#else
    return 0;
#endif
}

RHO_GLOBAL double mapview_state_center_lon()
{
#ifdef RHO_GOOGLE_API_KEY
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getCenterLongitude", "()D");
    if (!mid) return 0;

    return env->CallStaticDoubleMethod(cls, mid);
#else
    return 0;
#endif
}
#endif
