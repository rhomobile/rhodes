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

#include <stdlib.h>

#include <limits.h>
#include "c_locale.h"

#if defined (_STLP_REAL_LOCALE_IMPLEMENTED)
#  if defined (WIN32) || defined (_WIN32)
#    include "c_locale_win32/c_locale_win32.c"
#  elif defined (_STLP_USE_GLIBC) && ! defined (__CYGWIN__)
#    if (__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 2))
#      include "c_locale_glibc/c_locale_glibc2.c"
/* #      define _STLP_GLIBC_LOCALE_2 */ /* glibc 1.90 and newer */
/* #      include "c_locale_glibc/c_locale_glibc.c" */
#    else
#      include "c_locale_glibc/c_locale_glibc.c"
#    endif
#  elif defined __ISCPP__
#    include "c_locale_is/c_locale_is.cpp"
#  endif
#else /* !_STLP_REAL_LOCALE_IMPLEMENTED */
#  include "c_locale_dummy/c_locale_dummy.c"
#endif

#if defined(N_PLAT_NLM)
int wcslen( const wchar_t *_wc )
{ return unilen( (const unicode *)_wc ); }

int wcscmp( const wchar_t *_wc1, const wchar_t *_wc2 )
{ return unicmp( (const unicode *)_wc1, (const unicode *)_wc2 ); }

int wcsncmp( const wchar_t *_wc1, const wchar_t *_wc2, size_t n )
{ return unicmp( (const unicode *)_wc1, (const unicode *)_wc2, n ); }

wchar_t *wcsstr( const wchar_t *_wc1, const wchar_t *_wc2 )
{ return (wchar_t *)unistr( (const unicode *)_wc1, (const unicode *)_wc2 ); }

wchar_t *wcschr( const wchar_t *_wc1, wchar_t _wc2 )
{ return (wchar_t *)unichr((const unicode_t *)_wc1, (unicode_t)_wc2 ); }

wchar_t *wcsrchr( const wchar_t *_wc1, wchar_t _wc2 )
{ return (wchar_t *)unirchr((const unicode_t *)_wc1, (unicode_t)_wc2 ); }

wchar_t *wcscpy( wchar_t *_wc1, const wchar_t *_wc2 )
{ return (wchar_t *)unicpy((unicode_t *)_wc1, (const unicode_t *)_wc2 ); }

wchar_t *wcsncpy( wchar_t *_wc1, const wchar_t *_wc2, size_t n )
{ return (wchar_t *)unincpy((unicode_t *)_wc1, (const unicode_t *)_wc2, n ); }

wchar_t *wcspbrk( const wchar_t *_wc, const wchar_t *_wc2 )
{ return (wchar_t *)unipbrk( (const unicode *)_wc, (const unicode *)_wc2 ); }
#endif
