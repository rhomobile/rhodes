package com.motorolasolutions.rho.sensor;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import com.rho.sensor.ISensorSingleton;
import com.rho.sensor.SensorSingletonBase;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;

class SensorSingleton extends SensorSingletonBase implements ISensorSingleton {	

	private String TAG = "Rho::" + SensorSingleton.class.getSimpleName();  
	private SensorManager mSensorManager = (SensorManager) RhodesActivity.safeGetInstance().getSystemService("sensor");	

	public SensorSingleton(SensorFactory factory) {
		super();		
	}

	List<Object> getIDs() {
		Logger.D(TAG, "SensorSingleton getIDs");
		List<Object> ids = new LinkedList<Object>();	
		 
		ids.add(SENSOR_TYPE_ACCELEROMETER);
		ids.add(SENSOR_TYPE_TILT_ANGLE);
		ids.add(SENSOR_TYPE_DEVICE_ORIENTATION);
		ids.add(SENSOR_TYPE_MOTION);
		ids.add(SENSOR_TYPE_ECOMPASS);
		ids.add(SENSOR_TYPE_MAGNETOMETER);
		ids.add(SENSOR_TYPE_GYROSCOPE);
		ids.add(SENSOR_TYPE_AMBIENT_LIGHT);
		ids.add(SENSOR_TYPE_PROXIMITY);
		ids.add(SENSOR_TYPE_PROXIMITY_LONG_RANGE);
		ids.add(SENSOR_TYPE_PRESSURE);
		ids.add(SENSOR_TYPE_TEMPERATURE);
		ids.add(SENSOR_TYPE_HUMIDITY);
		ids.add(SENSOR_TYPE_GRAVITY);
		ids.add(SENSOR_TYPE_LINEAR_ACCELERATION);
		ids.add(SENSOR_TYPE_ROTATION);
		ids.add(SENSOR_TYPE_ORIENTATION);
       
        return ids;
	}

	@Override
	public void makeSensorByType(String type, IMethodResult result)
	{
		// check if the sensor objects are present and then add then to the
		// the supported list
		Logger.D(TAG, "Request sensor type " + type);
		System.out.println("Request sensor type " + type);
		if (type.equalsIgnoreCase(SENSOR_TYPE_ACCELEROMETER) && (mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER) != null))
		{
            result.set((String) getIDs().get(0));
		}
		else if (type.equalsIgnoreCase(SENSOR_TYPE_TILT_ANGLE) && (mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER) != null))  // Tilt angle is measured from the accelorometr data
		{
			result.set((String) getIDs().get(1));
		}
		//else if (type == SENSOR_TYPE_DEVICE_ORIENTATION)
			//result.set((String) getIDs().get(2));
		//else if (type == SENSOR_TYPE_MOTION)
			//result.set((String) getIDs().get(3));
		//else if (type == SENSOR_TYPE_ECOMPASS)
			//result.set((String) getIDs().get(4));
		else if (type.equalsIgnoreCase(SENSOR_TYPE_MAGNETOMETER) && (mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD) != null))
		{
            result.set((String) getIDs().get(5));
		}
		else if (type.equalsIgnoreCase(SENSOR_TYPE_GYROSCOPE) && (mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE) != null))
		{
			result.set((String) getIDs().get(6));
		}
		else if (type.equalsIgnoreCase(SENSOR_TYPE_AMBIENT_LIGHT) && (mSensorManager.getDefaultSensor(Sensor.TYPE_LIGHT) != null))
		{
            result.set((String) getIDs().get(7));
		}
		else if (type.equalsIgnoreCase(SENSOR_TYPE_PROXIMITY) && (mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY) != null))
		{
            result.set((String) getIDs().get(8));
		}
		//else if (type == SENSOR_TYPE_PROXIMITY_LONG_RANGE)
			//result.set((String) getIDs().get(9));
		else if (type.equalsIgnoreCase(SENSOR_TYPE_PRESSURE) && (mSensorManager.getDefaultSensor(Sensor.TYPE_PRESSURE) != null))
		{
            result.set((String) getIDs().get(10));
		}
		else if (type.equalsIgnoreCase(SENSOR_TYPE_TEMPERATURE) && (mSensorManager.getDefaultSensor(Sensor.TYPE_TEMPERATURE) != null))
		{
            result.set((String) getIDs().get(11));
		}
		//else if (type == SENSOR_TYPE_HUMIDITY)
			//result.set((String) getIDs().get(12));
		else if (type.equalsIgnoreCase(SENSOR_TYPE_GRAVITY) && (mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY) != null))
		{
            result.set((String) getIDs().get(13));
		}
		else if (type.equalsIgnoreCase(SENSOR_TYPE_LINEAR_ACCELERATION) && (mSensorManager.getDefaultSensor(Sensor.TYPE_LINEAR_ACCELERATION) != null))
		{
            result.set((String) getIDs().get(14));
		}
		else if (type.equalsIgnoreCase(SENSOR_TYPE_ROTATION) && (mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR) != null))
		{
            result.set((String) getIDs().get(15));
		}
		else if (type.equalsIgnoreCase(SENSOR_TYPE_ORIENTATION) && (mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION) != null))
		{
            result.set((String) getIDs().get(16));
		}
		else
		{
			//result.setError("Sensor type " + type + " not supported on Android");
			Logger.E(TAG, "Sensor type " + type + " not supported on Android");
		}
		
	}

	@Override
	public void enumerate(IMethodResult result)
	{
		result.set(getIDs());		
	}
}