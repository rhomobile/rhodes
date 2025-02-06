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

//#define RHO_ENABLE_LOG
#include "rhodes/JNIRhodes.h"

#include <dirent.h>
#include <dlfcn.h>
#include <android/fdsan.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <jni.h>

#include <cstring>
#include <algorithm>
#include <cassert>

#if __cplusplus == 201103L
#include <unordered_map>
#else
#include <map>
#endif

#include "rhodes/fileapi.h"
#include "rhodes/jni/com_rhomobile_rhodes_file_RhoFileApi.h"

/*#undef stdin;   
#undef stdout;   
#undef stderr; 
extern FILE __sF[] __REMOVED_IN(23);
FILE * stdin = (&__sF[0]);
FILE * stdout = (&__sF[1]);
FILE * stderr = (&__sF[2]);*/

#if (__ANDROID_API__ > 19)
#include <android/api-level.h>
#include <android/log.h>
#include <signal.h>
#include <dlfcn.h>

extern "C" {
  typedef __sighandler_t (*bsd_signal_func_t)(int, __sighandler_t);
  bsd_signal_func_t bsd_signal_func = NULL;

  __sighandler_t bsd_signal(int s, __sighandler_t f) {
    if (bsd_signal_func == NULL) {
      // For now (up to Android 7.0) this is always available 
      bsd_signal_func = (bsd_signal_func_t) dlsym(RTLD_DEFAULT, "bsd_signal");

      if (bsd_signal_func == NULL) {
        // You may try dlsym(RTLD_DEFAULT, "signal") or dlsym(RTLD_NEXT, "signal") here
        // Make sure you add a comment here in StackOverflow
        // if you find a device that doesn't have "bsd_signal" in its libc.so!!!

        __android_log_assert("", "bsd_signal_wrapper", "bsd_signal symbol not found!");
      }
    }

    return bsd_signal_func(s, f);
  }
}
#endif

#ifdef RHO_NOT_IMPLEMENTED
#undef RHO_NOT_IMPLEMENTED
#endif
#define RHO_NOT_IMPLEMENTED \
  __android_log_print(ANDROID_LOG_ERROR, "RhoFileApiNotImpl", \
    "WARNING: Call not implemented function: \"%s\" (defined here: %s:%d)", __PRETTY_FUNCTION__, __FILE__, __LINE__); \
  errno = EACCES; \
  return -1

//#define RHO_ENABLE_LOG
#if defined(OS_ANDROID) && defined(RHO_ENABLE_LOG)
#include <android/log.h>
#define RHO_LOG(fmt, ...) \
  __android_log_print(ANDROID_LOG_INFO, "RHO_LOG", "%s:%d: thread %08lx: " fmt, __FILE__, __LINE__, \
      (unsigned long)pthread_self(), ##__VA_ARGS__)

#else // OS_ANDROID
#define RHO_LOG(...)
#endif // OS_ANDROID

#define RHO_ERROR_LOG(fmt, ...) \
  __android_log_print(ANDROID_LOG_ERROR, "RHO_LOG", "%s:%d: thread %08lx: " fmt, __FILE__, __LINE__, \
      (unsigned long)pthread_self(), ##__VA_ARGS__)


#define RESET_ERRNO {errno = 0;}


#define RHO_TRACE_POINT RHO_LOG("trace point")

enum rho_fileapi_type_t
{
    rho_type_file,
    rho_type_dir
};

struct rho_stat_t
{
    rho_fileapi_type_t type;
    size_t size;
    uint64_t ino;
    unsigned long mtime;
};

#if __cplusplus == 201103L
typedef std::unordered_map<std::string, rho_stat_t> rho_stat_map_t;
#else
typedef std::map<std::string, rho_stat_t> rho_stat_map_t;
#endif


static rho_stat_map_t rho_stat_map;

struct rho_fd_data_t
{
    rho_fileapi_type_t type;
    jobject is;
    DIR *dirp;
    std::string fpath;
    loff_t pos;
    int refCount;
};

typedef rho::common::CMutexLock scoped_lock_t;
static rho::common::CMutex rho_file_mtx;
typedef std::map<int, rho_fd_data_t> rho_fd_map_t;
rho_fd_map_t rho_fd_map;
static std::vector<int> rho_fd_free;
static int rho_fd_counter = RHO_FD_BASE;

static uint64_t rho_ino = (uint64_t)-1;

struct rho_dir_data_t
{
    int fd;
    size_t index;
    std::vector<struct dirent> childs;

    rho_dir_data_t() :fd(-1), index(0) {}
};

typedef std::map<DIR*, rho_dir_data_t> rho_dir_map_t;
rho_dir_map_t rho_dir_map;
static std::vector<DIR*> rho_dir_free;
static int rho_dir_counter = -1;

static volatile RhoFsMode rho_fs_mode = RHO_FS_DISK_ONLY;

static jclass clsFileApi;
static jmethodID midCopy;
static jmethodID midOpen;
static jmethodID midClose;
static jmethodID midRead;
static jmethodID midSeek;
static jmethodID midGetChildren;
static jmethodID midReloadStatTable;
static jmethodID midForceAllFiles;
static jmethodID midDeleteRecursively;

static jobject jAssetManager;
static AAssetManager* pAssetManager = 0;

typedef FILE *(*func_sfp_t)();
typedef int (*func_sflags_t)(const char *mode, int *optr);

#ifndef DEFFILEMODE
#define DEFFILEMODE  (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#endif

#ifndef __aarch64__
static func_sflags_t __sflags;
#endif
static func_sfp_t __sfp;

// ssize_t pread(int fd, void *buf, size_t count, off_t offset);
// ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);

// ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
// int utime(const char *filename, const struct utimbuf *times);

// int utimes(const char *filename, const struct timeval times[2]);
// int futimes(int fd, const struct timeval tv[2]);

// int pipe2(int pipefd[2], int flags);

// int pipe(int pipefd[2]);


typedef int (*func_access_t)(const char *path, int mode);
typedef int (*func_close_t)(int fd);
typedef int (*func_dup2_t)(int fd, int fd2);
typedef int (*func_dup_t)(int fd);
typedef int (*func_chown_t)(const char *path, uid_t uid, gid_t gid);
typedef int (*func_fchown_t)(int fd, uid_t uid, gid_t gid);
typedef int (*func_lchown_t)(const char *path, uid_t uid, gid_t gid);
typedef int (*func_link_t)(const char *src, const char *dst);
typedef int (*func_symlink_t)(const char *src, const char *dst);
#if defined(__aarch64__)
typedef ssize_t (*func_readlink_t)(const char *path, char *buf, size_t bufsize);
#else
typedef int (*func_readlink_t)(const char *path, char *buf, size_t bufsize);
#endif
typedef int (*func_mkdir_t)(const char *path, mode_t mode);
typedef int (*func_fchdir_t)(int fd);
typedef int (*func_fcntl_t)(int fd, int command, ...);
typedef int (*func_ioctl_t)(int fd, int command, ...);
typedef int (*func_fdatasync_t)(int fd);
typedef int (*func_flock_t)(int fd, int operation);
typedef int (*func_fstat_t)(int filedes, struct stat *buf);
typedef int (*func_fsync_t)(int fd);
typedef int (*func_ftruncate_t)(int fd, off_t offset);
typedef int (*func_ftruncate64_t)(int fd, off64_t offset);
typedef int (*func_lstat_t)(const char *path, struct stat *buf);
typedef int (*func_open_t)(const char *path, int oflag, ...);
typedef int (*func_select_t)(int maxfd, fd_set *rfd, fd_set *wfd, fd_set *efd, struct timeval *tv);
typedef int (*func_stat_t)(const char *path, struct stat *buf);
typedef int (*func_unlink_t)(const char *path);
typedef loff_t (*func_lseek64_t)(int fd, loff_t offset, int whence);
typedef off_t (*func_lseek_t)(int fd, off_t offset, int whence);
typedef ssize_t (*func_read_t)(int fd, void *buf, size_t count);
typedef ssize_t (*func_write_t)(int fd, const void *buf, size_t count);

typedef ssize_t (*func_pread_t)(int fd, void *buf, size_t count, off_t offset);
typedef ssize_t (*func_pwrite_t)(int fd, const void *buf, size_t count, off_t offset);

typedef ssize_t (*func_sendfile_t)(int out_fd, int in_fd, off_t *offset, size_t count);

typedef int (*func_utime_t)(const char *filename, const struct utimbuf *times);
typedef int (*func_utimes_t)(const char *filename, const struct timeval times[2]);
typedef int (*func_futimes_t)(int fd, const struct timeval tv[2]);

typedef int (*func_pipe2_t)(int pipefd[2], int flags);
typedef int (*func_pipe_t)(int pipefd[2]);

typedef int (*func_getdents_t)(unsigned int, struct dirent *, unsigned int);
typedef int (*func_rmdir_t)(const char *path);
typedef DIR *(*func_opendir_t)(const char *dirpath);
typedef DIR *(*func_fdopendir_t)(int fd);
typedef struct dirent *(*func_readdir_t)(DIR *dirp);
typedef int (*func_readdir_r_t)(DIR *dirp, struct dirent *entry, struct dirent **result);
typedef int (*func_closedir_t)(DIR *dirp);
typedef void (*func_rewinddir_t)(DIR *dirp);
typedef int (*func_dirfd_t)(DIR *dirp);
typedef int (*func_scandir_t)(const char *dir, struct dirent ***namelist,
    int (*filter)(const struct dirent *),
    int (*compar)(const struct dirent **, const struct dirent **));

typedef size_t (*func_fread_t)(void* __buf, size_t __size, size_t __count, FILE* __fp);
typedef size_t (*func_fwrite_t)(const void* __buf, size_t __size, size_t __count, FILE* __fp);
typedef int (*func_fseek_t)(FILE* __fp, long __offset, int __whence);
typedef long (*func_ftell_t)(FILE* __fp);
typedef int (*func_fclose_t)(FILE* __fp);
typedef FILE * (*func_fopen_t)(const char* fname, const char* mode);

static func_fread_t real_fread;
static func_fwrite_t real_fwrite;
static func_fseek_t real_fseek;
static func_ftell_t real_ftell;
static func_fclose_t real_fclose;
static func_fopen_t real_fopen;
static func_fopen_t real_fopen64;

static func_access_t real_access;
static func_close_t real_close;
static func_dup2_t real_dup2;
static func_dup_t real_dup;
static func_chown_t real_chown;
static func_fchown_t real_fchown;
static func_lchown_t real_lchown;
static func_link_t real_link;
static func_symlink_t real_symlink;
static func_readlink_t real_readlink;
static func_mkdir_t real_mkdir;
static func_fchdir_t real_fchdir;
static func_fcntl_t real_fcntl;
static func_ioctl_t real_ioctl;
static func_fdatasync_t real_fdatasync;
static func_flock_t real_flock;
static func_fstat_t real_fstat;
static func_fsync_t real_fsync;
static func_ftruncate_t real_ftruncate;
static func_ftruncate64_t real_ftruncate64;
static func_lseek64_t real_lseek64;
static func_lseek_t real_lseek;
static func_lstat_t real_lstat;
static func_open_t real_open;
static func_open_t real_open64;
static func_read_t real_read;

static func_pread_t real_pread;
static func_pread_t real_pread64;
static func_pwrite_t real_pwrite;
static func_pwrite_t real_pwrite64;

static func_sendfile_t real_sendfile;

static func_utime_t real_utime;
static func_utimes_t real_utimes;
static func_futimes_t real_futimes;
static func_pipe2_t real_pipe2;
static func_pipe_t real_pipe;

static func_select_t real_select;
static func_stat_t real_stat;
static func_unlink_t real_unlink;
static func_write_t real_write;
static func_getdents_t real_getdents;
static func_rmdir_t real_rmdir;
static func_opendir_t real_opendir;
static func_fdopendir_t real_fdopendir;
static func_readdir_t real_readdir;
static func_readdir_r_t real_readdir_r;
static func_closedir_t real_closedir;
static func_rewinddir_t real_rewinddir;
static func_dirfd_t real_dirfd;
static func_scandir_t real_scandir;

struct stat librhodes_st;

static std::string g_apk_path;

RHO_GLOBAL void rho_file_set_fs_mode(int mode);

std::string const &rho_apk_path()
{
    return g_apk_path;
}

extern "C" void *rho_nativethread_start();

JNIEnv* jnienv_fileapi() {
    JNIEnv *env = jnienv();
    if (!env) {
        __android_log_print(ANDROID_LOG_ERROR, "fileapi", "ERROR!!! thread is not initialised by Rhodes !!!");
        rho_nativethread_start();
    }
    env = jnienv();
    return env;
}

static bool has_pending_exception()
{
    JNIEnv *env = jnienv_fileapi();
    if (env->ExceptionCheck())
    {
        __android_log_print(ANDROID_LOG_ERROR, "fileapi", "ERROR!!! Pending exception exist!");
        return true;
    }
    return false;
}

void updateStatTable(const std::string& path, rho_fileapi_type_t type, size_t size, unsigned long time)
{
    RHO_LOG("Update stat table: %s, %d, %u, %u", path.c_str(), (int)type, (unsigned long)size, (unsigned long)time);

    rho_stat_t st;
    st.type = type;
    st.size = size;
    st.ino = rho_ino--;
    st.mtime = time;

    rho_stat_map.insert(std::make_pair(path, st));
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_nativeInitPath
  (JNIEnv *env, jclass, jstring root_path, jstring sqlite_journals_path, jstring apk_path, jstring shared_path)
{
    android_set_path(rho_cast<std::string>(env, root_path), rho_cast<std::string>(env, sqlite_journals_path), rho_cast<std::string>(env, shared_path));
    g_apk_path = (apk_path != NULL) ? rho_cast<std::string>(env, apk_path) : std::string();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_nativeInitLogPath
  (JNIEnv *env, jclass, jstring path)
{
    android_set_log_path(rho_cast<std::string>(env, path));
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_nativeInitAssetManager
  (JNIEnv *env, jclass, jobject jLocalAssetMgr)
{
    jAssetManager = env->NewGlobalRef(jLocalAssetMgr);
    pAssetManager = AAssetManager_fromJava(env, jAssetManager);

}

static std::string make_full_path(const char *path);
static std::string make_full_path(std::string const &path);

void copyFileFromAsset(const std::string& relPath)
{
    RHO_LOG("Copy asset: %s", relPath.c_str());
    std::string fullPath = make_full_path(relPath);

    AAsset* asset = AAssetManager_open(pAssetManager, relPath.c_str(), AASSET_MODE_STREAMING);
    if(asset == 0)
    {
        //TODO: error
        RHO_ERROR_LOG("Cannot open asset: %s", relPath.c_str());
        return;
    }


    FILE* dst;
    if (!(dst = fopen(fullPath.c_str(), "wbc")))
    {
        RHO_ERROR_LOG("Cannot open file: %s", fullPath.c_str());
        return;
    }

    unsigned int toCopy = AAsset_getLength(asset);
    unsigned int bufSize = 1 << 16;

    if(bufSize > toCopy) bufSize = toCopy;

    unsigned char* buf = new unsigned char[bufSize];

    while (toCopy > 0) {
        unsigned int portionSize = bufSize;
        if (toCopy < portionSize) {
            portionSize = toCopy;
        }
        portionSize = AAsset_read(asset, buf, portionSize);
        fwrite(buf, 1, portionSize, dst);

        toCopy -= portionSize;
    }

    AAsset_close(asset);

    fflush(dst);
    fclose(dst);

    delete[] buf;
    RHO_LOG("File has written: %s", fullPath.c_str());
}

void processStatTable(const char* statTablePath, RhoFsSetupMode setupMode, bool switchEmulationOn)
{
    struct stat st;
    memset(&st,0, sizeof(st));
    const char* buffer = 0;
    off_t size = 0;
    AAsset* asset = 0;
    if (stat(statTablePath, &st) == 0)
    {
        std::string fullStatTablePath = make_full_path(statTablePath);
        RHO_LOG("Opening stat table from file system: %s", fullStatTablePath.c_str());
        FILE* file = fopen(fullStatTablePath.c_str(), "rb");
        size = st.st_size;
        buffer = new char[size];
        size_t readSize = fread(const_cast<void*>(static_cast<const void*>(buffer)), 1, size, file);
        if(readSize < size)
        {
            RHO_ERROR_LOG("Cannot read stat table: %s", fullStatTablePath.c_str());
            fclose(file);
            return;
        }
        fclose(file);
    }
    else
    {
        RHO_LOG("Opening stat table from package: %s", statTablePath);
        asset = AAssetManager_open(pAssetManager, statTablePath, AASSET_MODE_BUFFER);
        if(asset == 0)
        {
            RHO_ERROR_LOG("Cannot open stat table: %s", statTablePath);
            return;
        }
        size = AAsset_getLength(asset);
        buffer = static_cast<const char*>(AAsset_getBuffer(asset));
        if(buffer == 0)
        {
            RHO_ERROR_LOG("Cannot read stat table: %s", statTablePath);
            AAsset_close(asset);
            return;
        }
    }

    std::string strPath;
    std::string strType;
    std::string strSize;
    std::string strTime;

    const char* pos;
    for(const char* ptr = buffer; ptr-buffer < size; ++ptr)
    {
        for (pos = ptr; *ptr != '|'; ++ptr);
        strPath.assign(pos, ptr-pos);

        for (pos = ++ptr; *ptr != '|'; ++ptr);
        strType.assign(pos, ptr-pos);

        for (pos = ++ptr; *ptr != '|'; ++ptr);
        strSize.assign(pos, ptr-pos);

        for (pos = ++ptr; *ptr != '\n'; ++ptr);
        strTime.assign(pos, ptr-pos);

        int type;
        size_t filesize = 0;
        unsigned long mtime;

        if(strType.compare("file") == 0)
        {
            type = rho_type_file;
            if(setupMode == RHO_FS_SETUP_FORCE_FILES)
            {
                copyFileFromAsset(strPath);
            }
        }
        else if(strType.compare("dir") == 0)
        {
            type = rho_type_dir;
            if(setupMode == RHO_FS_SETUP_FORCE_DIRECTORIES || setupMode == RHO_FS_SETUP_FORCE_FILES)
            {
                mkdir(strPath.c_str(), S_IRWXU);
            }
        }
        else
        {
            RHO_ERROR_LOG("Wrong file type at stat table: %s; %s", strType.c_str(), strPath.c_str());
            return;
        }

        sscanf(strSize.c_str(), "%zu", &filesize);
        sscanf(strTime.c_str(), "%u", &mtime);

        updateStatTable(strPath, static_cast<rho_fileapi_type_t>(type), filesize, mtime);

    }

    if(asset != 0)
    {
        AAsset_close(asset);
    }
    else
    {
        delete [] buffer;
    }

    if(switchEmulationOn)
    {
        rho_file_set_fs_mode(RHO_FS_TRANSPARRENT);
    }
}

RHO_GLOBAL void rho_file_android_process_stat_table(int setupMode)
{
    processStatTable("rho.dat", static_cast<RhoFsSetupMode>(setupMode), false);
}


RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_processStatTable
  (JNIEnv *env, jclass, jboolean jEmulate, jboolean jForceReset)
{
    processStatTable("rho.dat", static_cast<bool>(jForceReset)?RHO_FS_SETUP_FORCE_FILES:RHO_FS_SETUP_NONE, static_cast<bool>(jEmulate));
}

void forceStatMapElement(const rho_stat_map_t::value_type& element)
{
    static bool pendingError = false;
    if(pendingError) return;

    const std::string& path = element.first;
    const rho_stat_t& st = element.second;

    switch(st.type)
    {
    case rho_type_file:
        copyFileFromAsset(path);
        break;
    case rho_type_dir:
        RHO_LOG("Create folder: %s", make_full_path(path).c_str());
        mkdir(make_full_path(path).c_str(), S_IRWXU);
        break;
    default:
//        {
//            pendingError = true;
//            jclass clsRE = getJNIClass(RHODES_JAVA_CLASS_RUNTIME_EXCEPTION);
//            if (!clsRE) return;
//            env->ThrowNew(clsRE, "Unknown type of file");
//        }
        RHO_ERROR_LOG("Wrong file type: %s", path.c_str());
        return;
    }
}

 

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_nativeInit
  (JNIEnv *env, jclass)
{
    clsFileApi = getJNIClass(RHODES_JAVA_CLASS_RHOFILEAPI);
    if (!clsFileApi) return;
    midCopy = getJNIClassStaticMethod(env, clsFileApi, "copy", "(Ljava/lang/String;)Z");
    if (!midCopy) return;
    midOpen = getJNIClassStaticMethod(env, clsFileApi, "openInPackage", "(Ljava/lang/String;)Ljava/io/InputStream;");
    if (!midOpen) return;
    midForceAllFiles = getJNIClassStaticMethod(env, clsFileApi, "doForceAllFiles", "()V");
    if (!midForceAllFiles) return;
    midClose = getJNIClassStaticMethod(env, clsFileApi, "close", "(Ljava/io/InputStream;)V");
    if (!midClose) return;
    midRead = getJNIClassStaticMethod(env, clsFileApi, "read", "(Ljava/io/InputStream;[B)I");
    if (!midRead) return;
    midSeek = getJNIClassStaticMethod(env, clsFileApi, "seek", "(Ljava/io/InputStream;I)V");
    if (!midSeek) return;
    midGetChildren = getJNIClassStaticMethod(env, clsFileApi, "getChildren", "(Ljava/lang/String;)[Ljava/lang/String;");
    if (!midGetChildren) return;
    midReloadStatTable = getJNIClassStaticMethod(env, clsFileApi, "reloadStatTable", "()V");
    if (!midReloadStatTable) return;
    midDeleteRecursively = getJNIClassStaticMethod(env, clsFileApi, "deleteRecursively", "(Ljava/lang/String;)I");
    if (!midDeleteRecursively) return;
    // Disable fdsan to prevent crashes.
    if (android_fdsan_set_error_level) {
        android_fdsan_set_error_level(ANDROID_FDSAN_ERROR_LEVEL_WARN_ONCE);
    }
    // End disabling fdsan

#if  defined(__aarch64__)
    const char *libc = "/system/lib64/libc.so";
#else
    const char *libc = "/system/lib/libc.so";
#endif
    void *pc = dlopen(libc, RTLD_LAZY);
    if(!pc) {
        const char* e = dlerror();
        RHO_ERROR_LOG("dlopen return: %s", e);
        assert(false);
    }
    assert(pc != nullptr);
#ifndef __aarch64__
    __sflags = (func_sflags_t)dlsym(pc, "__sflags");
#endif
    __sfp = (func_sfp_t)dlsym(pc, "__sfp");
    real_access = (func_access_t)dlsym(pc, "access");
    real_close = (func_close_t)dlsym(pc, "close");
    real_dup = (func_dup_t)dlsym(pc, "dup");
    real_dup2 = (func_dup2_t)dlsym(pc, "dup2");
    real_chown = (func_chown_t)dlsym(pc, "chown");
    real_fchown = (func_fchown_t)dlsym(pc, "fchown");
    real_lchown = (func_lchown_t)dlsym(pc, "lchown");
    real_link = (func_link_t)dlsym(pc, "link");
    real_symlink = (func_symlink_t)dlsym(pc, "symlink");
    real_readlink = (func_readlink_t)dlsym(pc, "readlink");
    real_mkdir = (func_mkdir_t)dlsym(pc, "mkdir");
    real_fchdir = (func_fchdir_t)dlsym(pc, "fchdir");
    real_fcntl = (func_fcntl_t)dlsym(pc, "fcntl");
    real_ioctl = (func_ioctl_t)dlsym(pc, "ioctl");
    real_fsync = (func_fsync_t)dlsym(pc, "fsync");
    real_fdatasync = (func_fdatasync_t)dlsym(pc, "fdatasync");
    real_fread = (func_fread_t)dlsym(pc,"fread");
    real_fwrite = (func_fwrite_t)dlsym(pc,"fwrite");
    real_fseek = (func_fseek_t)dlsym(pc,"fseek");
    real_ftell = (func_ftell_t)dlsym(pc,"ftell");
    real_fclose = (func_fclose_t)dlsym(pc,"fclose");
    real_fopen = (func_fopen_t)dlsym(pc,"fopen");
    real_fopen64 = (func_fopen_t)dlsym(pc,"fopen64");
    if (real_fdatasync == NULL) {
        //Android 2.1 have no fdatasync call. Use fsync instead
        RHO_LOG("No fdatasync implementation, using fsync instead");
        real_fdatasync = real_fsync;
    }

    real_flock = (func_flock_t)dlsym(pc, "flock");
    real_fstat = (func_fstat_t)dlsym(pc, "fstat");
    real_ftruncate = (func_ftruncate_t)dlsym(pc, "ftruncate");
    real_ftruncate64 = (func_ftruncate64_t)dlsym(pc, "ftruncate64");
    real_lseek = (func_lseek_t)dlsym(pc, "lseek");
    real_lseek64 = (func_lseek64_t)dlsym(pc, "lseek64");
    real_lstat = (func_lstat_t)dlsym(pc, "lstat");
    real_open = (func_open_t)dlsym(pc, "open");
    real_open64 = (func_open_t)dlsym(pc, "open64");
    real_read = (func_read_t)dlsym(pc, "read");

    real_pread = (func_pread_t)dlsym(pc, "pread");
    real_pread64 = (func_pread_t)dlsym(pc, "pread64");
    real_pwrite = (func_pwrite_t)dlsym(pc, "pwrite");
    real_pwrite64 = (func_pwrite_t)dlsym(pc, "pwrite");
    real_sendfile = (func_sendfile_t)dlsym(pc, "sendfile");

    real_utime = (func_utime_t)dlsym(pc, "utime");
    real_utimes = (func_utimes_t)dlsym(pc, "utimes");
    real_futimes = (func_futimes_t)dlsym(pc, "futimes");
    real_pipe2 = (func_pipe2_t)dlsym(pc, "pipe2");
    real_pipe = (func_pipe_t)dlsym(pc, "pipe");

    real_select = (func_select_t)dlsym(pc, "select");
    real_stat = (func_stat_t)dlsym(pc, "stat");
    real_unlink = (func_unlink_t)dlsym(pc, "unlink");
    real_write = (func_write_t)dlsym(pc, "write");
    real_getdents = (func_getdents_t)dlsym(pc, "getdents");
    real_rmdir = (func_rmdir_t)dlsym(pc, "rmdir");
    real_opendir = (func_opendir_t)dlsym(pc, "opendir");
    real_fdopendir = (func_fdopendir_t)dlsym(pc, "fdopendir");
    real_readdir = (func_readdir_t)dlsym(pc, "readdir");
    real_readdir_r = (func_readdir_r_t)dlsym(pc, "readdir_r");
    real_closedir = (func_closedir_t)dlsym(pc, "closedir");
    real_rewinddir = (func_rewinddir_t)dlsym(pc, "rewinddir");
    real_dirfd = (func_dirfd_t)dlsym(pc, "dirfd");
    real_scandir = (func_scandir_t)dlsym(pc, "scandir");
    dlclose(pc);

    // This is just to get typical stat of file
    std::string librhodes = rho_root_path() + "../lib/librhodes.so";
    RHO_LOG("Native library: %s", librhodes.c_str());

    real_stat(librhodes.c_str(), &librhodes_st);
    librhodes_st.st_mode = S_IFREG|S_IRWXU;
    librhodes_st.st_nlink = 1;
    librhodes_st.st_uid = getuid();
    librhodes_st.st_gid = getgid();

    RHO_LOG("Library stat (mode: %d, uid: %d, gid: %d)", librhodes_st.st_mode, librhodes_st.st_uid, librhodes_st.st_gid);

    // fill stat structure by stat of rhoconfig.txt
    {
        std::string rhoconfig = rho_root_path() + "apps/rhoconfig.txt";
        if (-1 == real_stat(rhoconfig.c_str(), &librhodes_st) ) {
            RHO_ERROR_LOG("Cannot open RHOCONFIG.TXT file: %s", rhoconfig.c_str());
        }
        else {
            RHO_ERROR_LOG("base stat copied from RHOCONFIG.TXT file: %s", rhoconfig.c_str());
        }


    }


}


extern "C" void rho_sysimpl_remove_bundle_files(const char* rootPath, const char* listFileName);

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_removeBundleUpgrade
  (JNIEnv *env, jclass)
{
    rho_sysimpl_remove_bundle_files((rho_root_path() + "apps").c_str(), "rhofilelist.txt");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_setFsModeTransparrent
  (JNIEnv *env, jclass , jboolean transparrent)
{
    rho_file_set_fs_mode(transparrent ? RHO_FS_TRANSPARRENT : RHO_FS_DISK_ONLY);
}


static std::string normalize_path(std::string path)
{
    //RHO_LOG("normalize_path: (1): path: %s", path.c_str());

    bool has_leading_slash = !path.empty() && path[0] == '/';

    std::vector<std::string> parts;
    for (;;)
    {
        size_t idx = path.find('/');
        std::string part = path.substr(0, idx);
        if (part == "..")
        {
            //RHO_LOG("normalize_path: (2)");
            if (!parts.empty())
                parts.erase(parts.end() - 1);
            else
                parts.push_back(part);
        }
        else if (!part.empty() && part != ".")
        {
            //RHO_LOG("normalize_path: (3): part: %s", part.c_str());
            parts.push_back(part);
        }
        if (idx == std::string::npos)
            break;
        path = path.substr(idx + 1);
        //RHO_LOG("normalize_path: (4): path: %s", path.c_str());
    }

    path.clear();
    for (size_t i = 0, lim = parts.size(); i < lim; ++i)
    {
        path += "/";
        path += parts[i];
    }

    if (!has_leading_slash)
        path.erase(0, 1);

    //RHO_LOG("normalize_path: return %s", path.c_str());
    return path;
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_normalizePath
  (JNIEnv *env, jclass, jstring pathObj)
{
    std::string path = normalize_path(rho_cast<std::string>(env, pathObj));
    return rho_cast<jstring>(env, path);
}

static std::string make_full_path(const char *path)
{
    if (path == NULL || *path == '\0') {
        RHO_LOG("make_full_path(%s) = %s", path, "''");
        return "";
    }

    if (*path == '/') {
        std::string np = normalize_path(path);
        RHO_LOG("make_full_path(%s) = %s", path, np.c_str());
        return np;
    }
    //std::string fpath = rho_root_path() + "/" + path;
    std::string fpath = rho_cur_path() + "/" + path;

    std::string np = normalize_path(fpath);
    RHO_LOG("make_full_path(%s) = %s", path, np.c_str());
    return np;
}

static std::string make_full_path(std::string const &path)
{
    return make_full_path(path.c_str());
}

static std::string make_rel_path(std::string const &fpath)
{
    std::string const &root_path = rho_root_path();
    std::string const &root_path_real = rho_root_path_real();
    if (fpath.find(root_path) == 0) {
        return fpath.substr(root_path.size());
    }
    if (fpath.find(root_path_real) == 0) {
        return fpath.substr(root_path_real.size());
    }
    return fpath;
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_makeRelativePath
  (JNIEnv *env, jclass, jstring pathObj)
{
    std::string path = rho_cast<std::string>(env, pathObj);
    return rho_cast<jstring>(env, make_rel_path(make_full_path(path)));
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_absolutePath
  (JNIEnv *env, jclass, jstring pathObj)
{
    std::string path = rho_cast<std::string>(env, pathObj);
    return rho_cast<jstring>(env, make_full_path(path));
}

static rho_stat_t *rho_stat(const char *path)
{
    std::string relpath = make_rel_path(make_full_path(path));
    rho_stat_map_t::iterator it = rho_stat_map.find(relpath);

    rho_stat_t *result = NULL;

    if (it == rho_stat_map.end()) {
        result = NULL;
    }
    else {
        result =  &(it->second);
    }
    RHO_LOG("rho_stat(%s), relpath = %s, RESULT = %s", path, relpath.c_str(), (result==NULL)?"NONE":"FOUND");

    return result;
}

static rho_stat_t *rho_stat(std::string const &path)
{
    return rho_stat(path.c_str());
}

#if 0
static rho_stat_t *rho_stat(int fd)
{
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return NULL;
    scoped_lock_t guard(rho_file_mtx);
    rho_fd_map_t::iterator it = rho_fd_map.find(fd);
    if (it == rho_fd_map.end())
        return NULL;
    return rho_stat(make_rel_path(it->second.fpath));
}
#endif

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

inline bool need_emulate_dir(std::string const &path)
{
    if (rho_fs_mode != RHO_FS_TRANSPARRENT) return false;

    std::string fpath = make_full_path(path);
    std::string const &root_path = rho_root_path();
    std::string const &root_path_real = rho_root_path_real();

    //RHO_LOG("$$$$$ root_path = %s, root_path_real = %s", root_path.c_str(), root_path_real.c_str());


    bool result = false;

    // check for exist
    struct stat st;
    if (real_stat(fpath.c_str(), &st) == -1) {
        //RHO_LOG("$$$$$ need_emulate_dir(%s), full_path =%s", path.c_str(), fpath.c_str());
        if ((fpath.size() < root_path.size()) && (fpath.size() < root_path_real.size())) {

        }
        else {
            result = ((::strncmp(fpath.c_str(), root_path.c_str(), root_path.size()) == 0) || (::strncmp(fpath.c_str(), root_path_real.c_str(), root_path_real.size()) == 0));
        }
    }
    RESET_ERRNO
    RHO_LOG("need_emulate_dir(%s) = %s", path.c_str(), result?"true":"false");
    return result;
}

inline bool need_emulate_dir(const char *path)
{
    return path ? need_emulate_dir(std::string(path)) : false;
}

static bool need_emulate(std::string const &path)
{
    if (rho_fs_mode != RHO_FS_TRANSPARRENT) return false;

    std::string fpath = make_full_path(path);
    std::string const &root_path = rho_root_path();
    std::string const &root_path_real = rho_root_path_real();
    bool result = false;
    //RHO_LOG("$$$$$ need_emulate(%s), full_path =%s", path.c_str(), fpath.c_str());
    if ((::strncmp(fpath.c_str(), root_path.c_str(), root_path.size()) == 0) ||
        (::strncmp(fpath.c_str(), root_path_real.c_str(), root_path_real.size()) == 0))
    {
        //RHO_LOG("$$$$$ need_emulate(%s) $$$ 01", path.c_str());
        struct stat st;
        if (real_stat(fpath.c_str(), &st) == -1)
        {
            //RHO_LOG("$$$$$ need_emulate(%s) $$$ 02", path.c_str());
            if (errno == ENOENT)
            {
                //RHO_LOG("$$$$$ need_emulate(%s) $$$ 03", path.c_str());
                rho_stat_t *rst = rho_stat(fpath);
                result = (rst != NULL);
            }

        }
        RESET_ERRNO
    }
    RHO_LOG("$$$$$ need_emulate(%s) = %s", path.c_str(), result?"true":"false");

    return result;
}

inline bool need_emulate(const char *path)
{
    return path ? need_emulate(std::string(path)) : false;
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_file_RhoFileApi_needEmulate
  (JNIEnv *env, jclass, jstring pathObj)
{
    //RHO_LOG("Java_com_rhomobile_rhodes_file_RhoFileApi_needEmulate");
    std::string path = rho_cast<std::string>(env, pathObj);
    //RHO_LOG("Java_com_rhomobile_rhodes_file_RhoFileApi_needEmulate: %s", path.c_str());
    bool need = need_emulate(path);
    //RHO_LOG("Java_com_rhomobile_rhodes_file_RhoFileApi_needEmulate: need: %d", (int)need);
    return need;
}

RHO_GLOBAL int j_open(const char *path, int oflag, ...)
{
    RAWLOG_INFO("j_open is stub!!!");
    return -1;
}

RHO_GLOBAL int open64(const char *path, int oflag, ...)
{
    std::string fpath;
    if (path)
        fpath = path;

    if (fpath.empty())
    {
        RHO_LOG("open: path is empty");
        errno = EFAULT;
        return -1;
    }

    RHO_LOG("open: %s...", path);

    //scoped_lock_t guard(rho_file_mtx);

    fpath = make_full_path(fpath);
    RHO_LOG("open: %s: fpath: %s", path, fpath.c_str());

    // try to found from opened


    bool emulate = need_emulate(fpath);
    RHO_LOG("open: %s: emulate: %d", path, (int)emulate);
    if (emulate && has_pending_exception())
    {
        RHO_LOG("open: %s: has_pending_exception, return -1", path);
        errno = EFAULT;
        return -1;
    }
    if (emulate && (oflag & (O_WRONLY | O_RDWR)))
    //if (emulate)
    {
        RHO_LOG("open: %s: copy from Android package", path);
        JNIEnv *env = jnienv_fileapi();
        jhstring relPathObj = rho_cast<jstring>(env, make_rel_path(fpath));
        env->CallStaticBooleanMethod(clsFileApi, midCopy, relPathObj.get());
        if (has_pending_exception())
        {
            RHO_LOG("open: %s: has_pending_exception, return -1", path);
            errno = EFAULT;
            return -1;
        }

        emulate = false;
    }
    RHO_LOG("open2: %s: emulate: %d", path, (int)emulate);

    int fd = -1;
    if (emulate)
    {
        RHO_LOG("open3: %s: emulate", path);
        JNIEnv *env = jnienv_fileapi();
        jhstring relPathObj = rho_cast<jstring>(env, make_rel_path(fpath));
        jhobject is = env->CallStaticObjectMethod(clsFileApi, midOpen, relPathObj.get());

        if (!is)
        {
            errno = EFAULT;
            fd = -1;
        }
        else
        {
            scoped_lock_t guard(rho_file_mtx);
            if (!rho_fd_free.empty())
            {
                fd = rho_fd_free[0];
                rho_fd_free.erase(rho_fd_free.begin());
            }
            else
                fd = rho_fd_counter++;
            rho_fd_data_t d;
            d.type = rho_type_file;
            d.is = env->NewGlobalRef(is.get());
            d.dirp = NULL;
            d.fpath = fpath;
            d.pos = 0;
            d.refCount = 1;
            rho_fd_map[fd] = d;
        }
    }
    else
    {
        RHO_LOG("open64: %s: native", path);
        mode_t mode = 0;
        if (oflag & O_CREAT)
        {
            va_list vl;
            va_start(vl, oflag);
            mode = va_arg(vl, int);
            va_end(vl);
        }
        fd = real_open64(path, oflag, mode);
    }
    RHO_LOG("openRESULT: %s => %d", path, fd);
}

RHO_GLOBAL int open(const char *path, int oflag, ...)
{
    std::string fpath;
    if (path)
        fpath = path;

    if (fpath.empty())
    {
        RHO_LOG("open: path is empty");
        errno = EFAULT;
        return -1;
    }

    RHO_LOG("open: %s...", path);

    //scoped_lock_t guard(rho_file_mtx);

    fpath = make_full_path(fpath);
    RHO_LOG("open: %s: fpath: %s", path, fpath.c_str());

    // try to found from opened


    bool emulate = need_emulate(fpath);
    RHO_LOG("open: %s: emulate: %d", path, (int)emulate);
    if (emulate && has_pending_exception())
    {
        RHO_LOG("open: %s: has_pending_exception, return -1", path);
        errno = EFAULT;
        return -1;
    }
    if (emulate && (oflag & (O_WRONLY | O_RDWR)))
    //if (emulate)
    {
        RHO_LOG("open: %s: copy from Android package", path);
        JNIEnv *env = jnienv_fileapi();
        jhstring relPathObj = rho_cast<jstring>(env, make_rel_path(fpath));
        env->CallStaticBooleanMethod(clsFileApi, midCopy, relPathObj.get());
        if (has_pending_exception())
        {
            RHO_LOG("open: %s: has_pending_exception, return -1", path);
            errno = EFAULT;
            return -1;
        }

        emulate = false;
    }
    RHO_LOG("open2: %s: emulate: %d", path, (int)emulate);

    int fd = -1;
    if (emulate)
    {
        RHO_LOG("open3: %s: emulate", path);
        JNIEnv *env = jnienv_fileapi();
        jhstring relPathObj = rho_cast<jstring>(env, make_rel_path(fpath));
        jhobject is = env->CallStaticObjectMethod(clsFileApi, midOpen, relPathObj.get());

        if (!is)
        {
            errno = EFAULT;
            fd = -1;
        }
        else
        {
            scoped_lock_t guard(rho_file_mtx);
            if (!rho_fd_free.empty())
            {
                fd = rho_fd_free[0];
                rho_fd_free.erase(rho_fd_free.begin());
            }
            else
                fd = rho_fd_counter++;
            rho_fd_data_t d;
            d.type = rho_type_file;
            d.is = env->NewGlobalRef(is.get());
            d.dirp = NULL;
            d.fpath = fpath;
            d.pos = 0;
            d.refCount = 1;
            rho_fd_map[fd] = d;
        }
    }
    else
    {
        RHO_LOG("open: %s: native", path);
        mode_t mode = 0;
        if (oflag & O_CREAT)
        {
            va_list vl;
            va_start(vl, oflag);
            mode = va_arg(vl, int);
            va_end(vl);
        }
        fd = real_open(path, oflag, mode);
    }
    RHO_LOG("openRESULT: %s => %d", path, fd);
    
    return fd;
}

RHO_GLOBAL int creat(const char* path, mode_t mode)
{
    return open(path, O_CREAT|O_WRONLY|O_TRUNC, mode);
}

RHO_GLOBAL int fcntl(int fd, int command, ...)
{
    RHO_LOG("fcntl: fd %d, command: %d", fd, command);

    int r = -1;

    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
    {
        if ( ( command == F_SETLK ) || ( command == F_GETLK ) || ( command == F_SETLKW ) ) {
            va_list vl1;
            va_start(vl1,command);
            flock* pLock = va_arg(vl1, flock*);
            va_end(vl1);
            r = real_fcntl(fd, command, pLock);
        } else { 
            va_list vl;
            va_start(vl, command);
            int arg = va_arg(vl, int);
            va_end(vl);
            r = real_fcntl(fd, command, arg);
        }

        if ( r < 0 ) {
            RHO_LOG("fcntl: errno %d", errno);
        }

        return r;
    }

    if (has_pending_exception())
    {
        errno = EFAULT;
        return -1;
    }

    switch (command)
    {
    case F_DUPFD: RHO_LOG("fcntl: F_DUPFD"); break;
    case F_GETFD:
        {
            RHO_LOG("fcntl: F_GETFD");
            scoped_lock_t guard(rho_file_mtx);
            rho_fd_map_t::iterator it = rho_fd_map.find(fd);
            if (it == rho_fd_map.end())
            {
                RAWLOG_ERROR("fcntl: EBADF");
                errno = EBADF;
                return -1;
            }
            return FD_CLOEXEC;
        }
        break;
    case F_SETFD: RHO_LOG("fcntl: F_SETFD"); break;
    case F_GETFL: RHO_LOG("fcntl: F_GETFL"); break;
    case F_SETFL: RHO_LOG("fcntl: F_SETFL"); break;
    case F_SETLK: RHO_LOG("fcntl: F_SETLK"); break;
    case F_SETLKW: RHO_LOG("fcntl: F_SETLKW"); break;
    case F_GETLK: RHO_LOG("fcntl: F_GETLK"); break;
    case F_SETOWN: RHO_LOG("fcntl: F_SETOWN"); break;
    case F_GETOWN: RHO_LOG("fcntl: F_GETOWN"); break;
    case F_SETSIG: RHO_LOG("fcntl: F_SETSIG"); break;
    case F_GETSIG: RHO_LOG("fcntl: F_GETSIG"); break;
    default: RHO_LOG("fcntl: unknown command: %d", command);
    }

    RHO_NOT_IMPLEMENTED;
}

RHO_GLOBAL int close(int fd)
{
    RHO_LOG("close: BEGIN fd %d", fd);

    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE) {
        int result =  real_close(fd);
        RHO_LOG("close: fd %d", fd);
        return result;
    }

    if (has_pending_exception())
    {
        errno = EFAULT;
        RHO_LOG("close: ERROR 1 fd %d", fd);
        return -1;
    }

    jobject is = NULL;
    {
        scoped_lock_t guard(rho_file_mtx);

        rho_fd_map_t::iterator it = rho_fd_map.find(fd);
        if (it == rho_fd_map.end())
        {
            errno = EBADF;
            RAWLOG_ERROR1("close: ERROR 2 fd %d", fd);
            return -1;
        }

        if (it->second.type == rho_type_file)
            is = it->second.is;
        rho_fd_map.erase(it);
        rho_fd_free.push_back(fd);
    }

    if (is)
    {
        JNIEnv *env = jnienv_fileapi();
        env->CallStaticVoidMethod(clsFileApi, midClose, is);
        env->DeleteGlobalRef(is);
    }

    RHO_LOG("close: fd %d emulate", fd);
    return 0;
}

RHO_GLOBAL ssize_t pread64(int fd, void* buf, size_t count, off64_t offset)
{
     RHO_LOG("pread: BEGIN fd %d: offset: %ld: count: %ld", fd, (long)offset, (long)count);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
    {
        ssize_t ret = real_pread64(fd, buf, count, offset);
        RHO_LOG("pread: fd %d: offset: %ld: count: %ld: return %ld bytes (native)", fd, (long)offset, (long)count, (long)ret);
        return ret;
    }

    off_t saved_offset = lseek(fd, 0, SEEK_CUR);
    lseek(fd, offset, SEEK_SET);

    ssize_t result = read(fd, buf, count);

    lseek(fd, saved_offset, SEEK_SET);

    RHO_LOG("pread: fd %d: offset: %ld: count: %ld: return %ld bytes (rho): errno: %d", fd, (long)offset, (long)count, (long)result, (int)errno);

    return result;
}

RHO_GLOBAL ssize_t pread(int fd, void *buf, size_t count, off_t offset)
{
    RHO_LOG("pread: BEGIN fd %d: offset: %ld: count: %ld", fd, (long)offset, (long)count);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
    {
        ssize_t ret = real_pread(fd, buf, count, offset);
        RHO_LOG("pread: fd %d: offset: %ld: count: %ld: return %ld bytes (native)", fd, (long)offset, (long)count, (long)ret);
        return ret;
    }

    off_t saved_offset = lseek(fd, 0, SEEK_CUR);
    lseek(fd, offset, SEEK_SET);

    ssize_t result = read(fd, buf, count);

    lseek(fd, saved_offset, SEEK_SET);


    RHO_LOG("pread: fd %d: offset: %ld: count: %ld: return %ld bytes (rho): errno: %d", fd, (long)offset, (long)count, (long)result, (int)errno);

    return result;
}

RHO_GLOBAL ssize_t pwrite64(int fd, const void* buf, size_t count, off64_t offset)
{
    RHO_LOG("pwrite: BEGIN fd %d: offset: %ld: count: %ld", fd, (long)offset, (long)count);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
    {
        ssize_t ret = real_pwrite64(fd, buf, count, offset);
        RHO_LOG("pwrite: fd %d: offset: %ld: count: %ld: return %ld bytes (native)", fd, (long)offset, (long)count, (long)ret);
        return ret;
    }

    off_t saved_offset = lseek(fd, 0, SEEK_CUR);
    lseek(fd, offset, SEEK_SET);

    ssize_t result = write(fd, buf, count);

    lseek(fd, saved_offset, SEEK_SET);

    RHO_LOG("pwrite: fd %d: offset: %ld: count: %ld: return %ld bytes (rho)", fd, (long)offset, (long)count, (long)result);
    return result;
}

RHO_GLOBAL ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    RHO_LOG("pwrite: BEGIN fd %d: offset: %ld: count: %ld", fd, (long)offset, (long)count);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
    {
        ssize_t ret = real_pwrite(fd, buf, count, offset);
        RHO_LOG("pwrite: fd %d: offset: %ld: count: %ld: return %ld bytes (native)", fd, (long)offset, (long)count, (long)ret);
        return ret;
    }

    off_t saved_offset = lseek(fd, 0, SEEK_CUR);
    lseek(fd, offset, SEEK_SET);

    ssize_t result = write(fd, buf, count);

    lseek(fd, saved_offset, SEEK_SET);

    RHO_LOG("pwrite: fd %d: offset: %ld: count: %ld: return %ld bytes (rho)", fd, (long)offset, (long)count, (long)result);
    return result;
}


RHO_GLOBAL ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count)
{
    RHO_LOG("sendfile: BEGIN out_fd %d: in_fd: %d: count: %ld", out_fd, in_fd, (long)count);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || in_fd < RHO_FD_BASE)
    {
        ssize_t ret = real_sendfile(out_fd, in_fd, offset, count);
        RHO_LOG("sendfile: out_fd %d: in_fd: %d: count: %ld: return %ld bytes (native)", out_fd, in_fd, (long)count, (long)ret);
        return ret;
    }

    off_t saved_offset = lseek(in_fd, 0, SEEK_CUR);
    if (offset != NULL) {
        lseek(in_fd, *offset, SEEK_SET);
    }

    unsigned char* buf = (unsigned char*)malloc(count);

    ssize_t readed_count = read(in_fd, buf, count);
    ssize_t result = write(out_fd, buf, readed_count);

    free(buf);

    if (offset != NULL) {
        lseek(in_fd, saved_offset, SEEK_SET);
        *offset = (long)saved_offset + (long)result;
    }

    RHO_LOG("sendfile: out_fd %d: in_fd: %d: count: %ld: return %ld bytes (rho)", out_fd, in_fd, (long)count, (long)result);
    return result;

}

RHO_GLOBAL int utime(const char *filename, const struct utimbuf *times)
{
    std::string fpath;
    if (filename)
        fpath = filename;

    if (fpath.empty())
    {
        RHO_LOG("utime: path is empty");
        errno = EFAULT;
        return -1;
    }

    //RHO_LOG("utime: %s...", filename);

    fpath = make_full_path(fpath);
    //RHO_LOG("open: %s: fpath: %s", path, fpath.c_str());

    bool emulate = need_emulate(fpath);
    RHO_LOG("utime: %s: emulate: %d", filename, (int)emulate);
    if (emulate && has_pending_exception())
    {
        RHO_LOG("utime: %s: has_pending_exception, return -1", filename);
        errno = EFAULT;
        return -1;
    }

    if (emulate) {

    }
    else {
        return real_utime(filename, times);
    }

    return 0;
}

RHO_GLOBAL int utimes(const char *filename, const struct timeval times[2]) {
    std::string fpath;
    if (filename)
        fpath = filename;

    if (fpath.empty())
    {
        RHO_LOG("utimes: path is empty");
        errno = EFAULT;
        return -1;
    }

    //RHO_LOG("utime: %s...", filename);

    fpath = make_full_path(fpath);
    //RHO_LOG("open: %s: fpath: %s", path, fpath.c_str());

    bool emulate = need_emulate(fpath);
    RHO_LOG("utimes: %s: emulate: %d", filename, (int)emulate);
    if (emulate && has_pending_exception())
    {
        RHO_LOG("utimes: %s: has_pending_exception, return -1", filename);
        errno = EFAULT;
        return -1;
    }

    if (emulate) {

    }
    else {
        return real_utimes(filename, times);
    }

    return 0;
}

RHO_GLOBAL int futimes(int fd, const struct timeval tv[2]) {
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
    {
        int ret = real_futimes(fd, tv);
        RHO_LOG("futimes: fd %d: return %d (native)", fd, ret);
        return ret;
    }
    RHO_LOG("futimes: fd: %d: emulate UNIMPLEMENTED !!!", fd);
    return 0;
}

RHO_GLOBAL int pipe2(int pipefd[2], int flags)
{
    RHO_LOG("pipe2: BEGIN fd_read %d: : fd_write: $d", pipefd[0], pipefd[1]);
    //if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
    {
        int ret = real_pipe2(pipefd, flags);
        RHO_LOG("pipe2: fd_read %d: : fd_write: $d: return %d (native)", pipefd[0], pipefd[1], ret);
        return ret;
    }
    //RHO_LOG("futimes: fd: %d: emulate UNIMPLEMENTED !!!", fd);
    //return 0;
}

RHO_GLOBAL int pipe(int pipefd[2])
{
    RHO_LOG("pipe: BEGIN fd_read %d: : fd_write: $d", pipefd[0], pipefd[1]);
    //if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
    {
        int ret = real_pipe(pipefd);
        RHO_LOG("pipe2: fd_read %d: : fd_write: $d: return %d (native)", pipefd[0], pipefd[1], ret);
        return ret;
    }
    //RHO_LOG("futimes: fd: %d: emulate UNIMPLEMENTED !!!", fd);
    //return 0;
}


RHO_GLOBAL ssize_t read(int fd, void *buf, size_t count)
{
    RHO_LOG("read: fd %d, maxcount= %d", fd, (int)count);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
    {
        ssize_t ret = real_read(fd, buf, count);
        RHO_LOG("read: fd %d: return %ld bytes (native)", fd, (long)ret);
        return ret;
    }

    if (has_pending_exception())
    {
        errno = EFAULT;
        return -1;
    }

    //scoped_lock_t guard(rho_file_mtx);
    jobject is = NULL;
    {
        scoped_lock_t guard(rho_file_mtx);

        rho_fd_map_t::iterator it = rho_fd_map.find(fd);
        if (it == rho_fd_map.end())
        {
            errno = EBADF;
            RAWLOG_ERROR1("read: ERROR 2 fd %d", fd);
            return (ssize_t)-1;
        }

        is = it->second.is;
    }

    JNIEnv *env = jnienv_fileapi();

    jbyteArray array = env->NewByteArray(count);
    jint result = env->CallStaticIntMethod(clsFileApi, midRead, is, array);

    if (result > 0)
    {
        {
            scoped_lock_t guard(rho_file_mtx);
            rho_fd_map_t::iterator it = rho_fd_map.find(fd);
            if (it == rho_fd_map.end())
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
    RHO_LOG("read: fd %d: return %ld bytes (java)", fd, (long)result);
    return result;
}

RHO_GLOBAL ssize_t write(int fd, const void *buf, size_t count)
{
    RHO_LOG("write: BEGIN fd %d: count: %ld", fd, (long)count);

    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE) {
        ssize_t result = real_write(fd, buf, count);
        RHO_LOG("write: fd %d: count: %ld: result: %ld (native)", fd, (long)count, (long)result);
        return result;
    }

    errno = EBADF;

    RAWLOG_ERROR2("write: fd %d: count: %ld UNIMPLEMENTED !!!", fd, (long)count);

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

RHO_GLOBAL int fchdir(int fd)
{
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_fchdir(fd);

    RHO_NOT_IMPLEMENTED;
}

RHO_GLOBAL int mkdir(const char *path, mode_t mode)
{
    RHO_LOG("mkdir: path=%s, mode=0x%x", path, (int)mode);
    std::string fpath = make_full_path(path);
    RHO_LOG("mkdir: fpath=%s", fpath.c_str());
    return real_mkdir(fpath.c_str(), mode);
}

RHO_GLOBAL int rmdir(const char *path)
{
    RHO_LOG("rmdir: path=%s", path);
    if (need_emulate(path))
    {
        rho_stat_t *st = rho_stat(path);
        if (!st)
            errno = ENOENT;
        else if (st->type != rho_type_dir)
            errno = ENOTDIR;
        else
            errno = EACCES;
        return -1;
    }

    return real_rmdir(path);
}

RHO_GLOBAL int chroot(const char *)
{
    RHO_NOT_IMPLEMENTED;
}

RHO_GLOBAL int chown(const char *path, uid_t uid, gid_t gid)
{
    RHO_LOG("chown: path=%s, uid=%d, gid=%d", path, (int)uid, (int)gid);
    std::string fpath = make_full_path(path);
    if (!need_emulate(fpath))
        return real_chown(path, uid, gid);

    errno = EACCES;
    return -1;
}

RHO_GLOBAL int fchown(int fd, uid_t uid, gid_t gid)
{
    RHO_LOG("fchown: fd=%d, uid=%d, gid=%d", fd, (int)uid, (int)gid);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_fchown(fd, uid, gid);

    std::string fpath;

    {
        scoped_lock_t guard(rho_file_mtx);

        rho_fd_map_t::iterator it = rho_fd_map.find(fd);
        if (it == rho_fd_map.end())
        {
            RAWLOG_ERROR("fchown: EBADF");
            errno = EBADF;
            return -1;
        }

        fpath = it->second.fpath;
    }

    return chown(fpath.c_str(), uid, gid);
}

RHO_GLOBAL int lchown(const char *path, uid_t uid, gid_t gid)
{
    RHO_LOG("lchown: path=%s, uid=%d, gid=%d", path, (int)uid, (int)gid);
    std::string fpath = make_full_path(path);
    if (!need_emulate(fpath))
        return real_lchown(path, uid, gid);

    return chown(path, uid, gid);
}

RHO_GLOBAL int link(const char *src, const char *dst)
{
    RHO_LOG("link: src=%s, dst=%s", src, dst);
    std::string fsrc = make_full_path(src);
    if (need_emulate(fsrc))
    {
        RHO_LOG("link: %s: copy from Android package", src);
        JNIEnv *env = jnienv_fileapi();
        jhstring relPathObj = rho_cast<jstring>(env, make_rel_path(fsrc));
        env->CallStaticBooleanMethod(clsFileApi, midCopy, relPathObj.get());
        if (has_pending_exception())
        {
            errno = EFAULT;
            return -1;
        }
    }

    rho_stat_t *st = rho_stat(dst);
    if (st)
    {
        errno = EEXIST;
        return -1;
    }

    return real_link(src, dst);
}

RHO_GLOBAL int symlink(const char *src, const char *dst)
{
    RHO_LOG("symlink: src=%s, dst=%s", src, dst);
    std::string fsrc = make_full_path(src);
    if (need_emulate(fsrc))
    {
        RHO_LOG("symlink: %s: copy from Android package", src);
        JNIEnv *env = jnienv_fileapi();
        jhstring relPathObj = rho_cast<jstring>(env, make_rel_path(fsrc));
        env->CallStaticBooleanMethod(clsFileApi, midCopy, relPathObj.get());
        if (has_pending_exception())
        {
            errno = EFAULT;
            return -1;
        }
    }

    rho_stat_t *st = rho_stat(dst);
    if (st)
    {
        errno = EEXIST;
        return -1;
    }

    return real_symlink(src, dst);
}

#if defined(__aarch64__)
RHO_GLOBAL ssize_t readlink(const char *path, char *buf, size_t bufsize)
#else
RHO_GLOBAL int readlink(const char *path, char *buf, size_t bufsize)
#endif
{
    RHO_LOG("readlink: path=%s", path);
    if (!need_emulate(path))
        return real_readlink(path, buf, bufsize);

    errno = EINVAL;
    return -1;
}

RHO_GLOBAL loff_t lseek64(int fd, loff_t offset, int whence)
{
    RHO_LOG("lseek64: fd %d, offset %lld, whence %d", fd, (long long)offset, whence);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
    {
        loff_t ret = real_lseek64(fd, offset, whence);
        RHO_LOG("lseek64: fd %d: return %llu (native)", fd, (unsigned long long)ret);
        return ret;
    }

    if (has_pending_exception())
    {
        errno = EFAULT;
        return -1;
    }

    jobject is = NULL;
    loff_t pos = 0;

    RHO_LOG("lseek64: fd %d: java", fd);

    //scoped_lock_t guard(rho_file_mtx);
    {
        scoped_lock_t guard(rho_file_mtx);

        rho_fd_map_t::iterator it = rho_fd_map.find(fd);
        if (it == rho_fd_map.end())
        {
            RAWLOG_ERROR("lseek64: EBADF");
            errno = EBADF;
            return -1;
        }

        rho_stat_t *st = rho_stat(make_rel_path(it->second.fpath));
        if (!st || st->type != rho_type_file)
        {
            RAWLOG_ERROR("lseek64: EBADF");
            errno = EBADF;
            return -1;
        }

        is = it->second.is;
        pos = it->second.pos;

        RHO_LOG("lseek64: fd %d: pos=%lld, size=%llu", fd, (long long)pos, (unsigned long long)st->size);
        switch (whence)
        {
        case SEEK_SET:
            if (offset < 0)
                offset = 0;
            if (offset > st->size)
                offset = st->size;
            pos = offset;
            RHO_LOG("lseek64: fd %d: SEEK_SET: pos=%lld", fd, (long long)pos);
            break;
        case SEEK_CUR:
            if (pos + offset < 0)
                offset = -pos;
            if (pos + offset > st->size)
                offset = st->size - pos;
            pos += offset;
            RHO_LOG("lseek64: fd %d: SEEK_CUR: pos=%lld", fd, (long long)pos);
            break;
        case SEEK_END:
            if (offset > 0)
                offset = 0;
            if (-offset > st->size)
                offset = -st->size;
            pos = st->size + offset;
            RHO_LOG("lseek64: fd %d: SEEK_END: pos=%lld", fd, (long long)pos);
            break;
        default:
            RHO_LOG("lseek64: fd %d: unknown whence value", fd);
            errno = EINVAL;
            return -1;
        }

        it->second.pos = pos;
    }

    RHO_LOG("lseek64: fd %d: is: %p, pos: %lld", fd, is, (long long)pos);

    if (is == NULL)
    {
        RAWLOG_ERROR1("lseek64: fd %d: return EBADF", fd);
        errno = EBADF;
        return -1;
    }

    JNIEnv *env = jnienv_fileapi();
    env->CallStaticVoidMethod(clsFileApi, midSeek, is, pos);

    RHO_LOG("lseek64: fd %d: return %lld (java)", fd, (long long)pos);
    return pos;
}

RHO_GLOBAL off_t lseek(int fd, off_t offset, int whence)
{
    RHO_LOG("lseek: fd %d", fd);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_lseek(fd, offset, whence);

    return lseek64(fd, offset, whence);
}

RHO_GLOBAL int dup(int fd)
{
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_dup(fd);

    RHO_NOT_IMPLEMENTED;
}

RHO_GLOBAL int dup2(int fd, int fd2)
{
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_dup2(fd, fd2);

    RHO_NOT_IMPLEMENTED;
}

RHO_GLOBAL int flock(int fd, int operation)
{
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_flock(fd, operation);

    RHO_NOT_IMPLEMENTED;
}

RHO_GLOBAL int tau_ioctl(int fd, unsigned long request, void* p) {
    return real_ioctl(fd, request, p);
}

RHO_GLOBAL int ioctl(int fd, int command, ...)
{
     RHO_LOG("ioctl: fd %d, command: %d", fd, command);
     if ((rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE) && (real_ioctl!=0) )
     {
         va_list vl;
         va_start(vl, command);
         int arg = va_arg(vl, int);
         va_end(vl);
         return real_ioctl(fd, command, arg);
     }

     RHO_NOT_IMPLEMENTED;
}


RHO_GLOBAL int fsync(int fd)
{
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_fsync(fd);

    RHO_LOG("fsync: fd %d", fd);
    errno = EINVAL;
    return -1;
}

RHO_GLOBAL int fdatasync(int fd)
{
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_fdatasync(fd);

    RHO_NOT_IMPLEMENTED;
}

RHO_GLOBAL int ftruncate64(int fd, off64_t offset)
{
    RHO_LOG("ftruncate64: fd %d", fd);
    if (offset < 0)
    {
        errno = EINVAL;
        return -1;
    }

    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE) {
        int res = real_ftruncate64(fd, offset);
        return res;
    }

    errno = EINVAL;
    return -1;
}

RHO_GLOBAL FILE* fdopen(int __fd, const char* __mode)
{
    RAWLOG_ERROR("fdopen stub");
    return NULL;
}

int dup3(int __old_fd, int __new_fd, int __flags)
{
    RAWLOG_ERROR("dup3 stub");
    return -1;
}


RHO_GLOBAL int ftruncate(int fd, off_t offset)
{
    RHO_LOG("ftruncate: fd %d", fd);
    if (offset < 0)
    {
        errno = EINVAL;
        return -1;
    }

    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE) {
        int res = real_ftruncate(fd, offset);
        return res;
    }

    errno = EINVAL;
    return -1;
}

static int stat_impl(std::string const &fpath, struct stat *buf)
{
    RHO_LOG("stat_impl: %s", fpath.c_str());

    if (has_pending_exception())
    {
        errno = EFAULT;
        return -1;
    }

    rho_stat_t *rst = rho_stat(fpath);
    if (!rst)
    {
        errno = ENOENT;
        return -1;
    }

    memcpy(buf, &librhodes_st, sizeof(struct stat));
    buf->st_mode = S_IRUSR|S_IWUSR;
    switch (rst->type)
    {
    case rho_type_file:
        buf->st_mode |= S_IFREG;
        break;
    case rho_type_dir:
        buf->st_mode |= S_IFDIR|S_IXUSR;
        break;
    default:
        RAWLOG_ERROR("stat_impl: EBADF");
        errno = EBADF;
        return -1;
    }
    buf->st_size = rst->size;
    buf->st_ino = rst->ino;
    time_t tm = rst->mtime;
    buf->st_atime = tm;
    buf->st_mtime = tm;
    buf->st_ctime = tm;

    RHO_LOG("stat_impl: %s: size %lu, mtime: %lu", fpath.c_str(), (unsigned long)rst->size, rst->mtime);

    return 0;
}

RHO_GLOBAL int stat(const char *path, struct stat *buf)
{
    RHO_LOG("stat: %s", path);
    std::string fpath = make_full_path(path);

    /* fail immediately if found invalid symbol */
    if (fpath.find(':')!=std::string::npos)
    {
        return -1;
    }

    if (!need_emulate(fpath))
    {
        int e = real_stat(path, buf);
        RHO_LOG("stat: %s: native, return %d (errno %d)", path, e, errno);
        return e;
    }

    return stat_impl(fpath, buf);
}

RHO_GLOBAL int fstat(int fd, struct stat *buf)
{
    RHO_LOG("fstat: fd %d", fd);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_fstat(fd, buf);

    std::string fpath;

    {
        scoped_lock_t guard(rho_file_mtx);

        rho_fd_map_t::iterator it = rho_fd_map.find(fd);
        if (it == rho_fd_map.end())
        {
            RAWLOG_ERROR("fstat: EBADF");
            errno = EBADF;
            return -1;
        }

        fpath = it->second.fpath;
    }

    return stat_impl(fpath, buf);
}

RHO_GLOBAL int lstat(const char *path, struct stat *buf)
{
    RHO_LOG("lstat: %s", path);
    std::string fpath = make_full_path(path);

    /* fail immediately if found invalid symbol */
    if (fpath.find(':')!=std::string::npos)
    {
        return -1;
    }

    if (!need_emulate(fpath))
        return real_lstat(path, buf);

    return stat_impl(fpath, buf);
}

RHO_GLOBAL int unlink(const char *path)
{
    RHO_LOG("unlink: %s", path);
    std::string fpath = make_full_path(path);
    if (!need_emulate(fpath))
        return real_unlink(path);

    // Check is there file with specified name in java package
    struct stat st;
    int e = stat_impl(fpath, &st);
    // If no such file in java package, then call real "unlink" function
    if (e == -1 && errno == ENOENT)
        return real_unlink(path);

    // Otherwise, return permission error (because there is no way remove
    // file from assets package
    errno = EPERM;
    return -1;
}
/*
static int __sread(void *cookie, char *buf, int n)
{
    RHO_LOG("__sread: %p", cookie);

    //scoped_lock_t guard(rho_file_mtx);

    FILE *fp = (FILE*)cookie;
    int ret;

    ret = read(fp->_file, buf, n);
    if (ret >= 0)
        fp->_offset += ret;
    else
        fp->_flags &= ~__SOFF;
    return ret;
}

static int __swrite(void *cookie, const char *buf, int n)
{
    RHO_LOG("__swrite: %p", cookie);

    //scoped_lock_t guard(rho_file_mtx);

    FILE *fp = (FILE*)cookie;

    if (fp->_flags & __SAPP)
        lseek(fp->_file, (off_t)0, SEEK_END);
    fp->_flags &= ~__SOFF;
    return write(fp->_file, buf, n);
}

static fpos_t __sseek(void *cookie, fpos_t offset, int whence)
{
    RHO_LOG("__sseek: %p", cookie);

    //scoped_lock_t guard(rho_file_mtx);

    FILE *fp = (FILE*)cookie;
    off_t ret = lseek(fp->_file, (off_t)offset, whence);
    if (ret == (off_t)-1)
        fp->_flags &= ~__SOFF;
    else
    {
        fp->_flags |= __SOFF;
        fp->_offset = ret;
    }
    return ret;
}

static int __sclose(void *cookie)
{
    RHO_LOG("__sclose: %p", cookie);
    return close(((FILE *)cookie)->_file);
}
*/


static int android_read(void* cookie, char* buf, int size) {
    return AAsset_read((AAsset*)cookie, buf, size);
}

static int android_write(void* cookie, const char* buf, int size) {
    return EACCES; // can't provide write access to the apk
}

static fpos_t android_seek(void* cookie, fpos_t offset, int whence) {
    return AAsset_seek((AAsset*)cookie, offset, whence);
}

static int android_close(void* cookie) {
    AAsset_close((AAsset*)cookie);
    return 0;
}

FILE* android_fopen(const char* fname, const char* mode) {
    if(mode[0] == 'w') return NULL;

    AAsset* asset = AAssetManager_open(pAssetManager, fname, 0);
    if(!asset) return NULL;

    return funopen(asset, android_read, android_write, android_seek, android_close);
}

#ifdef __aarch64__
int
__sflags(const char *mode, int *optr)
{
    int ret, m, o;
    switch (*mode++) {
        case 'r':	/* open for reading */
            //ret = __SRD;
            ret = 1;
            m = O_RDONLY;
            o = 0;
            break;
        case 'w':	/* open for writing */
            //ret = __SWR;
            ret = 1;
            m = O_WRONLY;
            o = O_CREAT | O_TRUNC;
            break;
        case 'a':	/* open for appending */
            //ret = __SWR;
            ret = 1;
            m = O_WRONLY;
            o = O_CREAT | O_APPEND;
            break;
        default:	/* illegal mode */
            errno = EINVAL;
            return (0);
    }
    /* [rwa]\+ or [rwa]b\+ means read and write */
    if (*mode == '+' || (*mode == 'b' && mode[1] == '+')) {
        //ret = __SRW;
        ret = 1;
        m = O_RDWR;
    }
    *optr = m | o;
    return (ret);
}
#endif

RHO_GLOBAL FILE *fopen64(const char *path, const char *mode)
{
    int flags, oflags;
    FILE *fp = 0;

    RHO_LOG("fopen: %s (%s)", path, mode);

    if ((flags = __sflags(mode, &oflags)) == 0) return NULL;
    rho::String relpath = make_rel_path(make_full_path(path));
    fp = real_fopen64(path, mode);

    if (fp == NULL) {
        fp = real_fopen64(relpath.c_str(), mode);
    }

    if (fp == NULL){
        fp = android_fopen(path, mode);
        if (fp == NULL){
            fp = android_fopen(relpath.c_str(), mode);
        }
    }
    // Do seek at our level as well even though oflags passed to open
    if (oflags & O_APPEND)
        fseek(fp, (fpos_t)0, SEEK_END);


    return fp;
}

RHO_GLOBAL FILE *fopen(const char *path, const char *mode)
{
    int flags, oflags;
    FILE *fp = 0;

    RHO_LOG("fopen: %s (%s)", path, mode);

    if ((flags = __sflags(mode, &oflags)) == 0) return NULL;
    rho::String relpath = make_rel_path(make_full_path(path));
    fp = real_fopen(path, mode);

    if (fp == NULL) {
        fp = real_fopen(relpath.c_str(), mode);
    }

    if (fp == NULL){
        fp = android_fopen(path, mode);
        if (fp == NULL){
            fp = android_fopen(relpath.c_str(), mode);
        }
    }
    // Do seek at our level as well even though oflags passed to open
    if (oflags & O_APPEND)
        fseek(fp, (fpos_t)0, SEEK_END);


    return fp;
}

size_t fread(void* __buf, size_t __size, size_t __count, FILE* __fp)
{
    int fd = fileno(__fp);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_fread(__buf, __size, __count, __fp);
    return read(fd,__buf,__size*__count);
}

size_t fwrite(const void* __buf, size_t __size, size_t __count, FILE* __fp)
{
    int fd = fileno(__fp);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_fwrite(__buf, __size, __count, __fp);
    return write(fd,__buf,__size*__count);
}

int fseek(FILE* __fp, long __offset, int __whence)
{
    int fd = fileno(__fp);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_fseek(__fp, __offset, __whence);
    return lseek(fd, __offset, __whence);
}

long ftell(FILE* __fp)
{
    //if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
    return real_ftell(__fp);
   // return tell(fd);
}

int fclose(FILE* __fp)
{
    int fd = fileno(__fp);
    if (rho_fs_mode == RHO_FS_DISK_ONLY || fd < RHO_FD_BASE)
        return real_fclose(__fp);
    return close(fd);
}

RHO_GLOBAL int select(int maxfd, fd_set *rfd, fd_set *wfd, fd_set *efd, struct timeval *tv)
{
    RHO_LOG("select: maxfd: %d", maxfd);
    int count = 0;
    if (rho_fs_mode == RHO_FS_TRANSPARRENT)
    {
        if (rfd)
        {
            fd_set fds;
            FD_ZERO(&fds);
            for (int i = RHO_FD_BASE; i < maxfd; ++i)
            {
             if (FD_ISSET(i, rfd))
                {

                 FD_SET(i, &fds);
                    ++count;


                }
            }
            if (count > 0)
          {

                RHO_LOG("select: return %d (rho)", count);
                memmove(rfd, &fds, sizeof(fds));
                if (wfd)
                    FD_ZERO(wfd);
                if (efd)
                    FD_ZERO(efd);
                return count;


            }
        }
    }

    count = real_select(maxfd, rfd, wfd, efd, tv);
    RHO_LOG("select: return %d (native)", count);
    return count;
}
RHO_GLOBAL int getdents(unsigned int fd, struct dirent *dirp, unsigned int count)
{
    RHO_LOG("getdents: fd=%u, dirp=%p, count=%u", fd, (void*)dirp, count);
    return real_getdents(fd, dirp, count);
}

RHO_GLOBAL DIR *opendir(const char *dirpath)
{
    std::string fpath;
    if (dirpath)
        fpath = make_full_path(dirpath);

    if (fpath.empty())
    {
        RHO_LOG("opendir: dirpath is empty");
        errno = ENOENT;
        return NULL;
    }

    RHO_LOG("opendir: dirpath=%s", dirpath);

    //scoped_lock_t guard(rho_file_mtx);

    bool emulate = need_emulate_dir(fpath);
    if (emulate && has_pending_exception())
    {
        errno = EFAULT;
        return NULL;
    }

    DIR *dirp = NULL;
    if (emulate)
    {
        {
            struct stat st;
            if (::stat(fpath.c_str(), &st) == -1)
                return NULL;
            if (!S_ISDIR(st.st_mode))
            {
                errno = ENOTDIR;
                return NULL;
            }
        }
        std::vector<std::string> children;
        children.push_back(".");
        children.push_back("..");
        {
            JNIEnv *env = jnienv_fileapi();
            jhstring relPathObj = rho_cast<jstring>(env, make_rel_path(fpath));
            jharray jChildren = static_cast<jobjectArray>(env->CallStaticObjectMethod(clsFileApi, midGetChildren, relPathObj.get()));

            if (!jChildren)
            {
                errno = EFAULT;
                return NULL;
            }

            for (jsize i = 0, lim = env->GetArrayLength(jChildren.get()); i != lim; ++i)
            {
                jhstring jc = static_cast<jstring>(env->GetObjectArrayElement(jChildren.get(), i));
                std::string const &ch = rho_cast<std::string>(env, jc);
                RHO_LOG("opendir: next children: %s", ch.c_str());
                children.push_back(ch);
            }
        }

        // Allocate dirp
        scoped_lock_t guard(rho_file_mtx);
        if (!rho_dir_free.empty())
        {
            dirp = rho_dir_free[0];
            rho_dir_free.erase(rho_dir_free.begin());
        }
        else
            dirp = reinterpret_cast<DIR *>(rho_dir_counter--);

        // Allocate fd
        int fd;
        if (!rho_fd_free.empty())
        {
            fd = rho_fd_free[0];
            rho_fd_free.erase(rho_fd_free.begin());
        }
        else
            fd = rho_fd_counter++;
        rho_fd_data_t d;
        d.type = rho_type_dir;
        d.is = NULL;
        d.dirp = dirp;
        d.fpath = fpath;
        d.pos = 0;
        rho_fd_map[fd] = d;

        // Collect children
        rho_dir_data_t dd;
        dd.fd = fd;
        for (std::vector<std::string>::const_iterator it = children.begin(), lim = children.end(); it != lim; ++it)
        {
            std::string cpath = fpath + "/" + *it;
            struct stat st;
            if (::stat(cpath.c_str(), &st) == -1)
                return NULL;

            struct dirent de;
            de.d_ino = st.st_ino;
            de.d_off = 0;
            de.d_reclen = sizeof(struct dirent);
            if (S_ISDIR(st.st_mode))
                de.d_type = DT_DIR;
            else if (S_ISREG(st.st_mode))
                de.d_type = DT_REG;
            else
                de.d_type = DT_UNKNOWN;
            strlcpy(de.d_name, it->c_str(), sizeof(de.d_name));
            dd.childs.push_back(de);
        }
        rho_dir_map[dirp] = dd;

        RHO_LOG("opendir: return %p (emulated), fd %d", (void*)dirp, fd);
    }
    else
    {
        dirp = real_opendir(dirpath);
        RHO_LOG("opendir: return %p (native)", (void*)dirp);
    }

    return dirp;
}

RHO_GLOBAL DIR *fdopendir(int fd)
{
    RHO_LOG("fdopendir: fd=%d", fd);
    if (rho_fs_mode == RHO_FS_DISK_ONLY  || fd < RHO_FD_BASE)
        return real_fdopendir(fd);

    scoped_lock_t guard(rho_file_mtx);

    for (rho_dir_map_t::const_iterator it = rho_dir_map.begin(), lim = rho_dir_map.end(); it != lim; ++it)
    {
        if (it->second.fd != fd)
            continue;

        return it->first;
    }

    errno = EBADF;
    RAWLOG_ERROR("fdopendir: EBADF");
    return NULL;
}

RHO_GLOBAL int dirfd(DIR *dirp)
{
    RHO_LOG("dirfd: dirp=%p", (void*)dirp);

    scoped_lock_t guard(rho_file_mtx);

    rho_dir_map_t::const_iterator it = rho_dir_map.find(dirp);
    if (it == rho_dir_map.end())
        return real_dirfd(dirp);

    return it->second.fd;
}

RHO_GLOBAL struct dirent *readdir(DIR *dirp)
{
    RHO_LOG("readdir: dirp=%p", (void*)dirp);

    scoped_lock_t guard(rho_file_mtx);

    rho_dir_map_t::iterator it = rho_dir_map.find(dirp);
    if (it == rho_dir_map.end())
    {
        struct dirent *ret = real_readdir(dirp);
        RHO_LOG("readdir: return %p (native)", (void*)ret);
        return ret;
    }

    rho_dir_data_t &d = it->second;
    if (d.index >= d.childs.size())
    {
        RHO_LOG("readdir: end of directory reached, return NULL");
        return NULL;
    }

    struct dirent *ret = &d.childs[d.index++];
    ret->d_off = ret->d_reclen;
    RHO_LOG("readdir: return %p (emulated)", (void*)ret);
    return ret;
}

RHO_GLOBAL int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)
{
    RHO_LOG("readdir_r: dirp=%p", (void*)dirp);

    scoped_lock_t guard(rho_file_mtx);

    rho_dir_map_t::iterator it = rho_dir_map.find(dirp);
    if (it == rho_dir_map.end())
    {
        int ret = real_readdir_r(dirp, entry, result);
        RHO_LOG("readdir_r: return %p (native)", (void*)*result);
        return ret;
    }

    rho_dir_data_t &d = it->second;
    if (d.index >= d.childs.size())
    {
        RHO_LOG("readdir_r: end of directory reached, return NULL");
        *result = NULL;
        return 0;
    }

    struct dirent &de = d.childs[d.index++];
    de.d_off = de.d_reclen;
    entry->d_ino = de.d_ino;
    entry->d_off = de.d_off;
    entry->d_reclen = de.d_reclen;
    entry->d_type = de.d_type;
    ::strlcpy(entry->d_name, de.d_name, sizeof(entry->d_name));

    *result = entry;

    RHO_LOG("readdir_r: return %p (emulated)", (void*)*result);
    return 0;
}

RHO_GLOBAL int closedir(DIR *dirp)
{
    RHO_LOG("closedir: dirp=%p", (void*)dirp);

    scoped_lock_t guard(rho_file_mtx);

    rho_dir_map_t::iterator it = rho_dir_map.find(dirp);
    if (it == rho_dir_map.end())
    {
        int ret = real_closedir(dirp);
        RHO_LOG("closedir: return %d (native)", ret);
        return ret;
    }

    rho_fd_map_t::iterator itt = rho_fd_map.find(it->second.fd);
    if (itt == rho_fd_map.end())
    {
        RAWLOG_ERROR("closedir: EBADF");
        errno = EBADF;
        return -1;
    }

    rho_fd_map.erase(itt);
    rho_fd_free.push_back(it->second.fd);

    rho_dir_map.erase(it);
    rho_dir_free.push_back(dirp);
    RHO_LOG("closedir: return 0 (emulated)");
    return 0;
}

RHO_GLOBAL void seekdir(DIR *dirp, long offset)
{
    RHO_LOG("seekdir: dirp=%p, offset=%ld", (void*)dirp, offset);

    scoped_lock_t guard(rho_file_mtx);

    rho_dir_map_t::iterator it = rho_dir_map.find(dirp);
    if (it == rho_dir_map.end())
        return;

    rho_dir_data_t &d = it->second;
    d.index = 0;
    for (std::vector<struct dirent>::iterator itt = d.childs.begin(), limm = d.childs.end(); itt != limm; ++itt, ++d.index)
    {
        if (offset >= itt->d_reclen)
        {
            offset -= itt->d_reclen;
            itt->d_off = itt->d_reclen;
            continue;
        }
        itt->d_off = offset;
        break;
    }
}

RHO_GLOBAL long telldir(DIR *dirp)
{
    RHO_LOG("telldir: dirp=%p", (void*)dirp);

    scoped_lock_t guard(rho_file_mtx);

    rho_dir_map_t::iterator it = rho_dir_map.find(dirp);
    if (it == rho_dir_map.end())
        return 0;

    rho_dir_data_t &d = it->second;
    long offset = 0;
    for (std::vector<struct dirent>::const_iterator itt = d.childs.begin(), limm = d.childs.end(); itt != limm; ++itt)
    {
        offset += itt->d_off;
        if (itt->d_off < itt->d_reclen)
            break;
    }

    return offset;
}

RHO_GLOBAL void rewinddir(DIR *dirp)
{
    RHO_LOG("rewinddir: dirp=%p", (void*)dirp);

    scoped_lock_t guard(rho_file_mtx);

    rho_dir_map_t::iterator it = rho_dir_map.find(dirp);
    if (it == rho_dir_map.end())
        real_rewinddir(dirp);
    else
    {
        rho_dir_data_t &d = it->second;
        d.index = 0;
        if (d.childs.size() > 0)
            d.childs[0].d_off = 0;
    }
}










int scandir_emulate(       const char *dirname,
	                       struct dirent ***namelist,
	                       int (*select) (const struct dirent *),
	                       int (*dcomp) (const struct dirent **, const struct dirent **) )
{
	register struct dirent *d, *p, **names;
	register size_t nitems;
	struct stat stb;
	long arraysz;
	DIR *dirp;
	int successful = 0;
	int rc = 0;
	dirp = NULL;
	names = NULL;
	if ((dirp = opendir(dirname)) == NULL) {
        RHO_LOG("@@@@@ scandir_emulate 01 ");
		return(-1);
    }
	if (fstat(dirfd(dirp), &stb) < 0) {
        RHO_LOG("@@@@@ scandir_emulate 02 ");
		goto cleanup;
    }
	/*
 	 * If there were no directory entries, then bail.
 	 */
	if (stb.st_size == 0) {
        RHO_LOG("@@@@@ scandir_emulate 03 ");
		goto cleanup;
    }
	/*
	 * estimate the array size by taking the size of the directory file
	 * and dividing it by a multiple of the minimum size entry.
	 */
	arraysz = (stb.st_size / 6);
	names = (struct dirent **)malloc(arraysz * sizeof(struct dirent *));
	if (names == NULL) {
        RHO_LOG("@@@@@ scandir_emulate 04 ");
		goto cleanup;
    }
	nitems = 0;
	while ((d = readdir(dirp)) != NULL) {
		if (select != NULL && !(*select)(d))
			continue;	/* just selected names */
		/*
		 * Make a minimum size copy of the data
		 */
		p = (struct dirent *)malloc(sizeof (struct dirent) + 2048);
		if (p == NULL) {
            RHO_LOG("@@@@@ scandir_emulate 05 ");
			goto cleanup;
        }
		p->d_ino = d->d_ino;
		p->d_reclen = d->d_reclen;
#ifdef _DIRENT_HAVE_D_NAMLEN
		p->d_namlen = d->d_namlen;
		bcopy(d->d_name, p->d_name, p->d_namlen + 1);
#else
               strcpy(p->d_name, d->d_name);
#endif
		/*
		 * Check to make sure the array has space left and
		 * realloc the maximum size.
		 */
		if (++nitems >= arraysz) {
            RHO_LOG("@@@@@ scandir_emulate 06 ");

			if (fstat(dirfd(dirp), &stb) < 0) {
                RHO_LOG("@@@@@ scandir_emulate 07 ");
				goto cleanup;
            }
			arraysz = stb.st_size / 12;
			names = (struct dirent **)realloc((char *)names,
				arraysz * sizeof(struct dirent *));
			if (names == NULL) {
                RHO_LOG("@@@@@ scandir_emulate 08 ");
				goto cleanup;
            }
		}
		names[nitems-1] = p;
	}
	successful = 1;
cleanup:
	closedir(dirp);
	if (successful) {
        RHO_LOG("@@@@@ scandir_emulate 09 ");

		if (nitems && dcomp != NULL) {
            RHO_LOG("@@@@@ scandir_emulate 10 ");
			qsort(names, nitems, sizeof(struct dirent *), (int (*)(const void*, const void*))dcomp);
        }
		*namelist = names;
		rc = nitems;
	} else {  /* We were unsuccessful, clean up storage and return -1.  */
        RHO_LOG("@@@@@ scandir_emulate 11 ");
		if ( names ) {
            RHO_LOG("@@@@@ scandir_emulate 12 ");

			int i;
			for (i=0; i < nitems; i++ )
				free( names[i] );
			free( names );
		}
		rc = -1;
	}
	return(rc);
}






RHO_GLOBAL int scandir(const char *dir, struct dirent ***namelist, int (*filter)(const struct dirent *),
    int (*compar)(const struct dirent **, const struct dirent **))
{
    RHO_LOG("scandir: dir=%s", dir);
    std::string fpath = make_full_path(dir);
    if (!need_emulate(fpath))
    {
        int ret = real_scandir(dir, namelist, filter, compar);
        RHO_LOG("scandir: return %d (native)", ret);
        return ret;
    }
    else {
        int ret = scandir_emulate(dir, namelist, filter, compar);
        RHO_LOG("scandir: return %d (emulate)", ret);
        return ret;

    }






    RHO_NOT_IMPLEMENTED;
}

RHO_GLOBAL void rho_file_set_fs_mode(int mode)
{
    switch (mode)
    {
    case RHO_FS_TRANSPARRENT:
    case RHO_FS_DISK_ONLY:
        rho_fs_mode = static_cast<RhoFsMode>(mode);
        LOG(TRACE) + "New FS mode: " + mode;
        break;
    default:
        LOG(ERROR) + "Wrong FS mode: " + mode;
    }
}

RHO_GLOBAL int rho_file_get_fs_mode()
{
    return rho_fs_mode;
}

RHO_GLOBAL void rho_android_file_reload_stat_table() {

    RHO_LOG("rho_android_file_reload_stat_table() START");

    rho_stat_map.clear();

    JNIEnv *env = jnienv_fileapi();
    env->CallStaticVoidMethod(clsFileApi, midReloadStatTable);

    RHO_LOG("rho_android_file_reload_stat_table() FINISH");
}

RHO_GLOBAL void rho_android_force_all_files() {

    RHO_LOG("rho_android_force_all_files() START");

    JNIEnv *env = jnienv_fileapi();

    env->CallStaticVoidMethod(clsFileApi, midForceAllFiles);

    RHO_LOG("rho_android_force_all_files() FINISH");
}

RHO_GLOBAL int rho_android_remove_item(const char* path) {

    RHO_LOG("rho_android_remove_item() START");

    JNIEnv *env = jnienv_fileapi();

    jhstring objStr = rho_cast<jstring>(env, path);

    int res = env->CallStaticIntMethod(clsFileApi, midDeleteRecursively, objStr.get());

    RHO_LOG("rho_android_remove_item() FINISH");

    return res;
}
