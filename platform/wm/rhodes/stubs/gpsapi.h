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
// ********************************************************
// GPS Intermediate Driver API
// ********************************************************

#ifndef _GPSAPI_H_
#define _GPSAPI_H_

#define GPS_MAX_SATELLITES      12
#define GPS_MAX_PREFIX_NAME     16
#define GPS_MAX_FRIENDLY_NAME   64

#define GPS_VERSION_1           1
#define GPS_VERSION_2           2
#define GPS_VERSION_CURRENT     GPS_VERSION_2

//
// Poll Version 1 drivers can handle some fields of GPS_POSITION structure
// Poll Version 2 drivers can handle all fields of GPS_POSITION structure
//
#define POLL_VERSION_1          1
#define POLL_VERSION_2          2

typedef enum {
    GPS_FIX_UNKNOWN = 0,
    GPS_FIX_2D,
    GPS_FIX_3D
}
GPS_FIX_TYPE;

typedef enum {
    GPS_FIX_SELECTION_UNKNOWN = 0,
    GPS_FIX_SELECTION_AUTO,
    GPS_FIX_SELECTION_MANUAL
}
GPS_FIX_SELECTION;

typedef enum {
    GPS_FIX_QUALITY_UNKNOWN = 0,
    GPS_FIX_QUALITY_GPS,
    GPS_FIX_QUALITY_DGPS
}
GPS_FIX_QUALITY;


//
// The following define the fix modes known to the operating system.
// The OEMs are free to define their own GPS_FIX_MODE starting with values
// greater than or equal to GPS_FIX_MODE_CUSTOM.
//
typedef enum {
    GPS_FIX_MODE_UNKNOWN = 0,
    GPS_FIX_MODE_MSA,
    GPS_FIX_MODE_MSB,        
    GPS_FIX_MODE_MSS,    
    
    // This must be the last entry for OS-supplied GPS_FIX_MODE_XXX values.
    // It's not an actual fix mode value - It's only a place holder
    // to keep count of values in the enum.
    GPS_FIX_MODE_COUNT
} 
GPS_FIX_MODE;

//
// This is the value which defines the start of the range where OEMs are 
// permitted to use their own fix mode values.
//
#define GPS_FIX_MODE_CUSTOM 256


//
// This structure contains the parameters to start the GPS device and trigger 
// a GPS fix operation. Pointer to this struct is used as pbParamData field in 
// calls to GPSSetDeviceParam function when dwParamName = GPS_START_FIX
//
typedef struct _GPS_START_FIX_CONFIG {
    DWORD dwFlags;
} GPS_START_FIX_CONFIG, *PGPS_START_FIX_CONFIG;



#define GPS_START_FIX          0
#define GPS_STOP_FIX           1
#define GPS_CONFIG_FIX_MODE    2
#define GPS_CONFIG_FIX_RATE    3
#define GPS_CONFIG_QOS         4
#define GPS_QUERY_FIX          5
#define GPS_CONFIG_COUNT       6 // Count of built-in GPS_PARAM values
#define GPS_CONFIG_CUSTOM    256 // This is the value which defines the start of 
                                 // the range where OEMs are permitted to use 
                                 // their own parameter values for use in 
                                 // GPSGetDeviceParam and GPSSetDeviceParam.

//
// This enumeration contains values that specify the current GPS Hardware state.
//
typedef enum {
    GPS_HW_STATE_UNKNOWN = 0,
    GPS_HW_STATE_ON,
    GPS_HW_STATE_IDLE
} GPS_HW_STATE;


//
// GPS_VALID_XXX bit flags in GPS_POSITION structure are valid.
//
#define GPS_VALID_UTC_TIME                                 0x00000001
#define GPS_VALID_LATITUDE                                 0x00000002
#define GPS_VALID_LONGITUDE                                0x00000004
#define GPS_VALID_SPEED                                    0x00000008
#define GPS_VALID_HEADING                                  0x00000010
#define GPS_VALID_MAGNETIC_VARIATION                       0x00000020
#define GPS_VALID_ALTITUDE_WRT_SEA_LEVEL                   0x00000040
#define GPS_VALID_ALTITUDE_WRT_ELLIPSOID                   0x00000080
#define GPS_VALID_POSITION_DILUTION_OF_PRECISION           0x00000100
#define GPS_VALID_HORIZONTAL_DILUTION_OF_PRECISION         0x00000200
#define GPS_VALID_VERTICAL_DILUTION_OF_PRECISION           0x00000400
#define GPS_VALID_SATELLITE_COUNT                          0x00000800
#define GPS_VALID_SATELLITES_USED_PRNS                     0x00001000
#define GPS_VALID_SATELLITES_IN_VIEW                       0x00002000
#define GPS_VALID_SATELLITES_IN_VIEW_PRNS                  0x00004000
#define GPS_VALID_SATELLITES_IN_VIEW_ELEVATION             0x00008000
#define GPS_VALID_SATELLITES_IN_VIEW_AZIMUTH               0x00010000
#define GPS_VALID_SATELLITES_IN_VIEW_SIGNAL_TO_NOISE_RATIO 0x00020000
#define GPS_VALID_POSITION_UNCERTAINTY_ERROR               0x00040000
#define GPS_VALID_FIX_MODE                                 0x00080000
#define GPS_VALID_FIX_ERROR                                0x00100000


//
// GPS_DEV_VALID_XXX bit flags in GPS_DEVICE_STATUS structure are valid. 
//
#define GPS_DEV_VALID_HW_STATE                             0x00000001 
#define GPS_DEV_VALID_EPH_SV_MASK                          0x00000002
#define GPS_DEV_VALID_ALM_SV_MASK                          0x00000004 
#define GPS_DEV_VALID_SAT_IN_VIEW_PRN                      0x00000008
#define GPS_DEV_VALID_SAT_IN_VIEW_CARRIER_TO_NOISE_RATIO   0x00000010
#define GPS_DEV_VALID_DEV_ERROR                            0x00000020


//
// GPS_DATA_FLAGS_XXX bit flags set in GPS_POSITION dwFlags field
// provide additional information about the state of the query.
// 

// Set when GPS hardware is not connected to GPSID and we 
// are returning cached data.
#define GPS_DATA_FLAGS_HARDWARE_OFF                        0x00000001


//
// GPSOpenDevice flags
//
#define GPS_OPEN_NO_HARDWARE_INIT                          0x00000001


//
// This structure contains estimated position error information returned in GPS_POSITION
// structure with dwVersion = GPS_VERSION_2. 
//
typedef struct _GPS_POSITION_ERROR
{
    //
    // Horizontal position uncertainty in meters of axis aligned with the angle 
    // specified in dWHorizontalErrorAngle of a two-dimension horizontal error ellipse. 
    // The value dwHorizontalConfidence gives the percentage of positions expected to 
    // fall within this ellipse, e.g. dwHorizontalConfidence = 39 indicates a 1-sigma 
    // error ellipse is given.
    //
    DWORD dwHorizontalErrorAlong;

    //
    // Angle of horizontal axis, with respect to true north, for a two-dimension 
    // horizontal error ellipse.
    //
    DWORD dwHorizontalErrorAngle;

    //
    // Horizontal position uncertainty in meters of axis perpendicular to angle 
    // specified in dwHorizontalErrorAngle of a two-dimension horizontal error ellipse. 
    // The value dwHorizontalConfidence gives the percentage of positions expected to 
    // fall within this ellipse, e.g. dwHorizontalConfidence = 39 indicates a 1-sigma 
    // error ellipse is given.
    //
    DWORD dwHorizontalErrorPerp;

    //
    // Vertical position uncertainty in meters with 1 sigma uncertainty. 
    // This value is always given as the 1-sigma estimate of vertical position error. 
    // It is not adjusted based on the value of  dwHorizontalConfidence.
    //
    DWORD dwVerticalError;

    //
    // Identifies the calculated probability in percent that the position estimate 
    // resides within the two dimension horizontal error ellipse specified by the 
    // three horizontal error values above. Note that appropriate rescaling of the 
    // ellipse dimensions can be used to achieve other confidence values. The special 
    // case where dwHorizontalErrorAlong are dwHorizontalErrorPerp set to the estimated 
    // 1 dimension standard deviation values will yield a confidence value of 39%. 
    // (2.45 sigma yield 95% confidence, 3.03 x sigma yields 99% confidence).
    //
    DWORD dwHorizontalConfidence;
} GPS_POSITION_ERROR, *PGPS_POSITION_ERROR;



//
// GPS_POSITION contains our latest physical coordinates, the time, 
// and satellites used in determining these coordinates. 
// dwVersion = GPS_VERSION_1 uses the fields upto (but NOT including) PositionUncertaintyError field.
// dwVersion = GPS_VERSION_2 uses all the defined fields upto (and including) dwFixError field.
// 
typedef struct _GPS_POSITION {
    DWORD dwVersion;             // Current version of GPSID client is using.
    DWORD dwSize;                // sizeof(_GPS_POSITION)

    // Not all fields in the structure below are guaranteed to be valid.  
    // Which fields are valid depend on GPS device being used, how stale the API allows
    // the data to be, and current signal.
    // Valid fields are specified in dwValidFields, based on GPS_VALID_XXX flags.
    DWORD dwValidFields;

    // Additional information about this location structure (GPS_DATA_FLAGS_XXX)
    DWORD dwFlags;
    
    //** Time related
    SYSTEMTIME stUTCTime;   //  UTC according to GPS clock.
    
    //** Position + heading related
    double dblLatitude;            // Degrees latitude.  North is positive
    double dblLongitude;           // Degrees longitude.  East is positive
    float  flSpeed;                // Speed in knots
    float  flHeading;              // Degrees heading (course made good).  True North=0
    double dblMagneticVariation;   // Magnetic variation.  East is positive
    float  flAltitudeWRTSeaLevel;  // Altitute with regards to sea level, in meters
    float  flAltitudeWRTEllipsoid; // Altitude with regards to ellipsoid, in meters

    //** Quality of this fix
    GPS_FIX_QUALITY     FixQuality;        // Where did we get fix from?
    GPS_FIX_TYPE        FixType;           // Is this 2d or 3d fix?
    GPS_FIX_SELECTION   SelectionType;     // Auto or manual selection between 2d or 3d mode
    float flPositionDilutionOfPrecision;   // Position Dilution Of Precision
    float flHorizontalDilutionOfPrecision; // Horizontal Dilution Of Precision
    float flVerticalDilutionOfPrecision;   // Vertical Dilution Of Precision

    //** Satellite information
    DWORD dwSatelliteCount;                                            // Number of satellites used in solution
    DWORD rgdwSatellitesUsedPRNs[GPS_MAX_SATELLITES];                  // PRN numbers of satellites used in the solution

    DWORD dwSatellitesInView;                                          // Number of satellites in view.  From 0-GPS_MAX_SATELLITES
    DWORD rgdwSatellitesInViewPRNs[GPS_MAX_SATELLITES];                // PRN numbers of satellites in view
    DWORD rgdwSatellitesInViewElevation[GPS_MAX_SATELLITES];           // Elevation of each satellite in view
    DWORD rgdwSatellitesInViewAzimuth[GPS_MAX_SATELLITES];             // Azimuth of each satellite in view
    DWORD rgdwSatellitesInViewSignalToNoiseRatio[GPS_MAX_SATELLITES];  // Signal to noise ratio of each satellite in view

    //** GPS API Extended fields (API v2)
    GPS_POSITION_ERROR PositionUncertaintyError;                       // Estimated position uncertainty error 
    GPS_FIX_MODE       FixMode;                                        // Fix mode used by the GPS hardware to calculate the position
    DWORD              dwFixError;                                     // GPS fix session error reported by GPS hardware
} GPS_POSITION, *PGPS_POSITION;


//
// This structure contains the status information about GPS Hardware used by GPSID. 
//
typedef struct _GPS_DEVICE_STATUS {
    DWORD        dwValidFields;
    GPS_HW_STATE ghsHardwareState;
    DWORD        dwEphSVMask;
    DWORD        dwAlmSVMask;
    DWORD        rgdwSatellitesInViewPRNs[GPS_MAX_SATELLITES];
    DWORD        rgdwSatellitesInViewCarrierToNoiseRatio[GPS_MAX_SATELLITES];
    DWORD        dwDeviceError;
} GPS_DEVICE_STATUS, *PGPS_DEVICE_STATUS;


//
// GPS_DEVICE contains information about the device driver and the
// service itself and is returned on a call to GPSGetDeviceState().
// States are indicated with SERVICE_STATE_XXX flags defined in service.h
// dwVersion = GPS_VERSION_1 uses the first 8 fields, upto gdsDeviceStatus. 
// dwVersion = GPS_VERSION_2 uses the first 9 fields, including gdsDeviceStatus.
// 
typedef struct _GPS_DEVICE {
    DWORD             dwVersion;                                 // Current version of GPSID client is using.
    DWORD             dwSize;                                    // sizeof this structure
    DWORD             dwServiceState;                            // State of the GPS Intermediate Driver service.  
    DWORD             dwDeviceState;                             // Status of the actual GPS device driver.
    FILETIME          ftLastDataReceived;                        // Last time that the actual GPS device sent information to the intermediate driver.
    WCHAR             szGPSDriverPrefix[GPS_MAX_PREFIX_NAME];    // Prefix name we are using to communicate to the base GPS driver
    WCHAR             szGPSMultiplexPrefix[GPS_MAX_PREFIX_NAME]; // Prefix name that GPS Intermediate Driver Multiplexer is running on
    WCHAR             szGPSFriendlyName[GPS_MAX_FRIENDLY_NAME];  // Friendly name real GPS device we are currently using
    GPS_DEVICE_STATUS gdsDeviceStatus;                           // Information about GPS Hardware
} *PGPS_DEVICE, GPS_DEVICE;

typedef struct _GPS_FIX_RATE {
    DWORD dwNumFixes;
    DWORD dwTimeBetweenFixes;
} GPS_FIX_RATE, *PGPS_FIX_RATE;


typedef struct _GPS_QOS {
    DWORD dwAccuracy;
    DWORD dwPerformance;
} GPS_QOS, *PGPS_QOS;


#ifdef __cplusplus
extern "C" {
#endif

HANDLE GPSOpenDevice(HANDLE hNewLocationData, HANDLE hDeviceStateChange, const WCHAR *szDeviceName, DWORD dwFlags);
DWORD  GPSCloseDevice(HANDLE hGPSDevice);
DWORD  GPSGetPosition(HANDLE hGPSDevice, GPS_POSITION *pGPSPosition, DWORD dwMaximumAge, DWORD dwFlags);
DWORD  GPSGetDeviceState(GPS_DEVICE *pGPSDevice);
DWORD  GPSGetDeviceParam(HANDLE hGPSDevice, DWORD dwParamName, PBYTE pbParamData, PDWORD pcbDataLen);
DWORD  GPSSetDeviceParam(HANDLE hGPSDevice, DWORD dwParamName, PBYTE pbParamData, DWORD cbDataLen);

#ifdef __cplusplus
}
#endif

#endif // _GPSAPI_H_


