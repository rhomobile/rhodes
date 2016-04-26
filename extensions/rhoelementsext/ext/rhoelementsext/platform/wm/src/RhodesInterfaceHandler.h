//  RhoElements 1.x
//#include "../../../../../../../../../Helium/HeliumForWindows/Implementation/PBCore/PBCore/stdafx.h"
#include "../../../../../../../neon/Helium/HeliumForWindows/Implementation/PBCore/PBCore/PBCore.h"
#include <common/RhodesApp.h>
using namespace rho::common;

//  Callbacks to Rhodes and WebKit, these are provided as function pointers to PBCore during initialisation,
//  this is the glue code to connect Rhodes with the RhoElementsOne extension.
BOOL NavigateCallback(const wchar_t* szUrl);
BOOL ExistsJavascriptCallback(const wchar_t* szJavascript);
BOOL InvokeJavascriptCallback(const wchar_t* szJavascript);
void LogCallback(int nSeverity, const wchar_t* szModule, const wchar_t* szMsg, const wchar_t* szFunctionName, int nLine);
void GPSControlCallback(bool);	//  TBD
void StopCallback();
void HistoryForwardCallback();
void HistoryBackCallback();
void RefreshCallback(BOOL bReloadFromCache);
void QuitCallback();
void MinimizeCallback();
void RestoreCallback();
void ResizeBrowserWindowCallback(RECT rcSize);
void ZoomPageCallback(float fZoom);
void ZoomTextCallback(int iZoom);
int GetTextZoomCallback();
void GetTitleCallback(WCHAR* szTitle, int iMaxLength);
void SetBrowserGesturingCallback(BOOL bSetBrowserGesturing);
void PassSipPositionToEngineCallback();
BOOL RegisterForMessageCallback(unsigned int iMsgId);
BOOL DeRegisterForMessageCallback(unsigned int iMsgId);
BOOL RegisterForPrimaryMessageCallback(unsigned int iMsgId);
BOOL DeRegisterForPrimaryMessageCallback(unsigned int iMsgId);


//  Direct calls coming from Rhodes, mimic the same inputs as were present in
//  RhoElementsOne executable
BOOL InitialiseRhoElementsExtension(HINSTANCE hInstance, HWND hWnd, HWND hWndWebKit, const wchar_t* szConfigFile, 
									   CExtManager* pExtManager);
void ShutDown();
BOOL InjectMetaTag(LPWSTR szHttpEquiv, LPWSTR szContents);
BOOL BeforeNavigateHandler(const wchar_t* szURL);
BOOL NavigateCompleteHandler(const wchar_t* szURL);
BOOL DocumentCompleteHandler(const wchar_t* szURL);
LRESULT NavigationTimeoutHandler(const wchar_t* szURL);
LRESULT NavigationErrorHandler(const wchar_t* szURL);
LRESULT LicenseErrorHandler(const wchar_t* szURL);
void WindowChangedHandler(LPVOID lparam);
bool WindowsMessage(MSG& msg);
bool HTMLMessage(MSG& msg);
LRESULT SipState(BOOL bSIPState);
LRESULT GeolocationData(int eeID, void* value);
//LRESULT JavascriptPopup(int iPopupType, void* pPopupData);
LRESULT AuthenticationRequest(int iEngineEvent, void* pAuthenticationData);
void AppActivateHandler(bool bActivate);


