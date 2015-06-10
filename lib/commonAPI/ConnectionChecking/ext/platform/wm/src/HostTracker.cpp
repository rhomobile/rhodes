#include <common/RhodesApp.h>
#include <time.h>
#include "resource.h"
#include "HostTracker.h"
#include "HostTrackerConfigInfo.h"

extern UINT WM_BROWSER_ONNAVIGATECOMPLETE ;

extern "C" HWND getMainWnd();
extern "C" HINSTANCE rho_wmimpl_get_appinstance();
extern "C" stHostTrackerConfigInfo* rho_wmimpl_get_hostTrackerInfo();
extern "C" WCHAR* rho_wmimpl_get_BadLinkURLPath();
extern "C" WCHAR* rho_wmimpl_get_HostTrackerDlgMsg();

using namespace rho::apiGenerator;
using namespace rho::common;

CHostTracker* HostTrackerFactory::createHostTracker() {
    return new CHostTracker();
}

HWND CHostTracker::m_hConnectDlg = NULL;
rho::StringW CHostTracker::m_szConnectionDlgMsg;
HBRUSH CHostTracker::m_hbrBackground = NULL;

CHostTracker::CHostTracker()
{
	m_hConnectDlg = NULL;	
	m_hTimeOutProcId = NULL;
	m_hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
}
CHostTracker::~CHostTracker()
{
	StopNetworkChecking();
	closeAllEvents();
	if(NULL != m_hbrBackground)
	{
		DeleteObject(m_hbrBackground);
	}

}
void CHostTracker::run()
{		
	//here we create the events used to perform various jobs
	m_hEvents[eCancelEventIndex] = CreateEvent(NULL, FALSE, FALSE, NULL);	
	m_hTimeOutProcCancelEvent = CreateEvent(NULL, TRUE, FALSE, NULL);	
	m_hEvents[eLicenseScreenHidesEventIndex] = CreateEvent(NULL, FALSE, FALSE, NULL);
	//do not start the loop till license screen hides
	LOG(INFO)  + "waiting for license screen hide event";
	WaitForSingleObject(m_hEvents[eLicenseScreenHidesEventIndex], INFINITE);	

	//if start page is not a badlink, we start TimeoutProc now
	if(false == m_bIsOnBadlink)
	{
		TimeoutProcHandler(true);
	}	
	LOG(INFO)  + "starting event listner";
	EventListner();	

}
void CHostTracker::EventListner (void)
{
	bool bResume = true;
	m_hEvents[eLicenseScreenPopupEventIndex] = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hEvents[eNavigateCompleteEventIndex] = CreateEvent(NULL, FALSE, FALSE, NULL);	
	while(bResume)
	{
		//let us wait for the events here
		DWORD dwEvent = WaitForMultipleObjects(4, m_hEvents, FALSE, INFINITE);
		DWORD dwEventIndex = dwEvent - WAIT_OBJECT_0;                 
		switch(dwEventIndex)
		{
		case eCancelEventIndex:
			{
				LOG(INFO)  + "cancel event handler is invoked";
				bResume = false;
				//fire cancel event for timeoutproc
				TimeoutProcHandler(false);
				break;
			}
		case eLicenseScreenPopupEventIndex:
			{
				LOG(INFO)  + "License popup event handler is invoked";
				//fire cancel event for timeoutproc
				TimeoutProcHandler(false);
				break;
			}
		case eLicenseScreenHidesEventIndex:
			{
				LOG(INFO)  + "license hide event handler is invoked";
				//start the timeoutproc
				TimeoutProcHandler(true);
				break;
			}
		case eNavigateCompleteEventIndex:
			{
				LOG(INFO)  + "nav complete event handler is invoked";
				//if navigate to a badlink, fire cancel event for timeoutproc
				//else start the timeoutproc
				if(m_bIsOnBadlink)
				{
					TimeoutProcHandler(false);
				}
				else
				{
					TimeoutProcHandler(true);
				}
				break;
			}
		}

	}
}
void CHostTracker::TimeoutProcHandler(bool bStartThread)
{
	if(bStartThread)
	{	
		//start the thread only after 2 seconds
		wait(2000);
		if(NULL == m_hTimeOutProcId)
		{			
			LOG(INFO) + "Starting TimeoutProc";
			m_hTimeOutProcId = CreateThread (NULL, 0, TimeoutProc, this, 0, NULL);
		}
		else
		{
			LOG(WARNING) + "TimeoutProc Already running";
		}
	}
	else
	{
		if(NULL != m_hTimeOutProcId)
		{
			if(m_hTimeOutProcCancelEvent)
			{
				LOG(INFO) + "firing cancel event for timeoutproc";
				SetEvent(m_hTimeOutProcCancelEvent);
				//never allow multiple timeproc, wait till safe exit
				WaitForSingleObject(m_hTimeOutProcId, INFINITE);
				CloseHandle(m_hTimeOutProcId);
				m_hTimeOutProcId = NULL;
				//resetting the event manually so that even if the thread would have alredy terminated, 
				//signal will be moved to non signald state
				ResetEvent(m_hTimeOutProcCancelEvent);
				LOG(INFO) + "Stoping TimeoutProc";

			}

		}
	}
}
DWORD CHostTracker::TimeoutProc (LPVOID pparam)
{
	CHostTracker *pthis = (CHostTracker*) pparam;
	pthis->ProcessTimeout ();
	return 0;
}
void CHostTracker::ProcessTimeout (void)
{
    bool bContinue = true;
    bool bConnectionExist = false;
    double dBadLinkTimer = 0;
	double dDuration;
	clock_t start;
    WCHAR logBuff[1024];
    while(bContinue)
	{	
		start = clock();		
        if(CheckConnectivity())
        {
            LOG(INFO) + "ConnectionChecking: CheckConnectivity passed";
            bConnectionExist = true;
            //on success hide dialog box
            HandleConnectionBox(eHideConnectionBox);
            //reset timer
            dBadLinkTimer =0.0;

        }
        else
        {
			dDuration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
            LOG(INFO) + "ConnectionChecking: CheckConnectivity failed";
            bConnectionExist = false;
            //on failure display connection box
            HandleConnectionBox(eShowConnectionBox);
        }
        if((WaitForSingleObject (m_hTimeOutProcCancelEvent, m_iNetworkPollInterval) == WAIT_TIMEOUT))
        {
            //if timed out
            if(false == bConnectionExist)
            {
                //if no connectivity increment timer and check for Dialog timeout condition
                dBadLinkTimer = dBadLinkTimer + dDuration + m_iNetworkPollInterval;
                wsprintf(logBuff, L"BadLinkTimer updated to nBadLinkTimer= %f,", dBadLinkTimer);
                LOG(INFO) + logBuff;
                if(dBadLinkTimer >= m_iConnectionDlgTimeout)
                {
                    LOG(INFO) + "ConnectionChecking: dialog timeout reached";
                    //hide connection box
                    HandleConnectionBox(eHideConnectionBox);
                    //navigate to badlink
                    rho::String navUrl = rho::common::convertToStringA(m_szBadLinkUrl);

                    RHODESAPP().navigateToUrl(navUrl.c_str());
                    //terminate the thread
                    bContinue = false;

                }
            }
        }
        else
        {
			HandleConnectionBox(eHideConnectionBox);
            //if EventListner fired a cancel event
            bContinue = false;
        }
    }

}
bool CHostTracker::InitConfig()
{	
	stHostTrackerConfigInfo* configInfo = rho_wmimpl_get_hostTrackerInfo();
	m_bIsFeatureEnabled = configInfo->isTrackConnectionSet;	
	if(m_bIsFeatureEnabled)
	{
		WCHAR logBuff[1024];
		LOG(INFO)  + "Setting host name as "+  configInfo->szHostName.c_str(); 
		SetHost(configInfo->szHostName);
		wsprintf(logBuff, L"Setting port no: %d ", configInfo->iPort);
		LOG(INFO)  + logBuff;
		SetPort(configInfo->iPort);
		wsprintf(logBuff, L"Setting poll interval: %d ", configInfo->iPollInterval);
		LOG(INFO)  + logBuff;
		SetNetworkPollInterval(configInfo->iPollInterval);
		SetConnectionTimeout(configInfo->iPingTimeout);
		wsprintf(logBuff, L"Setting dialog timeout as: %d ", configInfo->iDialogTimeout);
		LOG(INFO)  + logBuff;
		SetConnectionDlgTimeout(configInfo->iDialogTimeout);
		SetBadLinkUrl(rho_wmimpl_get_BadLinkURLPath());
		LOG(INFO) + "Setting badlink as " + m_szBadLinkUrl.c_str();
		m_szConnectionDlgMsg = rho_wmimpl_get_HostTrackerDlgMsg();	
	}
	
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
				LONG lLeft = 0;
				LONG lTop = 0;
				LONG lRight = 0;
				LONG lBottom = 0;
				GenerateConnectionBoxCoordinates(hwnd,lLeft, lTop, lRight, lBottom);
				//now set the new positon for the dialog box
				SetWindowPos(hwnd, 
					HWND_TOP, 
					lLeft, 
					lTop, 
					lRight, 
					lBottom,         
					SWP_NOACTIVATE);
				
				//draw a label on the dialog box, which fully occupy the area of dialogbox
				HWND hWndDlgText = CreateWindowEx(0,
					L"STATIC",
					m_szConnectionDlgMsg.c_str(),
					WS_CHILD | WS_VISIBLE | SS_LEFT ,
					3, 0, lRight-6, lBottom, //to avoid overlap of characters with the dialog boarder, keep margin for label
					hwnd,
					NULL,
					rho_wmimpl_get_appinstance(),
					NULL);				


				break; 
			}	
		case WM_CTLCOLORDLG:
			{				
				return (LONG)m_hbrBackground;		
			}
		case WM_CTLCOLORSTATIC:
			{				
				return (LONG)m_hbrBackground;
			}
		case WM_SETTINGCHANGE:
			{
				// Determine whether wParam's contains 
				// the value SETTINGCHANGE_RESET.
				if (wParam == SETTINGCHANGE_RESET)
				{
					// Redraw to fit the dimensions of the new client area.
					LONG lLeft = 0;
					LONG lTop = 0;
					LONG lRight = 0;
					LONG lBottom = 0;
					GenerateConnectionBoxCoordinates(hwnd,lLeft, lTop, lRight, lBottom);

					//now set the new positon for the dialog box
					SetWindowPos(hwnd, 
						HWND_TOP, 
						lLeft, 
						lTop, 
						lRight, 
						lBottom,         
						SWP_SHOWWINDOW);
					HWND hWndDlgText = GetWindow(hwnd, GW_CHILD);
					if(NULL != hwnd)
					{
						SetWindowPos(hWndDlgText,
							HWND_TOP,					
							3,
							0, 
							lRight -6, 
							lBottom,
							SWP_SHOWWINDOW
							);
					}

				}
				break;
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
    size_t f;
    while( (f= inputString.find(toReplace))!= -1)
    {
        inputString.replace(f, toReplace.length(), replaceWith);
    }
    return inputString;
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
void CHostTracker::GenerateConnectionBoxCoordinates(HWND hwnd, LONG& lLeft, LONG& lTop, LONG& lRight, LONG& lBottom)
{
	HWND hParentWnd; 
	RECT rcDlg, rcParent;			


	//As the coordinates varies from devices to device,height decison
	//based on the coordinates is not a good approach
	//hence hardcode the height of the dialog box in dialog units, and convert it to
	//pixel value at runtime using MapDialogRect API
	rcDlg.left =0;
	rcDlg.top =0;
	rcDlg.right = 0; 
	rcDlg.bottom  = 60;////initialize width

	MapDialogRect(hwnd, &rcDlg);

	//now calculate the left, top, right at runtime
	//Get the owner window and dialog box rectangles. 
	hParentWnd = GetParent(hwnd);
	GetWindowRect(hParentWnd, &rcParent); 

	lLeft = 20; //hardcoded to start with a margin of 20 from left
	lTop = (rcParent.bottom / 2) - (rcDlg.bottom / 2); //top  parentHeight/2 - dlgHeight/2
	lRight = rcParent.right-40; //width is parent width-40 (20 used for margins on both side)
	lBottom = rcDlg.bottom;
}
void CHostTracker::CreateNoConnectionDialog(int unused)
{
	m_hConnectDlg = CreateDialog(rho_wmimpl_get_appinstance(), MAKEINTRESOURCE(IDD_CONNECTION_DLG),
		getMainWnd(), &CHostTracker::ConnectDlgProc);
	if(m_hConnectDlg != NULL)
	{
		ShowWindow(m_hConnectDlg, SW_SHOW);		
		UpdateWindow(m_hConnectDlg);
	}
	else
	{
		LOG(ERROR) + "CreateDialog failed"; 
	}  				
}
void CHostTracker::HandleConnectionBox(eConnectionBoxMode mode)
{
	switch(mode)
	{
	case eShowConnectionBox:
		{
			LOG(INFO) + "inside show connection box";
			if(NULL == m_hConnectDlg)//if dlg not present create
			{
				LOG(INFO) + "inside create dialog";
				typedef void (*CREATE_NO_CONNECT_DIALOG)(int);
				CStaticClassFunctor<CREATE_NO_CONNECT_DIALOG, int>* FunPtr = new CStaticClassFunctor<CREATE_NO_CONNECT_DIALOG, int>(&CHostTracker::CreateNoConnectionDialog, 0);
				// Key blocking only has to be done once
				if(FunPtr != NULL)
				{
					//create the dialog box in ui thread
					rho_os_impl_performOnUiThread( FunPtr );
				}				
				
			}
			break;
		}
	case eHideConnectionBox:
		{			
			if(NULL != m_hConnectDlg)//if dlg present kill it
			{
                LOG(INFO) + "inside hide connection box";
                DestroyWindow(m_hConnectDlg);
                m_hConnectDlg = NULL;				
			}
			break;
		}
	}
}
void CHostTracker::fireEvent(eEventIndex eventIdx)
{
	WCHAR logBuff[1024];
	switch(eventIdx)
	{
	
	case eLicenseScreenHidesEventIndex:
		{
			if(false == m_bIsOnBadlink)
			{
				//fire this event if not on badlink
				if (m_hEvents[eLicenseScreenHidesEventIndex])
				{
					LOG(INFO) + "Firing license screen hide event";
					SetEvent(m_hEvents[eLicenseScreenHidesEventIndex]);
				}
			}
			break;
		}
	case eNavigateCompleteEventIndex:
		{
			m_bIsOnBadlink = isNavigatedToBadLink();
			//set this even if we already navigated to a badlink due to dialog timeout
			if (m_hEvents[eNavigateCompleteEventIndex])
			{
				LOG(INFO) + "Firing nav complete event";
				SetEvent(m_hEvents[eNavigateCompleteEventIndex]);
			}

			break;
		}
	default:
		{			
			if (m_hEvents[eventIdx])
			{
				wsprintf(logBuff, L"Firing event with inde = %d", eventIdx);
				LOG(INFO) + logBuff;
				SetEvent(m_hEvents[eventIdx]);
			}
			break;
		}
	}
}
void CHostTracker::closeAllEvents()
{
	for(int nIndex = 0; nIndex < eMaxEventCount; nIndex++)
	{
		if(m_hEvents[nIndex])
		{
			CloseHandle(m_hEvents[nIndex]);
			m_hEvents[nIndex] = NULL;
		}
	}
	if(m_hTimeOutProcCancelEvent)
	{
		CloseHandle(m_hTimeOutProcCancelEvent);
		m_hTimeOutProcCancelEvent=NULL;
	}
}

bool CHostTracker::isNavigatedToBadLink()
{
	bool isBadLink = true;
	rho::StringW navUrl = replaceString(m_szNavigatedUrl, L"%20", L" ");
	//replace back slash in badlink url to front slash
	rho::StringW badUrl = replaceString(m_szBadLinkUrl, L"\\", L"/");

	LOG(INFO)  + "CHostTracker::run navigated url"+  navUrl.c_str(); 
	LOG(INFO)  + "CHostTracker::run badlink url"+  badUrl.c_str();

	if(std::string::npos == navUrl.find(badUrl))
	{		
		isBadLink = false;
	}
	return isBadLink;
}
