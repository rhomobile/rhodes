#pragma once

#if defined(OS_PLATFORM_MOTCE)

#include <exdisp.h>
#include <exdispid.h>
#include <mshtmhst.h>
#include <mshtml.h>

#if defined (_WIN32_WCE) && !defined( OS_PLATFORM_MOTCE )
#include <pvdispid.h>
#include <piedocvw.h>
#endif

#define MAXURL 2049			///< The maximum allowable length of a URL in Internet Explorer		
#ifndef MAX_URL
#define MAX_URL MAXURL	///< Maximimum allowable length of a URL in Internet Explorer
#endif

extern UINT WM_BROWSER_ONDOCUMENTCOMPLETE;
extern UINT WM_BROWSER_ONNAVIGATECOMPLETE;
extern UINT WM_BROWSER_ONTITLECHANGE;
extern UINT WM_BROWSER_ONBEFORENAVIGATE;
extern UINT WM_BROWSER_ONNAVIGATIONTIMEOUT;
extern UINT WM_BROWSER_ONNAVIGATIONERROR;
extern UINT WM_BROWSER_ONSETSIPSTATE;
extern UINT WM_BROWSER_ONALERTPOPUP;
extern UINT WM_BROWSER_ONAUTHENTICATIONREQUEST;

#endif