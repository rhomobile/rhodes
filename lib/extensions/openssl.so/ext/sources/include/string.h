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


#ifndef __wcecompat__STRING_H__
#define __wcecompat__STRING_H__

#include "stddef.h"
#include "memory.h"

#ifdef __cplusplus
extern "C" {
#endif

char *  __cdecl strerror(int errnum);

// Widechar string functions
_CRTIMP wchar_t * __cdecl wcscat(wchar_t *, const wchar_t *);
_CRTIMP wchar_t * __cdecl wcschr(const wchar_t *, wchar_t);
_CRTIMP int       __cdecl wcscmp(const wchar_t *, const wchar_t *);
_CRTIMP wchar_t * __cdecl wcscpy(wchar_t *, const wchar_t *);
_CRTIMP size_t    __cdecl wcscspn(const wchar_t *, const wchar_t *);
_CRTIMP size_t    __cdecl wcslen(const wchar_t *);
_CRTIMP size_t    __cdecl mbstowcs(wchar_t *wcstr, const char *mbstr, size_t count);
_CRTIMP size_t    __cdecl wcstombs(char *mbstr, const wchar_t *wcstr, size_t count);
_CRTIMP wchar_t * __cdecl wcsncat(wchar_t *, const wchar_t *, size_t);
_CRTIMP int       __cdecl wcsncmp(const wchar_t *, const wchar_t *, size_t);
_CRTIMP wchar_t * __cdecl wcsncpy(wchar_t *, const wchar_t *, size_t);
_CRTIMP wchar_t * __cdecl wcspbrk(const wchar_t *, const wchar_t *);
_CRTIMP wchar_t * __cdecl wcsrchr(const wchar_t *, wchar_t);
_CRTIMP size_t    __cdecl wcsspn(const wchar_t *, const wchar_t *);
_CRTIMP wchar_t * __cdecl wcsstr(const wchar_t *, const wchar_t *);
_CRTIMP wchar_t * __cdecl wcstok(wchar_t *, const wchar_t *);
_CRTIMP wchar_t * __cdecl _wcsdup(const wchar_t *);
_CRTIMP int       __cdecl _wcsicmp(const wchar_t *, const wchar_t *);
_CRTIMP int       __cdecl _wcsnicmp(const wchar_t *, const wchar_t *, size_t);
_CRTIMP wchar_t * __cdecl _wcsnset(wchar_t *, wchar_t, size_t);
_CRTIMP wchar_t * __cdecl _wcsrev(wchar_t *);
_CRTIMP wchar_t * __cdecl _wcsset(wchar_t *, wchar_t);
_CRTIMP wchar_t * __cdecl _wcslwr(wchar_t *);
_CRTIMP wchar_t * __cdecl _wcsupr(wchar_t *);

void * __cdecl memcpy(void *, const void *, size_t);

// Non STDC old names for above
#define wcsdup    _wcsdup
#define wcsicmp   _wcsicmp
#define wcsnicmp  _wcsnicmp
#define wcsnset   _wcsnset
#define wcsrev    _wcsrev
#define wcsset    _wcsset
#define wcslwr    _wcslwr
#define wcsupr    _wcsupr

// ANSI string functions
size_t __cdecl strlen(const char *);
int    __cdecl strcmp(const char *, const char *);
char * __cdecl strcat(char *, const char *);
char * __cdecl strcpy(char *, const char *);

_CRTIMP char * __cdecl strchr(const char *, int);
_CRTIMP size_t __cdecl strcspn(const char *, const char *);
_CRTIMP char * __cdecl strncat(char *, const char *, size_t);
_CRTIMP int    __cdecl strncmp(const char *, const char *, size_t);
_CRTIMP char * __cdecl strncpy(char *, const char *, size_t);
_CRTIMP char * __cdecl strstr(const char *, const char *);
_CRTIMP char * __cdecl strtok(char *, const char *);

_CRTIMP int     __cdecl _stricmp(const char *, const char *);
_CRTIMP int     __cdecl _strnicmp(const char *, const char *, size_t);
_CRTIMP char *  __cdecl strpbrk(const char *, const char *);
_CRTIMP char *  __cdecl strrchr(const char *, int);
_CRTIMP size_t  __cdecl strspn(const char *, const char *);
_CRTIMP char *  __cdecl _strdup(const char *);
_CRTIMP char *  __cdecl _strnset(char *, int, size_t);
_CRTIMP char *  __cdecl _strrev(char *);
char *  __cdecl _strset(char *, int);
_CRTIMP char *  __cdecl _strlwr(char *);
_CRTIMP char *  __cdecl _strupr(char *);
_CRTIMP char * __cdecl strdup(const char * _Src);
#ifndef strncasecmp
#define strncasecmp _strnicmp
#endif
#ifndef strcasecmp
#define strcasecmp _stricmp
#endif

#ifdef __cplusplus
}
#endif

#endif /* __wcecompat__STRING_H__ */
