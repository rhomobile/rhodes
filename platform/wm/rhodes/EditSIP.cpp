#include "stdafx.h"
#include "EditSIP.h"

CEditSIP::CEditSIP()
{
	
	InitSIP();
}

CEditSIP::~CEditSIP()
{
	// GD Use FreeLibrary instead of CloseHandle
	FreeLibrary(hKeybdDriver);
	//CloseHandle(hKeybdDriver);

	hKeybdDriver = NULL;
	pfnSetKeyState = NULL;
}

void CEditSIP::InitSIP()
{

	hKeybdDriver = LoadLibrary(KEYBOARD_DLL);
	pfnSetKeyState = NULL;

	if (hKeybdDriver)
	{
		pfnSetKeyState = (LPFNSETKEYSTATE)GetProcAddress(hKeybdDriver, SETKEYSTATE);
		if (pfnSetKeyState)
		{
			//EMBPD00180707
			//pfnSetKeyState(UN_SHIFTED, 0, true);
		}
	}
}

BOOL CEditSIP::ToggleSystemSIP()
{

	//reset the keystate
	
	if (hKeybdDriver)
	{
		if (pfnSetKeyState)
		{
			//EMBPD00180707
			//pfnSetKeyState(UN_SHIFTED, 0, true);
		}
	}

	HWND hwndsipbut = FindWindow(L"MS_SIPBUTTON", NULL);

	if(hwndsipbut)
	{
		SendMessage(GetWindow(hwndsipbut, GW_CHILD), WM_LBUTTONDOWN, 1, 0xA0009);
		SendMessage(GetWindow(hwndsipbut, GW_CHILD), WM_LBUTTONUP, 0, 0x5000B);
		return TRUE;
	}
	
	return FALSE;
}

void CEditSIP::ToggleSIP()
{
	//first try the system sip (ie windows mobile)
	if(ToggleSystemSIP())
	{
		return;
	}

	//failing that we'll use the api

	SIPINFO sipinfo;
	HRESULT hr = 0;
	memset(&sipinfo, 0, sizeof(SIPINFO));
	sipinfo.cbSize = sizeof(SIPINFO);
	sipinfo.dwImDataSize = 0;

	SipGetInfo(&sipinfo);

	if(sipinfo.fdwFlags & SIPF_ON){
		SipShowIM(SIPF_OFF);				
	}
	else{
		SipShowIM(SIPF_ON);
	}
}





void ShowSIPWnd(BOOL bShow)
{
	CEditSIP m_Sip;

	if(!bShow)
	{
		HWND sipHWND = FindWindow(L"SipWndClass", NULL);
		if (sipHWND && IsWindowVisible(sipHWND))
		{
			m_Sip.ToggleSIP();
		}
	}
	if(bShow)
	{
		bool bSIPVisible = false;
		HWND sipHWND = FindWindow(L"SipWndClass", NULL);
		if (sipHWND && IsWindowVisible(sipHWND))
			bSIPVisible = true;
		if(!bSIPVisible)
		{
			m_Sip.ToggleSIP();
			
		}
	}
	

}

