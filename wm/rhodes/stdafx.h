// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifdef _X86_
    #pragma comment(linker, "/nodefaultlib:libc.lib")
    #pragma comment(linker, "/nodefaultlib:libcd.lib")
    #pragma comment(linker, "/nodefaultlib:oldnames.lib")
#endif

#ifndef STRICT
#define STRICT
#endif

#include <ceconfig.h>
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#define SHELL_AYGSHELL
#endif

// NOTE - this is value is not strongly correlated to the Windows CE OS version being targeted
#define WINVER _WIN32_WCE

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

// CE COM has no single threaded apartment (everything runs in the MTA).
// Hence we declare we're not concerned about thread safety issues:
#define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atlhost.h>
//#include <atlsiface.h>

#include <pvdispid.h>
#include <piedocvw.h>

#include "Macros.h"

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#ifndef _DEVICE_RESOLUTION_AWARE
#define _DEVICE_RESOLUTION_AWARE
#endif
#endif

#ifdef _DEVICE_RESOLUTION_AWARE
#include "DeviceResolutionAware.h"
#endif

using namespace ATL;

#ifdef SHELL_AYGSHELL
#include <aygshell.h>
#pragma comment(lib, "aygshell.lib") 
#include <tpcshell.h> // Required for SHSendBackToFocusWindow
#endif // SHELL_AYGSHELL

// TODO: temporary code, until the CE compilers correctly implement the /MT[d], /MD[d] switches, and MFCCE fixes some #pragma issues
#ifdef _DLL // /MD
    #if defined(_DEBUG)
        #pragma comment(lib, "msvcrtd.lib")
    #else
        #pragma comment(lib, "msvcrt.lib")
    #endif
#else // /MT
    #if defined(_DEBUG)
        #pragma comment(lib, "libcmtd.lib")
    #else
        #pragma comment(lib, "libcmt.lib")
    #endif
#endif

#if _WIN32_WCE < 0x500 && ( defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP) )
    #pragma comment(lib, "ccrtrtti.lib")
    #ifdef _X86_
        #if defined(_DEBUG)
            #pragma comment(lib, "libcmtx86d.lib")
        #else
            #pragma comment(lib, "libcmtx86.lib")
        #endif
    #endif
#endif
