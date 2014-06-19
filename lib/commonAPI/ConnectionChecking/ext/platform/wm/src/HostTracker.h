#ifndef _HOST_TRACKER
#define _HOST_TRACKER

#include "..\..\..\..\..\coreapi\ext\platform\wm\src\NetworkDetect.h"

const UINT WM_ON_CONNECTION_BOX   = WM_USER + 201;

typedef enum 
{
	eCancelEventIndex=0,
	eLicenseScreenPopupEventIndex,
	eLicenseScreenHidesEventIndex,
	eNavigateCompleteEventIndex,
	eMaxEventCount
}eEventIndex;
typedef enum 
{
	eShowConnectionBox =0,
	eHideConnectionBox,
	eNone      
}eConnectionBoxMode;

class CHostTracker :  public CNetworkDetection
{
public:
	
	CHostTracker();
	virtual ~CHostTracker();
	virtual void run();	
	virtual bool SetConnectionTimeout (int iTimeout);
	virtual bool StartNetworkChecking();
	virtual bool StopNetworkChecking();
	void Cleanup();
	virtual bool InitConfig(); 
	virtual bool SetConnectionDlgTimeout (int iTimeout);
	virtual bool SetBadLinkUrl(const wchar_t* badLinkUrl);
	virtual bool SetNavigatedUrl(const wchar_t* navigatedUrl);
	virtual bool isFeatureEnabled() { return m_bIsFeatureEnabled;}
	virtual void fireEvent(eEventIndex eventIdx);
	virtual bool onWndMsg(MSG& oMsg);
	rho::String getHostURL();
    int getDailogTimeout();
    int getPollInterval();
	rho::StringW  getMessage();
private:
	static HWND m_hConnectDlg;
	static rho::StringW m_szConnectionDlgMsg;
	static BOOL CALLBACK ConnectDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	rho::StringW CHostTracker::replaceString(rho::StringW inputString,
                      rho::StringW toReplace,
                      rho::StringW replaceWith);
	eConnectionBoxMode OnTimeoutEvent(int& nPollInterval, int& nBadLinkTimer);
	eConnectionBoxMode OnNavCompleteEvent(int& nPollInterval, int& nBadLinkTimer);
	void HandleConnectionBox(eConnectionBoxMode& mode);     
	void closeAllEvents();
	rho::StringW m_szBadLinkUrl;
	rho::StringW m_szNavigatedUrl;	
	int m_iConnectionDlgTimeout;
	int m_iPingTimeOut;
	bool m_bIsFeatureEnabled;
	bool m_bIsOnBadlink;
	HANDLE m_hEvents[4];
	
};

class HostTrackerFactory {
public:
    //implement for concrete platforms
    static CHostTracker* createHostTracker();
};



#endif // !defined( EXAMPLE_H )
