/*
 * Copyright (c) 1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */


#include "stlport_prefix.h"

#if defined  (__SUNPPRO_CC)  && !defined (_STLP_NO_NEW_C_HEADERS)
#  include <time.h>
// For sunpro, it chokes if time.h is included through stat.h
#endif

#include <fstream>

#ifdef __CYGWIN__
#  define __int64 long long
#endif

#if defined (_STLP_USE_UNIX_IO)
extern "C" {
// open/close/read/write
#  include <sys/stat.h>           // For stat
#  if !defined (_CRAY) && ! defined (__EMX__)
#    include <sys/mman.h>           // For mmap
#  endif

//  on HP-UX 11, this one contradicts with pthread.h on pthread_atfork, unless we unset this
#  if defined (__hpux) && defined (__GNUC__)
#    undef _INCLUDE_POSIX1C_SOURCE
#  endif

#  include <unistd.h>
#  include <fcntl.h>
}
#  ifdef __APPLE__
#    include <sys/sysctl.h>
#  endif
#elif defined (_STLP_USE_WIN32_IO)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>

#  ifdef __BORLANDC__
#      include <cfcntl.h>            // For _O_RDONLY, etc
#    include <sys/stat.h>         // For _fstat
#  elif !defined(_STLP_WCE)
#    include <io.h>               // For _get_osfhandle
#    include <fcntl.h>            // For _O_RDONLY, etc
#    include <sys/stat.h>         // For _fstat
#  endif
#  define _TEXTBUF_SIZE 0x1000
#elif defined (_STLP_USE_UNIX_EMULATION_IO)
#  if defined( __MSL__ )
#    include <unistd.h>
#  else
#    include <io.h>
#  endif
#  include <fcntl.h>
#  include <sys/stat.h>
#elif defined (_STLP_USE_STDIO_IO)
#  include <cstdio>
#  if !(defined(__MRC__) || defined(__SC__) || defined(__ISCPP__) )
extern "C" {
#    include <sys/stat.h>
}
#  endif
#  if defined( __MSL__ )
#    include <unix.h>
#  endif
#  if defined(__ISCPP__)
#    include <c_locale_is/filestat.h>
#  endif
#  if defined(__BEOS__) && defined(__INTEL__)
#    include <fcntl.h>
#    include <sys/stat.h>         // For _fstat
#    define _S_IREAD S_IREAD
#    define _S_IWRITE S_IWRITE
#    define _S_IFREG S_IFREG
#  endif
#else
#  error "Configure i/o !"
#endif

#if defined (_STLP_USE_WIN32_IO)
const _STLP_fd INVALID_STLP_FD = INVALID_HANDLE_VALUE;
#  if !defined (INVALID_SET_FILE_POINTER)
#    define INVALID_SET_FILE_POINTER 0xffffffff
#  endif
#elif defined (_STLP_USE_UNIX_EMULATION_IO) || defined (_STLP_USE_STDIO_IO) || defined (_STLP_USE_UNIX_IO)
const _STLP_fd INVALID_STLP_FD = -1;
#else
#  error "Configure i/o !"
#endif

// map permission masks
#if defined (_STLP_USE_UNIX_EMULATION_IO) || defined (_STLP_USE_STDIO_IO)
#  ifndef S_IRUSR
#    define S_IRUSR _S_IREAD
#    define S_IWUSR _S_IWRITE
#    define S_IRGRP _S_IREAD
#    define S_IWGRP _S_IWRITE
#    define S_IROTH _S_IREAD
#    define S_IWOTH _S_IWRITE
#  endif
#  ifndef O_RDONLY
#    define O_RDONLY _O_RDONLY
#    define O_WRONLY _O_WRONLY
#    define O_RDWR   _O_RDWR
#    define O_APPEND _O_APPEND
#    define O_CREAT  _O_CREAT
#    define O_TRUNC  _O_TRUNC
#    define O_TEXT   _O_TEXT
#    define O_BINARY _O_BINARY
#  endif

#  ifdef __MSL__
#    define _O_TEXT 0x0
#    if !defined( O_TEXT )
#      define O_TEXT _O_TEXT
#    endif
#    define _S_IFREG S_IFREG
#    define S_IREAD        S_IRUSR
#    define S_IWRITE       S_IWUSR
#    define S_IEXEC        S_IXUSR
#    define _S_IWRITE S_IWRITE
#    define _S_IREAD S_IREAD
#    define _open open
#    define _lseek lseek
#    define _close close
#    define _read read
#    define _write write
#  endif
#endif

#ifndef O_ACCMODE
#  define O_ACCMODE (O_RDONLY|O_WRONLY|O_RDWR)
#endif

#include "fstream_impl.h"

#ifdef _STLP_LONG_LONG
#  define ULL(x) ((unsigned _STLP_LONG_LONG)x)
#elif defined(__MRC__) || defined(__SC__)    //*TY 02/25/2000 - added support for MPW compilers
#  include <Math64.h>
#  define ULL(x) (U64SetU(x))
#elif defined(__ISCPP__)
#  include "uint64.h"
#else
#  error "there should be some long long type on the system!"
#endif

_STLP_BEGIN_NAMESPACE
// Compare with streamoff definition in stl/char_traits.h!

#ifdef _STLP_USE_DEFAULT_FILE_OFFSET
#  define FOPEN fopen
#  define FSEEK fseek
#  define FSTAT fstat
#  define STAT  stat
#  define FTELL ftell
#  define LSEEK lseek
#  define MMAP  mmap
#  define OPEN  open
#elif defined(_LARGEFILE_SOURCE) || defined(_LARGEFILE64_SOURCE) /* || defined(__USE_FILE_OFFSET64) */ \
      /* || (defined(_FILE_OFFSET_BITS) && (_FILE_OFFSET_BITS == 64)) */ /* || defined(__sgi) */
#  define FOPEN fopen64
#  define FSEEK fseeko64
#  define FSTAT fstat64
#  define STAT  stat64
#  define FTELL ftello64
#  define LSEEK lseek64
#  define MMAP  mmap64
#  define OPEN  open64
#else
#  define OPEN  open
#  define FSEEK fseek
#  define FSTAT fstat
#  define STAT  stat
#  define FTELL ftell
#  define LSEEK lseek
#  define MMAP  mmap
#  define OPEN  open
#endif
#ifdef _STLP_USE_UNIX_IO
#  ifndef MAP_FAILED /* MMAP failure return code */
#    define MAP_FAILED -1
#  endif
#elif defined (_STLP_USE_UNIX_EMULATION_IO)
#  define LSEEK _lseek
#endif

#if !defined(__MSL__) && !defined(__MRC__) && !defined(__SC__) && !defined(_STLP_WCE)    //*TY 04/15/2000 - exclude mpw compilers also
static ios_base::openmode flag_to_openmode(int mode) {
  ios_base::openmode ret = ios_base::__default_mode;

  switch(mode & O_ACCMODE) {
  case O_RDONLY:
    ret = ios_base::in; break;
  case O_WRONLY:
    ret = ios_base::out; break;
  case O_RDWR:
    ret = ios_base::in | ios_base::out; break;
  }

  if (mode & O_APPEND)
    ret |= ios_base::app;

#  if defined (_STLP_USE_WIN32_IO)
  if (mode & O_BINARY)
    ret |= ios_base::binary;
#  endif

  return ret;
}
#endif /* MSL */

_STLP_MOVE_TO_PRIV_NAMESPACE

// Helper functions for _Filebuf_base.

bool __is_regular_file(_STLP_fd fd) {

#if defined (_STLP_UNIX)

  struct STAT buf;
  return FSTAT(fd, &buf) == 0 && S_ISREG(buf.st_mode);

#elif defined(__MRC__) || defined(__SC__)    //*TY 02/25/2000 - added support for MPW compilers

#  pragma unused(fd)
  return true;  // each file is a regular file under mac os, isn't it? (we don't have fstat())

#elif defined(_STLP_USE_UNIX_EMULATION_IO) || defined (_STLP_USE_STDIO_IO)

  struct STAT buf;
  return FSTAT(fd, &buf) == 0 && (buf.st_mode & _S_IFREG) != 0 ;

#elif defined (_STLP_USE_WIN32_IO) && !defined(_STLP_WCE)

  return (GetFileType(fd) & ~FILE_TYPE_REMOTE) == FILE_TYPE_DISK;

#else
  (void)fd;    // dwa 4/27/00 - suppress unused parameter warning
  return false;
#endif
}

// Number of characters in the file.
streamoff __file_size(_STLP_fd fd) {
  streamoff ret = 0;

#if defined (_STLP_UNIX)

  struct STAT buf;
  if (FSTAT(fd, &buf) == 0 && S_ISREG(buf.st_mode))
    ret = buf.st_size > 0 ? buf.st_size : 0;

#elif defined(__MRC__) || defined(__SC__)    //*TY 02/25/2000 - added support for MPW compilers

#  pragma unused(fd)

#elif defined(_STLP_USE_UNIX_EMULATION_IO) || defined (_STLP_USE_STDIO_IO)

  struct STAT buf;
  if (FSTAT(fd, &buf) == 0 && (buf.st_mode & _S_IFREG) != 0)
    ret = buf.st_size > 0 ? buf.st_size : 0;

#elif defined (_STLP_USE_WIN32_IO)

 LARGE_INTEGER li;
 li.LowPart = GetFileSize(fd, (unsigned long*) &li.HighPart);
 if (li.LowPart != INVALID_FILE_SIZE || GetLastError() == NO_ERROR)
   ret = li.QuadPart;

#else
  (void)fd;    // dwa 4/27/00 - suppress unused parameter warning
#endif
  return ret;
}

_STLP_MOVE_TO_STD_NAMESPACE

// Visual C++ and Intel use this, but not Metrowerks
// Also MinGW, msvcrt.dll (but not crtdll.dll) dependent version
#if (!defined (__MSL__) && !defined (_STLP_WCE) && defined (_STLP_MSVC_LIB) && defined (_WIN32)) || \
    (defined (__MINGW32__) && defined (__MSVCRT__))

// fcntl(fileno, F_GETFL) for Microsoft library
// 'semi-documented' defines:
#  define IOINFO_L2E          5
#  define IOINFO_ARRAY_ELTS   (1 << IOINFO_L2E)
#  define _pioinfo(i) ( __pioinfo[(i) >> IOINFO_L2E] + \
              ((i) & (IOINFO_ARRAY_ELTS - 1)) )
#  define FAPPEND         0x20    // O_APPEND flag
#  define FTEXT           0x80    // O_TEXT flag
// end of 'semi-documented' defines

// 'semi-documented' internal structure
extern "C" {
  struct ioinfo {
    long osfhnd;    // the real os HANDLE
    char osfile;    // file handle flags
    char pipech;    // pipe buffer
#  if defined (_MT)
    // multi-threaded locking
    int lockinitflag;
    CRITICAL_SECTION lock;
#  endif  /* _MT */
  };
#  if defined (__MINGW32__)
 __MINGW_IMPORT ioinfo * __pioinfo[];
#  else
  extern _CRTIMP ioinfo * __pioinfo[];
#  endif
} // extern "C"
// end of 'semi-documented' declarations

static ios_base::openmode _get_osfflags(int fd, HANDLE oshandle) {
  char dosflags = 0;
  if (fd >= 0)
    dosflags = _pioinfo(fd)->osfile;
  //else
    //the file will be considered as open in binary mode with no append attribute
  // end of 'semi-documented' stuff

  int mode = 0;
  if (dosflags & FAPPEND)
    mode |= O_APPEND;

  if (dosflags & FTEXT)
    mode |= O_TEXT;
  else
    mode |= O_BINARY;

  // For Read/Write access we have to guess
  DWORD dummy, dummy2;
  BOOL writeOk = WriteFile(oshandle, &dummy2, 0, &dummy, 0);
  BOOL readOk = ReadFile(oshandle, &dummy2, 0, &dummy, NULL);
  if (writeOk && readOk)
    mode |= O_RDWR;
  else if (readOk)
    mode |= O_RDONLY;
  else
    mode |= O_WRONLY;

  return flag_to_openmode(mode);
}

#elif defined (__DMC__)

#  define FHND_APPEND 0x04
#  define FHND_DEVICE 0x08
#  define FHND_TEXT   0x10

extern "C" unsigned char __fhnd_info[_NFILE];

static ios_base::openmode _get_osfflags(int fd, HANDLE oshandle) {
  int mode = 0;

  if (__fhnd_info[fd] & FHND_APPEND)
    mode |= O_APPEND;

  if (__fhnd_info[fd] & FHND_TEXT == 0)
    mode |= O_BINARY;

  for (FILE *fp = &_iob[0]; fp < &_iob[_NFILE]; fp++) {
    if ((fileno(fp) == fd) && (fp->_flag & (_IOREAD | _IOWRT | _IORW))) {
      const int osflags = fp->_flag;

      if ((osflags & _IOREAD) && !(osflags & _IOWRT) && !(osflags & _IORW))
        mode |= O_RDONLY;
      else if ((osflags & _IOWRT) && !(osflags & _IOREAD) && !(osflags & _IORW))
        mode |= O_WRONLY;
      else
        mode |= O_RDWR;
      break;
    }
  }

  return flag_to_openmode(mode);
}
#endif

size_t _Filebuf_base::_M_page_size = 4096;

_Filebuf_base::_Filebuf_base()
  : _M_file_id(INVALID_STLP_FD),
#if defined (_STLP_USE_WIN32_IO)
    _M_view_id(0),
#endif
    _M_openmode(0),
    _M_is_open(false),
    _M_should_close(false)
{}

void _Filebuf_base::_S_initialize() {
#if defined (_STLP_UNIX)  && !defined (__DJGPP) && !defined (_CRAY)
#  if defined (__APPLE__)
  int mib[2];
  size_t pagesize, len;
  mib[0] = CTL_HW;
  mib[1] = HW_PAGESIZE;
  len = sizeof(pagesize);
  sysctl(mib, 2, &pagesize, &len, NULL, 0);
  _M_page_size = pagesize;
#  elif defined (__DJGPP) && defined (_CRAY)
  _M_page_size = BUFSIZ;
#  else
  _M_page_size = sysconf(_SC_PAGESIZE);
#  endif
#elif defined (_STLP_USE_WIN32_IO)
  SYSTEM_INFO SystemInfo;
  GetSystemInfo(&SystemInfo);
  _M_page_size = SystemInfo.dwPageSize;
  // might be .dwAllocationGranularity
#endif
}

// Return the size of the file.  This is a wrapper for stat.
// Returns zero if the size cannot be determined or is ill-defined.
streamoff _Filebuf_base::_M_file_size() {
  return _STLP_PRIV __file_size(_M_file_id);
}

bool _Filebuf_base::_M_open(const char* name, ios_base::openmode openmode,
                            long permission) {
  _STLP_fd file_no;

  if (_M_is_open)
    return false;

#if defined (_STLP_USE_UNIX_IO) || defined (_STLP_USE_UNIX_EMULATION_IO)
  int flags = 0;

  // Unix makes no distinction between text and binary files.
  switch(openmode & (~ios_base::ate & ~ios_base::binary)) {
  case ios_base::out:
  case ios_base::out | ios_base::trunc:
    flags = O_WRONLY | O_CREAT | O_TRUNC;
    break;
  case ios_base::out | ios_base::app:
    flags = O_WRONLY | O_CREAT | O_APPEND;
    break;
  case ios_base::in:
    flags = O_RDONLY;
    permission = 0;             // Irrelevant unless we're writing.
    break;
  case ios_base::in | ios_base::out:
    flags = O_RDWR;
    break;
  case ios_base::in | ios_base::out | ios_base::trunc:
    flags = O_RDWR | O_CREAT | O_TRUNC;
    break;
  default:                      // The above are the only combinations of
    return false;               // flags allowed by the C++ standard.
  }

#  if defined (_STLP_USE_UNIX_EMULATION_IO)
  if (openmode & ios_base::binary)
    flags |= O_BINARY;
  else
    flags |= O_TEXT;

  file_no = _open(name, flags, permission);
#  else
  file_no = OPEN(name, flags, permission);
#  endif /* _STLP_USE_UNIX_EMULATION_IO */

  if (file_no < 0)
    return false;

  _M_is_open = true;

  if (openmode & ios_base::ate)
    if (LSEEK(file_no, 0, SEEK_END) == -1)
      _M_is_open = false;

#elif defined (_STLP_USE_STDIO_IO)
  // use FILE-based i/o
  const char* flags;

  switch(openmode & (~ios_base::ate)) {
  case ios_base::out:
  case ios_base::out | ios_base::trunc:
    flags = "w";
    break;

  case ios_base::out | ios_base::binary:
  case ios_base::out | ios_base::trunc | ios_base::binary:
    flags = "wb";
    break;

  case ios_base::out | ios_base::app:
    flags = "a";
    break;

  case ios_base::out | ios_base::app | ios_base::binary:
    flags = "ab";
    break;

  case ios_base::in:
    flags = "r";
    break;

  case ios_base::in | ios_base::binary:
    flags = "rb";
    break;

  case ios_base::in | ios_base::out:
    flags = "r+";
    break;

  case ios_base::in | ios_base::out | ios_base::binary:
    flags = "r+b";
    break;


  case ios_base::in | ios_base::out | ios_base::trunc:
    flags = "w+";
    break;

  case ios_base::in | ios_base::out | ios_base::trunc | ios_base::binary:
    flags = "w+b";
    break;

  default:                      // The above are the only combinations of
    return false;               // flags allowed by the C++ standard.
  }

  // fbp : TODO : set permissions !
  (void)permission; // currently unused    //*TY 02/26/2000 - added to suppress warning message
  _M_file = FOPEN(name, flags);

  if (_M_file) {
    file_no = fileno(_M_file);
  }
  else
    return false;

  // unset buffering immediately
  setbuf(_M_file, 0);

  _M_is_open = true;

  if (openmode & ios_base::ate) {
    if (FSEEK(_M_file, 0, SEEK_END) == -1)
      _M_is_open = false;
  }

#elif defined (_STLP_USE_WIN32_IO)
  DWORD dwDesiredAccess, dwCreationDisposition;
  bool doTruncate = false;

  switch (openmode & (~ios_base::ate & ~ios_base::binary)) {
  case ios_base::out:
  case ios_base::out | ios_base::trunc:
    dwDesiredAccess = GENERIC_WRITE;
    dwCreationDisposition = OPEN_ALWAYS;
    // boris : even though it is very non-intuitive, standard
    // requires them both to behave same.
    doTruncate = true;
    break;
  case ios_base::out | ios_base::app:
    dwDesiredAccess = GENERIC_WRITE;
    dwCreationDisposition = OPEN_ALWAYS;
    break;
  case ios_base::in:
    dwDesiredAccess = GENERIC_READ;
    dwCreationDisposition = OPEN_EXISTING;
    permission = 0;             // Irrelevant unless we're writing.
    break;
  case ios_base::in | ios_base::out:
    dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
    dwCreationDisposition = OPEN_EXISTING;
    break;
  case ios_base::in | ios_base::out | ios_base::trunc:
    dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
    dwCreationDisposition = OPEN_ALWAYS;
    doTruncate = true;
    break;
  default:                      // The above are the only combinations of
    return false;               // flags allowed by the C++ standard.
  }

  DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;

#  if defined(_STLP_USE_WIDE_INTERFACE)
    file_no = CreateFile (_STLP_PRIV __ASCIIToWide(name).c_str(),
#  else
    file_no = CreateFileA(name,
#  endif
                          dwDesiredAccess, dwShareMode, 0,
                          dwCreationDisposition, permission, 0);

  if (file_no == INVALID_STLP_FD)
    return false;

  if ((doTruncate && SetEndOfFile(file_no) == 0) ||
      (((openmode & ios_base::ate) != 0) &&
       (SetFilePointer(file_no, 0, NULL, FILE_END) == INVALID_SET_FILE_POINTER))) {
    CloseHandle(file_no);
    return false;
  }

  _M_is_open = true;

#else
#  error "Port!"
#endif /* __unix */

  _M_file_id = file_no;
  _M_should_close = _M_is_open;
  _M_openmode = openmode;

  if (_M_is_open)
    _M_regular_file = _STLP_PRIV __is_regular_file(_M_file_id);

  return (_M_is_open != 0);
}


bool _Filebuf_base::_M_open(const char* name, ios_base::openmode openmode) {
  // This doesn't really grant everyone in the world read/write
  // access.  On Unix, file-creation system calls always clear
  // bits that are set in the umask from the permissions flag.
#ifdef _STLP_USE_WIN32_IO
  return this->_M_open(name, openmode, FILE_ATTRIBUTE_NORMAL);
#elif defined(__MRC__) || defined(__SC__)    //*TY 02/26/2000 - added support for MPW compilers
  return this->_M_open(name, openmode, 0);
#else
  return this->_M_open(name, openmode, S_IRUSR | S_IWUSR | S_IRGRP |
                                       S_IWGRP | S_IROTH | S_IWOTH);
#endif
}


#if defined (_STLP_USE_WIN32_IO)
bool _Filebuf_base::_M_open(_STLP_fd __id, ios_base::openmode init_mode) {
#  if (defined (_STLP_MSVC_LIB) && !defined (_STLP_WCE)) || \
      (defined (__MINGW32__) && defined (__MSVCRT__)) || defined (__DMC__)

  if (_M_is_open || __id == INVALID_STLP_FD)
    return false;

  if (init_mode != ios_base::__default_mode)
    _M_openmode = init_mode;
  else
    _M_openmode = _get_osfflags(-1, __id);

  _M_is_open = true;
  _M_file_id = __id;
  _M_should_close = false;
  _M_regular_file = _STLP_PRIV __is_regular_file(_M_file_id);

  return true;
#  else
  (void)__id;
  (void)init_mode;    // dwa 4/27/00 - suppress unused parameter warning

  // not available for the API
  return false;

#  endif
}
#endif /* _STLP_USE_WIN32_IO */

// Associated the filebuf with a file descriptor pointing to an already-
// open file.  Mode is set to be consistent with the way that the file
// was opened.
bool _Filebuf_base::_M_open(int file_no, ios_base::openmode init_mode) {
  if (_M_is_open || file_no < 0)
    return false;

#if defined (_STLP_UNIX)
  (void)init_mode;    // dwa 4/27/00 - suppress unused parameter warning
  int mode ;
  mode = fcntl(file_no, F_GETFL);

  if (mode == -1)
    return false;

  _M_openmode = flag_to_openmode(mode);
  _M_file_id = file_no;
#elif defined(__MRC__) || defined(__SC__)    //*TY 02/26/2000 - added support for MPW compilers
  (void)init_mode;    // dwa 4/27/00 - suppress unused parameter warning
  switch (_iob[file_no]._flag & (_IOREAD|_IOWRT|_IORW) )
  {
  case _IOREAD:
    _M_openmode = ios_base::in; break;
  case _IOWRT:
    _M_openmode = ios_base::out; break;
  case _IORW:
    _M_openmode = ios_base::in | ios_base::out; break;
  default:
    return false;
  }
  _M_file_id = file_no;
#elif defined (_STLP_USE_UNIX_EMULATION_IO) || defined (_STLP_USE_STDIO_IO)
  (void)init_mode;    // dwa 4/27/00 - suppress unused parameter warning
  int mode ;
  struct STAT buf;
  if (FSTAT(file_no, &buf) != 0)
    return false;
  mode = buf.st_mode;

  switch(mode & (_S_IWRITE | _S_IREAD) ) {
  case _S_IREAD:
    _M_openmode = ios_base::in; break;
  case _S_IWRITE:
    _M_openmode = ios_base::out; break;
  case (_S_IWRITE | _S_IREAD):
    _M_openmode = ios_base::in | ios_base::out; break;
  default:
    return false;
  }
  _M_file_id = file_no;
#elif (defined (_STLP_USE_WIN32_IO) && defined (_STLP_MSVC_LIB) && !defined (_STLP_WCE) ) || \
      (defined (__MINGW32__) && defined (__MSVCRT__)) || \
       defined (__DMC__)

  HANDLE oshandle = (HANDLE)_get_osfhandle(file_no);
  if (oshandle == INVALID_STLP_FD)
    return false;

  if (init_mode != ios_base::__default_mode)
    _M_openmode = init_mode;
  else
    _M_openmode = _get_osfflags(file_no, oshandle);

  _M_file_id = oshandle;
#else
  (void)init_mode;    // dwa 4/27/00 - suppress unused parameter warning
  // not available for the API
#  define _STLP_NO_IMPLEMENTATION
#endif

#if !defined (_STLP_NO_IMPLEMENTATION)
  _M_is_open = true;
  _M_should_close = false;
  _M_regular_file = _STLP_PRIV __is_regular_file(_M_file_id);
  return true;
#else
#  undef _STLP_NO_IMPLEMENTATION
  return false;
#endif
}

bool _Filebuf_base::_M_close() {
  if (!_M_is_open)
    return false;

  bool ok;

  if (!_M_should_close)
    ok = true;
  else {

#if defined (_STLP_USE_UNIX_IO)

    ok = (close(_M_file_id) == 0);

#elif defined (_STLP_USE_UNIX_EMULATION_IO)

    ok = (_close(_M_file_id) == 0);

#elif defined (_STLP_USE_STDIO_IO)

    ok = (fclose(_M_file) == 0);

#elif defined (_STLP_USE_WIN32_IO)

    if (_M_file_id != INVALID_STLP_FD) {
      ok = (CloseHandle(_M_file_id) != 0);
    }
    else {
      ok = false;
    }

#else

    ok = false;

#endif /* _STLP_USE_UNIX_IO */
  }

  _M_is_open = _M_should_close = false;
  _M_openmode = 0;
  return ok;
}


#define _STLP_LF 10
#define _STLP_CR 13
#define _STLP_CTRLZ 26

// Read up to n characters into a buffer.  Return value is number of
// characters read.
ptrdiff_t _Filebuf_base::_M_read(char* buf, ptrdiff_t n) {
#if defined (_STLP_USE_UNIX_IO)

  return read(_M_file_id, buf, n);

#elif defined (_STLP_USE_UNIX_EMULATION_IO)

  return _read(_M_file_id, buf, n);

#elif defined (_STLP_USE_WIN32_IO)
  ptrdiff_t readen = 0;
  //Here cast to size_t is safe as n cannot be negative.
  size_t chunkSize = (min)(size_t(0xffffffff), __STATIC_CAST(size_t, n));
  // The following, while validating that we are still able to extract chunkSize
  // charaters to the buffer, avoids extraction of too small chunk of datas
  // which would be counter performant.
  while (__STATIC_CAST(size_t, (n - readen)) >= chunkSize) {
    DWORD NumberOfBytesRead;
    ReadFile(_M_file_id, buf + readen, __STATIC_CAST(DWORD, chunkSize), &NumberOfBytesRead, 0);

    if (NumberOfBytesRead == 0)
      break;

    if (!(_M_openmode & ios_base::binary)) {
      // translate CR-LFs to LFs in the buffer
      char *to = buf + readen;
      char *from = to;
      char *last = from + NumberOfBytesRead - 1;
      for (; from <= last && *from != _STLP_CTRLZ; ++from) {
        if (*from != _STLP_CR)
          *to++ = *from;
        else { // found CR
          if (from < last) { // not at buffer end
            if (*(from + 1) != _STLP_LF)
              *to++ = _STLP_CR;
          }
          else { // last char is CR, peek for LF
            char peek = ' ';
            DWORD NumberOfBytesPeeked;
            ReadFile(_M_file_id, (LPVOID)&peek, 1, &NumberOfBytesPeeked, 0);
            if (NumberOfBytesPeeked != 0) {
              if (peek != _STLP_LF) { //not a <CR><LF> combination
                *to++ = _STLP_CR;
                if ((to < buf + n) && (peek != _STLP_CR))
                  //We have enough place to store peek and it is no a special
                  //_STLP_CR character, we can store it.
                  *to++ = peek;
                else
                  SetFilePointer(_M_file_id, (LONG)-1, 0, SEEK_CUR);
              }
              else {
                // A <CR><LF> combination, we keep the <LF>:
                *to++ = _STLP_LF;
              }
            }
            else {
              /* This case is tedious, we could
               *  - put peek back in the file but this would then generate an infinite loop
               *  - report an error as we don't know if in a future call to ReadFile we won't then
               *    get a <LF>. Doing so would make all files with a <CR> last an invalid file
               *    for STLport, a hard solution for STLport clients.
               *  - store the <CR> in the returned buffer, the chosen solution, even if in this
               *    case we could miss a <CR><LF> combination.
               */
              *to++ = _STLP_CR;
            }
          }
        } // found CR
      } // for
      // seek back to TEXT end of file if hit CTRL-Z
      if (from <= last) // terminated due to CTRLZ
        SetFilePointer(_M_file_id, (LONG)((last+1) - from), 0, SEEK_CUR);
      readen += to - (buf + readen);
    }
    else
      readen += NumberOfBytesRead;
  }
  return readen;

#elif defined (_STLP_USE_STDIO_IO)

  return fread(buf, 1, n, _M_file);

#else
#  error "Port!"
#endif /* __unix */
}

// Write n characters from a buffer.  Return value: true if we managed
// to write the entire buffer, false if we didn't.
bool _Filebuf_base::_M_write(char* buf, ptrdiff_t n) {
  for (;;) {
    ptrdiff_t written;

#if defined (_STLP_USE_UNIX_IO)

    written = write(_M_file_id, buf, n);

#elif defined (_STLP_USE_UNIX_EMULATION_IO)

    written = _write(_M_file_id, buf, n);

#elif defined (_STLP_USE_WIN32_IO)

    //In the following implementation we are going to cast most of the ptrdiff_t
    //values in size_t to work with coherent unsigned values. Doing so make code
    //more simple especially in the min function call.

    // In append mode, every write does an implicit seek to the end
    // of the file.
    if (_M_openmode & ios_base::app)
      _M_seek(0, ios_base::end);

    if (_M_openmode & ios_base::binary) {
      // binary mode
      size_t bytes_to_write = (size_t)n;
      DWORD NumberOfBytesWritten;
      written = 0;
      for (; bytes_to_write != 0;) {
        WriteFile(_M_file_id, buf + written,
                  __STATIC_CAST(DWORD, (min)(size_t(0xffffffff), bytes_to_write)),
                  &NumberOfBytesWritten, 0);
        if (NumberOfBytesWritten == 0)
          return false;
        bytes_to_write -= NumberOfBytesWritten;
        written += NumberOfBytesWritten;
      }
    }
    else {
      char textbuf[_TEXTBUF_SIZE + 1]; // extra 1 in case LF at end
      char * nextblock = buf, * ptrtextbuf = textbuf;
      char * endtextbuf = textbuf + _TEXTBUF_SIZE;
      char * endblock = buf + n;
      ptrdiff_t nextblocksize = (min) (n, (ptrdiff_t)_TEXTBUF_SIZE);
      char * nextlf;

      while ( (nextblocksize > 0) &&
              (nextlf = (char *)memchr(nextblock, _STLP_LF, nextblocksize)) != 0) {
        ptrdiff_t linelength = nextlf - nextblock;
        memcpy(ptrtextbuf, nextblock, linelength);
        ptrtextbuf += linelength;
        nextblock += (linelength + 1);
        * ptrtextbuf ++ = _STLP_CR;
        * ptrtextbuf ++ = _STLP_LF;
        nextblocksize = (min) (ptrdiff_t(endblock - nextblock),
                               (max) (ptrdiff_t(0), ptrdiff_t(endtextbuf - ptrtextbuf)));
      }
      // write out what's left, > condition is here since for LF at the end ,
      // endtextbuf may get < ptrtextbuf ...
      if (nextblocksize > 0) {
        memcpy(ptrtextbuf, nextblock, nextblocksize);
        ptrtextbuf += nextblocksize;
        nextblock += nextblocksize;
      }
      // now write out the translated buffer
      char * writetextbuf = textbuf;
      for (size_t NumberOfBytesToWrite = (size_t)(ptrtextbuf - textbuf);
           NumberOfBytesToWrite;) {
        DWORD NumberOfBytesWritten;
        WriteFile((HANDLE)_M_file_id, writetextbuf,
                  __STATIC_CAST(DWORD, (min)(size_t(0xffffffff), NumberOfBytesToWrite)),
                  &NumberOfBytesWritten, 0);
        if (!NumberOfBytesWritten) // write shortfall
          return false;
        writetextbuf += NumberOfBytesWritten;
        NumberOfBytesToWrite -= NumberOfBytesWritten;
      }
      // count non-translated characters
      written = (nextblock - buf);
    }

#elif defined (_STLP_USE_STDIO_IO)

    written = fwrite(buf, 1, n, _M_file);

#else
#  error "Port!"
#endif /* __unix */

    if (n == written)
      return true;
    else if (written > 0 && written < n) {
      n -= written;
      buf += written;
    }
    else
      return false;
  }
}


#ifdef _STLP_USE_WIN32_IO
#  define STL_SEEK_SET FILE_BEGIN
#  define STL_SEEK_CUR FILE_CURRENT
#  define STL_SEEK_END FILE_END
#else
#  define STL_SEEK_SET SEEK_SET
#  define STL_SEEK_CUR SEEK_CUR
#  define STL_SEEK_END SEEK_END
#endif

// Wrapper for lseek or the like.
streamoff _Filebuf_base::_M_seek(streamoff offset, ios_base::seekdir dir) {
  streamoff result = -1;
  int whence;

  switch(dir) {
  case ios_base::beg:
    if (offset < 0 /* || offset > _M_file_size() */ )
      return streamoff(-1);
    whence = STL_SEEK_SET;
    break;
  case ios_base::cur:
    whence = STL_SEEK_CUR;
    break;
  case ios_base::end:
    if (/* offset > 0 || */  -offset > _M_file_size() )
      return streamoff(-1);
    whence = STL_SEEK_END;
    break;
  default:
    return streamoff(-1);
  }

#if defined (_STLP_USE_UNIX_IO) || defined (_STLP_USE_UNIX_EMULATION_IO)

  result = LSEEK(_M_file_id, offset, whence);

#elif defined (_STLP_USE_STDIO_IO)

  result = FSEEK(_M_file, offset, whence);

#elif defined (_STLP_USE_WIN32_IO)

  LARGE_INTEGER li;
  li.QuadPart = offset;
  li.LowPart = SetFilePointer(_M_file_id, li.LowPart, &li.HighPart, whence);
  if (li.LowPart != INVALID_SET_FILE_POINTER || GetLastError() == NO_ERROR)
    result = li.QuadPart;

#else
#  error "Port!"
#endif

  return result;
}


// Attempts to memory-map len bytes of the current file, starting
// at position offset.  Precondition: offset is a multiple of the
// page size.  Postcondition: return value is a null pointer if the
// memory mapping failed.  Otherwise the return value is a pointer to
// the memory-mapped file and the file position is set to offset.
void* _Filebuf_base::_M_mmap(streamoff offset, streamoff len) {
  void* base;
#if defined (_STLP_UNIX) && !defined(__DJGPP) && !defined(_CRAY)
  base = MMAP(0, len, PROT_READ, MAP_PRIVATE, _M_file_id, offset);
  if (base != (void*)MAP_FAILED) {
    if (LSEEK(_M_file_id, offset + len, SEEK_SET) < 0) {
      this->_M_unmap(base, len);
      base = 0;
    }
  } else
    base =0;

#elif defined (_STLP_USE_WIN32_IO)
  _M_view_id = CreateFileMapping(_M_file_id, (PSECURITY_ATTRIBUTES)0 ,
                                 PAGE_READONLY, 0 /* len >> 32 */ ,
                                 0 /* len & 0xFFFFFFFF */ , // low-order DWORD of size
                                 0);

  if (_M_view_id) {
#  if 0
/*
    printf("view %x created from file %x, error = %d, size = %d, map_offset = %d map_len = %d\n",
     _M_view_id, _M_file_id, GetLastError(),
     (int)cur_filesize, ULL(offset) & 0xffffffff, len);
*/
#  endif
    base = MapViewOfFile(_M_view_id, FILE_MAP_READ, __STATIC_CAST(DWORD, ULL(offset) >> 32),
                         __STATIC_CAST(DWORD, ULL(offset) & 0xffffffff),
#  if !defined (__DMC__)
                         __STATIC_CAST(SIZE_T, len));
#  else
                         __STATIC_CAST(DWORD, len));
#  endif
    // check if mapping succeded and is usable
    if (base == 0  || _M_seek(offset + len, ios_base::beg) < 0) {
      this->_M_unmap(base, len);
      base = 0;
    }
  } else
    base = 0;
#else
  (void)len;    //*TY 02/26/2000 - unused variables
  (void)offset;    //*TY 02/26/2000 -
  base = 0;
#endif
  return base;
}

void _Filebuf_base::_M_unmap(void* base, streamoff len) {
  // precondition : there is a valid mapping at the moment
#if defined (_STLP_UNIX)  && !defined(__DJGPP) && !defined(_CRAY)
  munmap((char*)base, len);
#elif defined (_STLP_USE_WIN32_IO)
  if (base != NULL)
    UnmapViewOfFile(base);
  // destroy view handle as well
  if (_M_view_id != NULL)
    CloseHandle(_M_view_id);
  _M_view_id = NULL;
  (void)len; //unused variable
#else
  (void)len;    //*TY 02/26/2000 - unused variables
  (void)base;   //*TY 02/26/2000 -
#endif
}

// fbp : let us map 1 MB maximum, just be sure not to trash VM
#define MMAP_CHUNK 0x100000L

int _STLP_CALL
_Underflow<char, char_traits<char> >::_M_doit (basic_filebuf<char, char_traits<char> >* __this) {
  if (!__this->_M_in_input_mode) {
    if (!__this->_M_switch_to_input_mode())
      return traits_type::eof();
  }
  else if (__this->_M_in_putback_mode) {
    __this->_M_exit_putback_mode();
    if (__this->gptr() != __this->egptr()) {
      int_type __c = traits_type::to_int_type(*__this->gptr());
      return __c;
    }
  }

  // If it's a disk file, and if the internal and external character
  // sequences are guaranteed to be identical, then try to use memory
  // mapped I/O.  Otherwise, revert to ordinary read.
  if (__this->_M_base.__regular_file()
      && __this->_M_always_noconv
      && __this->_M_base._M_in_binary_mode()) {
    // If we've mmapped part of the file already, then unmap it.
    if (__this->_M_mmap_base)
      __this->_M_base._M_unmap(__this->_M_mmap_base, __this->_M_mmap_len);
    __this->_M_mmap_base = 0;
    __this->_M_mmap_len = 0;

    // Determine the position where we start mapping.  It has to be
    // a multiple of the page size.
    streamoff __cur = __this->_M_base._M_seek(0, ios_base::cur);
    streamoff __size = __this->_M_base._M_file_size();
    if (__size > 0 && __cur >= 0 && __cur < __size) {
      streamoff __offset = (__cur / __this->_M_base.__page_size()) * __this->_M_base.__page_size();
      streamoff __remainder = __cur - __offset;

      __this->_M_mmap_len = __size - __offset;

      if (__this->_M_mmap_len > MMAP_CHUNK)
        __this->_M_mmap_len = MMAP_CHUNK;

      if ((__this->_M_mmap_base =
        __this->_M_base._M_mmap(__offset, __this->_M_mmap_len)) != 0) {
        __this->setg((char*) __this->_M_mmap_base,
                     (char*) __this->_M_mmap_base + __STATIC_CAST(ptrdiff_t, __remainder),
                     (char*) __this->_M_mmap_base + __STATIC_CAST(ptrdiff_t, __this->_M_mmap_len));
        return traits_type::to_int_type(*__this->gptr());
      }
    } else /* size > 0 ... */ {
      // There is nothing to map. We unmapped the file above, now zap pointers.
      __this->_M_mmap_base = 0;
      __this->_M_mmap_len = 0;
    }
  }

  return __this->_M_underflow_aux();
}


//----------------------------------------------------------------------
// Force instantiation of filebuf and fstream classes.
#if !defined(_STLP_NO_FORCE_INSTANTIATE)

template class basic_filebuf<char, char_traits<char> >;
template class basic_ifstream<char, char_traits<char> >;
template class basic_ofstream<char, char_traits<char> >;
template class basic_fstream<char, char_traits<char> >;

#  if !defined (_STLP_NO_WCHAR_T)
template class _Underflow<wchar_t, char_traits<wchar_t> >;
template class basic_filebuf<wchar_t, char_traits<wchar_t> >;
template class basic_ifstream<wchar_t, char_traits<wchar_t> >;
template class basic_ofstream<wchar_t, char_traits<wchar_t> >;
template class basic_fstream<wchar_t, char_traits<wchar_t> >;
#  endif /* _STLP_NO_WCHAR_T */

#endif

_STLP_END_NAMESPACE

// Local Variables:
// mode:C++
// End:
