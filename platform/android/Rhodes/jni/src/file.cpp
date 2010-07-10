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

struct iss_data_t
{
    jobject is;
    std::string fpath;
    off_t pos;
};

typedef rho::common::CMutexLock scoped_lock_t;
static rho::common::CMutex iss_mtx;
typedef std::map<int, iss_data_t> iss_map_t;
iss_map_t iss_map;
static std::vector<int> iss_free;
static int iss_counter = RHO_FD_BASE;

static jclass clsFileApi;
static jmethodID midCopy;
static jmethodID midExists;
static jmethodID midSize;
static jmethodID midOpen;
static jmethodID midClose;
static jmethodID midRead;
static jmethodID midSeek;

typedef int (*func_open_t)(const char *path, int oflag, ...);
typedef int (*func_close_t)(int fd);
typedef ssize_t (*func_read_t)(int fd, void *buf, size_t count);
typedef ssize_t (*func_write_t)(int fd, const void *buf, size_t count);
typedef int (*func_stat_t)(const char *path, struct stat *buf);
typedef int (*func_fstat_t)(int filedes, struct stat *buf);
typedef int (*func_lstat_t)(const char *path, struct stat *buf);
typedef off_t (*func_lseek_t)(int fd, off_t offset, int whence);
typedef int (*func_fcntl_t)(int fd, int command, ...);
typedef int (*func_access_t)(const char *path, int mode);

static func_open_t real_open;
static func_close_t real_close;
static func_read_t real_read;
static func_write_t real_write;
static func_stat_t real_stat;
static func_fstat_t real_fstat;
static func_lstat_t real_lstat;
static func_lseek_t real_lseek;
static func_fcntl_t real_fcntl;
static func_access_t real_access;

struct stat file_stat;

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_nativeInit
  (JNIEnv *env, jclass)
{
    clsFileApi = getJNIClass(RHODES_JAVA_CLASS_RHOFILEAPI);
    if (!clsFileApi) return;
    midCopy = getJNIClassStaticMethod(env, clsFileApi, "copy", "(Ljava/lang/String;)Z");
    if (!midCopy) return;
    midExists = getJNIClassStaticMethod(env, clsFileApi, "exists", "(Ljava/lang/String;)Z");
    if (!midExists) return;
    midSize = getJNIClassStaticMethod(env, clsFileApi, "size", "(Ljava/lang/String;)J");
    if (!midSize) return;
    midOpen = getJNIClassStaticMethod(env, clsFileApi, "open", "(Ljava/lang/String;)Ljava/io/InputStream;");
    if (!midOpen) return;
    midClose = getJNIClassStaticMethod(env, clsFileApi, "close", "(Ljava/io/InputStream;)V");
    if (!midClose) return;
    midRead = getJNIClassStaticMethod(env, clsFileApi, "read", "(Ljava/io/InputStream;[B)I");
    if (!midRead) return;
    midSeek = getJNIClassStaticMethod(env, clsFileApi, "seek", "(Ljava/io/InputStream;I)V");
    if (!midSeek) return;

    const char *libc = "/system/lib/libc.so";
    void *pc = dlopen(libc, RTLD_LAZY);
    real_open = (func_open_t)dlsym(pc, "open");
    real_close = (func_close_t)dlsym(pc, "close");
    real_read = (func_read_t)dlsym(pc, "read");
    real_write = (func_write_t)dlsym(pc, "write");
    real_stat = (func_stat_t)dlsym(pc, "stat");
    real_fstat = (func_fstat_t)dlsym(pc, "fstat");
    real_lstat = (func_lstat_t)dlsym(pc, "lstat");
    real_lseek = (func_lseek_t)dlsym(pc, "lseek");
    real_fcntl = (func_fcntl_t)dlsym(pc, "fcntl");
    real_access = (func_access_t)dlsym(pc, "access");
    dlclose(pc);

    // This is just to get typical stat of file
    real_stat(libc, &file_stat);
    file_stat.st_mode = S_IFREG|S_IRWXU;
    file_stat.st_nlink = 1;
    file_stat.st_uid = getuid();
    file_stat.st_gid = getgid();
    time_t tm = time(NULL);
    file_stat.st_atime = tm;
    file_stat.st_mtime = tm;
    file_stat.st_ctime = tm;
}

#if 0
static void dump_stat(struct stat const &st)
{
    RHO_LOG("st.st_dev: %llx", (long long)st.st_dev);
    RHO_LOG("st.st_ino: %lx", (long)st.st_ino);
    RHO_LOG("st.st_mode: %x", (int)st.st_mode);
    RHO_LOG("st.st_nlink: %x", (int)st.st_nlink);
    RHO_LOG("st.st_uid: %lx", (unsigned long)st.st_uid);
    RHO_LOG("st.st_gid: %lx", (unsigned long)st.st_gid);
    RHO_LOG("st.st_rdev: %llx", (long long)st.st_rdev);
    RHO_LOG("st.st_size: %lld", (long long)st.st_size);
    RHO_LOG("st.st_blksize: %lu", (unsigned long)st.st_blksize);
    RHO_LOG("st.st_blocks: %llu", (unsigned long long)st.st_blocks);
    RHO_LOG("st.st_atime: %lu", (unsigned long)st.st_atime);
    RHO_LOG("st.st_atime_nsec: %lu", (unsigned long)st.st_atime_nsec);
    RHO_LOG("st.st_mtime: %lu", (unsigned long)st.st_mtime);
    RHO_LOG("st.st_mtime_nsec: %lu", (unsigned long)st.st_mtime_nsec);
    RHO_LOG("st.st_ctime: %lu", (unsigned long)st.st_ctime);
    RHO_LOG("st.st_ctime_nsec: %lu", (unsigned long)st.st_ctime_nsec);
    RHO_LOG("st.st_ino: %llx", (unsigned long long)st.st_ino);
}
#endif

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

static std::string make_rel_path(std::string const &fpath)
{
    std::string const &root_path = rho_root_path();
    return fpath.substr(root_path.size());
}

static bool need_java_way(std::string const &path)
{
    std::string fpath = make_full_path(path);
    std::string const &root_path = rho_root_path();
    if (strncmp(fpath.c_str(), root_path.c_str(), root_path.size()) == 0)
    {
        struct stat st;
        if (real_stat(fpath.c_str(), &st) == -1)
        {
            if (errno == ENOENT)
            {
                //RHO_LOG("No such file or directory: %s, need to read it from Android package", fpath.substr(root_path.size()).c_str());
                return true;
            }

        }
    }

    return false;
}

RHO_GLOBAL int open(const char *path, int oflag, ...)
{
    //RHO_LOG("open file %s...", path);
    if (oflag & O_CREAT)
    {
        //RHO_LOG("open: requested O_CREAT");
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
        //RHO_LOG("open: path is empty");
        errno = EFAULT;
        return -1;
    }
    fpath = make_full_path(fpath);
    
    bool java_way = need_java_way(fpath);
    if (java_way && (oflag & (O_WRONLY | O_RDWR)))
    {
        //RHO_LOG("open: for write");
        JNIEnv *env = jnienv();
        jstring relPathObj = rho_cast<jstring>(env, make_rel_path(fpath).c_str());
        env->CallStaticVoidMethod(clsFileApi, midCopy, relPathObj);
        env->DeleteLocalRef(relPathObj);

        java_way = false;
    }

    int fd;
    if (java_way)
    {
        //RHO_LOG("open: java way");

        JNIEnv *env = jnienv();
        jstring relPathObj = rho_cast<jstring>(env, make_rel_path(fpath).c_str());
        jobject is = env->CallStaticObjectMethod(clsFileApi, midOpen, relPathObj);
        env->DeleteLocalRef(relPathObj);

        fd = -1;
        if (is != NULL)
        {
            scoped_lock_t guard(iss_mtx);
            if (!iss_free.empty())
            {
                fd = iss_free[0];
                iss_free.erase(iss_free.begin());
            }
            else
                fd = iss_counter++;
            iss_data_t d;
            d.is = is;
            d.fpath = fpath;
            d.pos = 0;
            iss_map[fd] = d;
        }
    }
    else
    {
        //RHO_LOG("open: native way");
        fd = real_open(path, oflag);
    }
    //RHO_LOG("open file %s => %d", path, fd);
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
    if (fd < RHO_FD_BASE)
    {
        va_list vl;
        va_start(vl, command);
        int arg = va_arg(vl, int);
        va_end(vl);
        return real_fcntl(fd, command, arg);
    }

    RHO_NOT_IMPLEMENTED;
    return -1;
}

RHO_GLOBAL int close(int fd)
{
    //RHO_LOG("close fd %d", fd);
    if (fd < RHO_FD_BASE)
        return real_close(fd);

    jobject is = NULL;
    {
        scoped_lock_t guard(iss_mtx);

        iss_map_t::iterator it = iss_map.find(fd);
        if (it == iss_map.end())
        {
            errno = EBADF;
            return -1;
        }

        is = it->second.is;
        iss_map.erase(it);
        iss_free.push_back(fd);
    }

    JNIEnv *env = jnienv();
    env->CallStaticVoidMethod(clsFileApi, midClose, is);
    return 0;
}

RHO_GLOBAL ssize_t read(int fd, void *buf, size_t count)
{
    //RHO_LOG("read from fd %d...", fd);
    if (fd < RHO_FD_BASE)
        return real_read(fd, buf, count);

    jobject is = NULL;
    {
        scoped_lock_t guard(iss_mtx);

        iss_map_t::iterator it = iss_map.find(fd);
        if (it == iss_map.end())
        {
            errno = EBADF;
            return (ssize_t)-1;
        }

        is = it->second.is;
    }

    JNIEnv *env = jnienv();

    jbyteArray array = env->NewByteArray(count);
    jint result = env->CallStaticIntMethod(clsFileApi, midRead, is, array);

    if (result > 0)
    {
        {
            scoped_lock_t guard(iss_mtx);
            iss_map_t::iterator it = iss_map.find(fd);
            if (it == iss_map.end())
            {
                errno = EIO;
                return (off_t)-1;
            }
            it->second.pos += result;
        }

        jbyte *arr = env->GetByteArrayElements(array, NULL);
        if (!arr)
        {
            errno = EIO;
            return (ssize_t)-1;
        }
        std::memmove(buf, arr, result);
        env->ReleaseByteArrayElements(array, arr, JNI_ABORT);
    }
    env->DeleteLocalRef(array);
    return result;
}

RHO_GLOBAL ssize_t write(int fd, const void *buf, size_t count)
{
    //RHO_LOG("write %lu bytes to fd %d", (unsigned long)count, fd);
    if (fd < RHO_FD_BASE)
        return real_write(fd, buf, count);

    errno = EBADF;
    return -1;
}

RHO_GLOBAL int access(const char *path, int mode)
{
    struct stat st;
    if (stat(path, &st) == -1)
        return -1;

    if (((mode & R_OK) && (!(st.st_mode & S_IRUSR))) ||
        ((mode & W_OK) && (!(st.st_mode & S_IWUSR))) ||
        ((mode & X_OK) && (!(st.st_mode & S_IXUSR))))
    {
        errno = EACCES;
        return -1;
    }

    return 0;
}

RHO_GLOBAL off_t lseek(int fd, off_t offset, int whence)
{
    if (fd < RHO_FD_BASE)
        return real_lseek(fd, offset, whence);

    jobject is = NULL;
    off_t pos = 0;

    {
        scoped_lock_t guard(iss_mtx);

        iss_map_t::iterator it = iss_map.find(fd);
        if (it == iss_map.end())
        {
            errno = EBADF;
            return (off_t)-1;
        }

        is = it->second.is;
        pos = it->second.pos;
    }

    struct stat st;
    if (fstat(fd, &st) == -1)
        return -1;

    switch (whence)
    {
    case SEEK_SET:
        if (offset > st.st_size)
            offset = st.st_size;
        pos = offset;
        break;
    case SEEK_CUR:
        if (pos + offset > st.st_size)
            offset = st.st_size - pos;
        pos += offset;
        break;
    case SEEK_END:
        if (offset > st.st_size)
            offset = st.st_size;
        pos = st.st_size - offset;
        break;
    default:
        errno = EINVAL;
        return -1;
    }

    {
        scoped_lock_t guard(iss_mtx);

        iss_map_t::iterator it = iss_map.find(fd);
        if (it == iss_map.end())
        {
            errno = EBADF;
            return (off_t)-1;
        }
        it->second.pos = pos;
    }

    JNIEnv *env = jnienv();
    env->CallStaticVoidMethod(clsFileApi, midSeek, is, pos);

    return pos;
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
    std::string fpath = make_full_path(path);
    bool java_way = need_java_way(fpath);
    if (!java_way)
        return real_stat(path, buf);

    memset(buf, 0, sizeof(struct stat));

    JNIEnv *env = jnienv();
    jstring relPathObj = rho_cast<jstring>(env, make_rel_path(fpath).c_str());

    jboolean exists = env->CallStaticBooleanMethod(clsFileApi, midExists, relPathObj);
    if (!exists)
    {
        env->DeleteLocalRef(relPathObj);
        errno = ENOENT;
        return -1;
    }

    jlong size = env->CallStaticLongMethod(clsFileApi, midSize, relPathObj);
    if (size == -1)
    {
        env->DeleteLocalRef(relPathObj);
        errno = EFAULT;
        return -1;
    }

    memcpy(buf, &file_stat, sizeof(struct stat));
    buf->st_size = size;

    env->DeleteLocalRef(relPathObj);
    return 0;
}

RHO_GLOBAL int fstat(int fd, struct stat *buf)
{
    if (fd < RHO_FD_BASE)
        return real_fstat(fd, buf);

    std::string fpath;

    {
        scoped_lock_t guard(iss_mtx);

        iss_map_t::iterator it = iss_map.find(fd);
        if (it == iss_map.end())
        {
            errno = EBADF;
            return -1;
        }

        fpath = it->second.fpath;
    }

    return stat(fpath.c_str(), buf);
}

RHO_GLOBAL int lstat(const char *path, struct stat *buf)
{
    std::string fpath = make_full_path(path);
    if (!need_java_way(fpath))
        return real_lstat(path, buf);

    return stat(path, buf);
}
