/***************************************************************
  io.c

  author : uema2
  date   : Nov 30, 2002

  You can freely use, copy, modify, and redistribute
  the whole contents.
***************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

#include "common/app_build_capabilities.h"

#if defined(APP_BUILD_CAPABILITY_WINXPE)
#define _WIN32_WCE

extern int FAR PASCAL rb_w32_recv(int, char *, int, int);
extern int FAR PASCAL rb_w32_send(int, const char *, int, int);
#endif

#if defined(_WIN32_WCE)
#include "wince.h" /* for wce_mbtowc */
#endif

#include "common/RhoMutexLock.h"

#if defined(_WIN32_WCE) && !defined(APP_BUILD_CAPABILITY_WINXPE)
extern int _errno;
#endif

#if defined(_WIN32_WCE)

#define map_errno rb_w32_map_errno
#if 0
int rb_w32_rename(const char *oldname, const char *newname)
{
	wchar_t *wold, *wnew;
	int ret = 0;

	wold = wce_mbtowc(oldname);
	wnew = wce_mbtowc(newname);

	/* replace with MoveFile. */
	if ( MoveFileW(wold, wnew) == FALSE )
    {
        errno = map_errno(GetLastError());
        ret = -1;
    }

	free(wold);
	free(wnew);

	return ret;
}
#endif

int _rename(const char *oldname, const char *newname)
{
    return rb_w32_rename(oldname, newname);
}
#if 0
int rb_w32_unlink(const char *path)
{
	wchar_t *wfile = wce_mbtowc(path);
    int ret = 0;

	const DWORD attr = GetFileAttributes(path);
	if (attr != (DWORD)-1 && (attr & FILE_ATTRIBUTE_READONLY)) {
	    SetFileAttributes(path, attr & ~FILE_ATTRIBUTE_READONLY);
	}
	if (DeleteFileW(wfile) == FALSE) {
	    errno = map_errno(GetLastError());
	    ret = -1;
	    if (attr != (DWORD)-1 && (attr & FILE_ATTRIBUTE_READONLY)) {
		SetFileAttributes(path, attr);
	    }
	}

	free(wfile);
    return ret;
}
#endif

#if !defined(APP_BUILD_CAPABILITY_WINXPE)
int _unlink(const char *file)
{
    return rb_w32_unlink(file);
}
#endif

/*
int
vsnprintf(char *buf, size_t size, const char *format, va_list va)
{
    int ret = _vsnprintf(buf, size, format, va);
    if (size > 0) buf[size - 1] = 0;
    return ret;
}

int
snprintf(char *buf, size_t size, const char *format, ...)
{
    int ret;
    va_list va;

    va_start(va, format);
    ret = vsnprintf(buf, size, format, va);
    va_end(va);
    return ret;
}*/

int rb_w32_open( const char *file, int mode, ... )
{
    int ret = 0;
    va_list ap;
    va_start(ap, mode);
    ret = _open(file, mode, ap);
    va_end(ap);

    return ret;
}

RHO_INIT_LOCK(FileHandlers);
#define  g_nMaxFileHandlers  256
static HANDLE g_arFileHandlers[g_nMaxFileHandlers] = {0};
int get_NewFileNumber()
{
    int i = 0;
    int nRes = -1;
    RHO_LOCK(FileHandlers);
    for( i = 0; i < g_nMaxFileHandlers; i++ )
    {
        if ( g_arFileHandlers[i] == 0 )
        {
            nRes = i;
            break;
        }
    }
    RHO_UNLOCK(FileHandlers);
    return nRes+1;
}

void set_FileNumber(int fNumber, HANDLE osfh)
{
    RHO_LOCK(FileHandlers);
    g_arFileHandlers[fNumber-1] = osfh;
    RHO_UNLOCK(FileHandlers);
}

HANDLE get_OSHandleByFileNumber(int fNumber)
{
    HANDLE res;
    RHO_LOCK(FileHandlers);
    res = g_arFileHandlers[fNumber-1];
    RHO_UNLOCK(FileHandlers);
    return res;
}

#if !defined(APP_BUILD_CAPABILITY_WINXPE)
int _open(const char *path, int oflag, va_list arg)
{
    wchar_t *wfile = wce_mbtowc(path);
    int nRet = _wopen(wfile, oflag, arg);
    free(wfile);

    return nRet;
}

/* replace "open" with "CreateFile", etc. */
int _wopen(const wchar_t *path, int oflag, va_list arg)
{
    DWORD fileaccess = 0;               /* OS file access (requested) */
    DWORD fileshare = 0;                /* OS file sharing mode */
    DWORD filecreate = 0;               /* OS method of opening/creating */
    DWORD fileattrib = 0;               /* OS file attribute flags */
    SECURITY_ATTRIBUTES SecurityAttributes;
    HANDLE osfh;
    DWORD lasterror = 0;
    int fNumber = -1;

    SecurityAttributes.nLength = sizeof( SecurityAttributes );
    SecurityAttributes.lpSecurityDescriptor = NULL;

    if (oflag & _O_NOINHERIT) {
        SecurityAttributes.bInheritHandle = FALSE;
    }else {
        SecurityAttributes.bInheritHandle = TRUE;
    }

    /*
     * decode the access flags
     */
    switch( oflag & (_O_RDONLY | _O_WRONLY | _O_RDWR) ) {

        case _O_RDONLY:         /* read access */
                fileaccess = GENERIC_READ;
                fileshare = FILE_SHARE_READ;
                break;
        case _O_WRONLY:         /* write access */
                /* giving it read access as well
                 * because in append (a, not a+), we need
                 * to read the BOM to determine the encoding
                 * (ie. ANSI, UTF8, UTF16)
                 */
                if ((oflag & _O_APPEND) /*&& (oflag & (_O_WTEXT | _O_U16TEXT | _O_U8TEXT)) != 0*/ )
                {
                    fileaccess = GENERIC_READ | GENERIC_WRITE;
                }
                else
                {
                    fileaccess = GENERIC_WRITE;
                }
                break;
        case _O_RDWR:           /* read and write access */
                fileaccess = GENERIC_READ | GENERIC_WRITE;
                break;
    }

    /*
     * decode open/create method flags
     */
    switch ( oflag & (_O_CREAT | _O_EXCL | _O_TRUNC) ) {
        case 0:
        case _O_EXCL:                   // ignore EXCL w/o CREAT
            filecreate = OPEN_EXISTING;
            break;

        case _O_CREAT:
            filecreate = OPEN_ALWAYS;
            break;

        case _O_CREAT | _O_EXCL:
        case _O_CREAT | _O_TRUNC | _O_EXCL:
            filecreate = CREATE_NEW;
            break;

        case _O_TRUNC:
        case _O_TRUNC | _O_EXCL:        // ignore EXCL w/o CREAT
            filecreate = TRUNCATE_EXISTING;
            break;

        case _O_CREAT | _O_TRUNC:
            filecreate = CREATE_ALWAYS;
            break;
    }

    /*
     * decode file attribute flags if _O_CREAT was specified
     */
    fileattrib = FILE_ATTRIBUTE_NORMAL;     /* default */

    fNumber = get_NewFileNumber();
    if ( fNumber == 0 )
    {
    	errno = EMFILE;
        return -1;
    }

    if ( (osfh = CreateFileW( path,
                             fileaccess,
                             fileshare,
                             &SecurityAttributes,
                             filecreate,
                             fileattrib,
                             NULL )) == INVALID_HANDLE_VALUE  )
    {
        if ((fileaccess & (GENERIC_READ | GENERIC_WRITE)) == (GENERIC_READ | GENERIC_WRITE) &&
                (oflag & _O_WRONLY))
        {
            fileaccess &= ~GENERIC_READ;
            osfh = CreateFileW( path,
                     fileaccess,
                     fileshare,
                     &SecurityAttributes,
                     filecreate,
                     fileattrib, NULL );
        }
    }

    lasterror = GetLastError();

    if ( osfh != INVALID_HANDLE_VALUE  )
    {
        if ( (oflag & _O_APPEND) )
            SetFilePointer( osfh, 0, NULL, FILE_END );

        set_FileNumber(fNumber, osfh);
        return fNumber;
    }

    errno = rb_w32_map_errno(lasterror);
    return -1;
}
#endif

int close(int fd)
{
    if (fd-1 < g_nMaxFileHandlers)
    {
        HANDLE fHandle = get_OSHandleByFileNumber(fd);
        set_FileNumber( fd, 0);
	    CloseHandle( fHandle );
    }

	return 0;
}

int
rb_w32_fclose(FILE *fp)
{
    return fclose(fp);
}

#if !defined(APP_BUILD_CAPABILITY_WINXPE)
int _read(int fd, void *buffer, int length)
{
	DWORD dw;
    HANDLE fHandle = get_OSHandleByFileNumber(fd);

	if ( ReadFile( fHandle, buffer, length, &dw, NULL ) == FALSE )
    {
        errno = map_errno(GetLastError());
        return -1;
    }

	return (int)dw;
}


int _write(int fd, const void *buffer, unsigned count)
{
    DWORD dw;
    if ( fd < 0 ) {
        //char* buf = (char*) malloc(count+1);
        //memcpy(buf,buffer,count);
        //buf[count] = 0;
        //printf("%s",buf);
        //free(buf);
        //dw = count;
        //TBD: fix output of the long strings
       // dw = fwrite( buffer, 1, count, stdout);
    } else {
        HANDLE fHandle = get_OSHandleByFileNumber(fd);
        if ( WriteFile( fHandle, buffer, count, &dw, NULL ) == FALSE )
        {
            errno = map_errno(GetLastError());
            return -1;
        }

    }
    return (int)dw;
}

long _lseek(int fd, long offset, int origin)
{
	DWORD flag, ret = 0;
    HANDLE fHandle = get_OSHandleByFileNumber(fd);
	switch(origin)
	{
	case SEEK_SET: flag = FILE_BEGIN;   break;
	case SEEK_CUR: flag = FILE_CURRENT; break;
	case SEEK_END: flag = FILE_END;     break;
	default:       flag = FILE_CURRENT; break;
	}

	if ( SetFilePointer( fHandle, offset, NULL, flag ) == 0xFFFFFFFF )
    {
        errno = map_errno(GetLastError());
        ret = -1;
    }

	return ret ? -1 : 0;
}
#endif // APP_BUILD_CAPABILITY_WINXPE

size_t
rb_w32_read(int fd, void *buf, size_t size)
{
    if (rb_w32_is_socket(fd))
    	return rb_w32_recv(fd, buf, size, 0);

    return _read(fd,buf,size);
}

size_t
rb_w32_write(int fd, const void *buf, size_t size)
{
    if (rb_w32_is_socket(fd))
    	return rb_w32_send(fd, buf, size, 0);

    return _write(fd,buf,size);
}

/* _findfirst, _findnext, _findclose. */
/* replace them with FindFirstFile, etc. */
long _findfirst( char *file, struct _finddata_t *fi )
{
	HANDLE h;
	WIN32_FIND_DATAA fda;

    int fNumber = get_NewFileNumber();
    if ( fNumber == 0 )
        return -1;

	h = FindFirstFileA( file, &fda );
	if( h==NULL )
	{
		errno = map_errno(GetLastError());
        return -1;
	}

	fi->attrib      = fda.dwFileAttributes;
	fi->time_create = wce_FILETIME2time_t( &fda.ftCreationTime );
	fi->time_access = wce_FILETIME2time_t( &fda.ftLastAccessTime );
	fi->time_write  = wce_FILETIME2time_t( &fda.ftLastWriteTime );
	fi->size        = fda.nFileSizeLow + (fda.nFileSizeHigh<<32);
	strcpy( fi->name, fda.cFileName );

    set_FileNumber(fNumber,h);
	return fNumber;
}

int _findnext( long fd, struct _finddata_t *fi )
{
	WIN32_FIND_DATAA fda;
	BOOL b;
    HANDLE fHandle = get_OSHandleByFileNumber(fd);

	b = FindNextFileA( fHandle, &fda );

	if( b==FALSE )
	{
		errno = map_errno(GetLastError());
        return -1;
	}

	fi->attrib      = fda.dwFileAttributes;
	fi->time_create = wce_FILETIME2time_t( &fda.ftCreationTime );
	fi->time_access = wce_FILETIME2time_t( &fda.ftLastAccessTime );
	fi->time_write  = wce_FILETIME2time_t( &fda.ftLastWriteTime );
	fi->size        = fda.nFileSizeLow + (fda.nFileSizeHigh<<32);
	strcpy( fi->name, fda.cFileName );

	return 0;
}

int _findclose( long fd )
{
	BOOL b;
    int ret = 0;
    HANDLE fHandle = get_OSHandleByFileNumber(fd);
    set_FileNumber(fd,0);
	if ( FindClose( fHandle ) == FALSE )
    {
        errno = map_errno(GetLastError());
        ret = -1;
    }

	return ret;
}

/* below functions unsupported... */
/* I have no idea how to replace... */
//int _chsize(int handle, long size)
//{
//	errno = EACCES;
//	return -1;
//}

#if !defined(APP_BUILD_CAPABILITY_WINXPE)
int _umask(int cmask)
{
	return 0;
}

int _chmod(const char *path, int mode)
{
	return 0;
}
#endif

/* WinCE doesn't have dup and dup2.  */
/* so, we cannot use missing/dup2.c. */
int dup( int handle )
{
	errno = EBADF;
	return -1;
}

int _isatty(int fd)
{
	/*if( fd==(int)_fileno(stdin) || 
		fd==(int)_fileno(stdout)||
		fd==(int)_fileno(stderr) )
		return 1;
	else*/
		return 0;
}

int rb_w32_isatty(int fd)
{
    return _isatty(fd);
}

int _pipe(int *phandles )//, unsigned int psize, int textmode)
{
	return -1;
}

int rb_w32_pipe(int *phandles )//, unsigned int psize, int textmode)
{
	return -1;
}

#if !defined(APP_BUILD_CAPABILITY_WINXPE)
int _access(const char *filename, int flags)
{
    DWORD attr = GetFileAttributesA(filename);
    if (attr  == 0xffffffff) {
        return GetLastError();
    }

	return 0;
}

int _open_osfhandle( long osfhandle, int flags)
{
 /*	return 0; */
	return (int)osfhandle;
}

long _get_osfhandle( int filehandle )
{
 /*	return 0; */
	return (long)filehandle;
}
#endif

#endif