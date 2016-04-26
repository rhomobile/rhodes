#include "Scanner.h"
#include <pm.h>
//	TODO: This flag should not need to be global in true multiple instance.

//Reading LowBatteryScan value from config.xml
extern "C" bool rho_wmimpl_sharedconfig_getboolvalue(const wchar_t* szName);
extern "C" const wchar_t* rho_wmimpl_sharedconfig_getvalue(const wchar_t* szName);

#pragma region public_methods

CScanner::CScanner(int iInstanceID)
{
	//  Initialise Variables (no memory allocation in constructor
	InitializeCriticalSection(&m_scanner_prop_cs);
	m_hiddenWindow				= NULL;
	m_iInstanceID				= iInstanceID;
	//  Find a unique event name for this Scanner instance (to be used in Named Events)
	m_iInstanceID2				= iInstanceID;
	WCHAR szTempName[25];
	wsprintf(szTempName, L"NeonTempNam4%i", m_iInstanceID2);
	m_hUniqueEvent = CreateEvent(NULL, TRUE, FALSE, szTempName);
	while(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		m_iInstanceID2++;
		wsprintf(szTempName, L"NeonTempNam4%i", m_iInstanceID2);
		CloseHandle(m_hUniqueEvent);
		m_hUniqueEvent = CreateEvent(NULL, TRUE, FALSE, szTempName);
	}
	if (m_iInstanceID >= 500)
		m_iInstanceID -= 500;
	m_hScnAPIDLL				= NULL;
	InitialiseEMDKFunctionPointers();
	m_bScannerEnabled			= false;
	m_bScannerBeingEnabled		= false;
	m_bTriggerConnected			= true;
	m_hMetaMsgQRead				= NULL;
	m_hMetaMsgQWrite			= NULL;
//	m_hRSMMsgQRead				= NULL;
//	m_hRSMMsgQWrite				= NULL;
	m_hTagsOnCurrentPageQRead	= NULL;
	m_hTagsOnCurrentPageQWrite	= NULL;
	m_bScannerOpen				= false;
	m_szUserSpecifiedScanner	= NULL;
	m_szCurrentlyOpenScanner	= NULL;
	m_bUsingBluetoothScanner	= false;
	m_hScanner					= NULL;
	m_ScannerState				= NO_BT_CONNECTION;
	m_lpScanBuffer				= NULL;
	m_hReadComplete				= NULL;
	m_hEnableForTakeComplete	= NULL;
	m_hUniqueEvent				= NULL;
	m_dwRequestID				= -1;
	g_bShuttingDown				= false;
	//  Configuration Settings
	m_dwDecodeVolume			= DEFAULT_DECODE_VOLUME;
	m_dwDecodeFrequency			= DEFAULT_GOOD_DECODE_FREQUENCY;
	m_dwInvalidDecodeFrequency	= DEFAULT_INVALID_DECODE_FREQUENCY;
	m_dwDecodeDuration			= DEFAULT_DECODE_DURATION;
	m_szScanDecodeWav			= NULL;
	m_szScanInvalidWav			= NULL;
	//Developer - Abhineet Agarwal
	//Reading value from config.xml
   	m_bLowBatteryScan = rho_wmimpl_sharedconfig_getboolvalue(L"System\\LowBatteryScan");
    m_bLegacyNavBehaviour = !rho_wmimpl_sharedconfig_getboolvalue(L"Scanner\\DisableScannerDuringNavigation");
	m_bCurrentlyNavigating		= false;
	m_bResetScanBuffer			= false;
	m_bScannedDuringNavigate	= false;
	//  Scanner Properties
	m_bAutoEnter				= false;
	m_bAutoTab					= false;
	m_iDataBufferSize			= DEFAULT_SCAN_BUFFER_SIZE;
	m_bufferFormat				= BUFFER_TEXT;
	m_currentScanType			= SCAN_TYPE_BACKGROUND;
	m_iDisplayBTPairBarcode		= BT_PAIRING_BARCODE_INITIAL;
	m_dwEnableTimeout			= 0;
	//  RSM
	m_rsmSupportedAttributes	= NULL;
	m_iCountOfMarkedRSMAttributes = 0;
	//  Viewfinder
	m_iViewfinderRequestedLeft	= -1;
	m_iViewfinderRequestedRight	= -1;
	m_iViewfinderRequestedTop	= -1;
	m_iViewfinderRequestedBottom= -1;
	//  Low Battery Scan
	m_hPowerNotifications		= NULL;
	m_hLowBatteryScanMessageQueueRead	= NULL;
	m_hLowBatteryScanMessageQueueWrite	= NULL;
}

CScanner::~CScanner()
{
    LOG(TRACE) + L"Destroy scanner instance.";
	//	TODO: This flag should not need to be global in true multiple instance.
//	if (m_iInstanceID >= 0)
		g_bShuttingDown = true;
	//Modified by Developer - Abhineet Agarwal
	//Terminating thread i.e. terminating low battery check, if running.....
	if (!m_bLowBatteryScan){		
		POWER_BROADCAST PwrBcast[64];
		WriteMsgQueue(m_hLowBatteryScanMessageQueueWrite, &PwrBcast, 64, INFINITE, 0);
		StopPowerNotifications(m_hPowerNotifications);
		CloseMsgQueue(m_hLowBatteryScanMessageQueueRead);
		CloseMsgQueue(m_hLowBatteryScanMessageQueueWrite);
	}

	//  Shut down the thread processing RSM Tags for retrieval
	ATTRIB_NUMBER shutDownRSM = (ATTRIB_NUMBER)ATTR_END;
//	WriteMsgQueue(m_hRSMMsgQWrite, 
//						&shutDownRSM, 
//						sizeof(shutDownRSM), 
//						INFINITE, 0);

	m_pDecodeEvent				= NULL;
	m_pBluetoothStatusEvent		= NULL;
	m_pRSMEvent					= NULL;
	m_pDeviceCapabilities		= NULL;
	m_pTakeCallback				= NULL;
	if (m_szScanDecodeWav)
		delete[] m_szScanDecodeWav;
	if (m_szScanInvalidWav)
		delete[] m_szScanInvalidWav;

	CloseScanner();
    LOG(TRACE) + L"Scanner Closed.";

	if (!UnloadEMDK())
		return;

    LOG(TRACE) + L"EMDK unloaded";

	if (m_rsmSupportedAttributes)
	{
		delete[] m_rsmSupportedAttributes;
		m_rsmSupportedAttributes = NULL;
	}
	
	//  Close RSM Queues
//	if (m_hRSMMsgQWrite)
//	{
//		CloseMsgQueue(m_hRSMMsgQWrite);
//		m_hRSMMsgQWrite = NULL;
//	}
//	if (m_hRSMMsgQRead)
//	{
//		CloseMsgQueue(m_hRSMMsgQRead);
//		m_hRSMMsgQRead = NULL;
//	}

	//  Close Meta Tag Queues
	if (m_hMetaMsgQWrite)
	{
		CloseMsgQueue (m_hMetaMsgQWrite);
		m_hMetaMsgQWrite = NULL;
	}
	if (m_hMetaMsgQRead)
	{
		CloseMsgQueue (m_hMetaMsgQRead);
		m_hMetaMsgQRead = NULL;
	}

	//  Close the Tags on current page queues
	if (m_hTagsOnCurrentPageQWrite)
	{
		CloseMsgQueue(m_hTagsOnCurrentPageQWrite);
		m_hTagsOnCurrentPageQWrite = NULL;
	}
	if (m_hTagsOnCurrentPageQRead)
	{
		CloseMsgQueue(m_hTagsOnCurrentPageQRead);
		m_hTagsOnCurrentPageQRead = NULL;
	}

	delete m_decodeNotifier;
	delete m_keyInjector;
	CloseHandle(m_hReadComplete);
	CloseHandle(m_hEnableForTakeComplete);
	//  Free the memory into which the scanned data was stored
	delete[] m_lastScannedData.szData;
	CloseHandle(m_hUniqueEvent);

    //LOG(TRACE) + L"Cleanup done.";

}

BOOL CScanner::Initialise(bool bLaunchingAppHasFocus, const WCHAR* szScannerID)
{
	m_bApplicationActive = bLaunchingAppHasFocus;
	//  For RE4 we will know the Scanner ID when we launch
	if (szScannerID)
	{
		m_szUserSpecifiedScanner = new WCHAR[wcslen(szScannerID) + 1];
		wcscpy(m_szUserSpecifiedScanner, szScannerID);
	}
	//  Load Scanner DLL
	BOOL bReturnValue = LoadEMDK();
	if (!bReturnValue)
	{
		LOG(ERROR) + L"Unable to load Scanner Driver, quitting scanner initialisation";
	}
	else 
	{
		//  Populate Internal meta tag structure (to make it faster to parse tags)
		InitialiseMETATagMapForSet();
		InitialiseMETATagMapForGet();
		InitialiseDecoderStringMap();

		//  Initialise Meta Tag Message queues (to handle meta tags when scanner is not enabled)
		MSGQUEUEOPTIONS metaMsgQOpt;
		metaMsgQOpt.dwSize = sizeof(metaMsgQOpt);
		metaMsgQOpt.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		metaMsgQOpt.dwMaxMessages = 0;
		metaMsgQOpt.cbMaxMessage = sizeof(MetaTagQEntry);
		metaMsgQOpt.bReadAccess = TRUE;
		WCHAR metaMsgQName[16];
		wsprintf(metaMsgQName, L"MetaMsgQ%i", m_iInstanceID2);
		m_hMetaMsgQRead = CreateMsgQueue(metaMsgQName, &metaMsgQOpt);
		metaMsgQOpt.bReadAccess = FALSE;
		m_hMetaMsgQWrite = CreateMsgQueue(metaMsgQName, &metaMsgQOpt);

		//  Initialise RSM Message Queues for marking RSM values for retrieval
//		MSGQUEUEOPTIONS rsmMsgQOpt;
//		rsmMsgQOpt.dwSize = sizeof(rsmMsgQOpt);
//		rsmMsgQOpt.dwFlags = MSGQUEUE_ALLOW_BROKEN;
//		rsmMsgQOpt.dwMaxMessages = 0;
//		rsmMsgQOpt.cbMaxMessage = sizeof(ATTRIB_NUMBER);
//		rsmMsgQOpt.bReadAccess = TRUE;
//		WCHAR rsmMsgQName[16];
//		wsprintf(rsmMsgQName, L"rsmMsgQ%i", m_iInstanceID2);
//		m_hRSMMsgQRead = CreateMsgQueue(rsmMsgQName, &rsmMsgQOpt);
//		rsmMsgQOpt.bReadAccess = FALSE;
//		m_hRSMMsgQWrite = CreateMsgQueue(rsmMsgQName, &rsmMsgQOpt);

		//  Initialise the Tags on current page Message Queues, holding the tags parsed on
		//  the current page
		MSGQUEUEOPTIONS tagsOnPageMsgQOpt;
		tagsOnPageMsgQOpt.dwSize = sizeof(tagsOnPageMsgQOpt);
		tagsOnPageMsgQOpt.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		//  If you don't navigate from the page or minimize the app the queue of scanner requsts
		//            grows without limit.  Apply a limit here.  
		tagsOnPageMsgQOpt.dwMaxMessages = 250;
		tagsOnPageMsgQOpt.cbMaxMessage = sizeof(MetaTagQEntry);
		tagsOnPageMsgQOpt.bReadAccess = TRUE;
		WCHAR tagsOnPageMsgQName[16];
		wsprintf(tagsOnPageMsgQName, L"PageMsgQ%i", m_iInstanceID2);
		m_hTagsOnCurrentPageQRead = CreateMsgQueue(tagsOnPageMsgQName, &tagsOnPageMsgQOpt);
		tagsOnPageMsgQOpt.bReadAccess = FALSE;
		m_hTagsOnCurrentPageQWrite = CreateMsgQueue(tagsOnPageMsgQName, &tagsOnPageMsgQOpt);
	
		m_keyInjector = new CKeyInjector();
		m_decodeNotifier = new CDecodeNotifier();
		LPCTSTR pScanDecodeWavValue;
		pScanDecodeWavValue = rho_wmimpl_sharedconfig_getvalue(L"Sound\\ScanDecodeWav");
		if (pScanDecodeWavValue)
		{
			if (m_szScanDecodeWav){
				delete[] m_szScanDecodeWav;
				m_szScanDecodeWav = NULL;
			}
			m_szScanDecodeWav = new WCHAR[wcslen(pScanDecodeWavValue) + 1];
			wcscpy(m_szScanDecodeWav, pScanDecodeWavValue);
		}
		
		m_decodeNotifier->Initialise(m_dwDecodeVolume, m_dwDecodeFrequency, 
			m_dwInvalidDecodeFrequency, m_dwDecodeDuration, m_szScanDecodeWav,
			m_szScanInvalidWav);

		//  Initialise RSM
		//CloseHandle(CreateThread(NULL, 0, &ProcessRSMGetQueue, (void*)this, 0, NULL));

		//  Other Initialisation
		//  Named event will need to be instance specific in multi instance environment
		WCHAR szReadEventName[25];
		wsprintf(szReadEventName, L"NeonReadScanEven4%i", m_iInstanceID2);
		m_hReadComplete = CreateEvent(NULL, TRUE, FALSE, szReadEventName);
		WCHAR szTakeEventName[25];
		wsprintf(szTakeEventName, L"NeonTakeEven4%i", m_iInstanceID2);
		m_hEnableForTakeComplete = CreateEvent(NULL, TRUE, FALSE, szTakeEventName);
		//  Allocate the memory in m_lpScanBuffer to retrieve the data
		m_lastScannedData.szData = new WCHAR[m_iDataBufferSize];
		if (!m_lastScannedData.szData)
			LOG(ERROR) + L"Unable to allocate memory to return data buffer";

		//  Register a class for the hidden window to receive Windows Notifications
		WNDCLASS wndclass;
		memset (&wndclass, 0, sizeof wndclass);
		wndclass.lpfnWndProc = HiddenWindowProc;
		wndclass.hInstance = NULL;
		wsprintf(m_szHiddenWindowName, L"RhoElementsScan%i", m_iInstanceID2);
		wndclass.lpszClassName = m_szHiddenWindowName;
		RegisterClass (&wndclass);
	}
	return bReturnValue;
}
/*
//  Existing logging function removed for RE4 to allow Rhodes' existing log macro to work properly
BOOL CScanner::Log(int logSeverity, LPCTSTR pLogComment, 
		   LPCTSTR pFunctionName, DWORD dwLineNumber)
{
	DEBUGMSG(true, (L"Log: %s\n", pLogComment));
	LOG(ERROR) + pLogComment;
	return TRUE;
	//	return m_pModule->Log(logSeverity, pLogComment, pFunctionName, dwLineNumber);
}
*/

BOOL CScanner::BeforeNavigate()
{
	//  Browser is navigating, ignore any scans that occur
	m_bCurrentlyNavigating = true;
	m_bScannedDuringNavigate = false;

	//  Reset the Events to ""
	m_pDecodeEvent				= NULL;
	m_pBluetoothStatusEvent		= NULL;
	m_pRSMEvent					= NULL;
	m_pDeviceCapabilities		= NULL;
	m_pTakeCallback				= NULL;

	if (m_bLegacyNavBehaviour)
	{
		BOOL bSetTrigger = FALSE;
		(lpfn_SCAN_SetSoftTrigger(m_hScanner, &bSetTrigger) == E_SCN_SUCCESS);	
	}
	else
	{
		//  Empty the list of tags on the current page.
		ActionPendingMetaTags(m_hTagsOnCurrentPageQRead, false);
		//  Before we navigate disable the scanner by default
		CloseScanner();
	}
	return TRUE;
}

BOOL CScanner::DocumentComplete()
{
	m_bCurrentlyNavigating = false;

	//  Reopen the enabled Scanner which was closed during navigate 
	//  (will only happen if the user tried to scan during a navigate)
	if (m_bScannedDuringNavigate && m_bScannerEnabled)
	{
		m_bScannedDuringNavigate = false;
		SubmitRead();
	}

	//  Tags may have been queued whilst we were navigating, clear them here
	if (m_bScannerEnabled)
		ActionPendingMetaTags(m_hMetaMsgQRead, true);
	
	return TRUE;
}

BOOL CScanner::ApplicationFocusChange(bool bActivated)
{
	//  An Application focus change is received when this Scanner instance either
	//  goes to the background (bActivated False) or comes to the foreground
	//  (bActivated True).
	
	//  the application focus change was messing up
	//  registering the RSM scanner by scanning the barcode being displayed on the
	//  device.  If we have lost focus as a result of displaying the BT barcode ignore it
	HWND hBtWnd = FindWindow(L"DisplayBarcode", L"Scan barcode");
	if (!hBtWnd)
		hBtWnd = FindWindow(L"Dialog", L"Enter Passcode");
	if (!hBtWnd)
		hBtWnd = FindWindow(L"Dialog", L"Bluetooth");
	if (!hBtWnd)
		hBtWnd = FindWindow(L"Dialog", L"Device Added");
	if (!hBtWnd)
		hBtWnd = FindWindow(L"Dialog", L"Cannot Connect");		
	if (!hBtWnd)
		hBtWnd = FindWindow(L"MotoBTUI", L"MotoBTUI");
	//  EMDPD00153787: These values are for the APD progressbar Popup.  They are obtained from the APD prtlib resource file 
	//  (under source control in the BROWSER ClearCase VOB).  The scanner will now ignore focus changes from the APD
	//  Printer connection dialog.
	if (!hBtWnd)
		hBtWnd = FindWindow(L"Prtdrv", L"Symbol APD V2.19.06");	
	if (hBtWnd)
	{
		//  Scan Barcode window is shown, if it's the foreground window ignore the
		//  focus change.
		if (hBtWnd == GetForegroundWindow())
        {
            //LOG(TRACE) + "Bluetooth UI on top";
			return TRUE;
        }
		else
		{
			//  Some devices uses different application
			//  to show the BT association barcode.  Unfortunately that
			//  popup is unnamed but is always launched from BTUI window:
			if (hBtWnd == GetWindow(GetForegroundWindow(), GW_HWNDNEXT))
            {
                //LOG(TRACE) + "Bluetooth UI on top (MPA3)";
				return TRUE;
            }
		}
	}

	if (bActivated && !m_bApplicationActive)
	{
        //LOG(TRACE) + "Scanner UI activated";
		//  Application has become active.  Parse the tags processed on the current page.
		m_bApplicationActive = true;
		ActionPendingMetaTags(m_hTagsOnCurrentPageQRead, true);
	}
	else if (!bActivated && m_bApplicationActive)
	{
        //LOG(TRACE) + "Scanner UI deactived";
		//  Application has gone into the background.  Disable the Scanner and reset 
		//  it to its default state.
		m_bApplicationActive = false;
		DisableScannerAndResetToDefault();
	}
	else
	{
		WCHAR wLog[MAXLOG];
		wsprintf(wLog, L"Unexpected application activation received by scanner, current state is %u but request was for %u", 
			m_bApplicationActive, bActivated);
		//  Changing this log to trace as it is causing a lot of confusion in the field
		LOG(TRACE) + wLog;
	}
	return TRUE;
}

void CScanner::SetCallback(int iWhichCallback, rho::apiGenerator::CMethodResult pCallback)
{
	if (iWhichCallback == 0)
		m_pDecodeEvent = pCallback;
	else if (iWhichCallback == 1)
		m_pBluetoothStatusEvent = pCallback;
	else if (iWhichCallback == 2)
		m_pRSMEvent = pCallback;
	else if (iWhichCallback == 3)
		m_pDeviceCapabilities = pCallback;
	else if (iWhichCallback == 4)
		m_pTakeCallback = pCallback;
}

BOOL CScanner::SetPropertyOrMethod(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	BOOL bResult = FALSE;
	metaTagsSetIterator = metaTagsSetFunctions.find(szPropertyName);
	if (metaTagsSetIterator != metaTagsSetFunctions.end())
	{
		if (!AddMetaTagToQueue(m_hTagsOnCurrentPageQWrite, szPropertyName, szPropertyValue))
			LOG(WARNING) + L"Number of scanner methods / properties has exceeded the limit which can be re-applied when switching out to another native application (" + szPropertyName + ")";
		if (m_bApplicationActive)
		{
			//  Only process certain meta tags before the Scanner is enabled, all others are 
			//  queued up
			if (!m_bScannerEnabled || m_bCurrentlyNavigating)
			{
				for (int i = 0;;i++)
				{
					if (wAllowedTagsWhilstNotEnabled[i] == NULL)
					{
						//  Our tag was not allowed, queue it up
						return AddMetaTagToQueue(m_hMetaMsgQWrite, szPropertyName, szPropertyValue);
					}
					else if (cmp(wAllowedTagsWhilstNotEnabled[i], szPropertyName))
					{
						//  This tag is allowed
						break;
					}
				}
			}
			bResult = (*this.*(metaTagsSetFunctions[szPropertyName]))(szPropertyName, szPropertyValue);
		}
		else
		{
			//  set bResult to true as the tag will be processed after we become active
			bResult = true;
		}
	}
	else
	{
		//  Meta Tag was not recognised by the Scanner
		WCHAR wLog[MAXLOG];
		wsprintf(wLog, L"Parameter %s not found in Scanner Module", szPropertyName);
		LOG(WARNING) + wLog;
	}
	return bResult;
}

//  Call this to retrieve the value of an EMML Tag
int CScanner::RetrieveProperty(LPCTSTR szParameterName, WCHAR* szParameterValue)
{
	int iResult = -1;
	metaTagsGetIterator = metaTagsGetFunctions.find(szParameterName);
//	if (metaTagsGetFunctions[szParameterName])
	if (metaTagsGetIterator != metaTagsGetFunctions.end())
	{
		//  Only attempt to retrieve the current setting if the Scanner is enabled
		if (!m_bScannerEnabled)
		{
			//  Scanner is not enabled, ensure this parameter is retrieval when the scanner is not enabled
			for (int i = 0;;i++)
			{
				if (wAllowedTagsWhilstNotEnabled[i] == NULL)
				{
					LOG(INFO) + L"Unable to retrieve Scanner parameter as no scanner is enabled";
					return -1;
				}
				else if (cmp(wAllowedTagsWhilstNotEnabled[i], szParameterName))
				{
					//  This tag is allowed
					break;
				}
			}
		}
		//  Before we retrieve this property ensure we are an RS507 Scanner
		if (m_bScannerEnabled && !m_bUsingBluetoothScanner && 
			((metaTagsGetFunctions[szParameterName]) == &CScanner::rsm_GetPropertyValue))
		{
			LOG(WARNING) + L"Could not retrieve RSM Property " + szParameterName + " because the enabled scanner is not a bluetooth ring scanner";
			return -1;
		}
		iResult = (*this.*(metaTagsGetFunctions[szParameterName]))(szParameterName, szParameterValue);
	}
	else
	{
		//  Meta Tag was not recognised by the Scanner
		WCHAR wLog[MAXLOG];
		wsprintf(wLog, L"Parameter %s not found in Scanner Module", szParameterName);
		LOG(WARNING) + wLog;
	}
	return iResult;
}

void CScanner::RetrieveAllProperties(rho::apiGenerator::CMethodResult* pCallback)
{
	//  Iterate over the metaTagsGetFunctions which contains all the possible
	//  properties to retrieve
	rho::Hashtable<rho::String, rho::String> propsHash;
	for (metaTagsGetIterator = metaTagsGetFunctions.begin();
		metaTagsGetIterator != metaTagsGetFunctions.end();
		metaTagsGetIterator++)
	{
		//  Only return RSM properties for BT Scanners
		if (!(wcslen(metaTagsGetIterator->first) > 3 && wcsnicmp(metaTagsGetIterator->first, L"rsm", 3) == 0 &&
			!m_bUsingBluetoothScanner))
		{
			int iValueLength = RetrieveProperty(metaTagsGetIterator->first, NULL);
			if (iValueLength > 0)
			{
				WCHAR szValue[MAXIMUM_PARAMETER_LENGTH];
				if (!szValue) {return;}
				(*this.*(metaTagsGetFunctions[metaTagsGetIterator->first]))(metaTagsGetIterator->first, szValue);
				propsHash.put(rho::common::convertToStringA(metaTagsGetIterator->first), rho::common::convertToStringA(szValue));
			}
			else
				propsHash.put(rho::common::convertToStringA(metaTagsGetIterator->first), "Unavailable");
		}
	}
	pCallback->set(propsHash);
}

//  Get all properties supported on WM / CE
void CScanner::GetSupportedProperties(rho::apiGenerator::CMethodResult* pCallback)
{
	rho::Vector<rho::String> supportedProps;
	//  Iterate over the metaTagsGetFunctions which contains all the possible
	//  properties we support
	rho::Hashtable<rho::String, rho::String> propsHash;
	for (metaTagsGetIterator = metaTagsGetFunctions.begin();
		metaTagsGetIterator != metaTagsGetFunctions.end();
		metaTagsGetIterator++)
	{
		//  Only return RSM properties for BT Scanners
		if (!(wcslen(metaTagsGetIterator->first) > 3 && wcsnicmp(metaTagsGetIterator->first, L"rsm", 3) == 0 &&
			!m_bUsingBluetoothScanner))
		{
			supportedProps.addElement(rho::common::convertToStringA(metaTagsGetIterator->first));
		}
	}
	pCallback->set(supportedProps);
}


#pragma endregion

#pragma region EMDK_Interface

BOOL CScanner::LoadEMDK()
{
	if (m_hScnAPIDLL)
	{
		LOG(INFO) + L"Attempted to load scanner driver but it is already loaded";
		return FALSE;
	}
	m_hScnAPIDLL = LoadLibrary(EMDK_DLL);
	if (!m_hScnAPIDLL)
	{
		LOG(WARNING) + L"Failed to load scanner driver";	
		return FALSE;
	}
	
	//  Map function pointers to functions exported from the DLL
	lpfn_SCAN_AllocateBuffer = (LPFN_SCAN_ALLOCATEBUFFER_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_AllocateBuffer_EXPORT));
	lpfn_SCAN_CancelRead = (LPFN_SCAN_CANCELREAD_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_CancelRead_EXPORT));
	lpfn_SCAN_DeallocateBuffer = (LPFN_SCAN_DEALLOCATEBUFFER_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_DeallocateBuffer_EXPORT));
	lpfn_SCAN_Disable = (LPFN_SCAN_DISABLE_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_Disable_EXPORT));
	lpfn_SCAN_Flush = (LPFN_SCAN_FLUSH_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_Flush_EXPORT));
	lpfn_SCAN_GetDecoderParams = (LPFN_SCAN_GETDECODERPARAMS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_GetDecoderParams_EXPORT));
	lpfn_SCAN_GetEnabledDecoders = (LPFN_SCAN_GETENABLEDDECODERS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_GetEnabledDecoders_EXPORT));
	lpfn_SCAN_GetReaderParams = (LPFN_SCAN_GETREADERPARAMS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_GetReaderParams_EXPORT));
	lpfn_SCAN_GetScanParameters = (LPFN_SCAN_GETSCANPARAMETERS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_GetScanParameters_EXPORT));
	lpfn_SCAN_GetScanStatus = (LPFN_SCAN_GETSCANSTATUS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_GetScanStatus_EXPORT));
	lpfn_SCAN_GetSupportedDecoders = (LPFN_SCAN_GETSUPPORTEDDECODERS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_GetSupportedDecoders_EXPORT));
	lpfn_SCAN_GetUPCEANParams = (LPFN_SCAN_GETUPCEANPARAMS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_GetUPCEANParams_EXPORT));
	lpfn_SCAN_Open = (LPFN_SCAN_OPEN_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_Open_EXPORT));
	lpfn_SCAN_ReadLabelEvent = (LPFN_SCAN_READLABELEVENT_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_ReadLabelEvent_EXPORT));
	lpfn_SCAN_ReadLabelMsg = (LPFN_SCAN_READLABELMSG_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_ReadLabelMsg_EXPORT));
	lpfn_SCAN_RSMGetAttribute = (LPFN_SCAN_RSMGETATTRIBUTE_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_RSMGetAttribute_EXPORT));
	lpfn_SCAN_RSMGetSupportedAttributes = (LPFN_SCAN_RSMGETSUPPORTEDATTRIBUTES_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_RSMGetSupportedAttributes_EXPORT));
	lpfn_SCAN_RSMSetAttribute = (LPFN_SCAN_RSMSETATTRIBUTE_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_RSMSetAttribute_EXPORT));
	lpfn_SCAN_SetDecoderParams = (LPFN_SCAN_SETDECODERPARAMS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_SetDecoderParams_EXPORT));
	lpfn_SCAN_SetEnabledDecoders = (LPFN_SCAN_SETENABLEDDECODERS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_SetEnabledDecoders_EXPORT));
	lpfn_SCAN_SetReaderParams = (LPFN_SCAN_SETREADERPARAMS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_SetReaderParams_EXPORT));
	lpfn_SCAN_SetScanParameters = (LPFN_SCAN_SETSCANPARAMETERS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_SetScanParameters_EXPORT));
	lpfn_SCAN_SetSoftTrigger = (LPFN_SCAN_SETSOFTTRIGGER_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_SetSoftTrigger_EXPORT));
	lpfn_SCAN_SetUPCEANParams = (LPFN_SCAN_SETUPCEANPARAMS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_SetUPCEANParams_EXPORT));
	lpfn_SCAN_Close = (LPFN_SCAN_CLOSE_T)GetProcAddress(
		m_hScnAPIDLL, _T(SCAN_Close_EXPORT));
	lpfn_SCAN_Enable = (LPFN_SCAN_ENABLE_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_Enable_EXPORT));
	lpfn_SCAN_RegisterScanMessage = (LPFN_SCAN_REGISTERSCANMESSAGE_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_RegisterScanMessage_EXPORT));
	lpfn_SCAN_DeRegisterScanMessage = (LPFN_SCAN_DEREGISTERSCANMESSAGE_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_DeRegisterScanMessage_EXPORT));
	lpfn_SCAN_FindFirst	= (LPFN_SCAN_FINDFIRST_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_FindFirst_EXPORT));
	lpfn_SCAN_FindNext = (LPFN_SCAN_FINDNEXT_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_FindNext_EXPORT));
	lpfn_SCAN_FindClose = (LPFN_SCAN_FINDCLOSE_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_FindClose_EXPORT));
	lpfn_SCAN_GetInterfaceParams = (LPFN_SCAN_GETINTERFACEPARAMS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_GetInterfaceParams_EXPORT));
	lpfn_SCAN_SetInterfaceParams = (LPFN_SCAN_SETINTERFACEPARAMS_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_SetInterfaceParams_EXPORT));
	lpfn_SCAN_GetDeviceInfo = (LPFN_SCAN_GETDEVICEINFO_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_GetDeviceInfo_EXPORT));
	lpfn_SCAN_GetVersionInfo = (LPFN_SCAN_GETVERSIONINFO_T)GetProcAddress
		(m_hScnAPIDLL, _T(SCAN_GetVersionInfo_EXPORT));

	//  Check all functions have been loaded.  It is not an error for RSM functions
	//  to not be loaded, that functionality just won't be available
	if (!lpfn_SCAN_AllocateBuffer)
	{
		LOG(ERROR) + L"Unable to load SCAN_AllocateBuffer";
		return FALSE;
	}
	if (!lpfn_SCAN_CancelRead)	
		LOG(ERROR) + L"Unable to load SCAN_CancelRead";
	if (!lpfn_SCAN_DeallocateBuffer)			
		LOG(ERROR) + L"Unable to load SCAN_DeallocateBuffer";
	if (!lpfn_SCAN_Disable)					
		LOG(ERROR) + L"Unable to load SCAN_Disable";
	if (!lpfn_SCAN_Flush)						
		LOG(ERROR) + L"Unable to load SCAN_Flush";
	if (!lpfn_SCAN_GetDecoderParams)			
		LOG(ERROR) + L"Unable to load SCAN_GetDecoderParams";
	if (!lpfn_SCAN_GetEnabledDecoders)			
		LOG(ERROR) + L"Unable to load SCAN_GetEnabledDecoders";
	if (!lpfn_SCAN_GetReaderParams)			
		LOG(ERROR) + L"Unable to load SCAN_GetReaderParams";
	if (!lpfn_SCAN_GetScanParameters)			
		LOG(ERROR) + L"Unable to load SCAN_GetScanParameters";
	if (!lpfn_SCAN_GetScanStatus)				
		LOG(ERROR) + L"Unable to load SCAN_GetScanStatus";
	if (!lpfn_SCAN_GetSupportedDecoders)		
		LOG(ERROR) + L"Unable to load SCAN_GetSupportedDecoders";
	if (!lpfn_SCAN_GetUPCEANParams)			
		LOG(ERROR) + L"Unable to load SCAN_GetUPCEANParams";
	if (!lpfn_SCAN_Open)						
		LOG(ERROR) + L"Unable to load SCAN_Open";
	if (!lpfn_SCAN_ReadLabelEvent)				
		LOG(ERROR) + L"Unable to load SCAN_ReadLabelEvent";
	if (!lpfn_SCAN_ReadLabelMsg)				
		LOG(ERROR) + L"Unable to load SCAN_ReadLabelMsg";
	if (!lpfn_SCAN_SetDecoderParams)			
		LOG(ERROR) + L"Unable to load SCAN_SetDecoderParams";
	if (!lpfn_SCAN_SetEnabledDecoders)			
		LOG(ERROR) + L"Unable to load SCAN_SetEnabledDecoders";
	if (!lpfn_SCAN_SetReaderParams)			
		LOG(ERROR) + L"Unable to load SCAN_SetReaderParams";
	if (!lpfn_SCAN_SetScanParameters)			
		LOG(ERROR) + L"Unable to load SCAN_SetScanParameters";
	if (!lpfn_SCAN_SetSoftTrigger)				
		LOG(ERROR) + L"Unable to load SCAN_SetSoftTrigger";
	if (!lpfn_SCAN_SetUPCEANParams)			
		LOG(ERROR) + L"Unable to load SCAN_SetUPCEANParams";
	if (!lpfn_SCAN_Close)						
		LOG(ERROR) + L"Unable to load SCAN_Close";
	if (!lpfn_SCAN_Enable)						
		LOG(ERROR) + L"Unable to load SCAN_Enable";
	if (!lpfn_SCAN_RegisterScanMessage)
		LOG(ERROR) + L"Unable to load SCAN_RegisterScanMessage";
	if (!lpfn_SCAN_DeRegisterScanMessage)
		LOG(ERROR) + L"Unable to load SCAN_DeRegisterScanMessage";
	if (!lpfn_SCAN_FindFirst)
		LOG(ERROR) + L"Unable to load SCAN_FindFirst";
	if (!lpfn_SCAN_FindNext)
		LOG(ERROR) + L"Unable to load SCAN_FindNext";
	if (!lpfn_SCAN_FindClose)
		LOG(ERROR) + L"Unable to load SCAN_FindClose";
	if (!lpfn_SCAN_GetInterfaceParams)
		LOG(ERROR) + L"Unable to Load SCAN_GetInterfaceParams";
	if (!lpfn_SCAN_SetInterfaceParams)
		LOG(ERROR) + L"Unable to Load SCAN_SetInterfaceParams";
	if (!lpfn_SCAN_GetDeviceInfo)
		LOG(ERROR) + L"Unable to Load SCAN_GetDeviceInfo";
	if (!lpfn_SCAN_GetVersionInfo)
		LOG(ERROR) + L"Unable to Load SCAN_GetVersionInfo";

	//  Unable to Load RSM Attributes
	if (!(lpfn_SCAN_RSMGetAttribute && lpfn_SCAN_RSMGetSupportedAttributes && lpfn_SCAN_RSMSetAttribute))
		LOG(INFO) + L"RSM Functionality is not supported on this device";

	return TRUE;
}

BOOL CScanner::UnloadEMDK()
{
	if (m_hScnAPIDLL)
	{
		FreeLibrary(m_hScnAPIDLL);
		m_hScnAPIDLL = NULL;
	}
	else
	{
		LOG(WARNING) + L"Attempted to unload Scanner Driver but it is not loaded";
		return FALSE;
	}

	//  Nullify Function Pointers
	InitialiseEMDKFunctionPointers();

	return TRUE;
}

#pragma endregion

#pragma region InitialisationFunctions

void CScanner::InitialiseEMDKFunctionPointers()
{
	lpfn_SCAN_AllocateBuffer			= NULL;
	lpfn_SCAN_CancelRead				= NULL;
	lpfn_SCAN_DeallocateBuffer			= NULL;
	lpfn_SCAN_Disable					= NULL;
	lpfn_SCAN_Flush						= NULL;
	lpfn_SCAN_GetDecoderParams			= NULL;
	lpfn_SCAN_GetEnabledDecoders		= NULL;
	lpfn_SCAN_GetReaderParams			= NULL;
	lpfn_SCAN_GetScanParameters			= NULL;
	lpfn_SCAN_GetScanStatus				= NULL;
	lpfn_SCAN_GetSupportedDecoders		= NULL;
	lpfn_SCAN_GetUPCEANParams			= NULL;
	lpfn_SCAN_Open						= NULL;
	lpfn_SCAN_ReadLabelEvent			= NULL;
	lpfn_SCAN_ReadLabelMsg				= NULL;
	lpfn_SCAN_RSMGetAttribute			= NULL;
	lpfn_SCAN_RSMGetSupportedAttributes	= NULL;
	lpfn_SCAN_RSMSetAttribute			= NULL;
	lpfn_SCAN_SetDecoderParams			= NULL;
	lpfn_SCAN_SetEnabledDecoders		= NULL;
	lpfn_SCAN_SetReaderParams			= NULL;
	lpfn_SCAN_SetScanParameters			= NULL;
	lpfn_SCAN_SetSoftTrigger			= NULL;
	lpfn_SCAN_SetUPCEANParams			= NULL;
	lpfn_SCAN_Close						= NULL;
	lpfn_SCAN_Enable					= NULL;
	lpfn_SCAN_RegisterScanMessage		= NULL;
	lpfn_SCAN_DeRegisterScanMessage		= NULL;
	lpfn_SCAN_FindFirst					= NULL;
	lpfn_SCAN_FindNext					= NULL;
	lpfn_SCAN_FindClose					= NULL;
}


void CScanner::InitialiseMETATagMapForSet()
{
	//  Add All known meta tags to the map
	metaTagsSetFunctions[L"All_Decoders"]			= &CScanner::Set_all_decoders_MetaProc;
	metaTagsSetFunctions[L"AllDecoders"]			= &CScanner::Set_all_decoders_MetaProc;
	metaTagsSetFunctions[L"Auspostal"]				= &CScanner::Set_auspostal_MetaProc;
	metaTagsSetFunctions[L"aztec"]					= &CScanner::Set_AZTEC_MetaProc;
	metaTagsSetFunctions[L"Canpostal"]				= &CScanner::Set_canpostal_MetaProc;
	metaTagsSetFunctions[L"chinese_2of5"]			= &CScanner::Set_Chinese_2Of5_MetaProc;
	metaTagsSetFunctions[L"chinese2of5"]			= &CScanner::Set_Chinese_2Of5_MetaProc;
	metaTagsSetFunctions[L"codabar"]				= &CScanner::Set_codabar_MetaProc;
	metaTagsSetFunctions[L"codabarCLSIEditing"]		= &CScanner::Set_codabar_MetaProc;
	metaTagsSetFunctions[L"codabarMaxLength"]		= &CScanner::Set_codabar_MetaProc;
	metaTagsSetFunctions[L"codabarMinLength"]		= &CScanner::Set_codabar_MetaProc;
	metaTagsSetFunctions[L"codabarNotisEditing"]	= &CScanner::Set_codabar_MetaProc;
	metaTagsSetFunctions[L"codabarRedundancy"]		= &CScanner::Set_codabar_MetaProc;
	metaTagsSetFunctions[L"code11"]					= &CScanner::Set_code11_MetaProc;
	metaTagsSetFunctions[L"code11CheckDigitCount"]	= &CScanner::Set_code11_MetaProc;
	metaTagsSetFunctions[L"code11Redundancy"]		= &CScanner::Set_code11_MetaProc;
	metaTagsSetFunctions[L"code11ReportCheckdigit"]	= &CScanner::Set_code11_MetaProc;
	metaTagsSetFunctions[L"code128"]				= &CScanner::Set_code128_MetaProc;
	metaTagsSetFunctions[L"code128EAN128"]			= &CScanner::Set_code128_MetaProc;
	metaTagsSetFunctions[L"code128IsBT128"]			= &CScanner::Set_code128_MetaProc;
	metaTagsSetFunctions[L"code128MaxLength"]		= &CScanner::Set_code128_MetaProc;
	metaTagsSetFunctions[L"code128MinLength"]		= &CScanner::Set_code128_MetaProc;
	metaTagsSetFunctions[L"code39"]					= &CScanner::Set_code39_MetaProc;
	metaTagsSetFunctions[L"code39Code32Prefix"]		= &CScanner::Set_code39_MetaProc;
	metaTagsSetFunctions[L"code39Concatenation"]	= &CScanner::Set_code39_MetaProc;
	metaTagsSetFunctions[L"code39ConvertToCode32"]	= &CScanner::Set_code39_MetaProc;
	metaTagsSetFunctions[L"code39FullASCII"]		= &CScanner::Set_code39_MetaProc;
	metaTagsSetFunctions[L"code39MaxLength"]		= &CScanner::Set_code39_MetaProc;
	metaTagsSetFunctions[L"code39MinLength"]		= &CScanner::Set_code39_MetaProc;
	metaTagsSetFunctions[L"code39Redundancy"]		= &CScanner::Set_code39_MetaProc;
	metaTagsSetFunctions[L"code39ReportCheckDigit"] = &CScanner::Set_code39_MetaProc;
	metaTagsSetFunctions[L"code39VerifyCheckDigit"] = &CScanner::Set_code39_MetaProc;
	metaTagsSetFunctions[L"code93"]					= &CScanner::Set_code93_MetaProc;
	metaTagsSetFunctions[L"code93MaxLength"]		= &CScanner::Set_code93_MetaProc;
	metaTagsSetFunctions[L"code93MinLength"]		= &CScanner::Set_code93_MetaProc;
	metaTagsSetFunctions[L"code93Redundancy"]		= &CScanner::Set_code93_MetaProc;
	metaTagsSetFunctions[L"composit_ab"]			= &CScanner::Set_composit_ab_MetaProc;
	metaTagsSetFunctions[L"composite_ab"]			= &CScanner::Set_composit_ab_MetaProc;
	metaTagsSetFunctions[L"compositeab"]			= &CScanner::Set_composit_ab_MetaProc;
	metaTagsSetFunctions[L"composit_c"]				= &CScanner::Set_composit_c_MetaProc;
	metaTagsSetFunctions[L"composite_c"]			= &CScanner::Set_composit_c_MetaProc;
	metaTagsSetFunctions[L"compositec"]				= &CScanner::Set_composit_c_MetaProc;
	metaTagsSetFunctions[L"d2of5"]					= &CScanner::Set_d2of5_MetaProc;
	metaTagsSetFunctions[L"d2of5MaxLength"]			= &CScanner::Set_d2of5_MetaProc;
	metaTagsSetFunctions[L"d2of5MinLength"]			= &CScanner::Set_d2of5_MetaProc;
	metaTagsSetFunctions[L"d2of5Redundancy"]		= &CScanner::Set_d2of5_MetaProc;
	metaTagsSetFunctions[L"datamatrix"]				= &CScanner::Set_datamatrix_MetaProc;
	metaTagsSetFunctions[L"dutchPostal"]			= &CScanner::Set_dutchpostal_MetaProc;
	metaTagsSetFunctions[L"ean13"]					= &CScanner::Set_ean13_MetaProc;
	metaTagsSetFunctions[L"ean8"]					= &CScanner::Set_ean8_MetaProc;
	metaTagsSetFunctions[L"ean8ConvertToEAN13"]		= &CScanner::Set_ean8_MetaProc;
	metaTagsSetFunctions[L"i2of5"]					= &CScanner::Set_i2of5_MetaProc;
	metaTagsSetFunctions[L"i2of5ConvertToEAN13"]	= &CScanner::Set_i2of5_MetaProc;
	metaTagsSetFunctions[L"i2of5MaxLength"]			= &CScanner::Set_i2of5_MetaProc;
	metaTagsSetFunctions[L"i2of5MinLength"]			= &CScanner::Set_i2of5_MetaProc;
	metaTagsSetFunctions[L"i2of5Redundancy"]		= &CScanner::Set_i2of5_MetaProc;
	metaTagsSetFunctions[L"i2of5ReportCheckDigit"]	= &CScanner::Set_i2of5_MetaProc;
	metaTagsSetFunctions[L"i2of5VerifyCheckDigit"]	= &CScanner::Set_i2of5_MetaProc;
	metaTagsSetFunctions[L"japPostal"]				= &CScanner::Set_jappostal_MetaProc;
	metaTagsSetFunctions[L"Korean_3of5"]			= &CScanner::Set_Korean_3Of5_MetaProc;
	metaTagsSetFunctions[L"Korean3of5"]				= &CScanner::Set_Korean_3Of5_MetaProc;
	metaTagsSetFunctions[L"macroMicroPDF"]			= &CScanner::Set_macromicropdf_MetaProc;
	metaTagsSetFunctions[L"macroPDF"]				= &CScanner::Set_macropdf_MetaProc;
	metaTagsSetFunctions[L"maxiCode"]				= &CScanner::Set_maxicode_MetaProc;
	metaTagsSetFunctions[L"microPDF"]				= &CScanner::Set_micropdf_MetaProc;
	metaTagsSetFunctions[L"microQR"]				= &CScanner::Set_MicroQR_MetaProc;
	metaTagsSetFunctions[L"msi"]					= &CScanner::Set_msi_MetaProc;
	metaTagsSetFunctions[L"msiCheckDigits"]			= &CScanner::Set_msi_MetaProc;
	metaTagsSetFunctions[L"msicheckDigitScheme"]	= &CScanner::Set_msi_MetaProc;
	metaTagsSetFunctions[L"msiMaxLength"]			= &CScanner::Set_msi_MetaProc;
	metaTagsSetFunctions[L"msiMinLength"]			= &CScanner::Set_msi_MetaProc;
	metaTagsSetFunctions[L"msiRedundancy"]			= &CScanner::Set_msi_MetaProc;
	metaTagsSetFunctions[L"msiReportCheckDigit"]	= &CScanner::Set_msi_MetaProc;
	metaTagsSetFunctions[L"pdf417"]					= &CScanner::Set_pdf417_MetaProc;
	metaTagsSetFunctions[L"qrCode"]					= &CScanner::Set_qrcode_MetaProc;
	metaTagsSetFunctions[L"rss14"]					= &CScanner::Set_rss14_MetaProc;
	metaTagsSetFunctions[L"rssexp"]					= &CScanner::Set_rssexp_MetaProc;
	metaTagsSetFunctions[L"rsslim"]					= &CScanner::Set_rsslim_MetaProc;
	metaTagsSetFunctions[L"gs1dataBar"]				= &CScanner::Set_rss14_MetaProc;
	metaTagsSetFunctions[L"gs1dataBarExpanded"]		= &CScanner::Set_rssexp_MetaProc;
	metaTagsSetFunctions[L"gs1dataBarLimited"]		= &CScanner::Set_rsslim_MetaProc;
	metaTagsSetFunctions[L"signature"]				= &CScanner::Set_signature_MetaProc;
	metaTagsSetFunctions[L"tlc39"]					= &CScanner::Set_tlc39_MetaProc;
	metaTagsSetFunctions[L"trioptic39"]				= &CScanner::Set_trioptic39_MetaProc;
	metaTagsSetFunctions[L"trioptic39Redundancy"]	= &CScanner::Set_trioptic39_MetaProc;
	metaTagsSetFunctions[L"ukPostal"]				= &CScanner::Set_ukpostal_MetaProc;
	metaTagsSetFunctions[L"upc_eanbookland"]		= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upceanbookland"]			= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upc_eancoupon"]			= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upceancoupon"]			= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upc_eanlineardecode"]	= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upceanlineardecode"]		= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upc_eanRandomWeightCheckDigit"]	= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upceanRandomWeightCheckDigit"]	= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upc_eanRetryCount"]		= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upceanRetryCount"]		= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upc_eanSecurityLevel"]	= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upc_eanSupplemental2"]	= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upceanSupplemental2"]	= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upc_eanSupplemental5"]	= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upceanSupplemental5"]	= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upc_eanSupplementalMode"]= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upceanSupplementalMode"]	= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upca"]					= &CScanner::Set_upca_MetaProc;
	metaTagsSetFunctions[L"upcaPreamble"]			= &CScanner::Set_upca_MetaProc;
	metaTagsSetFunctions[L"upcaReportCheckDigit"]	= &CScanner::Set_upca_MetaProc;
	metaTagsSetFunctions[L"upce0"]					= &CScanner::Set_upce0_MetaProc;
	metaTagsSetFunctions[L"upce0ConvertToUPCA"]		= &CScanner::Set_upce0_MetaProc;
	metaTagsSetFunctions[L"upce0Preamble"]			= &CScanner::Set_upce0_MetaProc;
	metaTagsSetFunctions[L"upce0ReportCheckDigit"]	= &CScanner::Set_upce0_MetaProc;
	metaTagsSetFunctions[L"upce1"]					= &CScanner::Set_upce1_MetaProc;
	metaTagsSetFunctions[L"upce1ConvertToUPCA"]		= &CScanner::Set_upce1_MetaProc;
	metaTagsSetFunctions[L"upce1Preamble"]			= &CScanner::Set_upce1_MetaProc;
	metaTagsSetFunctions[L"upce1ReportCheckDigit"]	= &CScanner::Set_upce1_MetaProc;
	metaTagsSetFunctions[L"us4State"]				= &CScanner::Set_Us4State_MetaProc;
	metaTagsSetFunctions[L"usplanet"]				= &CScanner::Set_usplanet_MetaProc;
	metaTagsSetFunctions[L"uspostnet"]				= &CScanner::Set_uspostnet_MetaProc;
	metaTagsSetFunctions[L"webcode"]				= &CScanner::Set_webcode_MetaProc;
	metaTagsSetFunctions[L"matrix2of5"]				= &CScanner::Set_matrix2of5_MetaProc;
	metaTagsSetFunctions[L"matrix2of5MaxLength"]	= &CScanner::Set_matrix2of5_MetaProc;
	metaTagsSetFunctions[L"matrix2of5MinLength"]	= &CScanner::Set_matrix2of5_MetaProc;
	metaTagsSetFunctions[L"matrix2of5ReportCheckDigit"]	= &CScanner::Set_matrix2of5_MetaProc;
	metaTagsSetFunctions[L"matrix2of5VerifyCheckDigit"]	= &CScanner::Set_matrix2of5_MetaProc;
	metaTagsSetFunctions[L"us4StateFics"]			= &CScanner::Set_Us4StateFics_MetaProc;

	//  Top level Scanner Tags
	metaTagsSetFunctions[L"getDeviceCapabilities"]	= &CScanner::ScannerTag_MetaProc;	//  Not exposed to the user
	metaTagsSetFunctions[L"enable"]					= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"disable"]				= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"enabled"]				= &CScanner::ScannerTag_MetaProc;	//  Work around for T&V tests which attempt two methods in one EMML tag
	metaTagsSetFunctions[L"disabled"]				= &CScanner::ScannerTag_MetaProc;	//  Work around for T&V tests which attempt two methods in one EMML tag
	metaTagsSetFunctions[L"init"]					= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"initialise"]				= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"deinit"]					= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"deinitialise"]			= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"start"]					= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"stop"]					= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"enabled"]				= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"autoenter"]				= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"autoTab"]				= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"lowBatteryScan"]			= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"triggerConnected"]		= &CScanner::ScannerTag_MetaProc;
	metaTagsSetFunctions[L"rasterMode"]				= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"aimType"]				= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"aimMode"]				= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"linearSecurityLevel"]	= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"dbpMode"]				= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"focusMode"]				= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"picklistMode"]			= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"illuminationMode"]		= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"viewfinderMode"]			= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"viewfinderFeedback"]		= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"inverse1DMode"]			= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"timedAimDuration"]		= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"connectionIdleTimeout"]	= &CScanner::ScannerBluetoothTag_MetaProc;
	metaTagsSetFunctions[L"disconnectBTOnDisable"]	= &CScanner::ScannerBluetoothTag_MetaProc;
	metaTagsSetFunctions[L"displayBtAddressBarcodeOnEnable"]	= &CScanner::ScannerBluetoothTag_MetaProc;
	metaTagsSetFunctions[L"enableTimeout"]			= &CScanner::ScannerBluetoothTag_MetaProc;
	metaTagsSetFunctions[L"narrowbeam"]				= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"beamWidth"]				= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"scanTimeout"]			= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"klasseEins"]				= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"bidirectionalRedundancy"]= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"rasterHeight"]			= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"sameSymbolTimeout"]		= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"differentSymbolTimeout"]	= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"poorQuality1DMode"]		= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"dpmMode"]				= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"viewfinderX"]			= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"viewfinderY"]			= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"viewfinderWidth"]		= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"viewfinderHeight"]		= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"viewfinderFeedbackTime"]	= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"adaptiveScanning"]		= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"dataBufferSize"]			= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"scanType"]				= &CScanner::ScannerReaderTag_MetaProc;	//  Not exposed to the user	
	metaTagsSetFunctions[L"barcodeDataFormat"]		= &CScanner::ScannerReaderTag_MetaProc;
	metaTagsSetFunctions[L"disableScannerDuringNavigate"]	= &CScanner::ScannerTag_MetaProc;
	//  Scanner Beeper Parameters
	metaTagsSetFunctions[L"DecodeVolume"]				= &CScanner::ScannerBeeper_MetaProc;
	metaTagsSetFunctions[L"DecodeDuration"]				= &CScanner::ScannerBeeper_MetaProc;
	metaTagsSetFunctions[L"decodeFrequency"]			= &CScanner::ScannerBeeper_MetaProc;
	metaTagsSetFunctions[L"SuccessfulDecodeFrequency"]	= &CScanner::ScannerBeeper_MetaProc;
	metaTagsSetFunctions[L"InvalidDecodeFrequency"]		= &CScanner::ScannerBeeper_MetaProc;
	metaTagsSetFunctions[L"DecodeSound"]				= &CScanner::ScannerBeeper_MetaProc;
	metaTagsSetFunctions[L"InvalidDecodeSound"]			= &CScanner::ScannerBeeper_MetaProc;
	//  RSM Tags
	metaTagsSetFunctions[L"rsmBluetoothAuthentication"]		= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmBluetoothEncryption"]			= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmBluetoothPinCode"]			= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmBluetoothPinCodeType"]		= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmBluetoothReconnectionAttempts"]		= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmBluetoothBeepOnReconnectAttempt"]		= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmBluetoothHidAutoReconnect"]	= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmBluetoothFriendlyName"]		= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmBluetoothInquiryMode"]		= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmBluetoothAutoReconnect"]		= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmForceSavePairingBarcode"]		= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmLowBatteryIndication"]		= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmLowBatteryIndicationCycle"]	= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmScanLineWidth"]				= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmGoodScansDelay"]				= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmDecodeFeedback"]				= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmIgnoreCode128Usps"]			= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmScanTriggerWakeup"]			= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmMems"]						= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmProximityEnable"]				= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmProximityContinuous"]			= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmProximityDistance"]			= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmPagingEnable"]				= &CScanner::rsm_MetaProc;
	metaTagsSetFunctions[L"rsmPagingBeepSequence"]			= &CScanner::rsm_MetaProc;

	//  Parameters newly added in RhoElements 1.0
	metaTagsSetFunctions[L"code128CheckISBTTable"]					= &CScanner::Set_code128_MetaProc;
	metaTagsSetFunctions[L"compositeABUseUPCPreambleCheckDigitRules"]	= &CScanner::Set_composit_ab_MetaProc;
	metaTagsSetFunctions[L"compositeabucclinkmode"]					= &CScanner::Set_composit_ab_MetaProc;
	metaTagsSetFunctions[L"Korean3Of5Redundancy"]					= &CScanner::Set_Korean_3Of5_MetaProc;
	metaTagsSetFunctions[L"Korean3Of5MinLength"]					= &CScanner::Set_Korean_3Of5_MetaProc;
	metaTagsSetFunctions[L"Korean3Of5MaxLength"]					= &CScanner::Set_Korean_3Of5_MetaProc;
	metaTagsSetFunctions[L"macroMicroPDFReportAppendInfo"]			= &CScanner::Set_macromicropdf_MetaProc;
	metaTagsSetFunctions[L"macroMicroPDFBufferLabels"]				= &CScanner::Set_macromicropdf_MetaProc;
	metaTagsSetFunctions[L"macroMicroPdfConvertToMicroPdf"]			= &CScanner::Set_macromicropdf_MetaProc;
	metaTagsSetFunctions[L"macroMicroPDFExclusive"]					= &CScanner::Set_macromicropdf_MetaProc;
	metaTagsSetFunctions[L"macroPDFBufferLabels"]					= &CScanner::Set_macropdf_MetaProc;
	metaTagsSetFunctions[L"macroPDFExclusive"]						= &CScanner::Set_macropdf_MetaProc;
	metaTagsSetFunctions[L"macroPDFConvertToPDF417"]				= &CScanner::Set_macropdf_MetaProc;
	metaTagsSetFunctions[L"upcEANConvertRSSToUPCEAN"]				= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"upcEanConvertGs1dataBarToUpcEan"]		= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"webcodeDecodeGTSubtype"]					= &CScanner::Set_webcode_MetaProc;
	metaTagsSetFunctions[L"signatureImageQuality"]					= &CScanner::Set_signature_MetaProc;
	metaTagsSetFunctions[L"signatureImageWidth"]					= &CScanner::Set_signature_MetaProc;
	metaTagsSetFunctions[L"signatureImageHeight"]					= &CScanner::Set_signature_MetaProc;
	metaTagsSetFunctions[L"signatureImageSize"]						= &CScanner::Set_signature_MetaProc;
	metaTagsSetFunctions[L"upcEANSecurityLevel"]					= &CScanner::Set_upc_ean_MetaProc;
	metaTagsSetFunctions[L"code128IsBT128ConcatMode"]				= &CScanner::Set_code128_MetaProc;
	metaTagsSetFunctions[L"Code128SecurityLevel"]					= &CScanner::Set_code128_MetaProc;
	metaTagsSetFunctions[L"code128other128"]						= &CScanner::Set_code128_MetaProc;
	metaTagsSetFunctions[L"code128redundancy"]						= &CScanner::Set_code128_MetaProc;
	metaTagsSetFunctions[L"upcEANBooklandFormat"]					= &CScanner::Set_upc_ean_MetaProc;	

	//  Parameters newly added in RhoElements 2.1
	metaTagsSetFunctions[L"code39securityLevel"]					= &CScanner::Set_code39_MetaProc;
	metaTagsSetFunctions[L"code11maxLength"]						= &CScanner::Set_code11_MetaProc;
	metaTagsSetFunctions[L"code11minLength"]						= &CScanner::Set_code11_MetaProc;

}

void CScanner::InitialiseMETATagMapForGet()
{
	//  Add All known meta tags to the map for retrieval
//	metaTagsGetFunctions[L"All_Decoders"]			= &CScanner::Get_all_decoders_PropertyValue;
	metaTagsGetFunctions[L"AllDecoders"]			= &CScanner::Get_all_decoders_PropertyValue;
	metaTagsGetFunctions[L"Auspostal"]				= &CScanner::Get_auspostal_PropertyValue;
	metaTagsGetFunctions[L"aztec"]					= &CScanner::Get_AZTEC_PropertyValue;
	metaTagsGetFunctions[L"Canpostal"]				= &CScanner::Get_canpostal_PropertyValue;
//	metaTagsGetFunctions[L"chinese_2of5"]			= &CScanner::Get_Chinese_2Of5_PropertyValue;
	metaTagsGetFunctions[L"chinese2of5"]			= &CScanner::Get_Chinese_2Of5_PropertyValue;
	metaTagsGetFunctions[L"codabar"]				= &CScanner::Get_codabar_PropertyValue;
	metaTagsGetFunctions[L"codabarCLSIEditing"]		= &CScanner::Get_codabar_PropertyValue;
	metaTagsGetFunctions[L"codabarMaxLength"]		= &CScanner::Get_codabar_PropertyValue;
	metaTagsGetFunctions[L"codabarMinLength"]		= &CScanner::Get_codabar_PropertyValue;
	metaTagsGetFunctions[L"codabarNotisEditing"]	= &CScanner::Get_codabar_PropertyValue;
	metaTagsGetFunctions[L"codabarRedundancy"]		= &CScanner::Get_codabar_PropertyValue;
	metaTagsGetFunctions[L"code11"]					= &CScanner::Get_code11_PropertyValue;
	metaTagsGetFunctions[L"code11CheckDigitCount"]	= &CScanner::Get_code11_PropertyValue;
	metaTagsGetFunctions[L"code11Redundancy"]		= &CScanner::Get_code11_PropertyValue;
	metaTagsGetFunctions[L"code11ReportCheckdigit"]	= &CScanner::Get_code11_PropertyValue;
	metaTagsGetFunctions[L"code11MinLength"]		= &CScanner::Get_code11_PropertyValue;
	metaTagsGetFunctions[L"code11MaxLength"]		= &CScanner::Get_code11_PropertyValue;
	metaTagsGetFunctions[L"code128"]				= &CScanner::Get_code128_PropertyValue;
	metaTagsGetFunctions[L"code128EAN128"]			= &CScanner::Get_code128_PropertyValue;
	metaTagsGetFunctions[L"code128IsBT128"]			= &CScanner::Get_code128_PropertyValue;
	metaTagsGetFunctions[L"code128MaxLength"]		= &CScanner::Get_code128_PropertyValue;
	metaTagsGetFunctions[L"code128MinLength"]		= &CScanner::Get_code128_PropertyValue;
	metaTagsGetFunctions[L"code39"]					= &CScanner::Get_code39_PropertyValue;
	metaTagsGetFunctions[L"code39Code32Prefix"]		= &CScanner::Get_code39_PropertyValue;
//	metaTagsGetFunctions[L"code39Concatenation"]	= &CScanner::Get_code39_PropertyValue; //(EMDK Says Deprecated)
	metaTagsGetFunctions[L"code39ConvertToCode32"]	= &CScanner::Get_code39_PropertyValue;
	metaTagsGetFunctions[L"code39FullASCII"]		= &CScanner::Get_code39_PropertyValue;
	metaTagsGetFunctions[L"code39MaxLength"]		= &CScanner::Get_code39_PropertyValue;
	metaTagsGetFunctions[L"code39MinLength"]		= &CScanner::Get_code39_PropertyValue;
	metaTagsGetFunctions[L"code39Redundancy"]		= &CScanner::Get_code39_PropertyValue;
	metaTagsGetFunctions[L"code39ReportCheckDigit"] = &CScanner::Get_code39_PropertyValue;
	metaTagsGetFunctions[L"code39VerifyCheckDigit"] = &CScanner::Get_code39_PropertyValue;
	metaTagsGetFunctions[L"code39SecurityLevel"]	= &CScanner::Get_code39_PropertyValue;
	metaTagsGetFunctions[L"code93"]					= &CScanner::Get_code93_PropertyValue;
	metaTagsGetFunctions[L"code93MaxLength"]		= &CScanner::Get_code93_PropertyValue;
	metaTagsGetFunctions[L"code93MinLength"]		= &CScanner::Get_code93_PropertyValue;
	metaTagsGetFunctions[L"code93Redundancy"]		= &CScanner::Get_code93_PropertyValue;
//	metaTagsGetFunctions[L"composit_ab"]			= &CScanner::Get_composit_ab_PropertyValue;
//	metaTagsGetFunctions[L"composite_ab"]			= &CScanner::Get_composit_ab_PropertyValue;
	metaTagsGetFunctions[L"compositeab"]			= &CScanner::Get_composit_ab_PropertyValue;
//	metaTagsGetFunctions[L"composit_c"]				= &CScanner::Get_composit_c_PropertyValue;
//	metaTagsGetFunctions[L"composite_c"]			= &CScanner::Get_composit_c_PropertyValue;
	metaTagsGetFunctions[L"compositec"]				= &CScanner::Get_composit_c_PropertyValue;
	metaTagsGetFunctions[L"d2of5"]					= &CScanner::Get_d2of5_PropertyValue;
	metaTagsGetFunctions[L"d2of5MaxLength"]			= &CScanner::Get_d2of5_PropertyValue;
	metaTagsGetFunctions[L"d2of5MinLength"]			= &CScanner::Get_d2of5_PropertyValue;
	metaTagsGetFunctions[L"d2of5Redundancy"]		= &CScanner::Get_d2of5_PropertyValue;
	metaTagsGetFunctions[L"datamatrix"]				= &CScanner::Get_datamatrix_PropertyValue;
	metaTagsGetFunctions[L"dutchPostal"]			= &CScanner::Get_dutchpostal_PropertyValue;
	metaTagsGetFunctions[L"ean13"]					= &CScanner::Get_ean13_PropertyValue;
	metaTagsGetFunctions[L"ean8"]					= &CScanner::Get_ean8_PropertyValue;
	metaTagsGetFunctions[L"ean8ConvertToEAN13"]		= &CScanner::Get_ean8_PropertyValue;
	metaTagsGetFunctions[L"i2of5"]					= &CScanner::Get_i2of5_PropertyValue;
	metaTagsGetFunctions[L"i2of5ConvertToEAN13"]	= &CScanner::Get_i2of5_PropertyValue;
	metaTagsGetFunctions[L"i2of5MaxLength"]			= &CScanner::Get_i2of5_PropertyValue;
	metaTagsGetFunctions[L"i2of5MinLength"]			= &CScanner::Get_i2of5_PropertyValue;
	metaTagsGetFunctions[L"i2of5Redundancy"]		= &CScanner::Get_i2of5_PropertyValue;
	metaTagsGetFunctions[L"i2of5ReportCheckDigit"]	= &CScanner::Get_i2of5_PropertyValue;
	metaTagsGetFunctions[L"i2of5VerifyCheckDigit"]	= &CScanner::Get_i2of5_PropertyValue;
	metaTagsGetFunctions[L"japPostal"]				= &CScanner::Get_jappostal_PropertyValue;
//	metaTagsGetFunctions[L"Korean_3of5"]			= &CScanner::Get_Korean_3Of5_PropertyValue;
	metaTagsGetFunctions[L"Korean3of5"]				= &CScanner::Get_Korean_3Of5_PropertyValue;
	metaTagsGetFunctions[L"macroMicroPDF"]			= &CScanner::Get_macromicropdf_PropertyValue;
	metaTagsGetFunctions[L"macroPDF"]				= &CScanner::Get_macropdf_PropertyValue;
	metaTagsGetFunctions[L"maxiCode"]				= &CScanner::Get_maxicode_PropertyValue;
	metaTagsGetFunctions[L"microPDF"]				= &CScanner::Get_micropdf_PropertyValue;
	metaTagsGetFunctions[L"microQR"]				= &CScanner::Get_MicroQR_PropertyValue;
	metaTagsGetFunctions[L"msi"]					= &CScanner::Get_msi_PropertyValue;
	metaTagsGetFunctions[L"msiCheckDigits"]			= &CScanner::Get_msi_PropertyValue;
	metaTagsGetFunctions[L"msicheckDigitScheme"]	= &CScanner::Get_msi_PropertyValue;
	metaTagsGetFunctions[L"msiMaxLength"]			= &CScanner::Get_msi_PropertyValue;
	metaTagsGetFunctions[L"msiMinLength"]			= &CScanner::Get_msi_PropertyValue;
	metaTagsGetFunctions[L"msiRedundancy"]			= &CScanner::Get_msi_PropertyValue;
	metaTagsGetFunctions[L"msiReportCheckDigit"]	= &CScanner::Get_msi_PropertyValue;
	metaTagsGetFunctions[L"pdf417"]					= &CScanner::Get_pdf417_PropertyValue;
	metaTagsGetFunctions[L"qrCode"]					= &CScanner::Get_qrcode_PropertyValue;
//	metaTagsGetFunctions[L"rss14"]					= &CScanner::Get_rss14_PropertyValue;
//	metaTagsGetFunctions[L"rssexp"]					= &CScanner::Get_rssexp_PropertyValue;
//	metaTagsGetFunctions[L"rsslim"]					= &CScanner::Get_rsslim_PropertyValue;
	metaTagsGetFunctions[L"gs1dataBar"]				= &CScanner::Get_rss14_PropertyValue;
	metaTagsGetFunctions[L"gs1dataBarExpanded"]		= &CScanner::Get_rssexp_PropertyValue;
	metaTagsGetFunctions[L"gs1dataBarLimited"]		= &CScanner::Get_rsslim_PropertyValue;
	metaTagsGetFunctions[L"signature"]				= &CScanner::Get_signature_PropertyValue;
	metaTagsGetFunctions[L"tlc39"]					= &CScanner::Get_tlc39_PropertyValue;
	metaTagsGetFunctions[L"trioptic39"]				= &CScanner::Get_trioptic39_PropertyValue;
	metaTagsGetFunctions[L"trioptic39Redundancy"]	= &CScanner::Get_trioptic39_PropertyValue;
	metaTagsGetFunctions[L"ukPostal"]				= &CScanner::Get_ukpostal_PropertyValue;
//	metaTagsGetFunctions[L"upc_eanbookland"]		= &CScanner::Get_upc_ean_PropertyValue;
	metaTagsGetFunctions[L"upceanbookland"]			= &CScanner::Get_upc_ean_PropertyValue;
//	metaTagsGetFunctions[L"upc_eancoupon"]			= &CScanner::Get_upc_ean_PropertyValue;
	metaTagsGetFunctions[L"upceancoupon"]			= &CScanner::Get_upc_ean_PropertyValue;
//	metaTagsGetFunctions[L"upc_eanlineardecode"]	= &CScanner::Get_upc_ean_PropertyValue;
	metaTagsGetFunctions[L"upceanlineardecode"]		= &CScanner::Get_upc_ean_PropertyValue;
//	metaTagsGetFunctions[L"upc_eanRandomWeightCheckDigit"]	= &CScanner::Get_upc_ean_PropertyValue;
	metaTagsGetFunctions[L"upceanRandomWeightCheckDigit"]	= &CScanner::Get_upc_ean_PropertyValue;
//	metaTagsGetFunctions[L"upc_eanRetryCount"]		= &CScanner::Get_upc_ean_PropertyValue;
	metaTagsGetFunctions[L"upceanRetryCount"]		= &CScanner::Get_upc_ean_PropertyValue;
//	metaTagsGetFunctions[L"upc_eanSecurityLevel"]	= &CScanner::Get_upc_ean_PropertyValue;
//	metaTagsGetFunctions[L"upc_eanSupplemental2"]	= &CScanner::Get_upc_ean_PropertyValue;
	metaTagsGetFunctions[L"upceanSupplemental2"]	= &CScanner::Get_upc_ean_PropertyValue;
//	metaTagsGetFunctions[L"upc_eanSupplemental5"]	= &CScanner::Get_upc_ean_PropertyValue;
	metaTagsGetFunctions[L"upceanSupplemental5"]	= &CScanner::Get_upc_ean_PropertyValue;
//	metaTagsGetFunctions[L"upc_eanSupplementalMode"]= &CScanner::Get_upc_ean_PropertyValue;
	metaTagsGetFunctions[L"upceanSupplementalMode"]	= &CScanner::Get_upc_ean_PropertyValue;
	metaTagsGetFunctions[L"upca"]					= &CScanner::Get_upca_PropertyValue;
	metaTagsGetFunctions[L"upcaPreamble"]			= &CScanner::Get_upca_PropertyValue;
	metaTagsGetFunctions[L"upcaReportCheckDigit"]	= &CScanner::Get_upca_PropertyValue;
	metaTagsGetFunctions[L"upce0"]					= &CScanner::Get_upce0_PropertyValue;
	metaTagsGetFunctions[L"upce0ConvertToUPCA"]		= &CScanner::Get_upce0_PropertyValue;
	metaTagsGetFunctions[L"upce0Preamble"]			= &CScanner::Get_upce0_PropertyValue;
	metaTagsGetFunctions[L"upce0ReportCheckDigit"]	= &CScanner::Get_upce0_PropertyValue;
	metaTagsGetFunctions[L"upce1"]					= &CScanner::Get_upce1_PropertyValue;
	metaTagsGetFunctions[L"upce1ConvertToUPCA"]		= &CScanner::Get_upce1_PropertyValue;
	metaTagsGetFunctions[L"upce1Preamble"]			= &CScanner::Get_upce1_PropertyValue;
	metaTagsGetFunctions[L"upce1ReportCheckDigit"]	= &CScanner::Get_upce1_PropertyValue;
	metaTagsGetFunctions[L"us4State"]				= &CScanner::Get_Us4State_PropertyValue;
	metaTagsGetFunctions[L"usPlanet"]				= &CScanner::Get_usplanet_PropertyValue;
	metaTagsGetFunctions[L"usPostNet"]				= &CScanner::Get_uspostnet_PropertyValue;
	metaTagsGetFunctions[L"webcode"]				= &CScanner::Get_webcode_PropertyValue;
	metaTagsGetFunctions[L"matrix2of5"]				= &CScanner::Get_matrix2of5_PropertyValue;
	metaTagsGetFunctions[L"matrix2of5MaxLength"]	= &CScanner::Get_matrix2of5_PropertyValue;
	metaTagsGetFunctions[L"matrix2of5MinLength"]	= &CScanner::Get_matrix2of5_PropertyValue;
	metaTagsGetFunctions[L"matrix2of5ReportCheckDigit"]	= &CScanner::Get_matrix2of5_PropertyValue;
	metaTagsGetFunctions[L"matrix2of5VerifyCheckDigit"]	= &CScanner::Get_matrix2of5_PropertyValue;
	metaTagsGetFunctions[L"us4StateFics"]			= &CScanner::Get_Us4StateFics_PropertyValue;

	//  Top level Scanner Tags
//	metaTagsGetFunctions[L"getDeviceCapabilities"]	= &CScanner::ScannerTag_GetPropertyValue;
//	metaTagsGetFunctions[L"enable"]					= &CScanner::ScannerTag_GetPropertyValue;
//	metaTagsGetFunctions[L"disable"]				= &CScanner::ScannerTag_GetPropertyValue;
//	metaTagsGetFunctions[L"enabled"]				= &CScanner::ScannerTag_GetPropertyValue;	//  Work around for T&V tests which attempt two methods in one EMML tag
//	metaTagsGetFunctions[L"disabled"]				= &CScanner::ScannerTag_GetPropertyValue;	//  Work around for T&V tests which attempt two methods in one EMML tag
//	metaTagsGetFunctions[L"init"]					= &CScanner::ScannerTag_GetPropertyValue;
//	metaTagsGetFunctions[L"initialise"]				= &CScanner::ScannerTag_GetPropertyValue;
//	metaTagsGetFunctions[L"deinit"]					= &CScanner::ScannerTag_GetPropertyValue;
//	metaTagsGetFunctions[L"deinitialise"]			= &CScanner::ScannerTag_GetPropertyValue;
//	metaTagsGetFunctions[L"start"]					= &CScanner::ScannerTag_GetPropertyValue;
//	metaTagsGetFunctions[L"stop"]					= &CScanner::ScannerTag_GetPropertyValue;
//	metaTagsGetFunctions[L"enabled"]				= &CScanner::ScannerTag_GetPropertyValue;
	metaTagsGetFunctions[L"autoenter"]				= &CScanner::ScannerTag_GetPropertyValue;
	metaTagsGetFunctions[L"autoTab"]				= &CScanner::ScannerTag_GetPropertyValue;
	metaTagsGetFunctions[L"friendlyName"]			= &CScanner::ScannerTag_GetPropertyValue;
	metaTagsGetFunctions[L"triggerConnected"]		= &CScanner::ScannerTag_GetPropertyValue;
	metaTagsGetFunctions[L"rasterMode"]				= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"aimType"]				= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"aimMode"]				= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"linearSecurityLevel"]	= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"dbpMode"]				= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"focusMode"]				= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"picklistMode"]			= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"illuminationMode"]		= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"viewfinderMode"]			= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"viewfinderFeedback"]		= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"inverse1DMode"]			= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"timedAimDuration"]		= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"connectionIdleTimeout"]	= &CScanner::ScannerBluetoothTag_GetPropertyValue;
	metaTagsGetFunctions[L"disconnectBTOnDisable"]	= &CScanner::ScannerBluetoothTag_GetPropertyValue;
	metaTagsGetFunctions[L"displayBTAddressBarcodeOnEnable"]	= &CScanner::ScannerBluetoothTag_GetPropertyValue;
	metaTagsGetFunctions[L"enableTimeout"]			= &CScanner::ScannerBluetoothTag_GetPropertyValue;
//	metaTagsGetFunctions[L"narrowbeam"]				= &CScanner::ScannerReaderTag_GetPropertyValue;  //  Deprecated
	metaTagsGetFunctions[L"beamWidth"]				= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"scanTimeout"]			= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"klasseEins"]				= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"bidirectionalRedundancy"]= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"rasterHeight"]			= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"sameSymbolTimeout"]		= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"differentSymbolTimeout"]	= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"poorQuality1DMode"]		= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"dpmMode"]				= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"viewfinderX"]			= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"viewfinderY"]			= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"viewfinderWidth"]		= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"viewfinderHeight"]		= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"viewfinderFeedbackTime"]	= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"adaptiveScanning"]		= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"dataBufferSize"]			= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"scanType"]				= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"barcodeDataFormat"]		= &CScanner::ScannerReaderTag_GetPropertyValue;
	metaTagsGetFunctions[L"scannerType"]			= &CScanner::ScannerReaderTag_GetPropertyValue;
	//  Scanner Beeper Parameters
	metaTagsGetFunctions[L"DecodeVolume"]				= &CScanner::ScannerBeeper_GetPropertyValue;
	metaTagsGetFunctions[L"DecodeDuration"]				= &CScanner::ScannerBeeper_GetPropertyValue;
	metaTagsGetFunctions[L"DecodeFrequency"]			= &CScanner::ScannerBeeper_GetPropertyValue;
	metaTagsGetFunctions[L"InvalidDecodeFrequency"]		= &CScanner::ScannerBeeper_GetPropertyValue;
	metaTagsGetFunctions[L"DecodeSound"]				= &CScanner::ScannerBeeper_GetPropertyValue;
	metaTagsGetFunctions[L"InvalidDecodeSound"]			= &CScanner::ScannerBeeper_GetPropertyValue;
	metaTagsGetFunctions[L"lowBatteryScan"]				= &CScanner::ScannerTag_GetPropertyValue;;
	metaTagsGetFunctions[L"disableScannerDuringNavigate"]	= &CScanner::ScannerTag_GetPropertyValue;;
	//  RSM Tags
	metaTagsGetFunctions[L"rsmModelNumber"]					= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmSerialNumber"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmDateOfManufacture"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmDateOfService"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBluetoothAddress"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmFirmwareVersion"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmDeviceClass"]					= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBatteryStatus"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBatteryCapacity"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBatteryId"]					= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBluetoothAuthentication"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBluetoothEncryption"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBluetoothPinCode"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBluetoothPinCodeType"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBluetoothReconnectionAttempts"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBluetoothBeepOnReconnectAttempt"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBluetoothHidAutoReconnect"]	= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBluetoothFriendlyName"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBluetoothInquiryMode"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmBluetoothAutoReconnect"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmForceSavePairingBarcode"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmLowBatteryIndication"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmLowBatteryIndicationCycle"]	= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmScanLineWidth"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmGoodScansDelay"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmDecodeFeedback"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmIgnoreCode128Usps"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmScanTriggerWakeup"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmMems"]						= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmProximityEnable"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmProximityContinuous"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmProximityDistance"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmPagingEnable"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"rsmPagingBeepSequence"]			= &CScanner::rsm_GetPropertyValue;

/*
	metaTagsGetFunctions[L"markForRetrievalscanLineWidth"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievaldecodeFeedback"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbluetoothAuthentication"]	= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbluetoothEncryption"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbluetoothPINCode"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbluetoothReconnectAttempts"]	= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbluetoothBeepOnReconnectAttempt"]	= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbluetoothHIDAutoReconnect"]	= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbluetoothFriendlyName"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbluetoothPINCodeType"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbluetoothInquiryMode"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalignoreCode128USPS"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalmems"]						= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalproximityEnable"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalproximityDistance"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalpagingEnable"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalpagingBeepSequence"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievallowBatteryIndication"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalscanTriggerWakeup"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbluetoothAutoReconnect"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievallowBatteryIndicationCycle"]	= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalproximityContinuous"]		= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalgoodScansDelay"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalForceSavePairingBarcode"]	= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalmodelNumber"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalserialNumber"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievaldateOfManufacture"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievaldateOfService"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbluetoothAddress"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalfirmwareVersion"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievaldeviceClass"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbatteryStatus"]				= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbatteryCapacity"]			= &CScanner::rsm_GetPropertyValue;
	metaTagsGetFunctions[L"markForRetrievalbatteryId"]					= &CScanner::rsm_GetPropertyValue;
*/
	//  RSM Events
//	metaTagsGetFunctions[L"RSMGetEvent"] = &CScanner::rsm_GetPropertyValue;

	//  Parameters newly added in RhoElements 1.0
	metaTagsGetFunctions[L"code128CheckISBTTable"]					= &CScanner::Get_code128_PropertyValue;
	metaTagsGetFunctions[L"compositeABUseUPCPreambleCheckDigitRules"]	= &CScanner::Get_composit_ab_PropertyValue;
	metaTagsGetFunctions[L"compositeabucclinkmode"]					= &CScanner::Get_composit_ab_PropertyValue;
	metaTagsGetFunctions[L"Korean3Of5Redundancy"]					= &CScanner::Get_Korean_3Of5_PropertyValue;
	metaTagsGetFunctions[L"Korean3Of5MinLength"]					= &CScanner::Get_Korean_3Of5_PropertyValue;
	metaTagsGetFunctions[L"Korean3Of5MaxLength"]					= &CScanner::Get_Korean_3Of5_PropertyValue;
	metaTagsGetFunctions[L"macroMicroPDFReportAppendInfo"]			= &CScanner::Get_macromicropdf_PropertyValue;
	metaTagsGetFunctions[L"macroMicroPDFBufferLabels"]				= &CScanner::Get_macromicropdf_PropertyValue;
	metaTagsGetFunctions[L"macroMicroPdfConvertToMicroPdf"]			= &CScanner::Get_macromicropdf_PropertyValue;
	metaTagsGetFunctions[L"macroMicroPDFExclusive"]					= &CScanner::Get_macromicropdf_PropertyValue;
	metaTagsGetFunctions[L"macroPDFBufferLabels"]					= &CScanner::Get_macropdf_PropertyValue;
	metaTagsGetFunctions[L"macroPDFExclusive"]						= &CScanner::Get_macropdf_PropertyValue;
	metaTagsGetFunctions[L"macroPDFConvertToPDF417"]				= &CScanner::Get_macropdf_PropertyValue;
	metaTagsGetFunctions[L"upcEANConvertRSSToUPCEAN"]				= &CScanner::Get_upc_ean_PropertyValue;
	metaTagsGetFunctions[L"upcEanConvertGs1dataBarToUpcEan"]		= &CScanner::Get_upc_ean_PropertyValue;
	metaTagsGetFunctions[L"webcodeDecodeGTSubtype"]					= &CScanner::Get_webcode_PropertyValue;
	metaTagsGetFunctions[L"signatureImageQuality"]					= &CScanner::Get_signature_PropertyValue;
	metaTagsGetFunctions[L"signatureImageWidth"]					= &CScanner::Get_signature_PropertyValue;
	metaTagsGetFunctions[L"signatureImageHeight"]					= &CScanner::Get_signature_PropertyValue;
	metaTagsGetFunctions[L"signatureImageSize"]						= &CScanner::Get_signature_PropertyValue;
	metaTagsGetFunctions[L"upcEANSecurityLevel"]					= &CScanner::Get_upc_ean_PropertyValue;
	metaTagsGetFunctions[L"code128IsBT128ConcatMode"]				= &CScanner::Get_code128_PropertyValue;
	metaTagsGetFunctions[L"Code128SecurityLevel"]					= &CScanner::Get_code128_PropertyValue;
	metaTagsGetFunctions[L"code128other128"]						= &CScanner::Get_code128_PropertyValue;
	metaTagsGetFunctions[L"code128redundancy"]						= &CScanner::Get_code128_PropertyValue;
	metaTagsGetFunctions[L"upcEANBooklandFormat"]					= &CScanner::Get_upc_ean_PropertyValue;	
}

void CScanner::InitialiseDecoderStringMap()
{
	decoderStrings[(BYTE)*DECODER_UPCE0]		= L"upce0";
	decoderStrings[(BYTE)*DECODER_UPCE1]		= L"upce1";
	decoderStrings[(BYTE)*DECODER_UPCA]			= L"upca";
	decoderStrings[(BYTE)*DECODER_MSI]			= L"msi";
	decoderStrings[(BYTE)*DECODER_EAN8]			= L"ean8";
	decoderStrings[(BYTE)*DECODER_EAN13]		= L"ean13";
	decoderStrings[(BYTE)*DECODER_CODABAR]		= L"codabar";
	decoderStrings[(BYTE)*DECODER_CODE39]		= L"code39";
	decoderStrings[(BYTE)*DECODER_D2OF5]		= L"d2of5";
	decoderStrings[(BYTE)*DECODER_I2OF5]		= L"i2of5";
	decoderStrings[(BYTE)*DECODER_CODE11]		= L"code11";
	decoderStrings[(BYTE)*DECODER_CODE93]		= L"code93";
	decoderStrings[(BYTE)*DECODER_CODE128]		= L"code128";
	decoderStrings[(BYTE)*DECODER_PDF417]		= L"pdf417";
	decoderStrings[(BYTE)*DECODER_TRIOPTIC39]	= L"trioptic39";
	decoderStrings[(BYTE)*DECODER_MICROPDF]		= L"microPDF";
	decoderStrings[(BYTE)*DECODER_MACROPDF]		= L"macroPDF";
	decoderStrings[(BYTE)*DECODER_MAXICODE]		= L"maxicode";
	decoderStrings[(BYTE)*DECODER_DATAMATRIX]	= L"datamatrix";
	decoderStrings[(BYTE)*DECODER_QRCODE]		= L"qrCode";
	decoderStrings[(BYTE)*DECODER_MACROMICROPDF]= L"macroMicroPDF";
	decoderStrings[(BYTE)*DECODER_RSS14]		= L"rss14";
	decoderStrings[(BYTE)*DECODER_RSSLIM]		= L"rsslim";
	decoderStrings[(BYTE)*DECODER_RSSEXP]		= L"rssexp";
	decoderStrings[(BYTE)*DECODER_IMAGE]		= L"image";
	decoderStrings[(BYTE)*DECODER_SIGNATURE]	= L"signature";
	decoderStrings[(BYTE)*DECODER_DOCCAP]		= L"documentCapture";
	decoderStrings[(BYTE)*DECODER_WEBCODE]		= L"webcode";
	decoderStrings[(BYTE)*DECODER_COMPOSITE_AB] = L"compositeAb";
	decoderStrings[(BYTE)*DECODER_COMPOSITE_C]	= L"compositeC";
	decoderStrings[(BYTE)*DECODER_TLC39]		= L"tlc39";
	decoderStrings[(BYTE)*DECODER_USPOSTNET]	= L"usPostNet";
	decoderStrings[(BYTE)*DECODER_USPLANET]		= L"usPlanet";
	decoderStrings[(BYTE)*DECODER_UKPOSTAL]		= L"ukPostal";
	decoderStrings[(BYTE)*DECODER_JAPPOSTAL]	= L"japPostal";
	decoderStrings[(BYTE)*DECODER_AUSPOSTAL]	= L"ausPostal";
	decoderStrings[(BYTE)*DECODER_DUTCHPOSTAL]	= L"dutchPostal";
	decoderStrings[(BYTE)*DECODER_CANPOSTAL]	= L"canPostal";
	decoderStrings[(BYTE)*DECODER_CHINESE_2OF5] = L"chinese2of5";
	decoderStrings[(BYTE)*DECODER_AZTEC]		= L"aztec";
	decoderStrings[(BYTE)*DECODER_MICROQR]		= L"microQR";
	decoderStrings[(BYTE)*DECODER_KOREAN_3OF5]	= L"korean2of5";
	decoderStrings[(BYTE)*DECODER_US4STATE]		= L"us4state";
	decoderStrings[(BYTE)*DECODER_MATRIX_2OF5]	= L"matrix2of5";
	decoderStrings[(BYTE)*DECODER_US4STATE_FICS]= L"us4stateFics";
}

#pragma endregion

#pragma region Meta_Tag_Queue_Control

BOOL CScanner::AddMetaTagToQueue(HANDLE hQueueToAddTo, LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	MetaTagQEntry metaTag;
	if (szPropertyName){
		metaTag.szParameter = new WCHAR[wcslen(szPropertyName) + 1];
		wcscpy(metaTag.szParameter, szPropertyName);
	}
	else
		szPropertyName = NULL;

	if (szPropertyValue)
	{
		metaTag.szValue = new WCHAR[wcslen(szPropertyValue) + 1];
		wcscpy(metaTag.szValue, szPropertyValue);
	}
	else
		szPropertyValue = NULL;

	return WriteMsgQueue(hQueueToAddTo, &metaTag, sizeof(metaTag), 0, 0);
}


void CScanner::ActionPendingMetaTags(HANDLE hQueueToProcess, bool bParseTags)
{
	//  Read the pending Meta tags and resubmit them
	BOOL bMetaQReadValue, bProcessingMetaTags = TRUE;
	//PBMetaStruct defined in PBPlugin.h
	MetaTagQEntry metaTag;
	DWORD dwRead, dwFlags;
	if (bParseTags && 
		hQueueToProcess == m_hTagsOnCurrentPageQRead)
	{
		//  To prevent the TagsOnCurrentPage queue cyclically being populated
		//  put in a special entry to it
		AddMetaTagToQueue(m_hTagsOnCurrentPageQWrite, L"StopProcessing", L"StopProcessing");
	}
	while (bProcessingMetaTags)
	{
		bMetaQReadValue = 
			ReadMsgQueue(hQueueToProcess, &metaTag, 
			sizeof(MetaTagQEntry), &dwRead, 0, &dwFlags);
		if (bMetaQReadValue)
		{
			//  Successfully read a value from the queue of Meta Tags
			//  to parse.  Note if we are actioning the queue of tags on the current
			//  page we do not re-add them to the same queue, that is what the
			//  third parameter does below.
			if (wcscmp(metaTag.szParameter, L"StopProcessing") == 0 &&
				wcscmp(metaTag.szValue, L"StopProcessing") == 0)
			{
				bProcessingMetaTags = FALSE;
			}
			else
			{
				if (bParseTags)
					SetPropertyOrMethod(metaTag.szParameter, metaTag.szValue);
			}
			delete[] metaTag.szParameter;
			delete[] metaTag.szValue;
		}
		else
		{
			//  Failed to read a value from the queue, probably because
			//  there are no entries left to read
			bProcessingMetaTags = FALSE;
		}
	}
}

#pragma endregion

#pragma region Scanner_Control_Functions

BOOL CScanner::OpenScanner(LPCTSTR wScannerName)
{
	BOOL bReturnValue = FALSE;
	//  The request to the EMDK layer requires us to append a ':'
	WCHAR* wScannerToRequest = NULL;
	if (wScannerName != NULL && wcslen(wScannerName) > 0)
	{
		//  The user has specified a Scanner to Open
		wScannerToRequest = new WCHAR[wcslen(wScannerName) + 2];
		wsprintf(wScannerToRequest, L"%s:", wScannerName);
		if (m_szUserSpecifiedScanner)
			delete[] m_szUserSpecifiedScanner;
		m_szUserSpecifiedScanner = new WCHAR[wcslen(wScannerName) + 1];
		wcscpy(m_szUserSpecifiedScanner, wScannerName);
		if (FindScanner(wScannerToRequest, &m_bUsingBluetoothScanner, NULL))
		{
			//  The Scanner exists on the device, open it
			if (lpfn_SCAN_Open(wScannerToRequest, &m_hScanner) != E_SCN_SUCCESS)
			{
				m_hScanner = NULL;
				LOG(WARNING) + L"Failed to open Scanner";
			}
			else
			{
				if (m_szCurrentlyOpenScanner)
					delete[] m_szCurrentlyOpenScanner;
				m_szCurrentlyOpenScanner = new WCHAR[wcslen(wScannerName) + 1];
				wcscpy(m_szCurrentlyOpenScanner, wScannerName);
				bReturnValue = TRUE;
			}
		}
		else
		{
			WCHAR logMsg[MAXLOG];
			wsprintf(logMsg, L"Requested Scanner ID, %s, is not present on device", wScannerToRequest);
			LOG(WARNING) + logMsg;
		}
	}
	else
	{
		//  The user has not specified a Scanner to open, open the default
		//  scanner
		bool bDefaultScanOpenSuccess = false;
		if (m_szCurrentlyOpenScanner)
			delete[] m_szCurrentlyOpenScanner;
		m_szCurrentlyOpenScanner = new WCHAR[5];
		if (FindScanner(L"SCN1:", &m_bUsingBluetoothScanner, NULL))
		{
			bDefaultScanOpenSuccess = (lpfn_SCAN_Open(L"SCN1:", &m_hScanner) == E_SCN_SUCCESS);
			if (bDefaultScanOpenSuccess)
				wcscpy(m_szCurrentlyOpenScanner, L"SCN1");
		}
		else if (FindScanner(L"SCN2:", &m_bUsingBluetoothScanner, NULL))
		{
			bDefaultScanOpenSuccess = (lpfn_SCAN_Open(L"SCN2:", &m_hScanner) == E_SCN_SUCCESS);
			if (bDefaultScanOpenSuccess)
				wcscpy(m_szCurrentlyOpenScanner, L"SCN2");
		}
		if (!bDefaultScanOpenSuccess)
			LOG(WARNING) + L"Failed to open default scanner";

		bReturnValue = bDefaultScanOpenSuccess;
	}
	
	m_bScannerOpen = (bReturnValue == TRUE);
	if (wScannerToRequest)
		delete[] wScannerToRequest;
	if (!bReturnValue)
		m_bUsingBluetoothScanner = false;

	return bReturnValue;
}

//  Returns whether or not the specified Scanner is present on the device
//  bIsBluetoothScanner is an output parameter, set to true if the found scanner
//  is bluetooth.
BOOL CScanner::FindScanner(LPCTSTR wScannerName, bool* bIsBluetoothScanner, WCHAR* szFriendlyName)
{
	BOOL bReturnValue = FALSE;
	*bIsBluetoothScanner = false;

	SCAN_FINDINFO	scanFindInfo;
	memset(&scanFindInfo, 0, sizeof(scanFindInfo));
	scanFindInfo.StructInfo.dwAllocated = sizeof(scanFindInfo);
	HANDLE	scanFindHandle = NULL;

	DWORD dwResult = lpfn_SCAN_FindFirst(&scanFindInfo, &scanFindHandle);
	while(dwResult == E_SCN_SUCCESS)
	{
		//  Application has requested a specific scanner, check to see
		//  if this is the scanner requested
		if (!wcsicmp(wScannerName, scanFindInfo.szDeviceName))
		{
			//  Found the scanner requested by the user
			bReturnValue = TRUE;
			if (szFriendlyName)
			{
				wcscpy_s(szFriendlyName, 30, scanFindInfo.szFriendlyName);
			}
			//  Check to see if this is a Bluetooth scanner (these always
			//  start with the string 'Bluetooth' in their Friendly name
			if (wcsstr(scanFindInfo.szFriendlyName, L"Bluetooth") ||
				wcsstr(scanFindInfo.szFriendlyName, L"bluetooth"))
			{
				*bIsBluetoothScanner = true;
			}
			break;
		}
		dwResult = lpfn_SCAN_FindNext(&scanFindInfo, scanFindHandle);
	}
	lpfn_SCAN_FindClose(scanFindHandle);

	return bReturnValue;
}

BOOL CScanner::CloseScanner()
{
    //LOG(TRACE) + "Closing Scanner ...";
	//  Shut down all communication with the Scanner, 
	//  equivilent to ScanTerm() in PB3
	//  Free memory associated with the list of supported RSM Attributes
	//  which have not been returned to the user.
	delete[] m_rsmSupportedAttributes;
	m_rsmSupportedAttributes = NULL;
	for(int i=0; i < m_iCountOfMarkedRSMAttributes; i++)
	{
		delete[] m_szRSMValuesToReturn[i];
		m_szRSMValuesToReturn[i] = NULL;
	}
	m_iCountOfMarkedRSMAttributes = 0;
	m_bScannerOpen = false;
	m_bScannerEnabled = false;
	m_bScannerBeingEnabled = false;
	m_bTriggerConnected = true;
	if(m_bUsingBluetoothScanner)
	{
        //LOG(TRACE) + "Closing Bluetooth hidden windows...";
		SetBluetoothStatus(FALSE);
		//  Deregister for Windows Status notifications of Bluetooth events
		lpfn_SCAN_DeRegisterScanMessage(m_hScanner);
		//  Destroy the hidden window, listening for BT events from EMDK
        if (m_hiddenWindow)
        {
		    PostMessage(m_hiddenWindow, WM_CLOSE, 0, 0);
		    m_hiddenWindow = NULL;
        }
	}
	m_ScannerState = NO_BT_CONNECTION;
	m_bUsingBluetoothScanner = false;
//	if (m_szUserSpecifiedScanner)
//	{
//		delete[] m_szUserSpecifiedScanner;
//		m_szUserSpecifiedScanner = NULL;
//	}
	if (m_szCurrentlyOpenScanner)
	{
		delete[] m_szCurrentlyOpenScanner;
		m_szCurrentlyOpenScanner = NULL;
	}
	
	// If a scanner is open
	DWORD dwResult = E_SCN_SUCCESS;
	if (m_hScanner != NULL)
	{
		// Stop any active scan
		BOOL bSetTrigger = FALSE;
		lpfn_SCAN_SetSoftTrigger(m_hScanner, &bSetTrigger);
		// Disable the Scanner, flushing the pending read
		lpfn_SCAN_Flush(m_hScanner);
		dwResult = lpfn_SCAN_Disable(m_hScanner);
		
		if (dwResult == E_SCN_SUCCESS)
		{
			//  Deallocate the Scan Buffer
			if (m_lpScanBuffer != NULL)
			{
				// If the scan buffer is still pending, cancel the read
				if ( SCNBUF_GETSTAT(m_lpScanBuffer) == E_SCN_READPENDING )
				{
					lpfn_SCAN_CancelRead(m_hScanner, SCNBUF_GETREQID(m_lpScanBuffer));
				}
				// Deallocate the scan buffer
				lpfn_SCAN_DeallocateBuffer(m_lpScanBuffer);

				// Mark scan buffer unallocated
				m_lpScanBuffer = NULL;
			}
		}
	
		// Close the scanner
		dwResult = lpfn_SCAN_Close(m_hScanner);
		
		// Mark the scanner closed
		m_hScanner = NULL;
	}

	return (dwResult == E_SCN_SUCCESS);
}

BOOL CScanner::DisableScannerAndResetToDefault()
{
	//  When disabling the scanner EMDK automatically resets the Scanner
	//  settings back to default.
	return CloseScanner();
}

BOOL CScanner::EnableScanner()
{
	//  Creates the thread to enable the Scanner
	m_bScannerBeingEnabled = true;

	if (!m_bScannerOpen)
	{
		if (OpenScanner(m_szUserSpecifiedScanner) == FALSE)
		{
			LOG(ERROR) + "OpenScanner Failed";
			m_bScannerBeingEnabled = false;
			return FALSE;
		}
	}

    // the hidden for the bluetooth window has been mode to the thread spawned in the 
    // next line, becuae it will need a message pump.

	//  Enable the Scanner in a separate thread as this can block
	//  for Bluetooth Scanners
	CloseHandle(CreateThread(NULL, 0, &EnableScannerThread, (void*)this, 0, NULL));

	return TRUE;
}

BOOL CScanner::SubmitRead()
{
	//  Previously called GoScan()
	//  Enable the Scanner if it is not already enabled.
	if (!m_bScannerEnabled && !m_bScannerBeingEnabled)
	{	
		if (!EnableScanner())
		{
			LOG(WARNING) + "EnableScanner failed!";
			return FALSE;
		}
	}
	//  Setting Scanner parameters requires a read to not be pending, block here whilst the scanner parameters are set
	EnterCriticalSection(&m_scanner_prop_cs);
	LeaveCriticalSection(&m_scanner_prop_cs);

	//  Create a thread to submit the Scan.  If the user had previously disconnected the trigger we skip this step until
	//  the user reconnects the trigger.
	if (m_bTriggerConnected)
		CloseHandle(CreateThread(NULL, 0, &SubmitReadThread, (void*)this, 0, NULL));
	return TRUE;
}

BOOL CScanner::RetrieveDataFromScanBuffer()
{
	//  Put last scanned data in m_lastScannedData
	//  last scan data is stored in m_lpScanBuffer
	if (m_lpScanBuffer != NULL && !g_bShuttingDown)
	{
		//  If the user has requested a text barcode then just
		//  copy the data
		DWORD dataLen = (DWORD)SCNBUF_GETLEN(m_lpScanBuffer);
		if (SCNBUF_ISTEXT(m_lpScanBuffer))
			wcsncpy(m_lastScannedData.szData, (LPTSTR)SCNBUF_GETDATA(m_lpScanBuffer), m_iDataBufferSize);
		else
		{
			//  User has requested data to be read as binary, convert this
			//  to a Data URI before returning
			//  Converting to Base 64 will produce output 1/3 larger than the binary
			//  input
			char* szEncodedString = new char[dataLen * 2];
			memset(szEncodedString, 0, dataLen * 2);
			EncodeToBase64(SCNBUF_GETDATA(m_lpScanBuffer), SCNBUF_GETLEN(m_lpScanBuffer), szEncodedString);
			//  Because we are returning binary data the size of the receiving buffer will
			//  need to be enlargened to accommodate the extra space.
			//  30 is added for the "data:....." header for the encoding
			int iLengthOfReturnString = strlen(szEncodedString) + 30;
			delete[] m_lastScannedData.szData;
			m_lastScannedData.szData = new WCHAR[iLengthOfReturnString];
			if (!m_lastScannedData.szData)
			{
				LOG(ERROR) + L"Unable to allocate memory to return data buffer";
				delete[] szEncodedString;
				return FALSE;
			}			
			memset(m_lastScannedData.szData, 0, iLengthOfReturnString * sizeof(WCHAR));
			if (SCNBUF_GETLBLTYP(m_lpScanBuffer) == LABELTYPE_SIGNATURE)
			{
				//  User has scanned a barcode type which should be
				//  returned as Jpeg data
				//  Note: If scanner ever supports anything other than jpeg then
				//  this mime type will need to be changed.
				wcscpy(m_lastScannedData.szData, L"data:image/jpeg;base64,");
				mbstowcs(m_lastScannedData.szData + wcslen(L"data:image/jpeg;base64,"), 
					szEncodedString, iLengthOfReturnString);
			}
			else
			{
				//  User has scanned a barcode type which should be
				//  returned as text
				wcscpy(m_lastScannedData.szData, L"data:text/plain;base64,");
				mbstowcs(m_lastScannedData.szData + wcslen(L"data:text/plain;base64,"), 
					szEncodedString, iLengthOfReturnString);
			}
			delete[] szEncodedString;
		}

		SYSTEMTIME systime;
		wsprintf(m_lastScannedData.szLength, L"%d", dataLen);
		DWORD labelType = (DWORD)SCNBUF_GETLBLTYP(m_lpScanBuffer);
		wsprintf(m_lastScannedData.szType, L"0x%x", labelType);
		memset(m_lastScannedData.szSource, 0, MAX_SOURCE_SIZE * sizeof(TCHAR));
		//  Klocwork: error reported here and ignored as related to EMDK Macro
		wcsncpy(m_lastScannedData.szSource, SCNBUF_GETSRC(m_lpScanBuffer), MAX_SOURCE_SIZE - 1);
		if (m_lpScanBuffer->dwDirection == DIR_FORWARD)
			wcscpy(m_lastScannedData.szDirection, L"forward");
		else if (m_lpScanBuffer->dwDirection == DIR_REVERSE)
			wcscpy(m_lastScannedData.szDirection, L"reverse");
		else
			wcscpy(m_lastScannedData.szDirection, L"unavailable");

		//  Test to see if there is any auxiliary data associated with this barcode,
		//  eg. for Composite Barcodes
		if (m_lpScanBuffer->bIsMultiPart && SCNBUF_ISTEXT(m_lpScanBuffer))
		{
			//  Get the data in the auxiliary data area
			LPBYTE lpbyTemp = (LPBYTE)m_lpScanBuffer + m_lpScanBuffer->dwOffsetAuxDataBuff;
			DWORD dwAuxDataPackets = m_lpScanBuffer->dwNumRemaining;
			DWORD dwAuxiliaryDataLength = 0;
			for (DWORD dwIndex = 1; dwIndex <= dwAuxDataPackets; dwIndex++)
			{
				//  Get the total length of the auxiliary data 
				DWORD dwPacketLength = (MAKEWORD(lpbyTemp[2], lpbyTemp[1]));
				dwAuxiliaryDataLength += dwPacketLength;
				lpbyTemp += (3 + dwPacketLength);
			}
			//  Create a string to hold the auxiliary data
			lpbyTemp = (LPBYTE)m_lpScanBuffer + m_lpScanBuffer->dwOffsetAuxDataBuff;
			WCHAR* szAuxiliaryData = new WCHAR[dwAuxiliaryDataLength + 1];
			memset(szAuxiliaryData, 0, (dwAuxiliaryDataLength + 1) * 2);
			//  Copy Auxiliary Data to String
			DWORD dwPacketDataLocation = 0;
			for (DWORD dwIndex = 1; dwIndex <= dwAuxDataPackets; dwIndex++)
			{
				//  Untested with more than one Aux Packet
				DWORD dwPacketLength = (MAKEWORD(lpbyTemp[2], lpbyTemp[1]));
				MultiByteToWideChar(
					CP_OEMCP, 
					MB_PRECOMPOSED,
					(LPCSTR)&lpbyTemp[3], 
					dwPacketLength,
					(LPWSTR)&szAuxiliaryData[dwPacketDataLocation], 
					dwPacketLength * 2);

				// Move to the next packet
				lpbyTemp += (3 + dwPacketLength);
				dwPacketDataLocation += dwPacketLength;
			}
			//  Append the Auxiliary string to the end of the Scanned Data 
			//  if there is space.
			//  Klocwork: Error reported here and ignored as KW doesn't understand that if buffer
			//  was too small (m_iDataBufferSize) then EMDK would have thrown an error before we get here.
			if (wcslen(m_lastScannedData.szData) + wcslen(szAuxiliaryData) < m_iDataBufferSize)
				wcscat(m_lastScannedData.szData, szAuxiliaryData);
			else
				m_decodeNotifier->BadDecode();
			delete[] szAuxiliaryData;
		}
		
		// If the time stamp field was filled in
		if ( SI_FIELD_VALID(m_lpScanBuffer, TimeStamp) )
		{
			// Get time stamp
			systime = m_lpScanBuffer->TimeStamp;

			// Format it
			wsprintf(m_lastScannedData.szTimeStamp,
						TEXT("%02d:%02d:%02d"),
						systime.wHour,
						systime.wMinute,
						systime.wSecond);
		}
	}
	return TRUE;
}

BOOL CScanner::ResetScanBuffer()
{
	//  Buffer will be reallocated at next read request
	BOOL bRetVal = lpfn_SCAN_DeallocateBuffer(m_lpScanBuffer);
	m_lpScanBuffer = NULL;
	return bRetVal;
}

//  Changes the type of the next scanner read (foreground / background / monitor)
BOOL CScanner::ChangeReadType(tagSCANTYPE scanType)
{
	m_currentScanType = scanType;
	SCAN_PARAMS scan_params;
	memset(&scan_params, 0, sizeof(scan_params));
	SI_INIT(&scan_params);
	DWORD dwResult = lpfn_SCAN_GetScanParameters(m_hScanner, &scan_params);
	if (dwResult != E_SCN_SUCCESS)
		return FALSE;
	scan_params.dwScanType = scanType;
	dwResult = lpfn_SCAN_SetScanParameters(m_hScanner, &scan_params);
	if (dwResult != E_SCN_SUCCESS)
		return FALSE;
	else
		return TRUE;
}


#pragma endregion

#pragma region User_Interface_Routines

void CScanner::SetBluetoothStatus(BOOL bConnected)
{
   
	//  Only expect to become Connected when we are in the NO_BT_CONNECTION
	//  or the WAITING_FOR_BT_RECONNECTION states.
	//  Only expect to become Disconnected when we are in the BT_CONNECTED state or 
	//  when waiting for the user to scan the association barcode
	if ((bConnected && 
		(m_ScannerState == NO_BT_CONNECTION || m_ScannerState == WAITING_FOR_BT_RECONNECTION 
		|| m_ScannerState == WAITING_FOR_BT_CONNECTION))
		||
		(!bConnected && (m_ScannerState == BT_CONNECTED || m_ScannerState == WAITING_FOR_BT_CONNECTION)))
	{
        //LOG(INFO) + L"BT event recieved";
		//  The state transition is legal, action it.
		if (bConnected)
		{
			//  Scanner has become connected
			//  Set the application State to Bluetooth Connected
			ScannerState stateBeforeConnection = m_ScannerState;
			m_ScannerState = BT_CONNECTED;

			//  Call GoScan() if this is a _Reconnection_, not for the first connection.
			//  There is precidence for calling GoScan from the main event queue.
			//if (stateBeforeConnection == WAITING_FOR_BT_RECONNECTION)
			//	EnableScanner();

			//  Set the Connected String to Connected.  Use of a critical section
			//  ensures the m_szBluetoothStatus variable is updated only once
			wcscpy(m_szBluetoothStatus, L"BluetoothConnected");
		}
		else
		{
			//  Scanner has become disconnected (Transitioned from BT_CONNECTED to
			//  here).
			//  Set the application State to Waiting for Reconnection
			m_ScannerState = WAITING_FOR_BT_RECONNECTION;

			//  Set the Connected String to Disconnected.  Use of a critical section
			//  ensures the m_szBluetoothStatus variable is updated only once
			wcscpy(m_szBluetoothStatus, L"BluetoothDisconnected");
		}

		//  Inform the User of the Connect / Disconnect in a separate thread
		HANDLE hThread;
		hThread = CreateThread(NULL, 0, &InformUserOfBTEvent, (void*)this, 0, NULL);
		CloseHandle(hThread);


	}
    else if (bConnected && (m_ScannerState == BT_CONNECTED))
    {
        // This is not part of this SR, but it was observed that sometimes even if the scanner is connectd
        // messages seem to be coming saying its connected. why should we close the scanner 
        //if we get a message saying if the scanner is connected
        // eventhough its already connected. I am not sure if there can be any condition
        return;
    }
	else
	{
		//  Unexpected State Transition, this will be a case for debugging.
		//  This occurs sometimes if we take the battery off the Scanner as the EMDK 
		//  doesn't always seem to notify us via a Windows Message.
        TCHAR szLogMsg[MAX_PATH] = {0};
        wsprintf(szLogMsg, L"Unexpected Scanner BT State Transition, Disabling Scanner %d, %d", bConnected, m_ScannerState);
        LOG(WARNING) + szLogMsg;

		m_bUsingBluetoothScanner = false;
		CloseScanner();
	}
}


//  szDecodeEvent can not be NULL
/*BOOL CScanner::SetScannerEvent(LPCTSTR szUserDefinedEvent, WCHAR** szScannerMemberVariable)
{
	if (!szUserDefinedEvent)
		return FALSE;
	if (*szScannerMemberVariable)
		delete[] *szScannerMemberVariable;
	*szScannerMemberVariable = new WCHAR[(wcslen(szUserDefinedEvent) * sizeof(WCHAR)) + 1];
	wcscpy(*szScannerMemberVariable, szUserDefinedEvent);
	return TRUE;
}
*/
#pragma endregion

#pragma region Threads


//  Thread to enable the Scanner and connect to a BT scanner as 
//  appropriate
DWORD CScanner::EnableScannerThread(LPVOID lParam)
{
	CScanner* pScanner = (CScanner*)lParam;
	DWORD dwReturnValue = E_SCN_SUCCESS;

    // the hidden for the bluetooth window has been mode in here from the Enable scanner 
    // method, becuae it will need a message pump and that will keep the thraed blocked
    if(pScanner->m_bUsingBluetoothScanner)
	{
		if (NULL == pScanner->m_hiddenWindow)
		{
            LOG(TRACE) + "Create the bluetooth hidden window...";
			pScanner->m_hiddenWindow = CreateWindow(pScanner->m_szHiddenWindowName,	NULL, 0, 0, 0, 0, 0, NULL, (HMENU)0, NULL, NULL);

            if (pScanner->m_hiddenWindow)
            {
			    SetWindowLong(pScanner->m_hiddenWindow, GWL_USERDATA, (LONG)pScanner);
                //LOG(TRACE) + "Bluetooth hidden window created...";

            }
			//SetWindowLong(m_hiddenWindow, GWL_WNDPROC, (DWORD)HiddenWindowProc);
		}


		//  Test to see whether the user has requested to display the BT address barcode
		//  on enable or not
		if (pScanner->m_iDisplayBTPairBarcode > BT_PAIRING_BARCODE_INITIAL ||
			pScanner->m_dwEnableTimeout > -1)
		{
			INTERFACE_PARAMS interfaceParams;
			pScanner->RetrieveScannerInterfaceParameters(&interfaceParams);
			if (pScanner->m_iDisplayBTPairBarcode > BT_PAIRING_BARCODE_INITIAL)
				interfaceParams.InterfaceSpecific.SsiSpecific.bDisplayBTAddressBarcode = pScanner->m_iDisplayBTPairBarcode;
			if (pScanner->m_dwEnableTimeout > 0)
				interfaceParams.InterfaceSpecific.SsiSpecific.dwEstablishConnectionTime = pScanner->m_dwEnableTimeout;
			BOOL bRetVal = pScanner->SetScannerInterfaceParameters(&interfaceParams, sizeof(interfaceParams));
			if (!bRetVal)
				LOG(WARNING) + L"Unable to configure Bluetooth scanner configuration settings";

		}
	}

	pScanner->m_ScannerState = NO_BT_CONNECTION;

	if(pScanner->m_bUsingBluetoothScanner)
	{
		wcscpy(pScanner->m_szBluetoothStatus, L"BTScanAssociationBarcode");
		CloseHandle(CreateThread(NULL, 0, &InformUserOfBTEvent, (void*)pScanner, 0, NULL));
		pScanner->m_ScannerState = WAITING_FOR_BT_CONNECTION;
	}

	dwReturnValue = pScanner->lpfn_SCAN_Enable(pScanner->m_hScanner);
	if (dwReturnValue != E_SCN_SUCCESS)
	{
		// Scanner did not enable correctly, terminate scanning.
		pScanner->CloseScanner();
	}
	else
	{
		if(pScanner->m_bUsingBluetoothScanner)
		{
			pScanner->SetBluetoothStatus(TRUE);
			//  Register to receive BT Status event via a Windows message (this is the only way to do it)
			//  In EnableScanner() we created our own hidden window, associated with this
			//  scanner instance.
			DWORD dwResult = pScanner->lpfn_SCAN_RegisterScanMessage(pScanner->m_hScanner, 
				pScanner->m_hiddenWindow, WM_SCAN_EVENT_RHOELEMENTS);
			if (dwResult != E_SCN_SUCCESS)
			{
				LOG(WARNING) + L"Failed to register for BT Notification messages, BT status reporting may be inaccurate";
			}
		}
		
		//  Configure Scanner Parameters
		SCAN_PARAMS scan_params;
		memset(&scan_params, 0, sizeof(scan_params));
		SI_INIT(&scan_params);
		DWORD dwResult = pScanner->lpfn_SCAN_GetScanParameters(pScanner->m_hScanner, &scan_params);
		if (dwResult != E_SCN_SUCCESS)
			return dwResult;
		//  Disable the Beep and Wave at the Scanner Layer.  We do it manually in DecodeBeep()
		//  This is legacy behaviour which is being maintained in RhoElements as it will 
		//  better encapsulate a Scanner user.
		SI_SET_IF_CHANGED(&scan_params, dwCodeIdType, 0);
		SI_SET_IF_CHANGED(&scan_params, dwDecodeBeepTime, 0);
		WCHAR wWaveFile[1];
		wWaveFile[0] = 0;
		SI_SET_STRING(&scan_params, szWaveFile, wWaveFile);
		scan_params.dwScanType = pScanner->m_currentScanType;
		dwResult = pScanner->lpfn_SCAN_SetScanParameters(pScanner->m_hScanner, &scan_params);
		if (dwResult != E_SCN_SUCCESS)
			return dwResult;

		//  Scanner Initialised, get supported RSM attributes
		pScanner->RSMGetSupportedAttributes();

		//  Enable the Scanner
		pScanner->m_bScannerBeingEnabled = false;
		pScanner->m_bScannerEnabled = true;

		//  Action any tags which were pending our scanner being enabled
		if (!pScanner->m_bCurrentlyNavigating)
			pScanner->ActionPendingMetaTags(pScanner->m_hMetaMsgQRead, true);

		pScanner->SubmitRead();

        // keep the message loop for the hiddden window to recieve any kind of posted 
        // messages or send messages from another thread. For bluetooth scanners
        // this thread will live till the scanner is closed. Looks the now the WM_CLOSE
        // can reach the scanner
        //LOG(TRACE) + "Start the bluetooth message loop.";
        if(pScanner->m_bUsingBluetoothScanner)
	    {
            MSG msg;
            while (NULL != ::GetMessage(&msg, NULL, 0, 0))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            LOG(TRACE) + "Exit the bluetooth message listening thread.";
        }        

	}
	return dwReturnValue;
}


DWORD CScanner::SubmitReadThread(LPVOID lParam)
{
	CScanner* pScanner = (CScanner*)lParam;
	SetEvent(pScanner->m_hEnableForTakeComplete);
	DWORD dwResult;
	if (pScanner->m_lpScanBuffer == NULL)
	{
		//  Scan buffer is unallocated, allocate it
		pScanner->m_lpScanBuffer = pScanner->lpfn_SCAN_AllocateBuffer(
			pScanner->m_bufferFormat, pScanner->m_iDataBufferSize);
		if (pScanner->m_lpScanBuffer == NULL)
		{
			//  Some memory error allocating the Scanner buffer
			LOG(WARNING) + L"Unable to allocate enough memory for Scanning";
			return E_SCN_NOTENOUGHMEMORY;
		}
		else
			pScanner->m_lpScanBuffer->dwTimeout = 0;  //  Set timeout to 0 as we control it via scanTimeout meta tag
	}
	
	ResetEvent(pScanner->m_hReadComplete);
	//  Scan buffer has been allocated, submit the read event
	dwResult = pScanner->lpfn_SCAN_ReadLabelEvent(pScanner->m_hScanner, 
										pScanner->m_lpScanBuffer,
										pScanner->m_hReadComplete,
										0, &pScanner->m_dwRequestID);

	//  If the read failed disable the Scanner
	if (dwResult != E_SCN_SUCCESS)
	{
		//  Handle a BT Disconnection here, we don't want to call ScanTerm()
		//  for a BT Disconnection as this would require the user to 
		//  reassociate the scanner with the device.
		if (!(pScanner->m_ScannerState == BT_CONNECTED || 
			pScanner->m_ScannerState == WAITING_FOR_BT_RECONNECTION))
		{
			//  Scanner is not connected via Bluetooth, perform existing
			//  shutting down behaviour.
			LOG(WARNING) + L"Scanner Read failed, disabling scanner";
			pScanner->CloseScanner();
		}
	}
	else
	{
		//  The read request was successful, wait for the data
		//  Check we're not shutting down before we continue processing
		dwResult = WaitForSingleObject(pScanner->m_hReadComplete, INFINITE);
		ResetEvent(pScanner->m_hReadComplete);
		if (!pScanner->g_bShuttingDown && 
			pScanner->m_bScannerEnabled && dwResult == E_SCN_SUCCESS)
		{
			if (pScanner->m_bCurrentlyNavigating)
			{
				//  If we are in the process of navigating then indicate a bad decode
				//  and do not notify the application.
				LOG(INFO) + L"Attempted to Scan a barcode but data ignored as a navigation is in progress";
				pScanner->m_decodeNotifier->BadDecode();
				//  return early here if we experience a scan during navigation, customer reports
				//  that this is more reliable.
				if (!pScanner->m_bLegacyNavBehaviour)
				{
					pScanner->m_bScannedDuringNavigate = true;
//					pScanner->CloseScanner();	//  Closing the scanner here is bad as it's already being closed for the navigate
					return dwResult;
				}
			}
			else if (pScanner->m_lpScanBuffer != NULL && 
				SCNBUF_GETSTAT(pScanner->m_lpScanBuffer) == E_SCN_SUCCESS)
			{
				//  We have successfully read data from the scanner.
				//  Only do anything if the Scan buffer is not NULL
				pScanner->RetrieveDataFromScanBuffer();
				//  Beep that we made a good scan
				pScanner->m_decodeNotifier->GoodDecode();
				
				//  Call the callback function with Scanned data.  If we have specified a Take
				//  callback then this takes priority over a decode event, since a decode event
				//  can persist but take is a one short affair.
				if (pScanner->m_pTakeCallback.hasCallback())
				{
					rho::Hashtable<rho::String, rho::String> scannedData;
					scannedData.put( rho::common::convertToStringA(L"barcode"), 
						rho::common::convertToStringA(pScanner->m_lastScannedData.szData) );
					scannedData.put( rho::common::convertToStringA(L"status"), 
						rho::common::convertToStringA(L"ok") );
					pScanner->m_pTakeCallback.set(scannedData);
				}
				else if (pScanner->m_pDecodeEvent.hasCallback())
				{
					rho::Hashtable<rho::String, rho::String> scannedData;
					scannedData.put( rho::common::convertToStringA(L"data"), 
						rho::common::convertToStringA(pScanner->m_lastScannedData.szData) );
					scannedData.put( rho::common::convertToStringA(L"source"), 
						rho::common::convertToStringA(pScanner->m_lastScannedData.szSource) );
					scannedData.put( rho::common::convertToStringA(L"type"), 
						rho::common::convertToStringA(pScanner->m_lastScannedData.szType) );
					scannedData.put( rho::common::convertToStringA(L"time"), 
						rho::common::convertToStringA(pScanner->m_lastScannedData.szTimeStamp) );
					scannedData.put( rho::common::convertToStringA(L"length"), 
						rho::common::convertToStringA(pScanner->m_lastScannedData.szLength) );
					scannedData.put( rho::common::convertToStringA(L"direction"), 
						rho::common::convertToStringA(pScanner->m_lastScannedData.szDirection) );
					pScanner->m_pDecodeEvent.set(scannedData);
				}
				else if (SCNBUF_ISTEXT(pScanner->m_lpScanBuffer))
				{
					// User has not specified a decode event, output key strokes if the buffer was text
					pScanner->m_keyInjector->SendString(pScanner->m_lastScannedData.szData);
					if (pScanner->m_bAutoEnter)
						pScanner->m_keyInjector->SendKey(VK_RETURN);
					else if (pScanner->m_bAutoTab)
						pScanner->m_keyInjector->SendKey(VK_TAB);
					}
			}
			else if(pScanner->m_lpScanBuffer && SCNBUF_GETSTAT(pScanner->m_lpScanBuffer) == E_SCN_READINCOMPATIBLE)
			{
				LOG(ERROR) + L"The last selected setting(s) can not be applied to the scanner whilst it is Enabled, Disabling Scanner";
				pScanner->CloseScanner();
			}
			else if (pScanner->m_lpScanBuffer && SCNBUF_GETSTAT(pScanner->m_lpScanBuffer) == E_SCN_NOTENABLED)
			{
				LOG(ERROR) + L"The scan did not succeed as the Scanner is not enabled";
				pScanner->CloseScanner();
			}
			else if (pScanner->m_lpScanBuffer && SCNBUF_GETSTAT(pScanner->m_lpScanBuffer) == E_SCN_INVALIDHANDLE)
			{
				LOG(ERROR) + L"The scanner handle has returned an invalid status, please try re-enabling.";
				pScanner->CloseScanner();
			}
			else if(pScanner->m_lpScanBuffer && SCNBUF_GETSTAT(pScanner->m_lpScanBuffer) == E_SCN_BUFFERTOOSMALL)
			{
				LOG(WARNING) + L"The barcode was successfully scanned but the data was too big for the data buffer";
				pScanner->m_decodeNotifier->BadDecode();
			}
			else if(pScanner->m_lpScanBuffer && SCNBUF_GETSTAT(pScanner->m_lpScanBuffer) == E_SCN_DEVICEFAILURE)
			{
				LOG(ERROR) + L"Scanner Device Failure.  Is the scanner is already paired? or already in use? Try un-pairing";
				pScanner->CloseScanner();
			}
			else if (pScanner->m_lpScanBuffer != NULL && 
				SCNBUF_GETSTAT(pScanner->m_lpScanBuffer) != E_SCN_SUCCESS && 
				SCNBUF_GETSTAT(pScanner->m_lpScanBuffer) != E_SCN_READCANCELLED)
			{
				WCHAR szLog[256];
                long liStatus = SCNBUF_GETSTAT(pScanner->m_lpScanBuffer);
                wsprintf(szLog, L"The barcode was successfully scanned but the scan buffer has an invalid status: 0x%08x", liStatus);
                LOG(WARNING) + szLog;
                pScanner->m_decodeNotifier->BadDecode();
			}
			else
			{
				//  Data was successfully read but the Scan buffer was NULL, do 
				//  nothing as it could have been caused by a cancel read / scanner flush.
			}
			//  Continually read from the scanner until it is disabled. 
			if (pScanner->m_bResetScanBuffer)
			{
				pScanner->ResetScanBuffer();
				pScanner->m_bResetScanBuffer = false;
			}
			if (pScanner->m_bScannerEnabled && pScanner->m_bTriggerConnected)
				pScanner->SubmitRead();
		}
	}
	return dwResult;
}


//  Send the Device Capabilities (from SCAN_GetDeviceInfo and SCAN_GetVersionInfo) to the application
DWORD CScanner::DeviceCapabilitiesToApplication(LPVOID lParam)
{
	CScanner* pScanner = (CScanner*)lParam;
	DEVICE_INFO device_info;
	memset(&device_info, 0, sizeof(device_info));
	SI_INIT(&device_info);
	SCAN_VERSION_INFO version_info;
	memset(&version_info, 0, sizeof(version_info));
	SI_INIT(&version_info);

	DWORD retVal = pScanner->lpfn_SCAN_GetDeviceInfo(pScanner->m_hScanner, &device_info);
	if (retVal != E_SCN_SUCCESS)
	{
		LOG(WARNING) + L"Unable to retrieve device capabilities";
	}
	else
	{
		//  Successfully read the Device Info, try and read the Version Info
		retVal = pScanner->lpfn_SCAN_GetVersionInfo(pScanner->m_hScanner, &version_info);
		if (retVal != E_SCN_SUCCESS)
		{
			LOG(WARNING) + L"Unable to retrieve version information";
		}
		else
		{
			//  Supported Decoders
			WCHAR szDecodersArray[(MAX_DECODERS * MAX_DECODER_NAME_LENGTH) + 1];
			memset(szDecodersArray, 0, ((MAX_DECODERS * MAX_DECODER_NAME_LENGTH) + 1) * 2);
			pScanner->RetrieveSupportedDecoders(szDecodersArray);

			//  Supported Beam Widths
			//  TODO: MEMORY LEAK HERE BUT THIS CODE IS NEVER EXERCISED SO I HAVE NOT RESOLVED THIS.
			WCHAR* szSupportedBeamWidths = new WCHAR[35];
			wcscpy(szSupportedBeamWidths, L"new Array('normal',");
			if (device_info.dwSupportedBeamWidths & BEAM_WIDTH_NARROW)
				wcscat(szSupportedBeamWidths, L"'narrow',");
			if (device_info.dwSupportedBeamWidths & BEAM_WIDTH_WIDE)
				wcscat(szSupportedBeamWidths, L"'wide',");
			//  Remove the final ','
			szSupportedBeamWidths[wcslen(szSupportedBeamWidths) - 1] = L')';

			//  Aim Mode
			WCHAR szAimModeSupported[6];
			if (device_info.bAimMode)
				wcscpy(szAimModeSupported, L"true");
			else
				wcscpy(szAimModeSupported, L"false");

			//  Scan Direction Reporting
			WCHAR szScanDirectionSupported[6];
			if (device_info.bDirection)
				wcscpy(szScanDirectionSupported, L"true");
			else
				wcscpy(szScanDirectionSupported, L"false");

			//  Supported Image Formats
			//  TODO: MEMORY LEAK HERE BUT THIS CODE IS NEVER EXERCISED SO I HAVE NOT RESOLVED THIS.
			WCHAR* szSupportedImageFormats = new WCHAR[35];
			wcscpy(szSupportedImageFormats, L"new Array(");
			if (device_info.dwSupportedImageFormats & IMAGE_FORMAT_JPEG)
				wcscat(szSupportedImageFormats, L"'jpeg',");
			//  Remove the final ','
			if (szSupportedImageFormats[wcslen(szSupportedImageFormats) - 1] == L',')
				szSupportedImageFormats[wcslen(szSupportedImageFormats) - 1] = L')';
			else
				wcscat(szSupportedImageFormats, L")");

			//  Maximum Image Resolution
			//  TODO: MEMORY LEAK HERE BUT THIS CODE IS NEVER EXERCISED SO I HAVE NOT RESOLVED THIS.
			WCHAR* szMaximumImageRectangle = new WCHAR[35];
			wsprintf(szMaximumImageRectangle, L"new Array('%i','%i','%i','%i')", 
				device_info.MaxImageRect.left,
				device_info.MaxImageRect.right,
				device_info.MaxImageRect.top,
				device_info.MaxImageRect.bottom);

			//  DPM Available
			WCHAR szDpmAvailable[6];
			if (device_info.dwDPMCapable == DPM_ENABLED)
				wcscpy(szDpmAvailable, L"true");
			else
				wcscpy(szDpmAvailable, L"false");

			//  Viewfinder Supported
			WCHAR szViewfinderSupported[6];
			if (device_info.bViewfinder)
				wcscpy(szViewfinderSupported, L"true");
			else
				wcscpy(szViewfinderSupported, L"false");

			//  Adaptive Scanning Supported
			WCHAR szAdaptiveScanningSupported[6];
			if (device_info.bAdaptiveScanning)
				wcscpy(szAdaptiveScanningSupported, L"true");
			else
				wcscpy(szAdaptiveScanningSupported, L"false");

			//  Interface Type
			WCHAR szInterfaceType[30];
			INTERFACE_PARAMS interfaceParams;
			pScanner->RetrieveScannerInterfaceParameters(&interfaceParams);
			if (interfaceParams.dwInterfaceType == INTERFACE_TYPE_QSNAC)
				wcscpy(szInterfaceType, L"QSNAC Interface");
			else if (interfaceParams.dwInterfaceType == INTERFACE_TYPE_SSI)
				wcscpy(szInterfaceType, L"SSI Interface");
			else if (interfaceParams.dwInterfaceType == INTERFACE_TYPE_LS48XX)
				wcscpy(szInterfaceType, L"LS48XX Interface");
			else if (interfaceParams.dwInterfaceType == INTERFACE_TYPE_LIGHTHOUSE)
				wcscpy(szInterfaceType, L"Lighthouse ASIC");
			else if (interfaceParams.dwInterfaceType == INTERFACE_TYPE_CAMSCAN)
				wcscpy(szInterfaceType, L"CamScan Interface");

			//  Scanner Version
			WCHAR szHardwareVersion[16];
			WCHAR szDecoderVersion[32];
			WCHAR szPhysicalDeviceVersion[16];
			WCHAR szModuleDeviceVersion[16];
			WCHAR szAPIVersion[16];
			wsprintf(szHardwareVersion, L"%02i.%02i",
				((version_info.dwHardwareVersion & 0xFFFF0000) >> 16),
				version_info.dwHardwareVersion & 0xFFFF);
			wsprintf(szDecoderVersion,L"%02i.%02i.%02i.%02i",
				((version_info.dwDecoderVersion & 0xFFFF0000) >> 16),
				version_info.dwDecoderVersion & 0xFFFF,
				((version_info.dwDecoderBuildVersion & 0xFFFF0000) >> 16),
				version_info.dwDecoderBuildVersion & 0xFFFF);
			wsprintf(szPhysicalDeviceVersion, L"%02i.%02i",
				((version_info.dwPddVersion & 0xFFFF0000) >> 16),
				version_info.dwPddVersion & 0xFFFF);
			wsprintf(szModuleDeviceVersion, L"%02i.%02i",
				((version_info.dwMddVersion & 0xFFFF0000) >> 16),
				version_info.dwMddVersion & 0xFFFF);
			wsprintf(szAPIVersion, L"%02i.%02i",
				((version_info.dwCAPIVersion & 0xFFFF0000) >> 16),
				version_info.dwCAPIVersion & 0xFFFF);

			if(pScanner->m_pDeviceCapabilities.hasCallback())
			{
				//  TODO: Device Capabilities Callback
//				pScanner->m_pModule->SendPBNavigate(szDeviceCapabilitiesEventNames, pScanner->m_iInstanceID,
//					pScanner->m_szEventDeviceCapabilities, 
//					szDecodersArray, 
//					szSupportedBeamWidths,
//					szAimModeSupported,
//					szScanDirectionSupported,
//					szSupportedImageFormats,
//					szMaximumImageRectangle,
//					szDpmAvailable,
//					szViewfinderSupported,
//					szAdaptiveScanningSupported,
//					szInterfaceType,
//					szHardwareVersion,
//					szDecoderVersion,
//					szPhysicalDeviceVersion,
//					szModuleDeviceVersion,
//					szAPIVersion,
//					NULL);
			}
			delete[] szSupportedImageFormats;
			delete[] szSupportedBeamWidths;
			delete[] szMaximumImageRectangle;
		}
	}
	return 0;
}

DWORD CScanner::InformUserOfBTEvent(LPVOID lParam)
{
	CScanner* pScanner = (CScanner*)lParam;
	//  Notify the user of the bluetooth event using either the BluetoothStatusEvent or
	//  (for backward compatibility) the DecodeEvent
	WCHAR* wBTStatus = new WCHAR[wcslen(pScanner->m_szBluetoothStatus) + 1];
	memset(wBTStatus, 0, (wcslen(pScanner->m_szBluetoothStatus) + 1) * sizeof(WCHAR));
	wcscpy(wBTStatus, pScanner->m_szBluetoothStatus);
		
	//  First test to see if there's a Bluetooth Status Event
	if (pScanner->m_pBluetoothStatusEvent.hasCallback())
	{
		rho::Hashtable<rho::String, rho::String> btStatus;
		btStatus.put(rho::common::convertToStringA(L"status"), 
						rho::common::convertToStringA(wBTStatus) );
		pScanner->m_pBluetoothStatusEvent.set(btStatus);
	}
	else
	{
		WCHAR szLog[256];
		wsprintf(szLog, 
			L"A Bluetooth Status event occurred (%s) but the Bluetooth status event has not been registered for",
			wBTStatus);
		LOG(INFO) + szLog;
	}
	delete[] wBTStatus;
	return 0;
}
/**
* Developer - Abhineet Agarwal
* Checking Low Battery Status
* If LowBatteryScan is false, then disable the scanner, if battery is Low or Critical.
* else do nothing.
*/
VOID CScanner::StartOrDisableCheckingLowBattery(){
	static bool isCheckingStarted = false;
	if (m_bLowBatteryScan && isCheckingStarted){
		isCheckingStarted = false;
		LOG(INFO) + L"Terminating LowBatteryScan check.";
		//Low Battery Scan - True, No effect to scanner, if battery is low or critical...
		//Terminate thread if running.....
		POWER_BROADCAST PwrBcast[64];
		WriteMsgQueue(m_hLowBatteryScanMessageQueueWrite, &PwrBcast, 64, INFINITE, 0);
		StopPowerNotifications(m_hPowerNotifications);
		CloseMsgQueue(m_hLowBatteryScanMessageQueueRead);
		CloseMsgQueue(m_hLowBatteryScanMessageQueueWrite);
	}		
	else if(!m_bLowBatteryScan && !isCheckingStarted){
		isCheckingStarted = true;
		LOG(INFO) + L"Starting LowBatteryScan check.";
		//Low Battery Scan - False, Disable scanner, if battery is low or critical...
		//Start low battery thread, if not running and disable the scanner if the battery is low or critical....
		MSGQUEUEOPTIONS msgqOpt;
		msgqOpt.dwSize = sizeof(MSGQUEUEOPTIONS);
		msgqOpt.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		msgqOpt.dwMaxMessages = 0;
		msgqOpt.cbMaxMessage = 64;
		msgqOpt.bReadAccess = TRUE;
		WCHAR powerQName[16];
		wsprintf(powerQName, L"NeonPw4Q%i", m_iInstanceID2);
		m_hLowBatteryScanMessageQueueRead = CreateMsgQueue(powerQName, &msgqOpt);
		msgqOpt.bReadAccess = FALSE;
		m_hLowBatteryScanMessageQueueWrite = CreateMsgQueue(powerQName, &msgqOpt);
		if (m_hLowBatteryScanMessageQueueRead != NULL)
		{
			m_hPowerNotifications = RequestPowerNotifications(m_hLowBatteryScanMessageQueueRead,PBT_POWERINFOCHANGE);
			HANDLE handleToLowBatteryThread = CreateThread(NULL, 0, &LowBatteryScanThread, (void*)this, 0, NULL);
			if(handleToLowBatteryThread != NULL){
				CloseHandle(handleToLowBatteryThread);
				handleToLowBatteryThread = NULL;
			}else{
				wchar_t errorMsg[100];
				wsprintf(errorMsg,L"Unable to create LowBatteryScanThread. Error code is %ld.",GetLastError());
				LOG(INFO) + errorMsg;
			}
		}
	}
}

//  Thread responsible for monitoring the available battery power and if it gets too
//  low the Scanner will be disabled
DWORD CScanner::LowBatteryScanThread(LPVOID lParam)
{
	CScanner* pScanner = (CScanner*)lParam;
	LOG(INFO) + L"Starting Thread to monitor power state and disable Scanner if power gets low.";
	
	BOOL bQueueRead;
	POWER_BROADCAST PwrInfo[64];
	DWORD dwRead, dwFlags;
	//Modified by - Abhineet Agarwal
	while (!pScanner->m_bLowBatteryScan)
	{
		bQueueRead = ReadMsgQueue(pScanner->m_hLowBatteryScanMessageQueueRead, &PwrInfo, 64, &dwRead, INFINITE, &dwFlags);
		if (pScanner->g_bShuttingDown){
			break;
		}
		else
		{
			//  We have received a power notification, disable the scanner if it is a low power notification
			POWER_BROADCAST_POWER_INFO *pBPwrInfo = (POWER_BROADCAST_POWER_INFO*)PwrInfo->SystemPowerState;
			//Modified by Abhineet Agarwal, the battery flag need to be checked whether it is low or critical.
			if ((pBPwrInfo->bBatteryFlag != BATTERY_FLAG_UNKNOWN) && ((pBPwrInfo->bBatteryFlag & BATTERY_FLAG_LOW) || (pBPwrInfo->bBatteryFlag & BATTERY_FLAG_CRITICAL)))
			{
				LOG(INFO) + L"Power state is critical, disabling the Scanner";
				pScanner->CloseScanner();
			}
		}
	}
	return 0;
}

//  Message handler for our hidden window to resynchronize with the main thread (NPAPI is not 
//  threadsafe)
LRESULT CALLBACK CScanner::HiddenWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	//  TODO: TEST THAT THIS IS CALLED
	if (message == WM_SCAN_EVENT_RHOELEMENTS)
	{
        //LOG(INFO) + "Received a bluetooth status message";
        //OutputDebugString(L"Received a bluetooth status message\r\n");
		CScanner* pScanner = (CScanner*)GetWindowLong(hwnd, GWL_USERDATA);
		if (pScanner)
		{
			if (wparam == SCAN_EVENT_BT_CONNECTION_STATE_CHANGE)
			{
				//  We are only interested in Bluetooth Connection events
				if (lparam == SCAN_BT_CONNECTION_STATE_DISCONNECTED)
				{
					//  The Bluetooth Scanner has become Disconnected
					pScanner->SetBluetoothStatus(FALSE);
				}
				else if (lparam == SCAN_BT_CONNECTION_STATE_CONNECTED)
				{
					pScanner->SetBluetoothStatus(TRUE);
				}
			}
		}
	}
	else if (message == WM_CREATE)
    {
        //LOG(INFO) + "Hidden window  proc created...";
        //OutputDebugString(L"Received a request to close hidden window ...\r\n");
		return 0;
    }
	else if (message == WM_CLOSE)
    {
        //LOG(INFO) + "Received a request to close hidden window ...";
        //OutputDebugString(L"Received a request to close hidden window ...\r\n");

        // Not destroying the window can result
        // in a GDI window leak. The WM_CLOSE eventhough was posted in the earlier
        // logic never reached the window proc probably becuase the thread that created
        // the window was probably not spending time in the message loop.
        ::DestroyWindow(hwnd);
		return 0;
    }

	return DefWindowProc (hwnd, message, wparam, lparam);
}

#pragma endregion

