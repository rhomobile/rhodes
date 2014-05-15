#include "SensorApiDll.h"

namespace sensormodule
{

// Call back function pointer type definitions
typedef DWORD (WINAPI *LPFN_SENSOR_OPEN)(OPEN_MODE eMode, SENSOR_INFO_T * ptSensorInfo, HANDLE * phSensor);
typedef DWORD (WINAPI *LPFN_SENSOR_CLOSE)(HANDLE hSensor);
typedef DWORD (WINAPI *LPFN_SENSOR_FINDFIRST)(SENSOR_INFO_T * ptSensorInfo, HANDLE * phFindHandle); 
typedef DWORD (WINAPI *LPFN_SENSOR_FINDNEXT)(HANDLE hFindHandle, SENSOR_INFO_T * ptSensorInfo); 
typedef DWORD (WINAPI *LPFN_SENSOR_FINDCLOSE)(HANDLE hFindHandle);
typedef DWORD (WINAPI *LPFN_SENSOR_REGISTERDATANOTIFICATION)(HANDLE hSensor, NOTIFIER_TYPE eNotifierType, LPVOID lpNotifier, DWORD dwNotifyThreshold);
typedef DWORD (WINAPI *LPFN_SENSOR_DEREGISTERDATANOTIFICATION)(HANDLE hSensor);
typedef DWORD (WINAPI *LPFN_SENSOR_STARTSAMPLING)(HANDLE hSensor);
typedef DWORD (WINAPI *LPFN_SENSOR_STOPSAMPLING)(HANDLE hSensor);
typedef DWORD (WINAPI *LPFN_SENSOR_GETPROPERTY)(HANDLE hSensor, SENSOR_PROPERTY eProperty, LONG * pnValue);
typedef DWORD (WINAPI *LPFN_SENSOR_SETPROPERTY)(HANDLE hSensor, SENSOR_PROPERTY eProperty, LONG nValue);

CSensorApiDll::CSensorApiDll(void) :
        m_hSensorLibrary(NULL),
        m_hSensor(NULL)
{
    this->LoadModule();
}

CSensorApiDll::~CSensorApiDll(void)
{
    this->UnloadModule();
}


bool CSensorApiDll::SensorAPIsPresent() const
{
    if (NULL == m_hSensorLibrary) return false;

    // validate if the api's are present
    LPFN_SENSOR_OPEN						lpfnSENSOR_Open;
    LPFN_SENSOR_CLOSE						lpfnSENSOR_Close;
    LPFN_SENSOR_FINDFIRST					lpfnSENSOR_FindFirst;
    LPFN_SENSOR_FINDNEXT					lpfnSENSOR_FindNext;
    LPFN_SENSOR_FINDCLOSE					lpfnSENSOR_FindClose;
    LPFN_SENSOR_REGISTERDATANOTIFICATION	lpfnSENSOR_RegisterDataNotification;
    LPFN_SENSOR_DEREGISTERDATANOTIFICATION	lpfnSENSOR_DeregisterDataNotification;
    LPFN_SENSOR_STARTSAMPLING				lpfnSENSOR_StartSampling;
    LPFN_SENSOR_STOPSAMPLING				lpfnSENSOR_StopSampling;
    LPFN_SENSOR_GETPROPERTY					lpfnSENSOR_GetProperty;
    LPFN_SENSOR_SETPROPERTY					lpfnSENSOR_SetProperty;

    // Get function pointers from Sensor API library
    lpfnSENSOR_Open							= (LPFN_SENSOR_OPEN)GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_Open"));
    lpfnSENSOR_Close						= (LPFN_SENSOR_CLOSE)GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_Close"));
    lpfnSENSOR_FindFirst					= (LPFN_SENSOR_FINDFIRST)GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_FindFirst"));
    lpfnSENSOR_FindNext						= (LPFN_SENSOR_FINDNEXT)GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_FindNext"));
    lpfnSENSOR_FindClose					= (LPFN_SENSOR_FINDCLOSE)GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_FindClose"));
    lpfnSENSOR_RegisterDataNotification		= (LPFN_SENSOR_REGISTERDATANOTIFICATION)GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_RegisterDataNotification"));
    lpfnSENSOR_DeregisterDataNotification	= (LPFN_SENSOR_DEREGISTERDATANOTIFICATION)GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_DeregisterDataNotification"));
    lpfnSENSOR_StartSampling				= (LPFN_SENSOR_STARTSAMPLING)GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_StartSampling"));
    lpfnSENSOR_StopSampling					= (LPFN_SENSOR_STOPSAMPLING)GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_StopSampling"));
    lpfnSENSOR_GetProperty					= (LPFN_SENSOR_GETPROPERTY)GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_GetProperty"));
    lpfnSENSOR_SetProperty					= (LPFN_SENSOR_SETPROPERTY)GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_SetProperty"));

    if ((NULL == lpfnSENSOR_Open) ||
        (NULL == lpfnSENSOR_Close) ||
        (NULL == lpfnSENSOR_FindFirst) ||
        (NULL == lpfnSENSOR_FindNext) ||
        (NULL == lpfnSENSOR_FindClose) ||
        (NULL == lpfnSENSOR_RegisterDataNotification) ||
        (NULL == lpfnSENSOR_DeregisterDataNotification) ||
        (NULL == lpfnSENSOR_StartSampling) ||
        (NULL == lpfnSENSOR_StopSampling) ||
        (NULL == lpfnSENSOR_GetProperty) ||
        (NULL == lpfnSENSOR_SetProperty))
    {
        return false;
    }

    return true;
}

CSensorApiDll::operator HANDLE() const
{
    return m_hSensor;
}

DWORD CSensorApiDll::SensorOpen(OPEN_MODE eMode, SENSOR_INFO_T& sensorInfo)
{
    DWORD dwResult = ERROR_SUCCESS;

    if (NULL == m_hSensorLibrary)
    {
        dwResult = ERROR_INVALID_HANDLE;
        return dwResult;
    }

    if (NULL != m_hSensor)
    {
        dwResult = ERROR_ALREADY_EXISTS;
        return dwResult;
    }

    LPFN_SENSOR_OPEN lpfnSENSOR_Open = reinterpret_cast<LPFN_SENSOR_OPEN>(GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_Open")));
    if (NULL == lpfnSENSOR_Open)
    {
        dwResult = ::GetLastError();
        return dwResult;
    }

    dwResult = lpfnSENSOR_Open(eMode, &sensorInfo, &m_hSensor);

    return dwResult;

}

DWORD CSensorApiDll::SensorClose()
{
    DWORD dwResult = ERROR_SUCCESS;

    if ((NULL == m_hSensorLibrary) || (NULL == m_hSensor))
    {
        dwResult = ERROR_INVALID_HANDLE;
        return dwResult;
    }    

    LPFN_SENSOR_CLOSE lpfnSENSOR_Close = reinterpret_cast<LPFN_SENSOR_CLOSE>(GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_Close")));
    if (NULL == lpfnSENSOR_Close)
    {
        dwResult = ::GetLastError();
        return dwResult;
    }

    dwResult = lpfnSENSOR_Close(m_hSensor);
    if (E_SENSOR_SUCCESS == dwResult)
    {
        m_hSensor = NULL;
    }
    return dwResult;
}

DWORD CSensorApiDll::SensorRegisterDataNotification(NOTIFIER_TYPE eNotifierType, LPVOID lpNotifier, DWORD dwNotifyThreshold)
{
    DWORD dwResult = ERROR_SUCCESS;

    if ((NULL == m_hSensorLibrary) || (NULL == m_hSensor))
    {
        dwResult = ERROR_INVALID_HANDLE;
        return dwResult;
    }    

    LPFN_SENSOR_REGISTERDATANOTIFICATION lpfnSENSOR_RegisterDataNotification = reinterpret_cast<LPFN_SENSOR_REGISTERDATANOTIFICATION>(::GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_RegisterDataNotification")));
    if (NULL == lpfnSENSOR_RegisterDataNotification)
    {
        dwResult = ::GetLastError();
        return dwResult;
    }

    dwResult = lpfnSENSOR_RegisterDataNotification(m_hSensor, eNotifierType, lpNotifier, dwNotifyThreshold);
    return dwResult;
}

DWORD CSensorApiDll::SensorDeregisterDataNotification()
{
    DWORD dwResult = ERROR_SUCCESS;

    if ((NULL == m_hSensorLibrary) || (NULL == m_hSensor))
    {
        dwResult = ERROR_INVALID_HANDLE;
        return dwResult;
    }    

    LPFN_SENSOR_DEREGISTERDATANOTIFICATION lpfnSENSOR_DeregisterDataNotification = reinterpret_cast<LPFN_SENSOR_DEREGISTERDATANOTIFICATION>(GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_DeregisterDataNotification")));
    if (NULL == lpfnSENSOR_DeregisterDataNotification)
    {
        dwResult = ::GetLastError();
        return dwResult;
    }

    dwResult = lpfnSENSOR_DeregisterDataNotification(m_hSensor);
    return dwResult;
}

DWORD CSensorApiDll::SensorStartSampling()
{
    DWORD dwResult = ERROR_SUCCESS;

    if ((NULL == m_hSensorLibrary) || (NULL == m_hSensor))
    {
        dwResult = ERROR_INVALID_HANDLE;
        return dwResult;
    }    

    LPFN_SENSOR_STARTSAMPLING lpfnSENSOR_StartSampling = reinterpret_cast<LPFN_SENSOR_STARTSAMPLING>(GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_StartSampling")));
    if (NULL == lpfnSENSOR_StartSampling)
    {
        dwResult = ::GetLastError();
        return dwResult;
    }

    dwResult = lpfnSENSOR_StartSampling(m_hSensor);
    return dwResult;
}

DWORD CSensorApiDll::SensorStopSampling()
{
    DWORD dwResult = ERROR_SUCCESS;

    if ((NULL == m_hSensorLibrary) || (NULL == m_hSensor))
    {
        dwResult = ERROR_INVALID_HANDLE;
        return dwResult;
    }    

    LPFN_SENSOR_STOPSAMPLING lpfnSENSOR_StopSampling = reinterpret_cast<LPFN_SENSOR_STOPSAMPLING>(GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_StopSampling")));
    if (NULL == lpfnSENSOR_StopSampling)
    {
        dwResult = ::GetLastError();
        return dwResult;
    }

    dwResult = lpfnSENSOR_StopSampling(m_hSensor);
    return dwResult;
}

DWORD CSensorApiDll::SensorGetProperty(SENSOR_PROPERTY eProperty, LONG &nValue)
{
    DWORD dwResult = ERROR_SUCCESS;

    if ((NULL == m_hSensorLibrary) || (NULL == m_hSensor))
    {
        dwResult = ERROR_INVALID_HANDLE;
        return dwResult;
    }    

    LPFN_SENSOR_GETPROPERTY	lpfnSENSOR_GetProperty = reinterpret_cast<LPFN_SENSOR_GETPROPERTY>(GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_GetProperty")));
    if (NULL == lpfnSENSOR_GetProperty)
    {
        dwResult = ::GetLastError();
        return dwResult;
    }

    dwResult = lpfnSENSOR_GetProperty(m_hSensor, eProperty, &nValue);
    return dwResult;
}

DWORD CSensorApiDll::SensorSetProperty(SENSOR_PROPERTY eProperty, LONG  nValue)
{
    DWORD dwResult = ERROR_SUCCESS;

    if ((NULL == m_hSensorLibrary) || (NULL == m_hSensor))
    {
        dwResult = ERROR_INVALID_HANDLE;
        return dwResult;
    }    

    LPFN_SENSOR_SETPROPERTY lpfnSENSOR_SetProperty = reinterpret_cast<LPFN_SENSOR_SETPROPERTY>(GetProcAddress(m_hSensorLibrary, TEXT("SENSOR_SetProperty")));
    if (NULL == lpfnSENSOR_SetProperty)
    {
        dwResult = ::GetLastError();
        return dwResult;
    }

    dwResult = lpfnSENSOR_SetProperty(m_hSensor, eProperty, nValue);
    return dwResult;
}

DWORD CSensorApiDll::Enumerate(rho::Vector<SENSOR_TYPE>& sensorList)
{
    DWORD dwResult = ERROR_SUCCESS;

    HMODULE hSensorLibrary = ::LoadLibrary(L"SensorApi.dll");
    if (NULL == hSensorLibrary)
    {
        dwResult = ERROR_INVALID_HANDLE;
        return dwResult;
    }

    LPFN_SENSOR_FINDFIRST	lpfnSENSOR_FindFirst	= reinterpret_cast<LPFN_SENSOR_FINDFIRST>(GetProcAddress(hSensorLibrary, TEXT("SENSOR_FindFirst")));
    LPFN_SENSOR_FINDNEXT	lpfnSENSOR_FindNext		= reinterpret_cast<LPFN_SENSOR_FINDNEXT>(GetProcAddress(hSensorLibrary, TEXT("SENSOR_FindNext")));
    LPFN_SENSOR_FINDCLOSE	lpfnSENSOR_FindClose	= reinterpret_cast<LPFN_SENSOR_FINDCLOSE>(GetProcAddress(hSensorLibrary, TEXT("SENSOR_FindClose")));

    if ((NULL == lpfnSENSOR_FindFirst) || (NULL == lpfnSENSOR_FindNext) || (NULL == lpfnSENSOR_FindClose))
    {
        dwResult = ERROR_PROC_NOT_FOUND;        
    }
    else
    {
        HANDLE hFindHandle = NULL;
        SENSOR_INFO_T tSensorInfo;

        //Initialize the structure to all zeros.
        ::ZeroMemory(&tSensorInfo, sizeof(SENSOR_INFO_T));
        // Sets dwAllocated to the size of the structure.
        SI_INIT(&tSensorInfo);
        // Enumerating all available sensors.
        // It can also be set to a required sensor in which case only FindFirst needs to be called.
        tSensorInfo.eType = SENSOR_TYPE_ALL;
        //Sets dwUsed to the extent of eType.
        SI_SET_USED(&tSensorInfo, eType)

        dwResult = lpfnSENSOR_FindFirst(&tSensorInfo, &hFindHandle);
        while (E_SENSOR_SUCCESS == dwResult)
        {
            switch (tSensorInfo.eType)
            {
            case SENSOR_TYPE_ACCELEROMETER:
            case SENSOR_TYPE_ORIENTATION:            
            case SENSOR_TYPE_TILT_ANGLE:            
            case SENSOR_TYPE_MOTION:            
            case SENSOR_TYPE_ECOMPASS:            
            case SENSOR_TYPE_MAGNETOMETER:            
            case SENSOR_TYPE_GYROSCOPE:            
            case SENSOR_TYPE_AMBIENT_LIGHT:            
            case SENSOR_TYPE_PROXIMITY:            
            case SENSOR_TYPE_PROXIMITY_LONG_RANGE:
            case SENSOR_TYPE_PRESSURE:            
            case SENSOR_TYPE_TEMPERATURE:            
            case SENSOR_TYPE_HUMIDITY:
                sensorList.addElement(tSensorInfo.eType);
                break;
            default:
                break;
            }
            dwResult = lpfnSENSOR_FindNext(hFindHandle, &tSensorInfo);
        }

        dwResult = lpfnSENSOR_FindClose(hFindHandle);
    }
    if (NULL != hSensorLibrary)
    {
        ::FreeLibrary(hSensorLibrary);
        hSensorLibrary = NULL;
    }

    return dwResult;
}

void CSensorApiDll::ReadyForOpen(SENSOR_TYPE sensorType, SENSOR_INFO_T& sensorInfo)
{
    memset(&sensorInfo, 0,sizeof(SENSOR_INFO_T));
	SI_INIT(&sensorInfo);// sets the dwallocated field of the structure pointed to &tsensorinfo to the size of the structure. 
	sensorInfo.eType = sensorType; // modify the field (etype)
	SI_SET_USED(&sensorInfo, eType) //Sets the dwUsed field of the structure pointed to &tSensorInfo to the extent of field (eType).
}

DWORD CSensorApiDll::LoadModule()
{
    DWORD dwResult = ERROR_SUCCESS;
    // load the dll
    m_hSensorLibrary = ::LoadLibrary(L"SensorApi.dll");
    if (NULL == m_hSensorLibrary)
    {
        dwResult = ::GetLastError();
        return dwResult;
    }

    // validate if the api's are present
    if (false == this->SensorAPIsPresent())
    {
        UnloadModule();
        dwResult = ERROR_PROC_NOT_FOUND;
        return dwResult;
    }
    return dwResult;
}

DWORD CSensorApiDll::UnloadModule()
{
    DWORD dwResult = ERROR_SUCCESS;

    //TODO:: unregister any subscribed events and sensor handle

    //free the sensor module
    if (NULL != m_hSensorLibrary)
    {
        if (FALSE != ::FreeLibrary(m_hSensorLibrary))        
            m_hSensorLibrary = NULL;
        else
            dwResult = ::GetLastError();
    }
    
    return dwResult;
}

}