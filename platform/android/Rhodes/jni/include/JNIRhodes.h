#ifndef RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee
#define RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee

#include <assert.h>
#include <stdlib.h>

#include <ruby.h>
#include <ruby/ext/rho/rhoruby.h>

#include <jni.h>

#include <common/RhoDefs.h>
#include <logging/RhoLogConf.h>
#include <logging/RhoLog.h>

JavaVM *jvm();
void store_thr_jnienv(JNIEnv *env);
JNIEnv *jnienv();
jclass getJNIClass(const char *name);
jclass getJNIObjectClass(jobject obj);
jfieldID getJNIClassField(jclass cls, const char *name, const char *signature);
jfieldID getJNIClassStaticField(jclass cls, const char *name, const char *signature);
jmethodID getJNIClassMethod(jclass cls, const char *name, const char *signature);
jmethodID getJNIClassStaticMethod(jclass cls, const char *name, const char *signature);

VALUE convertJavaMapToRubyHash(jobject objMap);

#define RHODES_JAVA_CLASS_ANDROID_LOG "android/util/Log"
#define RHODES_JAVA_CLASS_ITERATOR "java/util/Iterator"
#define RHODES_JAVA_CLASS_SET "java/util/Set"
#define RHODES_JAVA_CLASS_MAP "java/util/Map"
#define RHODES_JAVA_CLASS_RHODES "com/rhomobile/rhodes/Rhodes"
#define RHODES_JAVA_CLASS_WEB_VIEW "com/rhomobile/rhodes/WebView"
#define RHODES_JAVA_CLASS_GEO_LOCATION "com/rhomobile/rhodes/geolocation/GeoLocation"
#define RHODES_JAVA_CLASS_CAMERA "com/rhomobile/rhodes/camera/Camera"
#define RHODES_JAVA_CLASS_DATE_TIME_PICKER "com/rhomobile/rhodes/datetime/DateTimePicker"
#define RHODES_JAVA_CLASS_PHONEBOOK "com/rhomobile/rhodes/phonebook/Phonebook"
#define RHODES_JAVA_CLASS_CONTACT "com/rhomobile/rhodes/phonebook/Contact"
#define RHODES_JAVA_CLASS_CONTACT_FIELD "com/rhomobile/rhodes/phonebook/ContactField"
#define RHODES_JAVA_CLASS_ALERT "com/rhomobile/rhodes/alert/Alert"
#define RHODES_JAVA_CLASS_RINGTONE_MANAGER "com/rhomobile/rhodes/RingtoneManager"

#define RHO_NOT_IMPLEMENTED RAWLOG_ERROR3("WARNING: Call not implemented function: \"%s\" (defined here: %s:%d)", __PRETTY_FUNCTION__, __FILE__, __LINE__)

//#define RHO_LOG_CALLBACK RAWLOG_INFO1("Callback \"%s\" called", __PRETTY_FUNCTION__)
//#define RHO_LOG_JNI_CALL RAWLOG_INFO1("JNI method \"%s\" called", __PRETTY_FUNCTION__)

#endif // RHO_JNI_RHODES_57d3a700b706402190ead97fd1383bee

