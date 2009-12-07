#include <com_rhomobile_rhodes_Rhodes.h>

#include <common/RhoConf.h>
#include <logging/RhoLogConf.h>
#include <common/RhodesApp.h>
#include <sync/SyncThread.h>

#include <sys/stat.h>

#include "JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Rhodes"

static rho::String g_rootPath;

static pthread_key_t g_thrkey;

static JavaVM *g_jvm = NULL;
JavaVM *jvm()
{
    return g_jvm;
}

namespace rho
{
namespace common
{

jclass clsAndroidLog = NULL;
jmethodID midAndroidLogI = NULL;
jstring tagAndroidLog;

class AndroidLogSink : public ILogSink
{
public:
    void writeLogMessage(String &strMsg)
    {
        JNIEnv *env = jnienv();
        if (!env) return;
        jstring strObj = env->NewStringUTF(strMsg.c_str());
        env->CallStaticIntMethod(clsAndroidLog, midAndroidLogI,
            tagAndroidLog, strObj);
        env->DeleteLocalRef(strObj);
    }

    int getCurPos()
    {
        // TODO
        return 0;
    }

    void clear()
    {
        // TODO
    }
};

static CAutoPtr<AndroidLogSink> g_androidLogSink(new AndroidLogSink());

} // namespace common
} // namespace rho

void store_thr_jnienv(JNIEnv *env)
{
    pthread_setspecific(g_thrkey, env);
}

JNIEnv *jnienv()
{
    JNIEnv *env = (JNIEnv *)pthread_getspecific(g_thrkey);
    return env;
}

std::map<std::string, jclass> g_classes;

jclass getJNIClass(const char *name)
{
    std::map<std::string, jclass>::const_iterator it = g_classes.find(name);
    if (it == g_classes.end())
    {
        RAWLOG_ERROR1("Can not find Java class: %s (JNI)", name);
        return NULL;
    }
    return it->second;
}

jclass getJNIObjectClass(jobject obj)
{
    JNIEnv *env = jnienv();
    jclass cls = env->GetObjectClass(obj);
    if (!cls)
        RAWLOG_ERROR1("Can not get class for object: %p (JNI)", obj);
    return cls;
}

jfieldID getJNIClassField(jclass cls, const char *name, const char *signature)
{
    JNIEnv *env = jnienv();
    jfieldID fid = env->GetFieldID(cls, name, signature);
    if (!fid)
        RAWLOG_ERROR3("Can not get field %s of signature %s for class %p", name, signature, cls);
    return fid;
}

jfieldID getJNIClassStaticField(jclass cls, const char *name, const char *signature)
{
    JNIEnv *env = jnienv();
    jfieldID fid = env->GetStaticFieldID(cls, name, signature);
    if (!fid)
        RAWLOG_ERROR3("Can not get static field %s of signature %s for class %p", name, signature, cls);
    return fid;
}

jmethodID getJNIClassMethod(jclass cls, const char *name, const char *signature)
{
    JNIEnv *env = jnienv();
    jmethodID mid = env->GetMethodID(cls, name, signature);
    if (!mid)
        RAWLOG_ERROR3("Can not get method %s of signature %s for class %p", name, signature, cls);
    return mid;
}

jmethodID getJNIClassStaticMethod(jclass cls, const char *name, const char *signature)
{
    JNIEnv *env = jnienv();
    jmethodID mid = env->GetStaticMethodID(cls, name, signature);
    if (!mid)
        RAWLOG_ERROR3("Can not get static method %s of signature %s for class %p", name, signature, cls);
    return mid;
}

const char* rho_native_rhopath()
{
    return g_rootPath.c_str();
}

jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    g_jvm = vm;
    jint jversion = JNI_VERSION_1_4;
    JNIEnv *env;
    if (vm->GetEnv((void**)&env, jversion) != JNI_OK)
        return -1;

    pthread_key_create(&g_thrkey, NULL);
    store_thr_jnienv(env);

    jclass cls = env->FindClass("android/util/Log");
    if (!cls) return -1;
    jmethodID mid = env->GetStaticMethodID(cls, "i", "(Ljava/lang/String;Ljava/lang/String;)I");
    if (!mid) return -1;

    rho::common::clsAndroidLog = (jclass)env->NewGlobalRef(cls);
    if (!rho::common::clsAndroidLog) return -1;
    env->DeleteLocalRef(cls);
    rho::common::midAndroidLogI = mid;

    jstring strTag = env->NewStringUTF("APP");
    if (!strTag) return -1;
    rho::common::tagAndroidLog = (jstring)env->NewGlobalRef(strTag);
    if (!rho::common::tagAndroidLog) return -1;
    env->DeleteLocalRef(strTag);

    // env->CallStaticIntMethod(rho::common::clsAndroidLog, rho::common::midAndroidLogI,
    //     rho::common::tagAndroidLog, env->NewStringUTF("++++++ INIT ++++++"));

    const char *classes[] = {
        RHODES_JAVA_CLASS_ANDROID_LOG,
        RHODES_JAVA_CLASS_ITERATOR,
        RHODES_JAVA_CLASS_SET,
        RHODES_JAVA_CLASS_MAP,
        RHODES_JAVA_CLASS_RHODES,
        RHODES_JAVA_CLASS_WEB_VIEW,
        RHODES_JAVA_CLASS_GEO_LOCATION,
        RHODES_JAVA_CLASS_CAMERA,
        RHODES_JAVA_CLASS_DATE_TIME_PICKER,
        RHODES_JAVA_CLASS_PHONEBOOK,
        RHODES_JAVA_CLASS_CONTACT,
        RHODES_JAVA_CLASS_CONTACT_FIELD,
        RHODES_JAVA_CLASS_ALERT,
        RHODES_JAVA_CLASS_RINGTONE_MANAGER
    };

//#define RHO_LOG_JNI_INIT

#ifdef RHO_LOG_JNI_INIT
    FILE *fp = fopen("/sdcard/rholog.txt", "wb");
#endif
    for(size_t i = 0, lim = sizeof(classes)/sizeof(classes[0]); i != lim; ++i)
    {
        const char *className = classes[i];
#ifdef RHO_LOG_JNI_INIT
        fprintf(fp, "Find class %s...\n", className);
#endif
        jclass cls = env->FindClass(className);
        if (!cls)
        {
#ifdef RHO_LOG_JNI_INIT
            fprintf(fp, "Can not resolve Java class: %s (JNI)\n", className);
#endif
            return -1;
        }
#ifdef RHO_LOG_JNI_INIT
        fprintf(fp, "Class found: %p\n", cls);
#endif
        g_classes[className] = (jclass)env->NewGlobalRef(cls);
        env->DeleteLocalRef(cls);
    }
#ifdef RHO_LOG_JNI_INIT
    fclose(fp);
#endif

    return jversion;
}

VALUE convertJavaMapToRubyHash(jobject objMap)
{
    jclass clsMap = getJNIClass(RHODES_JAVA_CLASS_MAP);
    if (!clsMap) return Qnil;
    jclass clsSet = getJNIClass(RHODES_JAVA_CLASS_SET);
    if (!clsSet) return Qnil;
    jclass clsIterator = getJNIClass(RHODES_JAVA_CLASS_ITERATOR);
    if (!clsIterator) return Qnil;

    jmethodID midGet = getJNIClassMethod(clsMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (!midGet) return Qnil;
    jmethodID midKeySet = getJNIClassMethod(clsMap, "keySet", "()Ljava/util/Set;");
    if (!midKeySet) return Qnil;
    jmethodID midIterator = getJNIClassMethod(clsSet, "iterator", "()Ljava/util/Iterator;");
    if (!midIterator) return Qnil;
    jmethodID midHasNext = getJNIClassMethod(clsIterator, "hasNext", "()Z");
    if (!midHasNext) return Qnil;
    jmethodID midNext = getJNIClassMethod(clsIterator, "next", "()Ljava/lang/Object;");
    if (!midNext) return Qnil;

    JNIEnv *env = jnienv();
    jobject objSet = env->CallObjectMethod(objMap, midKeySet);
    if (!objSet) return Qnil;
    jobject objIterator = env->CallObjectMethod(objSet, midIterator);
    if (!objIterator) return Qnil;

    VALUE retval = createHash();
    while(env->CallObjectMethod(objIterator, midHasNext))
    {
        jstring objKey = (jstring)env->CallObjectMethod(objIterator, midNext);
        if (!objKey) return Qnil;
        jstring objValue = (jstring)env->CallObjectMethod(objMap, midGet, objKey);
        if (!objValue) return Qnil;

        const char *strKey = env->GetStringUTFChars(objKey, JNI_FALSE);
        const char *strValue = env->GetStringUTFChars(objValue, JNI_FALSE);
        addStrToHash(retval, strKey, strValue, strlen(strValue));
        env->ReleaseStringUTFChars(objKey, strKey);
        env->ReleaseStringUTFChars(objValue, strValue);

        env->DeleteLocalRef(objKey);
        env->DeleteLocalRef(objValue);
    }
    return retval;
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Rhodes_setRootPath
  (JNIEnv *env, jobject, jstring path)
{
    const char *s = env->GetStringUTFChars(path, JNI_FALSE);
    g_rootPath = s;
    env->ReleaseStringUTFChars(path, s);
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Rhodes_makeLink
  (JNIEnv *env, jclass, jstring src, jstring dst)
{
    const char *strSrc = env->GetStringUTFChars(src, JNI_FALSE);
    const char *strDst = env->GetStringUTFChars(dst, JNI_FALSE);
    ::unlink(strDst);
    int err = ::symlink(strSrc, strDst);
    env->ReleaseStringUTFChars(src, strSrc);
    env->ReleaseStringUTFChars(dst, strDst);
    if (err != 0)
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "Can not create symlink");
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Rhodes_startRhodesApp
  (JNIEnv *env, jobject obj)
{
    // Init SQLite temp directory
    sqlite3_temp_directory = (char*)"/sqlite_stmt_journals";

    const char* szRootPath = rho_native_rhopath();

    // Init logconf
    rho_logconf_Init(szRootPath);

    // Disable log to stdout as on android all stdout redirects to /dev/null
    RHOCONF().setBool("LogToOutput", false);
    RHOCONF().saveToFile();
    LOGCONF().setLogToOutput(false);
    // Add android system log sink
    LOGCONF().setLogView(rho::common::g_androidLogSink);

    // Start Rhodes application
    rho_rhodesapp_create(szRootPath);
    rho_rhodesapp_start();
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Rhodes_stopRhodesApp
  (JNIEnv *, jobject)
{
    rho_rhodesapp_destroy();
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Rhodes_doSyncAllSources
  (JNIEnv *, jobject, jboolean show_status_popup)
{
    rho_sync_doSyncAllSources(show_status_popup);
}

JNIEXPORT jstring JNICALL Java_com_rhomobile_rhodes_Rhodes_getOptionsUrl
  (JNIEnv *env, jobject)
{
    const char *s = RHODESAPP().getOptionsUrl().c_str();
    return env->NewStringUTF(s);
}

JNIEXPORT jstring JNICALL Java_com_rhomobile_rhodes_Rhodes_getStartUrl
  (JNIEnv *env, jobject)
{
    const char *s = RHODESAPP().getStartUrl().c_str();
    return env->NewStringUTF(s);
}

JNIEXPORT jstring JNICALL Java_com_rhomobile_rhodes_Rhodes_getCurrentUrl
  (JNIEnv *env, jobject)
{
    const char *s = RHODESAPP().getCurrentUrl(0).c_str();
    return env->NewStringUTF(s);
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Rhodes_dblock
  (JNIEnv *, jobject)
{
    rho::sync::CSyncThread::getDBAdapter().executeSQL("begin immediate");
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Rhodes_dbunlock
  (JNIEnv *, jobject)
{
    rho::sync::CSyncThread::getDBAdapter().executeSQL("rollback");
}

