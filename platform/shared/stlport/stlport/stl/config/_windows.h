/*
 * Copyright (c) 1997-1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999
 * Boris Fomitchev
 *
 * Copyright (c) 2003
 * Francois Dumont
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
#ifndef _STLP_INTERNAL_WINDOWS_H
#define _STLP_INTERNAL_WINDOWS_H

#if !defined (_STLP_PLATFORM)
#  define _STLP_PLATFORM "Windows"
#endif

/* _STLP_WIN32_VERSION is used to detect targetted Windows platforms as
 * old ones are not supporting some Win32 functions that STLport use.
 * Limited OSs are going up to and including Windows 98 so they can be
 * detected using WINVER or _WIN32_WINDOWS macros, we do not have to use
 * _WINNT_WINDOWS macro for the moment.
 */
#if !defined (_STLP_WIN32_VERSION)
#  if defined (WINVER)
#    define _STLP_WIN32_VERSION WINVER
#  elif defined (_WIN32_WINDOWS)
#    define _STLP_WIN32_VERSION _WIN32_WINDOWS
#  endif
#endif

#if !defined (_STLP_BIG_ENDIAN) && !defined (_STLP_LITTLE_ENDIAN)
#  if defined (_MIPSEB)
#    define _STLP_BIG_ENDIAN 1
#  endif
#  if defined (__i386) || defined (_M_IX86) || defined (_M_ARM) || \
      defined (__amd64__) || defined (_M_AMD64) || defined (__x86_64__) || \
      defined (__alpha__)
#    define _STLP_LITTLE_ENDIAN 1
#  endif
#  if defined (__ia64__)
    /* itanium allows both settings (for instance via gcc -mbig-endian) - hence a seperate check is required */
#    if defined (__BIG_ENDIAN__)
#      define _STLP_BIG_ENDIAN 1
#    else
#      define _STLP_LITTLE_ENDIAN 1
#    endif
#  endif
#endif /* _STLP_BIG_ENDIAN */

#if !defined (_STLP_WINDOWS_H_INCLUDED)
#  define _STLP_WINDOWS_H_INCLUDED
#  if !(defined ( _STLP_MSVC ) || defined (__BORLANDC__) || defined (__ICL) || defined (__WATCOMC__) || \
        defined (__MINGW32__) || defined (__DMC__))
#    if defined (_STLP_USE_MFC)
#      include <afx.h>
#    else
#      include <windows.h>
#    endif
#  else
/* This section serves as a replacement for windows.h header for Visual C++ */
#    if defined (__cplusplus)
extern "C" {
#    endif
#    if (defined (_M_AMD64) || defined (_M_IA64) || (!defined (_STLP_WCE) && defined (_M_MRX000)) || defined (_M_ALPHA) || \
        (defined (_M_PPC) && (_STLP_MSVC_LIB >= 1000))) && !defined (RC_INVOKED)
#      define InterlockedIncrement       _InterlockedIncrement
#      define InterlockedDecrement       _InterlockedDecrement
#      define InterlockedExchange        _InterlockedExchange
/* Here we use a different macro name than the InterlockedExchangePointer SDK function
 * to avoid macro definition conflict as the SDK might already define InterlockedExchangePointer
 * as a macro.
 */
#      define STLPInterlockedExchangePointer _InterlockedExchangePointer
#      define _STLP_STDCALL
#    else
#      if defined (_MAC)
#        define _STLP_STDCALL _cdecl
#      else
#        define _STLP_STDCALL __stdcall
#      endif
#    endif

#    if defined (_STLP_NEW_PLATFORM_SDK)
_STLP_IMPORT_DECLSPEC long _STLP_STDCALL InterlockedIncrement(long volatile *);
_STLP_IMPORT_DECLSPEC long _STLP_STDCALL InterlockedDecrement(long volatile *);
_STLP_IMPORT_DECLSPEC long _STLP_STDCALL InterlockedExchange(long volatile *, long);
#      if defined (STLPInterlockedExchangePointer)
_STLP_IMPORT_DECLSPEC void* _STLP_STDCALL STLPInterlockedExchangePointer(void* volatile *, void*);
#      endif
_STLP_IMPORT_DECLSPEC long _STLP_STDCALL InterlockedExchangeAdd(long volatile *, long);
#    elif defined (_STLP_WCE)

/* start of eMbedded Visual C++ specific section */
#      include <windef.h> /* needed for basic windows types */

       /** in SDKs generated with PB5, windef.h somehow includes headers which then
       define setjmp. */
#      if (_WIN32_WCE >= 0x500)
#        define _STLP_NATIVE_SETJMP_H_INCLUDED
#      endif

#      ifndef _WINBASE_ /* winbase.h already included? */
long WINAPI InterlockedIncrement(long*);
long WINAPI InterlockedDecrement(long*);
long WINAPI InterlockedExchange(long*, long);
#      endif

#      ifndef __WINDOWS__ /* windows.h already included? */

#        if defined (x86)
#          include <winbase.h> /* needed for inline versions of Interlocked* functions */
#        endif

#        ifndef _MFC_VER

#          define MessageBox MessageBoxW
int WINAPI MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);

#          define wvsprintf wvsprintfW
int WINAPI wvsprintfW(LPWSTR, LPCWSTR, va_list ArgList);

void WINAPI ExitThread(DWORD dwExitCode);

#          if !defined (COREDLL)
#            define _STLP_WCE_WINBASEAPI DECLSPEC_IMPORT
#          else
#            define _STLP_WCE_WINBASEAPI
#          endif

_STLP_WCE_WINBASEAPI int WINAPI
MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr,
                    int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar);

_STLP_WCE_WINBASEAPI UINT WINAPI GetACP();

_STLP_WCE_WINBASEAPI BOOL WINAPI TerminateProcess(HANDLE hProcess, DWORD uExitCode);

#          define OutputDebugString OutputDebugStringW
void WINAPI OutputDebugStringW(LPCWSTR);

_STLP_WCE_WINBASEAPI void WINAPI Sleep(DWORD);

#          undef _STLP_WCE_WINBASEAPI

#        endif /* !_MFC_VER */

#      endif /* !__WINDOWS__ */

/* end of eMbedded Visual C++ specific section */

#    else
/* boris : for the latest SDK, you may actually need the other version of the declaration (above)
 * even for earlier VC++ versions. There is no way to tell SDK versions apart, sorry ...
 */
_STLP_IMPORT_DECLSPEC long _STLP_STDCALL InterlockedIncrement(long*);
_STLP_IMPORT_DECLSPEC long _STLP_STDCALL InterlockedDecrement(long*);
_STLP_IMPORT_DECLSPEC long _STLP_STDCALL InterlockedExchange(long*, long);
#    endif

#    if !defined (STLPInterlockedExchangePointer)
/* This API function do not exist in the old platform SDK and is equivalent to
 * InterlockedExchange on 32 bits platform:
 */
#      if defined (__cplusplus)
/* We do not define this function if we are not in a C++ translation unit just
 * because of the inline portability issue it would introduce. We will have to
 * fix it the day we need this function for a C translation unit.
 */
inline
void* _STLP_CALL STLPInterlockedExchangePointer(void* volatile* __a, void* __b) {
#        if defined (_STLP_MSVC)
/* Here MSVC produces warning if 64 bits portability issue is activated.
 * MSVC do not see that _STLP_ATOMIC_EXCHANGE_PTR is a macro which content
 * is based on the platform, Win32 or Win64
 */
#          pragma warning (push)
#          pragma warning (disable : 4311) // pointer truncation from void* to long
#          pragma warning (disable : 4312) // conversion from long to void* of greater size
#        endif
#        if !defined (_STLP_NO_NEW_STYLE_CASTS)
  return reinterpret_cast<void*>(InterlockedExchange(reinterpret_cast<long*>(const_cast<void**>(__a)),
                                                     reinterpret_cast<long>(__b)));
#        else
  return (void*)InterlockedExchange((long*)__a, (long)__b);
#        endif
#        if defined (_STLP_MSVC)
#          pragma warning (pop)
#        endif
}
#      endif
#    endif

#    if !defined (_STLP_WCE)
_STLP_IMPORT_DECLSPEC void _STLP_STDCALL Sleep(unsigned long);
_STLP_IMPORT_DECLSPEC void _STLP_STDCALL OutputDebugStringA(const char* lpOutputString);
#    endif

#    if defined (InterlockedIncrement)
#      pragma intrinsic(_InterlockedIncrement)
#      pragma intrinsic(_InterlockedDecrement)
#      pragma intrinsic(_InterlockedExchange)
#      pragma intrinsic(_InterlockedExchangePointer)
#    endif
#    if defined (__cplusplus)
} /* extern "C" */
#    endif

#  endif /* STL_MSVC __BORLANDC__ __ICL __WATCOMC__ __MINGW32__ __DMC__*/

#endif /* _STLP_WINDOWS_H_INCLUDED */

#endif /* _STLP_INTERNAL_WINDOWS_H */
