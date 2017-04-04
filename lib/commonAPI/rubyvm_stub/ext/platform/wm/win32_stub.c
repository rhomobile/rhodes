
#include <Windows.h>
#include <stdlib.h>

#include <ruby/wince/sys/types.h>
#include <ruby/wince/errno.h>

static char *envarea;

int rb_w32_map_errno(unsigned long winerr);

#define RUBY_CRITICAL(expr) do { expr; } while (0)
#define map_errno(e) rb_w32_map_errno(e)
#define AreFileApisANSI() 1

#if defined(_WIN32_WCE) || defined(WIN32) || defined(_WINRT_DLL) || defined(_WP8_LIB) || defined(_UWP_LIB)
# define strncasecmp _strnicmp
#endif

//////////////////////////////////////////////////////////////////////////

off_t
_lseeki64(int fd, off_t offset, int whence)
{
    long u, l;
    int e;
    HANDLE h = (HANDLE)_get_osfhandle(fd);

    if (!h) {
	errno = EBADF;
	return -1;
    }
    u = (long)(offset >> 32);
    if ((l = SetFilePointer(h, (long)offset, &u, whence)) == -1L &&
	(e = GetLastError())) {
	errno = map_errno(e);
	return -1;
    }
    return ((off_t)u << 32) | l;
}

char* rb_w32_getenv(const char *name)
{
    int len = strlen(name);
    char *env;

    if (len == 0) return NULL;
    if (envarea) FreeEnvironmentStrings(envarea);
    envarea = GetEnvironmentStrings();
    if (!envarea) {
        map_errno(GetLastError());
        return NULL;
    }

    for (env = envarea; *env; env += strlen(env) + 1)
        if (strncasecmp(env, name, len) == 0 && *(env + len) == '=')
            return env + len + 1;

    return NULL;
}

//////////////////////////////////////////////////////////////////////////
// file functions

int wrename(const WCHAR *oldpath, const WCHAR *newpath)
{
    int res = 0;
    int oldatts;
    int newatts;

    oldatts = GetFileAttributesW(oldpath);
    newatts = GetFileAttributesW(newpath);

    if (oldatts == -1) {
        errno = map_errno(GetLastError());
        return -1;
    }

#ifndef _WIN32_WCE
    RUBY_CRITICAL({
        if (newatts != -1 && newatts & FILE_ATTRIBUTE_READONLY)
            SetFileAttributesW(newpath, newatts & ~ FILE_ATTRIBUTE_READONLY);

        if (!MoveFileW(oldpath, newpath))
            res = -1;

        if (res) {
            switch (GetLastError()) {
case ERROR_ALREADY_EXISTS:
case ERROR_FILE_EXISTS:
    if (IsWinNT()) {
        if (MoveFileExW(oldpath, newpath, MOVEFILE_REPLACE_EXISTING))
            res = 0;
    } else 
    {
        for (;;) {
            if (!DeleteFileW(newpath) && GetLastError() != ERROR_FILE_NOT_FOUND)
                break;
            else if (MoveFileW(oldpath, newpath)) {
                res = 0;
                break;
            }
        }
    }
            }
        }

        if (res)
            errno = map_errno(GetLastError());
        else
            SetFileAttributesW(newpath, oldatts);
    });
#else
    RUBY_CRITICAL({
        if (newatts != -1 && newatts & FILE_ATTRIBUTE_READONLY)
            SetFileAttributesW(newpath, newatts & ~ FILE_ATTRIBUTE_READONLY);

        if (!MoveFileW(oldpath, newpath))
            res = -1;

        if (res) {
            switch (GetLastError()) {
case ERROR_ALREADY_EXISTS:
case ERROR_FILE_EXISTS:
    {
        for (;;) {
            if (!DeleteFileW(newpath) && GetLastError() != ERROR_FILE_NOT_FOUND)
                break;
            else if (MoveFileW(oldpath, newpath)) {
                res = 0;
                break;
            }
        }
    }
            }
        }

        if (res)
            errno = map_errno(GetLastError());
        else
            SetFileAttributesW(newpath, oldatts);
    });

#endif
    return res;
}

WCHAR* filecp_to_wstr(const char *str, long *plen)
{
    UINT cp = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
    WCHAR *ptr;
    int len = MultiByteToWideChar(cp, 0, str, -1, NULL, 0) - 1;
    if (!(ptr = malloc(sizeof(WCHAR) * (len + 1)))) return 0;
    MultiByteToWideChar(cp, 0, str, -1, ptr, len + 1);
    if (plen) *plen = len;
    return ptr;
}

int rb_w32_rename(const char *oldname, const char *newname)
{
    WCHAR *wfrom;
    WCHAR *wto;
    int ret = -1;

    if (!(wfrom = filecp_to_wstr(oldname, NULL)))
        return -1;
    if (!(wto = filecp_to_wstr(newname, NULL))) {
        free(wfrom);
        return -1;
    }
    ret = wrename(wfrom, wto);
    free(wto);
    free(wfrom);
    return ret;
}

int wunlink(const WCHAR *path)
{
    int ret = 0;
    RUBY_CRITICAL({
        const DWORD attr = GetFileAttributesW(path);
        if (attr != (DWORD)-1 && (attr & FILE_ATTRIBUTE_READONLY)) {
            SetFileAttributesW(path, attr & ~FILE_ATTRIBUTE_READONLY);
        }
        if (DeleteFileW(path) == FALSE) {
            errno = map_errno(GetLastError());
            ret = -1;
            if (attr != (DWORD)-1 && (attr & FILE_ATTRIBUTE_READONLY)) {
                SetFileAttributesW(path, attr);
            }
        }
    });
    return ret;
}

int rb_w32_unlink(const char *path)
{
    WCHAR *wpath;
    int ret;

    if (!(wpath = filecp_to_wstr(path, NULL)))
        return -1;
    ret = wunlink(wpath);
    free(wpath);
    return ret;
}

//////////////////////////////////////////////////////////////////////////
//TODO, this functions is not used, but should be defined for compilation fix 

int
rb_w32_map_errno(unsigned long winerr)
{
    return 0;
}

int 
rb_w32_recv(int fd, char *buf, int len, int flags)
{
    return 0;
}

int
rb_w32_is_socket(int fd)
{
    return 0;
}

int 
rb_w32_send(int fd, const char *buf, int len, int flags)
{
    return 0;
}
