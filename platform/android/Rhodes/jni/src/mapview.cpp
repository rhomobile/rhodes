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

#include <genconfig.h>
#include <stdio.h>
#include "rhodes/JNIRhodes.h"

#include "rhodes/JNIRhoRuby.h"

#include <common/rhoparams.h>
#include <common/map/MapEngine.h>
#include <common/map/BaseMapEngine.h>

#include <android/log.h>

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

static bool ourIsCachingEnabled = false;

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

    AndroidImage(int count, int bitmap);

    void init(JNIEnv *env);

public:
    AndroidImage(int bitmap);
    ~AndroidImage();

    AndroidImage *clone();

    int bitmap() const {return m_bitmap;}

    int width() const {return m_width;}
    int height() const {return m_height;}

private:
    int m_bitmap;
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
    void setPinImage(JNIEnv *env, int bitmap);
    void setPinCalloutImage(JNIEnv *env, int bitmap);
    void setPinCalloutLinkImage(JNIEnv *env, int bitmap);
    void setESRILogoImage(JNIEnv *env, int bitmap);
    void setGoogleLogoImage(JNIEnv *env, int bitmap);
    void setPinMyLocationImage(JNIEnv *env, int bitmap);

    rho_param *params() const { return m_params; }

    void setMapView(IMapView *mv);
    IMapView *mapView() const {return m_mapview;}

    IDrawingImage* createImage(String const &path, bool useAlpha);
    IDrawingImage* createImage(void const *p, size_t s, bool useAlpha);
    IDrawingImage* createImageEx(void const *p, size_t s, int x, int y, int w, int h, bool useAlpha);

    IDrawingImage* cloneImage(IDrawingImage *image);
    void destroyImage(IDrawingImage* image);

    IDrawingImage* createCalloutImage(String const &title, String const &subtitle, String const& url, int* x_offset, int* y_offset);
    
    
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
    std::auto_ptr<IDrawingImage> m_googleLogo_image;
    std::auto_ptr<IDrawingImage> m_pin_myLocation_image;
};

AndroidImage::AndroidImage(int jbitmap)
{
    RHO_MAP_TRACE("AndroidImage: ctor start");
    m_bitmap = jbitmap;
    JNIEnv *env = jnienv();
    //*m_bitmap = env->NewGlobalRef(jbitmap);
    //env->DeleteLocalRef(jbitmap);
    init(env);
    RHO_MAP_TRACE("AndroidImage: ctor finish");
}

AndroidImage::AndroidImage(int count, int pjbitmap)
{
    RHO_MAP_TRACE("AndroidImage: private ctor start");

    m_bitmap = pjbitmap;

    JNIEnv *env = jnienv();
    jclass& cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (cls) {
    	jmethodID midAddRef = getJNIClassStaticMethod(env, cls, "addRefImage", "(I)V");
    	if (midAddRef) {
    		env->CallStaticVoidMethod(cls, midAddRef, m_bitmap);
    	}
    }

    init(jnienv());
    RHO_MAP_TRACE("AndroidImage: private ctor finish");
}

void AndroidImage::init(JNIEnv *env)
{
    jclass& cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return;
    jmethodID midWidth = getJNIClassStaticMethod(env, cls, "getBitmapWidth", "(I)I");
    if (!midWidth) return;
    jmethodID midHeight = getJNIClassStaticMethod(env, cls, "getBitmapHeight", "(I)I");
    if (!midHeight) return;

    m_width = env->CallStaticIntMethod(cls, midWidth, m_bitmap);
    m_height = env->CallStaticIntMethod(cls, midHeight, m_bitmap);
    
}

AndroidImage::~AndroidImage()
{
    RHO_MAP_TRACE("AndroidImage: dtorimg start");
    {
        JNIEnv *env = jnienv();
        jclass& cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
        jmethodID mid = getJNIClassStaticMethod(env, cls, "destroyImage", "(I)V");

        if (cls && mid)
            env->CallStaticVoidMethod(cls, mid, m_bitmap);

    }
    RHO_MAP_TRACE("AndroidImage: dtorimg finish");
}

AndroidImage *AndroidImage::clone()
{
    return new AndroidImage(0, m_bitmap);
}

void AndroidDrawingContext::drawImage(int x, int y, IDrawingImage *image)
{
    if (!image)
        return;

    int bitmap = ((AndroidImage*)image)->bitmap();

    RHO_MAP_TRACE4("drawImage: x=%d, y=%d, image=%p id=%d", x, y, image, bitmap);


    JNIEnv *env = jnienv();
    jclass cls = env->GetObjectClass(m_device);
    if (!cls) return;
    jmethodID mid = getJNIClassMethod(env, cls, "drawImage", "(Landroid/graphics/Canvas;III)V");
    if (!mid) return;

    env->CallVoidMethod(m_device, mid, m_canvas, x, y, bitmap);

    /////
    env->DeleteLocalRef(cls);
    
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

    jhstring jhText = rho_cast<jstring>(env, text);
    env->CallVoidMethod(m_device, mid, m_canvas, x, y, width, height,
        jhText.get(), color);

    /////
    env->DeleteLocalRef(cls);
    
    RHO_MAP_TRACE("drawText done");
}

AndroidMapDevice::AndroidMapDevice(rho_param *p)
    :m_params(rho_param_dup(p)), m_mapview(NULL), m_jdevice(NULL)
{
    RHO_MAP_TRACE("AndroidMapDevice: ctor start");
    JNIEnv *env = jnienv();
    jclass& cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "create", "(J)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, (jlong)this);
    
    RHO_MAP_TRACE("AndroidMapDevice: ctor finish");
}

AndroidMapDevice::~AndroidMapDevice()
{
    RHO_MAP_TRACE("AndroidMapDevice: dtordev start");
    rho_param_free(m_params);
    JNIEnv *env = jnienv();
    if (m_jdevice) {
        jclass& cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
        if (!cls) return;
        jmethodID mid = getJNIClassMethod(env, cls, "destroyDevice", "()V");
        if (!mid) return;
        env->CallVoidMethod(m_jdevice, mid);

        env->DeleteGlobalRef(m_jdevice);
    }
    RHO_MAP_TRACE("AndroidMapDevice: dtordev finish");
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

        PIN_INFO pin_info1 = PIN_INFO();
        pin_info1.x_offset = 5;

        mv->setPinCalloutImage(m_pin_calloutimage.get(), pin_info1);
        mv->setPinCalloutLinkImage(m_pin_calloutlinkimage.get(), pin_info1);
        mv->setESRILogoImage(m_esriLogo_image.get());
        mv->setGoogleLogoImage(m_googleLogo_image.get());

        
        
        pin_info.x_offset = -2;
        pin_info.y_offset = -31;
        pin_info.click_rect_x = -2;
        pin_info.click_rect_y = -31;
        pin_info.click_rect_width = 32;
        pin_info.click_rect_height = 32;
        
        mv->setPinMyLocationImage(m_pin_myLocation_image.get(), pin_info);
        
        mv->set_file_caching_enable((int)ourIsCachingEnabled);

    }
    RHO_MAP_TRACE("AndroidMapDevice: setMapView: finish");
}

void AndroidMapDevice::setPinImage(JNIEnv *env, int bitmap)
{
    RHO_MAP_TRACE("AndroidMapDevice: setPinImage: start");
    m_pin_image.reset(new AndroidImage(bitmap));
    IMapView *mv = mapView();
    if (mv)
    {
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

void AndroidMapDevice::setPinCalloutImage(JNIEnv *env, int bitmap)
{
    RHO_MAP_TRACE("AndroidMapDevice: setPinCalloutImage: start");
    m_pin_calloutimage.reset(new AndroidImage(bitmap));
    IMapView *mv = mapView();
    if (mv){
        PIN_INFO pin_info = PIN_INFO();
        pin_info.x_offset = 5;

        mv->setPinCalloutImage(m_pin_calloutimage.get(), pin_info);
    }
    RHO_MAP_TRACE("AndroidMapDevice: setPinCalloutImage: finish");
}

void AndroidMapDevice::setPinCalloutLinkImage(JNIEnv *env, int bitmap)
{
    RHO_MAP_TRACE("AndroidMapDevice: setPinCalloutLinkImage: start");
    m_pin_calloutlinkimage.reset(new AndroidImage(bitmap));
    IMapView *mv = mapView();
    if (mv) {
        PIN_INFO pin_info = PIN_INFO();
        pin_info.x_offset = 5;

        mv->setPinCalloutLinkImage(m_pin_calloutlinkimage.get(), pin_info);
    }
    RHO_MAP_TRACE("AndroidMapDevice: setPinCalloutLinkImage: finish");
}

void AndroidMapDevice::setESRILogoImage(JNIEnv *env, int bitmap) {
    RHO_MAP_TRACE("AndroidMapDevice: setESRILogoImage: start");
    m_esriLogo_image.reset(new AndroidImage(bitmap));
    IMapView *mv = mapView();
    if (mv) {
        mv->setESRILogoImage(m_esriLogo_image.get());
    }
    RHO_MAP_TRACE("AndroidMapDevice: setESRILogoImage: finish");
}

void AndroidMapDevice::setGoogleLogoImage(JNIEnv *env, int bitmap) {
        RHO_MAP_TRACE("AndroidMapDevice: setGoogleLogoImage: start");
        m_googleLogo_image.reset(new AndroidImage(bitmap));
        IMapView *mv = mapView();
        if (mv) {
            mv->setGoogleLogoImage(m_googleLogo_image.get());
        }
        RHO_MAP_TRACE("AndroidMapDevice: setGoogleLogoImage: finish");
}

void AndroidMapDevice::setPinMyLocationImage(JNIEnv *env, int bitmap) {
        RHO_MAP_TRACE("AndroidMapDevice: setPinMyLocationImage: start");
        m_pin_myLocation_image.reset(new AndroidImage(bitmap));
        IMapView *mv = mapView();
        if (mv) {
            
            PIN_INFO pin_info;

            pin_info.x_offset = -2;
            pin_info.y_offset = -31;
            pin_info.click_rect_x = -2;
            pin_info.click_rect_y = -31;
            pin_info.click_rect_width = 32;
            pin_info.click_rect_height = 32;
            
            
            mv->setPinMyLocationImage(m_pin_myLocation_image.get(), pin_info);
        }
        RHO_MAP_TRACE("AndroidMapDevice: setGoogleLogoImage: finish");
}


IDrawingImage *AndroidMapDevice::createImage(String const &path, bool useAlpha)
{
    RHO_MAP_TRACE1("createImage: %s", path.c_str());

    JNIEnv *env = jnienv();
    jclass& cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "createImage", "(Ljava/lang/String;)I");
    if (!mid) return NULL;

    jhstring jhPath = rho_cast<jstring>(env, path);
    int bitmap = env->CallStaticIntMethod(cls, mid, jhPath.get());
    if (bitmap == 0) {
        return NULL;
    }
    IDrawingImage *image = new AndroidImage(bitmap);
    
    RHO_MAP_TRACE1("createImage: return image=%p", image);
    return image;
}

IDrawingImage *AndroidMapDevice::createImage(void const *p, size_t size, bool useAlpha)
{
    RHO_MAP_TRACE2("createImage: p=%p, size=%llu", p, (unsigned long long)size);

    JNIEnv *env = jnienv();
    jclass& cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "createImage", "([B)I");
    if (!mid) return NULL;

    jholder<jbyteArray> data = env->NewByteArray(size);
    if (!data) return NULL;
    env->SetByteArrayRegion(data.get(), 0, size, (jbyte const *)p);

    int bitmap = env->CallStaticIntMethod(cls, mid, data.get());
    IDrawingImage *image = new AndroidImage(bitmap);
    
    RHO_MAP_TRACE1("createImage: return image=%p", image);
    return image;
}
    
IDrawingImage* AndroidMapDevice::createCalloutImage(String const &title, String const &subtitle, String const& url, int* x_offset, int* y_offset)
{
    JNIEnv *env = jnienv();
    jclass cls = env->FindClass("com/rhomobile/rhodes/mapview/Callout");//getJNIClass("com/rhomobile/rhodes/mapview/Callout");
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "makeCallout", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Lcom/rhomobile/rhodes/mapview/Callout;");
    if (!mid) return NULL;
    
    jhstring jhTitle = rho_cast<jstring>(env, title);
    jhstring jhSubTitle = rho_cast<jstring>(env, subtitle);
    jhstring jhUrl = rho_cast<jstring>(env, url);
    jhobject jo_callout = env->CallStaticObjectMethod(cls, mid, jhTitle.get(), jhSubTitle.get(), jhUrl.get());
    
    jmethodID mid_bitmap = env->GetMethodID( cls, "getResultBitmapID", "()I");
    jmethodID mid_x_off = env->GetMethodID( cls, "getXOffset", "()I");
    jmethodID mid_y_off = env->GetMethodID( cls, "getYOffset", "()I");
    
    int bitmap = env->CallIntMethod(jo_callout.get(), mid_bitmap);
    IDrawingImage *image = new AndroidImage(bitmap);
    
    *x_offset = env->CallIntMethod(jo_callout.get(), mid_x_off);
    *y_offset = env->CallIntMethod(jo_callout.get(), mid_y_off);
    
    return image;
}
    

IDrawingImage *AndroidMapDevice::createImageEx(void const *p, size_t size, int x, int y, int w, int h, bool useAlpha) {
    RHO_MAP_TRACE2("createImageEx: p=%p, size=%llu", p, (unsigned long long)size);

    JNIEnv *env = jnienv();
    jclass& cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "createImageEx", "([BIIII)I");
    if (!mid) return NULL;

    jholder<jbyteArray> data = jholder<jbyteArray>(env->NewByteArray(size));
    if (!data) return NULL;
    env->SetByteArrayRegion(data.get(), 0, size, (jbyte const *)p);

    int bitmap = env->CallStaticIntMethod(cls, mid, data.get(), x, y, w, h);
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
        jclass& cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
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
        //TODO: investigate when this happens !
        //env->ThrowNew(getJNIClass(RHODES_JAVA_CLASS_RUNTIME_EXCEPTION), "Specified wrong native device");
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
        //TODO: investigate when this happens !
        //env->ThrowNew(getJNIClass(RHODES_JAVA_CLASS_RUNTIME_EXCEPTION), "Specified native device does not have mapview");
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
  (JNIEnv *env, jobject, jlong nativeDevice, int bitmap)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setPinImage: start");
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    d->setPinImage(env, bitmap);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setPinImage: finish");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_setPinCalloutImage
  (JNIEnv *env, jobject, jlong nativeDevice, int bitmap)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setPinCalloutImage: start");
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    d->setPinCalloutImage(env, bitmap);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setPinCalloutImage: finish");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_setPinCalloutLinkImage
  (JNIEnv *env, jobject, jlong nativeDevice, int bitmap)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setPinCalloutLinkImage: start");
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    d->setPinCalloutLinkImage(env, bitmap);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setPinCalloutLinkImage: finish");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_setESRILogoImage
  (JNIEnv *env, jobject, jlong nativeDevice, int bitmap)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setESRILogoImage: start");
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    d->setESRILogoImage(env, bitmap);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setESRILogoImage: finish");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_setGoogleLogoImage
(JNIEnv *env, jobject, jlong nativeDevice, int bitmap)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setGoogleLogoImage: start");
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    d->setGoogleLogoImage(env, bitmap);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setGoogleLogoImage: finish");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_setMyLocationImage
(JNIEnv *env, jobject, jlong nativeDevice, int bitmap)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setMyLocationImage: start");
    rhomap::AndroidMapDevice *d = device(env, nativeDevice);
    d->setPinMyLocationImage(env, bitmap);
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_setMyLocationImage: finish");
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



RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_mapview_MapView_click
  (JNIEnv *env, jobject, jlong nativeDevice, jint x, jint y)
{
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_click: start");
    rhomap::IMapView *mv = mapview(env, nativeDevice);
    if (mv) {
        if (mv->handleClick(x, y) ) {
            // close
            mapview_close();
            return 1;
        }
    }
    RHO_MAP_TRACE("Java_com_rhomobile_rhodes_mapview_MapView_click: finish");
    return 0;
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




//#endif


#ifdef RHO_GOOGLE_API_KEY
RHO_GLOBAL void google_mapview_create(rho_param *p);
RHO_GLOBAL void google_mapview_close();
RHO_GLOBAL VALUE google_mapview_state_started();
RHO_GLOBAL double google_mapview_state_center_lat();
RHO_GLOBAL double google_mapview_state_center_lon();
#endif

static bool ourIsOldGoogleEngineUsed = true;



RHO_GLOBAL void mapview_close()
{
#ifdef RHO_GOOGLE_API_KEY
    if (ourIsOldGoogleEngineUsed) {
        google_mapview_close();
    }
    else
#endif
    {
		if (s_mapdevice)
		{
			rhomap::IMapView *mv = s_mapdevice->mapView();
			rho_map_destroy(mv);
		}
		delete s_mapdevice;
		s_mapdevice = NULL;

	JNIEnv *env = jnienv();
    	jclass& cls = getJNIClass(RHODES_JAVA_CLASS_MAPVIEW);
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
    const char* engine = "Google";
    if (p && p->type == RHO_PARAM_HASH) {
	rho_param *eng_p = NULL;
        for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
            char *name = p->v.hash->name[i];
            rho_param *value = p->v.hash->value[i];
            if (strcasecmp(name, "provider") == 0)
                eng_p = value;
        }
	if (eng_p && eng_p->type == RHO_PARAM_STRING) {
           if ((eng_p->v.string != NULL) && (eng_p->v.string[0] != 0)) {
	      engine = eng_p->v.string;
           }
	}
    }
    if (strcasecmp(engine, "Google") == 0) {
        ourIsOldGoogleEngineUsed = true;
    }

#ifdef RHO_GOOGLE_API_KEY
    if (ourIsOldGoogleEngineUsed) {
        google_mapview_create(p);
    }
    else
#endif
    {
		if (rho_map_check_param(p)) {
			s_mapdevice = new rhomap::AndroidMapDevice(p);
		}
		else {
		        RAWLOG_ERROR("MapView.create: wrong input parameters ! (parameters not validated !)");
		}
    }
}

RHO_GLOBAL VALUE mapview_state_started()
{
#ifdef RHO_GOOGLE_API_KEY
    if (ourIsOldGoogleEngineUsed) {
        return google_mapview_state_started();
    }
    else
#endif
    {
        return rho_ruby_create_boolean(!!s_mapdevice);
    }
}

RHO_GLOBAL double mapview_state_center_lat()
{
#ifdef RHO_GOOGLE_API_KEY
    if (ourIsOldGoogleEngineUsed) {
        return google_mapview_state_center_lat();
    }
    else
#endif
    {
		if (!s_mapdevice)
			return 0;
		return s_mapdevice->mapView()->latitude();
	}
}

RHO_GLOBAL double mapview_state_center_lon()
{
#ifdef RHO_GOOGLE_API_KEY
    if (ourIsOldGoogleEngineUsed) {
        return google_mapview_state_center_lon();
    }
    else
#endif
    {
		if (!s_mapdevice)
			return 0;
		return s_mapdevice->mapView()->longitude();
	}
}

RHO_GLOBAL void mapview_set_file_caching_enable(int enable) {
    ourIsCachingEnabled = (enable != 0);
    if (!s_mapdevice)
        return;
    return s_mapdevice->mapView()->set_file_caching_enable(enable);
}



RHO_GLOBAL int  mapengine_request_make() {
	JNIEnv *env = jnienv();
	jclass cls = rho_find_class(env, "com/rhomobile/rhodes/mapview/MapView");//env->FindClass("com/rhomobile/rhodes/mapview/MapView");//rho_find_class(env, "com/rhomobile/rhodes/mapview/MapView");
	if (!cls) {
		RAWLOG_ERROR("mapengine_request : Can not found com/rhomobile/rhodes/mapview/MapView class !!!");
		return 0;
	}
	jmethodID mid = env->GetStaticMethodID( cls, "mapengine_request_make", "()I");
	if (!mid) {
		env->DeleteLocalRef(cls);
		return 0;
	}
	int res = env->CallStaticIntMethod(cls, mid);
	env->DeleteLocalRef(cls);
	return res;
}


RHO_GLOBAL int  mapengine_request_data(int request_id, const char* url, void** data, int* datasize) {
	JNIEnv *env = jnienv();
	jclass cls = rho_find_class(env, "com/rhomobile/rhodes/mapview/MapView");//env->FindClass("com/rhomobile/rhodes/mapview/MapView");//rho_find_class(env, "com/rhomobile/rhodes/mapview/MapView");
	if (!cls) {
		RAWLOG_ERROR("mapengine_request : Can not found com/rhomobile/rhodes/mapview/MapView class !!!");
		return 0;
	}
	jmethodID mid = env->GetStaticMethodID( cls, "mapengine_request_data", "(ILjava/lang/String;)I");
	if (!mid) {
		env->DeleteLocalRef(cls);
		return 0;
	}

    jhstring jhUrl = rho_cast<jstring>(env, url);
    int size = env->CallStaticIntMethod(cls, mid, request_id, jhUrl.get());

    if (size == 0) {
    	env->DeleteLocalRef(cls);
    	return 0;
    }

	jmethodID mid2 = env->GetStaticMethodID( cls, "mapengine_get_requested_data", "(I)[B");
	if (!mid2) {
		env->DeleteLocalRef(cls);
		return 0;
	}

	jholder<jbyteArray> jbytear = static_cast<jbyteArray>(env->CallStaticObjectMethod(cls, mid2, request_id));

	if (!jbytear) {
		env->DeleteLocalRef(cls);
		RAWLOG_ERROR("mapengine_request : byte array is NULL");
		return 0;
	}

	jbyte* buf_p = env->GetByteArrayElements(jbytear.get(), 0);

	if (buf_p == NULL) {
		env->DeleteLocalRef(cls);
		RAWLOG_ERROR("mapengine_request : buffer from byte array is NULL");
		return 0;
	}
	*data = malloc(size);
	if (*data == NULL) {
		env->DeleteLocalRef(cls);
		RAWLOG_ERROR("mapengine_request : cannot allocate data");
		return 0;
	}
	*datasize = size;
	memcpy(*data, buf_p, size);
	env->ReleaseByteArrayElements(jbytear.get(), buf_p, 0);
	env->DeleteLocalRef(cls);
	//RAWLOG_ERROR(ANDROID_LOG_INFO, "mapengine_request", "finish OK");
	return size;
}

RHO_GLOBAL void mapengine_request_cancel(int request_id) {
	JNIEnv *env = jnienv();
	jclass cls = rho_find_class(env, "com/rhomobile/rhodes/mapview/MapView");//env->FindClass("com/rhomobile/rhodes/mapview/MapView");//rho_find_class(env, "com/rhomobile/rhodes/mapview/MapView");
	if (!cls) {
		RAWLOG_ERROR("mapengine_request : Can not found com/rhomobile/rhodes/mapview/MapView class !!!");
		return;
	}
	jmethodID mid = env->GetStaticMethodID( cls, "mapengine_request_cancel", "(I)V");
	if (!mid) {
		env->DeleteLocalRef(cls);
		return;
	}
	env->CallStaticVoidMethod(cls, mid, request_id);
	env->DeleteLocalRef(cls);
}



RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_mapview_MapView_deletemapviewpointer
  (JNIEnv *, jclass, jint p)
{
	rho::common::map::BaseMapView* mvp = (rho::common::map::BaseMapView*)p;
	RAWLOG_INFO("delete_mapview_pointer : ^^^^^^^^^^^^^^^^ BEFORE");
	if (mvp != NULL) {
		mvp->cleanTiles();
		RAWLOG_INFO("delete_mapview_pointer : ^^^^^^^^^^^^^^^^ _____ 1 _____");
		delete mvp;
	}
	RAWLOG_INFO("delete_mapview_pointer : ^^^^^^^^^^^^^^^^ AFTER");
}

void mapengine_delete_mapview_in_ui_thread(void* p) {
	int pl = (int)p;

	JNIEnv *env = jnienv();
	jclass cls = rho_find_class(env, "com/rhomobile/rhodes/mapview/MapView");//env->FindClass("com/rhomobile/rhodes/mapview/MapView");//rho_find_class(env, "com/rhomobile/rhodes/mapview/MapView");
	if (!cls) {
		RAWLOG_ERROR("mapengine_request : Can not found com/rhomobile/rhodes/mapview/MapView class !!!");
		return;
	}
	jmethodID mid = env->GetStaticMethodID( cls, "mapengine_delete_mapview_in_ui_thread", "(I)V");
	if (!mid) {
		env->DeleteLocalRef(cls);
		return;
	}
	env->CallStaticVoidMethod(cls, mid, pl);
	env->DeleteLocalRef(cls);


}



