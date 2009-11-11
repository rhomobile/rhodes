#include <com_rhomobile_rhodes_Rhodes.h>

#include <logging/RhoLogConf.h>
#include <common/RhodesApp.h>

#include <sys/stat.h>

#include "JNIRhodes.h"

static rho::String g_appName;
static rho::String g_rootPath;

static pthread_key_t g_thrkey;

static JavaVM *g_jvm = NULL;
JavaVM *jvm()
{
    return g_jvm;
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

std::map<std::string, jclass> g_classes;

jclass getJNIClass(const char *name)
{
    JNIEnv *env = jnienv();
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
    if (g_rootPath.empty() && !g_appName.empty())
        g_rootPath = rho::String("/sdcard/rhomobile/") + g_appName + "/";
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

    std::vector<std::string> classes;
    classes.push_back("com/rhomobile/rhodes/Rhodes");
    classes.push_back("com/rhomobile/rhodes/WebView");

    //FILE *fp = fopen("/sdcard/rholog.txt", "ab");
    for(std::vector<std::string>::const_iterator it = classes.begin(), lim = classes.end();
        it != lim; ++it)
    {
        jclass cls = env->FindClass(it->c_str());
        //fprintf(fp, "Find class %s...\n", it->c_str());
        if (!cls)
        {
            //fprintf(fp, "Can not resolve Java class: %s (JNI)\n", it->c_str());
            continue;
        }
        //fprintf(fp, "Class found: %p\n", cls);
        g_classes[*it] = (jclass)env->NewGlobalRef(cls);
        env->DeleteLocalRef(cls);
    }
    //fclose(fp);

    return jversion;
}

JNIEXPORT jstring JNICALL Java_com_rhomobile_rhodes_Rhodes_getRootPath
  (JNIEnv *env, jobject obj)
{
    if (g_appName.empty())
    {
        jclass cls = env->GetObjectClass(obj);
        if (!cls) return NULL;
        jmethodID mid = getJNIClassMethod(cls, "getAppName", "()Ljava/lang/String;");
        if (!mid) return NULL;
        jstring str = (jstring)env->CallObjectMethod(obj, mid);
        const char *s = env->GetStringUTFChars(str, JNI_FALSE);
        g_appName = s;
        env->ReleaseStringUTFChars(str, s);
    }
    return env->NewStringUTF(rho_native_rhopath());
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Rhodes_startRhodesApp
  (JNIEnv *env, jobject obj)
{
    const char* szRootPath = rho_native_rhopath();
    rho_logconf_Init(szRootPath);
    rho_rhodesapp_create(szRootPath);
    rho_rhodesapp_start();
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Rhodes_stopRhodesApp
  (JNIEnv *, jobject)
{
    rho_rhodesapp_destroy();
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Rhodes_saveCurrentLocation
  (JNIEnv *, jobject, jstring)
{
    // TODO
}

JNIEXPORT jboolean JNICALL Java_com_rhomobile_rhodes_Rhodes_restoreLocation
  (JNIEnv *, jobject)
{
    // TODO
    return false;
}

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_Rhodes_doSyncAllSources
  (JNIEnv *, jobject, jboolean)
{
    // TODO
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
    const char *s = RHODESAPP().getCurrentUrl().c_str();
    return env->NewStringUTF(s);
}

