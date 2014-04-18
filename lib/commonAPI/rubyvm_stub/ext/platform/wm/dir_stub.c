#include <Windows.h>

#include <ruby/wince/sys/stat.h>
#include <ruby/wince/errno.h>
#include <ruby/win32/dir.h>
#include <ruby/ruby.h>
#include <ruby/encoding.h>
#include <ruby/regenc.h>

extern int rb_w32_map_errno(unsigned long winerr);
extern WCHAR* filecp_to_wstr(const char *str, long *plen);

//////////////////////////////////////////////////////////////////////////
// regenc.c

const unsigned short OnigEncAsciiCtypeTable[256] = {
    0x4008, 0x4008, 0x4008, 0x4008, 0x4008, 0x4008, 0x4008, 0x4008,
    0x4008, 0x420c, 0x4209, 0x4208, 0x4208, 0x4208, 0x4008, 0x4008,
    0x4008, 0x4008, 0x4008, 0x4008, 0x4008, 0x4008, 0x4008, 0x4008,
    0x4008, 0x4008, 0x4008, 0x4008, 0x4008, 0x4008, 0x4008, 0x4008,
    0x4284, 0x41a0, 0x41a0, 0x41a0, 0x41a0, 0x41a0, 0x41a0, 0x41a0,
    0x41a0, 0x41a0, 0x41a0, 0x41a0, 0x41a0, 0x41a0, 0x41a0, 0x41a0,
    0x78b0, 0x78b0, 0x78b0, 0x78b0, 0x78b0, 0x78b0, 0x78b0, 0x78b0,
    0x78b0, 0x78b0, 0x41a0, 0x41a0, 0x41a0, 0x41a0, 0x41a0, 0x41a0,
    0x41a0, 0x7ca2, 0x7ca2, 0x7ca2, 0x7ca2, 0x7ca2, 0x7ca2, 0x74a2,
    0x74a2, 0x74a2, 0x74a2, 0x74a2, 0x74a2, 0x74a2, 0x74a2, 0x74a2,
    0x74a2, 0x74a2, 0x74a2, 0x74a2, 0x74a2, 0x74a2, 0x74a2, 0x74a2,
    0x74a2, 0x74a2, 0x74a2, 0x41a0, 0x41a0, 0x41a0, 0x41a0, 0x51a0,
    0x41a0, 0x78e2, 0x78e2, 0x78e2, 0x78e2, 0x78e2, 0x78e2, 0x70e2,
    0x70e2, 0x70e2, 0x70e2, 0x70e2, 0x70e2, 0x70e2, 0x70e2, 0x70e2,
    0x70e2, 0x70e2, 0x70e2, 0x70e2, 0x70e2, 0x70e2, 0x70e2, 0x70e2,
    0x70e2, 0x70e2, 0x70e2, 0x41a0, 0x41a0, 0x41a0, 0x41a0, 0x4008,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

//////////////////////////////////////////////////////////////////////////
// defines 

#define IsWinNT() rb_w32_iswinnt()

#define GetBit(bits, i) ((bits)[(i) / CHAR_BIT] &  (1 << (i) % CHAR_BIT))
#define SetBit(bits, i) ((bits)[(i) / CHAR_BIT] |= (1 << (i) % CHAR_BIT))

#define BitOfIsDir(n) ((n) * 2)
#define BitOfIsRep(n) ((n) * 2 + 1)
#define DIRENT_PER_CHAR (CHAR_BIT / 2)

#define map_errno(e) rb_w32_map_errno(e)

#define rb_isascii(c) ((unsigned long)(c) < 128)
#define CTYPE_TO_BIT(ctype)  (1<<(ctype))
#define ONIGENC_IS_ASCII_CODE_CTYPE(code,ctype) ((OnigEncAsciiCtypeTable[code] & CTYPE_TO_BIT(ctype)) != 0)
#define ONIGENC_CTYPE_ALPHA     1
#define ctype_test(c, ctype) (rb_isascii(c) && ONIGENC_IS_ASCII_CODE_CTYPE((c), ctype))

//////////////////////////////////////////////////////////////////////////
// directory functions

int rb_isalpha(int c) 
{ 
    return ctype_test(c, ONIGENC_CTYPE_ALPHA); 
}

#ifdef WIN95
static int
win95_stat(const WCHAR *path, struct stati64 *st)
{
    int ret = _wstati64(path, st);
    if (ret) return ret;
    if (st->st_mode & S_IFDIR) {
        return check_valid_dir(path);
    }
    return 0;
}
#else
#define win95_stat(path, st) -1
#endif

HANDLE open_dir_handle(const WCHAR *filename, WIN32_FIND_DATAW *fd)
{
    HANDLE fh;
    static const WCHAR wildcard[] = L"\\*";
    WCHAR *scanname;
    WCHAR *p;
    int len, i;

    //
    // Create the search pattern
    //
    len = lstrlenW(filename);
    scanname = ALLOCA_N(WCHAR, len + sizeof(wildcard) / sizeof(WCHAR));
    lstrcpyW(scanname, filename);
    p = CharPrevW(scanname, scanname + len);
    if (*p == L'/' || *p == L'\\' || *p == L':')
        lstrcatW(scanname, wildcard + 1);
    else
        lstrcatW(scanname, wildcard);

    for( i = 0; scanname[i]; i++)
    {
        if ( scanname[i] == L'/' )
            scanname[i] = L'\\';
    }

    //
    // do the FindFirstFile call
    //
    fh = FindFirstFileW(scanname, fd);
    if (fh == INVALID_HANDLE_VALUE) 
    {
        DWORD dwErr = GetLastError();
        if ( dwErr == 18)
            fh = 0;
        else
            errno = map_errno(GetLastError());
    }
    return fh;
}

int isUNCRoot(const WCHAR *path)
{
    if (path[0] == L'\\' && path[1] == L'\\') {
        const WCHAR *p;
        for (p = path + 2; *p; p++) {
            if (*p == L'\\')
                break;
        }
        if (p[0] && p[1]) {
            for (p++; *p; p++) {
                if (*p == L'\\')
                    break;
            }
            if (!p[0] || !p[1] || (p[1] == L'.' && !p[2]))
                return 1;
        }
    }
    return 0;
}

unsigned fileattr_to_unixmode(DWORD attr, const WCHAR *path)
{
    unsigned mode = 0;

    if (attr & FILE_ATTRIBUTE_READONLY) {
        mode |= S_IREAD;
    }
    else {
        mode |= S_IREAD | S_IWRITE | S_IWUSR;
    }

    if (attr & FILE_ATTRIBUTE_DIRECTORY) {
        mode |= S_IFDIR | S_IEXEC;
    }
    else {
        mode |= S_IFREG;
    }

    if (path && (mode & S_IFREG)) {
        const WCHAR *end = path + lstrlenW(path);
        while (path < end) {
            end = CharPrevW(path, end);
            if (*end == L'.') {
                if ((_wcsicmp(end, L".bat") == 0) ||
                    (_wcsicmp(end, L".cmd") == 0) ||
                    (_wcsicmp(end, L".com") == 0) ||
                    (_wcsicmp(end, L".exe") == 0)) {
                        mode |= S_IEXEC;
                }
                break;
            }
        }
    }

    mode |= (mode & 0700) >> 3;
    mode |= (mode & 0700) >> 6;

    return mode;
}

int filetime_to_timeval(const FILETIME* ft, struct timeval *tv)
{
    ULARGE_INTEGER tmp;
    unsigned LONG_LONG lt;

    tmp.LowPart = ft->dwLowDateTime;
    tmp.HighPart = ft->dwHighDateTime;
    lt = tmp.QuadPart;

    /* lt is now 100-nanosec intervals since 1601/01/01 00:00:00 UTC,
       convert it into UNIX time (since 1970/01/01 00:00:00 UTC).
       the first leap second is at 1972/06/30, so we doesn't need to think
       about it. */
    lt /= 10;	/* to usec */
    lt -= (LONG_LONG)((1970-1601)*365.2425) * 24 * 60 * 60 * 1000 * 1000;

    tv->tv_sec = (long)(lt / (1000 * 1000));
    tv->tv_usec = (long)(lt % (1000 * 1000));

    return tv->tv_sec > 0 ? 0 : -1;
}

time_t filetime_to_unixtime(const FILETIME *ft)
{
    struct timeval tv;

    if (filetime_to_timeval(ft, &tv) == (time_t)-1)
        return 0;
    else
        return tv.tv_sec;
}

int check_valid_dir(const WCHAR *path)
{
    WIN32_FIND_DATAW fd = {0};
    HANDLE fh = open_dir_handle(path, &fd);
    if (fh == INVALID_HANDLE_VALUE)
        return -1;
    FindClose(fh);
    return 0;
}

int winnt_stat(const WCHAR *path, struct stati64 *st)
{
    HANDLE h;
    WIN32_FIND_DATAW wfd = {0};

    memset(st, 0, sizeof(*st));
    st->st_nlink = 1;

    if (wcspbrk(path, L"?*")) {
        errno = ENOENT;
        return -1;
    }
    h = FindFirstFileW(path, &wfd);
    if (h != INVALID_HANDLE_VALUE) {
        FindClose(h);
        st->st_mode  = fileattr_to_unixmode(wfd.dwFileAttributes, path);
        st->st_atime = filetime_to_unixtime(&wfd.ftLastAccessTime);
        st->st_mtime = filetime_to_unixtime(&wfd.ftLastWriteTime);
        st->st_ctime = filetime_to_unixtime(&wfd.ftCreationTime);
        st->st_size = ((__int64)wfd.nFileSizeHigh << 32) | wfd.nFileSizeLow;
    }
    else {
        // If runtime stat(2) is called for network shares, it fails on WinNT.
        // Because GetDriveType returns 1 for network shares. (Win98 returns 4)
        DWORD attr = GetFileAttributesW(path);
        if (attr == (DWORD)-1L) {
            errno = map_errno(GetLastError());
            return -1;
        }
        if (attr & FILE_ATTRIBUTE_DIRECTORY) {
            if (check_valid_dir(path)) return -1;
        }
        st->st_mode  = fileattr_to_unixmode(attr, path);
    }

    st->st_dev = st->st_rdev = (iswalpha(path[0]) && path[1] == L':') ?
        towupper(path[0]) - L'A' : _getdrive() - 1;

    return 0;
}

int wstati64(const WCHAR *path, struct stati64 *st)
{
    const WCHAR *p;
    WCHAR *buf1, *s, *end;
    int len, size;
    int ret;

    if (!path || !st) {
        errno = EFAULT;
        return -1;
    }
    size = lstrlenW(path) + 2;
    buf1 = ALLOCA_N(WCHAR, size);
    for (p = path, s = buf1; *p; p++, s++) {
        if (*p == L'/')
            *s = L'\\';
        else
            *s = *p;
    }
    *s = '\0';
    len = s - buf1;
    if (!len || L'\"' == *(--s)) {
        errno = ENOENT;
        return -1;
    }
    end = buf1 + len - 1;

    if (isUNCRoot(buf1)) {
        if (*end == L'.')
            *end = L'\0';
        else if (*end != L'\\')
            lstrcatW(buf1, L"\\");
    }
    else if (*end == L'\\' || (buf1 + 1 == end && *end == L':'))
        lstrcatW(buf1, L".");

    ret = IsWinNT() ? winnt_stat(buf1, st) : win95_stat(buf1, st);
    if (ret == 0) {
        st->st_mode &= ~(S_IWGRP | S_IWOTH);
    }
    return ret;
}

DIR* opendir_internal(HANDLE fh, WIN32_FIND_DATAW *fd)
{
    DIR *p;
    long len;
    long idx;
    WCHAR *tmpW;
    char *tmp;

    if (fh == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    //
    // Get us a DIR structure
    //
    p = calloc(sizeof(DIR), 1);
    if (p == NULL)
        return NULL;

    idx = 0;

    //
    // loop finding all the files that match the wildcard
    // (which should be all of them in this directory!).
    // the variable idx should point one past the null terminator
    // of the previous string found.
    //
    do {
        len = lstrlenW(fd->cFileName) + 1;

        //
        // bump the string table size by enough for the
        // new name and it's null terminator 
        //
        tmpW = realloc(p->start, (idx + len) * sizeof(WCHAR));
        if (!tmpW) {
error:
            rb_w32_closedir(p);
            FindClose(fh);
            errno = ENOMEM;
            return NULL;
        }

        p->start = tmpW;
        memcpy(&p->start[idx], fd->cFileName, len * sizeof(WCHAR));

        if (p->nfiles % DIRENT_PER_CHAR == 0) {
            tmp = realloc(p->bits, p->nfiles / DIRENT_PER_CHAR + 1);
            if (!tmp)
                goto error;
            p->bits = tmp;
            p->bits[p->nfiles / DIRENT_PER_CHAR] = 0;
        }
        if (fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            SetBit(p->bits, BitOfIsDir(p->nfiles));
        if (fd->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
            SetBit(p->bits, BitOfIsRep(p->nfiles));

        p->nfiles++;
        idx += len;
    } while (FindNextFileW(fh, fd));
    FindClose(fh);
    p->size = idx;
    p->curr = p->start;
    return p;
}

DIR* rb_w32_opendir(const char* filename)
{
    struct stati64 sbuf;
    WIN32_FIND_DATAW fd = {0};
    HANDLE fh;
    WCHAR *wpath;

    if (!(wpath = filecp_to_wstr(filename, NULL)))
        return NULL;

    //
    // check to see if we've got a directory
    //
    if (wstati64(wpath, &sbuf) < 0) {
        free(wpath);
        return NULL;
    }
    if (!(sbuf.st_mode & S_IFDIR) &&
        (!ISALPHA(filename[0]) || filename[1] != ':' || filename[2] != '\0' ||
        ((1 << ((filename[0] & 0x5f) - 'A')) & GetLogicalDrives()) == 0)) {
            free(wpath);
            errno = ENOTDIR;
            return NULL;
    }

    fh = open_dir_handle(wpath, &fd);
    free(wpath);
    return opendir_internal(fh, &fd);
}

void rb_w32_closedir(DIR *dirp)
{
    if (dirp) 
    {
        if (dirp->dirstr.d_name)
            free(dirp->dirstr.d_name);
        if (dirp->start)
            free(dirp->start);
        if (dirp->bits)
            free(dirp->bits);
        free(dirp);
    }
}

void move_to_next_entry(DIR *dirp)
{
    if (dirp->curr) {
        dirp->loc++;
        dirp->curr += lstrlenW(dirp->curr) + 1;
        if (dirp->curr >= (dirp->start + dirp->size)) {
            dirp->curr = NULL;
        }
    }
}

struct direct* readdir_internal(DIR *dirp, BOOL (*conv)(const WCHAR *, struct direct *, rb_encoding *), rb_encoding *enc)
{
    static int dummy = 0;

    if (dirp->curr) {

        //
        // first set up the structure to return
        //
        if (dirp->dirstr.d_name)
            free(dirp->dirstr.d_name);
        conv(dirp->curr, &dirp->dirstr, enc);

        //
        // Fake inode
        //
        dirp->dirstr.d_ino = dummy++;

        //
        // Attributes
        //
        dirp->dirstr.d_isdir = GetBit(dirp->bits, BitOfIsDir(dirp->loc));
        dirp->dirstr.d_isrep = GetBit(dirp->bits, BitOfIsRep(dirp->loc));

        //
        // Now set up for the next call to readdir
        //

        move_to_next_entry(dirp);

        return &(dirp->dirstr);

    } else
        return NULL;
}

char* wstr_to_filecp(const WCHAR *wstr, long *plen)
{
    UINT cp = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
    char *ptr;                
    int len = WideCharToMultiByte(cp, 0, wstr, -1, NULL, 0, NULL, NULL) - 1;
    if (!(ptr = malloc(len + 1))) return 0;
    WideCharToMultiByte(cp, 0, wstr, -1, ptr, len + 1, NULL, NULL);
    if (plen) *plen = len;
    return ptr;
}

BOOL win32_direct_conv(const WCHAR *file, struct direct *entry, rb_encoding *dummy)
{
    if (!(entry->d_name = wstr_to_filecp(file, &entry->d_namlen)))
        return FALSE;
    return TRUE;
}

struct direct* rb_w32_readdir(DIR *dirp)
{
    return readdir_internal(dirp, win32_direct_conv, NULL);
}  
