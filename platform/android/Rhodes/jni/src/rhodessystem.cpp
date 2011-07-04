#include <sys/stat.h>
#include <sys/resource.h>
#include <pwd.h>

#include "rhodes/JNIRhodes.h"
#include "common/AutoPointer.h"
#include "common/RhoStd.h"
#include "common/RhoConf.h"
#include "common/RhodesAppBase.h"
#include "sqlite/sqlite3.h"
#include "logging/RhoLogConf.h"

//--------------------------------------------------------------------------------------------------
static rho::common::CAutoPtr<rho::common::AndroidLogSink> s_logSink(new rho::common::AndroidLogSink());

static rho::String s_root_path;
static rho::String s_sqlite_path;

//--------------------------------------------------------------------------------------------------
RHO_GLOBAL void android_set_path(const rho::String& root, const rho::String& sqlite)
{
    s_root_path = root;
    s_sqlite_path = sqlite;
}
//--------------------------------------------------------------------------------------------------
rho::String const &rho_root_path()
{
    return s_root_path;
}
//--------------------------------------------------------------------------------------------------
const char* rho_native_rhopath()
{
    return rho_root_path().c_str();
}
//--------------------------------------------------------------------------------------------------
rho::String rho_cur_path()
{
    char buf[PATH_MAX];
    if (::getcwd(buf, sizeof(buf)) == NULL)
        return "";
    return buf;
}
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
RHO_GLOBAL void android_setup(JNIEnv *env)
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

    // Init SQLite temp directory
    sqlite3_temp_directory = (char*)s_sqlite_path.c_str();

    // Init logconf
    rho_logconf_Init(rho_native_rhopath(), "");

    // Disable log to stdout as on android all stdout redirects to /dev/null
    RHOCONF().setBool("LogToOutput", false, true);
    LOGCONF().setLogToOutput(false);
    // Add android system log sink
    LOGCONF().setLogView(s_logSink);
    
}
//--------------------------------------------------------------------------------------------------
RHO_GLOBAL void *rho_nativethread_start()
{
    JNIEnv *env;
    jvm()->AttachCurrentThread(&env, NULL);
    store_thr_jnienv(env);
    return NULL;
}
//--------------------------------------------------------------------------------------------------
RHO_GLOBAL void rho_nativethread_end(void *)
{
    jvm()->DetachCurrentThread();
}
//--------------------------------------------------------------------------------------------------
