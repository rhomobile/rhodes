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


#ifndef __wcecompat__STDLIB_H__
#define __wcecompat__STDLIB_H__

#include "stddef.h"
#include "malloc.h"
#include "memory.h"
#include "float.h"

#ifdef __cplusplus
extern "C" {
#endif

extern float ceilf(float);  /* inside default stdlib.h */
extern float fabsf(float);
extern float floorf(float);
extern float sqrtf(float);
#ifdef _X86_
extern float fmodf(float);
#endif

extern int _fmode;          /* default file translation mode */

void abort(void);

char * __cdecl getenv(const char *name);
int    __cdecl _putenv(const char *);
int    __cdecl _wputenv(const wchar_t *);
extern char** _environ;    /* pointer to environment table */
extern wchar_t** _wenviron;    /* pointer to wide environment table */
#define putenv _putenv
#define environ _environ
#ifdef UNICODE
#define _tputenv    _wputenv
#define _tenviron   _wenviron
#else
#define _tputenv    _putenv
#define _tenviron   _environ
#endif

void * __cdecl bsearch(const void *key, const void *base, size_t nmemb, size_t size,
        int (__cdecl * compar)(const void *, const void *));

extern int __argc;          /* count of cmd line args */
extern char ** __argv;      /* pointer to table of cmd line args */
extern wchar_t ** __wargv;  /* pointer to table of wide cmd line args */

// min and max macros
#define __max(a,b)    (((a) > (b)) ? (a) : (b))
#define __min(a,b)    (((a) < (b)) ? (a) : (b))

// Conversion function prototypes
_CRTIMP double __cdecl strtod(const char *, char **);
_CRTIMP long   __cdecl strtol(const char *, char **, int);
_CRTIMP unsigned long __cdecl strtoul(const char *, char **, int);
_CRTIMP __int64 __cdecl _atoi64(const char *);
_CRTIMP int    __cdecl atoi(const char *);
_CRTIMP double __cdecl atof(const char *);
_CRTIMP long   __cdecl atol(const char *);
char * __cdecl _itoa(int, char *, int);
char * __cdecl _ltoa(long, char *, int);
char * __cdecl _ultoa(unsigned long, char *, int);
char * __cdecl _ecvt(double, int, int *, int *);
char * __cdecl _fcvt(double, int, int *, int *);
char * __cdecl _gcvt(double, int, char *);
_CRTIMP int    __cdecl tolower(int);
_CRTIMP int    __cdecl toupper(int);
_CRTIMP double __cdecl wcstod(const wchar_t *, wchar_t **);
_CRTIMP long   __cdecl wcstol(const wchar_t *, wchar_t **, int);
wchar_t * __cdecl _ultow (unsigned long, wchar_t *, int);
wchar_t * __cdecl _itow (int, wchar_t *, int);
wchar_t * __cdecl _ltow (long, wchar_t *, int);
_CRTIMP long      __cdecl _wtol(const wchar_t *);
_CRTIMP __int64   __cdecl _wtoll(const wchar_t *);
unsigned long __cdecl wcstoul(const wchar_t *, wchar_t **, int);
#define _wtoi   _wtol
#define _wtoi64 _wtoll

// Extended logical Ops functions
unsigned long __cdecl _lrotl(unsigned long, int);
unsigned long __cdecl _lrotr(unsigned long, int);
unsigned int  __cdecl _rotl(unsigned int, int);
unsigned int  __cdecl _rotr(unsigned int, int);

// Other misc STDLIB functions
_CRTIMP void   __cdecl _swab(char *, char *, int);
_CRTIMP int    __cdecl rand(void);
_CRTIMP void   __cdecl srand(unsigned int);
void   __cdecl qsort(void *, size_t, size_t, int (__cdecl *)(const void *, const void *));
double __cdecl difftime(time_t, time_t);
int __cdecl atexit(void (__cdecl *)(void));

// Maximum value that can be returned by the rand function.
#define RAND_MAX 0x7fff

// onexit() defns
#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1
typedef int (__cdecl * _onexit_t)(void);
#define onexit_t _onexit_t

// Data structure & function definitions for div and ldiv runtimes
typedef struct _div_t {
    int quot;
    int rem;
} div_t;

typedef struct _ldiv_t {
    long quot;
    long rem;
} ldiv_t;

div_t  __cdecl div(int, int);
ldiv_t __cdecl ldiv(long, long);

unsigned __int64 _strtoui64(const char *nptr, char **endptr, int base);

#ifdef __cplusplus
}
#endif

#endif /* __wcecompat__STDLIB_H__ */
