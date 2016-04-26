#pragma once

#include "..\..\Common\Public\PB_Defines.h"
#include "Eng.h"
#include "..\..\Common\Private\Emml.h"

enum BadLinkType {
	BADLINK_STOPPED	= 0,	// Browser Stopped 
	BADLINK_TIMEOUT,		// Browser Timeout receiving page
	BADLINK_NAVERROR,		// Browser DNS Error (amongst other things)
	BADLINK_UNLICENSED,		// Application License did not apply
};

class CApp
{
public:
	CApp(void);
	~CApp(void);
	/*
	*
	*	
	*
	*
	*/
	BOOL Init(HINSTANCE hInstance,int iInstID,LPCTSTR szAppName,LPCTSTR szStartPage);
	BOOL DoPreloads();	
	
	BOOL				Activate		(BOOL bShow);///< places the window created in this object on the top
	void				DeActivate		();///<Cleans up and removes the window
	
	void				RunDefaultMetaTags();

	LPCTSTR				GetBadlink(LPCTSTR pCurrURL, BadLinkType eReason);//appends the current URL to the badlink URL 
	PPBSTRUCT			GetAppPBStruct	();
	PBStruct			m_pbStruct;
		
	BOOL				CallPlugMethod(LPCTSTR pTargetMod,LPCTSTR pMethod);
	BOOL				SetPlugProperty(LPCTSTR pTargetMod,LPCTSTR pParam,LPCTSTR pValue);
	BOOL				Home();// Navigate to the homepage
	CEMML				*m_pEMML;
	
	//CEng				*m_pEngine;
	WCHAR				m_szCurrentURL[MAX_URL + 1];
	
	
	WNDPROC				m_OwnerProc;
	LPWSTR				m_pHomePage;
	BoolSettingValue	m_bScrollBarsEnabled;
	eScrollTechnique	m_iScrollTechnique;
	BOOL				m_bDisableScrolling;
	BOOL				m_bBadlink;
	friend				class CAppManager;
private:
	BOOL				LoadRegExFile();
	/**
	*  Read the value of 'DebugButtonsEnabled' from the configuration file
	*  and display them if appropriate.
	*  \return Whether or not the value was successfully read from the config.
	*/
	BOOL				ShowDebugButtons();
	BOOL				ShowScrollBars();	// Needed for WM6.5 only
	BOOL				LoadBadlink();
	LPTSTR				m_pAppName;	
	TCHAR				m_szBadLinkURI[MAX_URL + 1];
	TCHAR				m_szBadLinkPlusURL[MAX_URL + 1];
	WCHAR				m_szScriptName[100];
	
	//BOOL				m_bFirstLoad;
	DWORD				m_dwNavTimeoutMs;
	
};
