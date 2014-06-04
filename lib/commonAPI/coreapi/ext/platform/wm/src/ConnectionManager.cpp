#include "ConnectionManager.h"
using namespace rho::common;
#ifndef dim
	/**
	 * Macro to return the number of elements in an array
	 */
	#define dim(x) (sizeof(x) /sizeof(x[0]))
#endif

CWAN::CWAN(HWND hParentWnd)
{
	//  Initialise Variables
	m_hConnection = NULL;
	m_hParentWnd = hParentWnd;
	m_hConnectionManagerWatchdog = NULL;
	m_hStopConnectionManagerWatchdog = NULL;
	m_szLastRequestedConnectionDestionation = NULL;
}

CWAN::~CWAN()
{
	SetEvent(m_hStopConnectionManagerWatchdog);
	if (WaitForSingleObject(m_hConnectionManagerWatchdog, 1000) == WAIT_TIMEOUT)
	{
		//  Terminate the thread if it did not shut down in a second
		TerminateThread(m_hConnectionManagerWatchdog, 0);
	}
	CloseHandle(m_hConnectionManagerWatchdog);
	CloseHandle(m_hStopConnectionManagerWatchdog);
	m_hConnectionManagerWatchdog = NULL;
	m_hStopConnectionManagerWatchdog = NULL;
	//  Deregister for Registry notifications
	for (int i = 0; i < RE_WAN_MAX_NOTIFICATIONS; i++)
	{
		if (hRegistryNotifications[i])
		{
			if(lpfn_Registry_CloseNotification)
				lpfn_Registry_CloseNotification(hRegistryNotifications[i]);
		}
	}
}

//  Function called whenever any of our registry entries we are interested in change
void CWAN::PhoneStatusCallback(HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData)
{
	RegistryCallbackData* data = (RegistryCallbackData*)dwUserData;
	CWAN* pWAN = (CWAN*)data->pObject;
	switch(data->dwCallbackType)
	{
	case RE_WAN_SIGNAL_STRENGTH:
		{
			if ((!pWAN->m_bPhoneHasSim) && (!pWAN->m_bCDMADevice)) break;
			pWAN->m_dwSignal = *pData;
			pWAN->ReportWANStatus();
		}
		break;
	case RE_WAN_PHONE_SERVICE:
		{
		pWAN->m_bPhoneService = !(*pData & SN_PHONENOSERVICE_BITMASK);
		pWAN->ReportWANStatus();
		}
		break;
	case RE_WAN_CELL_SYSTEM_AVAILABLE:
		{
		pWAN->m_dwCellSystemAvailable = *pData;
		pWAN->ReportWANStatus();
		WCHAR szLog[128];
		wsprintf(szLog, L"Cellular System Available value has changed to 0x%08x %s",pWAN->m_dwCellSystemAvailable, pWAN->m_szCellularSystemAvailable);
		LOG(INFO) + szLog;
		}
		break;
	case RE_WAN_CELL_SYSTEM_CONNECTED:
		{
		pWAN->m_dwCellSystemConnected = *pData;
		pWAN->ReportWANStatus();
		WCHAR szLog[128];
		wsprintf(szLog, L"Cellular System Connected value has changed to 0x%08x %s", pWAN->m_dwCellSystemConnected,pWAN->m_szCellularSystemConnected);
		LOG(INFO) + szLog;
		}
		break;
	default:
		break;
	}

}


BOOL CWAN::Initialise() 
{
	//  Initialise values read from registry
	m_dwSignal = 0;
	m_dwCellSystemAvailable = 0;
	m_dwCellSystemConnected = 0;
	m_bPhoneHasSim = false;
	m_bCDMADevice = false;
	m_bRadioPresent = false;
	m_bRadioOn = false;
	m_bPhoneService = false;
	wcscpy(m_szConnectionManagerStatus, L"");
	wcscpy(m_szOperatorName, L"");
	wcscpy(m_szCellularSystemAvailable, L"");
	wcscpy(m_szCellularSystemConnected, L"");
	m_hAygShellDLL = NULL;
	//  Load Connection Manager dynamically
	m_hConnManDLL = NULL;
	m_currentConnectionManagerState = RE_NOT_CONNECTED;
	if (!LoadConnectionManager())
		return FALSE;

	//  Load AYGShell dynamically
	if (!LoadAYGShell())
		return FALSE;

	//  Ensure the device has the appropriate registry settings and register for notifications on them
	BOOL bRegistryPrerequisitesMet = RetrievePrerequisitesFromRegistry();
	if (bRegistryPrerequisitesMet)
	{
		for (int i = 0; i < RE_WAN_MAX_NOTIFICATIONS; i++)
			hRegistryNotifications[i] = NULL;
		RegisterForNotifications();
	}
	else
	{
		LOG(WARNING) + "Not all registry keys could be found to ensure WAN support, WAN connectivity will not work";
		return FALSE;
	}

	m_hStopConnectionManagerWatchdog = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_hStopConnectionManagerWatchdog)
		return FALSE;

	//  Write out the available connections to the log file
    int idx=0;
    CONNMGR_DESTINATION_INFO DestInfo = {0};
	while( true )
	{
		HRESULT hResult = lpfn_ConMgr_EnumDestinations( idx, &DestInfo );
		if ( hResult != S_OK )
			break;

		TCHAR szGUID[64]={0};
		StringFromGUID2( DestInfo.guid, szGUID, dim(szGUID) );
		WCHAR szDebug[256];
		if (wcslen(DestInfo.szDescription) < 200)
		{
			wsprintf( szDebug, TEXT("Available Connection Destination: %s GUID %s"),  DestInfo.szDescription, szGUID );
			LOG(INFO) + szDebug;
		}
		idx++;
	}

	return TRUE;
}

BOOL CWAN::RegisterForNotifications()
{
	//  Register for notifications from the registry
	HRESULT hr = S_OK;
	if (lpfn_Registry_NotifyCallback)
	{
		//  Register for Signal Strength
		pCallbackData[RE_WAN_SIGNAL_STRENGTH].pObject = this;
		pCallbackData[RE_WAN_SIGNAL_STRENGTH].dwCallbackType = RE_WAN_SIGNAL_STRENGTH;
		hr = lpfn_Registry_NotifyCallback(
			SN_PHONESIGNALSTRENGTH_ROOT,
			SN_PHONESIGNALSTRENGTH_PATH,
			SN_PHONESIGNALSTRENGTH_VALUE,
			PhoneStatusCallback,
			(DWORD)&pCallbackData[RE_WAN_SIGNAL_STRENGTH],
			NULL,
			&hRegistryNotifications[RE_WAN_SIGNAL_STRENGTH]);
		if (hr != S_OK)
		{
			LOG(WARNING) + "Failed to register to receive WAN signal strength notifications";
		}

		//  Register for Phone Service
		NOTIFICATIONCONDITION nc;
		nc.dwMask = SN_PHONENOSERVICE_BITMASK;
		nc.ctComparisonType = REG_CT_ANYCHANGE;
		nc.TargetValue.dw = 0;
		pCallbackData[RE_WAN_PHONE_SERVICE].pObject = this;
		pCallbackData[RE_WAN_PHONE_SERVICE].dwCallbackType = RE_WAN_PHONE_SERVICE;
		hr = lpfn_Registry_NotifyCallback(
			SN_PHONENOSERVICE_ROOT,
			SN_PHONENOSERVICE_PATH,
			SN_PHONENOSERVICE_VALUE,
			PhoneStatusCallback,
			(DWORD)&pCallbackData[RE_WAN_PHONE_SERVICE],
			&nc,
			&hRegistryNotifications[RE_WAN_PHONE_SERVICE]);
		if (hr != S_OK)
		{
			LOG(WARNING) + "Failed to register to receive phone service notifications";
		}

		//  Register for Cellular System Available
		pCallbackData[RE_WAN_CELL_SYSTEM_AVAILABLE].pObject = this;
		pCallbackData[RE_WAN_CELL_SYSTEM_AVAILABLE].dwCallbackType = RE_WAN_CELL_SYSTEM_AVAILABLE;
		hr = lpfn_Registry_NotifyCallback(
			SN_CELLSYSTEMAVAILABLE_ROOT,
			SN_CELLSYSTEMAVAILABLE_PATH,
			SN_CELLSYSTEMAVAILABLE_VALUE,
			PhoneStatusCallback,
			(DWORD)&pCallbackData[RE_WAN_CELL_SYSTEM_AVAILABLE],
			NULL,
			&hRegistryNotifications[RE_WAN_CELL_SYSTEM_AVAILABLE]);
		if (hr != S_OK)
		{
			LOG(WARNING) + "Failed to register to receive WAN signal strength notifications";
		}


		//  Register for Cellular System Connected
		pCallbackData[RE_WAN_CELL_SYSTEM_CONNECTED].pObject = this;
		pCallbackData[RE_WAN_CELL_SYSTEM_CONNECTED].dwCallbackType = RE_WAN_CELL_SYSTEM_CONNECTED;
		hr = lpfn_Registry_NotifyCallback(
			SN_CELLSYSTEMCONNECTED_ROOT,
			SN_CELLSYSTEMCONNECTED_PATH,
			SN_CELLSYSTEMCONNECTED_VALUE,
			PhoneStatusCallback,
			(DWORD)&pCallbackData[RE_WAN_CELL_SYSTEM_CONNECTED],
			NULL,
			&hRegistryNotifications[RE_WAN_CELL_SYSTEM_CONNECTED]);
		if (hr != S_OK)
		{
			LOG(WARNING) + "Failed to register to receive WAN signal strength notifications";
		}
	}
	return (hr == S_OK);
}


BOOL CWAN::Connect(LPCTSTR szConnectionDestination, BOOL bUserRequest)
{
	BOOL bReturnValue = FALSE;
	if (bUserRequest)
	{
		if (m_szLastRequestedConnectionDestionation)
			delete[] m_szLastRequestedConnectionDestionation;
		m_szLastRequestedConnectionDestionation = new WCHAR[wcslen(szConnectionDestination) + 1];
		wcscpy(m_szLastRequestedConnectionDestionation, szConnectionDestination);
		//  User has attempted to connect, first ensure the device is able to
		//  make the connection
		//  Update the values from the registry first
		if (!RetrievePrerequisitesFromRegistry())
		{
			LOG(WARNING) + "Did not attempt to connect, device has no SIM";
			return FALSE;
		}
		if ((!m_bPhoneHasSim) && (!m_bCDMADevice))
		{
			LOG(WARNING) + "Did not attempt to connect, device has no SIM";
			return FALSE;
		}
		if (!m_bRadioPresent)
		{
			LOG(WARNING) + "Did not attempt to connect, device has no Radio";
			return FALSE;
		}
		if (!m_bRadioOn)
		{
			LOG(WARNING) + "Did not attempt to connect, Radio is not turned on";
			return FALSE;
		}
		if (!m_bRadioPresent)
		{
			LOG(WARNING) + "Did not attempt to connect, device has no Radio";
			return FALSE;
		}
		if (!m_bPhoneService)
		{
			LOG(WARNING) + "Did not attempt to connect, There is no phone service";
			return FALSE;
		}
	}

	if (m_hConnection)
	{
		//  Consider that we may already be connected to a network, this will be the case
		//  if the user specified to connect as a default meta tag so it is not serious.
		LOG(INFO) + "Instructed to Connect to WAN whilst already connected";
		return TRUE;
	}

	//  All the registry keys indicate we can connect, attempt to find the specified GUID
    int idx=0;
    CONNMGR_DESTINATION_INFO DestInfo = {0};
	GUID userSpecifiedGUID = {0};
	if (szConnectionDestination && (wcslen(szConnectionDestination) >= wcslen(L"Internet")) &&
		wcsicmp(szConnectionDestination, L"internet") == 0)
	{
		//  Connect to default Internet GUID
		userSpecifiedGUID = IID_DestNetInternet;
		LOG(INFO) + "Connecting to default Internet Destination";
	}
	else if (szConnectionDestination != NULL)
	{
		//  User has specified a network to connect to, find it.
		while( true )
		{
			HRESULT hResult = lpfn_ConMgr_EnumDestinations( idx, &DestInfo );
			if ( hResult != S_OK )
				break;

			if (wcsicmp(DestInfo.szDescription, szConnectionDestination) == 0 && wcslen(szConnectionDestination) < 200)
			{
				//  We have found the user specified network
				userSpecifiedGUID = DestInfo.guid;
				WCHAR wLog[256];
				wsprintf(wLog, L"Connecting to Destination %s", DestInfo.szDescription);
				LOG(INFO) + wLog;
				break;
			}
			idx++;
		}
	}

	if (!userSpecifiedGUID.Data1)
	{
		WCHAR wLog[256];
		wsprintf(wLog, L"Unable to find specified connection destination to connect to (%s)", 
			szConnectionDestination);
		LOG(INFO) + wLog;
	}
	else
	{
		CONNMGR_CONNECTIONINFO ConnInfo = {0};
		ConnInfo.cbSize      = sizeof(ConnInfo);
		ConnInfo.dwParams    = CONNMGR_PARAM_GUIDDESTNET;
		ConnInfo.dwPriority  = CONNMGR_PRIORITY_HIPRIBKGND;
		ConnInfo.guidDestNet = userSpecifiedGUID;
		ConnInfo.hWnd        = m_hParentWnd;
		ConnInfo.uMsg        = WM_USER_CONNECTION_MANGER_STATUS;
		ConnInfo.lParam      = (LPARAM)NULL;
		
		HRESULT hResult = lpfn_ConMgr_EstablishConnection( &ConnInfo, &m_hConnection );
		if ( hResult == S_OK )
		{
			//  Connected to WAN
			LOG(INFO) + "Connected to WAN";
			bReturnValue = TRUE;
		}
		else
		{
			//  Connection Manager failed
			m_hConnection = NULL;
			WCHAR wLog[128];
			wsprintf(wLog, L"Failed to connect to WAN, Connection Manager error code: %l", hResult);
			LOG(WARNING) + wLog;
		}
	}

	ConnectionManagerStatusUpdate();
	return bReturnValue;
}

BOOL CWAN::Disconnect(BOOL bUserRequest)
{
	//  Disconnect from the current connection
	if (m_hConnection)
	{
		if (S_OK == lpfn_ConMgr_ReleaseConnection(m_hConnection, 0))
		{
			m_hConnection = NULL;
			//  Stop the connection manager watchdog
			if (m_hConnectionManagerWatchdog && bUserRequest)
			{
				SetEvent(m_hStopConnectionManagerWatchdog);
				if (WaitForSingleObject(m_hConnectionManagerWatchdog, 1000) == WAIT_TIMEOUT)
				{
					//  Terminate the thread if it did not shut down within a second
					TerminateThread(m_hConnectionManagerWatchdog, 0);
				}
				m_currentConnectionManagerState = RE_NOT_CONNECTED;
				CloseHandle(m_hConnectionManagerWatchdog);
				m_hConnectionManagerWatchdog = NULL;
				ConnectionManagerStatusUpdate();
			}
		}
		else
		{
			LOG(WARNING) + "Failed to disconnect WAN, error releasing Connection Manager Connection";
		}
	}
	else
	{
		LOG(WARNING) + "Attempted to disconnect WAN but there is no WAN connection";
	}

	return TRUE;
}


BOOL CWAN::SetWanCallback(rho::apiGenerator::CMethodResult pCallback)
{
	m_pStatusCallback = pCallback;
	ConnectionManagerStatusUpdate();
	return TRUE;
}


void CWAN::ReportWANStatus()
{
	//  Only report the signal status to the user if there is a registered callback
	if (m_pStatusCallback.hasCallback())
	{
		//Make sure Values are up to date
		Sleep(200);
		if (lpfn_Registry_GetDWORD) lpfn_Registry_GetDWORD(SN_CELLSYSTEMCONNECTED_ROOT,SN_CELLSYSTEMCONNECTED_PATH,SN_CELLSYSTEMCONNECTED_VALUE,&m_dwCellSystemConnected);
		if (lpfn_Registry_GetDWORD) lpfn_Registry_GetDWORD(SN_CELLSYSTEMAVAILABLE_ROOT,SN_CELLSYSTEMAVAILABLE_PATH,SN_CELLSYSTEMAVAILABLE_VALUE,&m_dwCellSystemAvailable);

		GetCellularStatus(m_szCellularSystemAvailable, m_szCellularSystemConnected);
		WCHAR szSignalStrength[30]; 
		wsprintf(szSignalStrength, L"%d", m_dwSignal, m_dwCellSystemAvailable, m_dwCellSystemConnected);
		rho::Hashtable<rho::String, rho::String> callbackData;
		if (m_bPhoneService)
			callbackData.put("phoneSignalStrength", convertToStringA(szSignalStrength));
		else
			callbackData.put("phoneSignalStrength", "0");
		callbackData.put("networkOperator", convertToStringA(m_szOperatorName));
		callbackData.put("connectionTypeAvailable", convertToStringA(m_szCellularSystemAvailable));
		callbackData.put("connectionTypeConnected", convertToStringA(m_szCellularSystemConnected));
		callbackData.put("connectionManagerMessage", convertToStringA(m_szConnectionManagerStatus));
		m_pStatusCallback.set(callbackData);
	}
}


BOOL CWAN::RetrievePrerequisitesFromRegistry()
{
	//  Get Initial Values from Registry
	BOOL bReturnValue = TRUE;
	DWORD dwStatus;
	if (lpfn_Registry_GetString && lpfn_Registry_GetDWORD)
	{

		//  Read whether or not the phone has a radio installed
		if (S_OK != lpfn_Registry_GetDWORD(SN_PHONERADIOPRESENT_ROOT,SN_PHONERADIOPRESENT_PATH,SN_PHONERADIOPRESENT_VALUE, &dwStatus))
		{
			bReturnValue = FALSE;
			LOG(WARNING) + "Failed to retrieve whether the phone radio is present, connections via WAN are unavailable";
		}
		else
			m_bRadioPresent = (dwStatus & SN_PHONERADIOPRESENT_BITMASK) > 0;


		//  Read whether or not the phone's radio is turned off
		if (S_OK != lpfn_Registry_GetDWORD(SN_PHONERADIOOFF_ROOT,SN_PHONERADIOOFF_PATH,SN_PHONERADIOOFF_VALUE, &dwStatus))
		{
			bReturnValue = FALSE;
			LOG(WARNING) + "Failed to retrieve whether the phone radio is turned on, connections via WAN are unavailable";
		}
			m_bRadioOn = !(dwStatus & SN_PHONERADIOOFF_BITMASK);

		//  Read whether or not the phone reports no service
		if (S_OK != lpfn_Registry_GetDWORD(SN_PHONENOSERVICE_ROOT,SN_PHONENOSERVICE_PATH,SN_PHONENOSERVICE_VALUE, &dwStatus))
		{
			bReturnValue = FALSE;
			LOG(WARNING) + "Failed to retrieve the state of the phone service, connections via WAN are unavailable";
		}
		else
			m_bPhoneService = !(dwStatus & SN_PHONENOSERVICE_BITMASK);

		//  Read the current signal strength
		if (S_OK != lpfn_Registry_GetDWORD(SN_PHONESIGNALSTRENGTH_ROOT,SN_PHONESIGNALSTRENGTH_PATH,SN_PHONESIGNALSTRENGTH_VALUE,&m_dwSignal))
		{
			bReturnValue = FALSE;
			LOG(WARNING) + "Failed to retrieve phone signal strength, connections via WAN are unavailable";
		}

		//  Read the phone operator name
		if (S_OK != lpfn_Registry_GetString(SN_PHONEOPERATORNAME_ROOT, SN_PHONEOPERATORNAME_PATH, SN_PHONEOPERATORNAME_VALUE, m_szOperatorName, 64))
		{
			//  Failed to read operator name from registry
			LOG(WARNING) + "Failed to retrieve operator name, operator will not be reported in status event";
			wcscpy(m_szOperatorName, L"");
		}

		//  Read the current Cell Available Status
		if (S_OK != lpfn_Registry_GetDWORD(SN_CELLSYSTEMAVAILABLE_ROOT,SN_CELLSYSTEMAVAILABLE_PATH, SN_CELLSYSTEMAVAILABLE_VALUE, &m_dwCellSystemAvailable))
		{
			bReturnValue = FALSE;
			LOG(WARNING) + "Failed to retrieve Cell system Available, connections via WAN are unavailable";
		}

		//  Read the current Cell Connected Status
		if (S_OK != lpfn_Registry_GetDWORD(SN_CELLSYSTEMCONNECTED_ROOT,SN_CELLSYSTEMCONNECTED_PATH, SN_CELLSYSTEMCONNECTED_VALUE, &m_dwCellSystemConnected))
		{
			bReturnValue = FALSE;
			LOG(WARNING) + "Failed to retrieve Cell system Connected, connections via WAN are unavailable";
		}

		//  Read whether or not the phone has a SIM installed
		if (S_OK != lpfn_Registry_GetDWORD(SN_PHONENOSIM_ROOT,SN_PHONENOSIM_PATH,SN_PHONENOSIM_VALUE,&dwStatus))
		{
			bReturnValue = FALSE;
			LOG(WARNING) + "Failed to retrieve phone SIM status, connections via WAN are unavailable";
		}
		else
			m_bPhoneHasSim = !(dwStatus & SN_PHONENOSIM_BITMASK);

	
		//Update the string values
		GetCellularStatus(m_szCellularSystemAvailable, m_szCellularSystemConnected);

		//If we have Signal and Service but no SIM card, Then assume CDMA Device.
		if ( (!m_bPhoneHasSim) && (m_dwSignal) && (m_dwCellSystemAvailable))
		{
			m_bCDMADevice = true;
		}

	}
	return bReturnValue;
}


//  Ensure szCellSystemAvailable and szCellSystemConnected have sufficient space
BOOL CWAN::GetCellularStatus(TCHAR* szCellSystemAvailable, TCHAR* szCellSystemConnected)
{
	if (m_dwCellSystemAvailable == 0)
		wcscpy(szCellSystemAvailable, L"Unavailable");
	else if ((m_dwCellSystemAvailable & SN_CELLSYSTEMCONNECTED_GPRS_BITMASK) != 0)
		wcscpy(szCellSystemAvailable, L"GPRS");
	else if ((m_dwCellSystemAvailable & SN_CELLSYSTEMCONNECTED_1XRTT_BITMASK) != 0)
		wcscpy(szCellSystemAvailable, L"1XRTT");
	else if ((m_dwCellSystemAvailable & SN_CELLSYSTEMCONNECTED_1XEVDO_BITMASK) != 0)
		wcscpy(szCellSystemAvailable, L"EVDO");
	else if ((m_dwCellSystemAvailable & SN_CELLSYSTEMCONNECTED_EDGE_BITMASK) != 0)
		wcscpy(szCellSystemAvailable, L"EDGE");
	else if ((m_dwCellSystemAvailable & SN_CELLSYSTEMCONNECTED_UMTS_BITMASK) != 0)
		wcscpy(szCellSystemAvailable, L"UMTS");
	else if ((m_dwCellSystemAvailable & SN_CELLSYSTEMCONNECTED_EVDV_BITMASK) != 0)
		wcscpy(szCellSystemAvailable, L"EVDV");
	else if ((m_dwCellSystemAvailable & SN_CELLSYSTEMCONNECTED_HSDPA_BITMASK) != 0)
		wcscpy(szCellSystemAvailable, L"HSDPA");
	else if ((m_dwCellSystemAvailable & SN_CELLSYSTEMCONNECTED_CSD_BITMASK) != 0)
		wcscpy(szCellSystemAvailable, L"CSD");

	if (m_dwCellSystemConnected == 0)
		wcscpy(szCellSystemConnected, L"Not Connected");
	else if ((m_dwCellSystemConnected & SN_CELLSYSTEMCONNECTED_GPRS_BITMASK) != 0)
		wcscpy(szCellSystemConnected, L"GPRS");
	else if ((m_dwCellSystemConnected & SN_CELLSYSTEMCONNECTED_1XRTT_BITMASK) != 0)
		wcscpy(szCellSystemConnected, L"1XRTT");
	else if ((m_dwCellSystemConnected & SN_CELLSYSTEMCONNECTED_1XEVDO_BITMASK) != 0)
		wcscpy(szCellSystemConnected, L"EVDO");
	else if ((m_dwCellSystemConnected & SN_CELLSYSTEMCONNECTED_EDGE_BITMASK) != 0)
		wcscpy(szCellSystemConnected, L"EDGE");
	else if ((m_dwCellSystemConnected & SN_CELLSYSTEMCONNECTED_UMTS_BITMASK) != 0)
		wcscpy(szCellSystemConnected, L"UMTS");
	else if ((m_dwCellSystemConnected & SN_CELLSYSTEMCONNECTED_EVDV_BITMASK) != 0)
		wcscpy(szCellSystemConnected, L"EVDV");
	else if ((m_dwCellSystemConnected & SN_CELLSYSTEMCONNECTED_HSDPA_BITMASK) != 0)
		wcscpy(szCellSystemConnected, L"HSDPA");
	else if ((m_dwCellSystemConnected & SN_CELLSYSTEMCONNECTED_CSD_BITMASK) != 0)
		wcscpy(szCellSystemConnected, L"CSD");

	return TRUE;
}

void CWAN::ConnectionManagerStatusUpdate()
{
	GetConnectionStatus(m_szConnectionManagerStatus);
	WCHAR wLog[512];
	wsprintf(wLog, L"Connection Manager is reporting Status '%s'", m_szConnectionManagerStatus);
	LOG(INFO) + wLog;
	ReportWANStatus();
}


BOOL CWAN::GetConnectionStatus( TCHAR *szStatusBuffer )
{
	DWORD dwStatus=0;
    if ( m_hConnection == NULL )
        _tcscpy( szStatusBuffer, TEXT("No Connection") );
    else
    {
        lpfn_ConMgr_ConnectionStatus(m_hConnection, &dwStatus );
        _tcscpy( szStatusBuffer, GetConnectionStatusString( dwStatus ) );
		UpdateConnectionManagerState(dwStatus);
    }

    return dwStatus == CONNMGR_STATUS_CONNECTED;
}

void CWAN::UpdateConnectionManagerState(DWORD dwStatus)
{
	switch (m_currentConnectionManagerState)
	{
	case RE_NOT_CONNECTED:
		{
		//  We are not currently connected to any Network
			switch (dwStatus)
			{
				//  Connection Established
				case CONNMGR_STATUS_CONNECTED:
				case CONNMGR_STATUS_SUSPENDED:
					//  Start the Connection Manager watchdog timer if it is not started already
					if (m_hConnectionManagerWatchdog == NULL)
						m_hConnectionManagerWatchdog = CreateThread(NULL, 0,
						(LPTHREAD_START_ROUTINE)ConnectionManagerWatchdog, this, 0, NULL);
					m_currentConnectionManagerState = RE_CONNECTION_UP;
				//  No action for any other Connection Manager Status' in this State
					break;
			}
		}
		break;
	case RE_CONNECTION_UP:
		{
		//  We are currently connected to a Network
			switch (dwStatus)
			{
				//  Connection Dropped and requires reconnection
				case CONNMGR_STATUS_DISCONNECTED :             
				case CONNMGR_STATUS_NOPATHTODESTINATION :      
				case CONNMGR_STATUS_CONNECTIONFAILED :          
				case CONNMGR_STATUS_CONNECTIONCANCELED :        
				case CONNMGR_STATUS_CONNECTIONDISABLED :       
				case CONNMGR_STATUS_CONNECTIONLINKFAILED :      
				case CONNMGR_STATUS_PHONEOFF :                  
				case CONNMGR_STATUS_EXCLUSIVECONFLICT :         
				case CONNMGR_STATUS_NORESOURCES :               
				case CONNMGR_STATUS_AUTHENTICATIONFAILED :     
					m_currentConnectionManagerState = RE_CONNECTION_DOWN_AND_REQUIRES_RESTART;
					break;

			}
		}
		break;
	case RE_CONNECTION_DOWN_AND_REQUIRES_RESTART:
		{
			//  The connection is currently down and is being restarted by the
			//  watchdog timer
			switch (dwStatus)
			{
				//  Connection Established
				case CONNMGR_STATUS_CONNECTED:
				case CONNMGR_STATUS_SUSPENDED:
				//  Waiting for connection
				case CONNMGR_STATUS_WAITINGFORPATH :           
				case CONNMGR_STATUS_WAITINGFORRESOURCE :        
				case CONNMGR_STATUS_WAITINGFORPHONE :           
				case CONNMGR_STATUS_WAITINGFORNETWORK :         
				case CONNMGR_STATUS_WAITINGCONNECTION :        
				case CONNMGR_STATUS_WAITINGCONNECTIONABORT :    
				case CONNMGR_STATUS_WAITINGDISCONNECTION :      
					m_currentConnectionManagerState = RE_CONNECTION_UP;
					break;
			}
		}
		break;
	}
}

const TCHAR *CWAN::GetConnectionStatusString( int iStatus )
{
    switch( iStatus )
    { 
        case CONNMGR_STATUS_UNKNOWN :                   return TEXT("CONNMGR_STATUS_UNKNOWN. The status is unknown"); 
        case CONNMGR_STATUS_CONNECTED :                 return TEXT("The connection is up."); 
        case CONNMGR_STATUS_DISCONNECTED :              return TEXT("The connection has been disconnected."); 
        case CONNMGR_STATUS_WAITINGFORPATH :            return TEXT("A path to the destination exists but is not presently available (for example, the device is out of radio range or is not plugged into its cradle)."); 
        case CONNMGR_STATUS_WAITINGFORRESOURCE :        return TEXT("Another client is using resources that this connection requires."); 
        case CONNMGR_STATUS_WAITINGFORPHONE :           return TEXT("A voice call is in progress and is using resources that this connection requires."); 
        case CONNMGR_STATUS_WAITINGFORNETWORK :         return TEXT("The device is waiting for a task with a higher priority to connect to the network before connecting to the same network. This status value is only returned to clients that specified a priority of CONNMGR_PRIORITY_LOWBKGND when requesting a connection."); 
        case CONNMGR_STATUS_NOPATHTODESTINATION :       return TEXT("No path to the destination could be found."); 
        case CONNMGR_STATUS_CONNECTIONFAILED :          return TEXT("The connection failed and cannot be reestablished."); 
        case CONNMGR_STATUS_CONNECTIONCANCELED :        return TEXT("The user aborted the connection."); 
        case CONNMGR_STATUS_CONNECTIONDISABLED :        return TEXT("The connection can be made, but the connection itself is disabled. This value is only returned to clients that set the bDisabled value in the CONNMGR_CONNECTIONINFO structure."); 
        case CONNMGR_STATUS_WAITINGCONNECTION :         return TEXT("The device is attempting to connect."); 
        case CONNMGR_STATUS_WAITINGCONNECTIONABORT :    return TEXT("The device is aborting the connection attempt."); 
        case CONNMGR_STATUS_WAITINGDISCONNECTION :      return TEXT("The connection is being brought down."); 
        case CONNMGR_STATUS_SUSPENDED:                  return TEXT("The connection has been established, but has been suspended.");
        case CONNMGR_STATUS_PHONEOFF :                  return TEXT("The phone has been turned off.");
        case CONNMGR_STATUS_EXCLUSIVECONFLICT :         return TEXT("The connection could not be established because it would multi-home an exclusive connection.");
        case CONNMGR_STATUS_NORESOURCES :               return TEXT("The Connection Manager failed to allocate resources to make the connection.");
        case CONNMGR_STATUS_CONNECTIONLINKFAILED :      return TEXT("The connection link was prematurely disconnected.");
        case CONNMGR_STATUS_AUTHENTICATIONFAILED :      return TEXT("The user is could not be authenticated.");
    }

    return TEXT("Unknown connection manager status : %d");
}

BOOL CWAN::LoadConnectionManager()
{
	bool bReturnValue = FALSE;
	m_hConnManDLL = LoadLibrary(L"cellcore.dll");
	if (!m_hConnManDLL)
	{
		//  Error loading CellCore.dll (used for Connection Manager)
		LOG(INFO) + "Failed to load CellCore.dll, WAN connectivity will not be available";
	}
	else
	{
		lpfn_ConMgr_EstablishConnection = 
			(LPFN_CONMGR_ESTABLISHCONNECTION_T)GetProcAddress(m_hConnManDLL, _T("ConnMgrEstablishConnection"));
		lpfn_ConMgr_EnumDestinations = 
			(LPFN_CONMGR_ENUMDESTINATIONS_T)GetProcAddress(m_hConnManDLL, _T("ConnMgrEnumDestinations"));
		lpfn_ConMgr_ReleaseConnection = 
			(LPFN_CONMGR_RELEASECONNECTION_T)GetProcAddress(m_hConnManDLL, _T("ConnMgrReleaseConnection"));
		lpfn_ConMgr_ConnectionStatus = 
			(LPFN_CONMGR_CONNECTIONSTATUS_T)GetProcAddress(m_hConnManDLL, _T("ConnMgrConnectionStatus"));

		if (!lpfn_ConMgr_EstablishConnection)
		{
			LOG(ERROR) + "Unable to load ConnMgrEstablishConnection";
			bReturnValue = FALSE;
		}
		else if (!lpfn_ConMgr_EnumDestinations)
		{
			LOG(ERROR) + "Unable to load ConnMgrEnumDestinations";
		}
		else if (!lpfn_ConMgr_ReleaseConnection)
		{
			LOG(ERROR) + "Unable to load ConnMgrReleaseConnection";
		}
		else if (!lpfn_ConMgr_ConnectionStatus)
		{
			LOG(ERROR) + "Unable to load ConnMgrConnectionStatus";
		}
		else
			bReturnValue = TRUE;
	}
	return bReturnValue;
}

BOOL CWAN::LoadAYGShell()
{
	bool bReturnValue = FALSE;
	m_hAygShellDLL = LoadLibrary(L"aygshell.dll");
	if (!m_hAygShellDLL)
	{
		//  Error loading AygShell.dll (used for Retrieving values from the Registry)
		LOG(INFO) + "Failed to load AygShell.dll, WAN status event will not be available";
	}
	else
	{
		lpfn_Registry_CloseNotification = 
			(LPFN_REGISTRY_CLOSENOTIFICATION_T)GetProcAddress(m_hAygShellDLL, _T("RegistryCloseNotification"));
		lpfn_Registry_NotifyCallback = 
			(LPFN_REGISTRY_NOTIFYCALLBACK_T)GetProcAddress(m_hAygShellDLL, _T("RegistryNotifyCallback"));
		lpfn_Registry_GetString = 
			(LPFN_REGISTRY_GETSTRING_T)GetProcAddress(m_hAygShellDLL, _T("RegistryGetString"));
		lpfn_Registry_GetDWORD = 
			(LPFN_REGISTRY_GETDWORD_T)GetProcAddress(m_hAygShellDLL, _T("RegistryGetDWORD"));

		if (!lpfn_Registry_CloseNotification)
		{
			LOG(ERROR) + "Unable to load RegistryCloseNotification, WAN Status event will be unavailable";
			bReturnValue = FALSE;
		}
		else if (!lpfn_Registry_CloseNotification)
		{
			LOG(ERROR) + "Unable to load RegistryNotifyCallback, WAN Status event will be unavailable";
		}
		else if (!lpfn_Registry_GetString)
		{
			LOG(ERROR) + "Unable to load RegistryGetString, WAN Status event will be unavailable";
		}
		else if (!lpfn_Registry_GetDWORD)
		{
			LOG(ERROR) + "Unable to load RegistryGetDWORD, WAN Status event will be unavailable";
		}
		else
			bReturnValue = TRUE;
	}
	return bReturnValue;	
}

//////////////////////////
//  Private Threads		//
//////////////////////////

DWORD CWAN::ConnectionManagerWatchdog(LPVOID lpParameter)
{
	CWAN* pWAN = (CWAN*)lpParameter;
	bool exitThread = false;
	DWORD dwEvent;
	HANDLE hWaitHandles[1];
	hWaitHandles[0] = pWAN->m_hStopConnectionManagerWatchdog;
	DEBUGMSG(TRUE, (L"Connection Manager Watchdog Timer Starting\n"));

	while (true)
	{
		//  Wait for an exit event (indicating stop the thread) or timeout
		dwEvent = WaitForMultipleObjects(
			1,
			hWaitHandles,
			FALSE,
			CONNECTION_MANAGER_WATCHDOG_TIMER_TIMEOUT);
		switch (dwEvent)
		{
		case WAIT_OBJECT_0:
			{
				goto _exitThread;
			}
		case WAIT_TIMEOUT:
			{
				//  If we are currently connected to a network we would expect the Connection Manager 
				//  to be connected, if it has become disconnected then attempt to reconnect.  Only do this
				//  if there is a phone service available
				DEBUGMSG(TRUE, (L"Connection Manager Watchdog Timer Timeout\n"));
				if (pWAN->m_currentConnectionManagerState == RE_CONNECTION_DOWN_AND_REQUIRES_RESTART &&
					pWAN->m_hConnection && pWAN->m_szLastRequestedConnectionDestionation && 
					pWAN->m_bPhoneService)
				{
					LOG(WARNING) + "Connection Manager has lost connection, attempting to re-establish";
					pWAN->Disconnect(false);
					pWAN->Connect(pWAN->m_szLastRequestedConnectionDestionation, false);
				}
			}
		}  //  End Switch
	}	//  End While !exitThread
_exitThread:
	DEBUGMSG(TRUE, (L"Connection Manager Watchdog Thread Exiting\n"));
	return 0;
}
