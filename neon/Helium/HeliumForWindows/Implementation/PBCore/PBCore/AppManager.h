#pragma once

#include "App.h"
#ifndef BUILD_AS_RHODES_EXTENSION
#include "Eng.h"
#endif
#include "XMLReader.h"
#include "CriticalSect.h"





/**
*	Sets Browser into Fullscreen/smallscreen
*	@param bFullScreen if TRUE then Fullscreen; if FALSE non fullscreen
*
*/
void	PBScreenMode(PB_ScreenMode screenMode, BOOL bAccountForSIP, BOOL bAccountForIgnoreSettingChange = TRUE);
void	SetBrowserSize(RECT rcBrowser);


class CAppManager:public Lockable
{
public:
	CAppManager();
	~CAppManager(void);

#ifndef BUILD_AS_RHODES_EXTENSION
	BOOL	Init				(HINSTANCE hInstance,HWND hWndParent,PPBCORESTRUCT	pbCoreStruct,CEng *pEngine,LPCTSTR pAppName,LPCTSTR PStartPage);
#else
	BOOL	Init				(HINSTANCE hInstance,HWND hWndParent,PPBCORESTRUCT	pbCoreStruct,LPCTSTR pAppName,LPCTSTR PStartPage);
#endif
	void	RemoveApp			(int iID);
	int		AddApp				(LPCTSTR pAppName,LPCTSTR PStartPage);///<returns the Instance ID of the application 
	CApp	*GetApp				(int iID){return m_pApp[iID];};
	//stApps	*GetRecord			(int iIndex);
	LRESULT	SwitchAppInst		(int iInstID);
	LRESULT	ProcessOnTopMostWnd	(HWND hWnd, int tabIndex);
	//LRESULT OnMeta			(int iInstID,LPCTSTR pModuleName,LPCTSTR pName,LPCTSTR pValue);
	void	LoadDummy			();
	static	LRESULT CALLBACK HTMLWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	/**
	*  Function used by the engine to retrieve configuration settings.  A pointer
	*  to this function is provided to the engine at initialisation of each 
	*  application.
	*  \param iInstID Application instance whose configuration setting is being
	*         obtained.
	*  \param tcSetting Configuration setting being obtained with group name, 
	*         e.g. HTMLStyles\\FitToScreenEnabled.
	*  \param tcValue [out] Value read from the configuration XML file.  This 
	*         value will be NULL if the read did not succeed.
	*  \return S_OK
	*/
	static	LRESULT CALLBACK GetEngineConfig(int iInstID, LPCTSTR tcSetting, TCHAR* tcValue);
	CApp	*m_pApp[PB_APP_ARRSIZE];
	BOOL	InitApp(int iInstID,LPCTSTR pStartPage, LPCTSTR pAppName);
	void    InitBadlinkInfo(int iInstID);
	void	AddLicenceApp();
	LPCTSTR GetCurrURL(int iInstID){return GetApp(iInstID)->m_szCurrentURL;};
	void	SetCurrURL(int iInstID,LPCTSTR pURL){wcscpy(GetApp(iInstID)->m_szCurrentURL,pURL);};
#ifndef BUILD_AS_RHODES_EXTENSION
	CEng					*m_pEngine;
#endif
	int						m_iLastID;
	PPBCORESTRUCT			m_pbCoreStruct;
	LPWSTR					m_pEngDLLFilePath;
	//  Breaks multiple instance to have this indication at this level
	BOOL					m_bUsingLegacyEngine;
private:

	LPWSTR			m_pLoadFile;
	
	
	
	HINSTANCE		m_hInstance;
	HWND			m_hMainWnd;
	HANDLE			m_hRegTopWnd;
	IMORec *		deleteIMO		(IMORec * pIMORec);
	CApp			*m_pLicenseApp;
	CApp			*m_pMenuApp;
};
