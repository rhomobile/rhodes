/*  wcecompat: Windows CE C Runtime Library "compatibility" library.
 *
 *  Copyright (C) 2001-2002 Essemer Pty Ltd.  All rights reserved.
 *  http://www.essemer.com.au/
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef __wcecompat__STDIO_H__
#define __wcecompat__STDIO_H__

#include "stddef.h"
//#include "sys/types.h"	/* size_t */
#include "stdarg.h"

#ifdef __cplusplus
extern "C" {
#endif

// STDIO constant defns
#define _MAX_PATH    MAX_PATH

// EOF constants
#define EOF  (-1)
#define WEOF (wint_t)(0xFFFF)

// Seek method constants
#define SEEK_CUR    1
#define SEEK_END    2
#define SEEK_SET    0

// mode constants for _setmode()
#define _O_TEXT     0x4000  /* file mode is text (translated) */
#define _O_BINARY   0x8000  /* file mode is binary (untranslated) */

// FILE is an opaque handle in Win CE. Users have no access to the internals
#ifndef _FILE_DEFINED
typedef unsigned char FILE;
#define _FILE_DEFINED
#endif

#if _INTEGRAL_MAX_BITS >= 64
typedef __int64 fpos_t;
#else
typedef long fpos_t;
#endif

// Std handle defns
#define stdin  _getstdfilex(0)
#define stdout _getstdfilex(1)
#define stderr _getstdfilex(2)

// ANSI String formatting functions
_CRTIMP int    __cdecl sscanf(const char *, const char *, ...);
_CRTIMP int    __cdecl sprintf(char *, const char *, ...);
_CRTIMP int    __cdecl vsprintf(char *, const char *, va_list);
_CRTIMP int    __cdecl _snprintf(char *, size_t, const char *, ...);
_CRTIMP int    __cdecl _vsnprintf(char *, size_t, const char *, va_list);

// Always present since wsprintfW (used by kernel) redirects to these
_CRTIMP int __cdecl swprintf(wchar_t *, const wchar_t *, ...);
_CRTIMP int __cdecl vswprintf(wchar_t *, const wchar_t *, va_list);
_CRTIMP int __cdecl _snwprintf(wchar_t *, size_t, const wchar_t *, ...);
_CRTIMP int __cdecl _vsnwprintf(wchar_t *, size_t, const wchar_t *, va_list);

// WIDE String formatting functions
_CRTIMP int __cdecl swscanf(const wchar_t *, const wchar_t *, ...);

// ANSI Stdin/Out functions & macros
_CRTIMP int    __cdecl scanf(const char *, ...);
_CRTIMP int    __cdecl printf(const char *, ...);
_CRTIMP int    __cdecl vprintf(const char *, va_list);
_CRTIMP int    __cdecl getchar(void);
_CRTIMP char * __cdecl gets(char *);
_CRTIMP int    __cdecl putchar(int);
_CRTIMP int    __cdecl puts(const char *);

// ANSI functions for Stdin/out and/or general buffered file handling
_CRTIMP int    __cdecl fgetc(FILE *);
_CRTIMP char * __cdecl fgets(char *, int, FILE *);
_CRTIMP int    __cdecl fputc(int, FILE *);
_CRTIMP int    __cdecl fputs(const char *, FILE *);
_CRTIMP int    __cdecl ungetc(int, FILE *);

// ANSI functions for general buffered file handling
_CRTIMP FILE * __cdecl fopen(const char *, const char *);
_CRTIMP FILE*  __cdecl fdopen(int, const char *);
_CRTIMP int    __cdecl fscanf(FILE *, const char *, ...);
_CRTIMP int    __cdecl fprintf(FILE *, const char *, ...);
_CRTIMP int    __cdecl vfprintf(FILE *, const char *, va_list);

#define getc(_stream)     fgetc(_stream)
#define putc(_i, _stream) fputc(_i, _stream)

// functions for general buffered file handling in either ANSI or Wide
_CRTIMP FILE*  __cdecl _getstdfilex(int);
_CRTIMP int    __cdecl fclose(FILE *);
_CRTIMP int    __cdecl _fcloseall(void);
_CRTIMP size_t __cdecl fread(void *, size_t, size_t, FILE *);
_CRTIMP size_t __cdecl fwrite(const void *, size_t, size_t, FILE *);
_CRTIMP int    __cdecl fflush(FILE *);
_CRTIMP int    __cdecl _flushall(void);
_CRTIMP int    __cdecl feof(FILE *);
_CRTIMP int    __cdecl ferror(FILE *);
_CRTIMP void   __cdecl clearerr(FILE *);
_CRTIMP int    __cdecl fgetpos(FILE *, fpos_t *);
_CRTIMP int    __cdecl fsetpos(FILE *, const fpos_t *);
_CRTIMP int    __cdecl fseek(FILE *, long, int);
_CRTIMP long   __cdecl ftell(FILE *);
_CRTIMP int    __cdecl _fileno(FILE *);
_CRTIMP int    __cdecl _setmode(int fd, int mode);
_CRTIMP FILE*  __cdecl _wfdopen(void*, const wchar_t*);
_CRTIMP FILE*  __cdecl _wfreopen(const wchar_t *path, const wchar_t *mode, FILE *stream);
// old names
#define fcloseall _fcloseall
#define fileno    _fileno
#define flushall  _flushall

// WIDE Stdin/Out functions & macros
_CRTIMP int __cdecl wscanf(const wchar_t *, ...);
_CRTIMP int __cdecl wprintf(const wchar_t *, ...);
_CRTIMP int __cdecl vwprintf(const wchar_t *, va_list);
_CRTIMP wint_t __cdecl getwchar(void);
_CRTIMP wint_t __cdecl putwchar(wint_t);
_CRTIMP wchar_t * __cdecl _getws(wchar_t *);
_CRTIMP int __cdecl _putws(const wchar_t *);

// WIDE functions for Stdin/out and/or general buffered file handling
_CRTIMP wint_t __cdecl fgetwc(FILE *);
_CRTIMP wint_t __cdecl fputwc(wint_t, FILE *);
_CRTIMP wint_t __cdecl ungetwc(wint_t, FILE *);
_CRTIMP wchar_t * __cdecl fgetws(wchar_t *, int, FILE *);
_CRTIMP int __cdecl fputws(const wchar_t *, FILE *);

#define getwc(_stm)             fgetwc(_stm)
#define putwc(_c,_stm)          fputwc(_c,_stm)

// WIDE functions for general buffered file handling
_CRTIMP FILE * __cdecl _wfopen(const wchar_t *, const wchar_t *);
_CRTIMP int __cdecl fwscanf(FILE *, const wchar_t *, ...);
_CRTIMP int __cdecl fwprintf(FILE *, const wchar_t *, ...);
_CRTIMP int __cdecl vfwprintf(FILE *, const wchar_t *, va_list);

#define BUFSIZ (512)

void perror(const char *prefix);

#ifndef _IOFBF
#define _IOFBF	0
#endif
#ifndef _IOLBF
#define _IOLBF	1
#endif
#ifndef _IONBF
#define _IONBF	2
#endif

#ifndef FILENAME_MAX
#define FILENAME_MAX 260
#endif

int setbuf (FILE* stream, char* buffer);
int setvbuf(FILE *stream, char *buffer, int mode, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __wcecompat__STDIO_H__ */
