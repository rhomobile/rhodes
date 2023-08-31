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

#ifndef RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee
#define RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee

#include <assert.h>
#include <stdlib.h>

#include <rhodes.h>

#include <common/RhoDefs.h>
#include <common/RhoStd.h>
#include <common/RhodesApp.h>
#include <logging/RhoLogConf.h>
#include <logging/RhoLog.h>

#ifndef RHO_NO_RUBY
//This include is required for rhojava.inc below
#include <genconfig.h>
#endif

static int const RHO_FD_BASE = 512;

JavaVM *jvm();
void store_thr_jnienv(JNIEnv *env);
void initjnienv(JNIEnv* env);

extern "C" {
  void android_set_path(const rho::String& root, const rho::String& sqlite, const rho::String& shared);
  void android_set_log_path(const rho::String& path);
  void android_setup(JNIEnv *env);
}

enum rho_java_class_t {
#define RHODES_DEFINE_JAVA_CLASS(x, name) x,
#include "rhojava.inc"
#include "rhojava_extra.inc"
#undef RHODES_DEFINE_JAVA_CLASS
};

extern const char *rho_java_class[];

jclass& getJNIClass(int n);
jclass getJNIObjectClass(JNIEnv *env, jobject obj);
jfieldID getJNIClassField(JNIEnv *env, jclass cls, const char *name, const char *signature);
jfieldID getJNIClassStaticField(JNIEnv *env, jclass cls, const char *name, const char *signature);
jmethodID getJNIClassMethod(JNIEnv *env, jclass cls, const char *name, const char *signature);
jmethodID getJNIClassStaticMethod(JNIEnv *env, jclass cls, const char *name, const char *signature);

#define RHO_NOT_IMPLEMENTED RAWLOG_ERROR3("WARNING: Call not implemented function: \"%s\" (defined here: %s:%d)", __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define RHO_LOG_CALLBACK RAWLOG_INFO1("Callback \"%s\" called", __PRETTY_FUNCTION__)
#define RHO_LOG_JNI_CALL RAWLOG_INFO1("JNI method \"%s\" called", __PRETTY_FUNCTION__)

#define JNI_EXCEPTION_CHECK(env, result) if(env->ExceptionCheck()) { \
    jholder<jthrowable> jhexc = env->ExceptionOccurred(); \
    env->ExceptionClear(); \
    jholder<jclass> jhclass = env->GetObjectClass(jhexc.get()); \
    jmethodID mid = env->GetMethodID(jhclass.get(), "toString", "()Ljava/lang/String;"); \
    jhstring jhmsg = (jstring)env->CallObjectMethod(jhexc.get(), mid); \
    rho::String error = rho_cast<rho::String>(env, jhmsg); \
    RAWLOG_ERROR(error.c_str()); \
    result.setError(error); \
}

#define JNI_EXCEPTION_CHECK_AND_RETURN(env, result, value) if(env->ExceptionCheck()) { \
    jholder<jthrowable> jhexc = env->ExceptionOccurred(); \
    env->ExceptionClear(); \
    jholder<jclass> jhclass = env->GetObjectClass(jhexc.get()); \
    jmethodID mid = env->GetMethodID(jhclass.get(), "toString", "()Ljava/lang/String;"); \
    jhstring jhmsg = (jstring)env->CallObjectMethod(jhexc.get(), mid); \
    rho::String error = rho_cast<rho::String>(env, jhmsg); \
    RAWLOG_ERROR(error.c_str()); \
    result.setError(error); \
    return value;\
}


namespace rho
{
namespace common
{

class AndroidLogSink : public ILogSink
{
public:
    void writeLogMessage(String &strMsg);

    int getCurPos()
    {
        return 0;
    }

    void clear() {}
};

class AndroidMemoryInfoCollector : public IMemoryInfoCollector
{
public:
    virtual String collect();
};

class AndroidNetworkStatusMonitor : public INetworkStatusMonitor
{
private:
    INetworkStatusReceiver* m_pNetworkStatusReceiver;
public:
    AndroidNetworkStatusMonitor() : m_pNetworkStatusReceiver(0) {}
    virtual ~AndroidNetworkStatusMonitor() {}
    virtual void setPollInterval(int interval) {}
    virtual void setNetworkStatusReceiver(INetworkStatusReceiver* receiver) { m_pNetworkStatusReceiver = receiver; }

    void notifyReceiver(enNetworkStatus status)
    {
		RAWLOG_ERROR("enter notifyReceiver");
        if ( m_pNetworkStatusReceiver != 0 )
        {
			RAWLOG_ERROR("calling onNetworkStatusChanged");
            m_pNetworkStatusReceiver->onNetworkStatusChanged(status);
        }
    }
};

String clearException(JNIEnv* env);

} // namespace common
} // namespace rho


#endif // RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee
