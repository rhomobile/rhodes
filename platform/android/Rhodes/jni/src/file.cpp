#include "rhodes/JNIRhodes.h"

#include <dlfcn.h>
#include <android/log.h>

#include "rhodes/jni/com_rhomobile_rhodes_file_RhoFileApi.h"

static int const RHO_FD_BASE = 4096;

#ifdef RHO_NOT_IMPLEMENTED
#undef RHO_NOT_IMPLEMENTED
#endif
#define RHO_NOT_IMPLEMENTED __android_log_print(ANDROID_LOG_ERROR, "RhoFileApi", \
    "WARNING: Call not implemented function: \"%s\" (defined here: %s:%d)", __PRETTY_FUNCTION__, __FILE__, __LINE__)

static int rho_log(const char *fmt, ...)
#if defined(__GNUC__)
      __attribute__ ((format(printf, 1, 2)))
#endif
    ;

int rho_log(const char *fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    int ret = __android_log_vprint(ANDROID_LOG_INFO, "RhoFileApi", fmt, vl);
    va_end(vl);
    return ret;
}

#define RHO_LOG rho_log

static jclass clsFileApi;
static jmethodID midOpen;
static jmethodID midStatType;
static jmethodID midStatSize;

typedef int (*func_open_t)(const char *path, int oflag, ...);
typedef int (*func_close_t)(int fd);
typedef ssize_t (*func_read_t)(int fd, void *buf, size_t count);
typedef ssize_t (*func_write_t)(int fd, const void *buf, size_t count);
typedef int (*func_stat_t)(const char *path, struct stat *buf);
typedef int (*func_fstat_t)(int filedes, struct stat *buf);
typedef int (*func_lstat_t)(const char *path, struct stat *buf);

static func_open_t real_open;
static func_close_t real_close;
static func_read_t real_read;
static func_write_t real_write;
static func_stat_t real_stat;
static func_fstat_t real_fstat;
static func_lstat_t real_lstat;

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_init
  (JNIEnv *env, jclass)
{
    clsFileApi = getJNIClass(RHODES_JAVA_CLASS_RHOFILEAPI);
    if (!clsFileApi) return;
    midOpen = getJNIClassStaticMethod(env, clsFileApi, "open", "(Ljava/lang/String;)I");
    if (!midOpen) return;
    midStatType = getJNIClassStaticMethod(env, clsFileApi, "statType", "(Ljava/lang/String;)I");
    if (!midStatType) return;
    midStatSize = getJNIClassStaticMethod(env, clsFileApi, "statSize", "(Ljava/lang/String;)I");
    if (!midStatSize) return;

    void *pc = dlopen("/system/lib/libc.so", RTLD_LAZY);
    real_open = (func_open_t)dlsym(pc, "open");
    real_close = (func_close_t)dlsym(pc, "close");
    real_read = (func_read_t)dlsym(pc, "read");
    real_write = (func_write_t)dlsym(pc, "write");
    real_stat = (func_stat_t)dlsym(pc, "stat");
    real_fstat = (func_fstat_t)dlsym(pc, "fstat");
    real_lstat = (func_lstat_t)dlsym(pc, "lstat");
    dlclose(pc);
}

static std::string make_full_path(const char *path)
{
    if (path == NULL || *path == '\0')
        return "";

    if (*path == '/')
        return path;

    RHO_LOG("make_full_path: path (%s) is relative, detect full path...", path);
    char *dir = 0;
    char *buf = 0;
    size_t bufsize = 128;
    while (!buf)
    {
        buf = (char *)malloc(bufsize);
        dir = getcwd(buf, bufsize);
        if (!dir)
        {
            free(buf);
            if (errno != ERANGE)
                return "";
            buf = 0;
            bufsize *= 2;
        }
    }
    std::string fpath = std::string(dir) + "/" + path;
    free(buf);

    return fpath;
}

static std::string make_full_path(std::string const &path)
{
    return make_full_path(path.c_str());
}

static bool need_java_way(std::string const &path)
{
    std::string fpath = make_full_path(path);
    RHO_LOG("need_java_way: full path: %s", fpath.c_str());

    std::string const &root_path = rho_root_path();
    if (strncmp(fpath.c_str(), root_path.c_str(), root_path.size()) == 0)
    {
        RHO_LOG("need_java_way: file is located under root_path");
        struct stat st;
        if (real_stat(fpath.c_str(), &st) == -1)
        {
            RHO_LOG("need_java_way: errno: %d", errno);
            if (errno == ENOENT)
                return true;

        }
    }

    return false;
}

RHO_GLOBAL int open(const char *path, int oflag, ...)
{
    RHO_LOG("open file %s...", path);
    if (oflag & O_CREAT)
    {
        RHO_LOG("open: requested O_CREAT");
        va_list vl;
        va_start(vl, oflag);
        mode_t mode = va_arg(vl, int);
        va_end(vl);
        return real_open(path, oflag, mode);
    }

    std::string fpath;
    if (path)
        fpath = path;

    if (fpath.empty())
    {
        RHO_LOG("open: path is empty");
        errno = EFAULT;
        return -1;
    }
    
    bool java_way = false;
    if (!(oflag & (O_WRONLY | O_RDWR)))
    {
        RHO_LOG("open: read-only");
        java_way = need_java_way(make_full_path(fpath));
    }

    int fd;
    if (java_way)
    {
        RHO_LOG("open: java way");

        JNIEnv *env = jnienv();
        jstring fullPathObj = rho_cast<jstring>(env, fpath.c_str());
        jint jfd = env->CallStaticIntMethod(clsFileApi, midOpen, fullPathObj);
        env->DeleteLocalRef(fullPathObj);

        fd = jfd < 0 ? jfd : RHO_FD_BASE + jfd;
    }
    else
    {
        RHO_LOG("open: native way");
        fd = real_open(path, oflag);
    }
    RHO_LOG("open file %s => %d", path, fd);
    return fd;
}

RHO_GLOBAL int openat(int fd, const char* path, int mode, ...)
{
    RHO_NOT_IMPLEMENTED;
    return -1;
}

RHO_GLOBAL int creat(const char* path, mode_t mode)
{
    return open(path, O_CREAT|O_WRONLY|O_TRUNC, mode);
}

RHO_GLOBAL int unlinkat(int dirfd, const char *pathname, int flags)
{
    RHO_NOT_IMPLEMENTED;
    return -1;
}

RHO_GLOBAL int fcntl(int fd, int command, ...)
{
    RHO_NOT_IMPLEMENTED;
    return -1;
}

RHO_GLOBAL int close(int fd)
{
    RHO_LOG("close fd %d", fd);
    return real_close(fd);
}

RHO_GLOBAL ssize_t read(int fd, void *buf, size_t count)
{
    RHO_LOG("read %lu bytes from fd %d", (unsigned long)count, fd);
    return real_read(fd, buf, count);
}

RHO_GLOBAL ssize_t write(int fd, const void *buf, size_t count)
{
    RHO_LOG("write %lu bytes to fd %d", (unsigned long)count, fd);
    return real_write(fd, buf, count);
}

RHO_GLOBAL int access(const char *, int)
{
    RHO_NOT_IMPLEMENTED;
    return -1;
}

RHO_GLOBAL off_t lseek(int, off_t, int)
{
    RHO_NOT_IMPLEMENTED;
    return (off_t)-1;
}

RHO_GLOBAL loff_t lseek64(int, loff_t, int)
{
    RHO_NOT_IMPLEMENTED;
    return (loff_t)-1;
}

RHO_GLOBAL int dup(int fd)
{
    RHO_NOT_IMPLEMENTED;
    return -1;
}

RHO_GLOBAL int dup2(int , int )
{
    RHO_NOT_IMPLEMENTED;
    return -1;
}

RHO_GLOBAL int flock(int , int )
{
    RHO_NOT_IMPLEMENTED;
    return -1;
}

RHO_GLOBAL int ioctl(int, int, ...)
{
    RHO_NOT_IMPLEMENTED;
    return -1;
}

RHO_GLOBAL int fsync(int )
{
    RHO_NOT_IMPLEMENTED;
    return -1;
}

RHO_GLOBAL int fdatasync(int )
{
    RHO_NOT_IMPLEMENTED;
    return -1;
}

RHO_GLOBAL int ftruncate(int , off_t )
{
    RHO_NOT_IMPLEMENTED;
    return -1;
}

RHO_GLOBAL int stat(const char *path, struct stat *buf)
{
    RHO_LOG("stat for path %s", path);
    std::string fpath = make_full_path(path);
    bool java_way = need_java_way(fpath);
    if (!java_way)
        return real_stat(path, buf);

    memset(buf, 0, sizeof(struct stat));

    JNIEnv *env = jnienv();
    jstring fullPathObj = rho_cast<jstring>(env, fpath.c_str());

    jint ret = env->CallStaticIntMethod(clsFileApi, midStatType, fullPathObj);
    switch (ret)
    {
    case 1:
        buf->st_mode = S_IFREG;
    case 2:
        buf->st_mode = S_IFDIR;
    default:
        env->DeleteLocalRef(fullPathObj);
        errno = ENOENT;
        return -1;
    }
    buf->st_mode |= S_IRWXU;
    buf->st_nlink = 1;

    ret = env->CallStaticIntMethod(clsFileApi, midStatSize, fullPathObj);
    if (ret == -1)
    {
        env->DeleteLocalRef(fullPathObj);
        errno = ENOENT;
        return -1;
    }
    buf->st_size = ret;

    env->DeleteLocalRef(fullPathObj);

    return 0;
}

RHO_GLOBAL int fstat(int fd, struct stat *buf)
{
    RHO_LOG("fstat for fd %d", fd);
    return real_fstat(fd, buf);
}

RHO_GLOBAL int lstat(const char *path, struct stat *buf)
{
    RHO_LOG("lstat for path %s", path);
    return real_lstat(path, buf);
}
