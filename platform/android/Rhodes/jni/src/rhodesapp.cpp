#include <android/log.h>

#include "rhodes/jni/com_rhomobile_rhodes_RhodesService.h"
#include "rhodes/jni/com_rhomobile_rhodes_RhodesAppOptions.h"

#include <common/RhoConf.h>
#include <common/app_build_configs.h>
#include <logging/RhoLogConf.h>
#include <common/RhodesApp.h>
#include <common/AutoPointer.h>
#include <sync/SyncThread.h>
#include <sync/ClientRegister.h>

#include <sys/stat.h>
#include <sys/resource.h>
#include <pwd.h>

#include <genconfig.h>

#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoRuby.h"
#include "rhodes/RhoClassFactory.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhodesApp"


RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_makeLink
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

static std::string g_root_path;
static std::string g_sqlite_journals_path;
static std::string g_apk_path;
static rho::common::CAutoPtr<rho::common::AndroidLogSink> g_androidLogSink(new rho::common::AndroidLogSink());

std::string const &rho_root_path()
{
    return g_root_path;
}

const char* rho_native_rhopath()
{
    return rho_root_path().c_str();
}

std::string const &rho_apk_path()
{
    return g_apk_path;
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

static bool set_posix_environment(JNIEnv *env, jclass clsRE)
{
    // Set USER variable
    struct passwd *pwd = ::getpwuid(::getuid());
    if (!pwd)
    {
        env->ThrowNew(clsRE, "Can't find user name for current user");
        return false;
    }

    size_t len = ::strlen(pwd->pw_name) + 16;
    char *buf = (char *)::malloc(len + 1);
    buf[len] = '\0';
    ::snprintf(buf, len, "USER=%s", pwd->pw_name);
    int e = ::putenv(buf);
    ::free(buf);
    if (e != 0)
    {
        env->ThrowNew(clsRE, "Can't set USER environment variable");
        return false;
    }

    // Set HOME variable
    std::string root_path = rho_root_path();
    if (!root_path.empty() && root_path[root_path.size() - 1] == '/')
        root_path.erase(root_path.size() - 1);
    len = root_path.size() + 16;
    buf = (char *)::malloc(len + 1);
    buf[len] = '\0';
    ::snprintf(buf, len, "HOME=%s", root_path.c_str());
    e = ::putenv(buf);
    ::free(buf);
    if (e != 0)
    {
        env->ThrowNew(clsRE, "Can't set HOME environment variable");
        return false;
    }

    // Set TMP variable
    len = root_path.size() + 32;
    buf = (char *)::malloc(len + 1);
    buf[len] = '\0';
    ::snprintf(buf, len, "TMP=%s/tmp", root_path.c_str());
    e = ::putenv(buf);
    ::free(buf);
    if (e != 0)
    {
        env->ThrowNew(clsRE, "Can't set TMP environment variable");
        return false;
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

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_initClassLoader
  (JNIEnv *env, jobject, jobject cl)
{
    g_classLoader = env->NewGlobalRef(cl);
    jclass javaLangClassLoader = env->FindClass("java/lang/ClassLoader");
    g_loadClass = env->GetMethodID(javaLangClassLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_nativeInitPath
  (JNIEnv *env, jobject, jstring root_path, jstring sqlite_journals_path, jstring apk_path)
{
    g_root_path = rho_cast<std::string>(env, root_path);
    g_sqlite_journals_path = rho_cast<std::string>(env, sqlite_journals_path);
    g_apk_path = rho_cast<std::string>(env, apk_path);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesApplication_createRhodesApp
  (JNIEnv *env, jclass)
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

    if (!set_posix_environment(env, clsRE)) return;

    if (::chdir(rho_root_path().c_str()) == -1)
    {
        env->ThrowNew(clsRE, "Can not chdir to HOME directory");
        return;
    }

    if (!set_capabilities(env)) return;

    // Init SQLite temp directory
    sqlite3_temp_directory = (char*)g_sqlite_journals_path.c_str();

    const char* szRootPath = rho_native_rhopath();

    // Init logconf
    rho_logconf_Init(szRootPath, "");

    // Disable log to stdout as on android all stdout redirects to /dev/null
    RHOCONF().setBool("LogToOutput", false, true);
    LOGCONF().setLogToOutput(false);
    // Add android system log sink
    LOGCONF().setLogView(g_androidLogSink);

    // Start Rhodes application
    rho_rhodesapp_create(szRootPath);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesApplication_startRhodesApp
  (JNIEnv *, jclass)
{
    rho_rhodesapp_start();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesApplication_stopRhodesApp
  (JNIEnv *, jclass)
{
    rho_rhodesapp_destroy();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_doSyncAllSources
  (JNIEnv *, jobject, jboolean show_status_popup)
{
    rho_sync_doSyncAllSources(show_status_popup);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_doSyncSource
  (JNIEnv *env, jobject, jstring sourceObj)
{
    std::string source = rho_cast<std::string>(env, sourceObj);
    rho_sync_doSyncSourceByName(source.c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesApplication_setStartParameters
  (JNIEnv *env, jclass, jstring strUrl)
{
    std::string url = rho_cast<std::string>(env, strUrl);
    RHODESAPP().setStartParameters(url.c_str());
}


RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getOptionsUrl
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getOptionsUrl().c_str();
    return rho_cast<jhstring>(env, s).release();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getStartUrl
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getStartUrl().c_str();
    return rho_cast<jhstring>(env, s).release();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getCurrentUrl
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getCurrentUrl(0).c_str();
    return rho_cast<jhstring>(env, s).release();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getAppBackUrl
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getAppBackUrl().c_str();
    return rho_cast<jhstring>(env, s).release();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getBlobPath
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getBlobsDirPath().c_str();
    return rho_cast<jhstring>(env, s).release();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_doRequest
  (JNIEnv *env, jobject, jstring strUrl)
{
    std::string const &url = rho_cast<std::string>(strUrl);
    rho_net_request(url.c_str());
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesService_normalizeUrl
  (JNIEnv *env, jobject, jstring strUrl)
{
    std::string const &s = rho_cast<std::string>(strUrl);
    std::string const &cs = RHODESAPP().canonicalizeRhoUrl(s);
    return rho_cast<jhstring>(env, cs).release();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesService_getBuildConfig
  (JNIEnv *env, jclass, jstring key)
{
    std::string const &s = rho_cast<std::string>(key);
    const char* cs = get_app_build_config_item(s.c_str());
    return rho_cast<jhstring>(env, cs).release();
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhodesService_isOnStartPage
  (JNIEnv *env, jclass)
{
    bool res = RHODESAPP().isOnStartPage();
    return (jboolean)res;
}


RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhodesService_isTitleEnabled
  (JNIEnv *, jclass)
{
    bool value = true;
    const char* svalue = get_app_build_config_item("android_title");
    if (svalue != NULL) {
        value = svalue[0] != '0';
    } 
    return (jboolean)value;
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhodesApplication_canStartApp
  (JNIEnv *, jclass, jstring cmdLine, jstring sep)
{
    std::string const &strCmdLine = rho_cast<std::string>(cmdLine);
    std::string const &strSep = rho_cast<std::string>(sep);

    int nRes = rho_rhodesapp_canstartapp(strCmdLine.c_str(), strSep.c_str());
    return (jboolean)(nRes ? true : false);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_navigateBack
  (JNIEnv *, jclass)
{
    rho_rhodesapp_navigate_back();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_loadUrl
  (JNIEnv *env, jclass, jstring str)
{
    rho_rhodesapp_load_url(rho_cast<std::string>(env, str).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_onScreenOrientationChanged
  (JNIEnv *env, jclass, jint width, jint height, jint angle)
{
	rho_rhodesapp_callScreenRotationCallback(width, height, angle);
	//RAWLOG_ERROR3("$$$$$$$$$$$$$$$$ SCREEN : [%d]x[%d] angle[%d]", width, height, angle);	
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_callUiCreatedCallback
  (JNIEnv *, jclass)
{
    rho_rhodesapp_callUiCreatedCallback();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_callUiDestroyedCallback
  (JNIEnv *, jclass)
{
    rho_rhodesapp_callUiDestroyedCallback();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_callActivationCallback
  (JNIEnv *, jclass, jboolean active)
{
    rho_rhodesapp_callAppActiveCallback(active);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_setPushRegistrationId
  (JNIEnv *env, jobject, jstring jId)
{
    std::string id = rho_cast<std::string>(env, jId);
    rho::sync::CClientRegister* pClientRegister = rho::sync::CClientRegister::getInstance();
    if(pClientRegister && (pClientRegister->getDevicePin().compare(id) != 0))
    {
        rho::sync::CClientRegister::Destroy();
    }
    rho::sync::CClientRegister::Create(id.c_str());
    RHOCONF().setString("push_pin", id.c_str(), true);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesService_getPushRegistrationId
  (JNIEnv * env, jobject)
{
    return rho_cast<jhstring>(RHOCONF().getString("push_pin").c_str()).release();
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhodesService_callPushCallback
  (JNIEnv *env, jobject, jstring jData)
{
    std::string data = rho_cast<std::string>(env, jData);
    return (jboolean)rho_rhodesapp_callPushCallback(data.c_str());
}
