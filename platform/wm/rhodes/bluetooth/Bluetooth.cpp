#include "stdafx.h"


#ifndef OS_WINDOWS


#if defined(_WIN32_WCE)
#include <aygshell.h>
#endif
#include <atltime.h>
#include "ext/rho/rhoruby.h"
#include "../MainWindow.h"
#include "Bluetooth.h"
#include "common/RhodesApp.h"

#include "../logging/RhoLog.h"

#include "../Utils.h"


#ifdef _MSC_VER
// warning C4800: 'int' : forcing to bool 'true' or 'false' (performance warning)
#pragma warning ( disable : 4800 )
#endif

extern "C" HWND getMainWnd();

#if defined(_WIN32_WCE)
// strdup is implemented as part of ruby CE port
extern "C" char *strdup(const char * str);
extern "C" wchar_t* wce_mbtowc(const char* a);
extern "C" char* wce_wctomb(const wchar_t*);

#endif


extern "C" int rho_sys_get_screen_height();
extern "C" int rho_sys_get_screen_width();


BYTE rgbSdpRecord[] = {
	0x35, 0x4d, 0x09, 0x00, 0x01, 0x35, 0x11, 0x1c,
	0x00, 0x00, 0x11, 0x01, 0x00, 0x00, 0x10, 0x00,
	0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb,
	0x09, 0x00, 0x04, 0x35, 0x0c, 0x35, 0x03, 0x19,
	0x01, 0x00, 0x35, 0x05, 0x19, 0x00, 0x03, 0x08,
	0x1a, 0x09, 0x00, 0x06, 0x35, 0x09, 0x09, 0x65,
	0x6e, 0x09, 0x00, 0x6a, 0x09, 0x01, 0x00, 0x09,
	0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11,
	0x05, 0x09, 0x01, 0x00, 0x09, 0x01, 0x00, 0x25,
	0x06, 0x53, 0x65, 0x72, 0x69, 0x61, 0x6c
};

//SDP record size constant returned by bthnscreate.exe
#define SDP_RECORD_SIZE 0x0000004f
#define SDP_CHANNEL_OFFSET 40

//server GUID
//WCHAR strGUID[]=L"29 F9 C0 FD-BB 6E-47 97-9F A9-3E C9 A8 54 29 0C";
//WCHAR strGUID[]=L"00 00 11 01-00 00-10 00-80 00-00 80 5F 9B 34 FB";
WCHAR strGUID[]=L"00001101-0000-1000-8000-00805F9B34FB";



DWORD WINAPI runThreadOpenDiscoverDlg(LPVOID data) {
	return 0;
}

DWORD WINAPI runThreadOpenDiscoveredDlg(LPVOID data) {
	return 0;
}

DWORD WINAPI runThreadDiscoverDevices(LPVOID data) {
	RhoBluetoothManager::getInstance()->DiscoverDevices();
	int iNumDevices=0, iRetVal=0;
	iRetVal=RhoBluetoothManager::getInstance()->DiscoverDevices();
	RhoBluetoothManager::getInstance()->mDiscoverDlg.updateDeviceListFromManager();
	RhoBluetoothManager::getInstance()->terminateDiscoverThread();
	return 0;
}

/*
iRetVal=objBthUtils.OpenServerConnection(rgbSdpRecord, SDP_RECORD_SIZE, SDP_CHANNEL_OFFSET, DisplayMessage);
if(iRetVal!=0)
{
	StringCchPrintf(szMessage, ARRAYSIZE(szMessage), L"%d: Server socket failed.", iRetVal); 
	SendMessage(GetDlgItem(hDlg, IDC_MESSAGES), LB_ADDSTRING,0,(LPARAM)szMessage);					
}
else
{
	StringCchPrintf(szMessage, ARRAYSIZE(szMessage), L"You are now ready to receive messages.");
	SendMessage(GetDlgItem(hDlg, IDC_MESSAGES), LB_ADDSTRING,0,(LPARAM)szMessage);		
}

OpenServerConnection(BYTE *rgbSdpRecord, int cSdpRecord, int iChannelOffset, void (*funcPtr)(WCHAR *))
*/

DWORD WINAPI RhoBluetoothManager::runThreadDiscovered(LPVOID data) {
	int iNameLen=0;
	RhoBluetoothManager* mManager = RhoBluetoothManager::getInstance();
	if(mManager->m_socketServer!=INVALID_SOCKET) {
		mManager->fireCreateSessionCallBack(RHO_BT_ERROR, "");
		return 0;
	}
	mManager->m_socketServer = socket (AF_BT, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (mManager->m_socketServer  == INVALID_SOCKET) 
	{
		mManager->fireCreateSessionCallBack(RHO_BT_ERROR, "");
		return WSAGetLastError ();
	}

	SOCKADDR_BTH sa;
	memset (&sa, 0, sizeof(sa));
	sa.addressFamily = AF_BT;
	sa.port = 0;
	if (bind (mManager->m_socketServer, (SOCKADDR *)&sa, sizeof(sa))) 
	{
		mManager->fireCreateSessionCallBack(RHO_BT_ERROR, "");
		return WSAGetLastError ();
	}
	iNameLen = sizeof(sa);
	if (getsockname(mManager->m_socketServer, (SOCKADDR *)&sa, &iNameLen))	
	{
		mManager->fireCreateSessionCallBack(RHO_BT_ERROR, "");
		return WSAGetLastError ();
	}

	if(mManager->RegisterService(rgbSdpRecord, SDP_RECORD_SIZE, SDP_CHANNEL_OFFSET, (UCHAR)sa.port)!=0)
		mManager->fireCreateSessionCallBack(RHO_BT_ERROR, "");
		return WSAGetLastError();

	if (listen (mManager->m_socketServer, SOMAXCONN)) 
	{
		mManager->fireCreateSessionCallBack(RHO_BT_ERROR, "");
		return WSAGetLastError ();
	}
	int iSize=0, cbBytesRecd=0 ;
	WCHAR szMessage[MAX_MESSAGE_SIZE];
	char pbuf[MAX_MESSAGE_SIZE];
	iSize = sizeof(mManager->m_saClient);
	BTH_REMOTE_NAME remotename;
	memset(&remotename, sizeof(remotename), 0);

	SOCKET s = accept (mManager->m_socketServer, (SOCKADDR*)&(mManager->m_saClient), &iSize);
	remotename.bt = mManager->m_saClient.btAddr;

	setsockopt(mManager->m_socketServer,SOL_RFCOMM, SO_BTH_SET_READ_REMOTE_NAME, (char*)&remotename, sizeof(remotename));

	sockaddr peer;
	int size = sizeof(peer);
	if (getpeername( mManager->m_socketServer, &peer, &size)) {
		mManager->fireCreateSessionCallBack(RHO_BT_ERROR, "");
		return WSAGetLastError ();
	}
	memcpy(mManager->mConnectedDeviceName, peer.sa_data, size);
	mManager->mConnectedDeviceName[size] = 0;



	if (s != INVALID_SOCKET) 
	{
		for ( ; ; ) 
		{
			//receive data in pbuf
			cbBytesRecd = recv (s, pbuf, MAX_MESSAGE_SIZE, 0);
			//if error occured in receiving, return error code
			if (cbBytesRecd == SOCKET_ERROR) 
			{
				mManager->fireSessionCallBack(mManager->mConnectedDeviceName, RHO_BT_ERROR);
				return WSAGetLastError();
			}
			else
			{
				// something was received, then copy the contents in szMessage
				if(cbBytesRecd>0)
				{
					RhoDataBlock* block = new RhoDataBlock;
					block->buf = new unsigned char[cbBytesRecd];
					memcpy(block->buf, pbuf, cbBytesRecd);
					block->size = cbBytesRecd;
					mManager->addDataBlock(block);
					LOG(INFO)  + "RhoBluetoothManager::runThreadReadData() read data from Server Socket";
					mManager->fireSessionCallBack(mManager->mConnectedDeviceName, RHO_BT_SESSION_INPUT_DATA_RECEIVED);
				}
			}
		}
	}
	else {
		mManager->fireCreateSessionCallBack(RHO_BT_ERROR, "");
		mManager->terminateDiscoveredThread();
	}
	return 0;
}

void RhoBluetoothManager::onServerConnectionCreated() {
	getDiscoveredDlg()->closeDialog();
	fireCreateSessionCallBack(RHO_BT_OK, mConnectedDeviceName);
}


int RhoBluetoothManager::RegisterService(BYTE *rgbSdpRecord, int cSdpRecord, int iChannelOffset, UCHAR channel)
{
	ULONG recordHandle = 0;

	struct bigBlob
	{
		BTHNS_SETBLOB   b;

	}*pBigBlob;

	pBigBlob = (bigBlob *)malloc(sizeof(struct bigBlob)+cSdpRecord);
	ULONG ulSdpVersion = BTH_SDP_VERSION;
	pBigBlob->b.pRecordHandle   = &recordHandle;
	pBigBlob->b.pSdpVersion     = &ulSdpVersion;
	pBigBlob->b.fSecurity       = 0;
	pBigBlob->b.fOptions        = 0;
	pBigBlob->b.ulRecordLength  = cSdpRecord;

	memcpy (pBigBlob->b.pRecord, rgbSdpRecord, cSdpRecord);
	pBigBlob->b.pRecord[iChannelOffset] = (unsigned char)channel;
	BLOB blob;
	blob.cbSize    = sizeof(BTHNS_SETBLOB) + cSdpRecord - 1;
	blob.pBlobData = (PBYTE) pBigBlob;

	WSAQUERYSET Service;
	memset (&Service, 0, sizeof(Service));
	Service.dwSize = sizeof(Service);
	Service.lpBlob = &blob;
	Service.dwNameSpace = NS_BTH;
	if (WSASetService(&Service,RNRSERVICE_REGISTER,0) == SOCKET_ERROR)
	{
		free(pBigBlob);
		return WSAGetLastError();
	}
	else
	{
		free(pBigBlob);
		return 0;
	}
}



static RhoBluetoothManager* ourRhoBluetoothManager = NULL;

RhoBluetoothManager::RhoBluetoothManager() {
	init();
}

RhoBluetoothManager::~RhoBluetoothManager() {
	freeAll();
}



RhoBluetoothManager* RhoBluetoothManager::getInstance() {
	if (ourRhoBluetoothManager == NULL) {
		ourRhoBluetoothManager = new RhoBluetoothManager();
	}
	return ourRhoBluetoothManager;
}

void RhoBluetoothManager::releaseAll() {
	if (ourRhoBluetoothManager != NULL) {
		delete ourRhoBluetoothManager;
		ourRhoBluetoothManager = NULL;
	}
}

int RhoBluetoothManager::rho_bluetooth_is_bluetooth_available() {
	return mIsBluetoothEnabled;
}

void RhoBluetoothManager::rho_bluetooth_off_bluetooth() {
	releaseAll();
}

void RhoBluetoothManager::rho_bluetooth_set_device_name(const char* device_name) {

}

const char* RhoBluetoothManager::rho_bluetooth_get_device_name() {
	return mLocalDeviceName;
}

const char* RhoBluetoothManager::rho_bluetooth_get_last_error() {
	return RHO_BT_OK;
}

const char* RhoBluetoothManager::rho_bluetooth_create_session(const char* role, const char* callback_url) {
	LOG(INFO)  + "RhoBluetoothManager::rho_bluetooth_create_session( "+role+", "+callback_url+")";
	strcpy(mCreateSessionCallback,callback_url);
	if (strcmp(role, RHO_BT_ROLE_CLIENT) == 0) 
	{
		DWORD data = 0;
		CreateThread(NULL, 0, runThreadDiscoverDevices, (LPVOID)data, 0, NULL);
		//mDiscoverDlg.openDialog(this);
		HWND main_wnd = getMainWnd();
		::PostMessage(main_wnd, WM_BLUETOOTH_DISCOVER, 0, 0);
	}
	else 
	{
		DWORD data = 0;
		//mDiscoveredDlg.openDialog(this);
		HWND main_wnd = getMainWnd();
		::PostMessage(main_wnd, WM_BLUETOOTH_DISCOVERED, 0, 0);
		CreateThread(NULL, 0, runThreadDiscovered, (LPVOID)data, 0, NULL);
	}
	return RHO_BT_OK;
}

void RhoBluetoothManager::rho_bluetooth_session_set_callback(const char* connected_device_name, const char* callback_url) {
	LOG(INFO)  + "RhoBluetoothManager::rho_bluetooth_session_set_callback( "+connected_device_name+", "+callback_url+")";
	strcpy(mSessionCallback,callback_url);
}

void RhoBluetoothManager::rho_bluetooth_session_disconnect(const char* connected_device_name) {
	LOG(INFO)  + "RhoBluetoothManager::rho_bluetooth_session_disconnect("+connected_device_name+")";
	if (strcmp(connected_device_name, mConnectedDeviceName) != 0) {
		return;
	}
	freeAll();
	init();
	fireSessionCallBack(connected_device_name, RHO_BT_SESSION_DISCONNECT);
}

int RhoBluetoothManager::rho_bluetooth_session_get_status(const char* connected_device_name) {
	return getBlocksSummarySize();
}

const char* RhoBluetoothManager::rho_bluetooth_session_read_string(const char* connected_device_name) {
	LOG(INFO)  + "RhoBluetoothManager::rho_bluetooth_session_read_string("+connected_device_name+")";
	if (mReadedString) {
		delete mReadedString;
		mReadedString = NULL;
	}
	int size = 0;
	mReadedString = (char*)readAllBlocks(&size, 1);
	mReadedString[size] = 0;
	LOG(INFO)  + "RhoBluetoothManager::rho_bluetooth_session_read_string() readed:"+mReadedString;
	return mReadedString;
}

void RhoBluetoothManager::rho_bluetooth_session_write_data(const char* connected_device_name, void* buf, int datasize) {
	LOG(INFO)  + "RhoBluetoothManager::rho_bluetooth_session_write_data()";
	int iBytesSent = send (m_socketClient, (char *)buf, datasize, 0);
	if (iBytesSent != datasize)
	{
		fireSessionCallBack(mConnectedDeviceName, RHO_BT_ERROR);
		return;
		//return WSAGetLastError ();
	}
}

void RhoBluetoothManager::fireCreateSessionCallBack(const char* status, const char* connected_device_name) {
	if (strlen(mCreateSessionCallback) <= 0) {
		return;
	}
	char body[2048];
	strcpy(body, "&status=");
	strcat(body, status);
	strcat(body, "&connected_device_name=");
	strcat(body, connected_device_name);
	fireRhodeCallback(mCreateSessionCallback, body);
}

void RhoBluetoothManager::fireSessionCallBack(const char* connected_device_name, const char* event_type) {
	if (strlen(mSessionCallback) <= 0) {
		return;
	}
	char body[2048];
	strcpy(body, "&connected_device_name=");
	strcat(body, connected_device_name);
	strcat(body, "&event_type=");
	strcat(body, event_type);
	fireRhodeCallback(mSessionCallback, body);
}

void RhoBluetoothManager::fireRhodeCallback(const char* callback_url, const char* body) {
	LOG(INFO)  + "RhoBluetoothManager::fireRhodeCallback( "+callback_url+", "+body+")";
	rho_net_request_with_data(rho_http_normalizeurl(callback_url), body);
	//rho_rhodesapp_callBluetoothCallback(callback_url, body);
	/*
	HWND main_wnd = getMainWnd();
	char* mcallback = new char[strlen(callback_url)+1];
	strcpy(mcallback, callback_url);
	char* mbody = new char[strlen(body)+1];
	strcpy(mbody, body);
	::PostMessage(main_wnd, WM_BLUETOOTH_CALLBACK, (WPARAM)mcallback, (LPARAM)mbody);
	*/
}


void RhoBluetoothManager::init() {
	LOG(INFO)  + "RhoBluetoothManager::init()";
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 2 );
	WSAStartup( wVersionRequested, &wsaData );

	m_pDeviceList			= NULL;
	m_pStart				= NULL;
	m_pEnd					= NULL;
	m_pCurrentDevice		= NULL;
	m_iNumDevices			= 0;
	//pCallBackFunction		= NULL;
	m_hReadThread			= NULL;
	m_socketServer			= INVALID_SOCKET;
	m_socketClient			= INVALID_SOCKET;
	BthGetMode(&m_dwBluetoothMode);
	if(m_dwBluetoothMode==BTH_POWER_OFF)
	{
		BthSetMode(BTH_DISCOVERABLE);
	}
	RhoDeviceInfo devInfo;
	GetLocalDeviceName(&devInfo);

	m_hDiscoverThred = NULL;
	m_hDiscoveredThread = NULL;
	mReadedString = NULL;
	
	WideCharToMultiByte(CP_UTF8, 0, devInfo.szDeviceNameAddr, -1, mLocalDeviceName, MAX_NAME_SIZE, 0, 0);
	
	mIsBluetoothEnabled = true;
	strcpy(mConnectedDeviceName,"");
	strcpy(mCreateSessionCallback,"");
	strcpy(mSessionCallback,"");
}


void RhoBluetoothManager::terminateDiscoverThread() {
	if (m_hDiscoverThred) {
		DWORD dwExitCode = 0;
		TerminateThread(m_hDiscoverThred, dwExitCode);
		m_hDiscoverThred = NULL;
	}
}

void RhoBluetoothManager::terminateDiscoveredThread() {
	if (m_hDiscoveredThread) {
		DWORD dwExitCode = 0;
		TerminateThread(m_hDiscoveredThread, dwExitCode);
		m_hDiscoveredThread = NULL;
	}
}


void RhoBluetoothManager::freeAll() {
	LOG(INFO)  + "RhoBluetoothManager::freeAll()";
	//Set radio mode back to original state
	BthSetMode(m_dwBluetoothMode);
	if(m_pStart)
	{
		for(m_pCurrentDevice	= m_pStart;m_pCurrentDevice;)
		{
			RhoDeviceList *temp	= m_pCurrentDevice;
			m_pCurrentDevice	= m_pCurrentDevice->NextDevice;
			free(temp);
		}
		m_pStart=NULL;
	}

	if(m_socketClient) {
		closesocket (m_socketClient);
	}
	if(m_socketServer) {
		closesocket (m_socketServer);
	}

	//Terminate the read thread that receives chat messages from the client
	if(m_hReadThread)
	{
		DWORD dwExitCode = 0;
		TerminateThread(m_hReadThread, dwExitCode);
	}
	if (m_hDiscoverThred) {
		DWORD dwExitCode = 0;
		TerminateThread(m_hDiscoverThred, dwExitCode);
	}
	if (m_hDiscoveredThread) {
		DWORD dwExitCode = 0;
		TerminateThread(m_hDiscoveredThread, dwExitCode);
	}
	if (mReadedString) {
		delete mReadedString;
		mReadedString = NULL;
	}

	WSACleanup();

	freeAllBlocks();
}


void RhoBluetoothManager::addDataBlock(RhoDataBlock* block) {
	if (mFirstDataBlock == NULL) {
		mFirstDataBlock = block;
		mLastDataBlock = block;
		block->next = NULL;
		return;
	}
	if (mLastDataBlock != NULL) {
		mLastDataBlock->next = block;
		mLastDataBlock = block;
		block->next = NULL;
		return;
	}
}

void* RhoBluetoothManager::readAllBlocks(int* size, int additional_space) {
	RhoDataBlock* first = mFirstDataBlock;
	mFirstDataBlock = NULL;
	mLastDataBlock = NULL;
	int summsize = getBlocksSummarySizeFromBlock(first);
	if (summsize <= 0) {
		*size = 0;
		return NULL;
	}
	unsigned char* buf = new unsigned char[summsize+additional_space];
	unsigned char* p = buf;
	RhoDataBlock* b = first;
	while (b != NULL) {
		memcpy(p, b->buf, b->size);
		p+= b->size;
		b = b->next;
	}
	*size = summsize;
	return buf;
}

void RhoBluetoothManager::freeAllBlocks() {
	RhoDataBlock* b = mFirstDataBlock;
	mFirstDataBlock = NULL;
	mLastDataBlock = NULL;
	while (b != NULL) {
		RhoDataBlock* d = b;
		b = b->next;
		delete d;
	}
}

int RhoBluetoothManager::getBlocksSummarySize() {
	LOG(INFO)  + "RhoBluetoothManager::getBlocksSummarySize()";
	int size = getBlocksSummarySizeFromBlock(mFirstDataBlock);
	return size;
}

int RhoBluetoothManager::getBlocksSummarySizeFromBlock(RhoDataBlock* block) {
	int summsize = 0;
	RhoDataBlock* b = block;
	while (b != NULL) {
		summsize += b->size;
		b = b->next;
	}
	return summsize;
}




int RhoBluetoothManager::DiscoverDevices() {
	LOG(INFO)  + "RhoBluetoothManager::DiscoverDevices() START";
	WSAQUERYSET		wsaq;
	HANDLE			hLookup;
	RhoDeviceList *	tempDevice;

	union {
		CHAR buf[5000];
		double __unused;	// ensure proper alignment
	};

	LPWSAQUERYSET pwsaResults = (LPWSAQUERYSET) buf;
	DWORD dwSize  = sizeof(buf);
	BOOL bHaveName;

	ZeroMemory(&wsaq, sizeof(wsaq));
	wsaq.dwSize = sizeof(wsaq);
	wsaq.dwNameSpace = NS_BTH;
	wsaq.lpcsaBuffer = NULL;

	if (ERROR_SUCCESS != WSALookupServiceBegin (&wsaq, LUP_CONTAINERS, &hLookup))
	{
		LOG(INFO)  + "RhoBluetoothManager::DiscoverDevices() return ERROR";
		return WSAGetLastError();
	}

	ZeroMemory(pwsaResults, sizeof(WSAQUERYSET));
	pwsaResults->dwSize = sizeof(WSAQUERYSET);
	pwsaResults->dwNameSpace = NS_BTH;
	pwsaResults->lpBlob = NULL;

	if(m_pStart)
	{
		for(m_pCurrentDevice=m_pStart;m_pCurrentDevice;)
		{
			RhoDeviceList *temp=m_pCurrentDevice;
			m_pCurrentDevice=m_pCurrentDevice->NextDevice;
			free(temp);
		}
	}
	m_pEnd=m_pStart=NULL;
	m_iNumDevices=0;
	while (true)
	{	
		if(WSALookupServiceNext (hLookup, LUP_RETURN_NAME | LUP_RETURN_ADDR, &dwSize, pwsaResults)!=ERROR_SUCCESS)
			break;
		ASSERT (pwsaResults->dwNumberOfCsAddrs == 1);
		//Populate the link list		
		tempDevice=(RhoDeviceList*)malloc(sizeof(RhoDeviceList));
		tempDevice->NextDevice=NULL;
		if(m_pStart==NULL)
		{
			m_pStart = tempDevice;
			m_pEnd=m_pStart;
		}
		else
		{
			m_pEnd->NextDevice =tempDevice;
			m_pEnd=tempDevice;
		}
		m_iNumDevices++;
		m_pEnd->bthAddress = ((SOCKADDR_BTH *)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr;
		bHaveName = pwsaResults->lpszServiceInstanceName && *(pwsaResults->lpszServiceInstanceName);
		//If device name is available, add to node
		StringCchPrintf(m_pEnd->bthName, ARRAYSIZE(m_pEnd->bthName),L"%s",bHaveName ? pwsaResults->lpszServiceInstanceName : L"");
		{
			char name[1024];
			WideCharToMultiByte(CP_UTF8, 0, m_pEnd->bthName, -1, name, 1024, 0, 0);
			LOG(INFO)  + "RhoBluetoothManager::DiscoverDevices() FOUND = "+name;
		}
	}

	WSALookupServiceEnd(hLookup);
	LOG(INFO)  + "RhoBluetoothManager::DiscoverDevices() FINISH";
	//	LeaveCriticalSection(&criticalSection);
	return 0;
}


DWORD WINAPI RhoBluetoothManager::runThreadReadData(LPVOID voidArg) {
	LOG(INFO)  + "RhoBluetoothManager::runThreadReadData() START";
	int iSize=0, cbBytesRecd=0 ;
	//WCHAR szMessage[MAX_MESSAGE_SIZE];
	char pbuf[MAX_MESSAGE_SIZE];
	RhoBluetoothManager* functionInfo = (RhoBluetoothManager*) voidArg;
	iSize = sizeof(functionInfo->m_saClient);
	//BTH_REMOTE_NAME remotename;
	//memset(&remotename, sizeof(remotename), 0);

	SOCKET s = functionInfo->m_socketClient;//accept (functionInfo->m_socketServer, (SOCKADDR*)&(functionInfo->m_saClient), &iSize);
	//remotename.bt = functionInfo->m_saClient.btAddr;

	//setsockopt(functionInfo->m_socketServer,SOL_RFCOMM, SO_BTH_SET_READ_REMOTE_NAME, (char*)&remotename, sizeof(remotename));

	if (s != INVALID_SOCKET) 
	{
		for ( ; ; ) 
		{
			//receive data in pbuf
			cbBytesRecd = recv (s, pbuf, MAX_MESSAGE_SIZE, 0);
			//if error occured in receiving, return error code
			if (cbBytesRecd == SOCKET_ERROR) 
			{
				LOG(INFO)  + "RhoBluetoothManager::runThreadReadData() exit with error";
				functionInfo->fireSessionCallBack(functionInfo->mConnectedDeviceName, RHO_BT_ERROR);
				return WSAGetLastError();
			}
			else
			{
				// something was received, then copy the contents in szMessage
				if(cbBytesRecd>0)
				{
					RhoDataBlock* block = new RhoDataBlock;
					block->buf = new unsigned char[cbBytesRecd];
					memcpy(block->buf, pbuf, cbBytesRecd);
					block->size = cbBytesRecd;
					functionInfo->addDataBlock(block);
					LOG(INFO)  + "RhoBluetoothManager::runThreadReadData() read data from Socket";
					functionInfo->fireSessionCallBack(functionInfo->mConnectedDeviceName, RHO_BT_SESSION_INPUT_DATA_RECEIVED);
					//StringCchPrintf (szMessage, ARRAYSIZE(szMessage), L"%s", pbuf);
					//int o;
					//o = 9;
					//(*functionInfo->pCallBackFunction)(szMessage);
				}
			}
		}
	}
	return 0;
}

int RhoBluetoothManager::makeConnection(BT_ADDR bt_addr) {
	LOG(INFO)  + "RhoBluetoothManager::makeConnection() START";
	// client part
	if (m_socketClient==INVALID_SOCKET)
	{
		GUID ServerGuid;

		if(GetGUID(SerialPortConnectGUID, &ServerGuid))
			return -1;
		m_socketClient = socket (AF_BT, SOCK_STREAM, BTHPROTO_RFCOMM);

		if (m_socketClient == INVALID_SOCKET) 
		{
			LOG(INFO)  + "RhoBluetoothManager::makeConnection() return with invalid socket opened";
			return WSAGetLastError();
		}

		SOCKADDR_BTH sa;

		memset (&sa, 0, sizeof(sa));
		sa.addressFamily = AF_BT;			
		//Search for the selected device in the list box in the link list
		//m_pCurrentDevice=m_pStart;
		sa.serviceClassId=ServerGuid;

		//if(iSelectedDeviceIndex==-1)
		//{
		//	sa.btAddr=m_saClient.btAddr;
		//}
		//else
		//{
			//for (int iCount = 0 ;(m_pCurrentDevice)&&iCount!=iSelectedDeviceIndex;m_pCurrentDevice=m_pCurrentDevice->NextDevice,iCount++);
			//sa.btAddr = m_pCurrentDevice->bthAddress;
		//}
		sa.btAddr = bt_addr;

		if (connect (m_socketClient, (SOCKADDR *)&sa, sizeof(sa)) == SOCKET_ERROR) 
		{
			LOG(INFO)  + "RhoBluetoothManager::makeConnection() return with error during socket connection";
			m_socketClient=INVALID_SOCKET;
			return WSAGetLastError();
		}
		// start read thred
		m_hReadThread= CreateThread(NULL, 0, runThreadReadData, (LPVOID)this, 0, NULL);
	}
	LOG(INFO)  + "RhoBluetoothManager::makeConnection() FINISH";
	return 0;
}


void RhoBluetoothManager::onDiscoverDlgSelectDevice(int index) {
	LOG(INFO)  + "RhoBluetoothManager::onDiscoverDlgSelectDevice() START";
	if (index < 0) {
		fireCreateSessionCallBack(RHO_BT_CANCEL, "");
		LOG(INFO)  + "RhoBluetoothManager::onDiscoverDlgSelectDevice() invalid index";
		return;
	}

	// make client connection
	m_pCurrentDevice=m_pStart;
	if (m_pStart == NULL) {
		LOG(INFO)  + "RhoBluetoothManager::onDiscoverDlgSelectDevice() empty device list";
		fireCreateSessionCallBack(RHO_BT_ERROR, "");
		return;
	}
	for (int iCount = 0 ;(m_pCurrentDevice)&&iCount!=index;m_pCurrentDevice=m_pCurrentDevice->NextDevice,iCount++);
	BT_ADDR bt_addr = m_pCurrentDevice->bthAddress;
	
	if (makeConnection(bt_addr) == 0) {
		// get connected_device_name
		RhoDeviceInfo devInfo;
		GetDeviceInfo(&devInfo, index);
		WideCharToMultiByte(CP_UTF8, 0, devInfo.szDeviceNameAddr, -1, mConnectedDeviceName, MAX_NAME_SIZE, 0, 0);
		// fire callback
		LOG(INFO)  + "RhoBluetoothManager::onDiscoverDlgSelectDevice() FINISH OK";
		fireCreateSessionCallBack(RHO_BT_OK, mConnectedDeviceName);
		return;
	}
	LOG(INFO)  + "RhoBluetoothManager::onDiscoverDlgSelectDevice() cannot make connection !";

	// fire callback
	fireCreateSessionCallBack(RHO_BT_ERROR, "");
	
}

void RhoBluetoothManager::onDiscoverDlgCancel() {
	fireCreateSessionCallBack(RHO_BT_CANCEL, "");
}

void RhoBluetoothManager::onDiscoveredDlgCancel() {
	terminateDiscoveredThread();
	fireCreateSessionCallBack(RHO_BT_CANCEL, "");
}



//Function: GetDeviceInfo
//Purpose:	Returns name and address of all the devices in the link list in DeviceInfo. This is used by the UI to display the names and addresses of the devices found
//Output:	DeviceInfo: name and address
//Return: Success returns zero.
int RhoBluetoothManager::GetDeviceInfo(RhoDeviceInfo *pPeerDevicesInfo)
{
	int iCtr=0;
	for (m_pCurrentDevice = m_pStart;(m_pCurrentDevice);m_pCurrentDevice=m_pCurrentDevice->NextDevice,iCtr++) 
	{ 
		StringCchPrintf(pPeerDevicesInfo[iCtr].szDeviceNameAddr, ARRAYSIZE(pPeerDevicesInfo[iCtr].szDeviceNameAddr),  L"%s:(%04x%08x)", m_pCurrentDevice->bthName, GET_NAP(m_pCurrentDevice->bthAddress), GET_SAP(m_pCurrentDevice->bthAddress));		
	} 
	return 0;
}

//Function: GetDeviceInfo
//Purpose:	Searches the link list for the specified device and returns address and name in DeviceInfo
//Input:		The current device index selected in the UI
//Output:	DeviceInfo: name and address
//Return: Returns -1 in case of an error, otherwise returns zero.
int RhoBluetoothManager::GetDeviceInfo(RhoDeviceInfo *pPeerDeviceInfo, int iSelectedItem)
{
	int iCtr=0;

	for (m_pCurrentDevice = m_pStart;(m_pCurrentDevice);m_pCurrentDevice=m_pCurrentDevice->NextDevice,iCtr++) 
	{ 
		if(iCtr==iSelectedItem)
		{
			StringCchPrintf(pPeerDeviceInfo[0].szDeviceNameAddr, ARRAYSIZE(pPeerDeviceInfo[0].szDeviceNameAddr), L"%s:(%04x%08x)", m_pCurrentDevice->bthName, GET_NAP(m_pCurrentDevice->bthAddress), GET_SAP(m_pCurrentDevice->bthAddress));		
			return 0;
		}
	} 
	return -1;
}



//Function: GetGUID
//Purpose:	Conversts a string containing the GUID into a GUID datatype.
//Input:		string cotaining the GUID
//Output:	GUID type
//Return: Returns -1 in case of an error, otherwise returns zero.
int RhoBluetoothManager::GetGUID(WCHAR *psz, GUID *pGUID) 
{
	int data1, data2, data3;
	int data4[8];

	if (11 ==  swscanf(psz, L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
		&data1, &data2, &data3,
		&data4[0], &data4[1], &data4[2], &data4[3], 
		&data4[4], &data4[5], &data4[6], &data4[7])) {
			pGUID->Data1 = data1;
			pGUID->Data2 = data2 & 0xffff;
			pGUID->Data3 = data3 & 0xffff;

			for (int i = 0 ; i < 8 ; ++i)
				pGUID->Data4[i] = data4[i] & 0xff;

			return 0;
	}
	return -1;
}

//Function: GetLocalDeviceName
//Purpose:	Returns the name of the owner set in the registry
//Output:	DeviceInfo: (only)name
//Return: Returns -1 in case of an error, otherwise returns zero.

int RhoBluetoothManager::GetLocalDeviceName(RhoDeviceInfo *pLocalDeviceInfo)

{
	HKEY hKey;
	DWORD dwRegType, dwRegSize;
	if(RegOpenKeyEx(HKEY_CURRENT_USER,L"ControlPanel\\Owner",0,0,&hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKey,L"Name",0,&dwRegType,(LPBYTE)pLocalDeviceInfo->szDeviceNameAddr,&dwRegSize)==ERROR_SUCCESS)
		{
			if (dwRegSize>MAX_NAME_SIZE) 
			{
				RegCloseKey(hKey);
				return -1; 
			}
			RegCloseKey(hKey);
		}
		RegCloseKey(hKey);
	}
	return 0;
}















RhoDiscoverDlg::RhoDiscoverDlg() {

}

RhoDiscoverDlg::~RhoDiscoverDlg() {

}
void RhoDiscoverDlg::openDialog(RhoBluetoothManager* btManager) {
	LOG(INFO)  + "RhoDiscoverDlg::openDialog()";
	mBtManager = btManager;
	hCurs = GetCursor();
	//mShowCursor = ShowCursor(false);
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	dlg.mSelectedDevice = -1;
	int retVal = dlg.DoModal(getMainWnd());
	//dlg.ShowWindow();
	SetCursor(hCurs);
	ShowCursor(false);
	if (retVal != IDOK) {
		LOG(INFO)  + "RhoDiscoverDlg::openDialog() dialog closed with not OK";
		mBtManager->onDiscoverDlgSelectDevice(-1);
	}
	LOG(INFO)  + "RhoDiscoverDlg::openDialog() dialog closed with OK";
	mBtManager->onDiscoverDlgSelectDevice(dlg.mSelectedDevice);
}

void RhoDiscoverDlg::updateDeviceListFromManager() {
	LOG(INFO)  + "RhoDiscoverDlg::updateDeviceListFromManager() START";
	//mBtManager->onDiscoverDlgSelectDevice(0);

	int count = mBtManager->GetNumDevices();
	
	if (count <= 0) {
		LOG(INFO)  + "RhoDiscoverDlg::updateDeviceListFromManager() Device list is Empty !!!";
		::MessageBox(getMainWnd(), L"Not found any Bluetooth devices !", L"Warning", MB_OK | MB_ICONINFORMATION);
		dlg.EndDialog(IDCANCEL);
		//mBtManager->onDiscoverDlgSelectDevice(-1);
		return;
	}

	int i;
	for (i = 0; i < count; ++i) {
		RhoDeviceInfo devInfo;
		mBtManager->GetDeviceInfo(&devInfo, i);		
		SendMessage(dlg.GetDlgItem(IDC_BT_LIST).m_hWnd, LB_ADDSTRING,0,(LPARAM) devInfo.szDeviceNameAddr);
		SendMessage(dlg.GetDlgItem(IDC_BT_LIST).m_hWnd, LB_SETITEMDATA, i, (LPARAM) i);
	}

	SendMessage(dlg.GetDlgItem(IDC_BT_LIST).m_hWnd, LB_SETCURSEL, 0,0);
	SetCursor(hCurs);
	ShowCursor(false);
	SetWindowText(dlg.GetDlgItem(IDC_BT_STATIC).m_hWnd, L"Please select Bluetooth device to connect :");
	LOG(INFO)  + "RhoDiscoverDlg::updateDeviceListFromManager() FINISH";
	//dlg.RedrawWindow();
	//mBtManager->onDiscoverDlgSelectDevice(0);
}


RhoDiscoveredDlg::RhoDiscoveredDlg() {

}

RhoDiscoveredDlg::~RhoDiscoveredDlg() {

}

void RhoDiscoveredDlg::openDialog(RhoBluetoothManager* btManager) {
	LOG(INFO)  + "RhoDiscoveredDlg::openDialog()";
	mBtManager = btManager;
	hCurs = GetCursor();
	//mShowCursor = ShowCursor(false);
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	int retVal = dlg.DoModal(getMainWnd());
	//dlg.ShowWindow();
	SetCursor(hCurs);
	ShowCursor(false);
	if (retVal != 555) {
		LOG(INFO)  + "RhoDiscoveredDlg::openDialog() dialog closed with not OK";
		mBtManager->onDiscoveredDlgCancel();
	}
	LOG(INFO)  + "RhoDiscoveredDlg::openDialog() dialog closed with OK";
}

void RhoDiscoveredDlg::closeDialog() {
	dlg.EndDialog(555);
}













































int rho_bluetooth_is_bluetooth_available() {
	return RhoBluetoothManager::getInstance()->rho_bluetooth_is_bluetooth_available();
}

void rho_bluetooth_off_bluetooth() {
	RhoBluetoothManager::getInstance()->rho_bluetooth_off_bluetooth();
}

void rho_bluetooth_set_device_name(const char* device_name) {
	RhoBluetoothManager::getInstance()->rho_bluetooth_set_device_name(device_name);
}

const char* rho_bluetooth_get_device_name() {
	return RhoBluetoothManager::getInstance()->rho_bluetooth_get_device_name();
}

const char* rho_bluetooth_get_last_error() {
	return RhoBluetoothManager::getInstance()->rho_bluetooth_get_last_error();
}

const char* rho_bluetooth_create_session(const char* role, const char* callback_url) {
	return RhoBluetoothManager::getInstance()->rho_bluetooth_create_session(role, callback_url);
}

void rho_bluetooth_session_set_callback(const char* connected_device_name, const char* callback_url) {
	RhoBluetoothManager::getInstance()->rho_bluetooth_session_set_callback(connected_device_name, callback_url);
}

void rho_bluetooth_session_disconnect(const char* connected_device_name) {
	RhoBluetoothManager::getInstance()->rho_bluetooth_session_disconnect(connected_device_name);
}

int rho_bluetooth_session_get_status(const char* connected_device_name) {
	return RhoBluetoothManager::getInstance()->rho_bluetooth_session_get_status(connected_device_name); 
}

const char* rho_bluetooth_session_read_string(const char* connected_device_name) {
	return RhoBluetoothManager::getInstance()->rho_bluetooth_session_read_string(connected_device_name);
}

void rho_bluetooth_session_write_string(const char* connected_device_name, const char* str) {
	int length = strlen(str);
	if (length <= 0) {
		return;
	}
	RhoBluetoothManager::getInstance()->rho_bluetooth_session_write_data(connected_device_name, (void*)str, length);
}

VALUE rho_bluetooth_session_read_data(const char* connected_device_name) {
	int size = 0;
	void* buf = RhoBluetoothManager::getInstance()->readAllBlocks(&size, 0);
	VALUE val = rho_ruby_create_byte_array((unsigned char*)buf, size);
	free(buf);
	return val;
}

void rho_bluetooth_session_write_data(const char* connected_device_name, VALUE data) {
	int size = rho_ruby_unpack_byte_array(data, 0, 0);
	if (size <= 0) {
		return;
	}
	unsigned char* buf = (unsigned char*)malloc(size);
	size = rho_ruby_unpack_byte_array(data, buf, size);
	RhoBluetoothManager::getInstance()->rho_bluetooth_session_write_data(connected_device_name, buf, size);
	free(buf);
}



// CRhoBluetoothDiscoverDlg dialog

CRhoBluetoothDiscoverDlg::CRhoBluetoothDiscoverDlg ()
{
}

CRhoBluetoothDiscoverDlg::~CRhoBluetoothDiscoverDlg ()
{
}

LRESULT CRhoBluetoothDiscoverDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowText(_T("Discover Bluetooth devices"));

#if defined(_WIN32_WCE)

	SHINITDLGINFO shidi = { SHIDIM_FLAGS, m_hWnd, SHIDIF_SIZEDLGFULLSCREEN };
	RHO_ASSERT(SHInitDialog(&shidi));


	SHMENUBARINFO mbi = { sizeof(mbi), 0 };
	mbi.hwndParent = m_hWnd;
	mbi.nToolBarId = IDR_GETURL_MENUBAR;
	mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
	RHO_ASSERT(SHCreateMenuBar(&mbi));
	//GotoDlgCtrl(GetDlgItem(IDC_DATE_CTRL));

#else 

	CreateButtons();
	GotoDlgCtrl(m_btnOk);

#endif

	//DLG_ITEM_SET_FONT_BOLD (IDC_DATE_STATIC);
	//DLG_ITEM_SET_FONT_BOLD (IDC_TIME_STATIC);

	//if (m_format == CDateTimeMessage::FORMAT_DATE) {
	//	GetDlgItem(IDC_TIME_CTRL).ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_TIME_STATIC).ShowWindow(SW_HIDE);
	//}
	mSelectedDevice = -1;
	return FALSE;
}

LRESULT CRhoBluetoothDiscoverDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND hwnd, BOOL& /*bHandled*/)
{
	// set selected device

	int sel_item = SendMessage(GetDlgItem(IDC_BT_LIST).m_hWnd, LB_GETCURSEL, 0, 0);
	mSelectedDevice = SendMessage(GetDlgItem(IDC_BT_LIST).m_hWnd, LB_GETITEMDATA, (WPARAM)sel_item, 0);
	EndDialog(wID);
	return 0;
}


LRESULT CRhoBluetoothDiscoverDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}






// CRhoBluetoothDiscoveredDlg dialog

CRhoBluetoothDiscoveredDlg::CRhoBluetoothDiscoveredDlg ()
{
}

CRhoBluetoothDiscoveredDlg::~CRhoBluetoothDiscoveredDlg ()
{
}

LRESULT CRhoBluetoothDiscoveredDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowText(_T("Discoverable"));

#if defined(_WIN32_WCE)

	SHINITDLGINFO shidi = { SHIDIM_FLAGS, m_hWnd, SHIDIF_SIZEDLGFULLSCREEN };
	RHO_ASSERT(SHInitDialog(&shidi));


	SHMENUBARINFO mbi = { sizeof(mbi), 0 };
	mbi.hwndParent = m_hWnd;
	mbi.nToolBarId = IDR_GETURL_MENUBAR;
	mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
	RHO_ASSERT(SHCreateMenuBar(&mbi));
	//GotoDlgCtrl(GetDlgItem(IDC_DATE_CTRL));

#else 

	CreateButtons();
	GotoDlgCtrl(m_btnOk);

#endif

	//DLG_ITEM_SET_FONT_BOLD (IDC_DATE_STATIC);
	//DLG_ITEM_SET_FONT_BOLD (IDC_TIME_STATIC);

	//if (m_format == CDateTimeMessage::FORMAT_DATE) {
	//	GetDlgItem(IDC_TIME_CTRL).ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_TIME_STATIC).ShowWindow(SW_HIDE);
	//}
	return FALSE;
}

LRESULT CRhoBluetoothDiscoveredDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND hwnd, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}


LRESULT CRhoBluetoothDiscoveredDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}










#else // OS_WINDOWS

int rho_bluetooth_is_bluetooth_available() {
	return 0;
}

void rho_bluetooth_off_bluetooth() {
}

void rho_bluetooth_set_device_name(const char* device_name) {
}

const char* rho_bluetooth_get_device_name() {
	return "ERROR";
}

const char* rho_bluetooth_get_last_error() {
	return "ERROR";
}

const char* rho_bluetooth_create_session(const char* role, const char* callback_url) {
	return "ERROR";
}

void rho_bluetooth_session_set_callback(const char* connected_device_name, const char* callback_url) {
}

void rho_bluetooth_session_disconnect(const char* connected_device_name) {
}

int rho_bluetooth_session_get_status(const char* connected_device_name) {
	return -1; 
}

const char* rho_bluetooth_session_read_string(const char* connected_device_name) {
	return "ERROR";
}

void rho_bluetooth_session_write_string(const char* connected_device_name, const char* str) {
}

VALUE rho_bluetooth_session_read_data(const char* connected_device_name) {
	return 0;
}

void rho_bluetooth_session_write_data(const char* connected_device_name, VALUE data) {
}


#endif // OS_WINDOWS

