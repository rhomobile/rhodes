#include <common/RhodesApp.h>
#include "resource.h"
#include "HostTracker.h"
#include "HostTrackerConfigInfo.h"

extern UINT WM_BROWSER_ONNAVIGATECOMPLETE ;

extern "C" HWND getMainWnd();
extern "C" HINSTANCE rho_wmimpl_get_appinstance();
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
}
CHostTracker::~CHostTracker()
{
	StopNetworkChecking();
	closeAllEvents();

}
void CHostTracker::run()
{
	
	bool bConnectDialogTimeOut = false;	
	bool isConnectionBoxDisplayed = false;
	bool canResumeThread = true;
	int badLinkTimer = 0; //on timeout this variable desides whether to display box or not
	int nPollInterval = INFINITE; //this will be modified within the thread based on various events.
	eConnectionBoxMode mode = eNone;
	//here we create the events used to perform various jobs
	m_hEvents[eCancelEventIndex] = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hEvents[eLicenseScreenPopupEventIndex] = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hEvents[eLicenseScreenHidesEventIndex] = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hEvents[eNavigateCompleteEventIndex] = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	while(canResumeThread)
	{
		//let us wait for the events here
		DWORD dwEvent = WaitForMultipleObjects(4, m_hEvents, FALSE, nPollInterval);
		if (dwEvent == WAIT_TIMEOUT)
		{
			mode = OnTimeoutEvent(nPollInterval, badLinkTimer);
		}
		else
		{
			DWORD dwEventIndex = dwEvent - WAIT_OBJECT_0;                 
			switch(dwEventIndex)
			{
			case eCancelEventIndex:
				{
					canResumeThread = false;
					mode = eHideConnectionBox;
					break;
				}
			case eLicenseScreenPopupEventIndex:
				{
					mode = eHideConnectionBox;
					badLinkTimer = 0;
					nPollInterval = INFINITE;
					break;
				}
			case eLicenseScreenHidesEventIndex:
				{
					mode = eNone;
					badLinkTimer = 0;
					nPollInterval = 0;
					break;
				}
			case eNavigateCompleteEventIndex:
				{
					mode = OnNavCompleteEvent(nPollInterval, badLinkTimer);
					break;
				}
			}
		}
		HandleConnectionBox(mode);


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

	
	return true;
}
bool CHostTracker::SetConnectionDlgTimeout(int iTimeout)
{
	m_iConnectionDlgTimeout = iTimeout;
	return true;
}
void CHostTracker::Cleanup()
{
    stop(0);

	WSACleanup();
}
bool CHostTracker::SetConnectionTimeout (int iTimeout)
{
	m_iPingTimeOut = iTimeout;
	return CNetworkDetection::SetConnectionTimeout(iTimeout);
}
bool CHostTracker::SetBadLinkUrl(const wchar_t* badLinkUrl)
{	
	m_szBadLinkUrl = badLinkUrl;
	return true;
}
bool CHostTracker::SetNavigatedUrl(const wchar_t* navigatedUrl)
{
	m_szNavigatedUrl = navigatedUrl;
	return true;
}
BOOL CALLBACK CHostTracker::ConnectDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
		case WM_INITDIALOG:
			{				
				SetDlgItemText(hwnd, IDC_STATIC_CONNECTION_MSG, m_szConnectionDlgMsg.c_str());
				return TRUE;
			}
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
		fireEvent(eCancelEventIndex);
		stop(0);
		Cleanup();
	}
	return true;
}
rho::String CHostTracker::getHostURL()
{
	return m_szHost;
}
int CHostTracker::getDailogTimeout()
{
	return m_iConnectionDlgTimeout;
}
int CHostTracker::getPollInterval()
{
	return m_iNetworkPollInterval;
}
rho::StringW  CHostTracker::getMessage()
{
	return m_szConnectionDlgMsg;
}
eConnectionBoxMode CHostTracker::OnTimeoutEvent(int& nPollInterval, int& nBadLinkTimer)
{
	eConnectionBoxMode mode = eNone;
	m_bIsOnBadlink = false;
	nBadLinkTimer = nBadLinkTimer + nPollInterval;
	nPollInterval = m_iNetworkPollInterval;
	if( CheckConnectivity())
	{			
		mode = eHideConnectionBox;
		nBadLinkTimer =0;

	}
	else
	{	
		mode = eShowConnectionBox;
		nBadLinkTimer = nBadLinkTimer + m_iPingTimeOut;
		if(nBadLinkTimer >= m_iConnectionDlgTimeout)
		{
			//navigate to badlink
			rho::String navUrl = rho::common::convertToStringA(m_szBadLinkUrl);

			RHODESAPP().navigateToUrl(navUrl.c_str());

			//set badlink counter to zero
			nBadLinkTimer =0;
			mode = eHideConnectionBox;
			nPollInterval = INFINITE;//once dlg time out happens we have to infinite until we get a navcomplete or any other event
			m_bIsOnBadlink = true;

		}	
	}	
	return mode;
}
eConnectionBoxMode CHostTracker::OnNavCompleteEvent(int& nPollInterval, int& nBadLinkTimer)
{
	eConnectionBoxMode mode = eNone;
	nPollInterval = INFINITE;
	nBadLinkTimer = 0;
	rho::StringW navUrl = replaceString(m_szNavigatedUrl, L"%20", L" ");
	//replace back slash in badlink url to front slash
	rho::StringW badUrl = replaceString(m_szBadLinkUrl, L"\\", L"/");


	LOG(INFO)  + "CHostTracker::run navigated url"+  navUrl.c_str(); 
	LOG(INFO)  + "CHostTracker::run badlink url"+  badUrl.c_str();

	if(std::string::npos == navUrl.find(badUrl))
	{	
		//user moved out of badlink page, resume thread
		nPollInterval = 0;
	}
	return mode;
}

void CHostTracker::HandleConnectionBox(eConnectionBoxMode& mode)
{
	switch(mode)
	{
	case eShowConnectionBox:
		{
			if(NULL == m_hConnectDlg)//if dlg not present create
			{
				PostMessage(getMainWnd(), WM_ON_CONNECTION_BOX, 1, 0);     
			}
			break;
		}
	case eHideConnectionBox:
		{
			if(NULL != m_hConnectDlg)//if dlg present kill it
			{
				PostMessage(getMainWnd(), WM_ON_CONNECTION_BOX, 0, 0);
			}
			break;
		}
	}
}
void CHostTracker::fireEvent(eEventIndex eventIdx)
{
	switch(eventIdx)
	{
	
	case eLicenseScreenHidesEventIndex:
		{
			if(false == m_bIsOnBadlink)
			{
				//fire this event if not on badlink
				if (m_hEvents[eLicenseScreenHidesEventIndex])
				{
					SetEvent(m_hEvents[eLicenseScreenHidesEventIndex]);
				}
			}
			break;
		}
	case eNavigateCompleteEventIndex:
		{
			if(m_bIsOnBadlink)
			{
				//set this even if we already navigated to a badlink due to dialog timeout
				if (m_hEvents[eNavigateCompleteEventIndex])
				{
					SetEvent(m_hEvents[eNavigateCompleteEventIndex]);
				}
			}
			break;
		}
	default:
		{			
			if (m_hEvents[eventIdx])
			{
				SetEvent(m_hEvents[eventIdx]);
			}
			break;
		}
	}
}
bool CHostTracker::onWndMsg(MSG& oMsg)
{
	bool retStatus = false;
	if(oMsg.message == WM_ON_CONNECTION_BOX)
	{
		bool bCreate = static_cast<bool>(oMsg.wParam);
		if(bCreate)
		{
			m_hConnectDlg = CreateDialog(rho_wmimpl_get_appinstance(), MAKEINTRESOURCE(IDD_CONNECTION_DLG),
				oMsg.hwnd, &CHostTracker::ConnectDlgProc);
			if(m_hConnectDlg != NULL)
			{
				ShowWindow(m_hConnectDlg, SW_SHOW);
			}
			else
			{
				LOG(ERROR) + "CreateDialog in CHostTracker returned NULL"; 
					
			}
		}
		else
		{
			DestroyWindow(m_hConnectDlg);
			m_hConnectDlg = NULL;
		}
		retStatus = true;
	}
	return retStatus;
}
void CHostTracker::closeAllEvents()
{
	for(int nIndex = 0; nIndex < eMaxEventCount; nIndex++)
	{
		if(m_hEvents[nIndex])
		{
			CloseHandle(m_hEvents[nIndex]);
		}
	}
}

