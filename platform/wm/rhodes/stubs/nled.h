//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*++



File Name:	nled.h

Abstract:   Notification LED interface.

Notes:

The notification LED is distinguished from other LED's which may be on the system
in that it can be on or blinking even if the rest of the system is powered down.  This
implies a certain level of hardware support for this functionality.

--*/
#ifndef __NLED_H__
#define __NLED_H__

#ifdef __cplusplus
extern "C" {
#endif

// @CESYSGEN IF COREDLL_NLED

//
// NLedDriverGetDeviceInfo query definitions
//

/*		@DOC	EXTERNAL DRIVERS
		@const	ULONG | NLED_COUNT_INFO_ID |

Id for <f NLedDriverGetDeviceInfo> to get count of notification LED's

		@XREF

			<tab><f NLedDriverGetDeviceInfo><nl>
			<tab><t NLDI_COUNT_INFO><nl>

*/
#define NLED_COUNT_INFO_ID	0


/*      @DOC    EXTERNAL DRIVERS
		@STRUCT	NLED_COUNT_NFO |

Info about number of notification LED's.

		@XREF

			<tab><f NLedDriverGetDeviceInfo><nl>
			<tab><c NLED_COUNT_INFO_ID><nl>

		@COMM

A device will usually have one notification LED.  Some devices may have 
none however.  It would be unusual to have more than one because of the 
power drain.  


*/
struct NLED_COUNT_INFO
	{
	UINT	cLeds;				// @FIELD 	Count of LED's
	};




/*		@DOC	EXTERNAL DRIVERS
		@const	ULONG | NLED_SUPPORTS_INFO_ID |

Id for <f NLedDriverGetDeviceInfo> to get supported capabilities of a 
notification LED.  


		@XREF

			<tab><f NLedDriverGetDeviceInfo><nl>
			<tab><t NLDI_SUPPORTS_INFO><nl>

*/
#define NLED_SUPPORTS_INFO_ID	1


/*      @DOC    EXTERNAL DRIVERS
		@STRUCT	NLED_SUPPORTS_NFO |

Info about what a notification LED supports.

		@XREF

			<tab><f NLedDriverGetDeviceInfo><nl>
			<tab><c NLED_SUPPORTS_INFO_ID><nl>

		@COMM

Caller should first get the number of notification LED's on the system.  
This is usually one but may be 0 and could be greater than 1.  Caller 
should fill in the LedNum and then call <f NLedDriverGetDeviceInfo>.  Led 
numbering starts at 0.

The lCycleAdjust field is the granularity to which cycle time adjustments 
can be made.  E.g., if the granularity is 1/16 second, lCycleAdjust = 
62500 microseconds.  If the LED does not support blinking, this value 
should be 0.

Values are given in microseconds only to deal with computations involving 
fractional milliseconds.  Usually only settings in the milliseconds range 
are meaningful.  

Usually a device will report that it supports up to two of:

	fAdjustTotalCycleTime
	fAdjustOnTime
	fAdjustOffTime

since any two determine the third.  The most likely settings are:

	1. The cycle time is not adjustable:

		fAdjustTotalCycleTime == FALSE
		fAdjustOnTime == FALSE
		fAdjustOffTime == FALSE

	2. Only the overall cycle time is adjustable:

		fAdjustTotalCycleTime == TRUE
		fAdjustOnTime == FALSE
		fAdjustOffTime == FALSE

	3. The on and off times are independently adjustable.

		fAdjustTotalCycleTime == FALSE
		fAdjustOnTime == TRUE
		fAdjustOffTime == TRUE


*/

struct NLED_SUPPORTS_INFO
	{
	UINT	LedNum;						// @FIELD 	LED number, 0 is first LED
	LONG	lCycleAdjust;				// @FIELD	Granularity of cycle time adjustments (microseconds)
	BOOL	fAdjustTotalCycleTime;		// @FIELD	LED has an adjustable total cycle time
	BOOL	fAdjustOnTime;				// @FIELD	LED has separate on time
	BOOL	fAdjustOffTime;				// @FIELD	LED has separate off time
	BOOL	fMetaCycleOn;				// @FIELD	LED can do blink n, pause, blink n, ...
	BOOL	fMetaCycleOff;				// @FIELD	LED can do blink n, pause n, blink n, ...
	};





/*		@DOC	EXTERNAL DRIVERS
		@const	ULONG | NLED_SETTINGS_INFO_ID |

Id for <f NLedDriverGetDeviceInfo> to get current settings of a 
notification LED.  


		@XREF

			<tab><f NLedDriverGetDeviceInfo><nl>
			<tab><t NLDI_SETTINGS_INFO><nl>

*/
#define NLED_SETTINGS_INFO_ID	2


/*      @DOC    EXTERNAL DRIVERS
		@STRUCT	NLED_SETTINGS_NFO |

Info about the current settings of a notification LED.

		@XREF

			<tab><f NLedDriverGetDeviceInfo><nl>
			<tab><c NLED_SETTINGS_INFO_ID><nl>

		@COMM

Caller should first get the number of notification LED's on the system.  
This is usually one but may be 0 and could be greater than 1.  Caller 
should fill in the LedNum and then call <f NLedDriverGetDeviceInfo>.  Led 
numbering starts at 0.

Note that the hardware may have some minimum on or off time, so setting the OnTime or OffTime
fields to 0 may still result in a non-zero on or off time.

*/

struct NLED_SETTINGS_INFO
    {
    UINT    LedNum;                 // @FIELD   LED number, 0 is first LED
    INT     OffOnBlink;             // @FIELD   0 == off, 1 == on, 2 == blink
    LONG    TotalCycleTime;         // @FIELD   total cycle time of a blink in microseconds
    LONG    OnTime;                 // @FIELD   on time of a cycle in microseconds
    LONG    OffTime;                // @FIELD   off time of a cycle in microseconds
    INT     MetaCycleOn;            // @FIELD   number of on blink cycles
    INT     MetaCycleOff;           // @FIELD   number of off blink cycles
    };

// NLED driver IOCTL codes
#define IOCTL_NLED_GETDEVICEINFO            \
    CTL_CODE(FILE_DEVICE_NLED, 0x100, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_NLED_SETDEVICE                \
    CTL_CODE(FILE_DEVICE_NLED, 0x101, METHOD_BUFFERED, FILE_ANY_ACCESS)
    
// Battery driver interface GUID (used with AdvertiseInterface())
#define NLED_DRIVER_CLASS        _T("{CBB4F234-F35F-485b-A490-ADC7804A4EF3}")

// API event name for the NLED interface.  If calling OpenEvent() on this name returns a valid
// handle, the NLED APIs exist on the platform.  The handle will be signaled when the APIs become
// ready.
#define NLED_API_EVENT_NAME     _T("SYSTEM/NLedAPIsReady")

typedef BOOL (WINAPI *PFN_NLED_SET_DEVICE)( UINT	nDeviceId, void	*pInput );
typedef BOOL (WINAPI *PFN_NLED_GET_DEVICE_INFO)( UINT	nInfoId, void	*pOutput );

BOOL WINAPI NLedGetDeviceInfo( UINT	nInfoId, void	*pOutput );
BOOL WINAPI NLedSetDevice( UINT	nDeviceId, void	*pInput );

// @CESYSGEN ENDIF COREDLL_NLED

#ifdef __cplusplus
}
#endif



#endif

