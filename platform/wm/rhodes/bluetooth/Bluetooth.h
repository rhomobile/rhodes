
#pragma once

#include <windows.h>

#ifndef OS_WINDOWS


#include <winsock2.h>
#include <ws2bth.h>
#include <bthapi.h>
#include <bthutil.h>


#define MAX_NAME_SIZE 1024
#define MAX_ADDR_SIZE 32
#define MAX_MESSAGE_SIZE 256

#ifndef ARRAYSIZE
#define ARRAYSIZE(a)   (sizeof(a)/sizeof(a[0]))
#endif












#define RHO_BT_OK  "OK"
#define RHO_BT_CANCEL  "CANCEL"
#define RHO_BT_ERROR  "ERROR"

#define RHO_BT_ROLE_SERVER  "ROLE_SERVER"
#define RHO_BT_ROLE_CLIENT  "ROLE_CLIENT"

#define RHO_BT_SESSION_INPUT_DATA_RECEIVED "SESSION_INPUT_DATA_RECEIVED"
#define RHO_BT_SESSION_DISCONNECT "SESSION_DISCONNECT"

#define RHO_DISCOVER_TIME_MS	5000	
#define RHO_DISCOVERED_TIME_MS	5000
#define SerialPortConnectGUID L"00001101-0000-1000-8000-00805F9B34FB"

class RhoBluetoothManager;


// CRhoBluetoothDiscoverDlg dialog
class CRhoBluetoothDiscoverDlg : public //CDialog
#if defined(OS_WINDOWS)
	COkCancelModalDialog<CRhoBluetoothDiscoverDlg>
#else
	CDialogImpl <CRhoBluetoothDiscoverDlg>
#endif	
{
public:
	CRhoBluetoothDiscoverDlg  ();
	~CRhoBluetoothDiscoverDlg ();

	enum { IDD = IDD_BLUETOOTH_DISCOVER };

	BEGIN_MSG_MAP(CRhoBluetoothDiscoverDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

public:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	int mSelectedDevice;
};



// CRhoBluetoothDiscoveredDlg dialog
class CRhoBluetoothDiscoveredDlg : public //CDialog
#if defined(OS_WINDOWS)
	COkCancelModalDialog<CRhoBluetoothDiscoveredDlg>
#else
	CDialogImpl <CRhoBluetoothDiscoveredDlg>
#endif	
{
public:
	CRhoBluetoothDiscoveredDlg  ();
	~CRhoBluetoothDiscoveredDlg ();

	enum { IDD = IDD_BLUETOOTH_DISCOVERED };

	BEGIN_MSG_MAP(CRhoBluetoothDiscoveredDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

public:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};



struct RhoDeviceList
{ 
	BT_ADDR bthAddress;
	TCHAR bthName[40];
	RhoDeviceList *NextDevice;
};

struct RhoDeviceInfo 
{
	WCHAR szDeviceNameAddr[MAX_NAME_SIZE];

}; 

struct RhoDataBlock {
	RhoDataBlock() {
		buf = NULL;
		size = 0;
		next = NULL;
	}
	void* buf;
	int size;
	RhoDataBlock* next;
};


class RhoDiscoverDlg {
public:
	RhoDiscoverDlg();
	virtual ~RhoDiscoverDlg();

	void openDialog(RhoBluetoothManager* btManager);
	void updateDeviceListFromManager();

private:
	RhoBluetoothManager* mBtManager;
	CRhoBluetoothDiscoverDlg dlg;
	HCURSOR hCurs;
	int mShowCursor;
};

class RhoDiscoveredDlg {
public:
	RhoDiscoveredDlg();
	virtual ~RhoDiscoveredDlg();

	void openDialog(RhoBluetoothManager* btManager);
	void closeDialog();

private:
	RhoBluetoothManager* mBtManager;
	CRhoBluetoothDiscoveredDlg dlg;
	HCURSOR hCurs;
	int mShowCursor;
};



DWORD WINAPI runThreadDiscoverDevices(LPVOID data);

class RhoBluetoothManager {
// public API :
public :
	// Ruby API
	int rho_bluetooth_is_bluetooth_available();
	void rho_bluetooth_off_bluetooth();
	void rho_bluetooth_set_device_name(const char* device_name);
	const char* rho_bluetooth_get_device_name();
	const char* rho_bluetooth_get_last_error();
	const char* rho_bluetooth_create_session(const char* role, const char* callback_url);
	void rho_bluetooth_session_set_callback(const char* connected_device_name, const char* callback_url);
	void rho_bluetooth_session_disconnect(const char* connected_device_name);
	int rho_bluetooth_session_get_status(const char* connected_device_name);
	const char* rho_bluetooth_session_read_string(const char* connected_device_name);
	void rho_bluetooth_session_write_data(const char* connected_device_name, void* buf, int datasize);
	
	// access API
	static RhoBluetoothManager* getInstance();
	static void releaseAll();

	RhoDiscoverDlg* getDiscoverDlg() {return &mDiscoverDlg;}
	RhoDiscoveredDlg* getDiscoveredDlg() {return &mDiscoveredDlg;}

public:
	RhoBluetoothManager();
	virtual ~RhoBluetoothManager();

	int DiscoverDevices();
	int GetNumDevices(){return m_iNumDevices;};
	int GetDeviceInfo(RhoDeviceInfo *pPeerDevicesInfo);
	int GetLocalDeviceName(RhoDeviceInfo *pLocalDeviceInfo);
	int GetDeviceInfo(RhoDeviceInfo *pPeerDeviceInfo, int iSelectedItem);
	int GetGUID(WCHAR *psz, GUID *pGUID) ;



	void fireCreateSessionCallBack(const char* status, const char* connected_device_name);

	void onDiscoverDlgSelectDevice(int index);
	void onDiscoverDlgCancel();
	void onDiscoveredDlgCancel();
	void onServerConnectionCreated();

	RhoDiscoverDlg mDiscoverDlg;
	RhoDiscoveredDlg mDiscoveredDlg;

	void addDataBlock(RhoDataBlock* block);
	void* readAllBlocks(int* size, int additional_space);
	int getBlocksSummarySize();

	void terminateDiscoverThread();
	void terminateDiscoveredThread();

private:

	void fireRhodeCallback(const char* callback_url, const char* body);
	void fireSessionCallBack(const char* connected_device_name, const char* event_type);

	void init();
	void freeAll();
	void freeAllBlocks();
	int getBlocksSummarySizeFromBlock(RhoDataBlock* block);
	
	int RegisterService(BYTE *rgbSdpRecord, int cSdpRecord, int iChannelOffset, UCHAR channel);

	int makeConnection(BT_ADDR bt_addr);

	RhoDeviceList *m_pDeviceList, *m_pStart, *m_pEnd, *m_pCurrentDevice;

	int m_iNumDevices;
	HANDLE m_hReadThread; // thread with read from socket, infinity
	HANDLE m_hDiscoverThred; // thread with find BT devices, 
	HANDLE m_hDiscoveredThread; // thread with discovered this device and read data
	SOCKET m_socketServer;
	SOCKET m_socketClient;
	DWORD m_dwBluetoothMode;
	SOCKADDR_BTH m_saClient;

	bool mIsBluetoothEnabled;

	char mConnectedDeviceName[MAX_NAME_SIZE];
	char mLocalDeviceName[MAX_NAME_SIZE];

	char mCreateSessionCallback[MAX_NAME_SIZE];
	char mSessionCallback[MAX_NAME_SIZE];
	
	RhoDataBlock* mFirstDataBlock;
	RhoDataBlock* mLastDataBlock;

	char* mReadedString;

	static DWORD WINAPI runThreadReadData(LPVOID voidArg);
	static DWORD WINAPI runThreadDiscovered(LPVOID data);

};






























#endif // OS_WINDOWS

extern "C" int rho_bluetooth_is_bluetooth_available();

extern "C" void rho_bluetooth_off_bluetooth();

extern "C" void rho_bluetooth_set_device_name(const char* device_name);

extern "C" const char* rho_bluetooth_get_device_name();

extern "C" const char* rho_bluetooth_get_last_error();

extern "C" const char* rho_bluetooth_create_session(const char* role, const char* callback_url);

extern "C" void rho_bluetooth_session_set_callback(const char* connected_device_name, const char* callback_url);

extern "C" void rho_bluetooth_session_disconnect(const char* connected_device_name);

extern "C" int rho_bluetooth_session_get_status(const char* connected_device_name);

extern "C" const char* rho_bluetooth_session_read_string(const char* connected_device_name);

extern "C" void rho_bluetooth_session_write_string(const char* connected_device_name, const char* str);

extern "C" VALUE rho_bluetooth_session_read_data(const char* connected_device_name);

extern "C" void rho_bluetooth_session_write_data(const char* connected_device_name, VALUE data);


