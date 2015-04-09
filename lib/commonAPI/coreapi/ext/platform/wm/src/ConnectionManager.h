/**
 *  \file WAN.h
 *  \brief	This class is responsible for interfacing with the connection manager to establish
 *			and break connections as well as providing feedback to the user on the connection
 *			availability.  Note the most reliable way of obtaining connected / disconnected
 *			is to use the NetworkEvent.
 * \author	Darryn Campbell (DCC, JRQ768)
 * \date	November 2011 (Initial Creation)
 */

//#include "../../common/Public/PBPlugin.h"
#pragma once
#include "stubs/snapi.h"
#include "stubs/regext.h"
#define INITGUID
///  Globally unique identifier for the Internet
static const GUID IID_DestNetInternet = {0x436ef144, 0xb4fb, 0x4863, 0xa0, 0x41, 0x8f, 0x90, 0x5a, 0x62, 0xc5, 0x72};
///  Time (in milliseconds) between each watchdog check that we haven't lost the connection from Connection Manager
const int CONNECTION_MANAGER_WATCHDOG_TIMER_TIMEOUT = 15000;
#include "stubs/ConnMgr.h"
#define WM_USER_CONNECTION_MANGER_STATUS	WM_USER + 30
#include "logging/RhoLog.h"
#include "api_generator/MethodResult.h"

typedef HRESULT (WINAPI* LPFN_CONMGR_ESTABLISHCONNECTION_T)	(CONNMGR_CONNECTIONINFO*,HANDLE*);
typedef HRESULT (WINAPI* LPFN_CONMGR_ENUMDESTINATIONS_T) (int, CONNMGR_DESTINATION_INFO*);
typedef HRESULT (WINAPI* LPFN_CONMGR_RELEASECONNECTION_T) (HANDLE, LONG);
typedef HRESULT (WINAPI* LPFN_CONMGR_CONNECTIONSTATUS_T) (HANDLE, DWORD*);

typedef HRESULT (WINAPI* LPFN_REGISTRY_CLOSENOTIFICATION_T) (HREGNOTIFY);
typedef HRESULT (WINAPI* LPFN_REGISTRY_NOTIFYCALLBACK_T) (HKEY, LPCTSTR, LPCTSTR, REGISTRYNOTIFYCALLBACK, DWORD, NOTIFICATIONCONDITION*, HREGNOTIFY*);
typedef HRESULT (WINAPI* LPFN_REGISTRY_GETSTRING_T) (HKEY, LPCTSTR, LPCTSTR, LPTSTR, UINT);
typedef HRESULT (WINAPI* LPFN_REGISTRY_GETDWORD_T) (HKEY, LPCTSTR, LPCTSTR, DWORD*);

//  Cellular system available and connected
# ifndef SN_CELLSYSTEMAVAILABLE_ROOT
#define SN_CELLSYSTEMAVAILABLE_ROOT	HKEY_LOCAL_MACHINE						///< Cell system available registry key
#define SN_CELLSYSTEMAVAILABLE_PATH	TEXT("System\\State\\Phone")			///< Cell system available registry path
#define SN_CELLSYSTEMAVAILABLE_VALUE	TEXT("Cellular System Available")	///< Cell system available registry value
#define SN_CELLSYSTEMCONNECTED_ROOT	HKEY_LOCAL_MACHINE						///< Cell system connected registry key
#define SN_CELLSYSTEMCONNECTED_PATH	TEXT("System\\State\\Phone")			///< Cell system connected registry path
#define SN_CELLSYSTEMCONNECTED_VALUE	TEXT("Cellular System Connected")	///< Cell system connected registry value
#define SN_CELLSYSTEMCONNECTED_GPRS_BITMASK		1							///< Cell system registry bitmask
#define SN_CELLSYSTEMCONNECTED_1XRTT_BITMASK	2							///< Cell system registry bitmask
#define SN_CELLSYSTEMCONNECTED_1XEVDO_BITMASK	4							///< Cell system registry bitmask
#define SN_CELLSYSTEMCONNECTED_EDGE_BITMASK		8							///< Cell system registry bitmask
#define SN_CELLSYSTEMCONNECTED_UMTS_BITMASK		16							///< Cell system registry bitmask
#define SN_CELLSYSTEMCONNECTED_EVDV_BITMASK		32							///< Cell system registry bitmask
#define SN_CELLSYSTEMCONNECTED_HSDPA_BITMASK	64							///< Cell system registry bitmask
#define SN_CELLSYSTEMCONNECTED_CSD_BITMASK		2147483648					///< Cell system registry bitmask
#endif

///  Names of returned values
static TCHAR* szWANEventNames[] = 
{
	L"phoneSignalStrength",		///<  The Phone Signal level (0 - 100)
	L"networkOperator",			///<  The current network operator
	L"connectionTypeAvailable",	///<  The connection type available, e.g. GPRS, HSPDA
	L"connectionTypeConnected",	///<  The connection type to which the phone is connected, e.g. GPRS, HSPDA
	L"connectionManagerMessage",///<  The last message received from the Connection Manager
	NULL
};

class CWAN;

/**
*  Callback data associated with the registry notifications, when data is received via a 
*  RegistryNotifyCallback we need both a pointer to the CWAN object and the type of 
*  callback being recieved.
*/
struct RegistryCallbackData
{
	CWAN* pObject;			///<  Pointer to CWAN which asked for the Data
	DWORD dwCallbackType;	///<  Type of registry key associated with the callback.
};


/**
*  Class to wrap the WAN functionality, provides interfaces with both the Connection
*  Manager and Windows Registry Notifications (to provide callback on cell status, 
*  phone level e.t.c.).
* \author	Darryn Campbell (DCC, JRQ768)
* \date	November 2011 (Initial Creation)
*/
class CWAN
{
public:
	/**
	*  Constructor for CWAN, initialises member variables.
	*  \param iInstanceID Unique RhoElements application Identifier
	*  \param pModule Pointer to associated PBModule which represents the PB
	*  instance to which this object is related.
	*/
	CWAN(HWND hParendWnd);
	
	/**
	*  Performs shutdown behaviour
	*/ 
	~CWAN();

	/**
	*  Perform initialisation steps, memory allocation e.t.c.  Also outputs
	*  the list of available connections to the log.
	*  \return Whether or not the WAN class successfully initialised.
	*/
	BOOL Initialise();

	/**
	*  Connect to a specified destination.
	*  \param szConnectionDestination Destination to connect to, must have been
	*  previously configured using the Connection Manager.  When CWAN is initialised
	*  a list of available connections is written to the log.
	*  \param bUserRequest Set this to true if this function is being called as the
	*  request of a user, doing so will disconnect any connection in progress
	*  \return Whether or not the connection was successful.
	*/
	BOOL Connect(LPCTSTR szConnectionDestination, BOOL bUserRequest);

	/**
	*  Disconnect from the current destination.
	*  \param bUserRequest Set this to true if this function is being called as the
	*  request of a user, doing so will stop the watchdog timer
	*  \return Whether or not the disconnection was successful
	*/
	BOOL Disconnect(BOOL bUserRequest);

	/**
	*  Called whenever there is a new Connection Manager status available, retrieves
	*  the textual representation of the connection manager status and notifies the
	*  user via the WAN Status Event
	*/
	void ConnectionManagerStatusUpdate();

	/**
	*  Specify the Navigation string.
	*  \param szNavURI The Navigation String to be sent when invoking SendPBNavigate
	*  \return Whether or not the Navigation URI was successfully set.
	*/
	BOOL SetWanCallback(rho::apiGenerator::CMethodResult pCallback);

	/**
	*  Accessor for m_hConnManDLL
	*  \return m_hConnManDLL
	*/
	HMODULE GetConnManDLL() {return m_hConnManDLL;}

private:  
	/**
	*  Load the Connection Manager DLL into memory and get the addresses for the functions we require
	*  \return TRUE if Connection Manager was successfully loaded
	*/
	BOOL LoadConnectionManager();

	/**
	*  Load the AYGShell DLL into memory and get the addresses for the functions we require.  Failing to 
	*  load AYGShell will mean we are not able to retrieve notifications when registry values change.
	*  \return TRUE if AYGShell was successfully loaded.
	*/
	BOOL LoadAYGShell();

	/**
	*  Sends the WAN Status Event to the user, this function is invoked whenever any of the parameters in 
	*  the WAN Status event change, e.g. the phone signal changes or the device's connected data network 
	*  changes.
	*/
	void ReportWANStatus();

	/**
	*  Register for callbacks for when the registry values change representing our phone signal, connected
	*  data network e.t.c.  Windows system will update these registry values when the parameters change and
	*  this is the most reliable way to get notified of the events.  AYGShell needs to be present on the device
	*  for this function to work.
	*  \return TRUE if the registration was successful.
	*/
	BOOL RegisterForNotifications();

	/**
	*  The callback that will get invoked whenever any of the registry values we're interested in changes
	*  (as specified in RegisterForNotifications).
	*  \param hNotify Not Used
	*  \param dwUserData RegistryCallbackData structure associated with the callback (CWAN object and type of callback)
	*  \param pData The new data value
	*  \param cbData Not Used
	*/
	static void PhoneStatusCallback(HREGNOTIFY hNotify, DWORD dwUserData, const PBYTE pData, const UINT cbData);

	/**
	*  Retrieve the current values of all the registry keys we are interested in, including whether the phone
	*  has a SIM card and whether the phone is turned on.  We will not attempt to connect if the prerequisites
	*  are not met (e.g. the radio is not on)
	*  \return TRUE if all the registry keys are present, regardless of their values.
	*/
	BOOL RetrievePrerequisitesFromRegistry();

	/**
	*  Retrieve the current registry values representing the Cell Available status and Cell Connected status,
	*  e.g. GPRS, HSPDA e.t.c.
	*  \param szCellSystemAvailable [out] populated with the cell available status.  Must be preallocated by caller.
	*  \param szCellSystemConnected [out] populated wtih the cell connected status.  Must be preallocated by caller.
	*  \return TRUE if the values were successfully retrieved.
	*/
	BOOL GetCellularStatus(TCHAR* szCellSystemAvailable, TCHAR* szCellSystemConnected);

	/**
	*  Retrieve the current connection status from the Connection Manager
	*  \param szStatusBuffer [out] Buffer into which the connection manager status will be copied.  Must be preallocated by caller.
	*  \return TRUE if the connection status was successfully obtained
	*/
	BOOL GetConnectionStatus( TCHAR *szStatusBuffer );

	/**
	*  Updates the internal state machine of the connection manager's state given the new Connection
	*  manager status.
	*  \param dwStatus The new Connection manager status
	*/
	void UpdateConnectionManagerState(DWORD dwStatus);

	/**
	*  Helper function to convert the Connection Manager integer status into a string
	*  \param iStatus The status to converted
	*  \return user readable string representation of iStatus
	*/
	const TCHAR *GetConnectionStatusString( int iStatus );

private:	//  Threads
	/**
	*  Thread which runs monitoring the Connection Manager status, every X milliseconds it checks the connection
	*  manager is connected or pending connection, if it is not connected it will attempt to re-establish the 
	*  connection.  This thread only runs between calls of connect() and disconnect()
	*  \param lpParameter Pointer to the CWAN object associated with this thread.
	*  \return 0 when the thread exits
	*/
	static DWORD ConnectionManagerWatchdog(LPVOID lpParameter);

private:
	int				m_iInstanceID;			///<  RhoElements Application identifier (used for logging and SendPBNavigate)
	HWND m_hParentWnd;						///<  Window that will receive Connection Manager Status Messages
	HANDLE m_hConnection;					///<  The connection opened through Connection Manager (NULL for no connection)
	DWORD m_dwSignal;						///<  The current signal level (0 - 100)
	DWORD m_dwCellSystemAvailable;			///<  The current type of cellular system available
	DWORD m_dwCellSystemConnected;			///<  The current type of cellular system connected to
	bool m_bPhoneHasSim;					///<  Whether or not the phone has a Sim card installed
	bool m_bCDMADevice;						///<  Whether the device is CDMA device rather than GSM
	bool m_bRadioPresent;					///<  Whether or not the phone radio is present on the device
	bool m_bRadioOn;						///<  Whether or not the phone radio is turned on
	bool m_bPhoneService;					///<  Whether or not there is any phone service on the network
	WCHAR	m_szOperatorName[64];			///<  The current operator name, e.g. Orange
	WCHAR m_szCellularSystemAvailable[16];	///<  String representation of the current type of cellular system available, e.g. GPRS, HSPDA
	WCHAR m_szCellularSystemConnected[16];	///<  String representation of the current type of cellular system connected to, e.g. GPRS
	WCHAR m_szConnectionManagerStatus[128];	///<  User readable String representation of Connection Manager status
	WCHAR* m_szLastRequestedConnectionDestionation;	///<  The last connection destination requested by the user
	HMODULE m_hAygShellDLL;					///<  HMODULE for the dynamically loaded AYGShell DLL
	HMODULE m_hConnManDLL;					///<  HMODULE for the dynamically loaded Connection Manager DLL
	HANDLE m_hStopConnectionManagerWatchdog;///<  Handle to event signaled to indicate the connection manager watchdog should stop
	HANDLE m_hConnectionManagerWatchdog;	///<  Handle to thread which monitors the Connection Manager connection
	rho::apiGenerator::CMethodResult m_pStatusCallback;	///<  Callback when WAN status changes

	LPFN_CONMGR_ESTABLISHCONNECTION_T	lpfn_ConMgr_EstablishConnection;	///< Fn Ptr for Connection Manager Establish Connection
	LPFN_CONMGR_ENUMDESTINATIONS_T		lpfn_ConMgr_EnumDestinations;		///< Fn Ptr for Connection Manager Enum Destinations
	LPFN_CONMGR_RELEASECONNECTION_T		lpfn_ConMgr_ReleaseConnection;		///< Fn Ptr for Connection Manager Release Connection
	LPFN_CONMGR_CONNECTIONSTATUS_T		lpfn_ConMgr_ConnectionStatus;		///< Fn Ptr for Connection Manager Connection Status

	LPFN_REGISTRY_CLOSENOTIFICATION_T	lpfn_Registry_CloseNotification;	///< Fn Ptr for AYGShell DLL Close Notification
	LPFN_REGISTRY_NOTIFYCALLBACK_T		lpfn_Registry_NotifyCallback;		///< Fn Ptr for AYGShell DLL Notify Callback
	LPFN_REGISTRY_GETSTRING_T			lpfn_Registry_GetString;			///< Fn Ptr for AYGShell DLL Get String
	LPFN_REGISTRY_GETDWORD_T			lpfn_Registry_GetDWORD;				///< Fn Ptr for AYGShell DLL Get DWORD

	/**
	*  Enumeration of the different registry values we receive notifications on
	*/
	enum PhoneNotifications
	{
		RE_WAN_SIGNAL_STRENGTH = 0,		///<  Phone Signal Strength
		RE_WAN_PHONE_SERVICE,			///<  Whether or not there is phone service
		RE_WAN_CELL_SYSTEM_AVAILABLE,	///<  The type of Cellular system available, e.g. GPRS, HSPDA
		RE_WAN_CELL_SYSTEM_CONNECTED,	///<  The type of Cellular system connected, e.g. GPRS, HSPDA
		RE_WAN_MAX_NOTIFICATIONS		///<  Used for Array processing, the number of phone notifications
	};

	HREGNOTIFY hRegistryNotifications[RE_WAN_MAX_NOTIFICATIONS];	///<  Array of notification handles (used for de-registering on shutdown)
	RegistryCallbackData pCallbackData[RE_WAN_MAX_NOTIFICATIONS];	///<  Array of callback data associated with each of the registry notifications

	enum ConnectionManagerState
	{
		RE_NOT_CONNECTED = 0,
		RE_CONNECTION_UP,
		RE_CONNECTION_DOWN_AND_REQUIRES_RESTART
	} m_currentConnectionManagerState;
};