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

typedef struct engineMETATagType
{
    LPWSTR tcHTTPEquiv;	///< Contents of the Meta tag's HTTP-Equiv String
    LPWSTR tcContents;	///< Contents of the Meta tag's Contents string
} EngineMETATag;

enum EngineEventID
{
	EEID_BEFORENAVIGATE = 0,///< Fired Before a new page is navigated to, if the calling application instructs a CEngine::Navigate or the user clicks on a link.
	EEID_NAVIGATECOMPLETE,	///< Fired once the server responds to the client's navigation request, the page now starts loading.
	EEID_DOCUMENTCOMPLETE,  ///< Fired when the page is 100% loaded included all embedded images.
	EEID_NAVIGATIONERROR,	///< Fired if there is an error navigating to the page, for example attempting to navigate to http://www.motorola.com if the device does not have a network connection.
	EEID_METATAG,			///< Fired when a Meta Tag is parsed on the loaded page.  This event is fired once for each meta tag, so a page with 3 meta tags will invoke this event 3 times.
	/**
	* Fires when the title of a document in the object becomes available or changes.  
	* Because the title might change while an HTML page is downloading, the URL of 
	* the document is set as the title. If the HTML page specifies a title, it is parsed, 
	* and the title is changed to reflect the actual title.
	*/
	EEID_TITLECHANGE,		
	EEID_NAVIGATIONTIMEOUT, ///< Fired when the page load takes longer than the value specified by CEngine::SetNavigationTimeout
	EEID_STATUSTEXTCHANGE,	///< Fired when the status bar text on the currently loaded page changes.
	EEID_PROGRESSCHANGE,	///< Fired when the progress of a download operation is updated.  LOWORD(value) contains the maximum progress value.  HIWORD(value) contains the current progress value, or -1 if the download is complete.
	EEID_TOPMOSTHWNDAVAILABLE,	///<  Deprecated - use EEID_ENGINE_INITIALISED instead to get the engine window handle
	EEID_SETSIPSTATE,			///<  Fired when an editable field gains or loses focus (deprecated)
	EEID_CONSOLEMESSAGE,		///<  Fired when a console message is available from WebKit (could be a JS error or window.console.log)
	EEID_JSPOPUP_ALERT,			///<  Fired when the rendering engine encounters a JS Alert
	EEID_JSPOPUP_CONFIRM,		///<  Fired when the rendering engine encounters a JS Confirm
	EEID_JSPOPUP_PROMPT,		///<  Fired when the rendering engine encounters a JS Prompt
	EEID_JSPOPUP_AUTHENTICATION, ///<  Fired when the engine requires authentication
	EEID_GPS_POSITIONRECEIVED,	///<  Position message from GPS Driver
	EEID_GPS_LOCATIONERROR,		///<  Location Error message from the GPS Driver
	EEID_SETSIPINPUTTYPE,		///<  Fired when an editable field gains or loses focus (replaces EEID_SETSIPSTATE)
	EEID_APPLICATIONUNLICENSED,	///<  Fired when the Engine can not navigate to a specified URL because the application is unlicensed.
    EEID_IS_REQUEST_HANDLED,    ///< If this request is going to be handled by the platform return true.
	EEID_LOG,
	EEID_ENGINE_INITIALISED,	///<  Fired when the engine has completed initialisation and the TopMost Window handle is available.   Value contains the HWND cast to an integer.  TabID is not used.
	EEID_DIALOG,				///<  Fired when the engine pops up or closes a JS Dialog
	EEID_ENGINE_EXIT,			///<  Fired when the engine has shutdown.  Handler must return TRUE to override default behaviour (which is to call PostQuitMessage).
	//this event ID MUST remain at the END of the list
	EEID_MAXEVENTID			///< Maximum event Identifier.  This does not represent an event but can be used in sizing arrays etc. 
};

extern UINT WM_BROWSER_ONDOCUMENTCOMPLETE;
extern UINT WM_BROWSER_ONNAVIGATECOMPLETE;
extern UINT WM_BROWSER_ONTITLECHANGE;
extern UINT WM_BROWSER_ONBEFORENAVIGATE;
extern UINT WM_BROWSER_ONNAVIGATIONTIMEOUT;
extern UINT WM_BROWSER_ONNAVIGATIONERROR;
extern UINT WM_BROWSER_ONSETSIPSTATE;
extern UINT WM_BROWSER_ONALERTPOPUP;
extern UINT WM_BROWSER_ONAUTHENTICATIONREQUEST;

namespace rho
{
namespace browser
{

BOOL IsRelativeURL(LPCTSTR tcURL);
//
int GetProtocolFromURL(LPCTSTR tcURL, LPTSTR lpRetStr);
//
BOOL DereferenceURL(LPCTSTR tcRelativeURLConst, TCHAR* tcDereferencedURL, TCHAR* tcCurrentURL);
//
HRESULT InitHTMLControl(HINSTANCE hInstance);

} //end of browser
} //end of rho

#endif