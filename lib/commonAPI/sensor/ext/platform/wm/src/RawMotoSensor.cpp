#include <windows.h>
#include "common/RhoStd.h"
#include "common/StringConverter.h"
#include "common/RhoMutexLock.h"
#include "logging/RhoLog.h"
#include "../../../shared/generated/cpp/ISensor.h"
#include "SensorEventListener.h"
#include "RawMotoSensor.h"


namespace sensormodule
{
const char* SENSOR_NAME_INVALID = "Invalid";
int const DEFAULT_TIME_GAP    = 200;

CRawMotoSensor::CRawMotoSensor(const char* sensorType) :
    m_sensorApiDll(NULL),
    m_minimumInterval(DEFAULT_TIME_GAP),
    m_status(SENSOR_STATUS_NOT_READY),
    m_type(ConvertFromTypeDescription(sensorType)),
    m_dwQuietTime(::GetTickCount()),
    m_lScale(1),
    m_bFirstSample(true)
{
    CSensorApiDll* sensorApiDll = new CSensorApiDll();
    if((NULL != sensorApiDll) && sensorApiDll->SensorAPIsPresent())
    {
        m_sensorApiDll = sensorApiDll;
    }
    else
    {
        delete sensorApiDll;
    }    
}

CRawMotoSensor::~CRawMotoSensor(void)
{
    LOG(TRACE) + "Begin Destroy ensor instance";
    CloseSensor();

    if (m_sensorApiDll)
    {
        delete m_sensorApiDll;
        m_sensorApiDll = NULL;
    }
    m_status = SENSOR_STATUS_NOT_READY;
}

bool CRawMotoSensor::SensorAPIsPresent() const
{
    return (NULL != m_sensorApiDll);    
}

int CRawMotoSensor::getMinimumGap()
{
    return m_minimumInterval;
}
void CRawMotoSensor::setMinimumGap(int minimumGap)
{
    m_minimumInterval = minimumGap;
}
const char* CRawMotoSensor::getType()
{
    return ConvertToTypeDescription(m_type);
}
const char* CRawMotoSensor::getStatus()
{
    return ConvertToStatusDescription(m_status);
}

bool CRawMotoSensor::RegisterSensor(ISensorEventListener* eventListener)
{
    bool success = true;

    

    this->AddEventListener(m_type, eventListener);
    switch (m_type)
    {
        case SENSOR_TYPE_ACCELEROMETER:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_ACCELEROMETER,*/ OnReceiveAccelerometerSensorData));
            break;
        case SENSOR_TYPE_ORIENTATION:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_ORIENTATION,*/ OnReceiveOrientationSensorData));
            break;
        case SENSOR_TYPE_TILT_ANGLE:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_TILT_ANGLE,*/ OnReceiveTiltAngleSensorData));
            break;
        case SENSOR_TYPE_MOTION:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_MOTION,*/ OnReceiveMotionSensorData));
            break;
        case SENSOR_TYPE_ECOMPASS:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_ECOMPASS,*/ OnReceiveECompassSensorData));
            break;
        case SENSOR_TYPE_MAGNETOMETER:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_MAGNETOMETER,*/ OnReceiveMagnetometerSensorData));
            break;
        case SENSOR_TYPE_GYROSCOPE:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_GYROSCOPE,*/ OnReceiveGyroscopeSensorData));
            break;
        case SENSOR_TYPE_AMBIENT_LIGHT:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_AMBIENT_LIGHT,*/ OnReceiveAmbientLightSensorData));
            break;
        case SENSOR_TYPE_PROXIMITY:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_PROXIMITY,*/ OnReceiveProximitySensorData));
            break;
        case SENSOR_TYPE_PROXIMITY_LONG_RANGE:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_PROXIMITY_LONG_RANGE,*/ OnReceiveProximityLongRangeSensorData));
            break;
        case SENSOR_TYPE_PRESSURE:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_PRESSURE,*/ OnReceivePressureSensorData));
            break;
        case SENSOR_TYPE_TEMPERATURE:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_TEMPERATURE,*/ OnReceiveTemperatureSensorData));
            break;
        case SENSOR_TYPE_HUMIDITY:
            success = (ERROR_SUCCESS == this->OpenSensor(/*SENSOR_TYPE_HUMIDITY,*/ OnReceiveHumiditySensorData));
            break;
        default:
            LOG(INFO) + "Unable to open sensor " + ConvertToTypeDescription(this->m_type) + " : " + rho::common::convertToStringA<int>((static_cast<int>(this->m_type)));
            success = false;
            break;        
    }
    /*if (success)
    {
        LOG(INFO) + "Sensor opened : " + ConvertToTypeDescription(this->m_type) + " : " + rho::common::convertToStringA<int>((static_cast<int>(this->m_type)));
        Start();
    }*/
    return success;
}


void CRawMotoSensor::EnumerateSensors(rho::Vector<rho::String>& sensorList)
{
    static rho::Vector<SENSOR_TYPE> sensorTypes;
    if (sensorTypes.empty())
    {
        CSensorApiDll::Enumerate(sensorTypes);
    }

    rho::Vector<SENSOR_TYPE>::const_iterator citr;

    for(citr = sensorTypes.begin(); citr != sensorTypes.end(); citr++)
    {    
        switch (*citr)
        {
        case SENSOR_TYPE_ACCELEROMETER:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_ACCELEROMETER);
            break;
        case SENSOR_TYPE_ORIENTATION:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_DEVICE_ORIENTATION);
            break;
        case SENSOR_TYPE_TILT_ANGLE:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_TILT_ANGLE);
            break;
        case SENSOR_TYPE_MOTION:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_MOTION);
            break;
        case SENSOR_TYPE_ECOMPASS:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_ECOMPASS);
            break;
        case SENSOR_TYPE_MAGNETOMETER:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_MAGNETOMETER);
            break;
        case SENSOR_TYPE_GYROSCOPE:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_GYROSCOPE);
            break;
        case SENSOR_TYPE_AMBIENT_LIGHT:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_AMBIENT_LIGHT);
            break;
        case SENSOR_TYPE_PROXIMITY:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_PROXIMITY);
            break;
        case SENSOR_TYPE_PROXIMITY_LONG_RANGE:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_PROXIMITY_LONG_RANGE);
            break;
        case SENSOR_TYPE_PRESSURE:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_PRESSURE);
            break;
        case SENSOR_TYPE_TEMPERATURE:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_TEMPERATURE);
            break;
        case SENSOR_TYPE_HUMIDITY:
            sensorList.addElement(rho::ISensor::SENSOR_TYPE_HUMIDITY);
            break;
        default:
            break;
        }        
    }    
}

DWORD CRawMotoSensor::OpenSensor(/*SENSOR_TYPE sensorType,*/ PFN_SENSOR_DATA_CALLBACK pfnSensorDataCallback)
{
	DWORD dwResult;
	SENSOR_INFO_T tSensorInfo;
	
    CSensorApiDll::ReadyForOpen(m_type, tSensorInfo);

	// Open the sensor in default mode. 
    dwResult = m_sensorApiDll->SensorOpen(SENSOR_OPEN_MODE_DEFAULT, tSensorInfo);
	if(E_SENSOR_SUCCESS != dwResult)
	{
        LOG(WARNING) + "Sensor failed to open : " + ConvertToTypeDescription(this->m_type) + " error : " + rho::common::convertToStringA<DWORD>(dwResult);
        m_status = SENSOR_STATUS_ERROR;
		return dwResult;
	}

	// Getting the scale factor
    //LONG& scaleFactor = g_scale.at(sensorType);
    dwResult = m_sensorApiDll->SensorGetProperty(SENSOR_PROP_SCALE, /*scaleFactor*/this->m_lScale);	
	if(E_SENSOR_SUCCESS != dwResult)
    {
        LOG(WARNING) + "Failed to read scale factor from sensor : " + ConvertToTypeDescription(this->m_type) + " error : " + rho::common::convertToStringA<DWORD>(dwResult);
        m_status = SENSOR_STATUS_ERROR;    
		return dwResult;
	}

	if (SAMPLING_TYPE_ON_RATE == tSensorInfo.eSamplingType) // Sampling type - on rate
	{
		// Setting the Sample rate to 1Hz
		LONG nSampleRate = SAMPLE_RATE_1_HZ;

        dwResult = m_sensorApiDll->SensorSetProperty(SENSOR_PROP_SAMPLE_RATE, nSampleRate);		
		if(E_SENSOR_SUCCESS != dwResult)
		{
            LOG(WARNING) + "Failed to set sampling rate to 1Hz for sensor : " + ConvertToTypeDescription(this->m_type) + " error : " + rho::common::convertToStringA<DWORD>(dwResult);
			// In case of a failure, continue with the default sample rate without returning 
		}
	}

	// Register for sensor data callback. Notify threshold should be 1 if default buffer size is used
    dwResult = m_sensorApiDll->SensorRegisterDataNotification(NOTIFIER_TYPE_CALLBACK, pfnSensorDataCallback, 1);
	if(E_SENSOR_SUCCESS != dwResult)
	{
        LOG(WARNING) + "Failed to register callback for sensor : " + ConvertToTypeDescription(this->m_type) + " error : " + rho::common::convertToStringA<DWORD>(dwResult);
        m_status = SENSOR_STATUS_ERROR;
		return (dwResult);
	}
    m_status = SENSOR_STATUS_READY;
	return (dwResult);
}

DWORD CRawMotoSensor::CloseSensor()
{
    DWORD dwResult = ERROR_SUCCESS;
    m_status = SENSOR_STATUS_NOT_READY;

    LOG(INFO) + "Closing sensor : " + ConvertToTypeDescription(this->m_type);

    this->Stop();

    if (m_sensorApiDll != NULL)
    {

        // Register for sensor data callback. Notify threshold should be 1 if default buffer size is used
        dwResult = m_sensorApiDll->SensorDeregisterDataNotification();
	    if(E_SENSOR_SUCCESS != dwResult)
	    {
            LOG(WARNING) + "Failed to unregister call back for sensor : " + ConvertToTypeDescription(this->m_type) + " error : " + rho::common::convertToStringA<DWORD>(dwResult);
            m_status = SENSOR_STATUS_ERROR;
		    //return (dwResult);
	    }

        RemoveEventListener(m_type);
        SENSOR_TYPE type = m_type;
        this->m_type = SENSOR_TYPE_UNDEFINED;

        // Open the sensor in default mode. 
        dwResult = m_sensorApiDll->SensorClose();
	    if(E_SENSOR_SUCCESS != dwResult)
	    {
            LOG(WARNING) + "Error closing sensor : " + ConvertToTypeDescription(type) + " error : " + rho::common::convertToStringA<DWORD>(dwResult);
            m_status = SENSOR_STATUS_ERROR;
		    //return dwResult;
	    }

        
        
    }

    return dwResult;
}


bool CRawMotoSensor::Start()
{
    bool retStatus = true;
	DWORD dwResult = E_SENSOR_SUCCESS;

    LOG(INFO) + "Attempt to kick start sampling for  sensor : " + ConvertToTypeDescription(this->m_type);

    if (SENSOR_STATUS_STARTED != m_status)
    {	
	    // Start sampling
        dwResult = m_sensorApiDll->SensorStartSampling();	
	    if(E_SENSOR_SUCCESS != dwResult)
	    {
            LOG(WARNING) + "Attempt to kick start sampling for  sensor : " + ConvertToTypeDescription(this->m_type) + " resulted in error: " + rho::common::convertToStringA<DWORD>(dwResult);
            retStatus = false;
		    m_status = SENSOR_STATUS_ERROR;        
	    }
        else
        {
            m_status = SENSOR_STATUS_STARTED;
            m_bFirstSample = true;
        }
    }
    else
    {
        LOG(WARNING) + "Sampling already started sensor: " + ConvertToTypeDescription(this->m_type);
    }
    return (retStatus);
}

bool CRawMotoSensor::Stop()
{
    bool retStatus = true;
	DWORD dwResult = E_SENSOR_SUCCESS;
    LOG(INFO) + "Attempt to stop sampling for  sensor : " + ConvertToTypeDescription(this->m_type);
	
    if (SENSOR_STATUS_STARTED == m_status)
    {
	    // Start sampling
        dwResult = m_sensorApiDll->SensorStopSampling();	
	    if(E_SENSOR_SUCCESS != dwResult)
	    {
            LOG(WARNING) + "Attempt to stop sampling for  sensor : " + ConvertToTypeDescription(this->m_type) + " resulted in error: " + rho::common::convertToStringA<DWORD>(dwResult);
            retStatus = false;
		    m_status = SENSOR_STATUS_ERROR;        
	    }    
        else
        {
            m_status = SENSOR_STATUS_READY;
        }
    }
    else
    {
        LOG(WARNING) + "Sampling already disabled for sensor : " + ConvertToTypeDescription(this->m_type);
    }
    return (retStatus);
}

void CRawMotoSensor::AddEventListener(SENSOR_TYPE sensorType, ISensorEventListener* listener)
{
    HANDLE hSensor = NULL;
    rho::common::CMutexLock globalLock(m_cs);
    if (NULL != this->m_sensorApiDll)
    {
        
        if (NULL != listener)
            g_listenerMap.put(static_cast<DWORD>(sensorType), listener);
    }

}

void CRawMotoSensor::RemoveEventListener(SENSOR_TYPE sensorType)
{
    HANDLE hSensor = NULL;
    
    rho::common::CMutexLock globalLock(m_cs);
    if (NULL != this->m_sensorApiDll)
    {
        if (g_listenerMap.containsKey(static_cast<DWORD>(sensorType)))
            g_listenerMap.remove(static_cast<DWORD>(sensorType));
    }
}

DWORD CRawMotoSensor::OnReceiveAccelerometerSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{

    DWORD dwResult = ERROR_SUCCESS;
    LOG(TRACE) + "Accerolmeter <<<";

    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_ACCELEROMETER))
    {
        LOG(WARNING) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_ACCELEROMETER) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_ACCELEROMETER);
    if (NULL == listener)
    {
        LOG(WARNING) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_ACCELEROMETER) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext()); //not using dynamic cast to avoid RTTI
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;
        char szBuffer[32] = {0};
        rho::String x,y,z;

        if (DATA_TYPE_VECTOR == ptSensorData->eType)
	    {
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_ACCELEROMETER]*/sensor->m_lScale)
		    {
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nX/sensor->m_lScale/*g_scale[SENSOR_TYPE_ACCELEROMETER]*/);
                x = szBuffer;
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nY/sensor->m_lScale/*g_scale[SENSOR_TYPE_ACCELEROMETER]*/);		
                y = szBuffer;
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nZ/sensor->m_lScale/*g_scale[SENSOR_TYPE_ACCELEROMETER]*/);
                z = szBuffer;
		    }
		    else
		    {
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nX);
                x = szBuffer;
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nY);
                y = szBuffer;
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nZ);
                z = szBuffer;
		    }
            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_ACCELEROMETER) + "(" + x + "," + y + "," + z + ")";
            listener->OnReceiveAccelerometerSensorData(x, y, z);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }
    else
    {
        LOG(TRACE) + "Ignore data for (minium gap)" + ConvertToTypeDescription(SENSOR_TYPE_ACCELEROMETER);
    }
    return dwResult;
}

DWORD CRawMotoSensor::OnReceiveOrientationSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{
    LOG(TRACE) + "Device Orientation <<<";

    DWORD dwResult = ERROR_SUCCESS;
    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_ORIENTATION))
    {
        LOG(TRACE) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_ORIENTATION) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_ORIENTATION);
    if (NULL == listener)
    {
        LOG(TRACE) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_ORIENTATION) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext());
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;

        rho::String orientation;

        if (DATA_TYPE_SCALAR == ptSensorData->eType)
	    {
            LONG lOrientation = 0;
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_ORIENTATION]*/sensor->m_lScale)
		    {
			    lOrientation = ptSensorData->data.scalar.nValue/sensor->m_lScale/*g_scale[SENSOR_TYPE_ORIENTATION]*/;
		    }
		    else
		    {
                lOrientation = ptSensorData->data.scalar.nValue;
		    }

            switch(lOrientation)
		    {
			case DEV_ORIENTATION_PORTRAIT_DOWN:
                orientation = "Portrait Down";				
				break;
			case DEV_ORIENTATION_PORTRAIT_UP:
				orientation =  "Portrait Up";					
				break;
			case DEV_ORIENTATION_LANDSCAPE_LEFT:
				orientation = "Landscape Left";
				break;
			case DEV_ORIENTATION_LANDSCAPE_RIGHT:
				orientation = "Landscape Right";
				break;
			case DEV_ORIENTATION_FACE_UP:
				orientation = "Face Up";	
				break;
			case DEV_ORIENTATION_FACE_DOWN:
				orientation = "Face Down";
				break;
			case DEV_ORIENTATION_UNKNOWN:
			default:
				orientation =  "Unknown Orientation";	
				break;
		    }
            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_ORIENTATION) + "(" + orientation + ")";
            listener->OnReceiveOrientationSensorData(orientation);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }   
    return dwResult;
}

DWORD CRawMotoSensor::OnReceiveTiltAngleSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{
    LOG(TRACE) + "TiltAngle <<<";

    DWORD dwResult = ERROR_SUCCESS;

    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_TILT_ANGLE))
    {
        LOG(TRACE) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_TILT_ANGLE) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_TILT_ANGLE);
    if (NULL == listener)
    {
        LOG(TRACE) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_TILT_ANGLE) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext());
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;       
        char szBuffer[32] = {0};
        rho::String x,y,z;

        if (DATA_TYPE_VECTOR == ptSensorData->eType)
	    {
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_TILT_ANGLE]*/sensor->m_lScale)
		    {
			    sprintf(szBuffer, "%3.2f", (float)ptSensorData->data.vector.nX/sensor->m_lScale/*g_scale[SENSOR_TYPE_TILT_ANGLE]*/);
                x = szBuffer;
			    sprintf(szBuffer, "%3.2f", (float)ptSensorData->data.vector.nY/sensor->m_lScale/*g_scale[SENSOR_TYPE_TILT_ANGLE]*/);		
                y = szBuffer;
			    sprintf(szBuffer, "%3.2f", (float)ptSensorData->data.vector.nZ/sensor->m_lScale/*g_scale[SENSOR_TYPE_TILT_ANGLE]*/);
                z = szBuffer;
		    }
		    else
		    {
			    sprintf(szBuffer, "%3.2f", (float)ptSensorData->data.vector.nX);
                x = szBuffer;
			    sprintf(szBuffer, "%3.2f", (float)ptSensorData->data.vector.nY);
                y = szBuffer;
			    sprintf(szBuffer, "%3.2f", (float)ptSensorData->data.vector.nZ);
                z = szBuffer;
		    }

            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_TILT_ANGLE) + "(" + x + "," + y + "," + z + ")";
            listener->OnReceiveTiltAngleSensorData(x, y, z);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }
    return dwResult;
}

DWORD CRawMotoSensor::OnReceiveMotionSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{
    LOG(TRACE) + "Motion <<<";

    DWORD dwResult = ERROR_SUCCESS;
    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_MOTION))
    {
        LOG(TRACE) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_MOTION) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_MOTION);
    if (NULL == listener)
    {
        LOG(TRACE) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_MOTION) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext());
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;

        if (DATA_TYPE_SCALAR == ptSensorData->eType)
	    {
            char szBuffer[32] = {0};            
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_ORIENTATION]*/sensor->m_lScale)
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue/sensor->m_lScale/*g_scale[SENSOR_TYPE_MOTION]*/);
		    }
		    else
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue);
		    }
            rho::String motionVal = szBuffer;

            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_MOTION) + "(" + motionVal + ")";            
            listener->OnReceiveMotionSensorData(motionVal);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }      
    return dwResult;
}

DWORD CRawMotoSensor::OnReceiveECompassSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{
    LOG(TRACE) + "ECompass <<<";
    DWORD dwResult = ERROR_SUCCESS;
   
    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_ECOMPASS))
    {
        LOG(TRACE) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_ECOMPASS) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_ECOMPASS);
    if (NULL == listener)
    {
        LOG(TRACE) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_ECOMPASS) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext());
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;
        if (DATA_TYPE_SCALAR == ptSensorData->eType)
	    {
            char szBuffer[32] = {0};            
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_ECOMPASS]*/sensor->m_lScale)
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue/sensor->m_lScale/*g_scale[SENSOR_TYPE_ECOMPASS]*/);
		    }
		    else
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue);
		    }
            rho::String ecompassVal = szBuffer;

            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_ECOMPASS) + "(" + ecompassVal + ")";
            listener->OnReceiveECompassSensorData(ecompassVal);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }

    return dwResult;
}

DWORD CRawMotoSensor::OnReceiveMagnetometerSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{
    LOG(TRACE) + "Magnetometer <<<";

    DWORD dwResult = ERROR_SUCCESS;
    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_MAGNETOMETER))
    {
        LOG(TRACE) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_MAGNETOMETER) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_MAGNETOMETER);
    if (NULL == listener)
    {
        LOG(TRACE) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_MAGNETOMETER) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext());
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;

        char szBuffer[32] = {0};
        rho::String x,y,z;

        if (DATA_TYPE_VECTOR == ptSensorData->eType)
	    {
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_MAGNETOMETER]*/sensor->m_lScale)
		    {
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nX/sensor->m_lScale/*g_scale[SENSOR_TYPE_MAGNETOMETER]*/);
                x = szBuffer;
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nY/sensor->m_lScale/*g_scale[SENSOR_TYPE_MAGNETOMETER]*/);		
                y = szBuffer;
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nZ/sensor->m_lScale/*g_scale[SENSOR_TYPE_MAGNETOMETER]*/);
                z = szBuffer;
		    }
		    else
		    {
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nX);
                x = szBuffer;
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nY);
                y = szBuffer;
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nZ);
                z = szBuffer;
		    }

            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_MAGNETOMETER) + "(" + x + "," + y + "," + z + ")";
            listener->OnReceiveMagnetometerSensorData(x, y, z);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }   
    return dwResult;
}

DWORD CRawMotoSensor::OnReceiveGyroscopeSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{
    LOG(TRACE) + "Gyroscope <<<";
    DWORD dwResult = ERROR_SUCCESS;
    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_GYROSCOPE))
    {
        LOG(TRACE) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_GYROSCOPE) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_GYROSCOPE);
    if (NULL == listener)
    {
        LOG(TRACE) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_GYROSCOPE) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext());
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;

        char szBuffer[32] = {0};
        rho::String x,y,z;

        if (DATA_TYPE_VECTOR == ptSensorData->eType)
	    {
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_GYROSCOPE]*/sensor->m_lScale)
		    {
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nX/sensor->m_lScale/*g_scale[SENSOR_TYPE_GYROSCOPE]*/);
                x = szBuffer;
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nY/sensor->m_lScale/*g_scale[SENSOR_TYPE_GYROSCOPE]*/);		
                y = szBuffer;
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nZ/sensor->m_lScale/*g_scale[SENSOR_TYPE_GYROSCOPE]*/);
                z = szBuffer;
		    }
		    else
		    {
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nX);
                x = szBuffer;
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nY);
                y = szBuffer;
			    sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.vector.nZ);
                z = szBuffer;
		    }

            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_MAGNETOMETER) + "(" + x + "," + y + "," + z + ")";
            listener->OnReceiveGyroscopeSensorData(x, y, z);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }   
    return dwResult;
}

DWORD CRawMotoSensor::OnReceiveAmbientLightSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{
    LOG(TRACE) + "AmbientLight <<<";
    DWORD dwResult = ERROR_SUCCESS;
    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_AMBIENT_LIGHT))
    {
        LOG(TRACE) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_AMBIENT_LIGHT) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_AMBIENT_LIGHT);
    if (NULL == listener)
    {
        LOG(TRACE) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_AMBIENT_LIGHT) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext());
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;

        if (DATA_TYPE_SCALAR == ptSensorData->eType)
	    {
            char szBuffer[32] = {0};            
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_AMBIENT_LIGHT]*/sensor->m_lScale)
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue/sensor->m_lScale/*g_scale[SENSOR_TYPE_AMBIENT_LIGHT]*/);
		    }
		    else
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue);
		    }
            rho::String ambientVal = szBuffer;

            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_AMBIENT_LIGHT) + "(" + ambientVal + ")";
            listener->OnReceiveAmbientLightSensorData(ambientVal);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }
    return dwResult;
}

DWORD CRawMotoSensor::OnReceiveProximitySensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{
    LOG(TRACE) + "Proximity <<<";

    DWORD dwResult = ERROR_SUCCESS;
    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_PROXIMITY))
    {
        LOG(TRACE) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_PROXIMITY) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_PROXIMITY);
    if (NULL == listener)
    {
        LOG(TRACE) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_PROXIMITY) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext());
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;
        if (DATA_TYPE_SCALAR == ptSensorData->eType)
	    {
            char szBuffer[32] = {0};            
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_PROXIMITY]*/sensor->m_lScale)
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue/sensor->m_lScale/*g_scale[SENSOR_TYPE_PROXIMITY]*/);
		    }
		    else
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue);
		    }
            rho::String proximityVal = szBuffer;

            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_PROXIMITY) + "(" + proximityVal + ")";
            listener->OnReceiveProximitySensorData(proximityVal);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }
    return dwResult;
}

DWORD CRawMotoSensor::OnReceiveProximityLongRangeSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{
    LOG(TRACE) + "ProximityLR <<<";

    DWORD dwResult = ERROR_SUCCESS;

    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_PROXIMITY_LONG_RANGE))
    {
        LOG(TRACE) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_PROXIMITY_LONG_RANGE) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_PROXIMITY_LONG_RANGE);
    if (NULL == listener)
    {
        LOG(TRACE) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_PROXIMITY_LONG_RANGE) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext());
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;
        if (DATA_TYPE_SCALAR == ptSensorData->eType)
	    {
            char szBuffer[32] = {0};            
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_PROXIMITY_LONG_RANGE]*/sensor->m_lScale)
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue/sensor->m_lScale/*g_scale[SENSOR_TYPE_PROXIMITY_LONG_RANGE]*/);
		    }
		    else
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue);
		    }
            rho::String proximityLrVal = szBuffer;

            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_PROXIMITY_LONG_RANGE) + "(" + proximityLrVal + ")";
            listener->OnReceiveProximityLongRangeSensorData(proximityLrVal);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }
    return dwResult;
}

DWORD CRawMotoSensor::OnReceivePressureSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{
    LOG(TRACE) + "Pressure <<<";
    DWORD dwResult = ERROR_SUCCESS;
    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_PRESSURE))
    {
        LOG(TRACE) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_PRESSURE) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_PRESSURE);
    if (NULL == listener)
    {
        LOG(TRACE) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_PRESSURE) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext());
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;
        if (DATA_TYPE_SCALAR == ptSensorData->eType)
	    {
            char szBuffer[32] = {0};            
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_PRESSURE]*/sensor->m_lScale)
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue/sensor->m_lScale/*g_scale[SENSOR_TYPE_PRESSURE]*/);
		    }
		    else
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue);
		    }
            rho::String pressure = szBuffer;

            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_PRESSURE) + "(" + pressure + ")";
            listener->OnReceivePressureSensorData(pressure);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }
    return dwResult;
}

DWORD CRawMotoSensor::OnReceiveTemperatureSensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{
    LOG(TRACE) + "Temperature <<<";
    DWORD dwResult = ERROR_SUCCESS;
    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_TEMPERATURE))
    {
        LOG(TRACE) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_TEMPERATURE) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_TEMPERATURE);
    if (NULL == listener)
    {
        LOG(TRACE) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_TEMPERATURE) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext());
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;
        if (DATA_TYPE_SCALAR == ptSensorData->eType)
	    {
            char szBuffer[32] = {0};            
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_TEMPERATURE]*/sensor->m_lScale)
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue/sensor->m_lScale/*g_scale[SENSOR_TYPE_TEMPERATURE]*/);
		    }
		    else
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue);
		    }
            rho::String temperature = szBuffer;

            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_TEMPERATURE) + "(" + temperature + ")";
            listener->OnReceiveTemperatureSensorData(temperature);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }
    return dwResult;
}

DWORD CRawMotoSensor::OnReceiveHumiditySensorData(HANDLE hSensor, SENSOR_DATA_T* ptSensorData, DWORD dwSize, DWORD dwDrop)
{
    LOG(TRACE) + "Humidity <<<";
    DWORD dwResult = ERROR_SUCCESS;
    rho::common::CMutexLock lock(m_cs);

    // if don't have the call back registered ignore the computation
    if (!g_listenerMap.containsKey(SENSOR_TYPE_HUMIDITY))
    {
        LOG(TRACE) + "Callback for " + ConvertToTypeDescription(SENSOR_TYPE_HUMIDITY) + " is not registered.";
        return dwResult;
    }

    // if we cant find the listener avoid the computation
    ISensorEventListener* listener = g_listenerMap.get(SENSOR_TYPE_HUMIDITY);
    if (NULL == listener)
    {
        LOG(TRACE) + "No listener for " + ConvertToTypeDescription(SENSOR_TYPE_HUMIDITY) + "found.";
        return dwResult;
    }

    CRawMotoSensor* sensor = reinterpret_cast<CRawMotoSensor*>(listener->GetCurrentContext());
    
    DWORD dwTC = ::GetTickCount();
    ULONGLONG ullElapsed = 0;
    if (sensor->m_dwQuietTime <= dwTC) // perhaps that tickcount hasn't rolled over
    {
        ullElapsed = dwTC - sensor->m_dwQuietTime;
    }
    else
    {
        ullElapsed = (ULONGLONG)(((DWORD)(0xffffffff)) - sensor->m_dwQuietTime) + dwTC;
    }

    if ((sensor->m_bFirstSample) ||(ullElapsed >= sensor->m_minimumInterval))
    {
        if (sensor->m_bFirstSample)
            sensor->m_bFirstSample = false;
        if (DATA_TYPE_SCALAR == ptSensorData->eType)
	    {
            char szBuffer[32] = {0};            
		    // Convert values to signed values and record
            if (0 != /*g_scale[SENSOR_TYPE_HUMIDITY]*/sensor->m_lScale)
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue/sensor->m_lScale/*g_scale[SENSOR_TYPE_HUMIDITY]*/);
		    }
		    else
		    {
                sprintf(szBuffer, "%4.4f", (float)ptSensorData->data.scalar.nValue);
		    }
            rho::String humidity = szBuffer;

            LOG(TRACE) + "Data for " + ConvertToTypeDescription(SENSOR_TYPE_HUMIDITY) + "(" + humidity + ")";
            listener->OnReceiveHumiditySensorData(humidity);
            sensor->m_dwQuietTime = dwTC;
            
	    }
    }
    return dwResult;
}

const char* CRawMotoSensor::ConvertToTypeDescription(SENSOR_TYPE sensorType)
{
    const char* type = NULL;

    switch (sensorType)
    {
        case SENSOR_TYPE_ACCELEROMETER:
            type = rho::ISensor::SENSOR_TYPE_ACCELEROMETER;
            break;
        case SENSOR_TYPE_ORIENTATION:
            type = rho::ISensor::SENSOR_TYPE_DEVICE_ORIENTATION;
            break;
        case SENSOR_TYPE_TILT_ANGLE:
            type = rho::ISensor::SENSOR_TYPE_TILT_ANGLE;
            break;
        case SENSOR_TYPE_MOTION:
            type = rho::ISensor::SENSOR_TYPE_MOTION;
            break;
        case SENSOR_TYPE_ECOMPASS:
            type = rho::ISensor::SENSOR_TYPE_ECOMPASS;
            break;
        case SENSOR_TYPE_MAGNETOMETER:
            type = rho::ISensor::SENSOR_TYPE_MAGNETOMETER;
            break;
        case SENSOR_TYPE_GYROSCOPE:
            type = rho::ISensor::SENSOR_TYPE_GYROSCOPE;
            break;
        case SENSOR_TYPE_AMBIENT_LIGHT:
            type = rho::ISensor::SENSOR_TYPE_AMBIENT_LIGHT;
            break;
        case SENSOR_TYPE_PROXIMITY:
            type = rho::ISensor::SENSOR_TYPE_PROXIMITY;
            break;
        case SENSOR_TYPE_PROXIMITY_LONG_RANGE:
            type = rho::ISensor::SENSOR_TYPE_PROXIMITY_LONG_RANGE;
            break;
        case SENSOR_TYPE_PRESSURE:
            type = rho::ISensor::SENSOR_TYPE_PRESSURE;
            break;
        case SENSOR_TYPE_TEMPERATURE:
            type = rho::ISensor::SENSOR_TYPE_TEMPERATURE;
            break;
        case SENSOR_TYPE_HUMIDITY:
            type = rho::ISensor::SENSOR_TYPE_HUMIDITY;
            break;
        default:
			type = SENSOR_NAME_INVALID;
            break;
        
    }

    return type;
}

SENSOR_TYPE CRawMotoSensor::ConvertFromTypeDescription(const char* type)
{
    SENSOR_TYPE sensortype = SENSOR_TYPE_UNDEFINED;

    if (_stricmp(type, rho::ISensor::SENSOR_TYPE_ACCELEROMETER) == 0)
	{
        sensortype = SENSOR_TYPE_ACCELEROMETER;    
	}
	else if (_stricmp(type, rho::ISensor::SENSOR_TYPE_DEVICE_ORIENTATION) == 0)
	{
        sensortype = SENSOR_TYPE_ORIENTATION;
	}
	else if (_stricmp(type, rho::ISensor::SENSOR_TYPE_TILT_ANGLE) == 0)
	{
        sensortype = SENSOR_TYPE_TILT_ANGLE;
	}
	else if (_stricmp(type, rho::ISensor::SENSOR_TYPE_MOTION) == 0)
	{
        sensortype = SENSOR_TYPE_MOTION;
	}
	else if (_stricmp(type, rho::ISensor::SENSOR_TYPE_ECOMPASS) == 0)
	{
        sensortype = SENSOR_TYPE_ECOMPASS;
	}
	else if (_stricmp(type, rho::ISensor::SENSOR_TYPE_MAGNETOMETER) == 0)
	{
        sensortype = SENSOR_TYPE_MAGNETOMETER;
	}
	else if (_stricmp(type, rho::ISensor::SENSOR_TYPE_GYROSCOPE) == 0)
	{
        sensortype = SENSOR_TYPE_GYROSCOPE;
	}
	else if (_stricmp(type, rho::ISensor::SENSOR_TYPE_AMBIENT_LIGHT) == 0)
	{
        sensortype = SENSOR_TYPE_AMBIENT_LIGHT;
	}
	else if (_stricmp(type, rho::ISensor::SENSOR_TYPE_PROXIMITY) == 0)
	{
        sensortype = SENSOR_TYPE_PROXIMITY;
	}
	else if (_stricmp(type, rho::ISensor::SENSOR_TYPE_PROXIMITY_LONG_RANGE) == 0)
	{
        sensortype = SENSOR_TYPE_PROXIMITY_LONG_RANGE;
	}
	else if (_stricmp(type, rho::ISensor::SENSOR_TYPE_PRESSURE) == 0)
	{
        sensortype = SENSOR_TYPE_PRESSURE;
	}
	else if (_stricmp(type, rho::ISensor::SENSOR_TYPE_TEMPERATURE) == 0)
	{
        sensortype = SENSOR_TYPE_TEMPERATURE;
	}
	else if (_stricmp(type, rho::ISensor::SENSOR_TYPE_HUMIDITY) == 0)
	{
        sensortype = SENSOR_TYPE_HUMIDITY;
	}

    return sensortype;
}

const char* CRawMotoSensor::ConvertToStatusDescription(SensorStatus sensorStatus)
{
    const char* status = NULL;
    switch (sensorStatus)
    {
    case SENSOR_STATUS_NOT_READY:
        status = rho::ISensor::SENSOR_STATUS_NOT_READY;
        break;
    case SENSOR_STATUS_READY:
        status = rho::ISensor::SENSOR_STATUS_READY;
        break;
    case SENSOR_STATUS_STARTED:
        status = rho::ISensor::SENSOR_STATUS_STARTED;
        break;
    case SENSOR_STATUS_ERROR:
    default:
        status = rho::ISensor::SENSOR_STATUS_ERROR;
        break;
    
    }

    return status;

}

SensorStatus CRawMotoSensor::ConvertFromStatusDescription(const char* status)
{
    SensorStatus sensorStatus = SENSOR_STATUS_ERROR;;

    if ((NULL == status) || (0 == *status))
    {
        sensorStatus = SENSOR_STATUS_ERROR;
    }
    else if (_stricmp(status, rho::ISensor::SENSOR_STATUS_NOT_READY) == 0)
    {
        sensorStatus = SENSOR_STATUS_NOT_READY;
    }
    else if (_stricmp(status, rho::ISensor::SENSOR_STATUS_READY) == 0)
    {
        sensorStatus = SENSOR_STATUS_READY;
    }
    else if (_stricmp(status, rho::ISensor::SENSOR_STATUS_STARTED) == 0)
    {
        sensorStatus = SENSOR_STATUS_STARTED;
    }
    else if (_stricmp(status, rho::ISensor::SENSOR_STATUS_ERROR) == 0)
    {
        sensorStatus = SENSOR_STATUS_ERROR;
    }

    return sensorStatus;
}

rho::Hashtable<DWORD, sensormodule::ISensorEventListener*> CRawMotoSensor::g_listenerMap;
//std::vector<LONG> CRawMotoSensor::g_scale(16,1);
rho::common::CMutex  CRawMotoSensor::m_cs;

} // namespace ends
