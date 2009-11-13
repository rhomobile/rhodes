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
#  define _STLP_OUTERMOST_HEADER_ID 0x205
#  include <stl/_prolog.h>
#elif (_STLP_OUTERMOST_HEADER_ID == 0x205) && !defined (_STLP_DONT_POP_HEADER_ID)
#  define _STLP_DONT_POP_HEADER_ID
#endif

#ifdef _STLP_WCE
// only show message when directly including this file in a non-library build
#  if !defined(__BUILDING_STLPORT) && (_STLP_OUTERMOST_HEADER_ID == 0x205)
#    pragma message("eMbedded Visual C++ 3 and .NET don't have a errno.h header; STLport won't include native errno.h here")
#  endif
#else
#  ifndef errno
/* We define the following macro first to guaranty the header reentrancy: */
#    define _STLP_NATIVE_ERRNO_H_INCLUDED
#    include _STLP_NATIVE_C_HEADER(errno.h)
#  endif /* errno */

#  if !defined (_STLP_NATIVE_ERRNO_H_INCLUDED)
/* If errno has been defined before inclusion of native errno.h including it from STLport errno.h
 * becomes impossible because if:
 * #define errno foo
 * then
 * #include _STLP_NATIVE_C_HEADER(errno.h)
 * becomes:
 * #include _STLP_NATIVE_C_HEADER(foo.h)
 *
 * To fix this problem you have to find where this definition comes from and include errno.h before it.
 */
#    error errno has been defined before inclusion of errno.h header.
#  endif

#  ifdef __cplusplus
#    ifndef errno /* errno still not defined */
_STLP_BEGIN_NAMESPACE
#      if !defined (__BORLANDC__)
using ::errno;
#      else
using _STLP_VENDOR_CSTD::errno;
#      endif
_STLP_END_NAMESPACE
#   endif /* errno */
#  endif /* __cplusplus */

#endif

#if (_STLP_OUTERMOST_HEADER_ID == 0x205)
#  if ! defined (_STLP_DONT_POP_HEADER_ID)
#    include <stl/_epilog.h>
#    undef  _STLP_OUTERMOST_HEADER_ID
#  endif
#  undef  _STLP_DONT_POP_HEADER_ID
#endif

/* Local Variables:
 * mode:C++
 * End:
 */
