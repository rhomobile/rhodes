/*
 * Copyright (c) 1999
 * Silicon Graphics
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

#ifndef _STLP_RANGE_ERRORS_H
#define _STLP_RANGE_ERRORS_H

// A few places in the STL throw range errors, using standard exception
// classes defined in <stdexcept>.  This header file provides functions
// to throw those exception objects.

// _STLP_DONT_THROW_RANGE_ERRORS is a hook so that users can disable
// this exception throwing.
#if defined (_STLP_CAN_THROW_RANGE_ERRORS) && defined (_STLP_USE_EXCEPTIONS) && \
   !defined (_STLP_DONT_THROW_RANGE_ERRORS)
#  define _STLP_THROW_RANGE_ERRORS
#endif

// For the STLport iostreams, only declaration here, definition is in the lib
#if !defined (_STLP_USE_NO_IOSTREAMS) && \
    !defined (_STLP_USE_NO_EXTERN_RANGE_ERRORS) && \
    !defined (_STLP_EXTERN_RANGE_ERRORS)
#  define _STLP_EXTERN_RANGE_ERRORS
#endif

_STLP_BEGIN_NAMESPACE
void _STLP_FUNCTION_THROWS _STLP_DECLSPEC _STLP_CALL __stl_throw_runtime_error(const char* __msg);
void _STLP_FUNCTION_THROWS _STLP_DECLSPEC _STLP_CALL __stl_throw_range_error(const char* __msg);
void _STLP_FUNCTION_THROWS _STLP_DECLSPEC _STLP_CALL __stl_throw_out_of_range(const char* __msg);
void _STLP_FUNCTION_THROWS _STLP_DECLSPEC _STLP_CALL __stl_throw_length_error(const char* __msg);
void _STLP_FUNCTION_THROWS _STLP_DECLSPEC _STLP_CALL __stl_throw_invalid_argument(const char* __msg);
void _STLP_FUNCTION_THROWS _STLP_DECLSPEC _STLP_CALL __stl_throw_overflow_error(const char* __msg);
_STLP_END_NAMESPACE

#if !defined (_STLP_EXTERN_RANGE_ERRORS)

#  if defined(_STLP_THROW_RANGE_ERRORS)
#    ifndef _STLP_INTERNAL_STDEXCEPT
#      include <stl/_stdexcept.h>
#    endif
#    ifndef _STLP_STRING
#      include <string>
#    endif
#    define _STLP_THROW_MSG(ex,msg)  throw ex(string(msg))
#  else
#    if defined (_STLP_RTTI_BUG)
#      define _STLP_THROW_MSG(ex,msg)  TerminateProcess(GetCurrentProcess(), 0)
#    else
#      ifndef _STLP_INTERNAL_CSTDLIB
#        include <stl/_cstdlib.h>
#      endif
#      ifndef _STLP_INTERNAL_CSTDIO
#        include <stl/_cstdio.h>
#      endif
#      define _STLP_THROW_MSG(ex,msg)  puts(msg),_STLP_ABORT()
#    endif
#  endif

// For mode without library and throwing range errors, include the
// stdexcept header and throw the appropriate exceptions directly.

_STLP_BEGIN_NAMESPACE

inline void _STLP_DECLSPEC _STLP_CALL __stl_throw_runtime_error(const char* __msg)
{ _STLP_THROW_MSG(runtime_error, __msg); }

inline void _STLP_DECLSPEC _STLP_CALL __stl_throw_range_error(const char* __msg)
{ _STLP_THROW_MSG(range_error, __msg); }

inline void _STLP_DECLSPEC _STLP_CALL __stl_throw_out_of_range(const char* __msg)
{ _STLP_THROW_MSG(out_of_range, __msg); }

inline void _STLP_DECLSPEC _STLP_CALL __stl_throw_length_error(const char* __msg)
{ _STLP_THROW_MSG(length_error, __msg); }

inline void _STLP_DECLSPEC _STLP_CALL __stl_throw_invalid_argument(const char* __msg)
{ _STLP_THROW_MSG(invalid_argument, __msg); }

inline void _STLP_DECLSPEC _STLP_CALL __stl_throw_overflow_error(const char* __msg)
{ _STLP_THROW_MSG(overflow_error, __msg); }

_STLP_END_NAMESPACE

#  undef _STLP_THROW_MSG

#endif /* EXTERN_RANGE_ERRORS */

#endif /* _STLP_RANGE_ERRORS_H */

// Local Variables:
// mode:C++
// End:
