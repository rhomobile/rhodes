#include "stdafx.h"
#include "Generic.h"
#include "..\..\Common\Private\GenericHeader.h"
#include "AppManager.h"
#include "App.h"
#include "Meta.h"

extern HWND				g_hBaseWnd;
extern CAppManager		*g_pAppManager;
//extern LRESULT CALLBACK TagProc( LPPBNAMEVALUE pNameVal);
extern BOOL Log			(LogTypeInterface logSeverity,LPCTSTR pLogComment, 
						LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule);
extern WCHAR				g_szConfigFilePath[MAX_PATH + 1];
extern CConfig				*g_pConfig;
extern CMeta				*g_pPlgManager;
/******************************************************************************/
CGenericManager::CGenericManager ()
{
	
	hThread = 0;
}

/******************************************************************************/
CGenericManager::~CGenericManager ()
{
	Stop ();
}

/******************************************************************************/
void CGenericManager::Start (void)
{
	
	// Already started?
	if (hThread)
		return;

	hQuitEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
	hThread = CreateThread (NULL, 0, StaticWaitForRequest, this, 0, NULL);
}

/******************************************************************************/
void CGenericManager::Stop (void)
{
	// Running?
	if (!hThread)
		return;

	SetEvent (hQuitEvent);
	if (WaitForSingleObject (hThread, 5000) == WAIT_TIMEOUT)
		TerminateThread (hThread, 0);

	CloseHandle (hQuitEvent);
	hThread = 0;
}

/******************************************************************************/
void CGenericManager::WaitForRequest (void)
{
	HANDLE in_queue = INVALID_HANDLE_VALUE, out_queue = INVALID_HANDLE_VALUE;
	MSGQUEUEOPTIONS options;
	BYTE *pbuffer;
	DWORD count, flags;
	NEONHEADER *pheader;
	NEONLOG *plog;
	NEONRESPONSE *presponse;
	LPWSTR pequiv, pcontent, pcomment, psetting, pvalue, papp;
	HANDLE wait [2];
	int iInstID = 0;//set to zero for PB3 first release
	CApp *pApp = NULL;
	pbuffer = new BYTE [NEON_MAX_MESSAGE_LENGTH];
	
	WCHAR szMessageQueueName[40];
	DWORD dwEngineProcId = GetCurrentProcessId();
	
	// Open queue from host
	wsprintf(szMessageQueueName, L"%s%lu", NEON_TO_HOST, dwEngineProcId);
	memset (&options, 0, sizeof options);
	options.dwSize = sizeof options;
	options.dwFlags = MSGQUEUE_NOPRECOMMIT | MSGQUEUE_ALLOW_BROKEN;
	options.dwMaxMessages = 0;
	options.cbMaxMessage = NEON_MAX_MESSAGE_LENGTH;
	options.bReadAccess = TRUE;
	if ((in_queue = CreateMsgQueue (szMessageQueueName, &options)) == NULL)
		goto Exit;

	// Open queue to host
	wsprintf(szMessageQueueName, L"%s%lu", NEON_FROM_HOST, dwEngineProcId);
	options.bReadAccess = FALSE;
	if ((out_queue = CreateMsgQueue (szMessageQueueName, &options)) == NULL)
		goto Exit;

	wait [0] = hQuitEvent;
	wait [1] = in_queue;

	while (1)
	{
		// Wait for incoming message
		if (WaitForMultipleObjects (2, wait, FALSE, INFINITE) == WAIT_OBJECT_0)
			break;

		// Read message from queue - there will be a message available as the object is signalled and there are no other readers
		// But check the return just in case
		if (!ReadMsgQueue (in_queue, pbuffer, NEON_MAX_MESSAGE_LENGTH, &count, 0, &flags))
			continue;

		// Get header
		pheader = (NEONHEADER*) pbuffer;

		// Check message type
		switch (pheader->uType)
		{
		case NHT_INVOKE:
			// Extract parameters
			pequiv = (LPWSTR) (pbuffer + sizeof (NEONHEADER));
			pcontent = pequiv + wcslen (pequiv) + 1;

			// Send to core for processing

			/* PAUL - DO YOUR THING HERE! REMEMBER THAT THE MESSAGE HAS ARRIVED ON A DIFFERENT THREAD!! */
			/* ALSO THERE IS A 30 SECOND TIMEOUT IN THE ACTIVEX CONTROL OR THERE WILL BE A JAVASCRIPT ERROR */
			
			//  Note InvokeMetaFunction is sent without expecting a response
			
			if(g_hBaseWnd){
				/* USING SENDMESSAGE CAUSES THREADLOCK WHEN QUITTING! */
				if(!_memicmp(pequiv,L"application",11*2))
				{
					if(!_memicmp(pcontent,L"quit",4*2))
					{
						g_pAppManager->m_pbCoreStruct->pBrowserQuit(0, L"Core");
						//PostMessage(g_hBaseWnd,PB_GEN_QUIT,0,0);
						break;
					}
				}
				SendMessage(g_hBaseWnd,PB_GEN_ACTIVEX,(WPARAM)pequiv,(LPARAM)pcontent);
			}
			
			break;

		case NHT_SHOW_HOURGLASS:
			
			g_pAppManager->LockObj();
			pApp = g_pAppManager->GetApp(0);
			g_pAppManager->UnlockObj();
			if(pApp){
				g_pPlgManager->LockObj();
				g_pPlgManager->SetPlugProperty(pApp->GetAppPBStruct(),L"Hourglass",L"Visibility",L"Visible",L"AcxtiveX");
				g_pPlgManager->UnlockObj();

			}
			
			break;

		case NHT_HIDE_HOURGLASS:
			g_pAppManager->LockObj();
			pApp = g_pAppManager->GetApp(0);
			g_pAppManager->UnlockObj();
			if(pApp){
				g_pPlgManager->LockObj();
				g_pPlgManager->SetPlugProperty(pApp->GetAppPBStruct(),L"Hourglass",L"Visibility",L"Hidden",L"AcxtiveX");
				g_pPlgManager->UnlockObj();

			}
			break;

		case NHT_LOG:
			plog = (NEONLOG*) (pbuffer + sizeof (NEONHEADER));
			pcomment = (LPWSTR) (pbuffer + sizeof (NEONHEADER) + sizeof (NEONLOG));
			switch(plog->uSeverity)//user logs utilizes szCaller for it's severity
			{
			case 1:
				Log(PB_LOG_USER,pcomment,NULL,plog->uSeverity,L"Low");
			break;
				
			case 2:
				Log(PB_LOG_USER,pcomment,NULL,plog->uSeverity,L"Medium");
			break;

			case 3:
				Log(PB_LOG_USER,pcomment,NULL,plog->uSeverity,L"High");
			break;


			}
			
			
			break;

		case NHT_WRITE_USER_SETTING://change to WriteUserSetting
			// Extract parameters
			psetting = (LPWSTR) (pbuffer + sizeof (NEONHEADER));
			pvalue = psetting + wcslen (psetting) + 1;

			// PAUL - WRITE psetting AND pvalue TO YOUR CONFIG

			presponse = (NEONRESPONSE*) pbuffer;
			presponse->iValue = WriteSetting(psetting,pvalue);	// return non zero if error

			WriteMsgQueue (out_queue, pbuffer, sizeof (NEONRESPONSE), 0, 0);

			break;

		case NHT_READ_USER_SETTING://change to ReadUserSetting
			// Extract parameters
			psetting = (LPWSTR) (pbuffer + sizeof (NEONHEADER));
			
			pvalue = (WCHAR*) (pbuffer + sizeof (NEONRESPONSE));
			presponse = (NEONRESPONSE*) pbuffer;
			switch(ReadSetting(psetting)){
				case PB_READ_OK:
					presponse->iValue = 0;
					_tcscpy (pvalue, m_szResponse);	// REPLACE WITH ACTUAL SETTING VALUE
					break;
				case PB_READ_ERR:			//something has gone wrong 
					presponse->iValue = PB_READ_ERR;
					break;
				case PB_READ_NOTFOUND:		//the setting was not found
					wcscpy (pvalue,L"undefined");	
					presponse->iValue = 0;
					break;
				case PB_READ_NODATA:		//the setting contains no data
					wcscpy (pvalue,L"undefined");
					presponse->iValue = 0;
				break;

			};
						
			
			WriteMsgQueue (out_queue, pbuffer, sizeof (NEONRESPONSE) + (wcslen (pvalue) * 2) + 2, 0, 0);

			break;



		case NHT_WRITE_CONFIG_SETTING:
			// Extract parameters
			psetting = (LPWSTR) (pbuffer + sizeof (NEONHEADER));
			pvalue = psetting + wcslen (psetting) + 1;
			papp = pvalue + wcslen (pvalue) + 1;

			presponse = (NEONRESPONSE*) pbuffer;
			presponse->iValue = WriteConfigSetting(psetting,pvalue,papp)?0:1;// return non zero if error

			WriteMsgQueue (out_queue, pbuffer, sizeof (NEONRESPONSE), 0, 0);

			break;

		case NHT_READ_CONFIG_SETTING:
			// Extract parameters
			psetting = (LPWSTR) (pbuffer + sizeof (NEONHEADER));
			papp = psetting + wcslen (psetting) + 1;
			
			pvalue = (WCHAR*) (pbuffer + sizeof (NEONRESPONSE));
			presponse = (NEONRESPONSE*) pbuffer;
			
			switch(ReadConfigSetting(psetting,papp)){
				case PB_READ_OK:
					presponse->iValue = 0;
					_tcscpy (pvalue, m_szResponse);	// REPLACE WITH ACTUAL SETTING VALUE
					break;
				case PB_READ_ERR:			//something has gone wrong 
					presponse->iValue = PB_READ_ERR;
					break;
				case PB_READ_NOTFOUND:		//the setting was not found
					wcscpy (pvalue,L"undefined");	
					presponse->iValue = 0;
					break;
				case PB_READ_NODATA:		//the setting contains no data
					wcscpy (pvalue,L"undefined");
					presponse->iValue = 0;
				break;

			};
						
			WriteMsgQueue (out_queue, pbuffer, sizeof (NEONRESPONSE) + (wcslen (pvalue) * 2) + 2, 0, 0);

			break;
		}
	}

Exit:
	if (in_queue != INVALID_HANDLE_VALUE)
		CloseMsgQueue (in_queue);

	if (out_queue != INVALID_HANDLE_VALUE)
		CloseMsgQueue (out_queue);

	delete [] pbuffer;
}

/******************************************************************************/
DWORD CGenericManager::StaticWaitForRequest (LPVOID pparam)
{
	CGenericManager* pthis = (CGenericManager*) pparam;
	pthis->WaitForRequest ();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// read/write config settings
//
////////////////////////////////////////////////////////////////////////////////

//return the length of the destination string
int CGenericManager::Escape(LPCTSTR pIn,LPTSTR pEscapedStr)
{
	int iCount,iLen = _tcslen(pIn);
	LPTSTR pTarg;
	
	for(pTarg = pEscapedStr,iCount=0;*pIn;pIn++) 
	{
		if(*pIn==L'\\')*pTarg++=L'\\';
		
		*pTarg++ = *pIn;
	}
	
	return _tcslen(pEscapedStr);
}

//////////////////////////////////////////////////////////////////////////
//
//	configuration setting will be written to the file config.xml, but will
//	not effect the current settings of PB until PB is restarted  
//
//////////////////////////////////////////////////////////////////////////
int CGenericManager::ReadConfigSetting(LPCTSTR pSetting,LPCTSTR pAppName)
{
	XML_ATT pAtt;
	int iLoop,iRet= PB_READ_ERR;
	BOOL bGlobal;
	CConfig *pConfig = NULL;
	LPTSTR pEscapedStr = NULL;
	TCHAR szXmlSetStr[100];//just make sure the length of any of the XML path settings are shorter than this 
	memset(m_szResponse,0,MAX_LEN_RETURN * sizeof(TCHAR));
	
	LPTSTR pOutPut = m_szResponse;
	
	if(!pSetting || _tcsclen(pSetting)< 5 || !pAppName || !_tcslen(pAppName)){
	  goto _Cleanup;
	}
    
	// now translate setting name to path
	if(!_tcscmp(pAppName,L"GLOBAL")){//we are accessing a global setting
		bGlobal = TRUE;
		if(!Translate(pSetting,szXmlSetStr)){
			//couldn't find the setting  
			iRet = PB_READ_NOTFOUND;
			goto _Cleanup;
		}
		
	}
	else{
		bGlobal = FALSE;
		if(!TranslateApp(pSetting,szXmlSetStr)){
			//couldn't find the setting  
			iRet = PB_READ_NOTFOUND;
			goto _Cleanup;
		}
	}

	
	
  
	//open the file config.xml
	
	if((pConfig = new CConfig()) == NULL){
		goto _Cleanup;
	}
	//initialise the object with the path of the config file
	if(!pConfig->Init(g_szConfigFilePath)){
		goto _Cleanup;
	}
	
	
	_tcscpy(pOutPut++,L"[");
	if(bGlobal){//we are accessing a global setting
		pAtt = pConfig->GetSetting(szXmlSetStr,L"Value",0);
		if(!pAtt){
			iRet = PB_READ_NODATA;
			goto _Cleanup;
		}
		 
		_tcscpy(pOutPut++,L"\"");
		pOutPut+= Escape(pAtt->lpValue,pOutPut);//copies: \Program Files\myProg\help.txt to \\Program Files\\myProg\\help.txt
		_tcscpy(pOutPut++,L"\"");
		for(iLoop = 1;pAtt;iLoop++)
		{
			pAtt = pConfig->GetSetting(szXmlSetStr,L"Value",iLoop);
			if(!pAtt) 	break;
			_tcscpy(pOutPut,L",\"");
			pOutPut+=2;
			pOutPut+= Escape(pAtt->lpValue,pOutPut);//copies: \Program Files\myProg\help.txt to \\Program Files\\myProg\\help.txt
			_tcscpy(pOutPut++,L"\"");
				      
		}
				
		
	}
	else{//we are accessing an application setting
		
		pAtt = pConfig->GetAppSettingByName(0,pAppName,szXmlSetStr,L"Value");
		
		if(!pAtt){
			iRet = PB_READ_NODATA;
			goto _Cleanup;
		}
				
		_tcscpy(pOutPut++,L"\"");
		DEBUGMSG(1,(L"\n"));
		DEBUGMSG(1,(pAtt->lpValue));
		pOutPut+= Escape(pAtt->lpValue,pOutPut);
		_tcscpy(pOutPut++,L"\"");
		for(iLoop = 1;pAtt;iLoop++)
		{
			pAtt = pConfig->GetAppSettingByName(iLoop,pAppName,szXmlSetStr,L"Value");
			if(!pAtt)break;
			
			_tcscpy(pOutPut,L",\"");
			pOutPut+=2;
			DEBUGMSG(1,(L"\n"));
			DEBUGMSG(1,(pAtt->lpValue));
			pOutPut+= Escape(pAtt->lpValue,pOutPut);
			_tcscpy(pOutPut++,L"\"");
			
      
		}
		
	}
	_tcscpy(pOutPut++,L"]");
	iRet = PB_READ_OK;
	
_Cleanup:	
	delete [] pEscapedStr;
	delete pConfig;
	return iRet;
}

BOOL CGenericManager::WriteConfigSetting(LPCTSTR pSetting,LPCTSTR pValue,LPCTSTR pAppName)
{
	
	BOOL bGlobal,bRet = FALSE;
	CConfig *pConfig = NULL;
	
	TCHAR szXmlSetStr[100];//just make sure the length of any of the XML path settings are shorter than this 
	
	if(!pSetting || _tcsclen(pSetting)< 5 ||!pValue || !pAppName || !_tcslen(pAppName)){
	  goto _Cleanup;
	}
    
	// now translate setting name to path
	if(!_tcscmp(pAppName,L"GLOBAL")){//we are accessing a global setting
		bGlobal = TRUE;
		if(!Translate(pSetting,szXmlSetStr)){
			//couldn't find the setting  
			goto _Cleanup;
		}
		
	}
	else{
		bGlobal = FALSE;
		if(!TranslateApp(pSetting,szXmlSetStr)){
			//couldn't find the setting  
			goto _Cleanup;
		}
	}
	
	
  
	//open the file config.xml
	if((pConfig = new CConfig()) == NULL){
		goto _Cleanup;
	}
	//initialise the object with the path of the config file
	if(!pConfig->Init(g_szConfigFilePath)){
		goto _Cleanup;
	}
	
	  
	if(bGlobal){//we are accessing a global setting
		if(!pConfig->SetGlobalValue(szXmlSetStr,pValue)) goto _Cleanup;
	}
	else{//we are accessing an application setting
		if(!pConfig->SetAppValue(pAppName,szXmlSetStr,pValue)) goto _Cleanup;
		
	}
	bRet = pConfig->Save();
	
_Cleanup:	
	delete pConfig;
	return bRet;

}

BOOL CGenericManager::TranslateApp(LPCTSTR pSettName,LPTSTR pRetStr)
{
	if(!_tcscmp(pSettName,L"STARTPAGE")){
		_tcscpy(pRetStr,L"General\\StartPage");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"NAME")){
		_tcscpy(pRetStr,L"General\\Name");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"USEREGULAREXPRESSIONS")){
		_tcscpy(pRetStr,L"General\\UseRegularExpressions");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"HTTPPROXY")){
		_tcscpy(pRetStr,L"HTTP_Proxy");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"PRELOAD")){
		_tcscpy(pRetStr,L"Preloads\\Preload");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"SCROLLBARSENABLED")){
		_tcscpy(pRetStr,L"ScrollBars\\ScrollBarsEnabled");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"SCROLLTECHNIQUE")){
		_tcscpy(pRetStr,L"Scrolling\\ScrollTechnique");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"AUTHUSER_GLOBAL")){
		_tcscpy(pRetStr,L"Authentication\\Username");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"AUTHPASS_GLOBAL")){
		_tcscpy(pRetStr,L"Authentication\\Password");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"FITTOSCREENENABLED")){
		_tcscpy(pRetStr,L"HTMLStyles\\FitToScreenEnabled");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"CLEARTYPEENABLED")){
		_tcscpy(pRetStr,L"HTMLStyles\\ClearTypeEnabled");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"JAVASCRIPTENABLED")){
		_tcscpy(pRetStr,L"HTMLStyles\\JavascriptEnabled");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"TEXTSELECTIONENABLED")){
		_tcscpy(pRetStr,L"HTMLStyles\\TextSelectionEnabled");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"FONTFAMILY")){
		_tcscpy(pRetStr,L"HTMLStyles\\FontFamily");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"FONTDIRECTORY")){
		_tcscpy(pRetStr,L"HTMLStyles\\FontDirectory");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"RESIZEONSIP")){
		_tcscpy(pRetStr,L"SIP\\ResizeOnSIP");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"LOWBATTERYSCAN")){
		_tcscpy(pRetStr,L"System\\LowBatteryScan");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"DISABLESCANNERDURINGNAV")){
		_tcscpy(pRetStr,L"Scanner\\DisableScannerDuringNavigation");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"DECODEVOLUME")){
		_tcscpy(pRetStr,L"Sound\\DecodeVolume");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"DECODEFREQUENCY")){
		_tcscpy(pRetStr,L"Sound\\DecodeFrequency");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"INVALIDDECODEFREQUENCY")){
		_tcscpy(pRetStr,L"Sound\\InvalidDecodeFrequency");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"DECODEDURATION")){
		_tcscpy(pRetStr,L"Sound\\DecodeDuration");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"DECODEFREQUENCY")){
		_tcscpy(pRetStr,L"Sound\\DecodeFrequency");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"INVALIDDECODEFREQUENCY")){
		_tcscpy(pRetStr,L"Sound\\InvalidDecodeFrequency");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"DECODEDURATION")){
		_tcscpy(pRetStr,L"Sound\\DecodeDuration");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"SCANDECODEWAV")){
		_tcscpy(pRetStr,L"Sound\\ScanDecodeWav");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"SCANINVALIDWAV")){
		_tcscpy(pRetStr,L"Sound\\ScanInvalidWav");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"IMAGERCAPTUREWAV")){
		_tcscpy(pRetStr,L"Sound\\ImagerCaptureWav");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"SIGNALREFRESH")){
		_tcscpy(pRetStr,L"GUI\\SignalRefresh");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"BATTERYREFRESH")){
		_tcscpy(pRetStr,L"GUI\\BatteryRefresh");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"HOURGLASSENABLED")){
		_tcscpy(pRetStr,L"GUI\\HourglassEnabled");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"HOURGLASSLEFT")){
		_tcscpy(pRetStr,L"GUI\\HourglassLeft");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"HOURGLASSTOP")){
		_tcscpy(pRetStr,L"GUI\\HourglassTop");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"BADLINKURI")){
		_tcscpy(pRetStr,L"Navigation\\BadLinkURI");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"USERAGENT")){
		_tcscpy(pRetStr,L"Navigation\\UserAgent");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"VIEWPORTENABLED")){
		_tcscpy(pRetStr,L"Navigation\\ViewportEnabled");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"VIEWPORTWIDTH")){
		_tcscpy(pRetStr,L"Navigation\\ViewportWidth");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"CAFILE")){
		_tcscpy(pRetStr,L"Navigation\\CaFile");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"CAPATH")){
		_tcscpy(pRetStr,L"Navigation\\CaPath");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"VERIFYCERTIFICATES")){
		_tcscpy(pRetStr,L"Navigation\\VerifyCertificates");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"VERIFYPEERCERTIFICATE")){
		_tcscpy(pRetStr,L"Navigation\\VerifyPeerCertificate");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"NETWORKCOOKIEDATABASE")){
		_tcscpy(pRetStr,L"Navigation\\NetworkCookieDatabase");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"NAVIGATIONCACHE")){
		_tcscpy(pRetStr,L"Navigation\\Cache");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"ENABLECTRLKEY_O")){
		_tcscpy(pRetStr,L"DeviceKeys\\EnableCtrlKey_O");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"ENABLECTRLKEY_P")){
		_tcscpy(pRetStr,L"DeviceKeys\\EnableCtrlKey_P");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"ENABLECTRLKEY_A")){
		_tcscpy(pRetStr,L"DeviceKeys\\EnableCtrlKey_A");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"ENABLECTRLKEY_C")){
		_tcscpy(pRetStr,L"DeviceKeys\\EnableCtrlKey_C");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"ENABLECTRLKEY_V")){
		_tcscpy(pRetStr,L"DeviceKeys\\EnableCtrlKey_V");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"DEFAULTMETATAG")){
		_tcscpy(pRetStr,L"DefaultMetaTags\\MetaTag");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"WEBSTORAGEDBPATH")){
		_tcscpy(pRetStr,L"WebDB\\WebStorageDBPath");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"WEBSQLDBQUOTA")){
		_tcscpy(pRetStr,L"WebDB\\WebSQLDBQuota");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"WEBSQLDBPATH")){
		_tcscpy(pRetStr,L"WebDB\\WebSQLDBPath");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"APPLICATIONCACHEQUOTA")){
		_tcscpy(pRetStr,L"ApplicationCache\\ApplicationCacheQuota");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"APPLICATIONCACHEPATH")){
		_tcscpy(pRetStr,L"ApplicationCache\\ApplicationCachePath");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"NPAPIDIRECTORY")){
		_tcscpy(pRetStr,L"NPAPI\\NPAPIDirectory");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"PRELOADLEGACYGENERIC")){
		_tcscpy(pRetStr,L"NPAPI\\Preloads\\PreloadLegacyGeneric");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"PRELOADLEGACYODAX")){
		_tcscpy(pRetStr,L"NPAPI\\Preloads\\PreloadLegacyODAX");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"PRELOADLEGACYNOSIP")){
		_tcscpy(pRetStr,L"NPAPI\\Preloads\\PreloadLegacyNoSIP");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"PRELOADLEGACYAIRBEAM")){
		_tcscpy(pRetStr,L"NPAPI\\Preloads\\PreloadLegacyAirBeam");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"PRELOADLEGACYAPD")){
		_tcscpy(pRetStr,L"NPAPI\\Preloads\\PreloadLegacyAPD");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"PRELOADLEGACYJSOBJECTS")){
		_tcscpy(pRetStr,L"NPAPI\\Preloads\\PreloadJSObjects");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"ENABLEBACKLIGHTSLIDER")){
		_tcscpy(pRetStr,L"DeviceKeys\\EnableBacklightSlider");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"ENABLEVOLUMESLIDER")){
		_tcscpy(pRetStr,L"DeviceKeys\\EnableVolumeSlider");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"HTTPSPROXY")){
		_tcscpy(pRetStr,L"HTTPS_Proxy");
		return TRUE;
	}

	return FALSE;

  
}



BOOL CGenericManager::Translate(LPCTSTR pSettName,LPTSTR pRetStr)
{
	if(!_tcscmp(pSettName,L"DEBUGBUTTONSENABLED")){
		_tcscpy(pRetStr,L"Configuration\\DebugButtons\\DebugButtonsEnabled");
		return TRUE;
	}
	else if(!_tcscmp(pSettName,L"COMPATIBILITYMODEENABLED")){
		_tcscpy(pRetStr,L"Configuration\\CompatibilityMode\\CompatibilityModeEnabled");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"LOGPROTOCOL")){
		_tcscpy(pRetStr,L"Configuration\\Logger\\LogProtocol");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"LOGPORT")){
		_tcscpy(pRetStr,L"Configuration\\Logger\\LogPort");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"LOGURI")){
		_tcscpy(pRetStr,L"Configuration\\Logger\\LogURI");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"LOGERROR")){
		_tcscpy(pRetStr,L"Configuration\\Logger\\LogError");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"LOGWARNING")){
		_tcscpy(pRetStr,L"Configuration\\Logger\\LogWarning");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"LOGINFO")){
		_tcscpy(pRetStr,L"Configuration\\Logger\\LogInfo");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"LOGUSER")){
		_tcscpy(pRetStr,L"Configuration\\Logger\\LogUser");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"LOGMAXSIZE")){
		_tcscpy(pRetStr,L"Configuration\\Logger\\LogMaxSize");
		return TRUE;

	}
	
	else if(!_tcscmp(pSettName,L"LOGMEMORY")){
		_tcscpy(pRetStr,L"Configuration\\Logger\\Logmemory");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"LOGMEMPERIOD")){
		_tcscpy(pRetStr,L"Configuration\\Logger\\LogMemPeriod");
		return TRUE;

	}
	
	else if(!_tcscmp(pSettName,L"REGEXFILE")){
		_tcscpy(pRetStr,L"Configuration\\FileLocations\\RegEXFile");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"PLUGINFILE")){
		_tcscpy(pRetStr,L"Configuration\\FileLocations\\PluginFile");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"FULLSCREEN")){
		_tcscpy(pRetStr,L"Configuration\\Screen\\FullScreen");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"FUNCTIONKEYSCAPTURABLE")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\FunctionKeysCapturable");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"ENABLEFUNCTIONKEY_F1")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\EnableFunctionKey_F1");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"ENABLEFUNCTIONKEY_F2")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\EnableFunctionKey_F2");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"ENABLEFUNCTIONKEY_F3")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\EnableFunctionKey_F3");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"ENABLEFUNCTIONKEY_F4")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\EnableFunctionKey_F4");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"ENABLEFUNCTIONKEY_F5")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\EnableFunctionKey_F5");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"ENABLEFUNCTIONKEY_F6")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\EnableFunctionKey_F6");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"ENABLEFUNCTIONKEY_F7")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\EnableFunctionKey_F7");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"ENABLEFUNCTIONKEY_F8")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\EnableFunctionKey_F8");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"ENABLEFUNCTIONKEY_F9")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\EnableFunctionKey_F9");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"ENABLEFUNCTIONKEY_F10")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\EnableFunctionKey_F10");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"ENABLEFUNCTIONKEY_F11")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\EnableFunctionKey_F11");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"ENABLEFUNCTIONKEY_F12")){
		_tcscpy(pRetStr,L"Configuration\\DeviceKeys\\EnableFunctionKey_F12");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"NAVTIMEOUT")){
		_tcscpy(pRetStr,L"Configuration\\Navigation\\NavTimeout");
		return TRUE;

	}	
	else if(!_tcscmp(pSettName,L"AUTOROTATE")){
		_tcscpy(pRetStr,L"Configuration\\ScreenOrientation\\AutoRotate");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"WEBSENABLED")){
		_tcscpy(pRetStr,L"Configuration\\WebServer\\Enabled");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"WEBSPORT")){
		_tcscpy(pRetStr,L"Configuration\\WebServer\\Port");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"WEBSFOLDER")){
		_tcscpy(pRetStr,L"Configuration\\WebServer\\WebFolder");
		return TRUE;

	}
	else if(!_tcscmp(pSettName,L"WEBSPUBLIC")){
		_tcscpy(pRetStr,L"Configuration\\WebServer\\Public");
		return TRUE;

	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//
// read/write user settings
//
////////////////////////////////////////////////////////////////////////////////
int CGenericManager::ReadSetting(LPCTSTR pSetting)
{
	XML_TAG xTag;
	Lock AutoLock(g_pConfig->GetCriticalSect());
	
	if(pSetting){
		LPTSTR pXml = new TCHAR[_tcslen(pSetting)+ 30];
		if(pXml){
			//build the XML path
			_tcscpy(pXml,L"Configuration\\UserData\\");
			_tcscat(pXml,pSetting);
			xTag = g_pConfig->GetTag(pXml,0);
			delete []pXml; 
			if(xTag){
				if(xTag->m_pSzData){
					_tcscpy(m_szResponse,xTag->m_pSzData);
					return PB_READ_OK;
				}
				else{
					return PB_READ_NODATA;
				}
			}
			return PB_READ_NOTFOUND;
			

		}
	}
	return PB_READ_ERR;
}

//returns error code
int CGenericManager::WriteSetting(LPCTSTR pSetting, LPCTSTR pValue)
{
	UINT iSetLen,iValLen;
	XML_TAG xTag,xSetTag;
	TCHAR szUserXml[]={L"Configuration\\UserData"};
	LPTSTR pTagName;
	Lock AutoLock(g_pConfig->GetCriticalSect()); 
	
	//first check for an existing setting
	if(pSetting &&  pValue){
		iSetLen = _tcslen(pSetting);
		iValLen = _tcslen(pValue);
		if(iSetLen && iValLen){
			pTagName = new TCHAR[iSetLen+3];
			if(pTagName){
				_tcscpy(pTagName,L"<");
				_tcscat(pTagName,pSetting);
				_tcscat(pTagName,L">");
				xTag = g_pConfig->GetTag(szUserXml,0);
				if(xTag){
					xSetTag = g_pConfig->GetTag(pSetting,0,xTag);
					if(!xSetTag){
						//the XML element does not exist so create it
						xSetTag = xTag->NewChild(pTagName,OPEN_TAG);
						
					}
					delete [] pTagName;
					if(xSetTag){
						xSetTag->AddData(pValue,iValLen);
						if(g_pConfig->Save()){
							return 0;//return no error
						}
					}
				}
			}
		}
	}
	
	//if found write new data
	//if not create the XML element
	return 1;
}
/*
LPCTSTR CGenericManager::ReadSetting(LPCTSTR pFileName, LPCTSTR pXmlPath)
{
	LPCTSTR pRet = NULL;
	if(pFileName && *pFileName){
		m_pXML = new CXMLReader();
		if(m_pXML){
			if(m_pXML->ParseFile(pFileName) == SUCCESS){
				pRet = GetJSON(pXmlPath);
			}
			delete m_pXML;
		}
	}
	return pRet;
}


LPCTSTR CGenericManager::GetJSON(LPCTSTR pXmlPath)
{
	int iLen,iIndex;
	XML_TAG xTag = NULL;
	LPCTSTR pXml = pXmlPath;
	if(pXmlPath && *pXmlPath ){
		iLen = (int)_tcslen(pXml);
		
		_tcscpy(m_szJSON,L"[");
		if(*pXml == L'\\' && iLen==1){
			buildRootJSON();//root request
		}
		else{
			if(*pXml == L'\\')pXml++;//index past the first backslash
			xTag = GetTag(pXml,0);
			for(iIndex = 1;xTag;iIndex++)
			{
				buildJSON(xTag);//build a JSON class for this element
				
				xTag = GetTag(pXml,iIndex);
				if(!xTag)
					break;
				_tcscat(m_szJSON,L",");
				
			}
		}
		_tcscat(m_szJSON,L"];\0");
		OutputDebugString(m_szJSON);
		return m_szJSON;
		
	}
	m_szJSON[0]=NULL;
	return NULL;
}


LPCTSTR CGenericManager::buildRootJSON()
{
	
	XML_TAG xChild,xTag = m_pXML->GetRoot();
	
	
	_tcscat(m_szJSON,L"{\"elementName\":\"\\");
	_tcscat(m_szJSON,L"\",\"arrChildElements\" :[");
	
	//
	//for each child
	//
	for(xChild =xTag;xChild;xChild = xChild->m_pSibTag)
	{
		_tcscat(m_szJSON,L"\"");
		_tcscat(m_szJSON,xChild->m_pSzName);
		_tcscat(m_szJSON,L"\"");
		if(xChild->m_pSibTag){
			_tcscat(m_szJSON,L",");
		}

	}
	_tcscat(m_szJSON,L"],");
	_tcscat(m_szJSON,L"\"arrAttribNames\" :[],");
	_tcscat(m_szJSON,L"\"arrAttribValues\" :[],");
	
	//add the tag data		
	_tcscat(m_szJSON,L"\"innerData\" :\"\"};");
	
	return m_szJSON;

}

LPCTSTR CGenericManager::buildJSON(XML_TAG xTag)
{
	XML_TAG xChild	= NULL;
	XML_ATT xAtt	= NULL;

	if(xTag){
		_tcscat(m_szJSON,L"{\"elementName\":\"");
		_tcscat(m_szJSON,xTag->m_pSzName);
		_tcscat(m_szJSON,L"\",\"arrChildElements\" :[");
		
		//
		//for each child
		//
		for(xChild =xTag->m_pChildTag;xChild;xChild = xChild->m_pSibTag)
		{
			_tcscat(m_szJSON,L"\"");
			_tcscat(m_szJSON,xChild->m_pSzName);
			_tcscat(m_szJSON,L"\"");
			if(xChild->m_pSibTag){
				_tcscat(m_szJSON,L",");
			}

		}
		_tcscat(m_szJSON,L"],");

		

		_tcscat(m_szJSON,L"\"arrAttribNames\" :[");
		//for each attribute name
		//
		for(xAtt =xTag->m_pAttrbs;xAtt;xAtt = xAtt->pAttrbs)
		{
			_tcscat(m_szJSON,L"\"");
			_tcscat(m_szJSON,xAtt->lpName);
			_tcscat(m_szJSON,L"\"");
			if(xAtt->pAttrbs){
				_tcscat(m_szJSON,L",");
			}

		}
		_tcscat(m_szJSON,L"],");

		
		_tcscat(m_szJSON,L"\"arrAttribValues\" :[");
		//for each attribute value
		//
		for(xAtt =xTag->m_pAttrbs;xAtt;xAtt = xAtt->pAttrbs)
		{
			_tcscat(m_szJSON,L"\"");
			_tcscat(m_szJSON,xAtt->lpValue);
			_tcscat(m_szJSON,L"\"");
			if(xAtt->pAttrbs){
				_tcscat(m_szJSON,L",");
			}

		}
		_tcscat(m_szJSON,L"],");


		//add the tag data		
		_tcscat(m_szJSON,L"\"innerData\" :\"");
		if(xTag->m_pSzData){
			_tcscat(m_szJSON,xTag->m_pSzData);
		}
		_tcscat(m_szJSON,L"\"}");
		
		return m_szJSON;
	}
	return NULL;
}

//can be called with or without 'xFromTag'
XML_TAG CGenericManager::GetTag(LPCTSTR pXmlPath,int iIndex,XML_TAG xFromTag)
{
	LPCTSTR		pTagStart,pStr;
	XML_TAG		xTag[20];//20 levels deep is the max query EG: \one\two\three == three levels
	WCHAR		szTag[MAXLEN_TAGNAME + 1];
	int			iLen,iTagCount,iTagLen; 
	
	if(!pXmlPath || *pXmlPath==NULL || !m_pXML){
		return NULL;
	}
	
	iLen = (int)wcslen(pXmlPath);
		

	if(xFromTag){
		xTag[0] = xFromTag;
		iTagCount	= 1;
	}
	else{//xFromTag == NULL
		iTagCount	= 0;
	}
	
	
	
	
	iTagLen		= 0;	
	if(iLen && m_pXML){
		
		for(pStr = pTagStart = pXmlPath;;pStr++)
		{
			if(*pStr==L'\\'){
				//if(!iTagLen) continue;
				wcsncpy(szTag,pTagStart,iTagLen);
				szTag[iTagLen]=NULL;
				iTagLen = 0;
				pTagStart = pStr + 1; 
				if(iTagCount == 0){
					xTag[iTagCount] = m_pXML->GetElement(szTag,0);
					if(!xTag[iTagCount]){
						break;
					}

				}
				else{
					xTag[iTagCount] = m_pXML->GetChildElement(xTag[iTagCount - 1],szTag,0);
					if(!xTag[iTagCount]){
						break;;
					}
				}
				iTagCount++;
				continue;
				
			}
			
			else if(*pStr == NULL){
				wcsncpy(szTag,pTagStart,iTagLen);
				szTag[iTagLen]=NULL;
				xTag[iTagCount] =  m_pXML->GetChildElement(xTag[iTagCount - 1],szTag,iIndex);
				if(!xTag[iTagCount]){
					break;
				}
				return xTag[iTagCount];
			}
			iTagLen++;
		}

	}
	return NULL;	
}

*/
