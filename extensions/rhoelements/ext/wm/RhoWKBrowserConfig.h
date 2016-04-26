#pragma once

#include "..\..\..\..\lib\commonAPI\ConnectionChecking\ext\platform\wm\src\HostTrackerConfigInfo.h"
#include "../PBCore/Config.h"

namespace rho
{

#define NUM_F_KEYS 24
#define NUM_APP_KEYS 16

class CRhoWKBrowserConfig
{
private:
    static CRhoWKBrowserConfig* m_pInstance;
	stHostTrackerConfigInfo m_HostTrackerInfo;

    bool m_isSharedRuntime;
    bool m_isConfigRead;
    WCHAR m_ConfigFilePath[MAX_PATH + 1];
    TCHAR m_StartPage[MAX_PATH + 1];
	TCHAR m_BadLinkPage[MAX_PATH + 1];
	TCHAR m_HostTrackerDlgMsg[MAX_PATH + 1];
    char m_webEngine[MAX_PATH + 1];
    char m_LogPath[MAX_PATH + 1];
    char m_LogURL[MAX_PATH + 1];
    char m_AppRoot[MAX_PATH + 1];
	char m_HttpProxy[MAX_PATH + 1];
    bool m_FullScreen;
	double m_dPageZoom;
    bool m_GeolocationEnabled;
    bool m_ResizeOnSIP;
    bool m_isVersion2;
	bool m_isTextSelectionEnabled;
	unsigned int* m_LogMaxSize;
    int* m_LogLevels;
    int* m_LogLevel;
	unsigned int* m_LogMemPeriod;
	bool m_enableFunctionKeys[NUM_F_KEYS];
	int m_nControlKeys, *m_pControlKeys;	///< Enabled control keys
	bool m_enableApplicationKeys[NUM_APP_KEYS];
	bool m_bFunctionKeysCapturable;

    CConfig* m_pConfig;
    WCHAR m_szInstallDirectory[MAX_PATH + 1];

    CRhoWKBrowserConfig();
    virtual ~CRhoWKBrowserConfig();

    void readConfigFile();
	bool FileExists(LPCTSTR szPath);
	void DisplayMessage(LPCWSTR title, LPCWSTR format, LPCWSTR str);
public:
    static CRhoWKBrowserConfig* getInstance();
    CConfig* getConfig();

    void setConfigFilePath(const char* path);
    void setConfigFilePath(const WCHAR* path);
    void setStartPage(const char* path);
    void setStartPage(const TCHAR* tpath);
    void setLogPath(const TCHAR* tpath);
    void setLogURL(const TCHAR* url);
	void setLogMaxSize(const TCHAR* max_size);
	void setLogLevels(int* levels, int nSize);
	void setLogMemPeriod(const TCHAR* log_mem_period);
    void setWebEngine(const TCHAR* web_engine);

    WCHAR* getConfigFilePath();
    TCHAR* getStartPage();
    const char* getLogPath();
    const char* getLogURL();
	const char* getHttpProxy();
    const char* getWebEngine();
    const unsigned int* getLogMaxSize();
	int isLogLevelEnabled(int nLogLevel);
	const int* getLogLevel();
	const unsigned int* getLogMemPeriod();

    void setFullScreen(bool full_screen);
    bool getFullScreen();
	void setPageZoom(const TCHAR* page_zoom);
 	double getPageZoom();
	stHostTrackerConfigInfo* getHostTrackerData();
	WCHAR* getBadLinkURLPath();
	WCHAR* getHostTrackerDlgMsg();
	bool getCustomDOMElementsPath(char* szFileName);
    void setGeolocationEnabled(bool geolocation_enabled);
    bool getGeolocationEnabled();
    void setResizeOnSIP(bool resizeOnSIP);
    bool getResizeOnSIP();
	void setFunctionKeysCapturable(bool bCapturable);
	bool getFunctionKeysCapturable();
	void setFunctionKeyEnabled(int iFKey, bool fKeyBlocked);
	bool getFunctionKeyEnabled(int iFKey);
	void setApplicationKeyEnabled(int iAppKey, bool aKeyBlocked);
	bool getApplicationKeyEnabled(int iAppKey);
	int getControlKeysCount();
	int* getControlKeys();
    void setIsVersion2(const char* path);
    bool getIsVersion2();
	bool getTextSelectionEnabled();

    bool isSharedRuntime(){ return m_isSharedRuntime; }
    bool isHybridApp(){ return isSharedRuntime() && !getIsVersion2(); }
};

}
