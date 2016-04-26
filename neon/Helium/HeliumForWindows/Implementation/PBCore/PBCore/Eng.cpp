#include "Eng.h"

extern CConfig				*g_pConfig;		//pointer to our global config object
extern PPBCORESTRUCT		g_pPBCore;
CEng *g_pEng;

#ifndef RHO_ELEMENTS
////  Only include this Extern if the engine is in the RhoElements runtime
extern BOOL Log(LogType logSeverity,LPCTSTR pLogComment, 
	LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule);
#else
#include "logging/RhoLog.h"
#endif


#define INST_ID		    0
#define SYMBOLWEBKIT      L"SymbolWebkit"
#define SYMBOLWEBKITINPROCESS      L"SymbolWebkitInProcess"
#define MAX_INSTANCES   10  // actual MAX is limited by IPC and is 5 or 6 depending on usage, 
                            // but 10 allows application time to quit without preventing a restart



CEng::CEng(HWND hParentWnd,CConfig *pConfig)//,BYTE LogMask)
{
	isWebkitInProcess			= true; // Setting default value to true as we will be using webkit in process from RMS 5.1 onwards.
    m_szPreNavString			= NULL;
    m_bInitialisationComplete	= FALSE;
    m_bNavEventsRegComplete		= FALSE;
    //m_Log						= LogFuncPtr;
	m_pConfig					= pConfig;
	m_MessageProc				= NULL;
	m_PrimaryMessageProc		= NULL;
	m_iPhysicalLimit			= DEFAULT_PHYSMEM_LIMIT;
	m_iVirtualLimit				= DEFAULT_VIRTMEM_LIMIT;
	int iIndex;
	for(iIndex = 0;iIndex < MAX_TABS ;iIndex++) m_arrHTMLWindows[iIndex] = NULL;
	m_bScrollBars				= SETTING_OFF;
	g_pEng = this;
}

CEng::~CEng(void)
{
}

BOOL CEng::Start(LPWSTR pEnginePath)
{
	LPCWSTR pPhyMem =  m_pConfig->GetSettingPtr(PHYSMEM_LIMIT_XML,L"value");
	LPCWSTR pVirtMem =  m_pConfig->GetSettingPtr(VIRTMEM_LIMIT_XML,L"value");
	if(pPhyMem){
		m_iPhysicalLimit = _wtoi(pPhyMem);
	}
	if(pVirtMem){
		m_iVirtualLimit = _wtoi(pVirtMem);
	}
 	
 int iInst;
    for (iInst = 0; iInst < MAX_INSTANCES ;iInst++)
    {
        WCHAR szInstance[32];
		
		if(!isWebkitInProcess)
		{
			//Starting webkit outprocess instance by 1 and 
			//thereafter if next outprocess engine is launched 
			//then it will increment by 1 i.e. 1,2,3...
			iInst = iInst + 1;
			wsprintf(szInstance, L"%s%d", SYMBOLWEBKIT, iInst);
		}
		else
		{
			wsprintf(szInstance, L"%s%d", SYMBOLWEBKITINPROCESS, iInst);
		}
		
        m_hInstanceEvent = CreateEvent(NULL, TRUE, FALSE, szInstance);
        if (m_hInstanceEvent && (GetLastError() != ERROR_ALREADY_EXISTS)) {
            OutputDebugString(L"\nCEng using ");
            OutputDebugString(szInstance);
            OutputDebugString(L"\n");
            break;
        } 
 
        CloseHandle(m_hInstanceEvent);
        m_hInstanceEvent = NULL;
    }
    
    if (!m_hInstanceEvent)
    {
        EngineLog(LOG_ERROR, L"Max Symbol webkit instances reached", _T(__FUNCTION__), __LINE__, _T(__FILE__));
        return FALSE;
    }
	
	if(!isWebkitInProcess)
	{
		//In case of webkit out process, the instance id should be different everytime
		//This should always set to different instance id 
		//or else it doesnot allow to launch application properly.
		return (start(iInst, pEnginePath));
	}
	else
	{	  
		//In case of webkit in process, the instance id should be set to zero
		//or else it doesnot allow second application to launch properly.
		return (start(0,pEnginePath));
	}
	
 
}

BOOL CEng::availableMemory()
{
	
	//m_iVirtualLimit = DEFAULT_VIRTMEM_LIMIT;
	//m_iPhysicalLimit = DEFAULT_PHYSMEM_LIMIT;

	//Get the memory stats
	MEMORYSTATUS sMemStat;
	sMemStat.dwLength = sizeof(MEMORYSTATUS);

	GlobalMemoryStatus(&sMemStat); 
	
	double dAvailPhys,dTotalPhys,dAvailVirt,dTotalVirt;
	dAvailPhys = sMemStat.dwAvailPhys;
	dTotalPhys = sMemStat.dwTotalPhys;
	dAvailVirt = sMemStat.dwAvailVirtual;
	dTotalVirt = sMemStat.dwTotalVirtual;
	
	double dPhyUsed = (double)(((dTotalPhys-dAvailPhys)/dTotalPhys)*100);
	double dVirtUsed = (double)(((dTotalVirt - dAvailVirt)/dTotalVirt)*100);

	

#ifdef _DEBUG
	
	
	WCHAR szPhyUsed[5];
	WCHAR szVirtUsed[5];
	WCHAR szMsg[200];
	_itow((int)dPhyUsed,szPhyUsed,10);
	_itow((int)dVirtUsed,szVirtUsed,10);


	wcscpy(szMsg,L"Physical Usage: ");
	wcscat(szMsg,szPhyUsed);
	wcscat(szMsg,L"%\n");
	wcscat(szMsg,L"Virtual Usage: ");
	wcscat(szMsg,szVirtUsed);
	wcscat(szMsg,L"%\n");
	OutputDebugString(szMsg);
	//LOG(LOG_INFO,szMsg);

#endif	
	
	
	if((int)dPhyUsed >= m_iPhysicalLimit){
		EngineLog(LOG_WARNING, L"Mem limit reached,see NewTabPhysicalMemLimit in config file ", _T(__FUNCTION__), __LINE__, _T(__FILE__));
		return FALSE;
	}


	if((int)dVirtUsed >= m_iVirtualLimit){
		EngineLog(LOG_WARNING, L"Mem limit reached,see NewTabVirtualMemLimit in config file ", _T(__FUNCTION__), __LINE__, _T(__FILE__));
		return FALSE;
	}

	return TRUE;

}


BOOL CEng::Navigable(LPCTSTR szURL)
{
	if(!szURL){
		return FALSE;
	}
	//  No application license in effect, all navigations are OK
	return TRUE;
}

void CEng::onInitialisationComplete()
{
    if (m_bInitialisationComplete)
        return;

    m_bInitialisationComplete = TRUE;

   	TCHAR szPlatform[128];
    memset(szPlatform, 0, 127 * sizeof(TCHAR));
    SystemParametersInfo(SPI_GETOEMINFO, 127, szPlatform, 0);
    _wcsupr(szPlatform);
    if ((wcsstr(szPlatform, L"ES40") != 0) || (wcsstr(szPlatform, L"VC70") != 0)) {
        SetScrollingThreshold(32);
    }

    if (m_bNavEventsRegComplete && m_szPreNavString) {
        NavigateOnTab(m_szPreNavString, INST_ID);
        delete [] m_szPreNavString;
        m_szPreNavString = NULL;
    }
}


//  Darryn Campbell
BOOL CEng::ConfigureEnginePaging(LPCWSTR pEnginePath)
{
	//  The assumption is always that the DLLs or Exe on the device is pageable, therefore
	//  this function only needs to be called to set the DLL/EXE to non-pageable.
	//  If we are running on CE6 we want to load a non-pageable variant of the WebKit and WebkitLibrary
	//  DLLs, we only want to do this on CE6 as setting this flag on CE7 will cause the DLL
	//  to not be loaded.
	//  Note we don't want to try setting the pageable-ness on low memory devices:
	TCHAR szPlatform[128];
	memset(szPlatform, 0, 127 * sizeof(TCHAR));
	SystemParametersInfo(SPI_GETOEMINFO, 127, szPlatform, 0);
	_wcsupr(szPlatform);

	OSVERSIONINFO osvi;
	memset(&osvi, 0, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	bool bIsCE6 = (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 && (wcsstr(szPlatform,L"MC21") == 0 || wcsstr(szPlatform,L"SB1") == 0));
	if (bIsCE6)
	{
		ENGINELOG(LOG_INFO, L"The Webkit engine is being set to non-pageable to improve performance on CE6 devices");
		return SetPageable(pEnginePath, TRUE /*  TRUE here indicates to set to non-pageable  */);
	}
	else
	{
		//  We are not CE6 OR we are a CE6 device which has insufficient memory for a non-pageable DLL
		ENGINELOG(LOG_INFO,L"OutProcessWK.exe is set to Pageable.  Either your device is not running CE6 or your device does not have enough memory to run non-pageable");
		return FALSE;
	}

}


BOOL CEng::EngineLog(LogType logSeverity,LPCTSTR pLogComment, 
						LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule)
{
	//  How the engine logs will determine whether it is built as part of RhoElements Extension
	//  or RhoElements Runtime
#ifdef RHO_ELEMENTS
	//  RhoElements Extension
	if (logSeverity == PB_LOG_INFO || logSeverity == PB_LOG_USER)
		LOG(INFO) + "Message from " + pCallingModule + ":" + pFunctionName + " - " + pLogComment + " Line: " + dwLineNumber;
	else if (logSeverity == PB_LOG_WARNING)
		LOG(WARNING) + "Message from " + pCallingModule + ":" + pFunctionName + " - " + pLogComment + " Line: " + dwLineNumber;
	else if (logSeverity == PB_LOG_ERROR)
		LOG(ERROR) + "Message from " + pCallingModule + ":" + pFunctionName + " - " + pLogComment + " Line: " + dwLineNumber;
	
	
	return TRUE;
#else
	return Log((LogType)logSeverity,pLogComment, pFunctionName, __LINE__, pCallingModule);
	//return g_pEng->m_Log((LogTypeInterface)logSeverity,pLogComment, pFunctionName, __LINE__, pCallingModule);
	//return FALSE;// Log(logSeverity, pLogComment, pFunctionName, __LINE__, pCallingModule);
#endif
}

//  Gary Crean
BOOL CEng::SetPageable(LPCWSTR szDllName,BOOL bSet)
{
	HANDLE					hFile;
	DWORD					dwRead;
	DWORD					dwWrite;
	DWORD					dwSize;
	DWORD					dwFilePos;
	IMAGE_NT_HEADERS		NTHeaders = {0};
	PIMAGE_SECTION_HEADER	lpSections = NULL;
	IMAGE_DOS_HEADER		dos_header ={0};

	//Open Dll File
	hFile = CreateFile(szDllName,GENERIC_READ | GENERIC_WRITE,NULL,NULL,OPEN_EXISTING,NULL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;

	//Load DOS Header
	ReadFile(hFile,&dos_header,sizeof(dos_header),&dwRead,NULL);
	if (dos_header.e_magic != IMAGE_DOS_SIGNATURE)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	//Load NT Header
	SetFilePointer(hFile,dos_header.e_lfanew,0,FILE_BEGIN);
	ReadFile(hFile,&NTHeaders,sizeof(IMAGE_NT_HEADERS),&dwRead,NULL);
	if (NTHeaders.Signature != IMAGE_NT_SIGNATURE)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	//Save File Position
	dwFilePos = SetFilePointer(hFile,0,0,FILE_CURRENT);
	dwSize = sizeof(IMAGE_SECTION_HEADER) * NTHeaders.FileHeader.NumberOfSections;
	lpSections = (PIMAGE_SECTION_HEADER) LocalAlloc(LPTR,dwSize);
	if (!lpSections)
	{
		CloseHandle(hFile);
		return FALSE;	
	}

	//Load Section Tables
	ReadFile(hFile,lpSections,dwSize,&dwRead,NULL);
	for (DWORD Idx=0;Idx<NTHeaders.FileHeader.NumberOfSections;Idx++)
	{
		if (strncmp((LPCSTR)lpSections[Idx].Name,".text",5) == 0)
		{
			if (bSet)
			{
				lpSections[Idx].Characteristics |= IMAGE_SCN_MEM_NOT_PAGED;
			}else{
				lpSections[Idx].Characteristics &= ~IMAGE_SCN_MEM_NOT_PAGED;
			}
		}
	}

	//Write Section back
	SetFilePointer(hFile,dwFilePos,0,FILE_BEGIN);
	WriteFile(hFile,lpSections,dwSize,&dwWrite,NULL);

	//Clean up
	CloseHandle(hFile);
	LocalFree(lpSections);
	return TRUE;	
}

//LRESULT CALLBACK GetAppConfig(int iInstID, LPCTSTR tcSetting, WCHAR* tcValue)
//{
//LPCTSTR tcValueRead;
//	tcValueRead = CEng::m_pConfig->GetAppSettingPtr(iInstID, tcSetting, L"Value");
//	if (tcValueRead != NULL)
//		wcscpy(tcValue, tcValueRead);
//	else
//		tcValue = NULL;
//	return S_OK;
/*
LPWSTR ptemp;
#ifdef _DEBUG
	WCHAR szMessage[100];
#endif
	if(CEng::m_pConfig){
		if((ptemp = (WCHAR*)CEng::m_pConfig->GetAppSettingPtr(iInstID,tcSetting, L"Value"))!=NULL){
		wcscpy(tcValue,ptemp);
#ifdef _DEBUG			
			wsprintf(szMessage,L"Set %s:%s\n",tcSetting,tcValue);
			OutputDebugString(szMessage);
			
#endif
			return S_OK;
		}
	}
	tcSetting = NULL;
#ifdef _DEBUG	
	wsprintf(szMessage,L"Could not find %s:\n",tcSetting);
	OutputDebugString(szMessage);
#endif
	return S_OK;
*/
	


//LPCWSTR GetConfig(LPCTSTR tcSetting, WCHAR* tcValue)
//{
//	
//	if(CEng::m_pConfig){
//		return CEng::m_pConfig->GetSettingPtr(tcSetting, L"Value");
//		
//	}	
//	return NULL;
//}
