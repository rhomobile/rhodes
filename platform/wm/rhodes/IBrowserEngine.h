#pragma once

class CMainWindow;
namespace rho
{

struct IBrowserEngine
{
public:
    virtual ~IBrowserEngine(void){};

    virtual BOOL Navigate(LPCTSTR szURL, int iTabID) = 0;
    virtual HWND GetHTMLWND(int iTabID) = 0;
    virtual BOOL ResizeOnTab(int iInstID,RECT rcNewSize) = 0;
    virtual BOOL BackOnTab(int iInstID,int iPagesBack = 1) = 0;
    virtual BOOL ForwardOnTab(int iInstID) = 0;
    virtual BOOL ReloadOnTab(bool bFromCache, UINT iTab) = 0;
    virtual BOOL StopOnTab(UINT iTab) = 0;
    virtual BOOL NavigateToHtml(LPCTSTR szHtml) = 0;
    virtual LRESULT OnWebKitMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) = 0;
    virtual void RunMessageLoop(CMainWindow& mainWnd) = 0;
    virtual void SetCookie(char* url, char* cookie) = 0;
    virtual bool isExistJavascript(const wchar_t* szJSFunction, int index) = 0;
    virtual void executeJavascript(const wchar_t* szJSFunction, int index) = 0;
    virtual BOOL ZoomPageOnTab(float fZoom, UINT iTab) = 0;
    virtual BOOL ZoomTextOnTab(int nZoom, UINT iTab) = 0;
    virtual int GetTextZoomOnTab(UINT iTab) = 0;
    virtual BOOL GetTitleOnTab(LPTSTR szURL, UINT iMaxLen, UINT iTab) = 0;
    virtual void OnDocumentComplete(LPCTSTR url) = 0;
    virtual void setBrowserGesturing(bool bEnableGesturing) = 0;
    virtual void NotifyEngineOfSipPosition() = 0;
    virtual void setNavigationTimeout(unsigned int dwMilliseconds) = 0;
	virtual bool RegisterForMessage(unsigned int iMsgId) = 0;
	virtual bool DeRegisterForMessage(unsigned int iMsgId) = 0;
	virtual bool RegisterForPrimaryMessage(unsigned int iMsgId) = 0;
	virtual bool DeRegisterForPrimaryMessage(unsigned int iMsgId) = 0;

    virtual int NewTab() = 0;//returns	the new tab ID 
	virtual int SwitchTab(int iTabID) = 0;//returns the previous tab ID
	virtual BOOL CloseTab(int iTabID) = 0;//returns TRUE if successful
};

}