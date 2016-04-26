#pragma once

#include "browser/IBrowserEngine.h"
#include "RhoWKBrowserConfig.h"

class CEng;
extern "C" CEng* rho_wmimpl_get_webkitbrowser(HWND hParentWnd, HINSTANCE hInstance);

template<class T>
int inline findAndReplace(T& source, const T& find, const T& replace)
{
	int num=0;
	typename size_t fLen = find.size();
	typename size_t rLen = replace.size();
	for (size_t pos=0; (pos=source.find(find, pos))!=T::npos; pos+=rLen)
	{
		num++;
		source.replace(pos, fLen, replace);
	}
	return num;
}

namespace rho
{

class CRhoWKBrowserEngine :  public rho::IBrowserEngine
{
    DEFINE_LOGCLASS;

private:
    static CRhoWKBrowserEngine* m_pInstance;

    static WNDPROC m_WebKitOwnerProc;
    CRhoWKBrowserConfig* m_WKConfig;
    CEng* m_pEngine;
    HWND  m_hParentWnd;

    static LRESULT CALLBACK WK_HTMLWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK WK_WMMsgHandler(int iTabID,LPMSG pMsg);
    static LRESULT CALLBACK WK_GetEngineConfig(int iInstID, LPCTSTR tcSetting, TCHAR* tcValue);

    LRESULT ProcessOnTopMostWnd(HWND hWnd, int tabIndex);
    //void LoadEMMLManager();
    //BOOL LoadRegExFile();

    CRhoWKBrowserEngine(HWND hParentWnd, HINSTANCE hInstance);
    virtual ~CRhoWKBrowserEngine(void);

public:
    bool  m_bLoadingComplete, m_bLicenseChecked;

    static CRhoWKBrowserEngine* getInstance();
    static CRhoWKBrowserEngine* getInstance(HWND hParentWnd, HINSTANCE hInstance);
	
	//added for tab instance support PH
    HWND GetHTMLWND(int iTabID);
	
	
	int NewTab();//returns	the new tab ID 
	int SwitchTab(int iTabID);//returns the previous tab ID
	BOOL CloseTab(int iTabID);//returns TRUE if successful


    virtual CEng* getWebKitBrowser();
    virtual BOOL Navigate(LPCTSTR szURL, int iTabID);
    //virtual HWND GetHTMLWND();
    virtual BOOL ResizeOnTab(int iInstID,RECT rcNewSize);
    virtual BOOL BackOnTab(int iInstID,int iPagesBack = 1);
    virtual BOOL ForwardOnTab(int iInstID);
    virtual BOOL ReloadOnTab(bool bFromCache, UINT iTab);
    virtual BOOL StopOnTab(UINT iTab);
    virtual BOOL NavigateToHtml(LPCTSTR szHtml);
    virtual LRESULT OnWebKitMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual void RunMessageLoop(CMainWindow& mainWnd);
    virtual void SetCookie(char* url, char* cookie);
    virtual bool isExistJavascript(const wchar_t* szJSFunction, int index);
    virtual void executeJavascript(const wchar_t* szJSFunction, int index);
    virtual BOOL ZoomPageOnTab(float fZoom, UINT iTab);
    virtual BOOL ZoomTextOnTab(int nZoom, UINT iTab);
    virtual int GetTextZoomOnTab(UINT iTab);
    virtual BOOL GetTitleOnTab(LPTSTR szURL, UINT iMaxLen, UINT iTab);
    virtual void OnDocumentComplete(LPCTSTR url);
    virtual void setBrowserGesturing(bool bEnableGesturing);
    virtual void NotifyEngineOfSipPosition();
    virtual void setNavigationTimeout(unsigned int dwMilliseconds);
    virtual bool RegisterForMessage(unsigned int iMsgId);
    virtual bool DeRegisterForMessage(unsigned int iMsgId);
    virtual bool RegisterForPrimaryMessage(unsigned int iMsgId);
    virtual bool DeRegisterForPrimaryMessage(unsigned int iMsgId);

    HWND GetParentHWND(){ return m_hParentWnd;}
    CEng* getWebKitEngine() const;
    CRhoWKBrowserConfig* getWKConfig(){ return m_WKConfig; }
};

}
