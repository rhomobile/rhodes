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

#ifndef _GETDEVICEUNIQUEID_INC
#define _GETDEVICEUNIQUEID_INC


#ifdef __cplusplus
extern "C" {
#endif


#define GETDEVICEUNIQUEID_V1                1
#define GETDEVICEUNIQUEID_V1_MIN_APPDATA    8
#define GETDEVICEUNIQUEID_V1_OUTPUT         20


HRESULT
GetDeviceUniqueID(LPBYTE pbApplicationData, 
                  DWORD cbApplicationData, 
                  DWORD dwDeviceIDVersion,
                  LPBYTE pbDeviceIDOutput, 
                  DWORD *pcbDeviceIDOutput);


// GETDEVICEUNIQUEDEVICEID_V1               - constant which is passed as the dwDeviceIdVersion
// GETDEVICEUNIQUEDEVICEID_V1_MIN_APPDATA   - minimum length for pbApplicationData
// GETDEVICEUNIQUEDEVICEID_V1_OUTPUT        - minimum length for pbDeviceIDOutput, with no truncation
//
// This API returns an application specific hash of the device ID which is usable by an application for uniquely identifying 
// the device.  This is callable by untrusted processes.  
//
// The purpose of this API is to protect the privacy of the device.  Multiple applications need to use a device unique ID
// for communicating with servers, and from a privacy perspective, it is beneficial if those servers can not correlate data
// from the same device.  The API does not use any other information than the provided application data, so any one who passes
// in the same application data buffer will get the same hash.  The goal of this API is to make it easy for applications to 
// do the right thing, not to enforce security.  A "rogue" application which learns the application data that your application
// passes to this API can generate the same hash.  This is by design.
//
//      LPBYTE      pbApplicationData       - Application provided Salt data which is hashed with the DeviceID
//                                            As long as the same data is provided as this input, the same hash
//                                            will always be returned on the same device, even if the software is
//                                            reflashed and/or the device is cold booted.  This application data must
//                                            have a minimum length of 8 bytes and should be unique to the application.
//                                            If pbApplicationData is NULL, E_POINTER is returned.
//
//      DWORD       cbApplicationData       - Length of the application data provided above.  This length must be at least
//                                            8 bytes or the API will fail with E_INVALIDARG.
//      
//      DWORD       dwDeviceIDVersion       - This is the version number of the algorithm used for calculating 
//                                            the device id returned from this API.  Currently, the only defined 
//                                            version number is 1, and this parameter must have that value, or else
//                                            E_INVALIDARG will be returned.
//
//      LPBYTE      pbDeviceIDOutput        - Application provided output buffer for the device ID.  This buffer should be
//                                            at least GETDEVICEUNIQUEID_V1_OUTPUT bytes.  If the provided buffer is smaller
//                                            than the complete hash, the hash will be truncated and only the bytes that fit
//                                            will be copied into the output buffer.  If this pointer is NULL, the API returns
//                                            the maximum output hash length in pcbDeviceIDOutput.
//
//      DWORD *     pcbDeviceIDOutput       - In/Out paramater for the device ID length.  The input parameter is the length
//                                            of the application supplied buffer.  The output parameter is the number of bytes
//                                            written into the output buffer.  If this pointer is NULL, the API will fail with
//                                            E_POINTER.
//
// There are several possible combinations for pbDeviceIDOutput and pcbDeviceIDOutput, depending on how the application is
// using the method:
//
//      (determine size of required output buffer)
//      pbDeviceIDOutput is NULL
//      pcbDeviceIDOutput is non-NULL
//      *pcbDeviceIDOutput is (ignored)     - this returns S_OK, and places the required buffer size in *pcbDeviceIDOutput.
//
//      (output buffer is too small)
//      pbDeviceIDOutput is non-NULL
//      pcbDeviceIDOutput is non-NULL
//      *pcbDeviceIDOutput is too small     - this returns HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER)), fills pbDeviceIDOutput with
//                                            as much data as will fit, and places the amount copied into *pcbDeviceIDOutput
//
//      (output buffer is large enough)
//      pbDeviceIDOutput is non-NULL
//      pcbDeviceIDOutput is non-NULL
//      *pcbDeviceIDOutput is large enough  - this returns S_OK, fills pbDeviceIDOutput with the output data, and places the amount
//                                            copied into *pcbDeviceIDOutput
//
// Sample call:
//
//  #define DEVICE_ID_LENGTH            20
//  #define APPLICATION_DATA            "@^!MyAppName!^@"
//  #define APPLICATION_DATA_LENGTH     15
//
//    HRESULT         hr              = NOERROR;
//    BYTE            rgDeviceId[DEVICE_ID_LENGTH];
//    DWORD           cbDeviceId      = sizeof(rgDeviceId);
//
//    hr = GetDeviceUniqueID(reinterpret_cast<PBYTE>(APPLICATION_DATA),
//                           APPLICATION_DATA_LENGTH,
//                           GETDEVICEUNIQUEID_V1,
//                           rgDeviceId,
//                           &cbDeviceId);
//    CHR(hr);
//
//    hr = DoSomethingWithDeviceId(rgDeviceId, cbDeviceId);
//        .
//        .
//        .


#ifdef __cplusplus
}
#endif


#endif
