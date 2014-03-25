#pragma once
#include "SensorApiDll.h"
#include "RawSensor.h"
namespace sensormodule
{

struct ISensorEventListener
{
    virtual void OnReceiveAccelerometerSensorData(rho::String& x, rho::String& y, rho::String& z) = 0; 
	virtual void OnReceiveOrientationSensorData(rho::String& orientation) = 0;
	virtual void OnReceiveTiltAngleSensorData(rho::String& tilt_x, rho::String& tilt_y, rho::String& tilt_z) = 0;	
	virtual void OnReceiveMotionSensorData(rho::String& motion) = 0; 
	virtual void OnReceiveECompassSensorData(rho::String& ecompassVal) = 0;
	virtual void OnReceiveMagnetometerSensorData(rho::String& mm_x, rho::String& mm_y, rho::String& mm_z) = 0;
	virtual void OnReceiveGyroscopeSensorData(rho::String& gyro_x, rho::String& gyro_y, rho::String& gyro_z) = 0; 
	virtual void OnReceiveAmbientLightSensorData(rho::String& ambientVal) = 0;
	virtual void OnReceiveProximitySensorData(rho::String& proximityVal) = 0;
	virtual void OnReceiveProximityLongRangeSensorData(rho::String& proximitylrVal) = 0; 
	virtual void OnReceivePressureSensorData(rho::String& pressure) = 0;
	virtual void OnReceiveTemperatureSensorData(rho::String& temperature) = 0;
	virtual void OnReceiveHumiditySensorData(rho::String& humidity) = 0;

    virtual IRawSensor* GetCurrentContext() = 0;
};

} //namespace ends