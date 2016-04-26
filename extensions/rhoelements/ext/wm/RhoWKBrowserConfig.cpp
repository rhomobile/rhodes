#include "stdafx.h"
#include "RhoWKBrowserConfig.h"

#include "logging/RhoLog.h"
//#include "common/app_build_capabilities.h"
#include "common/app_build_configs.h"
#include "common/SplashScreen.h"
#include "common/RhodesApp.h"
#include "common/RhoStd.h"
#include "common/StringConverter.h"
#include "RhoElementsMsgs.h"
#include "../PBCore/Eng.h"
#include "logging/RhoLog.h"

namespace rho
{
CRhoWKBrowserConfig* CRhoWKBrowserConfig::m_pInstance = 0;

extern "C" 
{
    void rho_wmimpl_set_configfilepath(const char* path)
    {
        CRhoWKBrowserConfig::getInstance()->setConfigFilePath(path);
    }
    void rho_wmimpl_set_configfilepath_wchar(const WCHAR* path)
    {
        CRhoWKBrowserConfig::getInstance()->setConfigFilePath(path);
    }
    void rho_wmimpl_set_startpage(const char* path)
    {
        CRhoWKBrowserConfig::getInstance()->setStartPage(path);
    }
    void rho_wmimpl_set_startpage_tchar(const TCHAR* path)
    {
        CRhoWKBrowserConfig::getInstance()->setStartPage(path);
    }
    void rho_wmimpl_set_logpath(const TCHAR* path)
    {
        CRhoWKBrowserConfig::getInstance()->setLogPath(path);
    }
    void rho_wmimpl_set_is_version2(const char* path)
    {
        CRhoWKBrowserConfig::getInstance()->setIsVersion2(path);
    }
    WCHAR* rho_wmimpl_get_configfilepath()
    {
        return CRhoWKBrowserConfig::getInstance()->getConfigFilePath();
    }
	WCHAR* rho_wmimpl_get_BadLinkURLPath()
    {
        return CRhoWKBrowserConfig::getInstance()->getBadLinkURLPath();
    }
    WCHAR* rho_wmimpl_get_HostTrackerDlgMsg()
    {
        return CRhoWKBrowserConfig::getInstance()->getHostTrackerDlgMsg();
    }    
    TCHAR* rho_wmimpl_get_startpage()
    {
        return CRhoWKBrowserConfig::getInstance()->getStartPage();
    }
    const char* rho_wmimpl_get_logurl()
    {
        return CRhoWKBrowserConfig::getInstance()->getLogURL();
    }
    const char* rho_wmimpl_get_httpproxy()
    {
        return CRhoWKBrowserConfig::getInstance()->getHttpProxy();
    }
    const char* rho_wmimpl_get_logpath()
    {
        return CRhoWKBrowserConfig::getInstance()->getLogPath();
    }
	bool rho_wmimpl_sharedconfig_getboolvalue(const wchar_t* szName)
	{
		LPCTSTR pValue = CRhoWKBrowserConfig::getInstance()->getConfig()->GetAppSettingPtr(0, szName, L"Value");
		if (pValue && pValue[0] == L'0')
            return false;

		return true;
	}

    const wchar_t* rho_wmimpl_sharedconfig_getvalue(const wchar_t* szName)
    {
        	return CRhoWKBrowserConfig::getInstance()->getConfig()->GetAppSettingPtr(0, szName, L"Value");
    }
    const wchar_t* rho_wmimpl_getNavTimeOutVal()
    {
		return CRhoWKBrowserConfig::getInstance()->getConfig()->GetSettingPtr(L"Configuration\\Navigation\\NavTimeout", L"Value");
    }
    const wchar_t* rho_wmimpl_getSplashScreenPathVal()
    {
		return CRhoWKBrowserConfig::getInstance()->getConfig()->GetSettingPtr(L"Configuration\\SplashScreen\\SplashScreenPath", L"Value");
    }
    const wchar_t* rho_wmimpl_getSplashScreenDuration()
    {
		return CRhoWKBrowserConfig::getInstance()->getConfig()->GetSettingPtr(L"Configuration\\SplashScreen\\SplashScreenDuration", L"Value");
    }
    bool rho_wmimpl_get_resize_on_sip()
    {
        return CRhoWKBrowserConfig::getInstance()->getResizeOnSIP();
    }
    bool rho_wmimpl_get_fullscreen()
    {
        return CRhoWKBrowserConfig::getInstance()->getFullScreen();
    }
    double rho_wmimpl_get_pagezoom()
    {
 	return CRhoWKBrowserConfig::getInstance()->getPageZoom();
    }
    stHostTrackerConfigInfo* rho_wmimpl_get_hostTrackerInfo()
    {
	return CRhoWKBrowserConfig::getInstance()->getHostTrackerData();
    }
    bool rho_wmimpl_get_is_version2()
    {
        return CRhoWKBrowserConfig::getInstance()->getIsVersion2();
    }
    CConfig* rho_wmimpl_get_webkitconfig()
    {
        return CRhoWKBrowserConfig::getInstance()->getConfig();
    }
	const unsigned int* rho_wmimpl_get_logmaxsize()
	{
		return CRhoWKBrowserConfig::getInstance()->getLogMaxSize();
	}
        int rho_wmimpl_is_loglevel_enabled(int nLogLevel)
	{
		return CRhoWKBrowserConfig::getInstance()->isLogLevelEnabled(nLogLevel);
	}
	const int* rho_wmimpl_get_loglevel()
	{
		return CRhoWKBrowserConfig::getInstance()->getLogLevel();
	}
	bool rho_wmimpl_get_textselectionenabled()
	{
		return CRhoWKBrowserConfig::getInstance()->getTextSelectionEnabled();
	}
	const unsigned int* rho_wmimpl_get_logmemperiod()
	{
		return CRhoWKBrowserConfig::getInstance()->getLogMemPeriod();
	}
	bool rho_wmimpl_get_function_key_enabled(int iFKey)
	{
		return CRhoWKBrowserConfig::getInstance()->getFunctionKeyEnabled(iFKey);
	}
	bool rho_wmimpl_get_application_key_enabled(int iAppKey)
	{
		return CRhoWKBrowserConfig::getInstance()->getApplicationKeyEnabled(iAppKey);
	}
	bool rho_wmimpl_get_function_keys_capturable()
	{
		return CRhoWKBrowserConfig::getInstance()->getFunctionKeysCapturable();
	}
	int rho_wmimpl_get_control_keys_count()
	{
		return CRhoWKBrowserConfig::getInstance()->getControlKeysCount();
	}
	int* rho_wmimpl_get_control_keys()
	{
		return CRhoWKBrowserConfig::getInstance()->getControlKeys();
	}
    const char* rho_wmimpl_get_webengine()
    {
        return CRhoWKBrowserConfig::getInstance()->getWebEngine();
    }
	bool rho_wmimpl_get_custom_dom_elements_path(char* szFileName)
	{
		return CRhoWKBrowserConfig::getInstance()->getCustomDOMElementsPath(szFileName);
}
}


CConfig* CRhoWKBrowserConfig::getConfig()
{
    readConfigFile();
    return m_pConfig;
}

WCHAR* CRhoWKBrowserConfig::getConfigFilePath()
{
    readConfigFile();
    return m_ConfigFilePath;
}
WCHAR* CRhoWKBrowserConfig::getBadLinkURLPath()
{
    readConfigFile();
    return m_BadLinkPage;
}
WCHAR* CRhoWKBrowserConfig::getHostTrackerDlgMsg()
{
    readConfigFile();
    return m_HostTrackerDlgMsg;
}
bool CRhoWKBrowserConfig::getCustomDOMElementsPath(char* szFileName)
{
	bool retStatus = false;
	std::string szCustomPath;
    LPCTSTR szCustomDOMElePath = m_pConfig->GetSettingPtr(L"Configuration\\Applications\\Application\\CustomDOMElements",L"Value");
	if(szCustomDOMElePath)
	{
		std::wstring customDOMElePath (szCustomDOMElePath);
		size_t pos;
		if ((pos = customDOMElePath.find(_T("%INSTALLDIR%"))) != rho::StringW::npos)
			customDOMElePath.replace(pos, 12, m_szInstallDirectory);
		if ((pos = customDOMElePath.find(_T("\\\\"))) != rho::StringW::npos)
			customDOMElePath.replace(pos, 2, _T("\\"));
		 if (customDOMElePath.substr(0,7).compare(_T("file://"))==0)
             customDOMElePath.erase(0,7);

		 szCustomPath = rho::common::convertToStringA(customDOMElePath );
		 strcpy(szFileName, szCustomPath.c_str());
		 retStatus = true;

	}
    return retStatus;
}


TCHAR* CRhoWKBrowserConfig::getStartPage()
{
    readConfigFile();
    return m_StartPage;
}

const char* CRhoWKBrowserConfig::getLogPath()
{
    readConfigFile();
    return m_LogPath;
}

const char* CRhoWKBrowserConfig::getLogURL()
{
    readConfigFile();
    return m_LogURL;
}
const char* CRhoWKBrowserConfig::getHttpProxy()
{
    readConfigFile();
    return m_HttpProxy;
}

const unsigned int* CRhoWKBrowserConfig::getLogMaxSize()
{
    readConfigFile();
    return m_LogMaxSize;
}

int CRhoWKBrowserConfig::isLogLevelEnabled(int nLogLevel)
{
    readConfigFile();
    return m_LogLevels[nLogLevel];
}

const int* CRhoWKBrowserConfig::getLogLevel()
{
    readConfigFile();
    return m_LogLevel;
}

bool CRhoWKBrowserConfig::getTextSelectionEnabled()
{
	return m_isTextSelectionEnabled;
}

bool CRhoWKBrowserConfig::getGeolocationEnabled()
{
    readConfigFile();
    return m_GeolocationEnabled;
}

bool CRhoWKBrowserConfig::getResizeOnSIP()
{
    readConfigFile();
    return m_ResizeOnSIP;
}

bool CRhoWKBrowserConfig::getFullScreen()
{
    readConfigFile();
    return m_FullScreen;
}

double CRhoWKBrowserConfig::getPageZoom()
{
    readConfigFile();
    return m_dPageZoom;
}
stHostTrackerConfigInfo* CRhoWKBrowserConfig::getHostTrackerData(){
	readConfigFile();
	return &m_HostTrackerInfo;
}

bool CRhoWKBrowserConfig::getIsVersion2()
{
    return m_isVersion2;
}

const char* CRhoWKBrowserConfig::getWebEngine()
{
    return m_webEngine;
}

const unsigned int* CRhoWKBrowserConfig::getLogMemPeriod()
{
	return m_LogMemPeriod;
}

void CRhoWKBrowserConfig::setConfigFilePath(const char* path)
{
    StringW wpath;
    rho::common::convertToStringW(path, wpath);
    _tcscpy(m_ConfigFilePath, wpath.c_str());
}

void CRhoWKBrowserConfig::setConfigFilePath(const WCHAR* path)
{
    _tcscpy(m_ConfigFilePath, path);
}

void CRhoWKBrowserConfig::setStartPage(const char* path)
{
    StringW wpath;
    rho::common::convertToStringW(path, wpath);
    setStartPage(wpath.c_str());
}

void CRhoWKBrowserConfig::setStartPage(const TCHAR* tpath)
{
    _tcscpy(m_StartPage, tpath);
    // Store command line parameter in the registry
    HKEY hKey;
    DWORD Disposition;
    RegCreateKeyEx( HKEY_CURRENT_USER, L"Software\\Symbol\\SymbolPB\\Temp", 0, NULL, 0, 0, 0, &hKey, &Disposition ); 
    RegSetValueEx(hKey, L"cmdline", 0, REG_MULTI_SZ, (const BYTE *) tpath, (wcslen(tpath)+1)*2); 
    RegCloseKey(hKey);
}

void CRhoWKBrowserConfig::setLogPath(const TCHAR* tpath)
{
    String logPath = rho::common::convertToStringA(tpath);
    strncpy(m_LogPath, logPath.c_str(), MAX_PATH);
    m_LogPath[MAX_PATH] = 0;
}

void CRhoWKBrowserConfig::setLogURL(const TCHAR* url)
{
    String logURL = rho::common::convertToStringA(url);
    strncpy(m_LogURL, logURL.c_str(), MAX_PATH);
    m_LogURL[MAX_PATH] = 0;
}

void CRhoWKBrowserConfig::setLogMaxSize(const TCHAR* max_size)
{
    String logMaxSize = rho::common::convertToStringA(max_size);
	if (m_LogMaxSize == NULL)
		m_LogMaxSize = (unsigned int*)malloc(sizeof(unsigned int));
    *m_LogMaxSize = 1024 * atoi(logMaxSize.c_str());
}

void CRhoWKBrowserConfig::setWebEngine(const TCHAR* web_engine)
{
    String webEngine = rho::common::convertToStringA(web_engine);
    strncpy(m_webEngine, webEngine.c_str(), MAX_PATH);
    m_webEngine[MAX_PATH] = 0;
}

void CRhoWKBrowserConfig::setLogMemPeriod(const TCHAR* log_mem_period)
{
	if (m_LogMemPeriod == NULL)
		m_LogMemPeriod = (unsigned int*)malloc(sizeof(unsigned int));
	if (log_mem_period) {
		String logMemPeriod = rho::common::convertToStringA(log_mem_period);
		*m_LogMemPeriod = atoi(logMemPeriod.c_str());
	} else {
		*m_LogMemPeriod = 5000;
	}
}

void CRhoWKBrowserConfig::setLogLevels(int* levels, int nSize)
{
	if (m_LogLevels == NULL)
		m_LogLevels = new int[nSize];
	if (m_LogLevel == NULL) {
		m_LogLevel = (int*)malloc(sizeof(int));
		*m_LogLevel = L_FATAL;
	}
	for( int i = 0; i < nSize; i++ )
        if (m_LogLevels[i] = levels[i])
		    switch (i) {
				case rho::common::IRhoExtManager::eLogError:
					*m_LogLevel = L_ERROR;
					break;
				case rho::common::IRhoExtManager::eLogWarning:
					*m_LogLevel = L_WARNING;
					break;
				case rho::common::IRhoExtManager::eLogInfo:
					*m_LogLevel = L_INFO;
					break;
				case rho::common::IRhoExtManager::eLogTrace:
					*m_LogLevel = L_TRACE;
					break;
				//case rho::common::IRhoExtManager::eLogUser:
				//	*m_LogLevel = L_INFO;
				//	break;
				case rho::common::IRhoExtManager::eLogDebug:
					*m_LogLevel = L_TRACE;
					break;
			}
}

void CRhoWKBrowserConfig::setGeolocationEnabled(bool geolocation_enabled)
{
    m_GeolocationEnabled = geolocation_enabled;
}

void CRhoWKBrowserConfig::setResizeOnSIP(bool resizeOnSIP)
{
    m_ResizeOnSIP = resizeOnSIP;
}

void CRhoWKBrowserConfig::setFunctionKeyEnabled(int iFKey, bool fKeyEnabled)
{
	if (iFKey < NUM_F_KEYS)
		m_enableFunctionKeys[iFKey] = fKeyEnabled;
}

bool CRhoWKBrowserConfig::getFunctionKeyEnabled(int iFKey)
{
	return m_enableFunctionKeys[iFKey];
}

void CRhoWKBrowserConfig::setApplicationKeyEnabled(int iAppKey, bool aKeyEnabled)
{
	if (iAppKey < NUM_APP_KEYS)
		m_enableApplicationKeys[iAppKey] = aKeyEnabled;
}

bool CRhoWKBrowserConfig::getApplicationKeyEnabled(int iAKey)
{
	return m_enableApplicationKeys[iAKey];
}

void CRhoWKBrowserConfig::setFunctionKeysCapturable(bool bCapturable)
{
	m_bFunctionKeysCapturable = bCapturable;
}

bool CRhoWKBrowserConfig::getFunctionKeysCapturable()
{
	return m_bFunctionKeysCapturable;
}

int CRhoWKBrowserConfig::getControlKeysCount()
{
	return m_nControlKeys;
}

int* CRhoWKBrowserConfig::getControlKeys()
{
	return m_pControlKeys;
}

CRhoWKBrowserConfig* CRhoWKBrowserConfig::getInstance()
{
    if (!m_pInstance)
        m_pInstance = new CRhoWKBrowserConfig();
    return m_pInstance;
}

void CRhoWKBrowserConfig::setFullScreen(bool full_screen)
{
    m_FullScreen = full_screen;
}

void CRhoWKBrowserConfig::setPageZoom(const TCHAR* page_zoom)
{
	String strPageZoom = rho::common::convertToStringA(page_zoom);
	m_dPageZoom = atof(strPageZoom.c_str());
	/****************************************/
	/*
		
		Issue Description - Setting PageZoom value less than 0 with config.xml makes the character non readable
		Enhancement - Value less than 1.0 should be defaulted to 1.0
		Developer Name - Abhineet Agarwal
		File Name - RhoWKBrowserConfig.cpp
		Function Name - setPageZoom
		Date - 09/06/2014
	*/
	/****************************************/

	if(m_dPageZoom <= 1.0){
		m_dPageZoom = 1.0;
	}
}
 
void CRhoWKBrowserConfig::setIsVersion2(const char* path)
{
	strncpy(m_AppRoot, path, MAX_PATH);
	m_AppRoot[MAX_PATH] = 0;
    m_isVersion2 = true;
}

CRhoWKBrowserConfig::CRhoWKBrowserConfig() :
    m_isConfigRead(false),
    m_FullScreen(false),
	m_dPageZoom(1.0),//Default - no zoom
    m_GeolocationEnabled(false),
    m_ResizeOnSIP(true),
	m_LogMaxSize(0),
	m_LogLevels(0),
	m_LogLevel(0),
	m_LogMemPeriod(0),
	m_isTextSelectionEnabled(0)
{
    ::ZeroMemory(&m_HostTrackerInfo,sizeof(stHostTrackerConfigInfo));
	m_HostTrackerInfo.isTrackConnectionSet = false;
	m_HostTrackerInfo.szHostName = "www.SymbolTechnologies.com";
	//_tcscpy(m_HostTrackerInfo.szHostMessage, TEXT("Establishing Connection..."));
	_tcscpy(m_HostTrackerInfo.szBadLinkURL, TEXT("file://%INSTALLDIR%\\HTML\\BadLink.html"));
	m_HostTrackerInfo.iPingTimeout = 1000;
	m_HostTrackerInfo.iDialogTimeout = 30000;
	m_HostTrackerInfo.iPollInterval = 5000;
	m_HostTrackerInfo.iPort = 80;
	
    m_webEngine[0] = 0;
    m_ConfigFilePath[0] = 0;
    m_StartPage[0] = 0;
    m_LogPath[0] = 0;
    m_LogURL[0] = 0;
	m_HttpProxy[0] = 0;
	m_AppRoot[0] = 0;
    const char* szSharedRuntime = get_app_build_config_item("shared-runtime");
    m_isSharedRuntime = szSharedRuntime && (strcmp(szSharedRuntime, "1") == 0);
	m_isVersion2 = !m_isSharedRuntime;

    if(GetModuleFileName(NULL, m_szInstallDirectory, MAX_PATH)){
        //trim the file name off the end
        WCHAR* pStr = wcsrchr(m_szInstallDirectory,L'\\');
        if(pStr) *(pStr+1) = NULL;
    }

    m_pConfig = new CConfig();
}

bool CRhoWKBrowserConfig::FileExists(LPCTSTR szPath)
{
	if (szPath==NULL) return false;
    rho::StringW path (szPath);
    size_t pos;
    if ((pos = path.find(_T("%INSTALLDIR%"))) != rho::StringW::npos)
        path.replace(pos, 12, m_szInstallDirectory);
    if ((pos = path.find(_T("\\\\"))) != rho::StringW::npos)
        path.replace(pos, 2, _T("\\"));
    if (path.substr(0,7).compare(_T("file://"))==0)
        path.erase(0,7);
	DWORD dwAttrib = GetFileAttributes(path.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void CRhoWKBrowserConfig::DisplayMessage(LPCWSTR title, LPCWSTR format, LPCWSTR str)
{
	WCHAR* szConfigErrorMsg = new WCHAR[MAX_PATH + 40];
	wsprintf(szConfigErrorMsg, format, str);
	MessageBoxW(NULL, szConfigErrorMsg, title, MB_OK);
	delete[] szConfigErrorMsg;
}

void CRhoWKBrowserConfig::readConfigFile()
{
    // don't read Config.xml twice
    if (m_isConfigRead)
        return;

    if (m_isVersion2 && (m_ConfigFilePath[0] == 0) && ((m_AppRoot[0] != 0) || (!m_isSharedRuntime))) {
		// look for Config.xml at rho/apps folder of the RE2 application first
		struct stat st;
		rho::String confPath;
		if (m_AppRoot[0] != 0)
			confPath = m_AppRoot;
		else {
			confPath = rho::common::convertToStringA(m_szInstallDirectory);
			String_replace(confPath, '\\', '/');
			confPath.append("rho/");
		}
		confPath.append("apps/Config.xml");
		if (stat(confPath.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
			rho::StringW confPathW = rho::common::convertToStringW(confPath);
			wcscpy(m_ConfigFilePath,confPathW.c_str());
		}
	}

    if (m_ConfigFilePath[0] == 0) {
		// if path to config file is not specified at command line,
        // then look for Config.xml at the default location
        wcscpy(m_ConfigFilePath,m_szInstallDirectory);
        wcscat(m_ConfigFilePath,L"Config/Config.xml");
    }

    // load Config.xml from file
    if((m_pConfig->Init(m_ConfigFilePath))==NULL)
	{
		DisplayMessage(L"Config Error", L"Please check your config.xml (%s)", m_ConfigFilePath);
        exit(1);
	}
	// assert that RegEx.xml and Plugin.xml exist
	LPCTSTR str;
	if(!FileExists(str = m_pConfig->GetSettingPtr(L"Configuration\\FileLocations\\RegEXFile",L"Value")))
	{
		DisplayMessage(L"Config Error", L"Please check your RegEx.xml (%s)", str);
        exit(1);
	}
	if(!FileExists(str = m_pConfig->GetSettingPtr(L"Configuration\\FileLocations\\PluginFile",L"Value")))
	{
		DisplayMessage(L"Config Error", L"Please check your Plugin.xml (%s)", str);
        exit(1);
	}

    if ((!m_isVersion2) && m_isSharedRuntime) {
        // proceed options specific for RhoElements v1 applications

        LPCTSTR logProtocol = 0;
        LPCTSTR startPage = 0;
        LPCTSTR logURL = 0;
        LPCTSTR logURI = 0;
		LPCTSTR logPort = 0;

        // start page from Config.xml
        startPage = m_pConfig->GetAppSettingPtr(0,L"General\\StartPage",L"Value");

        // logging options from Config.xml
        logProtocol = m_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogProtocol",L"Value");
        logURL = m_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogURI",L"Value");
		logPort = m_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogPort",L"Value");
        if (logProtocol) {
            if (wcsnicmp(logProtocol,L"HTTP",4)==0) {
                // set remote logging URL if present
				if (logURL){
                    if(logPort)
					{
						WCHAR* url = new WCHAR[MAX_PATH];
						wcsncpy(url, logURL, MAX_PATH);
						wcsncpy(url + wcslen(url), L":", MAX_PATH);
						wcsncpy(url + wcslen(url), logPort, MAX_PATH);
						setLogURL(url);
					}
					else
						setLogURL(logURL);
				}
            } else if (wcsnicmp(logProtocol,L"FILE",4)==0) {
                // use log folder from config if it is a file log
                logURI = logURL;
            }
        }
        // use start page folder from command line (if it's present and is file) as log path
        if ((!logURI) && (m_StartPage[0]!=0) && (wcsnicmp(m_StartPage,_T("file://"),7)==0))
            logURI = m_StartPage;
        // use start page folder from config (if it's present and is file) as log path
        if ((!logURI) && startPage && (wcsnicmp(startPage,L"file://",7)==0))
            logURI = startPage;
        // set up logging
        if (logURI) {
            rho::StringW logPath (logURI);
            size_t pos;
            if ((pos = logPath.find(_T("%INSTALLDIR%"))) != rho::StringW::npos)
                logPath.replace(pos, 12, m_szInstallDirectory);
            if ((pos = logPath.find(_T("\\\\"))) != rho::StringW::npos)
                logPath.replace(pos, 2, _T("\\"));
            if (logPath.substr(0,7).compare(_T("file://"))==0)
                logPath.erase(0,7);
            /*rho::StringW logFolder (logPath.substr(0,logPath.find_last_of(_T("/\\"))+1));
            WCHAR szLogFolder[MAX_PATH + 1];
            wcsncpy(szLogFolder,logFolder.c_str(),MAX_PATH);
            szLogFolder[MAX_PATH] = 0;
            setLogPath(szLogFolder);*/
			setLogPath(logPath.c_str());
        }

		// max size of the log in Kb
        LPCTSTR logMaxSize = m_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogMaxSize",L"Value");
		if (logMaxSize)
			setLogMaxSize(logMaxSize);

        int log_levels[rho::common::IRhoExtManager::eLogLevelsCount] = {0};
		LPCTSTR logLevel = m_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogError",L"Value");
        if (logLevel && (wcscmp(logLevel,L"1")==0))
			log_levels[rho::common::IRhoExtManager::eLogError] = 1;
		logLevel = m_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogWarning",L"Value");
        if (logLevel && (wcscmp(logLevel,L"1")==0))
			log_levels[rho::common::IRhoExtManager::eLogWarning] = 1;
		logLevel = m_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogInfo",L"Value");
        if (logLevel && (wcscmp(logLevel,L"1")==0))
			log_levels[rho::common::IRhoExtManager::eLogInfo] = 1;
		logLevel = m_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogTrace",L"Value");
       if (logLevel && (wcscmp(logLevel,L"1")==0))
			log_levels[rho::common::IRhoExtManager::eLogTrace] = 1;	
		logLevel = m_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogUser",L"Value");
        if (logLevel && (wcscmp(logLevel,L"1")==0))
			log_levels[rho::common::IRhoExtManager::eLogUser] = 1;
		logLevel = m_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogDebug",L"Value");
        if (logLevel && (wcscmp(logLevel,L"1")==0))
			log_levels[rho::common::IRhoExtManager::eLogDebug] = 1;

		setLogLevels(log_levels, rho::common::IRhoExtManager::eLogLevelsCount);

		LPCTSTR logMemory = m_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogMemory",L"Value");
		if (logMemory && (wcscmp(logMemory,L"1")==0))
			setLogMemPeriod(m_pConfig->GetSettingPtr(L"Configuration\\Logger\\LogMemPeriod",L"Value"));

        // use full screen value from Config.xml
        LPCTSTR fullScreen = m_pConfig->GetSettingPtr(L"Configuration\\Screen\\FullScreen",L"Value");
        if (fullScreen && (wcscmp(fullScreen,L"1")==0))
            setFullScreen(true);

        // if start page is not specified in command line,
        // then use start page from Config.xml
        if ((m_StartPage[0]==0) && startPage) {
            std::wstring startPagePath (startPage);
            size_t pos;
            if ((pos = startPagePath.find(_T("%INSTALLDIR%"))) != rho::StringW::npos)
                startPagePath.replace(pos, 12, m_szInstallDirectory);
            if ((pos = startPagePath.find(_T("\\\\"))) != rho::StringW::npos)
                startPagePath.replace(pos, 2, _T("\\"));
            setStartPage(startPagePath.c_str());
        }
    }
	try
	{
		//use TrackConnection from Config.xml
		LPCTSTR szIsTrackConnection = m_pConfig->GetSettingPtr(L"Configuration\\VoidConnection\\TrackConnection",L"Value"); 
		if(szIsTrackConnection)
		{
			if(_tcscmp(szIsTrackConnection,TEXT("1")) == 0){
				m_HostTrackerInfo.isTrackConnectionSet = true;		
			}
			if(m_HostTrackerInfo.isTrackConnectionSet)
			{
				//use HostURL from Config.xml
				LPCTSTR szHostName = m_pConfig->GetSettingPtr(L"Configuration\\VoidConnection\\HostURL",L"Value");   
				if(szHostName){

					std::string szHost;
					szHost = rho::common::convertToStringA(szHostName);	


					if (szHost.length() > 7 && szHost.compare(0, 7, "http://") == 0)
					{
						szHost = szHost.substr(7);
					}
					if (szHost.length() > 8 && szHost.compare(0, 8, "https://") == 0)
					{
						szHost = szHost.substr(8);
					}
					int nPos = 0;
					nPos = szHost.find(":");
					if(-1 != nPos)
					{
						//if port number is appended
						std::string szPort  = szHost.substr(nPos + 1);
						m_HostTrackerInfo.iPort = atoi(szPort.c_str());
						m_HostTrackerInfo.szHostName = szHost.substr(0,nPos);

					}
					else
					{
						//if port number not appended
						m_HostTrackerInfo.szHostName = szHost;
					}

				}
				//use Message from Config.xml
				LPCTSTR szMessage = m_pConfig->GetSettingPtr(L"Configuration\\VoidConnection\\Message",L"Value");    
				if(szMessage){
					std::wstring dlgMessage (szMessage);
					_tcscpy(m_HostTrackerDlgMsg, dlgMessage.c_str());
					//_tcscpy(m_HostTrackerInfo.szHostMessage, dlgMessage.c_str());
				}
				//use Timeout from Config.xml
				LPCTSTR szTimeout = m_pConfig->GetSettingPtr(L"Configuration\\VoidConnection\\Timeout",L"Value");    
				if(szTimeout){
					int tempDlgTimeout = _ttoi(szTimeout);
					if(tempDlgTimeout > 30000)
					{
						m_HostTrackerInfo.iDialogTimeout  = tempDlgTimeout;
					}
					else
					{
						LOG(WARNING) + "Connection Checking Extention will default dialog tme out to 30000, Ensure that you have entered a valid value which is not less than 30000";
					}



				}
				//use PollInterval from Config.xml
				LPCTSTR szPollInterval = m_pConfig->GetSettingPtr(L"Configuration\\VoidConnection\\PollInterval",L"Value");
				if(szPollInterval){
					int tempPollInterval= _ttoi(szPollInterval);
					if(tempPollInterval > 5000)
					{
						m_HostTrackerInfo.iPollInterval  = tempPollInterval;
					}
					else
					{
						LOG(WARNING) + "Connection Checking Extention will default pollinterval to 5000, Ensure that you have entered a valid value which is not less than 5000";
					}

				}
				//use BadLinkURI from Config.xml
				LPCTSTR szBadLinkURI = m_pConfig->GetSettingPtr(L"Configuration\\Applications\\Application\\Navigation\\BadLinkURI",L"Value");
				if(szBadLinkURI){


					std::wstring badLinkPath (szBadLinkURI);
					size_t pos;
					if ((pos = badLinkPath.find(_T("%INSTALLDIR%"))) != rho::StringW::npos)
						badLinkPath.replace(pos, 12, m_szInstallDirectory);
					if ((pos = badLinkPath.find(_T("\\\\"))) != rho::StringW::npos)
						badLinkPath.replace(pos, 2, _T("\\"));
					_tcscpy(m_BadLinkPage, badLinkPath.c_str());
					_tcscpy(m_HostTrackerInfo.szBadLinkURL, badLinkPath.c_str());

				}
				//here add additional validation for dialog time out and poll interval
				if((m_HostTrackerInfo.iDialogTimeout/m_HostTrackerInfo.iPollInterval )< 3)
				{
					m_HostTrackerInfo.iDialogTimeout=30000;
					m_HostTrackerInfo.iPollInterval=5000;
					LOG(WARNING) + "Connection checking Dialog timeout must be 3 times greater than poll interval, browser will use default values";
				}

			}
		}
	}
	catch(...)
	{
		LOG(WARNING) + "Unknown Exception while reading VoidConnection params, feature will be disabled";
		m_HostTrackerInfo.isTrackConnectionSet = false;
	}
    LPCTSTR szHttpProxy = m_pConfig->GetSettingPtr(L"Configuration\\Applications\\Application\\HTTP_Proxy",L"Value");
    if(szHttpProxy)
    {
        String httpProxy = rho::common::convertToStringA(szHttpProxy);
        strncpy(m_HttpProxy, httpProxy.c_str(), MAX_PATH);
        m_HttpProxy[MAX_PATH] = 0;
    }
    // use page zoom value from Config.xml
	LPCTSTR pagezoom = m_pConfig->GetSettingPtr(L"Configuration\\Screen\\PageZoom",L"Value");
    if(pagezoom)
		setPageZoom(pagezoom);

    // use geolocation settings
    LPCTSTR geolocationEnabled = m_pConfig->GetSettingPtr(L"Configuration\\Geolocation\\GeolocationEnabled",L"Value");
    if (geolocationEnabled && (wcscmp(geolocationEnabled,L"1")==0))
        setGeolocationEnabled(true);

    // do resize on SIP?
    LPCTSTR resizeOnSIP = m_pConfig->GetAppSettingPtr(0,L"SIP\\ResizeOnSIP",L"Value");
    if (resizeOnSIP && (wcscmp(resizeOnSIP,L"0")==0))
        setResizeOnSIP(false);

	LPCTSTR textSelectionEnabled = m_pConfig->GetAppSettingPtr(0,L"HTMLStyles\\TextSelectionEnabled",L"Value");
    if (textSelectionEnabled && (wcscmp(textSelectionEnabled,L"1")==0))
        m_isTextSelectionEnabled = true;

	//  All function keys capturable?
	LPCTSTR funcKeysCapturable = m_pConfig->GetSettingPtr(L"Configuration\\DeviceKeys\\FunctionKeysCapturable",L"Value");
    if (funcKeysCapturable && (wcscmp(funcKeysCapturable,L"0")==0))
        setFunctionKeysCapturable(false);
	else
		setFunctionKeysCapturable(true);

	TCHAR tcFunctionKeyTag[50];
	memset(tcFunctionKeyTag, 0, sizeof(TCHAR) * 50);
	LPCTSTR pFunctionKeyEnabled;
	for (int i = 1; i <= NUM_F_KEYS; i++)
	{
		wsprintf(tcFunctionKeyTag, L"Configuration\\DeviceKeys\\EnableFunctionKey_F%i", i);
		pFunctionKeyEnabled = 
			m_pConfig->GetSettingPtr(tcFunctionKeyTag, L"Value");
		setFunctionKeyEnabled(i-1, true);
		if (pFunctionKeyEnabled != NULL)
		{
			//  Configuration setting found for this function key, see if it
			//  is zero
			if (*pFunctionKeyEnabled == L'0')
			{
				//  Function key is specified to be blocked
				setFunctionKeyEnabled(i-1, false);
			}
		}
	}

	TCHAR tcApplicationKeyTag[50];
	memset(tcApplicationKeyTag, 0, sizeof(TCHAR) * 50);
	LPCTSTR pApplicationKeyEnabled;
	for (int i = 1; i <= NUM_APP_KEYS; i++)
	{
		wsprintf(tcApplicationKeyTag, L"Configuration\\DeviceKeys\\EnableApplicationKey_A%i", i);
		pApplicationKeyEnabled = 
			m_pConfig->GetSettingPtr(tcApplicationKeyTag, L"Value");
		setApplicationKeyEnabled(i-1, true);
		if (pApplicationKeyEnabled != NULL)
		{
			//  Configuration setting found for this application key, see if it
			//  is zero
			if (*pApplicationKeyEnabled == L'0')
			{
				//  Application key is specified to be blocked
				setApplicationKeyEnabled(i-1, false);
			}
		}
	}

	m_pControlKeys = new int [1];
	m_pControlKeys [0] = 0;
	WCHAR key, setting [64];
	LPCWSTR pvalue;
	int index;

	// Count enabled control keys
	m_nControlKeys = 0;
	for (key = 'A'; key <= 'Z'; key++)
	{
		wsprintf (setting, L"Configuration\\Applications\\Application\\DeviceKeys\\EnableCtrlKey_%c", key);
		pvalue = m_pConfig->GetSettingPtr(setting, L"Value");
		if (pvalue && pvalue [0] == L'1')
			m_nControlKeys++;
	}

	// Allocate control key list, if needed, and fill it
	if (m_nControlKeys == 0)
		m_pControlKeys = NULL;
	else
	{
		m_pControlKeys = new int [m_nControlKeys];

		index = 0;
		for (key = 'A'; key <= 'Z'; key++)
		{
			wsprintf (setting, L"Configuration\\Applications\\Application\\DeviceKeys\\EnableCtrlKey_%c", key);
			pvalue = m_pConfig->GetSettingPtr(setting, L"Value");
			if (pvalue && pvalue [0] == L'1')
				m_pControlKeys [index++] = (int) key;
		}
	}


    //
    LPCTSTR webEngine = m_pConfig->GetSettingPtr(L"Configuration\\Engine\\EngineInUse",L"Value");
    if (webEngine)
        setWebEngine(webEngine);

    m_isConfigRead = true;
}

CRhoWKBrowserConfig::~CRhoWKBrowserConfig()
{
	if (m_LogMemPeriod)
		free(m_LogMemPeriod);
	if (m_LogMaxSize)
		free(m_LogMaxSize);
	if (m_LogLevel)
		free(m_LogLevel);
	if (m_LogLevels)
		delete m_LogLevels;
}

}
