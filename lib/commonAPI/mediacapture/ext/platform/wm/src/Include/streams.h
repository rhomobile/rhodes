//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of your Microsoft Windows CE
// Source Alliance Program license form.  If you did not accept the terms of
// such a license, you are not authorized to use this source code.
//
//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//
//--------------------------------------------------------------------------;

// Streams architecture overall header file, January 1995

#ifndef __STREAMS__
#define __STREAMS__

#ifdef	_MSC_VER
// disable some level-4 warnings, use #pragma warning(enable:###) to re-enable
#pragma warning(disable:4100) // warning C4100: unreferenced formal parameter
#pragma warning(disable:4201) // warning C4201: nonstandard extension used : nameless struct/union
#pragma warning(disable:4511) // warning C4511: copy constructor could not be generated
#pragma warning(disable:4512) // warning C4512: assignment operator could not be generated
#pragma warning(disable:4514) // warning C4514: "unreferenced inline function has been removed"

#if _MSC_VER>=1100
#define AM_NOVTABLE __declspec(novtable)
#else
#define AM_NOVTABLE
#endif
#endif	// MSC_VER

#include <windows.h>
#include <windowsx.h>
#include <olectl.h>
#include <ddraw.h>
#include <mmsystem.h>

#ifdef UNDER_CE
#include <winx.h>
#endif 

#ifndef NUMELMS
   #define NUMELMS(aa) (sizeof(aa)/sizeof((aa)[0]))
#endif

#include <strmif.h>     // Generated IDL header file for streams interfaces

#include <reftime.h>    // Helper class for REFERENCE_TIME management
#include <wxdebug.h>    // Debug support for logging and ASSERTs
#include <amvideo.h>    // ActiveMovie video interfaces and definitions
//include amaudio.h explicitly if you need it.  it requires the DX SDK.
//#include <amaudio.h>    // ActiveMovie audio interfaces and definitions
#include <wxutil.h>     // General helper classes for threads etc
#include <combase.h>    // Base COM classes to support IUnknown
#include <dllsetup.h>   // Filter registration support functions
#include <measure.h>    // Performance measurement
#include <comlite.h>    // Light weight com function prototypes

#include <cache.h>      // Simple cache container class
#include <wxlist.h>     // Non MFC generic list class
#include <msgthrd.h>	// CMsgThread
#include <mtype.h>      // Helper class for managing media types
#include <fourcc.h>     // conversions between FOURCCs and GUIDs
#include <control.h>    // generated from control.odl
#include <ctlutil.h>    // control interface utility classes
#include <evcode.h>     // event code definitions
#include <amfilter.h>   // Main streams architecture class hierachy
#include <transfrm.h>   // Generic transform filter
#include <transip.h>    // Generic transform-in-place filter
#include <uuids.h>      // declaration of type GUIDs and well-known clsids
#include <source.h>	    // Generic source filter
#include <outputq.h>    // Output pin queueing
#include <errors.h>     // HRESULT status and error definitions
#include <renbase.h>    // Base class for writing ActiveX renderers
#include <renbaseasync.h>    // Base class for writing ActiveX asynchronous renderers
#include <winutil.h>    // Helps with filters that manage windows
#include <winctrl.h>    // Implements the IVideoWindow interface
#include <videoctl.h>   // Specifically video related classes
#include <refclock.h>	// Base clock class
#include <sysclock.h>	// System clock
#include <pstream.h>    // IPersistStream helper class
#include <vtrans.h>     // Video Transform Filter base class
#ifndef UNDER_CE
#include <amextra.h>
#include <cprop.h>      // Base property page class
#endif // UNDER_CE
#include <strmctl.h>    // IAMStreamControl support
#include <edevdefs.h>   // External device control interface defines
#include <dvdevcod.h>   // DVD error code definitions

#else
    #ifdef DEBUG
    #pragma message("STREAMS.H included TWICE")
    #endif
#endif // __STREAMS__

