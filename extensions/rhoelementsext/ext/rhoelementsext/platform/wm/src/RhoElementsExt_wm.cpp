
#include <common/RhodesApp.h>
#include <logging/RhoLogConf.h>

#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>

#include <RhoNativeViewManager.h>
#include "rubyext/WebView.h"

#include "ruby/ext/rho/rhoruby.h"
#include "common/RhoFilePath.h"

#include "rhoelementsext_wm.h"

#include "RhodesInterfaceHandler.h"
using namespace rho::common;

extern "C" HWND getMainWnd();
extern "C" HINSTANCE rho_wmimpl_get_appinstance();
extern "C" HWND rho_wmimpl_get_mainwnd();

extern "C" void initialiseRhoElementsExt();

class CEmmlApiExtension: public IRhoExtension
{
public:
    CEmmlApiExtension(){}
	~CEmmlApiExtension()
	{
		DEBUGMSG(TRUE, (L"Shutting down RhoElementsExt Extension"));
		ShutDown();
	}
    virtual void onSetPropertiesData( const wchar_t* pPropID, const wchar_t* pData, const CRhoExtData& oExtData )
    {
		//  From EV: Call RHODESAPP().getExtManager().onUnhandledProperty( pModuleName, pPropName, pPropValue, oExtData );
		//           if tag is unrecognised.  We can not do this as it is not supported by the RE1 architecture.
		//  pData is not a const wchar_t*, it is actually a pointer to EngineMETATag*, I have
		//  asked Rhodes to modify the interface.
		EngineMETATag* metaTag = (EngineMETATag*)pData;
//		LOG(INFO) + "Received Meta Tag, Module: " + metaTag->tcHTTPEquiv + " Contents: " + metaTag->tcContents;
		if (wcscmp(metaTag->tcHTTPEquiv, L"initialiseRhoElementsExtension") == 0)
		{
			//initialiseRhoElementsExt();
		}
		else
			InjectMetaTag(metaTag->tcHTTPEquiv, metaTag->tcContents);
    }
    virtual void onSetProperty( const wchar_t* pName, const wchar_t* pValue, const CRhoExtData& oExtData )
    {/*do nothing, since you implement onSetPropertiesData*/}


    virtual void onBeforeNavigate(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData)
	{
		BeforeNavigateHandler(szUrlBeingNavigatedTo);
	}

   virtual void onNavigateComplete(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData)
	{
		NavigateCompleteHandler(szUrlBeingNavigatedTo);
	}

    virtual void onDocumentComplete(const wchar_t* szUrlOfDocument, const CRhoExtData& oExtData)
	{
		DocumentCompleteHandler(szUrlOfDocument);
	}

    virtual bool onWndMsg(MSG& oMsg)
	{	
		if (/*oMsg.message == WM_MOUSEMOVE || */oMsg.message == 0x119)  //  (0x119 is WM_GESTURE)
			return false;
		else
			return WindowsMessage(oMsg);
	}

	virtual bool onHTMLWndMsg(MSG& oMsg)
	{
		return HTMLMessage(oMsg);
	}

    virtual long OnNavigateTimeout(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData)
	{
		return NavigationTimeoutHandler(szUrlBeingNavigatedTo);
	}

    virtual long OnSIPState(bool bSIPState, const CRhoExtData& oExtData)
	{
		return SipState(bSIPState);
	}

   /* virtual long OnAlertPopup(int nEnum, void* pData, const CRhoExtData& oExtData)
	{
		return JavascriptPopup(nEnum, pData);
	}*/

	virtual long OnAuthenticationRequest(int nEnum, void* pData, const CRhoExtData& oExtData)
	{
		return AuthenticationRequest(nEnum, pData);
	}

    virtual long OnNavigateError(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData)
	{
		return NavigationErrorHandler(szUrlBeingNavigatedTo);
	}

    virtual long OnLicenseError(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData)
	{
		return LicenseErrorHandler(szUrlBeingNavigatedTo);
	}

    virtual void OnAppActivate(bool bActivate, const CRhoExtData& oExtData)
	{
		AppActivateHandler(bActivate);
	}

	virtual long OnGeolocationData(int nEnum, void* pData, const CRhoExtData& oExtData)
	{
		return GeolocationData(nEnum, pData);
	}

	virtual void OnWindowChanged(LPVOID lparam)
	{
		WindowChangedHandler(lparam);
	}


};

extern "C" void registerRhoExtension()
{
//	LOG(INFO) + "Registering Rho Extensions";
    RHODESAPP().getExtManager().registerExtension("emmlapi", new CEmmlApiExtension() );

	//  Load all Ruby objects by default
	RHODESAPP().getExtManager().requireRubyFile("symbolapi/elements");

//	LOG(INFO) + "Rho Extensions Registered, Initialising Interface";
	//  Ensure RhoElements 1.x is initialised in the correct thread, post this
	//  substitute meta tag to initilise as part of the meta tag handling
/*	EngineMETATag metaTag;
	metaTag.tcHTTPEquiv = L"initialiseRhoElementsExtension";
	metaTag.tcContents = L"initialise";
	SendMessage(rho_wmimpl_get_mainwnd(),PB_ONMETA,(WPARAM)0,(LPARAM)&metaTag);
*/
}


extern "C" void initialiseRhoElementsExt()
{
	//  Initialise RhoElements 1.x
	//  Rhodes was previously returning the wrong HWND and HINSTANCE in 
	//  this call but it now seems to be resolved
	CRhoExtData rhodesData = RHODESAPP().getExtManager().makeExtData();
	//  Something to do with the definition of OS_WINDOWS
//	HWND hWndRhodes = getMainWnd();
//    HINSTANCE hInstRhodes = rho_wmimpl_get_appinstance();

	//  Pass in an empty configuration file and use the default 
	//  (InstallDir)/Config/config.xml
	const wchar_t* szConfigPath = RHODESAPP().getExtManager().getConfigPath().c_str();
	InitialiseRhoElementsExtension((HINSTANCE)rhodesData.m_hInstance, rhodesData.m_hWnd, rhodesData.m_hBrowserWnd,
		szConfigPath,
		&RHODESAPP().getExtManager());

}


extern "C" VALUE emmlapi_meta_proc(const char* module_name, const char* contents, const char* ignored)
{
	//  Convert char* to wchar*
	int iHttpEquivLength = strlen(module_name) + 1;
	WCHAR* szHttpEquiv = new WCHAR[iHttpEquivLength];
	mbstowcs(szHttpEquiv, module_name, iHttpEquivLength);
	int iContentsLength = strlen(contents) + 1;
	WCHAR* szContents = new WCHAR[iContentsLength];
	mbstowcs(szContents, contents, iContentsLength);
	BOOL bRetVal = InjectMetaTag(szHttpEquiv, szContents);
	delete[] szHttpEquiv;
	delete[] szContents;
	return bRetVal;
}







