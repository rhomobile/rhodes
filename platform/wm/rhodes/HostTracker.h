#ifndef _HOST_TRACKER
#define _HOST_TRACKER

#include "..\..\..\lib\commonAPI\coreapi\ext\platform\wm\src\NetworkDetect.h"


class CHostTracker : public CNetworkDetection
{
public:
	
	CHostTracker();
	virtual ~CHostTracker();
	virtual void run();	
	virtual bool SetConnectionTimeout (unsigned int iTimeout);
	virtual bool Initialise();
	virtual bool SetConnectionDlgTimeout (unsigned int iTimeout);
	virtual bool SetBadLinkUrl(const wchar_t* badLinkUrl);
	virtual bool SetNavigatedUrl(const wchar_t* navigatedUrl);
	virtual bool isWaitingForWindowMsg();
	virtual bool isFeatureEnabled() { return m_bIsFeatureEnabled;}
private:
	static HWND m_hConnectDlg;
	static rho::StringW m_szConnectionDlgMsg;
	static DWORD ConnectionDlgCreator(LPVOID lParam);
	static BOOL CALLBACK ConnectDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	rho::StringW CHostTracker::replaceString(rho::StringW inputString,
                      rho::StringW toReplace,
                      rho::StringW replaceWith);
	rho::StringW m_szBadLinkUrl;
	rho::StringW m_szNavigatedUrl;	
	unsigned int m_iConnectionDlgTimeout;
	unsigned int m_iPingTimeOut;
	bool m_bIsWaiting;
	bool m_bIsFeatureEnabled;
	
};

class HostTrackerFactory {
public:
    //implement for concrete platforms
    static CHostTracker* createHostTracker();
};



#endif // !defined( EXAMPLE_H )
