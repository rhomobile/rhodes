#include "rhodes/JNIRhodes.h"

#include <common/rhoparams.h>
#include <common/map/MapEngine.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MapView"

static rho::common::map::IMapView *s_mapview = NULL;

namespace rho
{
namespace common
{
namespace map
{

class AndroidDrawingImage : public IDrawingImage
{
public:
    int width() const;
    int height() const;
};

class AndroidDrawingDevice : public IDrawingDevice
{
public:
    IDrawingImage* createImage(String const &path);
    IDrawingImage* createImage(void const *p, size_t s);
    IDrawingImage* cloneImage(IDrawingImage *image);
    void destroyImage(IDrawingImage* image);

    void requestRedraw();
};

int AndroidDrawingImage::width() const
{
    // TODO:
    return 0;
}

int AndroidDrawingImage::height() const
{
    // TODO:
    return 0;
}

IDrawingImage *AndroidDrawingDevice::createImage(String const &path)
{
    // TODO:
    return 0;
}

IDrawingImage *AndroidDrawingDevice::createImage(void const *p, size_t s)
{
    // TODO:
    return 0;
}

IDrawingImage *AndroidDrawingDevice::cloneImage(IDrawingImage *image)
{
    // TODO:
    return 0;
}

void AndroidDrawingDevice::destroyImage(IDrawingImage *image)
{
    // TODO:
}

void AndroidDrawingDevice::requestRedraw()
{
    // TODO:
}

} // namespace map
} // namespace common
} // namespace rho

RHO_GLOBAL void mapview_create(rho_param *p)
{
    s_mapview = rho_map_create(p, new rho::common::map::AndroidDrawingDevice());
}

RHO_GLOBAL void mapview_close()
{
    if (s_mapview)
        RHOMAPPROVIDER().destroyMapView(s_mapview);
    s_mapview = NULL;
}

RHO_GLOBAL VALUE mapview_state_started()
{
    return rho_ruby_create_boolean(!!s_mapview);
}

RHO_GLOBAL double mapview_state_center_lat()
{
    if (!s_mapview)
        return 0;
    return s_mapview->latitude();
}

RHO_GLOBAL double mapview_state_center_lon()
{
    if (!s_mapview)
        return 0;
    return s_mapview->longitude();
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
