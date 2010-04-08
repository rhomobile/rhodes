#include "../stdafx.h"

#include "getdeviceuniqueid.h"

#define DEVICE_ID_LENGTH 20
#define APPLICATION_DATA "@^!MyAppName!^@"
#define APPLICATION_DATA_LENGTH 15

void getDeviceUniqueID(char deviceId[])
{
    HRESULT hr = NOERROR;
    BYTE rgDeviceId[DEVICE_ID_LENGTH];
    DWORD cbDeviceId = sizeof(deviceId);
    hr = GetDeviceUniqueID(reinterpret_cast<PBYTE>(APPLICATION_DATA),
       APPLICATION_DATA_LENGTH,
       GETDEVICEUNIQUEID_V1,
       rgDeviceId,
       &cbDeviceId);

    memcpy(deviceId, rgDeviceId, sizeof(deviceId));
}

// End of file
