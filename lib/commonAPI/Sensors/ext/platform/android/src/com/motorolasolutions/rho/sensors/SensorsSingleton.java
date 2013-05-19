package com.motorolasolutions.rho.sensors;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import com.rho.sensors.ISensorsSingleton;
import com.rho.sensors.SensorsSingletonBase;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;

class SensorsSingleton extends SensorsSingletonBase implements ISensorsSingleton {	

	private String TAG = "Rho::" + SensorsSingleton.class.getSimpleName();  

	private enum SENSOR
	{
		accelerometer,
		tiltAngle,
		deviceOrientation,
		motion,
		eCompass,
		magnetometer,
		gyroscope,
		ambientLight,
		proximity,
		proximityLongRange,
		pressure,
		temperature,
		humidity,
		gravity,
		linearAcceleration,
		rotation,
		orientation;
	}

	private static final int size = SENSOR.values().length;
	/**
	 * Sensor Manager variable for enabling/disabling all the sensors
	 */
	private SensorManager mSensorManager;	
	/**
	 * Sensor Navigation URLs for all the sensors
	 */
	private static IMethodResult[] sensorUrl = new IMethodResult[size];

	/**
	 * Minimum Interval between any two sensor event updates
	 */
	private static int[] minimumInterval = new int[size];

	/**
	 * Variable that records the event sending data to avoid sending another event before minimum interval
	 */
	private static long[] dwQuietStart = new long[size];		

	private final SensorEventListener accelerometerListener = new SensorEventListener() 
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1) {}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			accelerometerSensorChanged(arg0);
		}
	};

	private final SensorEventListener magnetometerListener = new SensorEventListener() 
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1) {}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			magnetometerSensorChanged(arg0);
		}
	};

	private final SensorEventListener gyroscopeListener = new SensorEventListener() 
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1) {}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			gyroscopeSensorChanged(arg0);
		}
	};

	private final SensorEventListener ambientlightListener = new SensorEventListener() 
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1) {}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			ambientlightSensorChanged(arg0);
		}
	};

	private final SensorEventListener proximityListener = new SensorEventListener() 
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1) {}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			proximitySensorChanged(arg0);
		}
	};

	private final SensorEventListener pressureListener = new SensorEventListener() 
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1) {}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			pressureSensorChanged(arg0);
		}
	};

	private final SensorEventListener temperatureListener = new SensorEventListener() 
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1) {}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			temperatureSensorChanged(arg0);
		}
	};	


	private final SensorEventListener gravityListener = new SensorEventListener() 
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1) {}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			gravitySensorChanged(arg0);
		}
	};	


	private final SensorEventListener linearListener = new SensorEventListener() 
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1) {}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			linearSensorChanged(arg0);
		}
	};	


	private final SensorEventListener rotationListener = new SensorEventListener() 
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1) {}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			rotationSensorChanged(arg0);
		}
	};	


	private final SensorEventListener orientationListener = new SensorEventListener() 
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1) {}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			orientationSensorChanged(arg0);
		}
	};

	/**
	 * Handle the data from Accelerometer sensor
	 * @param event
	 */
	private void accelerometerSensorChanged(SensorEvent event) {

		Map<String, Object> props = new HashMap<String, Object>();
		double accel_x = event.values[0];
		double accel_y = event.values[1];
		double accel_z = event.values[2];	

		//Logger.D(TAG, "Sensor change detected");
		if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER)
		{
			//Logger.D(TAG, "Accelerometer Sensor change detected");
			if (sensorUrl[SENSOR.accelerometer.ordinal()] != null)
			{
				//Logger.D(TAG, "Acceleromert fire the event");
				//get time from the last event
				long elapsed = System.currentTimeMillis() - dwQuietStart[SENSOR.accelerometer.ordinal()];

				if (elapsed >= minimumInterval[SENSOR.accelerometer.ordinal()])
				{
					//Logger.D(TAG, "Accelerometer minimum interval surpassed");
					props.put("x", String.valueOf(accel_x));
					props.put("y", String.valueOf(accel_y));
					props.put("z", String.valueOf(accel_z));	

					sensorUrl[SENSOR.accelerometer.ordinal()].set(props);
					Logger.T(TAG, "Accelerometer event fired: "+ sensorUrl[SENSOR.accelerometer.ordinal()]+":" + props);
					dwQuietStart[SENSOR.accelerometer.ordinal()] = System.currentTimeMillis();	
				}				
			}		

			if (sensorUrl[SENSOR.tiltAngle.ordinal()] != null)
			{
				//get time from the last event
				long elapsed = System.currentTimeMillis() - dwQuietStart[SENSOR.tiltAngle.ordinal()];

				if (elapsed >= minimumInterval[SENSOR.tiltAngle.ordinal()])
				{
					double PI = 3.14159265;				
					// This algorithm is taken from the Windows Sensor API
					double tilt_y =  (Math.atan( accel_x/Math.sqrt( accel_y*accel_y + accel_z*accel_z ) ) * 180 / PI);
					double tilt_x =  (Math.atan( accel_y/Math.sqrt( accel_x*accel_x + accel_z*accel_z ) ) * 180 / PI);        
					double tilt_z =  (Math.atan( accel_z/Math.sqrt( accel_x*accel_x + accel_y*accel_y ) ) * 180 / PI);  

					props.put("x", String.valueOf(tilt_x));
					props.put("y", String.valueOf(tilt_y));
					props.put("z", String.valueOf(tilt_z));	

					sensorUrl[SENSOR.tiltAngle.ordinal()].set(props);
					dwQuietStart[SENSOR.tiltAngle.ordinal()] = System.currentTimeMillis();
				}					
			}	       
		}
	}

	/**
	 * Handle data from Magnetic sensor
	 * @param event
	 */
	private void magnetometerSensorChanged(SensorEvent event) {

		Map<String, Object> props = new HashMap<String, Object>();

		if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD)
		{        		
			if (sensorUrl[SENSOR.magnetometer.ordinal()] != null)
			{
				//get time from the last event
				long elapsed = System.currentTimeMillis() - dwQuietStart[SENSOR.magnetometer.ordinal()];

				if (elapsed >= minimumInterval[SENSOR.magnetometer.ordinal()])
				{
					props.put("x", String.valueOf(event.values[0]));
					props.put("y", String.valueOf(event.values[1]));
					props.put("z", String.valueOf(event.values[2]));	

					sensorUrl[SENSOR.magnetometer.ordinal()].set(props);
					dwQuietStart[SENSOR.magnetometer.ordinal()] = System.currentTimeMillis();	
				}
			}	
		}		
	}

	/**
	 * handle data from Gyroscope sensor
	 * @param event
	 */
	private void gyroscopeSensorChanged(SensorEvent event) {

		Map<String, Object> props = new HashMap<String, Object>();

		if (event.sensor.getType() == Sensor.TYPE_GYROSCOPE)
		{        		
			if (sensorUrl[SENSOR.gyroscope.ordinal()] != null)
			{
				//get time from the last event
				long elapsed = System.currentTimeMillis() - dwQuietStart[SENSOR.gyroscope.ordinal()];

				if (elapsed >= minimumInterval[SENSOR.gyroscope.ordinal()])
				{
					props.put("x", String.valueOf(event.values[0]));
					props.put("y", String.valueOf(event.values[1]));
					props.put("z", String.valueOf(event.values[2]));	

					sensorUrl[SENSOR.gyroscope.ordinal()].set(props);
					dwQuietStart[SENSOR.gyroscope.ordinal()] = System.currentTimeMillis();	
				}				
			}	
		}
	}

	/**
	 * Handle data from Ambient light sensor
	 * @param event
	 */
	private void ambientlightSensorChanged(SensorEvent event) {		

		if (event.sensor.getType() == Sensor.TYPE_LIGHT)
		{        		
			if (sensorUrl[SENSOR.ambientLight.ordinal()] != null)
			{
				//get time from the last event
				long elapsed = System.currentTimeMillis() - dwQuietStart[SENSOR.ambientLight.ordinal()];

				if (elapsed >= minimumInterval[SENSOR.ambientLight.ordinal()])
				{
					sensorUrl[SENSOR.ambientLight.ordinal()].set(String.valueOf(event.values[0]));
					dwQuietStart[SENSOR.ambientLight.ordinal()] = System.currentTimeMillis();	
				}				
			}	
		}
	}

	/**
	 * handle data from the proximity sensor
	 * @param event
	 */
	private void proximitySensorChanged(SensorEvent event) {

		if (event.sensor.getType() == Sensor.TYPE_PROXIMITY)
		{        		
			if (sensorUrl[SENSOR.proximity.ordinal()] != null)
			{
				//get time from the last event
				long elapsed = System.currentTimeMillis() - dwQuietStart[SENSOR.proximity.ordinal()];

				if (elapsed >= minimumInterval[SENSOR.proximity.ordinal()])
				{
					sensorUrl[SENSOR.proximity.ordinal()].set(String.valueOf(event.values[0]));
					dwQuietStart[SENSOR.proximity.ordinal()] = System.currentTimeMillis();	
				}				
			}	
		}
	}

	/**
	 * Handle data from the pressure sensor
	 * @param event
	 */
	private void pressureSensorChanged(SensorEvent event) {

		if (event.sensor.getType() == Sensor.TYPE_PRESSURE)
		{        		
			if (sensorUrl[SENSOR.pressure.ordinal()] != null)
			{
				//get time from the last event
				long elapsed = System.currentTimeMillis() - dwQuietStart[SENSOR.pressure.ordinal()];

				if (elapsed >= minimumInterval[SENSOR.pressure.ordinal()])
				{
					sensorUrl[SENSOR.pressure.ordinal()].set(String.valueOf(event.values[0]));
					dwQuietStart[SENSOR.pressure.ordinal()] = System.currentTimeMillis();	
				}				
			}	
		}
	}

	/**
	 * Handle data from the temperature sensor
	 * @param event
	 */
	private void temperatureSensorChanged(SensorEvent event) {

		if (event.sensor.getType() == Sensor.TYPE_TEMPERATURE)
		{        		
			if (sensorUrl[SENSOR.temperature.ordinal()] != null)
			{
				//get time from the last event
				long elapsed = System.currentTimeMillis() - dwQuietStart[SENSOR.temperature.ordinal()];

				if (elapsed >= minimumInterval[SENSOR.temperature.ordinal()])
				{
					sensorUrl[SENSOR.temperature.ordinal()].set(String.valueOf(event.values[0]));
					dwQuietStart[SENSOR.temperature.ordinal()] = System.currentTimeMillis();	
				}				
			}	
		}
	}

	/**
	 * Handle data from the gravity sensor
	 * @param event
	 */
	private void gravitySensorChanged(SensorEvent event) {
		Map<String, Object> props = new HashMap<String, Object>();

		if (event.sensor.getType() == Sensor.TYPE_GRAVITY)
		{        		
			if (sensorUrl[SENSOR.gravity.ordinal()] != null)
			{
				//get time from the last event
				long elapsed = System.currentTimeMillis() - dwQuietStart[SENSOR.gravity.ordinal()];

				if (elapsed >= minimumInterval[SENSOR.gravity.ordinal()])
				{
					props.put("x", String.valueOf(event.values[0]));
					props.put("y", String.valueOf(event.values[1]));
					props.put("z", String.valueOf(event.values[2]));	

					sensorUrl[SENSOR.gravity.ordinal()].set(props);
					dwQuietStart[SENSOR.gravity.ordinal()] = System.currentTimeMillis();	
				}				
			}	
		}
	}

	/**
	 * Handle data from the linear acceleration sensor
	 * @param event
	 */
	private void linearSensorChanged(SensorEvent event) {

		Map<String, Object> props = new HashMap<String, Object>();

		if (event.sensor.getType() == Sensor.TYPE_LINEAR_ACCELERATION)
		{        		
			if (sensorUrl[SENSOR.linearAcceleration.ordinal()] != null)
			{
				//get time from the last event
				long elapsed = System.currentTimeMillis() - dwQuietStart[SENSOR.linearAcceleration.ordinal()];

				if (elapsed >= minimumInterval[SENSOR.linearAcceleration.ordinal()])
				{
					props.put("x", String.valueOf(event.values[0]));
					props.put("y", String.valueOf(event.values[1]));
					props.put("z", String.valueOf(event.values[2]));	

					sensorUrl[SENSOR.linearAcceleration.ordinal()].set(props);
					dwQuietStart[SENSOR.linearAcceleration.ordinal()] = System.currentTimeMillis();	
				}				
			}	
		}
	}

	/**
	 * handle data from the rotation sensor
	 * @param event
	 */
	private void rotationSensorChanged(SensorEvent event) {

		Map<String, Object> props = new HashMap<String, Object>();

		if (event.sensor.getType() == Sensor.TYPE_ROTATION_VECTOR)
		{        		
			if (sensorUrl[SENSOR.rotation.ordinal()] != null)
			{
				//get time from the last event
				long elapsed = System.currentTimeMillis() - dwQuietStart[SENSOR.rotation.ordinal()];

				if (elapsed >= minimumInterval[SENSOR.rotation.ordinal()])
				{
					props.put("x", String.valueOf(event.values[0]));
					props.put("y", String.valueOf(event.values[1]));
					props.put("z", String.valueOf(event.values[2]));	

					sensorUrl[SENSOR.rotation.ordinal()].set(props);
					dwQuietStart[SENSOR.rotation.ordinal()] = System.currentTimeMillis();	
				}				
			}	
		}
	}

	/**
	 * handle data from the orientation sensor
	 * @param event
	 */
	private void orientationSensorChanged(SensorEvent event) {		

		Map<String, Object> props = new HashMap<String, Object>();

		if (event.sensor.getType() == Sensor.TYPE_ORIENTATION)
		{        		
			if (sensorUrl[SENSOR.orientation.ordinal()] != null)
			{
				//get time from the last event
				long elapsed = System.currentTimeMillis() - dwQuietStart[SENSOR.orientation.ordinal()];

				if (elapsed >= minimumInterval[SENSOR.orientation.ordinal()])
				{
					props.put("x", String.valueOf(event.values[0]));
					props.put("y", String.valueOf(event.values[1]));
					props.put("z", String.valueOf(event.values[2]));	

					sensorUrl[SENSOR.orientation.ordinal()].set(props);
					dwQuietStart[SENSOR.orientation.ordinal()] = System.currentTimeMillis();	
				}				
			}	
		}
	}

	public SensorsSingleton(SensorsFactory factory) {
		super();
		mSensorManager = (SensorManager) RhodesActivity.safeGetInstance().getSystemService("sensor");	

		for (SENSOR s : SENSOR.values())
		{
			sensorUrl[s.ordinal()] = null;
			dwQuietStart[s.ordinal()] = System.currentTimeMillis();        	
			minimumInterval[s.ordinal()] = 200; //Default - 200 milli seconds
		}

		// List the available sensors
		List<Sensor> sensors = mSensorManager.getSensorList(Sensor.TYPE_ALL);
		for (Sensor sensor : sensors) 
		{
			Logger.I(TAG, "Available Sensor: " + sensor.getName());	    		       
		}
	}

	List<String> getIDs() {
		List<String> ids = new LinkedList<String>();
		ids.add("SCN1");
		ids.add("SCN2");
		return ids;
	}

	@Override
	public void watchAccelerometer(int minimumGap, IMethodResult result)
	{
		if (minimumGap == 0)
		{
			//return data immediately
			//TODO: Currently not possible in Android
			//it isn't possible to directly get the current sensor value and, while some more recent versions of Android 
			//will generate an onSensorChanged event as soon as the listener is registered (allowing you to get a baseline), 
			//earlier versions don't.
			result.setError("Android platform doesn't allow immediate sensor data retrieval so please specify a callback and non zero minimumGap value");
		}
		else
		{
			minimumInterval[SENSOR.accelerometer.ordinal()] = minimumGap;	
			sensorUrl[SENSOR.accelerometer.ordinal()] = result;		
			mSensorManager.registerListener(accelerometerListener, mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
			Logger.D(TAG, "Accelerometer sensor registered successfully");
		}
	}

	@Override
	public void stopAccelerometer(IMethodResult result)
	{		
		mSensorManager.unregisterListener(accelerometerListener);
		Logger.D(TAG, "Accelerometer sensor unregistered successfully");
	}

	@Override
	public void watchTiltAngle(int minimumGap, IMethodResult result)
	{
		if (minimumGap == 0)
		{
			//return data immediately
			//TODO: Currently not possible in Android
			//it isn't possible to directly get the current sensor value and, while some more recent versions of Android 
			//will generate an onSensorChanged event as soon as the listener is registered (allowing you to get a baseline), 
			//earlier versions don't.
			result.setError("Android platform doesn't allow immediate sensor data retrieval so please specify a callback and non zero minimumGap value");
		}
		else
		{
			minimumInterval[SENSOR.tiltAngle.ordinal()] = minimumGap;	
			sensorUrl[SENSOR.tiltAngle.ordinal()] = result;		
			mSensorManager.registerListener(accelerometerListener, mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
			Logger.D(TAG, "tiltAngle sensor registered successfully");
		}		
	}

	@Override
	public void stopTiltAngle(IMethodResult result)
	{
		mSensorManager.unregisterListener(accelerometerListener);
		Logger.D(TAG, "tiltAngle sensor unregistered successfully");		
	}

	@Override
	public void watchDeviceOrientation(int minimumGap, IMethodResult result)
	{
		result.setError("device orientation sensor not supported on Android platform");		
	}

	@Override
	public void stopDeviceOrientation(IMethodResult result)
	{
		result.setError("device orientation sensor not supported on Android platform");		
	}

	@Override
	public void watchMotion(int minimumGap, IMethodResult result)
	{
		result.setError("motion sensor not supported on Android platform");		
	}

	@Override
	public void stopMotion(IMethodResult result)
	{
		result.setError("motion sensor not supported on Android platform");	
	}

	@Override
	public void watchECompass(int minimumGap, IMethodResult result)
	{
		result.setError("eCompass sensor not supported on Android platform");	
	}

	@Override
	public void stopECompass(IMethodResult result)
	{
		result.setError("eCompass sensor not supported on Android platform");	
	}

	@Override
	public void watchMagnetometer(int minimumGap, IMethodResult result)
	{
		if (minimumGap == 0)
		{
			//return data immediately
			//TODO: Currently not possible in Android
			//it isn't possible to directly get the current sensor value and, while some more recent versions of Android 
			//will generate an onSensorChanged event as soon as the listener is registered (allowing you to get a baseline), 
			//earlier versions don't.
			result.setError("Android platform doesn't allow immediate sensor data retrieval so please specify a callback and non zero minimumGap value");
		}
		else
		{
			minimumInterval[SENSOR.magnetometer.ordinal()] = minimumGap;	
			sensorUrl[SENSOR.magnetometer.ordinal()] = result;		
			mSensorManager.registerListener(magnetometerListener, 	mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD),SensorManager.SENSOR_DELAY_NORMAL);
			Logger.D(TAG, "magnetometer sensor registered successfully");
		}
	}

	@Override
	public void stopMagnetometer(IMethodResult result)
	{
		mSensorManager.unregisterListener(magnetometerListener);
		Logger.D(TAG, "magnetometer sensor unregistered successfully");	
	}

	@Override
	public void watchGyroscope(int minimumGap, IMethodResult result)
	{
		if (minimumGap == 0)
		{
			//return data immediately
			//TODO: Currently not possible in Android
			//it isn't possible to directly get the current sensor value and, while some more recent versions of Android 
			//will generate an onSensorChanged event as soon as the listener is registered (allowing you to get a baseline), 
			//earlier versions don't.
			result.setError("Android platform doesn't allow immediate sensor data retrieval so please specify a callback and non zero minimumGap value");
		}
		else
		{
			minimumInterval[SENSOR.gyroscope.ordinal()] = minimumGap;	
			sensorUrl[SENSOR.gyroscope.ordinal()] = result;		
			mSensorManager.registerListener(gyroscopeListener, 		mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE), 	SensorManager.SENSOR_DELAY_NORMAL);
			Logger.D(TAG, "gyroscope sensor registered successfully");
		}
	}

	@Override
	public void stopGyroscope(IMethodResult result)
	{
		mSensorManager.unregisterListener(gyroscopeListener);
		Logger.D(TAG, "gyroscope sensor unregistered successfully");	
	}

	@Override
	public void watchAmbientLight(int minimumGap, IMethodResult result)
	{
		if (minimumGap == 0)
		{
			//return data immediately
			//TODO: Currently not possible in Android
			//it isn't possible to directly get the current sensor value and, while some more recent versions of Android 
			//will generate an onSensorChanged event as soon as the listener is registered (allowing you to get a baseline), 
			//earlier versions don't.
			result.setError("Android platform doesn't allow immediate sensor data retrieval so please specify a callback and non zero minimumGap value");
		}
		else
		{
			minimumInterval[SENSOR.ambientLight.ordinal()] = minimumGap;	
			sensorUrl[SENSOR.ambientLight.ordinal()] = result;		
			mSensorManager.registerListener(ambientlightListener, 	mSensorManager.getDefaultSensor(Sensor.TYPE_LIGHT), 		SensorManager.SENSOR_DELAY_NORMAL);
			Logger.D(TAG, "ambientLight sensor registered successfully");
		}
	}

	@Override
	public void stopAmbientLight(IMethodResult result)
	{
		mSensorManager.unregisterListener(ambientlightListener);
		Logger.D(TAG, "ambientLight sensor unregistered successfully");	
	}

	@Override
	public void watchProximity(int minimumGap, IMethodResult result)
	{
		if (minimumGap == 0)
		{
			//return data immediately
			//TODO: Currently not possible in Android
			//it isn't possible to directly get the current sensor value and, while some more recent versions of Android 
			//will generate an onSensorChanged event as soon as the listener is registered (allowing you to get a baseline), 
			//earlier versions don't.
			result.setError("Android platform doesn't allow immediate sensor data retrieval so please specify a callback and non zero minimumGap value");
		}
		else
		{
			minimumInterval[SENSOR.proximity.ordinal()] = minimumGap;	
			sensorUrl[SENSOR.proximity.ordinal()] = result;		
			mSensorManager.registerListener(proximityListener, 		mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY), 	SensorManager.SENSOR_DELAY_NORMAL);
			Logger.D(TAG, "proximity sensor registered successfully");
		}
	}

	@Override
	public void stopProximity(IMethodResult result)
	{
		mSensorManager.unregisterListener(proximityListener);
		Logger.D(TAG, "proximity sensor unregistered successfully");	
	}

	@Override
	public void watchProximityLongRange(int minimumGap, IMethodResult result)
	{
		result.setError("proximity long range sensor not supported on Android platform");	
	}

	@Override
	public void stopProximityLongRange(IMethodResult result)
	{
		result.setError("proximity long range sensor not supported on Android platform");	
	}

	@Override
	public void watchPressure(int minimumGap, IMethodResult result)
	{
		if (minimumGap == 0)
		{
			//return data immediately
			//TODO: Currently not possible in Android
			//it isn't possible to directly get the current sensor value and, while some more recent versions of Android 
			//will generate an onSensorChanged event as soon as the listener is registered (allowing you to get a baseline), 
			//earlier versions don't.
			result.setError("Android platform doesn't allow immediate sensor data retrieval so please specify a callback and non zero minimumGap value");
		}
		else
		{
			minimumInterval[SENSOR.pressure.ordinal()] = minimumGap;	
			sensorUrl[SENSOR.pressure.ordinal()] = result;		
			mSensorManager.registerListener(pressureListener, 		mSensorManager.getDefaultSensor(Sensor.TYPE_PRESSURE), 		SensorManager.SENSOR_DELAY_NORMAL);
			Logger.D(TAG, "pressure sensor registered successfully");
		}
	}

	@Override
	public void stopPressure(IMethodResult result)
	{
		mSensorManager.unregisterListener(pressureListener);
		Logger.D(TAG, "pressure sensor unregistered successfully");	
	}

	@Override
	public void watchTemperature(int minimumGap, IMethodResult result)
	{
		if (minimumGap == 0)
		{
			//return data immediately
			//TODO: Currently not possible in Android
			//it isn't possible to directly get the current sensor value and, while some more recent versions of Android 
			//will generate an onSensorChanged event as soon as the listener is registered (allowing you to get a baseline), 
			//earlier versions don't.
			result.setError("Android platform doesn't allow immediate sensor data retrieval so please specify a callback and non zero minimumGap value");
		}
		else
		{
			minimumInterval[SENSOR.temperature.ordinal()] = minimumGap;	
			sensorUrl[SENSOR.temperature.ordinal()] = result;		
			mSensorManager.registerListener(temperatureListener, 	mSensorManager.getDefaultSensor(Sensor.TYPE_TEMPERATURE),	SensorManager.SENSOR_DELAY_NORMAL);
			Logger.D(TAG, "temperature sensor registered successfully");
		}
	}

	@Override
	public void stopTemperature(IMethodResult result)
	{
		mSensorManager.unregisterListener(temperatureListener);	
		Logger.D(TAG, "temperature sensor unregistered successfully");	
	}

	@Override
	public void watchHumidity(int minimumGap, IMethodResult result)
	{
		result.setError("humidity sensor not supported on Android platform");	
	}

	@Override
	public void stopHumidity(IMethodResult result)
	{
		result.setError("humidity sensor not supported on Android platform");	
	}

	@Override
	public void watchGravity(int minimumGap, IMethodResult result)
	{
		if (minimumGap == 0)
		{
			//return data immediately
			//TODO: Currently not possible in Android
			//it isn't possible to directly get the current sensor value and, while some more recent versions of Android 
			//will generate an onSensorChanged event as soon as the listener is registered (allowing you to get a baseline), 
			//earlier versions don't.
			result.setError("Android platform doesn't allow immediate sensor data retrieval so please specify a callback and non zero minimumGap value");
		}
		else
		{
			minimumInterval[SENSOR.gravity.ordinal()] = minimumGap;	
			sensorUrl[SENSOR.gravity.ordinal()] = result;		
			mSensorManager.registerListener(gravityListener, 		mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY), 		SensorManager.SENSOR_DELAY_NORMAL);
			Logger.D(TAG, "gravity sensor registered successfully");
		}
	}

	@Override
	public void stopGravity(IMethodResult result)
	{
		mSensorManager.unregisterListener(gravityListener);	
		Logger.D(TAG, "gravity sensor unregistered successfully");	
	}

	@Override
	public void watchLinearAcceleration(int minimumGap, IMethodResult result)
	{
		if (minimumGap == 0)
		{
			//return data immediately
			//TODO: Currently not possible in Android
			//it isn't possible to directly get the current sensor value and, while some more recent versions of Android 
			//will generate an onSensorChanged event as soon as the listener is registered (allowing you to get a baseline), 
			//earlier versions don't.
			result.setError("Android platform doesn't allow immediate sensor data retrieval so please specify a callback and non zero minimumGap value");
		}
		else
		{
			minimumInterval[SENSOR.linearAcceleration.ordinal()] = minimumGap;	
			sensorUrl[SENSOR.linearAcceleration.ordinal()] = result;		
			mSensorManager.registerListener(linearListener, 		mSensorManager.getDefaultSensor(Sensor.TYPE_LINEAR_ACCELERATION),SensorManager.SENSOR_DELAY_NORMAL);
			Logger.D(TAG, "linearAcceleration sensor registered successfully");
		}
	}

	@Override
	public void stopLinearAcceleration(IMethodResult result)
	{
		mSensorManager.unregisterListener(linearListener);	
		Logger.D(TAG, "linearAcceleration sensor unregistered successfully");	
	}

	@Override
	public void watchRotation(int minimumGap, IMethodResult result)
	{
		if (minimumGap == 0)
		{
			//return data immediately
			//TODO: Currently not possible in Android
			//it isn't possible to directly get the current sensor value and, while some more recent versions of Android 
			//will generate an onSensorChanged event as soon as the listener is registered (allowing you to get a baseline), 
			//earlier versions don't.
			result.setError("Android platform doesn't allow immediate sensor data retrieval so please specify a callback and non zero minimumGap value");
		}
		else
		{
			minimumInterval[SENSOR.rotation.ordinal()] = minimumGap;	
			sensorUrl[SENSOR.rotation.ordinal()] = result;		
			mSensorManager.registerListener(rotationListener, 		mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR), 		SensorManager.SENSOR_DELAY_NORMAL);
			Logger.D(TAG, "rotation sensor registered successfully");
		}
	}

	@Override
	public void stopRotation(IMethodResult result)
	{
		mSensorManager.unregisterListener(rotationListener);		
		Logger.D(TAG, "rotation sensor unregistered successfully");	
	}

	@Override
	public void watchOrientation(int minimumGap, IMethodResult result)
	{
		if (minimumGap == 0)
		{
			//return data immediately
			//TODO: Currently not possible in Android
			//it isn't possible to directly get the current sensor value and, while some more recent versions of Android 
			//will generate an onSensorChanged event as soon as the listener is registered (allowing you to get a baseline), 
			//earlier versions don't.
			result.setError("Android platform doesn't allow immediate sensor data retrieval so please specify a callback and non zero minimumGap value");
		}
		else
		{
			minimumInterval[SENSOR.orientation.ordinal()] = minimumGap;	
			sensorUrl[SENSOR.orientation.ordinal()] = result;		
			mSensorManager.registerListener(orientationListener, 	mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION), 	SensorManager.SENSOR_DELAY_NORMAL);
			Logger.D(TAG, "orientation sensor registered successfully");
		}
	}

	@Override
	public void stopOrientation(IMethodResult result)
	{
		mSensorManager.unregisterListener(orientationListener);	
		Logger.D(TAG, "orientation sensor unregistered successfully");	
	}
}