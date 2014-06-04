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
#pragma once

////////////////////////////////////////////////////////////////////////////////
//
// @enum    BTH_RADIO_MODE |
//          Enumerate all the possible modes of operation of the bluetooth radio
//
////////////////////////////////////////////////////////////////////////////////

enum BTH_RADIO_MODE
{
    BTH_POWER_OFF,
    BTH_CONNECTABLE,
    BTH_DISCOVERABLE
};

#ifdef __cplusplus
extern "C"{
#endif 

////////////////////////////////////////////////////////////////////////////////
//
// @func    int | BthSetMode |
//          This function set the bluetooth Mode of operation and reflects it in 
//          the control panel.  It also persist that state across hardware insertion
//          and reboot
//
// @syntax  BthSetMode( DWORD dwMode )
//
// @parm    DWORD | dwMode |
//          BTH_POWER_OFF to turn the bluetooth radio OFF
//          BTH_CONNECTABLE to turn the bluetooth radio ON and CONNECTABLE
//          BTH_DISCOVERABLE to turn the bluetooth radio ON and CONNECTABLE and DISCOVERABLE
//
// @rdesc   int: ERROR_SUCCESS on success.  Error code describing error on failure.
//
////////////////////////////////////////////////////////////////////////////////

int BthSetMode( DWORD dwMode );

////////////////////////////////////////////////////////////////////////////////
//
// @func    int | BthGetMode |
//          This function retrieves the current mode of operation of the bluetooth radio
//
// @syntax  BthGetMode( DWORD* pdwMode )
//
// @parm    DWORD* | pdwMode |
//          BTH_POWER_OFF to indicate the bluetooth radio is OFF
//          BTH_CONNECTABLE to indicate the bluetooth radio ON and CONNECTABLE
//          BTH_DISCOVERABLE to indicate the bluetooth radio ON and CONNECTABLE and DISCOVERABLE
//
// @rdesc   int: ERROR_SUCCESS on success.  Error code describing error on failure.
//
////////////////////////////////////////////////////////////////////////////////

int BthGetMode( DWORD* pdwMode );

#ifdef __cplusplus
}
#endif


