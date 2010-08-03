#include <android/log.h>

#include "rhodes/jni/com_rhomobile_rhodes_Rhodes.h"

#include <common/RhoConf.h>
#include <logging/RhoLogConf.h>
#include <common/RhodesApp.h>
#include <sync/SyncThread.h>

#include <sys/stat.h>
#include <sys/resource.h>

#include "rhodes/JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Rhodes"

const char *rho_java_class[] = {
#define RHODES_DEFINE_JAVA_CLASS(x, name) name,
#include <rhodes/details/rhojava.inc>
#undef RHODES_DEFINE_JAVA_CLASS
};

static std::string g_root_path;
static std::string g_sqlite_journals_path;

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

class AndroidLogSink : public ILogSink
{
public:
    void writeLogMessage(String &strMsg)
    {
        __android_log_write(ANDROID_LOG_INFO, "APP", strMsg.c_str());
    }

    int getCurPos()
    {
        return 0;
    }

    void clear() {}
};

static CAutoPtr<AndroidLogSink> g_androidLogSink(new AndroidLogSink());

} // namespace common
} // namespace rho

RhoValueConverter::RhoValueConverter(JNIEnv *e)
    :env(e), init(false)
{
    clsHashMap = getJNIClass(RHODES_JAVA_CLASS_HASHMAP);
    if (!clsHashMap) return;
    clsVector = getJNIClass(RHODES_JAVA_CLASS_VECTOR);
    if (!clsVector) return;
    midHashMapConstructor = getJNIClassMethod(env, clsHashMap, "<init>", "()V");
    if (!midHashMapConstructor) return;
    midVectorConstructor = getJNIClassMethod(env, clsVector, "<init>", "()V");
    if (!midVectorConstructor) return;
    midPut = getJNIClassMethod(env, clsHashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (!midPut) return;
    midAddElement = getJNIClassMethod(env, clsVector, "addElement", "(Ljava/lang/Object;)V");
    if (!midAddElement) return;
    init = true;
}

jobject RhoValueConverter::createObject(rho_param *p)
{
    if (!init || !p) return NULL;
    switch (p->type) {
    case RHO_PARAM_STRING:
        return rho_cast<jstring>(p->v.string);
        break;
    case RHO_PARAM_ARRAY:
        {
            jobject v = env->NewObject(clsVector, midVectorConstructor);
            if (!v) return NULL;

            for (int i = 0, lim = p->v.array->size; i < lim; ++i) {
                jobject value = createObject(p->v.array->value[i]);
                env->CallVoidMethod(v, midAddElement, value);
                env->DeleteLocalRef(value);
            }
            return v;
        }
        break;
    case RHO_PARAM_HASH:
        {
            jobject v = env->NewObject(clsHashMap, midHashMapConstructor);
            if (!v) return NULL;

            for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
                jstring key = rho_cast<jstring>(p->v.hash->name[i]);
                jobject value = createObject(p->v.hash->value[i]);
                env->CallObjectMethod(v, midPut, key, value);
                env->DeleteLocalRef(key);
                env->DeleteLocalRef(value);
            }
            return v;
        }
        break;
    default:
        return NULL;
    }
}

void store_thr_jnienv(JNIEnv *env)
{
    pthread_setspecific(g_thrkey, env);
}

JNIEnv *jnienv()
{
    JNIEnv *env = (JNIEnv *)pthread_getspecific(g_thrkey);
    return env;
}

std::vector<jclass> g_classes;

jclass getJNIClass(int n)
{
    if (n < 0 || (size_t)n >= g_classes.size())
    {
        RAWLOG_ERROR1("Illegal index when call getJNIClass: %d", n);
        return NULL;
    }
    return g_classes[n];
}

jclass getJNIObjectClass(JNIEnv *env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    if (!cls)
        RAWLOG_ERROR1("Can not get class for object: %p (JNI)", obj);
    return cls;
}

jfieldID getJNIClassField(JNIEnv *env, jclass cls, const char *name, const char *signature)
{
    jfieldID fid = env->GetFieldID(cls, name, signature);
    if (!fid)
        RAWLOG_ERROR3("Can not get field %s of signature %s for class %p", name, signature, cls);
    return fid;
}

jfieldID getJNIClassStaticField(JNIEnv *env, jclass cls, const char *name, const char *signature)
{
    jfieldID fid = env->GetStaticFieldID(cls, name, signature);
    if (!fid)
        RAWLOG_ERROR3("Can not get static field %s of signature %s for class %p", name, signature, cls);
    return fid;
}

jmethodID getJNIClassMethod(JNIEnv *env, jclass cls, const char *name, const char *signature)
{
    jmethodID mid = env->GetMethodID(cls, name, signature);
    if (!mid)
        RAWLOG_ERROR3("Can not get method %s of signature %s for class %p", name, signature, cls);
    return mid;
}

jmethodID getJNIClassStaticMethod(JNIEnv *env, jclass cls, const char *name, const char *signature)
{
    jmethodID mid = env->GetStaticMethodID(cls, name, signature);
    if (!mid)
        RAWLOG_ERROR3("Can not get static method %s of signature %s for class %p", name, signature, cls);
    return mid;
}

std::string const &rho_root_path()
{
    return g_root_path;
}

const char* rho_native_rhopath()
{
    return rho_root_path().c_str();
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

    for(size_t i = 0, lim = sizeof(rho_java_class)/sizeof(rho_java_class[0]); i != lim; ++i)
    {
        const char *className = rho_java_class[i];
        jclass cls = env->FindClass(className);
        if (!cls)
            return -1;
        g_classes.push_back((jclass)env->NewGlobalRef(cls));
        env->DeleteLocalRef(cls);
    }

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

    JNIEnv *env = jnienv();

    jmethodID midGet = getJNIClassMethod(env, clsMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (!midGet) return Qnil;
    jmethodID midKeySet = getJNIClassMethod(env, clsMap, "keySet", "()Ljava/util/Set;");
    if (!midKeySet) return Qnil;
    jmethodID midIterator = getJNIClassMethod(env, clsSet, "iterator", "()Ljava/util/Iterator;");
    if (!midIterator) return Qnil;
    jmethodID midHasNext = getJNIClassMethod(env, clsIterator, "hasNext", "()Z");
    if (!midHasNext) return Qnil;
    jmethodID midNext = getJNIClassMethod(env, clsIterator, "next", "()Ljava/lang/Object;");
    if (!midNext) return Qnil;

    jobject objSet = env->CallObjectMethod(objMap, midKeySet);
    if (!objSet) return Qnil;
    jobject objIterator = env->CallObjectMethod(objSet, midIterator);
    if (!objIterator) return Qnil;
                                  
    CHoldRubyValue retval(rho_ruby_createHash());
    while(env->CallBooleanMethod(objIterator, midHasNext))
    {
        jstring objKey = (jstring)env->CallObjectMethod(objIterator, midNext);
        if (!objKey) return Qnil;
        jstring objValue = (jstring)env->CallObjectMethod(objMap, midGet, objKey);
        if (!objValue) return Qnil;

        std::string const &strKey = rho_cast<std::string>(objKey);
        std::string const &strValue = rho_cast<std::string>(objValue);
        addStrToHash(retval, strKey.c_str(), strValue.c_str());

        env->DeleteLocalRef(objKey);
        env->DeleteLocalRef(objValue);
    }
    return retval;
}

namespace details
{

std::string rho_cast_helper<std::string, jstring>::operator()(JNIEnv *env, jstring s)
{
    const char *ts = env->GetStringUTFChars(s, JNI_FALSE);
    std::string ret(ts);
    env->ReleaseStringUTFChars(s, ts);
    return ret;
}

jstring rho_cast_helper<jstring, char const *>::operator()(JNIEnv *env, char const *s)
{
    return env->NewStringUTF(s);
}

} // namespace details

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Rhodes_makeLink
  (JNIEnv *env, jclass, jstring src, jstring dst)
{
    // We should not use rho_cast functions here because this function
    // called very early when jnienv() is not yet initialized
    const char *strSrc = env->GetStringUTFChars(src, JNI_FALSE);
    const char *strDst = env->GetStringUTFChars(dst, JNI_FALSE);
    ::unlink(strDst);
    int err = ::symlink(strSrc, strDst);
    env->ReleaseStringUTFChars(src, strSrc);
    env->ReleaseStringUTFChars(dst, strDst);
    if (err != 0)
        env->ThrowNew(getJNIClass(RHODES_JAVA_CLASS_RUNTIME_EXCEPTION), "Can not create symlink");
}

static bool set_capabilities(JNIEnv *env)
{
    char const *caps[] = {
#define RHO_DEFINE_CAP(x) #x,
#include <rhocaps.inc>
#undef RHO_DEFINE_CAP
    };
    std::map<std::string, bool> actual_caps;
#define RHO_DEFINE_CAP(x) actual_caps[#x] = RHO_CAP_ ## x ## _ENABLED;
#include <rhocaps.inc>
#undef RHO_DEFINE_CAP

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_CAPABILITIES);
    if (!cls) return false;
    for (size_t i = 0, lim = sizeof(caps)/sizeof(caps[0]); i < lim; ++i)
    {
        std::string field_name = std::string(caps[i]) + "_ENABLED";
        jfieldID fid = getJNIClassStaticField(env, cls, field_name.c_str(), "Z");
        if (!fid) return false;
        env->SetStaticBooleanField(cls, fid, actual_caps[caps[i]]);
    }
    return true;
}

static jobject g_classLoader = NULL;
static jmethodID g_loadClass = NULL;

jclass rho_find_class(JNIEnv *env, const char *c)
{
    jstring className = env->NewStringUTF(c);
    jclass cls = (jclass)env->CallObjectMethod(g_classLoader, g_loadClass, className);
    env->DeleteLocalRef(className);
    return cls;
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Rhodes_initClassLoader
  (JNIEnv *env, jobject, jobject cl)
{
    g_classLoader = env->NewGlobalRef(cl);
    jclass javaLangClassLoader = env->FindClass("java/lang/ClassLoader");
    g_loadClass = env->GetMethodID(javaLangClassLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Rhodes_nativeInitPath
  (JNIEnv *env, jobject, jstring root_path, jstring sqlite_journals_path)
{
    g_root_path = rho_cast<std::string>(env, root_path);
    g_sqlite_journals_path = rho_cast<std::string>(env, sqlite_journals_path);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Rhodes_createRhodesApp
  (JNIEnv *env, jobject)
{
    jclass clsRE = getJNIClass(RHODES_JAVA_CLASS_RUNTIME_EXCEPTION);
    if (!clsRE)
        return;

    struct rlimit rlim;
    if (getrlimit(RLIMIT_NOFILE, &rlim) == -1)
    {
        env->ThrowNew(clsRE, "Can not get maximum number of open files");
        return;
    }
    if (rlim.rlim_max < (unsigned long)RHO_FD_BASE)
    {
        env->ThrowNew(clsRE, "Current limit of open files is less then RHO_FD_BASE");
        return;
    }
    if (rlim.rlim_cur > (unsigned long)RHO_FD_BASE)
    {
        rlim.rlim_cur = RHO_FD_BASE;
        rlim.rlim_max = RHO_FD_BASE;
        if (setrlimit(RLIMIT_NOFILE, &rlim) == -1)
        {
            env->ThrowNew(clsRE, "Can not set maximum number of open files");
            return;
        }
    }

    if (!set_capabilities(env)) return;

    // Init SQLite temp directory
    sqlite3_temp_directory = (char*)g_sqlite_journals_path.c_str();

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
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Rhodes_startRhodesApp
  (JNIEnv *env, jobject obj)
{
    rho_rhodesapp_start();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Rhodes_stopRhodesApp
  (JNIEnv *, jobject)
{
    rho_rhodesapp_destroy();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Rhodes_doSyncAllSources
  (JNIEnv *, jobject, jboolean show_status_popup)
{
    rho_sync_doSyncAllSources(show_status_popup);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_Rhodes_getOptionsUrl
  (JNIEnv *env, jobject)
{
    const char *s = RHODESAPP().getOptionsUrl().c_str();
    return rho_cast<jstring>(env, s);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_Rhodes_getStartUrl
  (JNIEnv *env, jobject)
{
    const char *s = RHODESAPP().getStartUrl().c_str();
    return rho_cast<jstring>(env, s);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_Rhodes_getCurrentUrl
  (JNIEnv *env, jobject)
{
    const char *s = RHODESAPP().getCurrentUrl(0).c_str();
    return rho_cast<jstring>(env, s);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_Rhodes_getAppBackUrl
  (JNIEnv *env, jobject)
{
    const char *s = RHODESAPP().getAppBackUrl().c_str();
    return rho_cast<jstring>(env, s);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_Rhodes_getBlobPath
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getBlobsDirPath().c_str();
    return rho_cast<jstring>(env, s);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Rhodes_doRequest
  (JNIEnv *env, jobject, jstring strUrl)
{
    std::string const &url = rho_cast<std::string>(strUrl);
    rho_net_request(url.c_str());
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_Rhodes_normalizeUrl
  (JNIEnv *env, jobject, jstring strUrl)
{
    std::string const &s = rho_cast<std::string>(strUrl);
    std::string const &cs = RHODESAPP().canonicalizeRhoUrl(s);
    return rho_cast<jstring>(env, cs);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Rhodes_navigateBack
  (JNIEnv *, jclass)
{
    rho_rhodesapp_navigate_back();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Rhodes_loadUrl
  (JNIEnv *env, jclass, jstring str)
{
    rho_rhodesapp_load_url(rho_cast<std::string>(env, str).c_str());
}
