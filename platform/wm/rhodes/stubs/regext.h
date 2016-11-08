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
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// State and Notification Broker
//
// This file defines the State and Notification Broker API.
// For the list of specific states and notifications available from the 
// State and Notification Broker, see SNApi.h
//
#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define E_ALREADY_REGISTERED         HRESULT_FROM_WIN32(ERROR_ALREADY_REGISTERED)
#define E_DATATYPE_MISMATCH          HRESULT_FROM_WIN32(ERROR_DATATYPE_MISMATCH)
#define E_INSUFFICIENT_BUFFER        HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER)
#define E_INVALID_HANDLE             HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE)
#define E_NOT_READY                  HRESULT_FROM_WIN32(ERROR_NOT_READY)

DECLARE_HANDLE(HREGNOTIFY); // transient notification handle


// **************************************************************************
// Enumeration Name: REG_COMPARISONTYPE
// 
// Purpose: used to define how state property values should be compared to
//          target values for conditional change notifications. When executing 
//          the REG_COMPARISONTYPE, the changed value is always used as the
//          l-value, that is, REG_CT_LESS would mean fire the notification if
//          the changed value is less than the target value.
//
// Description:
//    the following shows what statements will be true when the comparison is
//    done. In the actual implementation a case insensitive CompareString using
//    the system locale (LOCALE_SYSTEM_DEFAULT) is used to compare the strings,
//    and the DWORDs are always masked before the comparison. The following
//    information is just to clarify the intent of the REG_COMPARISONTYPE
//    values.
//    Let "cv" represent the changed value and "psz" or "dw" represent the
//    TargetValue specified in the NOTIFICATIONCONDITION structure. Let "l"
//    represent the length of the string specified in psz (wcslen(psz)) and
//    "cvl" represents the string length of the changed value (wcslen(cv)).
//                                        REG_SZ                REG_DWORD
//    REG_CT_ANYCHANGE        |           always             |   always   |
//    REG_CT_EQUAL            | (0 == strcmp(cv, psz))       | (cv == dw) |
//    REG_CT_NOT_EQUAL        | (0 != strcmp(cv, psz))       | (cv != dw) | 
//    REG_CT_GREATER          | (0 <  strcmp(cv, psz))       | (cv >  dw) |
//    REG_CT_GREATER_OR_EQUAL | (0 <= strcmp(cv, psz))       | (cv >= dw) | 
//    REG_CT_LESS             | (0 >  strcmp(cv, psz))       | (cv <  dw) |
//    REG_CT_LESS_OR_EQUAL    | (0 >= strcmp(cv, psz))       | (cv <= dw) |
//    REG_CT_CONTAINS         | (0 != strstr(cv, psz))       |     N/A    |
//    REG_CT_STARTS_WITH      | (0 == strncmp(cv, psz, l))   |     N/A    |
//    REG_CT_ENDS_WITH        | (0 == strcmp(cv+cvl-l, psz)) |     N/A    |
//
// **************************************************************************
typedef enum tagREG_COMPARISONTYPE 
{ 
    REG_CT_ANYCHANGE,
    REG_CT_EQUAL,
    REG_CT_NOT_EQUAL,
    REG_CT_GREATER,
    REG_CT_GREATER_OR_EQUAL,
    REG_CT_LESS,
    REG_CT_LESS_OR_EQUAL,
    REG_CT_CONTAINS,
    REG_CT_STARTS_WITH,
    REG_CT_ENDS_WITH
} REG_COMPARISONTYPE;


// **************************************************************************
// Structure Name: NOTIFICATIONCONDITION
// 
// Purpose: used to define a condition under which property state change
//          notifications should be fired
//
// Description:
//    REG_COMPARSIONTYPE ctComparisonType - how to compare the changed value
//                                          with the value specified in
//                                          TargetValue
//    DWORD dwMask - If this value is set to any value other than 0 then
//                   TargetValue is interpreted as a DWORD. This mask is applied
//                   to the changed DWORD value as well as TargetValue before
//                   the comparison is done. If this value is set to 0 then
//                   TargetValue is interpreted as a string.
//    union TargetValue - if dwMask is set to 0 then the comparison is done
//                        between psz and the changed value. If dwMask is set
//                        to any value other than 0 then the comparison is done
//                        between the masked dw and the masked changed value.
//                        If the changed value is not of type REG_SZ nor
//                        REG_DWORD or if the changed value is of type REG_DWORD
//                        and dwMask is set to 0 or if the changed value is of
//                        type REG_SZ and dwMask is not set to 0, then the
//                        notification is ignored. If the changed value is
//                        deleted and the comparison is to be done against dw,
//                        0 is used as the changed value and the comparison
//                        proceeds as usual. If the value is deleted and the
//                        comparison is to be done against psz, NULL is used as
//                        the changed value and only REG_CT_EQUAL and
//                        REG_CT_NOT_EQUAL are processed by the comparison, the
//                        deletion notification is ignored if any other
//                        ctComparisonType is used. When REG_CT_ANYCHANGE is
//                        specified for ctComparisonType, this value is ignored.
//
// **************************************************************************
typedef struct tagNOTIFICATIONCONDITION
{
    REG_COMPARISONTYPE ctComparisonType;
    DWORD dwMask;
    union
    {
        LPCTSTR psz;
        DWORD dw;
    } TargetValue;
} NOTIFICATIONCONDITION;


// **************************************************************************
// Flags: RNAF_*
//
// Purpose: defines the flags that can be passed to RegistryNotifyApp
//
// Description:
//    RNAF_NONAMEONCMDLINE - the "/notify "pszName"" string will not be passed
//                           on the command line to the application when
//                           launched
//
// **************************************************************************
#define RNAF_NONAMEONCMDLINE 0x00000001


// **************************************************************************
// Structure Name: NOTIFYMSGQUEUEPACKET
// 
// Purpose: used to define the layout of the packets sent to the message queue
//          in RegistryNotifyMsgQueue
//
// Description:
//    HREGNOTIFY hNotify - the same hNotify as returned in phNotify on the call
//                         to RegistryNotifyMsgQueue
//    DWORD dwUserData -  the user data passed as dwUserData on the call to
//                        RegistryNotifyMsgQueue
//    UINT cbData - the number of bytes to follow (for deletes this value is 0)
//    BYTE rgData[] - the new value for pszValueName
//
// **************************************************************************
typedef struct tagNOTIFYMSGQUEUEPACKET
{
    HREGNOTIFY hNotify;
    DWORD dwUserData;
    UINT cbData;
    BYTE rgData[1];
} NOTIFYMSGQUEUEPACKET;


// **************************************************************************
// Function Prototype: REGISTRYNOTIFYCALLBACK
//
// Purpose: defines the prototype of the callback used by RegistryNotifyCallback
//
// Arguments:
//    IN HREGNOTIFY hNotify - the handle to a valid HREGNOTIFY, this will be the
//                            same handle returned from RegistryNotifyCallback
//    IN DWORD dwUserData - user data that was passed to RegistryNotifyCallback
//    IN const PBYTE pData - a pointer to the new value for the value, this
//                           will be set to NULL if the value was deleted
//    IN const UINT cbData - the number of bytes to pointed to by pData, this
//                           value will be set to 0 if the value was deleted
//
// Description:
//    this callback is used to notify clients that registered for notifications
//    using RegistryNotifyCallback. It is safe to call RegistryCloseNotification
//    from within the callback if no further notifications are required. All
//    notifications happen synchronously, so it is important to not block while
//    executing this callback.
//
// **************************************************************************
typedef void (*REGISTRYNOTIFYCALLBACK)(HREGNOTIFY hNotify,
                                       DWORD dwUserData,
                                       const PBYTE pData,
                                       const UINT cbData);


// **************************************************************************
// Function Name: RegistryGetDWORD
// 
// Purpose: used to read a REG_DWORD registry value
//
// Arguments:
//    IN HKEY hKey - handle to a currently open key, or a predefined root value
//    IN LPCTSTR pszSubKey - the key under which the value is stored (if this
//                           value is null pszValueName is assumed to be under
//                           hKey)
//    IN LPCTSTR pszValueName - the name of the value to retrieve (may be NULL
//                              to retrieve the default value)
//    OUT DWORD * pdwData - a pointer to the buffer which will receive the 
//                          data associated with the value
//
// Return Values:
//    HRESULT
//    S_OK - the data was copied to the buffer
//    E_INVALIDARG - hKey or pdwData is invalid
//    E_DATATYPE_MISMATCH - the value is not of type REG_DWORD
//    An error value returned from RegOpenKeyEx or RegQueryValueEx wrapped as 
//    a FACILITY_WIN32 HRESULT
//
// Results:
//    SUCCEEDED - pData points to the data associated with the value
//    FAILED - no change.
// 
// Description:
//    the DWORD associated with the value is copied to the buffer pointed to
//    by pData. If the key pointed to by hKey+pszSubKey does not exist,
//    RegistryGetDWORD will fail since it uses RegOpenKey to access the key
//
// **************************************************************************
HRESULT WINAPI RegistryGetDWORD(HKEY hKey,
                                LPCTSTR pszSubKey,
                                LPCTSTR pszValueName,
                                DWORD * pdwData);


// **************************************************************************
// Function Name: RegistryGetString
// 
// Purpose: used to read a REG_SZ registry value
//
// Arguments:
//    IN HKEY hKey - handle to a currently open key, or a predefined root value
//    IN LPCTSTR pszSubKey - the key under which the value is stored (if this
//                           value is null pszValueName is assumed to be under
//                           hKey)
//    IN LPCTSTR pszValueName - the name of the value to retrieve (may be NULL
//                              to retrieve the default value)
//    OUT LPTSTR pszData - a pointer to the buffer which will receive the 
//                          data associated with the value
//    IN UINT cchData - a pointer to the variable which is length in characters
//                      of the buffer pointed to by pData
//
// Return Values:
//    HRESULT
//    S_OK - the data was copied to the buffer
//    E_INVALIDARG - hKey or pszData is invalid
//    E_DATATYPE_MISMATCH - the value is not of type REG_SZ
//    E_INSUFFICIENT_BUFFER - the size of the buffer pointed to by pszData, as
//                            determined by cchData is not large enough to hold
//                            the string
//    An error value returned from RegOpenKeyEx or RegQueryValueEx wrapped as 
//    a FACILITY_WIN32 HRESULT
//
// Results:
//    SUCCEEDED - pData points to the data associated with the value
//    FAILED - no change.
// 
// Description:
//    the string associated with the value is copied to the buffer pointed to
//    by pData. If the key pointed to by hKey+pszSubKey does not exist, 
//    RegistryGetString will fail since it uses RegOpenKey to access the key
//
// **************************************************************************
HRESULT WINAPI RegistryGetString(HKEY hKey,
                                 LPCTSTR pszSubKey,
                                 LPCTSTR pszValueName,
                                 LPTSTR pszData,
                                 UINT cchData);


// **************************************************************************
// Function Name: RegistrySetDWORD
// 
// Purpose: used to set a REG_DWORD registry value
//
// Arguments:
//    IN HKEY hKey - handle to a currently open key, or a predefined root value
//    IN LPCTSTR pszSubKey - the key under which the value is stored (if this
//                           value is null pszValueName is assumed to be under
//                           hKey)
//    IN LPCTSTR pszValueName - the name of the value to set (may be NULL
//                              to set the default value)
//    IN DWORD dwData - the new value
//
// Return Values:
//    HRESULT
//    S_OK - the data value for pszValueName was changed to the data in dwData
//    E_INVALIDARG - hKey is invalid
//    E_DATATYPE_MISMATCH - the value is not of type REG_DWORD
//    An error value returned from RegOpenKeyEx or RegQueryValueEx wrapped as 
//    a FACILITY_WIN32 HRESULT
//
// Results:
//    SUCCEEDED - the data associated with the value was changed
//    FAILED - no change
// 
// Description:
//    the data associated with the value is changed to the new value. If the
//    key pointed to by hKey+pszSubKey does not exist, RegistrySetDWORD will
//    fail since it uses RegOpenKey to access the key
//
// **************************************************************************
HRESULT WINAPI RegistrySetDWORD(HKEY hKey,
                                LPCTSTR pszSubKey,
                                LPCTSTR pszValueName,
                                DWORD dwData);


// **************************************************************************
// Function Name: RegistrySetString
// 
// Purpose: used to set a REG_SZ registry value
//
// Arguments:
//    IN HKEY hKey - handle to a currently open key, or a predefined root value
//    IN LPCTSTR pszSubKey - the key under which the value is stored (if this
//                           value is null pszValueName is assumed to be under
//                           hKey)
//    IN LPCTSTR pszValueName - the name of the value to set (may be NULL
//                              for to set the default value)
//    IN LPCTSTR pszData - the new value. This string must be null terminated.
//
// Return Values:
//    HRESULT
//    S_OK - the data value for pszValueName was changed to the data in pszData
//    E_INVALIDARG - hKey or pszData is invalid
//    E_DATATYPE_MISMATCH - the value is not of type REG_SZ
//    An error value returned from RegOpenKeyEx or RegQueryValueEx wrapped as 
//    a FACILITY_WIN32 HRESULT
//    An error HRESULT returned from StringCbLength
//
// Results:
//    SUCCEEDED - the data associated with the value was changed
//    FAILED - no change
// 
// Description:
//    the data associated with the value is changed to the new value. If the
//    key pointed to by hKey+pszSubKey does not exist, RegistrySetString will
//    fail since it uses RegOpenKey to access the key
//
// **************************************************************************
HRESULT WINAPI RegistrySetString(HKEY hKey,
                                 LPCTSTR pszSubKey,
                                 LPCTSTR pszValueName,
                                 LPCTSTR pszData);

// **************************************************************************
// Function Name: RegistryDeleteValue
// 
// Purpose: used to delete a registry value
//
// Arguments:
//    IN HKEY hKey - handle to a currently open key, or a predefined root value
//    IN LPCTSTR pszSubKey - the key under which the value is stored (if this
//                           value is null pszValueName is assumed to be under
//                           hKey)
//    IN LPCTSTR pszValueName - the name of the value to delete (may be NULL
//                              to delete the default value)
//
// Return Values:
//    HRESULT
//    S_OK - The value was deleted successfully
//    S_FALSE - The key or value does not exist.
//    E_INVALIDARG - hKey is invalid
//
// Results:
//    SUCCEEDED - The value pszValueName under the key hKey and subkey pszSubKey is
//                deleted
//    FAILED - no change.
// 
// Description:
//    The value under the hKey+pszSubKey will be deleted if it exists, if the value
//    does not exist, there is no change and this method will succeed.
//    If the key pointed to by hKey+pszSubKey does not exist,
//    RegistryDeleteValue will fail since it uses RegOpenKey to access the key
//
// **************************************************************************
HRESULT WINAPI RegistryDeleteValue(HKEY hKey, 
                                   LPCTSTR pszSubKey,
                                   LPCTSTR pszValueName);

// **************************************************************************
// Function Name: RegistryTestExchangeDWORD
// 
// Purpose: used to atomically set a value based on a condition
//
// Arguments:
//    IN HKEY hKey - handle to a currently open key, or a predefined root value
//    IN LPCTSTR pszSubKey - the key under which the value is stored (if this
//                           value is null pszValueName is assumed to be under
//                           hKey)
//    IN LPCTSTR pszValueName - the name of the value to set (may be NULL
//                              to set the default value)
//    IN DWORD dwOldValue - the value to check against
//    IN DWORD dwNewValue - the value to set conditionally
//
// Return Values:
//    HRESULT
//    S_OK - the data value was changed to dwNewValue
//    S_FALSE - the target value was not set to dwNewValue because the DWORD
//              value assocated with pszValueName was not equal to dwOldValue
//    E_INVALIDARG - the handle or one of the pointers passed in was invalid
//    E_DATATYPE_MISMATCH - the value is not of type REG_DWORD
//    An error value wrapped as a FACILITY_WIN32 HRESULT
//
// Results:
//    SUCCEEDED - the data value associated with pszData was dwOldValue and is
//                now dwNewValue
//    FAILED - no change
//
// Description:
//    This function acts the same way as the Win32 InterlockedTestExchange
//    function. Is an interlocked function — in other words, it can be
//    considered atomic. It checks to see if the DWORD value associated with
//    pszValueName is equal to OldValue. If so, it sets the target to NewValue,
//    otherwise, it fails.
//
// **************************************************************************
HRESULT WINAPI RegistryTestExchangeDWORD(HKEY hKey,
                                         LPCTSTR pszSubKey,
                                         LPCTSTR pszValueName,
                                         DWORD dwOldValue,
                                         DWORD dwNewValue);


// **************************************************************************
// Function Name: RegistryNotifyApp
// 
// Purpose: used to request that an app be launched or notified when a
//          specified value has been changed
//
// Arguments:
//    IN HKEY hKey - handle to a currently open key, or a predefined root value
//    IN LPCTSTR pszSubKey - the key under which the value is stored (if this
//                           value is null pszValueName is assumed to be under
//                           hKey)
//    IN LPCTSTR pszValueName - the name of the value on which change
//                              notifications are requested (may be NULL to
//                              indicate the default value)
//    IN LPCTSTR pszName - a case insensitive user defined string representing
//                         this notification request, the string should be
//                         passed to RegistryStopNotification when notifications
//                         are no longer needed. Each notification in the system
//                         must have a unique name.
//    IN LPCTSTR pszApp - pointer to string that is the path to the executable
//                        to launch
//    IN LPCTSTR pszClass  - once the executable is launched, or if it is
//    IN LPCTSTR pszWindow   already running, a window with this window name and
//                           class type is located in the process and the
//                           notification is passed to it. If both of these
//                           parameter are null this function will only launch
//                           the application.
//    IN UINT msg - the message that will be passed to the window
//    IN DWORD dwFlags - one or more of the RNAF_* flags which will define the
//                       behavior of RegistryNotifyApp
//    IN NOTIFICATIONCONDITION * pCondition - the condition under which change
//                                            notifications should be sent when
//                                            a comparison of it to the new
//                                            registry value is TRUE (may be
//                                            NULL to indicate that any change
//                                            should result in a notification)
//
// Return Values:
//    HRESULT
//    S_OK - the request for notification has been added to the notification 
//           list
//    E_INVALIDARG - hKey, pszApp or pszName is invalid
//    E_ALREADY_REGISTERED - a notification with a name equal to that specified
//                           by pszName already exists
//    An error value wrapped as a FACILITY_WIN32 HRESULT
//
// Results:
//    SUCCEEDED - the caller will now be notified every time a change to this
//                value is made.
//    FAILED - no change
//
// Description:  
//    The notification request is added to the notification list. The caller
//    must call RegistryStopNotification to stop further notifications. This
//    type of notification request is permanent, that is, the notification
//    request will be active even if the device is reset. On notification, this
//    function will determine if an executable with the name specified in pszApp
//    is already running by doing a FindWindow on the class name and window name
//    specified by pszClass and pszWindow, if not it launches the app. The
//    command line passed to the application should be specified in the pszApp
//    string. If RNAF_NONAMEONCMDLINE is not specified in dwFlags, the following
//    is also appended to the user defined command line:
//        /notify "pszName"
//        pszName - the handle string passed to the function in the pszName
//                  parameter
//    After the app is launched a FindWindow is done looking for a window with 
//    the class name and window name specified in pszClass and pszWindow. If the
//    window is found, the message specified by msg is posted exactly as in
//    RegistryNotifyWindow, via PostMessage. The parameters to the PostMessage
//    are as follows:
//        WPARAM - for values of type REG_DWORD this is the new value or 0
//                 if the value was deleted; for all other types this value
//                 is 0
//        LPARAM - 0
//    The msg parameter should be unique for each call to RegistryNotifyApp
//    so that the client can differentiate between the multiple notifications.
//    The client will be notified when the value is added as well as on changes.
//    When a notification arrives, if the application pointed to by pszApp can
//    not be launched or a window with a class of type pszClass can not be found
//    or the PostMessage fails, the notification will be removed from the
//    notification list.
//    It is also possible to preregister a RegistryNotifyApp notification by
//    adding the notification directly to the registry. The notification will
//    become active the next time the device is soft reset. The registry format
//    is as follows (<notification name> represents the identifier of the
//    notification, that is, the name to pass to RegistryStopNotification):
//    [HKEY_LOCAL_MACHINE\System\Notifications\<notification name>]
//      ; the following are required
//        "HKEY"=dword:0x80000001                       ; a root registry key
//        "Application"="MyApp.exe param1 param2"         ; the app to launch
//        "Trust"=dword:0  ; trust level required to delete this notification
//      ; the following are optional
//        "Key"="System\\State"                       ; the registry key path
//        "Value Name"="SomeState"                       ; the value to watch
//        "Class Name"="MYWNDCLASS"              ; the class name to look for 
//        "Window Name"="My Window"             ; the window name to look for
//        "Message"=dword:400                           ; the message to post
//        "Flags"=dword:1                   ; any combination of RNAF_* flags
//        "Conditional Comparison"=dword:1             ; a REG_COMPARISONTYPE
//        "Conditional Mask"=dword:0                   ; the conditional mask
//        "Conditional Target"="change"          ; the psz conditional target
//        "Conditional Target"=dword:1            ; the dw conditional target
// **************************************************************************
HRESULT WINAPI RegistryNotifyApp(HKEY hKey,
                                 LPCTSTR pszSubKey,
                                 LPCTSTR pszValueName,
                                 LPCTSTR pszName,
                                 LPCTSTR pszApp,
                                 LPCTSTR pszClass,
                                 LPCTSTR pszWindow,
                                 UINT msg,
                                 DWORD dwFlags,
                                 NOTIFICATIONCONDITION * pCondition);


// **************************************************************************
// Function Name: RegistryNotifyWindow
// 
// Purpose: used to request that a window be notified when a specified value
//          has been changed
//
// Arguments:
//    IN HKEY hKey - handle to a currently open key, or a predefined root value
//    IN LPCTSTR pszSubKey - the key under which the value is stored (if this
//                           value is null pszValueName is assumed to be under
//                           hKey)
//    IN LPCTSTR pszValueName - the name of the value on which change
//                              notifications are requested (may be NULL to
//                              indicate the default value)
//    IN HWND hWnd - the handle of the window to which the message will be sent
//    IN UINT msg - the message that will be passed to the window
//    IN DWORD dwUserData - user data that will be passed back to the user
//                          with the notification
//    IN NOTIFICATIONCONDITION * pCondition - the condition under which change
//                                            notifications should be sent when
//                                            a comparison of it to the new
//                                            registry value is TRUE (may be
//                                            NULL to indicate that any change
//                                            should result in a notification)
//    OUT HREGNOTIFY * phNotify - receives the handle to the notification
//                                request. This handle should be closed using
//                                RegistryCloseNotification when notifications
//                                on this key are no longer needed.
//
// Return Values:
//    HRESULT
//    S_OK - the request for notification has been added to the notification
//           list
//    E_INVALIDARG - hKey, phNotify or hWnd is invalid
//    An error value wrapped as a FACILITY_WIN32 HRESULT
//
// Results:
//    SUCCEEDED - the caller will now be notified every time a change to this
//                value is made.
//    FAILED - no change
// 
// Description:
//    The msg parameter should be unique for each call to RegistryNotifyWindow
//    so that the client can differentiate between the multiple notifications.
//    The notification request is added to the notification list. The caller
//    must call RegistryCloseNotification to stop further notifications and to
//    close the notification handle. This type of notification request is
//    transient, that is, if the device is reset the notification request will
//    no longer exist. When the value specified by pszValueName is changed, the
//    client is notified via a PostMessage. If the PostMessage fails or the
//    window specified by hWnd is no longer valid the notification request is
//    removed from the notification queue and the handle returned in phNotify
//    is closed.
//    The parameters passed on the PostMessage are as follows:
//      WPARAM - for values of type REG_DWORD this is the new value or 0
//               if the value was deleted; for all other types this value is 0
//      LPARAM - the value passed in on dwUserData
//    If the value does not exist at the time of the call to
//    RegistryNotifyWindow, the client will be notified when the value is added.
//
// **************************************************************************
HRESULT WINAPI RegistryNotifyWindow(HKEY hKey,
                                    LPCTSTR pszSubKey,
                                    LPCTSTR pszValueName,
                                    HWND hWnd,
                                    UINT msg,
                                    DWORD dwUserData,
                                    NOTIFICATIONCONDITION * pCondition,
                                    HREGNOTIFY * phNotify);


// **************************************************************************
// Function Name: RegistryNotifyMsgQueue
// 
// Purpose: used to request that a message queue be notified when a specified
//          value has been changed
//
// Arguments:
//    IN HKEY hKey - handle to a currently open key, or a predefined root value
//    IN LPCTSTR pszSubKey - the key under which the value is stored (if this
//                           value is null pszValueName is assumed to be under
//                           hKey)
//    IN LPCTSTR pszValueName - the name of the value on which change
//                              notifications are requested (may be NULL to
//                              indicate the default value)
//    IN LPCTSTR pszMsgQueue - A pointer to a string that is the name of the
//                             message queue to notify. If this message queue
//                             has not yet been created, RegistryNotifyMsgQueue
//                             will create it.
//    IN DWORD dwUserData - user data that will be passed back to the user
//                          with the notification
//    IN NOTIFICATIONCONDITION * pCondition - the condition under which change
//                                            notifications should be sent when
//                                            a comparison of it to the new
//                                            registry value is TRUE (may be
//                                            NULL to indicate that any change
//                                            should result in a notification)
//    OUT HREGNOTIFY * phNotify - receives the handle to the notification
//                                request. This handle should be closed using
//                                RegistryCloseNotification when notifications
//                                on this key are no longer needed.
//
// Return Values:
//    HRESULT
//    S_OK - the request for notification has been added to the notification
//           list
//    E_INVALIDARG - hKey, phNotify, or pszMsgQueue is invalid
//    An error value wrapped as a FACILITY_WIN32 HRESULT
//
// Results:
//    SUCCEEDED - the caller will now be notified every time a change to this
//                value is made.
//    FAILED - no change
// 
// Description:  
//    The notification request is added to the notification list. The caller
//    must call RegistryCloseNotification to stop further notifications and
//    close the notification handle. This type of notification request is
//    transient, that is, if the device is reset the notification request will
//    no longer exist. The client will be notified of changes via the message
//    queue. Once the client is notified, information about the changed key can
//    be retrieved using the ReadMsgQueue function. The data returned is in the
//    following format:
//      | HREGNOTIFY hNotify | DWORD dwUserData | UINT byte count | BYTE[] new value |
//      hNotify - the same hNotify as returned in phNotify
//      dwUserData - the user data passed as dwUserData
//      byte count - the number of bytes to follow (for deletes this value is 0)
//      new value - the new value for pszValueName
//    The data sent to the message queue can be cast to a NOTIFYMSGQUEUEPACKET
//    for easier access. If the value does not exist at the time of the call to
//    RegistryNotifyMsgQueue, the client will be notified when the value is
//    added.
//
// **************************************************************************
HRESULT WINAPI RegistryNotifyMsgQueue(HKEY hKey,
                                      LPCTSTR pszSubKey,
                                      LPCTSTR pszValueName,
                                      LPCTSTR pszMsgQueue,
                                      DWORD dwUserData,
                                      NOTIFICATIONCONDITION * pCondition,
                                      HREGNOTIFY * phNotify);


// **************************************************************************
// Function Name: RegistryNotifyCallback
//
// Purpose: used to request that a callback be notified when a specified
//          value has been changed
//
// Arguments:
//    IN HKEY hKey - handle to a currently open key, or a predefined root value
//    IN LPCTSTR pszSubKey - the key under which the value is stored (if this
//                           value is null pszValueName is assumed to be under
//                           hKey)
//    IN LPCTSTR pszValueName - the name of the value on which change
//                              notifications are requested (may be NULL to
//                              indicate the default value)
//    IN REGISTRYNOTIFYCALLBACK pfnRegistryNotifyCallback - A pointer to a
//                                                          function that will
//                                                          be called back when
//                                                          a notification
//                                                          arrives
//    IN DWORD dwUserData - user data that will be passed back to the user
//                          with the notification
//    IN NOTIFICATIONCONDITION * pCondition - the condition under which change
//                                            notifications should be sent when
//                                            a comparison of it to the new
//                                            registry value is TRUE (may be
//                                            NULL to indicate that any change
//                                            should result in a notification)
//    OUT HREGNOTIFY * phNotify - receives the handle to the notification
//                                request. This handle should be closed using
//                                RegistryCloseNotification when notifications
//                                on this key are no longer needed.
//
// Return Values:
//    HRESULT
//    S_OK - the request for notification has been added to the notification
//           list
//    E_INVALIDARG - hKey, phNotify, or pfnRegistryNotifyCallback is invalid
//    An error value wrapped as a FACILITY_WIN32 HRESULT
//
// Results:
//    SUCCEEDED - the caller will now be notified every time a change to this
//                value is made.
//    FAILED - no change
// 
// Description:  
//    The notification request is added to the notification list. The caller
//    must call RegistryCloseNotification to stop further notifications and
//    close the notification handle. This type of notification request is
//    transient, that is, if the device is reset the notification request will
//    no longer exist. The client will be notified of changes via the callback,
//    which is executed on private thread seperate from the thread that called
//    RegistryNotifyCallback. If the value does not exist at the time of the
//    call to RegistryNotifyCallback, the client will be notified when the value
//    is added.
//
// **************************************************************************
HRESULT WINAPI RegistryNotifyCallback(HKEY hKey,
                                      LPCTSTR pszSubKey,
                                      LPCTSTR pszValueName,
                                      REGISTRYNOTIFYCALLBACK pfnRegistryNotifyCallback,
                                      DWORD dwUserData,
                                      NOTIFICATIONCONDITION * pCondition,
                                      HREGNOTIFY * phNotify);


// **************************************************************************
// Function Name: RegistryCloseNotification
// 
// Purpose: used remove a request for notifications from the notification list
//          and close the notification handle
//
// Arguments:
//    IN HREGNOTIFY hNotify - the handle to a valid HREGNOTIFY, must have been
//                            returned one of the RegistryNotify* functions
//
// Return Values:
//    HRESULT
//    S_OK - the notification request was removed from the list
//    E_INVALID_HANDLE - hNotify is invalid
//    An error value wrapped as a FACILITY_WIN32 HRESULT
//
// Results:
//    SUCCEEDED - the notification request has been removed, hNotify is now
//                an invalid handle
//    FAILED - no change
// 
// Description:  
//    the notification list is searched for references to hNotify, if found
//    they are removed the queue and the memory associated with hNotify is
//    freed. Any notifications that have not yet been dispatched to the client
//    will be lost.
//
// **************************************************************************
HRESULT WINAPI RegistryCloseNotification(HREGNOTIFY hNotify);


// **************************************************************************
// Function Name: RegistryStopNotification
// 
// Purpose: used to remove a request for a permanent notification from 
//          the notification list
//
// Arguments:
//    IN LPCTSTR pszName - a case insensitive string representing a permanent
//                         notification that has already been registered using
//                         RegistryNotifyApp
//
// Return Values:
//    HRESULT
//    S_OK - the notification request was removed from the list
//    E_INVALIDARG - pszName is invalid
//    An error value wrapped as a FACILITY_WIN32 HRESULT
//
// Results:
//    SUCCEEDED - the notification request has been removed
//    FAILED - no change
// 
// Description:  
//    The notification list is searched for references to the notification,
//    if found they are removed the queue. Any notifications that have not
//    yet been dispatched to the client will be lost.
//
// **************************************************************************
HRESULT WINAPI RegistryStopNotification(LPCTSTR pszName);


// **************************************************************************
// Function Name: RegistryBatchNotification
// 
// Purpose: used to batch frequently occurring notifications
//
// Arguments:
//    IN HREGNOTIFY hNotify - the handle to a valid HREGNOTIFY, must have been
//                            returned one of the RegistryNotify* functions
//    IN DWORD dwMillisecondsIdle - the number of milliseconds the value should
//                                  be constant before the notification is fired
//    IN DWORD dwMillisecondsMax - the maximum number of milliseconds between
//                                 the time the first change happens and the
//                                 notification is sent
//
// Return Values:
//    HRESULT
//    S_OK - the notification request was removed from the list
//    E_INVALIDARG - dwMillisecondsIdle is set to INFINITE
//    E_INVALID_HANDLE - hNotify is invalid
//    An error value wrapped as a FACILITY_WIN32 HRESULT
//
// Results:
//    SUCCEEDED - the new batch times are set
//    FAILED - no change
// 
// Description:  
//    when a value is changed, the notification system will wait
//    dwMillisecondsIdle milliseconds and then send the notification. If a
//    another change happens during that wait period the timer is reset and
//    the notification system will wait another dwMillisecondsIdle
//    milliseconds to send the notification. In order to ensure that the
//    dwMillisecondsIdle doesn’t prevent the notification from ever being
//    sent, dwMillisecondsMax is used. dwMillisecondsMax is the maximum
//    number of milliseconds that can pass from the time the first notification
//    arrives and the time notification is sent. If dwMillisecondsMax is set to
//    INFINITE the notification will batch until the value is idle.
//    dwMillisecondsIdle can not be INFINITE.
//
// **************************************************************************
HRESULT WINAPI RegistryBatchNotification(HREGNOTIFY hNotify,
                                         DWORD dwMillisecondsIdle,
                                         DWORD dwMillisecondsMax);


#ifdef __cplusplus
}
#endif


