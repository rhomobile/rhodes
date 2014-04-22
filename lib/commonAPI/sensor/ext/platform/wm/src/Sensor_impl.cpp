#include <common/RhodesApp.h>
#include "../../../shared/generated/cpp/SensorBase.h"
#include <common/StringConverter.h>
#include "SensorAPI.h"
#include "RawMotoSensor.h"
#include <algorithm>
#include "SensorEventListener.h"

namespace rho
{

using namespace apiGenerator;

enum SensorChipManufacturer
{
    SCM_MOTOROLA_SOLUTIONS = 0,
    SCM_UNKNOWN
};

static SensorChipManufacturer g_sensorChipManfacturer = SCM_UNKNOWN;

class CSensorImpl : public CSensorBase, public common::IRhoExtension, public sensormodule::ISensorEventListener
{
public:
    CSensorImpl(const rho::String& strID);
    virtual ~CSensorImpl();

    virtual void getMinimumGap(rho::apiGenerator::CMethodResult& oResult);
    virtual void setMinimumGap(int minimumGap, rho::apiGenerator::CMethodResult& oResult);
    virtual void getType(rho::apiGenerator::CMethodResult& oResult);
    virtual void getStatus(rho::apiGenerator::CMethodResult& oResult);
    virtual void start(rho::apiGenerator::CMethodResult& oResult);
    virtual void readData(rho::apiGenerator::CMethodResult& oResult);
    virtual void stop(rho::apiGenerator::CMethodResult& oResult);
    virtual void getProperty(const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult);
    virtual void getProperties(const rho::Vector<rho::String>& arrayofNames, rho::apiGenerator::CMethodResult& oResult);
    virtual void getAllProperties(rho::apiGenerator::CMethodResult& oResult);
    virtual void setProperty(const rho::String& propertyName,  const rho::String& propertyValue, rho::apiGenerator::CMethodResult& oResult);
    virtual void setProperties(const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);

    // sensor event listeners
    virtual void OnReceiveAccelerometerSensorData(rho::String& accel_x, rho::String& accel_y, rho::String& accel_z);
    virtual void OnReceiveOrientationSensorData(rho::String& orientation);
    virtual void OnReceiveTiltAngleSensorData(rho::String& tilt_x, rho::String& tilt_y, rho::String& tilt_z);
    virtual void OnReceiveMotionSensorData(rho::String& motion);
    virtual void OnReceiveECompassSensorData(rho::String& ecompassVal);
    virtual void OnReceiveMagnetometerSensorData(rho::String& mm_x, rho::String& mm_y, rho::String& mm_z);
    virtual void OnReceiveGyroscopeSensorData(rho::String& gyro_x, rho::String& gyro_y, rho::String& gyro_z);
    virtual void OnReceiveAmbientLightSensorData(rho::String& ambientVal);
    virtual void OnReceiveProximitySensorData(rho::String& proximityVal);
    virtual void OnReceiveProximityLongRangeSensorData(rho::String& proximitylrVal);
    virtual void OnReceivePressureSensorData(rho::String& temperature);
    virtual void OnReceiveTemperatureSensorData(rho::String& temperature);
    virtual void OnReceiveHumiditySensorData(rho::String& humidity);

    virtual sensormodule::IRawSensor* GetCurrentContext();

protected:

private:
    sensormodule::IRawSensor                        *m_rawSensor;
    rho::String                                     m_sensorId;
    rho::apiGenerator::CMethodResult                *m_callbackMethodResult;
    rho::Hashtable<rho::String, rho::String>        m_cache;
    rho::common::CMutex                             m_cs;

};

CSensorImpl::CSensorImpl(const rho::String& strID) : CSensorBase(), m_rawSensor(NULL), m_callbackMethodResult(NULL)
{
    m_hashProps.put("ID", strID); //TODO: understand backgrounf
    if (SCM_MOTOROLA_SOLUTIONS == g_sensorChipManfacturer)
    {
        LOG(INFO) + "Initialising Motorola Sensor Interface for type " + strID;
        RHODESAPP().getExtManager().registerExtension((strID), this);
        // its a motorola sensor, use the motorola class
        sensormodule::CRawMotoSensor *rawSensor = new sensormodule::CRawMotoSensor(strID.c_str());
        if (NULL != rawSensor)
        {
            if (rawSensor->RegisterSensor(this))
            {
                m_rawSensor = rawSensor;
                m_sensorId = strID;
            }
            else
                delete rawSensor;
        }
    }

}

CSensorImpl::~CSensorImpl()
{
    LOG(INFO) + "Shutting down Motorola Sensor Interface for type " + m_sensorId;
    if (NULL != m_rawSensor)
    {
        delete m_rawSensor;
        m_rawSensor = NULL;
    }
    m_sensorId.clear();
}

void CSensorImpl::getMinimumGap(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Request received for minimum gap : " + m_sensorId;
    if (NULL == m_rawSensor)
    {
        oResult.setError("No sensor found. Cannot get minimumGap.");
    }
    else
    {
        oResult.set(this->m_rawSensor->getMinimumGap());
    }

}

void CSensorImpl::setMinimumGap(int minimumGap, rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Request received to set minimum gap : " + m_sensorId + " : to " + rho::common::convertToStringA<int>(minimumGap);
    if (NULL == m_rawSensor)
    {
        oResult.setError("No sensor found. Cannot set minimumegap.");
    }
    else
    {
        if (minimumGap < 200)
            minimumGap = 200;         
        this->m_rawSensor->setMinimumGap(minimumGap);
        oResult.set(true);
    }
}

void CSensorImpl::getType(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Request received for type : " + m_sensorId;
    if (NULL == m_rawSensor)
    {
        oResult.setError("No sensor found. Cannot get type.");
    } else
    {
        oResult.set(this->m_rawSensor->getType());
    }
}

void CSensorImpl::getStatus(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Request received for status : " + m_sensorId;
    if (NULL == m_rawSensor)
    {
        oResult.setError("No sensor found. Cannot get status.");
    } else
    {
        oResult.set(this->m_rawSensor->getStatus());
    }

}

void CSensorImpl::start(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Received request to start sensor: " + m_sensorId;
    if (NULL == m_rawSensor)
    {
        oResult.setError("No sensor found. Cannot get start.");
    }
    else
    {
        //const char * status = m_rawSensor->getStatus();
        this->m_rawSensor->Start();
        rho::common::CMutexLock lock(m_cs);
        if (oResult.hasCallback())
        {
            m_callbackMethodResult = &oResult;
        }
        else
        {
            m_callbackMethodResult = NULL;
        }
    }

}

void CSensorImpl::readData(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Received request to readData: " + m_sensorId;
    if (NULL == m_rawSensor)
    {
        oResult.setError("No sensor found. Cannot get stop.");
    }
    else
    {
        rho::common::CMutexLock lock(m_cs);
        if (m_cache.empty())
        {
            rho::Hashtable<rho::String, rho::String> props;
            props.put("message", "No Samples received from sensor. Check status");// The sampling is stopped (check status) or retry based on the minium gap property or reduce the gap interval");
	        props.put("status", "error");
            oResult.set(props);
        }
        else
            oResult.set(m_cache);
    }
}

void CSensorImpl::stop(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Received request to stop sensor: " + m_sensorId;
    if (NULL == m_rawSensor)
    {
        oResult.setError("No sensor found. Cannot get stop.");
    }
    else
    {
        
        this->m_rawSensor->Stop();
        if ((NULL != m_callbackMethodResult) && (m_callbackMethodResult->hasCallback()))
        {
            m_callbackMethodResult = NULL;

        }
        rho::common::CMutexLock lock(m_cs);
        m_cache.clear();
    }
}

void CSensorImpl::getProperty(const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Received request for get property : " + m_sensorId + " : " + propertyName;
    if (NULL == m_rawSensor)
    {
        oResult.setError("No sensor found. Cannot set " + propertyName);
    } else
    {
        if (_stricmp(propertyName.c_str(), "minimumGap") == 0)
        {
            oResult.set(this->m_rawSensor->getMinimumGap());
        } else if (_stricmp(propertyName.c_str(), "type") == 0)
        {
            oResult.set(this->m_rawSensor->getType());
        } else if (_stricmp(propertyName.c_str(), "status") == 0)
        {
            oResult.set(this->m_rawSensor->getStatus());
        } else
        {
            oResult.setError("Invalid attribute");
        }
    }
}

void CSensorImpl::getProperties(const rho::Vector<rho::String>& arrayofNames, rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Received request for get properties : " + m_sensorId;

    if (NULL == m_rawSensor)
    {
        oResult.setError("No sensor found. Cannot get properties");
    } else
    {
        rho::Hashtable<rho::String, rho::String> propsHash;
        for (rho::Vector<rho::String>::const_iterator namesIterator = arrayofNames.begin(); namesIterator != arrayofNames.end(); namesIterator++)
        {
            if (_stricmp((*namesIterator).c_str(), "minimumGap") == 0)
            {
                propsHash.put(*namesIterator, rho::common::convertToStringA<int>(this->m_rawSensor->getMinimumGap()));
            } else if (_stricmp((*namesIterator).c_str(), "type") == 0)
            {
                propsHash.put(*namesIterator, this->m_rawSensor->getType());
            } else if (_stricmp((*namesIterator).c_str(), "status") == 0)
            {
                propsHash.put(*namesIterator, this->m_rawSensor->getStatus());
            } else
            {
                propsHash.put(*namesIterator, "Invalid attribute");
            }
        }
        oResult.set(propsHash);
    }


}

void CSensorImpl::getAllProperties(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Received request for get all properties : " + m_sensorId;

    if (NULL == m_rawSensor)
    {
        oResult.setError("No sensor found. Cannot get properties");
    } else
    {
        rho::Hashtable<rho::String, rho::String> propsHash;

        propsHash.put("minimumGap", rho::common::convertToStringA<int>(this->m_rawSensor->getMinimumGap()));
        propsHash.put("type", this->m_rawSensor->getType());
        propsHash.put("status", this->m_rawSensor->getStatus());
        oResult.set(propsHash);

    }

}

void CSensorImpl::setProperty(const rho::String& propertyName,  const rho::String& propertyValue, rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Received request to set property : " + m_sensorId + " : " + propertyName + " = " + propertyValue;
    if (NULL == m_rawSensor)
    {
        oResult.setError("No sensor found. Cannot set property " + propertyName);
    } else
    {
        if (_stricmp(propertyName.c_str(), "minimumGap") == 0)
        {
            int minimumGap = atoi(propertyValue.c_str());
            if (minimumGap < 200)
            {
                minimumGap = 200;
            }
            this->m_rawSensor->setMinimumGap(minimumGap);            
        } else
        {
            oResult.setError(propertyName + " is readonly or is invalid.");
        }
    }
}

void CSensorImpl::setProperties(const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Received request to set properties : " + m_sensorId;

    if (NULL == m_rawSensor)
    {
        oResult.setError("No sensor found. Cannot set properties");
    } else
    {
        for (rho::Hashtable<rho::String, rho::String>::const_iterator propMapItr = propertyMap.begin(); propMapItr != propertyMap.end(); propMapItr++)
        {
            if (_stricmp(propMapItr->first.c_str(), "minimumGap") == 0)
            {
                int minimumGap = atoi(propMapItr->second.c_str());
                if (minimumGap < 200)
                {
                    minimumGap = 200;
                }
                this->m_rawSensor->setMinimumGap(minimumGap);
            } else
            {
                oResult.setError(propMapItr->first + " is readonly or is invalid.");
            }
        }
    }
}

void CSensorImpl::OnReceiveAccelerometerSensorData(rho::String& accel_x, rho::String& accel_y, rho::String& accel_z)
{
    LOG(TRACE) + "Received accelerometer event: " + m_sensorId;
    
    rho::Hashtable<rho::String, rho::String> props;

    props.put("accelerometer_x", accel_x);
	props.put("accelerometer_y", accel_y);
	props.put("accelerometer_z", accel_z);
    props.put("message", "");
	props.put("status", "ok");


    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
}

void CSensorImpl::OnReceiveOrientationSensorData(rho::String& orientation)
{
    LOG(TRACE) + "Received device orientation event: " + m_sensorId;

    rho::Hashtable<rho::String, rho::String> props;

    props.put("deviceorientation_value", orientation);
    props.put("message", "");
	props.put("status", "ok");

    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
}

void CSensorImpl::OnReceiveTiltAngleSensorData(rho::String& tilt_x, rho::String& tilt_y, rho::String& tilt_z)
{
    LOG(TRACE) + "Received Tilt angle sensor event: " + m_sensorId;

    rho::Hashtable<rho::String, rho::String> props;

    props.put("tiltangle_x", tilt_x);
	props.put("tiltangle_y", tilt_y);
	props.put("tiltangle_z", tilt_z);
    props.put("message", "");
	props.put("status", "ok");

    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
    
}

void CSensorImpl::OnReceiveMotionSensorData(rho::String& motion)
{
    LOG(TRACE) + "Received motion sensor event: " + m_sensorId;

    rho::Hashtable<rho::String, rho::String> props;

    props.put("motion_value", motion);
    props.put("message", "");
	props.put("status", "ok");

    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
}

void CSensorImpl::OnReceiveECompassSensorData(rho::String& ecompassVal)
{
    LOG(TRACE) + "Received ecompass event: " + m_sensorId;

    rho::Hashtable<rho::String, rho::String> props;

    props.put("ecompass_value", ecompassVal);
    props.put("message", "");
	props.put("status", "ok");

    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
}

void CSensorImpl::OnReceiveMagnetometerSensorData(rho::String& mm_x, rho::String& mm_y, rho::String& mm_z)
{
    LOG(TRACE) + "Received magnetometer event: " + m_sensorId;

    rho::Hashtable<rho::String, rho::String> props;

    props.put("magnetometer_x", mm_x);
	props.put("magnetometer_y", mm_y);
	props.put("magnetometer_z", mm_z);
    props.put("message", "");
	props.put("status", "ok");

    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
}

void CSensorImpl::OnReceiveGyroscopeSensorData(rho::String& gyro_x, rho::String& gyro_y, rho::String& gyro_z)
{
    LOG(TRACE) + "Received gyroscope event: " + m_sensorId;

    rho::Hashtable<rho::String, rho::String> props;

    props.put("gyroscope_x", gyro_x);
	props.put("gyroscope_y", gyro_y);
	props.put("gyroscope_z", gyro_z);
    props.put("message", "");
	props.put("status", "ok");

    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
}

void CSensorImpl::OnReceiveAmbientLightSensorData(rho::String& ambientVal)
{
    LOG(TRACE) + "Received ambient light sensor event: " + m_sensorId;

    rho::Hashtable<rho::String, rho::String> props;

    props.put("ambientlight_value", ambientVal);
    props.put("message", "");
	props.put("status", "ok");

    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
}

void CSensorImpl::OnReceiveProximitySensorData(rho::String& proximityVal)
{
    LOG(TRACE) + "Received proximity sensor event: " + m_sensorId;
    rho::Hashtable<rho::String, rho::String> props;

    props.put("proximity_value", proximityVal);
    props.put("message", "");
	props.put("status", "ok");

    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
}

void CSensorImpl::OnReceiveProximityLongRangeSensorData(rho::String& proximitylrVal)
{
    LOG(TRACE) + "Received proximitylr sensor event: " + m_sensorId;

    rho::Hashtable<rho::String, rho::String> props;

    props.put("proximitylongrange_value", proximitylrVal);
    props.put("message", "");
	props.put("status", "ok");

    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
}

void CSensorImpl::OnReceivePressureSensorData(rho::String& pressure)
{
    LOG(TRACE) + "Received pressure sensor event: " + m_sensorId;

    rho::Hashtable<rho::String, rho::String> props;

    props.put("pressure_value", pressure);
    props.put("message", "");
	props.put("status", "ok");

    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
}

void CSensorImpl::OnReceiveTemperatureSensorData(rho::String& temperature)
{
    LOG(TRACE) + "Received temperature sensor event: " + m_sensorId;

    rho::Hashtable<rho::String, rho::String> props;

    props.put("temperature_value", temperature);
    props.put("message", "");
	props.put("status", "ok");

    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
}

void CSensorImpl::OnReceiveHumiditySensorData(rho::String& humidity)
{
    LOG(TRACE) + "Received humidity sensor event: " + m_sensorId;

    rho::Hashtable<rho::String, rho::String> props;

    props.put("humidity_value", humidity);
    props.put("message", "");
	props.put("status", "ok");

    rho::common::CMutexLock lock(m_cs);
    m_cache = props;    
    if (NULL != m_callbackMethodResult)
    {
        m_callbackMethodResult->set(props);
        LOG(TRACE) + "Invoke callback for " + m_sensorId;
    }
}

sensormodule::IRawSensor* CSensorImpl::GetCurrentContext()
{
    return m_rawSensor;
}



class CSensorSingleton : public CSensorSingletonBase
{
public:
    CSensorSingleton();
    virtual ~CSensorSingleton();

    //methods
    virtual void makeSensorByType(const rho::String& type, rho::apiGenerator::CMethodResult& oResult);
    virtual void enumerate(rho::apiGenerator::CMethodResult& oResult);


    //virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor) {}
    //virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) {}
protected:

private:


};

CSensorSingleton::CSensorSingleton()
{
    rho::Vector<rho::String> sensorList;
    // whether motorola sensor apis are supported is done by
    // an enumerate test
    sensormodule::CRawMotoSensor::EnumerateSensors(sensorList);
    if (sensorList.size() > 0)
    {
        g_sensorChipManfacturer = SCM_MOTOROLA_SOLUTIONS;
        sensorList.clear();
        LOG(INFO) + "Motorola sensor found in device.";
    } else
    {
        LOG(WARNING) + "No sensor found in device.";
    }

}

CSensorSingleton::~CSensorSingleton()
{


}

void CSensorSingleton::makeSensorByType(const rho::String& type, rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Requested sensor type : " + type;
    if (SCM_MOTOROLA_SOLUTIONS == g_sensorChipManfacturer)
    {
        rho::Vector<rho::String> sensorIDs;
        sensormodule::CRawMotoSensor::EnumerateSensors(sensorIDs);
        rho::Vector<rho::String>::iterator found = std::find(sensorIDs.begin(), sensorIDs.end(), type);
        if (found != sensorIDs.end())
        {
            rho::String result = *found;
            oResult.set(result);
            LOG(INFO) + "Requested sensor type  " + type + " found under Motorola supported sensors.";
        } else
        {
            LOG(WARNING) + "Requested sensor type  " + type + " not found under Motorola supported sensors.";
            //oResult.setError("Sensor type " + type + " not supported.");

        }
    }
    else
    {
        //oResult.setError("Sensor type " + type + " not supported. No Motorola Sensor found.");
        LOG(WARNING) + "Requested sensor type  " + type + " not found under Motorola supported sensors..";
    }

}

void CSensorSingleton::enumerate(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "Enumerate sensor types";
    if (SCM_MOTOROLA_SOLUTIONS == g_sensorChipManfacturer)
    {
        rho::Vector<rho::String> sensorIDs;
        sensormodule::CRawMotoSensor::EnumerateSensors(sensorIDs);
        oResult.set(sensorIDs);
        LOG(TRACE) + "Found " + sensorIDs.size() + " Motorola based sensor types";
    }
}

class CSensorFactory : public CSensorFactoryBase
{
    ~CSensorFactory() { }
    virtual ISensorSingleton* createModuleSingleton();
    virtual ISensor* createModuleByID(const rho::String& strID);

};

extern "C"
void Init_Sensor_extension()
{
    LOG(TRACE) + "Initialize sensor extension.";
    CSensorFactory::setInstance(new CSensorFactory());
    Init_Sensor_API();
}

ISensorSingleton* CSensorFactory::createModuleSingleton()
{
    LOG(TRACE) + "Create sensor singleton.";
    return new CSensorSingleton();
}

ISensor* CSensorFactory::createModuleByID(const rho::String& strID)
{
	LOG(TRACE) + "Create sensor by id: " + strID;
    return new CSensorImpl(strID);
}
} //namespace ends
