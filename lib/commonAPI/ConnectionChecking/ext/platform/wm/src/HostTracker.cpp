
//#include "stdafx.h"

//#include "MainWindow.h"
#include <common/RhodesApp.h>
#include "resource.h"
#include "HostTracker.h"
#include "HostTrackerConfigInfo.h"

extern UINT WM_BROWSER_ONNAVIGATECOMPLETE ;

extern "C" HWND getMainWnd();
//extern int rho_webview_active_tab(); 
extern "C" stHostTrackerConfigInfo* rho_wmimpl_get_hostTrackerInfo();
extern "C" WCHAR* rho_wmimpl_get_BadLinkURLPath();
extern "C" WCHAR* rho_wmimpl_get_HostTrackerDlgMsg();




CHostTracker* HostTrackerFactory::createHostTracker() {
    return new CHostTracker();
}

HWND CHostTracker::m_hConnectDlg = NULL;
rho::StringW CHostTracker::m_szConnectionDlgMsg;

CHostTracker::CHostTracker()
{
	m_hConnectDlg = NULL;
	m_bIsWaiting = false;
}
CHostTracker::~CHostTracker()
{
	if(NULL != m_hConnectDlg)
	{
		PostMessage(m_hConnectDlg,WM_CLOSE,NULL,NULL);
	}
	m_hConnectDlg = NULL;
}

void CHostTracker::run()
{
	unsigned int badLinkTimer = 0;
	bool bConnectDialogTimeOut = false;
	while(true)
	{
		//wait for the event to ensure user is not on a badlink. else wait infinite
		if( CheckConnectivity())
		{			
			//ping passed, if a dialog present kill it
			if(NULL != m_hConnectDlg)
			{
				SendMessage(m_hConnectDlg,WM_CLOSE,NULL,NULL);
			}
			badLinkTimer =0;
			
		}
		else
		{	
			if(NULL == m_hConnectDlg)
			{
				//check failed 2s ping timeout			
				CloseHandle(CreateThread(NULL,0,&CHostTracker::ConnectionDlgCreator,NULL,0,0));
			}
			badLinkTimer = badLinkTimer + m_iPingTimeOut;
			if(badLinkTimer >= m_iConnectionDlgTimeout)
			{
				
				//navigate to badlink
				rho::String navUrl = rho::common::convertToStringA(m_szBadLinkUrl);

				RHODESAPP().navigateToUrl(navUrl.c_str());

				//set badlink counter to zero
				badLinkTimer =0;
				//reset the event sothat this thread will wait until user navigate out of badlink
				
				bConnectDialogTimeOut = true;
				
			}		
				
			

		}	
		
		if (bConnectDialogTimeOut)
		{
			//kill the box
			SendMessage(m_hConnectDlg,WM_CLOSE,NULL,NULL);
			//StopNetworkChecking();
			MSG msg;				
			m_bIsWaiting = true;
			while(GetMessage(&msg, NULL, 0, 0) > 0)
			{
				//wait until navigated out of badlink
				if(msg.message == WM_BROWSER_ONNAVIGATECOMPLETE)
				{	
					
					//replace %20 with space from the navigated url
					rho::StringW navUrl = replaceString(m_szNavigatedUrl, L"%20", L" ");
					//replace back slash in badlink url to front slash
					rho::StringW badUrl = replaceString(m_szBadLinkUrl, L"\\", L"/");
				 

					LOG(ERROR)  + "CHostTracker::run navigated url"+  navUrl.c_str(); 
					LOG(ERROR)  + "CHostTracker::run badlink url"+  badUrl.c_str();
					
					if(std::string::npos == navUrl.find(badUrl))
					{
						//if navigated out of badlink, resume our thread by breaking this loop	
						LOG(ERROR)  + "CHostTracker::run thread will be resumed as it is a diffrent page";
						break;

					}
					
				}
			}		

			m_bIsWaiting = false;
			bConnectDialogTimeOut= false;
		}
		else
		{

			//here we should wait for and even with timeout
			wait(m_iNetworkPollInterval);
			//at the end of every poll, if dialog present increment badlink timer by poll interval
			if(NULL != m_hConnectDlg)
			{
				badLinkTimer = badLinkTimer + m_iNetworkPollInterval;
			}
		}
	}
}
bool CHostTracker::InitConfig()
{	
	stHostTrackerConfigInfo* configInfo = rho_wmimpl_get_hostTrackerInfo();
	m_bIsFeatureEnabled = configInfo->isTrackConnectionSet;
	SetHost(configInfo->szHostName);
	SetPort(configInfo->iPort);
	SetNetworkPollInterval(configInfo->iPollInterval);
	SetConnectionTimeout(configInfo->iPingTimeout);
	SetConnectionDlgTimeout(configInfo->iDialogTimeout);
	SetBadLinkUrl(rho_wmimpl_get_BadLinkURLPath());
	m_szConnectionDlgMsg = rho_wmimpl_get_HostTrackerDlgMsg();

	
	/*else
	{
		LOG(WARNING) + "Unable to initialize HostTracker from config file, default settings will be used";
	}*/
	return true;
}
bool CHostTracker::SetConnectionDlgTimeout(unsigned int iTimeout)
{
	m_iConnectionDlgTimeout = iTimeout;
	return true;
}
bool CHostTracker::SetConnectionTimeout (unsigned int iTimeout)
{
	m_iPingTimeOut = iTimeout;
	return CNetworkDetection::SetConnectionTimeout(iTimeout);
}
bool CHostTracker::SetBadLinkUrl(const wchar_t* badLinkUrl)
{	
	m_szBadLinkUrl = badLinkUrl;
	return true;
}
bool CHostTracker::isWaitingForWindowMsg()
{
	return m_bIsWaiting;
}
bool CHostTracker::SetNavigatedUrl(const wchar_t* navigatedUrl)
{
	m_szNavigatedUrl = navigatedUrl;
	return true;
}
DWORD CHostTracker::ConnectionDlgCreator(LPVOID lParam)
{	
	int ret = DialogBox(GetModuleHandle(NULL), 
                MAKEINTRESOURCE(IDD_CONNECTION_DLG), getMainWnd(), &CHostTracker::ConnectDlgProc);
	m_hConnectDlg =NULL;
	return 1;
}

BOOL CALLBACK CHostTracker::ConnectDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG:
			{
				m_hConnectDlg = hwnd;
				SetDlgItemText(hwnd, IDC_STATIC_CONNECTION_MSG, m_szConnectionDlgMsg.c_str());

        return TRUE;
			}
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hwnd, IDOK);
                break;
                case IDCANCEL:
                    EndDialog(hwnd, IDCANCEL);
                break;
            }
        break;
        default:
            return FALSE;
    }
    return TRUE;
}
rho::StringW CHostTracker::replaceString(rho::StringW inputString,
                      rho::StringW toReplace,
                      rho::StringW replaceWith)
{
	rho::StringW outString;
	size_t f;
	 while( (f= inputString.find(toReplace))!= -1)
	 {
		 outString = inputString.replace(f, toReplace.length(), replaceWith);
	 }
	 return outString;
}
bool CHostTracker::StartNetworkChecking()
{
	if (!IsChecking())
	{				
		Initialise();
		CNetworkDetection::StartNetworkChecking();	
	}
	return true;
}
bool CHostTracker::StopNetworkChecking()
{
	if ( IsChecking() ) {
		stop(0);
		Cleanup();
	}
	return true;
}
void CHostTracker::resumeNetworkChecking(const wchar_t* navigatedUrl)
{
	if(m_bIsWaiting)
	{
		//we may need to create an event listner thread to stop or start network cheking
		//listner thread can listen multiple events such as license, on navigate complete etc.
		//we can remove this post message after implementing a listner thread
		SetNavigatedUrl(navigatedUrl);		
		PostThreadMessage(getThreadID(),WM_BROWSER_ONNAVIGATECOMPLETE,NULL,NULL); 
	}



}


