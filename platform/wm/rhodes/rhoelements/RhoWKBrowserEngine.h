#pragma once

#include "IBrowserEngine.h"
#include "rhoelements/PBCore/PBCore/Config.h"

class CEng;

namespace rho
{

class CRhoWKBrowserEngine :  public rho::IBrowserEngine
{
    //static WNDPROC m_WebKitOwnerProc;
    //static CConfig* g_pConfig;
    //static WCHAR CRhoWKBrowserEngine::g_szConfigFilePath[MAX_PATH + 1];
    //static WCHAR CRhoWKBrowserEngine::g_szInstallDirectory[MAX_PATH + 1];
    CEng* m_pEngine;

    //static LRESULT CALLBACK WK_HTMLWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    //static LRESULT CALLBACK WK_GetEngineConfig(int iInstID, LPCTSTR tcSetting, TCHAR* tcValue);

    //LRESULT ProcessOnTopMostWnd(HWND hWnd, int tabIndex);
public:
    CRhoWKBrowserEngine(HWND hParentWnd, HINSTANCE hInstance);
    virtual ~CRhoWKBrowserEngine(void);

    virtual BOOL Navigate(LPCTSTR szURL);
    virtual HWND GetHTMLWND();
    virtual BOOL ResizeOnTab(int iInstID,RECT rcNewSize);
    virtual BOOL BackOnTab(int iInstID,int iPagesBack = 1);
    virtual BOOL ForwardOnTab(int iInstID);
    virtual BOOL ReloadOnTab(bool bFromCache, UINT iTab);
    virtual BOOL NavigateToHtml(LPCTSTR szHtml);
    virtual LRESULT OnWebKitMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual void RunMessageLoop(CMainWindow& mainWnd);
    virtual void SetCookie(char* url, char* cookie);

    CEng* getWebKitEngine() const;
};

}
