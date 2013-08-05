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

#define GMAP_FACADE_CLASS "com.rhomobile.rhodes.gmaps.GMapActivity"

RHO_GLOBAL void google_mapview_create(rho_param *p)
{
    JNIEnv *env = jnienv();
    static jclass clsMapView = rho_find_class(env, GMAP_FACADE_CLASS);
    if (!clsMapView) return;
    jmethodID midCreate = getJNIClassStaticMethod(env, clsMapView, "create", "(Ljava/util/Map;)V");
    if (!midCreate) return;

    if (p->type != RHO_PARAM_HASH) {
        RAWLOG_ERROR("create: wrong input parameter (expect Hash)");
        return;
    }

    jhobject paramsObj = RhoValueConverter(env).createObject(p);
    env->CallStaticVoidMethod(clsMapView, midCreate, paramsObj.get());
}

RHO_GLOBAL void google_mapview_close()
{
    JNIEnv *env = jnienv();
    static jclass clsMapView = rho_find_class(env, GMAP_FACADE_CLASS);
    if (!clsMapView) return;
    jmethodID midClose = getJNIClassStaticMethod(env, clsMapView, "close", "()V");
    if (!midClose) return;

    env->CallStaticVoidMethod(clsMapView, midClose);
}

RHO_GLOBAL VALUE google_mapview_state_started()
{
    VALUE nil = rho_ruby_get_NIL();
    JNIEnv *env = jnienv();
    static jclass cls = rho_find_class(env, GMAP_FACADE_CLASS);
    if (!cls) return nil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "isStarted", "()Z");
    if (!mid) return nil;

    return rho_ruby_create_boolean(env->CallStaticBooleanMethod(cls, mid));
}

RHO_GLOBAL double google_mapview_state_center_lat()
{
    JNIEnv *env = jnienv();
    static jclass cls = rho_find_class(env, GMAP_FACADE_CLASS);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getCenterLatitude", "()D");
    if (!mid) return 0;

    return env->CallStaticDoubleMethod(cls, mid);
}

RHO_GLOBAL double google_mapview_state_center_lon()
{
    JNIEnv *env = jnienv();
    static jclass cls = rho_find_class(env, GMAP_FACADE_CLASS);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getCenterLongitude", "()D");
    if (!mid) return 0;

    return env->CallStaticDoubleMethod(cls, mid);
}

RHO_GLOBAL void Init_GMaps()
{
}


