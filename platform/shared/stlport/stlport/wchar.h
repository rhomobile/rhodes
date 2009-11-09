/*
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

#if !defined (_STLP_OUTERMOST_HEADER_ID)
#  define _STLP_OUTERMOST_HEADER_ID 0x278
#  include <stl/_prolog.h>
#elif (_STLP_OUTERMOST_HEADER_ID == 0x278) && !defined (_STLP_DONT_POP_HEADER_ID)
#  define _STLP_DONT_POP_HEADER_ID
#endif

#if !defined (_STLP_WCE_EVC3) && !defined (_STLP_NO_WCHAR_T)

#  if defined (__BORLANDC__)
/*
#  include <cstring>
*/
#    include _STLP_NATIVE_CPP_C_HEADER(_str.h)
#    ifdef __cplusplus
using _STLP_VENDOR_CSTD::strlen;
using _STLP_VENDOR_CSTD::strspn;
#    endif
#  endif

#  if (((__GNUC__ < 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ < 3))) && defined (__APPLE__)) || defined (__OpenBSD__)
#    include _STLP_NATIVE_C_HEADER(stddef.h)
#  elif defined(__MWERKS__) && defined(N_PLAT_NLM)
/*
 * MSL library has wrong definition of wint_t (with -wchar_t on) in wchar_t.h header.
 * IMHO the best way is to comment line
 *       typedef wchar_t wint_t;
 * (I use this way).
 *
 * Another solution is to define _WINT_T (to avoid conflict with wint_t definitions in
 * Novell SDK headers, _WCHAR_T defined in nlm_prefix.h). But you should define one
 * before include any header from Novell's SDK, so this isn't too good choice.
 *
 * And third solution is don't use anyware (even here) wchar_t.h
 *
 *      - ptr, 2003 and 2005-05-07
 */
/*
#if __option(wchar_type)
# define _WINT_T
#endif
*/
#   include _STLP_NATIVE_CPP_C_HEADER(wchar_t.h)
#   include _STLP_NATIVE_C_HEADER(stddef.h)
#   include _STLP_NATIVE_C_HEADER(stdio.h)
#   include <unicode.h>
int wcslen( const wchar_t *_wc );
int wcscmp( const wchar_t *_wc1, const wchar_t *_wc2 );
int wcsncmp( const wchar_t *_wc1, const wchar_t *_wc2, size_t n );
wchar_t *wcsstr( const wchar_t *_wc1, const wchar_t *_wc2 );
wchar_t *wcschr( const wchar_t *_wc1, wchar_t _wc2 );
wchar_t *wcsrchr( const wchar_t *_wc1, wchar_t _wc2 );
wchar_t *wcscpy( wchar_t *_wc1, const wchar_t *_wc2 );
wchar_t *wcsncpy( wchar_t *_wc1, const wchar_t *_wc2, size_t n );
wchar_t *wcspbrk( const wchar_t *_wc, const wchar_t *_wc2 );
#  else
#    include _STLP_NATIVE_C_HEADER(wchar.h)
#  endif
#endif /* !defined (_STLP_WCE_EVC3) && !defined (_STLP_NO_WCHAR_T) */

#if defined(OS_ANDROID)
#  include _STLP_NATIVE_C_HEADER(wchar.h)
#  define _STLP_INTERNAL_MBSTATE_T 
#endif

#ifndef _STLP_INTERNAL_MBSTATE_T
#  include <stl/_mbstate_t.h>
#endif

#if (_STLP_OUTERMOST_HEADER_ID == 0x278)
#  if ! defined (_STLP_DONT_POP_HEADER_ID)
#    include <stl/_epilog.h>
#    undef  _STLP_OUTERMOST_HEADER_ID
#  else
#    undef  _STLP_DONT_POP_HEADER_ID
#  endif
#endif

/*
 Local Variables:
 mode:C++
 End:
*/
