#ifndef _HOST_TRACKER
#define _HOST_TRACKER

#include "..\..\..\..\..\coreapi\ext\platform\wm\src\NetworkDetect.h"



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
	rho::String getHostURL();
    int getDailogTimeout();
    int getPollInterval();
	rho::StringW  getMessage();
private:
	static HWND m_hConnectDlg;
	static HBRUSH m_hbrBackground;
    static rho::StringW m_szConnectionDlgMsg;
    static BOOL CALLBACK ConnectDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	static void CreateNoConnectionDialog(int unused);
    static void GenerateConnectionBoxCoordinates(HWND hwnd, LONG& lLeft, LONG& lTop, LONG& lRight, LONG& lBottom);
    void TimeoutProcHandler(bool bStartThread);
    static DWORD TimeoutProc (LPVOID pparam);
    void ProcessTimeout (void);
    void EventListner (void);	
    rho::StringW CHostTracker::replaceString(rho::StringW inputString,
                      rho::StringW toReplace,
                      rho::StringW replaceWith);	
	void HandleConnectionBox(eConnectionBoxMode mode); 
	void closeAllEvents();
	bool isNavigatedToBadLink();
	rho::StringW m_szBadLinkUrl;
	rho::StringW m_szNavigatedUrl;	
	int m_iConnectionDlgTimeout;
	int m_iPingTimeOut;
	bool m_bIsFeatureEnabled;
	bool m_bIsOnBadlink;
	HANDLE m_hEvents[4];
	HANDLE m_hTimeOutProcCancelEvent;
	HANDLE m_hTimeOutProcId;
	
};

class HostTrackerFactory {
public:
    //implement for concrete platforms
    static CHostTracker* createHostTracker();
};



#endif // !defined( EXAMPLE_H )
