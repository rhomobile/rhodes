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
#include "wince.h" /* for wce_mbtowc */
#include "common/RhoMutexLock.h"

extern int _errno;

int rb_w32_rename(const char *oldname, const char *newname)
{
	wchar_t *wold, *wnew;
	BOOL rc;

	wold = wce_mbtowc(oldname);
	wnew = wce_mbtowc(newname);

	/* replace with MoveFile. */
	rc = MoveFileW(wold, wnew);

	free(wold);
	free(wnew);

	return rc==TRUE ? 0 : -1;
}

int _rename(const char *oldname, const char *newname)
{
    return rb_w32_rename(oldname, newname);
}

int rb_w32_unlink(const char *file)
{
	wchar_t *wfile;
	BOOL rc;

	/* replace with DeleteFile. */
	wfile = wce_mbtowc(file);
	rc = DeleteFileW(wfile);
	free(wfile);

	return rc==TRUE ? 0 : -1;
}

int _unlink(const char *file)
{
    return rb_w32_unlink(file);
}

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
static HANDLE g_arFileHandlers[g_nMaxFileHandlers];
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

/* replace "open" with "CreateFile", etc. */
int _open(const char *path, int oflag, va_list arg)
{
    DWORD fileaccess = 0;               /* OS file access (requested) */
    DWORD fileshare = 0;                /* OS file sharing mode */
    DWORD filecreate = 0;               /* OS method of opening/creating */
    DWORD fileattrib = 0;               /* OS file attribute flags */
    SECURITY_ATTRIBUTES SecurityAttributes;
    HANDLE osfh;
    DWORD lasterror = 0;
    wchar_t *wfile = 0;
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
        return -1;

    wfile = wce_mbtowc(path);
    if ( (osfh = CreateFileW( wfile,
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
            osfh = CreateFileW( wfile,
                     fileaccess,
                     fileshare,
                     &SecurityAttributes,
                     filecreate,
                     fileattrib, NULL );
        }
    }

    lasterror = GetLastError();
    free(wfile);

    if ( osfh != INVALID_HANDLE_VALUE  )
    {
        if ( (oflag & _O_APPEND) )
            SetFilePointer( osfh, 0, NULL, FILE_END );

        set_FileNumber(fNumber, osfh);
        return fNumber;
    }

    return -1;
}
/*
	wchar_t *wfile;
	DWORD access=0, share=0, create=0, lasterror = 0;
	HANDLE h;
    int fd = 0;

	if( (mode&_O_RDWR) != 0 )
		access = GENERIC_READ|GENERIC_WRITE;
	else if( (mode&_O_WRONLY) != 0 )
		access = GENERIC_WRITE;
	else //if( (mode&_O_RDONLY) != 0 )
		access = GENERIC_READ;

	if( (mode&_O_CREAT) != 0 )
		create = CREATE_ALWAYS;
	else
		create = OPEN_ALWAYS;

	wfile = wce_mbtowc(file);

	h = CreateFileW(wfile, access, share, NULL,
			create, 0, NULL );
  
  lasterror = GetLastError();
	
  free(wfile);
	return (int)h;
}*/

int close(int fd)
{
    HANDLE fHandle = get_OSHandleByFileNumber(fd);
    set_FileNumber( fd, 0);
	CloseHandle( fHandle );
	return 0;
}

int
rb_w32_fclose(FILE *fp)
{
    return fclose(fp);
}

int _read(int fd, void *buffer, int length)
{
	DWORD dw;
    HANDLE fHandle = get_OSHandleByFileNumber(fd);

	ReadFile( fHandle, buffer, length, &dw, NULL );
	return (int)dw;
}

size_t
rb_w32_read(int fd, void *buf, size_t size)
{
    if (rb_w32_is_socket(fd))
    	return rb_w32_recv(fd, buf, size, 0);

    return _read(fd,buf,size);
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
      dw = fwrite( buffer, 1, count, stdout);
    } else {
      HANDLE fHandle = get_OSHandleByFileNumber(fd);
      WriteFile( fHandle, buffer, count, &dw, NULL );
    }
    return (int)dw;
}

size_t
rb_w32_write(int fd, const void *buf, size_t size)
{
    if (rb_w32_is_socket(fd))
    	return rb_w32_send(fd, buf, size, 0);

    return _write(fd,buf,size);
}

long _lseek(int fd, long offset, int origin)
{
	DWORD flag, ret;
    HANDLE fHandle = get_OSHandleByFileNumber(fd);
	switch(origin)
	{
	case SEEK_SET: flag = FILE_BEGIN;   break;
	case SEEK_CUR: flag = FILE_CURRENT; break;
	case SEEK_END: flag = FILE_END;     break;
	default:       flag = FILE_CURRENT; break;
	}

	ret = SetFilePointer( fHandle, offset, NULL, flag );
	return ret==0xFFFFFFFF ? -1 : 0;
}
/*
long _lseeki64(int handle, long offset, int origin)
{
    return _lseek(handle,offset,origin);
}*/

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
		errno = EINVAL; return -1;
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
		errno = ENOENT; return -1;
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
    HANDLE fHandle = get_OSHandleByFileNumber(fd);
    set_FileNumber(fd,0);
	b = FindClose( fHandle );
	return b==FALSE ? -1 : 0;
}

/* below functions unsupported... */
/* I have no idea how to replace... */
int _chsize(int handle, long size)
{
	errno = EACCES;
	return -1;
}

int _umask(int cmask)
{
	return 0;
}

int _chmod(const char *path, int mode)
{
	return 0;
}

/* WinCE doesn't have dup and dup2.  */
/* so, we cannot use missing/dup2.c. */
int dup( int handle )
{
	errno = EBADF;
	return -1;
}
/*
int dup2( int handle1, int handle2 )
{
	errno = EBADF;
	return -1;
}
*/
int _isatty(int fd)
{
	if( fd==(int)_fileno(stdin) || 
		fd==(int)_fileno(stdout)||
		fd==(int)_fileno(stderr) )
		return 1;
	else
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
