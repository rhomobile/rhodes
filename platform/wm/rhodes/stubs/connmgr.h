//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//

#ifndef _connmgr_h_
#define _connmgr_h_

#ifdef __cplusplus
extern "C" {
#endif

// {436EF144-B4FB-4863-A041-8F905A62C572}
DEFINE_GUID(IID_DestNetInternet,     0x436ef144, 0xb4fb, 0x4863, 0xa0, 0x41, 0x8f, 0x90, 0x5a, 0x62, 0xc5, 0x72);

// {A1182988-0D73-439e-87AD-2A5B369F808B}
DEFINE_GUID(IID_DestNetCorp,         0xa1182988, 0x0d73, 0x439e, 0x87, 0xad, 0x2a, 0x5b, 0x36, 0x9f, 0x80, 0x8b);

// {7022E968-5A97-4051-BC1C-C578E2FBA5D9}
DEFINE_GUID(IID_DestNetWAP,          0x7022e968, 0x5a97, 0x4051, 0xbc, 0x1c, 0xc5, 0x78, 0xe2, 0xfb, 0xa5, 0xd9);

// {F28D1F74-72BE-4394-A4A7-4E296219390C}
DEFINE_GUID(IID_DestNetSecureWAP,    0xf28d1f74, 0x72be, 0x4394, 0xa4, 0xa7, 0x4e, 0x29, 0x62, 0x19, 0x39, 0x0c);

//
// Connection Manager
//

//
// Client API
//

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Valid Params | Defines valid parameters in CONNMGR_CONNECTIONINFO
//
// @comm The following flags in the dwParams field of the CONNMGR_CONNECTIONINFO structure
// define which optional parameters are valid.
//
// -----------------------------------------------------------------------------
#define CONNMGR_PARAM_GUIDDESTNET       (0x1) // @constdefine guidDestNet field is valid
#define CONNMGR_PARAM_MAXCOST           (0x2) // @constdefine MaxCost field is valid
#define CONNMGR_PARAM_MINRCVBW          (0x4) // @constdefine MinRcvBw field is valid
#define CONNMGR_PARAM_MAXCONNLATENCY    (0x8) // @constdefine MaxConnLatency field is valid

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Proxy flags | Defines which proxies the application supports
//
// @comm The following flags in the dwFlags field of the CONNMGR_CONNECTIONINFO structure
// define special connection properties supported by client apps, typically which proxies are
// supported by each app. For example, each CONNMGR_FLAG_PROXY_* specifies that this connection
// can be specified by configuring an appropriate proxy server.  e.g. HTTP this is appropriate
// for Internet Explorer and other HTTP based protocols, including HTTP-DAV synchronization.
// If none of these flags are specified, then only a direct IP connection (or tunnel) will be attempted.
//
// -----------------------------------------------------------------------------
#define CONNMGR_FLAG_PROXY_HTTP		(0x1) // @constdefine HTTP Proxy supported
#define CONNMGR_FLAG_PROXY_WAP      (0x2) // @constdefine WAP Proxy (gateway) supported
#define CONNMGR_FLAG_PROXY_SOCKS4   (0x4) // @constdefine SOCKS4 Proxy supported
#define CONNMGR_FLAG_PROXY_SOCKS5   (0x8) // @constdefine SOCKS5 Proxy supported

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Control flags | Defines connection properties
//
// @comm The following flags in the dwFlags field of the CONNMGR_CONNECTIONINFO structure
// define special connection properties supported by client apps,
//
// -----------------------------------------------------------------------------
#define CONNMGR_FLAG_SUSPEND_AWARE    (0x10) // @constdefine suspended connections supported
#define CONNMGR_FLAG_REGISTERED_HOME  (0x20) // @constdefine only dial out if we're registered on the home network
#define CONNMGR_FLAG_NO_ERROR_MSGS    (0x40) // @constdefine don't show any error messages for failed connections
#define CONNMGR_FLAG_WAKE_ON_INCOMING (0x80) // @constdefine to satisfy request use only those interfaces that can wake the system on incoming traffic

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Priority | Defines which priority of the applications connections
//
// @comm The following flags in the dwPriority field of the CONNMGR_CONNECTIONINFO structure
// define the priority of the connection. Connection manager arbitrates among multiple connections,
// satisfying as many as possible at a time but giving preference to higher priorities
//
// -----------------------------------------------------------------------------

#define CONNMGR_PRIORITY_VOICE          		0x20000
// @constdefine Voice, highest priority, reserved for internal use only.

#define CONNMGR_PRIORITY_USERINTERACTIVE		0x08000
// @constdefine User initiated action caused this request, and UI is        	
// currently pending on the creation of this connection.
// This is appropriate for an interactive browsing session,
// or if the user selects "MORE" at the bottom of a truncated
// mail message, etc.


#define CONNMGR_PRIORITY_USERBACKGROUND		    0x02000
// @constdefine User initiated connection which has recently become idle.
// A connection should be marked as idle when it is no
// longer the user's current task.		

#define CONNMGR_PRIORITY_USERIDLE				0x0800		
// @constdefine Interactive user task which has been idle for an application
// specified time.  The application should toggle the state
// between CONNMGR_PRIORITY_USERIDLE and CONNMGR_PRIORITY_USERINTERACTIVE as the user
// uses the application.  This helps ConnectionManager
// optimize responsiveness to the interactive application,
// while sharing the connection with background applications.

#define CONNMGR_PRIORITY_HIPRIBKGND			    0x0200
// @constdefine High priority background connection

#define CONNMGR_PRIORITY_IDLEBKGND			    0x0080
// @constdefine Idle priority background connection

#define CONNMGR_PRIORITY_EXTERNALINTERACTIVE	0x0020
// @constdefine Connection is requested on behalf of an external entity, but
// is an interactive session (e.g. AT Command Iterpreter)

#define CONNMGR_PRIORITY_LOWBKGND               0x0008
// @constdefine Lowest priority. Only connects if another higher priority client is already using the same path.

#define CONNMGR_PRIORITY_CACHED                 0x0002
// @constdefine Cached connection, reserved for internal use only.

#define CONNMGR_PRIORITY_ALWAYS_ON              0x0001
// @constdefine Always on  connection, reserved for internal use only.

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct CONNMGR_CONNECTIONINFO | Information about connection request
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct _CONNMGR_CONNECTIONINFO
{
    DWORD cbSize;       // @field Size of this structure
    DWORD dwParams;     // @field Valid parms, set of CONNMGR_PARAM_*
    DWORD dwFlags;      // @field Connection flags, set of CONNMGR_FLAG_*
    DWORD dwPriority;   // @field Priority, one of CONNMGR_PRIORITY_*
    BOOL bExclusive;    // @field Connection is exclusive, see comments
    BOOL bDisabled;     // @field Don't actually connect
    GUID guidDestNet;   // @field GUID of network to connect to
    HWND hWnd;          // @field hWnd to post status change messages to
    UINT uMsg;          // @field Msg to use when posting status changes
    LPARAM lParam;      // @field lParam to use when posting status changes
    ULONG ulMaxCost;    // @field Max acceptable cost of connection
    ULONG ulMinRcvBw;   // @field Min acceptable receive bandwidth of connection
    ULONG ulMaxConnLatency; // @field Max acceptable connect latency
} CONNMGR_CONNECTIONINFO;

// @comm bExclusive: If false, the connection is shared among all applications, and other
// applications with an interest in a connection to this network will be notified that
// the connection is available.  If true, then this connection can not be shared with other
// applications, and no other applications will be notified, and any application requesting
// a connection to the same network will be treated as a contender for
// the same resource, and not permitted to share the existing connection.  A decision will be made
// between this connection and the others based on connection priority.

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Creates a connection request.
//
// @comm Return Value:  S_OK if success, error code otherwise
//
// -----------------------------------------------------------------------------
HRESULT WINAPI ConnMgrEstablishConnection
    (
    CONNMGR_CONNECTIONINFO *pConnInfo,  // @parm Params describing requested connection
    HANDLE *phConnection                // @parm Returned connection handle
    );

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Creates a connection request.
//
// @comm Return Value:  Same as ConnMgrEstablishConnection, but doesn't return
// until connection has either been established or failed.
//
// -----------------------------------------------------------------------------
HRESULT WINAPI ConnMgrEstablishConnectionSync
    (
    CONNMGR_CONNECTIONINFO *pConnInfo,  // @parm Params describing requested connection
    HANDLE *phConnection,               // @parm Returned connection handle
    DWORD dwTimeout,                    // @parm Timeout
    DWORD *pdwStatus                    // @parm Final status value, one of CONNMGR_STATUS_*
    );

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Status values | Describes the current status of the connection
//
// @comm none
//
// -----------------------------------------------------------------------------

#define CONNMGR_STATUS_UNKNOWN                    0x00  // @constdefine Unknown status

#define CONNMGR_STATUS_CONNECTED                  0x10  // @constdefine Connection is up
#define CONNMGR_STATUS_SUSPENDED                  0x11  // @constdefine Connection is up but suspended

#define CONNMGR_STATUS_DISCONNECTED               0x20  // @constdefine Connection is disconnected
#define CONNMGR_STATUS_CONNECTIONFAILED           0x21  // @constdefine Connection failed and cannot not be reestablished
#define CONNMGR_STATUS_CONNECTIONCANCELED         0x22  // @constdefine User aborted connection
#define CONNMGR_STATUS_CONNECTIONDISABLED         0x23  // @constdefine Connection is ready to connect but disabled
#define CONNMGR_STATUS_NOPATHTODESTINATION        0x24  // @constdefine No path could be found to destination
#define CONNMGR_STATUS_WAITINGFORPATH             0x25  // @constdefine Waiting for a path to the destination
#define CONNMGR_STATUS_WAITINGFORPHONE            0x26  // @constdefine Voice call is in progress
#define CONNMGR_STATUS_PHONEOFF                   0x27  // @constdefine Phone resource needed and phone is off
#define CONNMGR_STATUS_EXCLUSIVECONFLICT          0x28  // @constdefine the connection could not be established because it would multi-home an exclusive connection
#define CONNMGR_STATUS_NORESOURCES                0x29  // @constdefine Failed to allocate resources to make the connection.
#define CONNMGR_STATUS_CONNECTIONLINKFAILED       0x2A  // @constdefine Connection link disconnected prematurely.
#define CONNMGR_STATUS_AUTHENTICATIONFAILED       0x2B  // @constdefine Failed to authenticate user.
#define CONNMGR_STATUS_NOPATHWITHPROPERTY         0x2C  // @constdefine Path with connection having requested property, ex. WAKE_ON_INCOMING, is not available.

#define CONNMGR_STATUS_WAITINGCONNECTION          0x40  // @constdefine Attempting to connect
#define CONNMGR_STATUS_WAITINGFORRESOURCE         0x41  // @constdefine Resource is in use by another connection
#define CONNMGR_STATUS_WAITINGFORNETWORK          0x42  // @constdefine Network is used by higher priority thread or device is roaming.

#define CONNMGR_STATUS_WAITINGDISCONNECTION       0x80  // @constdefine Connection is being brought down
#define CONNMGR_STATUS_WAITINGCONNECTIONABORT     0x81  // @constdefine Aborting connection attempt


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Returns status about the current connection.
//
// @comm none
//
// -----------------------------------------------------------------------------
HRESULT WINAPI ConnMgrConnectionStatus
    (
    HANDLE hConnection,    // @parm Handle to connection, returned from ConnMgrEstablishConnection
    DWORD *pdwStatus       // @parm Returns current connection status, one of CONNMGR_STATUS_*
    );

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Deletes specified connection request, potentially dropping the physical connection.
//
// @comm none
//
// -----------------------------------------------------------------------------
HRESULT WINAPI ConnMgrReleaseConnection
    (
    HANDLE hConnection,    // @parm Handle to connection, returned from ConnMgrEstablishConnection
    LONG lCache            // @parm ConnMgr can cache connection
    );

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Changes a connection's priority.
//
// @comm none
//
// -----------------------------------------------------------------------------
HRESULT WINAPI ConnMgrSetConnectionPriority
    (
    HANDLE hConnection,    // @parm Handle to connection, returned from ConnMgrEstablishConnection
    DWORD dwPriority       // @parm New priority
    );

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func General purpose (backdoor) API for exchanging information with planner or providers.
//
// @comm none
//
// -----------------------------------------------------------------------------
HRESULT WINAPI ConnMgrProviderMessage
    (
    HANDLE hConnection,         // @parm Optional, Handle to connection
    const GUID *pguidProvider,  // @parm Provider GUID
    DWORD *pdwIndex,            // @parm Optional index, used to address multiple providers associated with connection
    DWORD dwMsg1,               // @parm General param 1
    DWORD dwMsg2,               // @parm General param 2
    PBYTE pParams,              // @param Pointer to params structure
    ULONG cbParamSize           // @param size of params structure
    );

#define CONNMGR_MAX_DESC 128    // @constdefine Max size of a network description

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct CONNMGR_DESTINATION_INFO | Information about a specific network
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct _CONNMGR_DESTINATION_INFO
{
    GUID guid;  // @field GUID associated with network
    TCHAR szDescription[CONNMGR_MAX_DESC];  // @field Description of network
    BOOL fSecure; // @field Is it OK to allow multi-homing on the network
} CONNMGR_DESTINATION_INFO;


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Enumerates available networks.
//
// @comm none
//
// -----------------------------------------------------------------------------
HRESULT WINAPI ConnMgrEnumDestinations
    (
    int nIndex,                            // @param Index of network
    CONNMGR_DESTINATION_INFO *pDestInfo    // @param ptr to structure to fill in with info about network
    );

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SCHEDULEDCONNECTIONINFO | Information about a scheduled connection
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct _SCHEDULEDCONNECTIONINFO
{
    GUID guidDest;                  // @field Guid of network
    UINT64 uiStartTime;             // @field Starting time, same ref as filetime
    UINT64 uiEndTime;               // @field Ending time, same ref as filetime
    UINT64 uiPeriod;                // @field Period between schedule attempts
    TCHAR szAppName[MAX_PATH];    // @field App name to execute when scheduled
    TCHAR szCmdLine[MAX_PATH];    // @field Cmd line to execute when scheduled
    TCHAR szToken[32];            // @field Unique token identifying this scheduled connection
    BOOL bPiggyback;                // @field If true, execute app whenever network is available
} SCHEDULEDCONNECTIONINFO;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Registers a scheduled connection
//
// @comm none
//
// -----------------------------------------------------------------------------
HRESULT WINAPI ConnMgrRegisterScheduledConnection
    (
     SCHEDULEDCONNECTIONINFO *pSCI  // @param Ptr to struct describing scheduled connection
     );

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Unregisters a scheduled connection
//
// @comm none
//
// -----------------------------------------------------------------------------
HRESULT WINAPI ConnMgrUnregisterScheduledConnection
    (
     LPCTSTR pwszToken  // @param Token of scheduled connection to unregister
     );

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Maps a URL to a destination network GUID
//
// @comm none
//
// -----------------------------------------------------------------------------
HRESULT WINAPI ConnMgrMapURL
    (
     LPCTSTR pwszURL, // @parm URL to map
     GUID *pguid,     // @parm Returned network GUID
     DWORD *pdwIndex  // @parm Index in table for next search
     );

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Returns a handle to an event which becomes signaled when the ConnMgr API
// is ready to be used. Caller is responsible for calling CloseHandle on the returned event.
//
// @comm none
//
// -----------------------------------------------------------------------------
HANDLE WINAPI ConnMgrApiReadyEvent();

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Defines the type of a connection reference
//
// @comm none
//
// -----------------------------------------------------------------------------

typedef enum _ConnMgrConRefTypeEnum
{
    ConRefType_NAP = 0,     // @constdefine NAP connection reference
    ConRefType_PROXY        // @constdefine PROXY connection reference
} ConnMgrConRefTypeEnum;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Maps a connection reference to its corresponding GUID
//
// @comm none
//
// -----------------------------------------------------------------------------

HRESULT WINAPI ConnMgrMapConRef
(
    ConnMgrConRefTypeEnum e,        // @parm Specify type of connection reference
    LPCTSTR szConRef,               // @parm Connection reference to map
    GUID *pGUID                     // @parm Returned connection reference GUID
);

#ifdef __cplusplus
}
#endif

#endif // _connmgr_h_
