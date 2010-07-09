#include "rhodes/JNIRhodes.h"

#include <dlfcn.h>
#include <android/log.h>

#include "rhodes/jni/com_rhomobile_rhodes_file_RhoFileApi.h"

#ifdef RHO_NOT_IMPLEMENTED
#undef RHO_NOT_IMPLEMENTED
#endif
#define RHO_NOT_IMPLEMENTED __android_log_print(ANDROID_LOG_ERROR, "RhoFileApi", \
    "WARNING: Call not implemented function: \"%s\" (defined here: %s:%d)", __PRETTY_FUNCTION__, __FILE__, __LINE__)

typedef int (*func_open_t)(const char *path, int oflag, ...);
typedef int (*func_close_t)(int fd);
typedef ssize_t (*func_read_t)(int fd, void *buf, size_t count);
typedef ssize_t (*func_write_t)(int fd, const void *buf, size_t count);

static func_open_t real_open;
static func_close_t real_close;
static func_read_t real_read;
static func_write_t real_write;

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

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_init
  (JNIEnv *, jclass)
{
    void *pc = dlopen("/system/lib/libc.so", RTLD_LAZY);
    real_open = (func_open_t)dlsym(pc, "open");
    real_close = (func_close_t)dlsym(pc, "close");
    real_read = (func_read_t)dlsym(pc, "read");
    real_write = (func_write_t)dlsym(pc, "write");
    dlclose(pc);
}

RHO_GLOBAL int open(const char *path, int oflag, ...)
{
    mode_t mode;
    RHO_LOG("open file %s...", path);
    if (oflag & O_CREAT)
    {
        va_list vl;
        va_start(vl, oflag);
        mode = va_arg(vl, int);
        va_end(vl);
        return real_open(path, oflag, mode);
    }
    int fd = real_open(path, oflag);
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
