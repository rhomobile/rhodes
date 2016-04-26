/**
 * RawSensor plugin returns the raw values from the sensors
 */
package com.rho.rhoelements.plugins;

import java.util.List;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;

/**
 * @author pxb743
 * The RawSensors Module is used to retrieve the raw data values of the specified sensors.
 */
public class RawSensorsPlugin extends Plugin {

	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onSetting(com.rho.rhoelements.PluginSetting)
	 */
	
	/**
	 * Sensor Manager variable for enabling/disabling all the sensors
	 */
	private SensorManager mSensorManager;	
	/**
	 * Sensor Navigation URL
	 */
	private static String sensorUrl = null;
	/**
	 * Minimum Interval between any two sensor event updates
	 */
	private int minimumInterval = 1000;
	/**
	 * Variable that records the event sending data to avopid sending another before minimum interval
	 */
	private long dwQuietStart;		
	
	private Boolean accelEnabled = false;
	private Boolean tiltEnabled = false;
	
	private int noOfEntries = 33;
	/**
	 *  event names corresponding to navigate name value pairs
	 */
	private static final String[] NAMES = new String[] 
	{
		"accelerometerX",	// X co-ordinate value of the Accelerometer sensor
		"accelerometerY",	// Y co-ordinate value of the Accelerometer sensor
		"accelerometerZ",	// Z co-ordinate value of the Accelerometer sensor
		"deviceOrientation",// value of the orientation sensor
		"tiltangleX",		// X co-ordinate value of the tiltangle sensor
		"tiltangleY",		// Y co-ordinate value of the tiltangle sensor
		"tiltangleZ",		// Z co-ordinate value of the tiltangle sensor
		"motion",			// value of the motion sensor
		"ecompass",			// value of the ecompass sensor
		"magnetometerX",	// X value of the magnetometer sensor
		"magnetometerY",	// Y value of the magnetometer sensor
		"magnetometerZ",	// Z value of the magnetometer sensor
		"gyroscopeX",		// X co-ordinate value of the gyroscope sensor
		"gyroscopeY",		// Y co-ordinate value of the gyroscope sensor
		"gyroscopeZ",		// Z co-ordinate value of the gyroscope sensor
		"ambientLight",		// value of the ambient Light sensor
		"proximity",		// value of the proximity sensor
		"proximitylongrange",	// value of the proximitylongrange sensor
		"pressure",			// value of the pressure sensor
		"temperature",		// value of the temperature sensor
		"humidity",			// value of the humidity sensor
		"gravityX",			// X co-ordinate value of the gravity sensor
		"gravityY",			// Y co-ordinate value of the gravity sensor
		"gravityZ",			// Z co-ordinate value of the gravity sensor
		"linearAccelerationX",// X co-ordinate value of the linear acceleration sensor
		"linearAccelerationY",// Y co-ordinate value of the linear acceleration sensor
		"linearAccelerationZ",// Z co-ordinate value of the linear acceleration sensor
		"rotationX",		// X co-ordinate value of the rotation sensor
		"rotationY",		// Y co-ordinate value of the rotation sensor
		"rotationZ",		// Z co-ordinate value of the rotation sensor
		"orientationX",		// X co-ordinate value of the orientation sensor
		"orientationY",		// Y co-ordinate value of the orientation sensor
		"orientationZ"		// Z co-ordinate value of the orientation sensor
	};
	
	/**
	 * Values array that provide the attribute values for the above array
	 */
	private String[] values = new String[noOfEntries];
	
	/**
	 * Constructor
	 */
	public RawSensorsPlugin()
	{
		mSensorManager = (SensorManager) Common.mainActivity.getSystemService("sensor");	
		dwQuietStart = System.currentTimeMillis();
		for (int i=0;i<noOfEntries;i++)
		{
			values[i] = "";
		}
		
		// List the available sensors
		List<Sensor> sensors = mSensorManager.getSensorList(Sensor.TYPE_ALL);
	    for (Sensor sensor : sensors) 
	    {
	    	Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Available Sensor: " + sensor.getName()));	       
	    }
	}
	
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
		
		try
		{
			if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER)
			{
				double PI = 3.14159265;
				
				double accel_x = event.values[0];
		        double accel_y = event.values[1];
		        double accel_z = event.values[2];        
		     
		        // This algorithm is taken from the Windows Sensor API
		        double tilt_y =  (Math.atan( accel_x/Math.sqrt( accel_y*accel_y + accel_z*accel_z ) ) * 180 / PI);
		        double tilt_x =  (Math.atan( accel_y/Math.sqrt( accel_x*accel_x + accel_z*accel_z ) ) * 180 / PI);        
		        double tilt_z =  (Math.atan( accel_z/Math.sqrt( accel_x*accel_x + accel_y*accel_y ) ) * 180 / PI);      
		        
		        if (accelEnabled)
		        {
					values[0] = String.valueOf(accel_x);
					values[1] = String.valueOf(accel_y);
					values[2] = String.valueOf(accel_z);
		        }
				
		        if (tiltEnabled)
		        {
					values[4] = String.valueOf(tilt_x);
					values[5] = String.valueOf(tilt_y);
					values[6] = String.valueOf(tilt_z);
		        }
			}			
			
			//get time from the last event
			long elapsed = System.currentTimeMillis() - dwQuietStart;
	
			if (elapsed >= minimumInterval)
			{
				navigate(sensorUrl, NAMES, values);					
				dwQuietStart = System.currentTimeMillis();			
			} 
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Accelerometer/Tilt Navigate Exception.. length is " + e.GetLength()));
		}
    }
	
	/**
	 * Handle data from Magnetic sensor
	 * @param event
	 */
	private void magnetometerSensorChanged(SensorEvent event) {
		
		try
		{
			if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD)
			{        		
				values[9]  = String.valueOf(event.values[0]);
				values[10] = String.valueOf(event.values[1]);
				values[11] = String.valueOf(event.values[2]);
			}				
			//get time from the last event
			long elapsed = System.currentTimeMillis() - dwQuietStart;
	
			if (elapsed >= minimumInterval)
			{
				navigate(sensorUrl, NAMES, values);			
				dwQuietStart = System.currentTimeMillis();			
			} 
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Magnetometer Navigate Exception.. length is " + e.GetLength()));
		}
    }
	
	/**
	 * handle data from Gyroscope sensor
	 * @param event
	 */
	private void gyroscopeSensorChanged(SensorEvent event) {
		
		try
		{
			if (event.sensor.getType() == Sensor.TYPE_GYROSCOPE)
			{        		
				values[12] = String.valueOf(event.values[0]);
				values[13] = String.valueOf(event.values[1]);
				values[14] = String.valueOf(event.values[2]);
			}			
			
			//get time from the last event
			long elapsed = System.currentTimeMillis() - dwQuietStart;
	
			if (elapsed >= minimumInterval)
			{				
				navigate(sensorUrl, NAMES, values);				
				dwQuietStart = System.currentTimeMillis();			
			} 	
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Gyroscope Navigate Exception.. length is " + e.GetLength()));
		}
    }
	
	/**
	 * Handle data from Ambient light sensor
	 * @param event
	 */
	private void ambientlightSensorChanged(SensorEvent event) {
		
		try
		{
			if (event.sensor.getType() == Sensor.TYPE_LIGHT)
			{        		
				values[15] = String.valueOf(event.values[0]);
			}
			
			long elapsed = System.currentTimeMillis() - dwQuietStart;
	
			if (elapsed >= minimumInterval)
			{				
				navigate(sensorUrl, NAMES, values);				
				dwQuietStart = System.currentTimeMillis();			
			} 
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Ambient Light Navigate Exception.. length is " + e.GetLength()));
		}
    }
	
	/**
	 * handle data from the proximity sensor
	 * @param event
	 */
	private void proximitySensorChanged(SensorEvent event) {
		
		try
		{
			if (event.sensor.getType() == Sensor.TYPE_PROXIMITY)
			{        		
				values[16] = String.valueOf(event.values[0]);
			}			
			
			// Calculate time from the last event
			long elapsed = System.currentTimeMillis() - dwQuietStart;
	
			if (elapsed >= minimumInterval)
			{				
				navigate(sensorUrl, NAMES, values);				
				dwQuietStart = System.currentTimeMillis();			
			} 
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Proximity Navigate Exception.. length is " + e.GetLength()));
		}
    }
	
	/**
	 * Handle data from the pressure sensor
	 * @param event
	 */
	private void pressureSensorChanged(SensorEvent event) {
		
		try
		{
			if (event.sensor.getType() == Sensor.TYPE_PRESSURE)
			{        		
				values[18] = String.valueOf(event.values[0]);
			}		
			
			//Time from the last event
			long elapsed = System.currentTimeMillis() - dwQuietStart;
	
			if (elapsed >= minimumInterval)
			{			
				navigate(sensorUrl, NAMES, values);				
				dwQuietStart = System.currentTimeMillis();			
			} 
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Pressure Navigate Exception.. length is " + e.GetLength()));
		}
    }
	
	/**
	 * Handle data from the temperature sensor
	 * @param event
	 */
	private void temperatureSensorChanged(SensorEvent event) {
		
		try
		{
		if (event.sensor.getType() == Sensor.TYPE_TEMPERATURE)
		{        		
			values[19] = String.valueOf(event.values[0]);
		}			
		
		//Time from the last event
		long elapsed = System.currentTimeMillis() - dwQuietStart;

		if (elapsed >= minimumInterval)
		{			
			navigate(sensorUrl, NAMES, values);			
			dwQuietStart = System.currentTimeMillis();			
		} 	
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Temperature Navigate Exception.. length is " + e.GetLength()));
		}
    }
	
	/**
	 * Handle data from the gravity sensor
	 * @param event
	 */
	private void gravitySensorChanged(SensorEvent event) {
		try
		{
			if (event.sensor.getType() == Sensor.TYPE_GRAVITY)
			{        		
				values[21] = String.valueOf(event.values[0]);
				values[22] = String.valueOf(event.values[1]);
				values[23] = String.valueOf(event.values[2]);
			}			
			
			//Time from the last event
			long elapsed = System.currentTimeMillis() - dwQuietStart;
	
			if (elapsed >= minimumInterval)
			{				
				navigate(sensorUrl, NAMES, values);				
				dwQuietStart = System.currentTimeMillis();			
			} 
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Gravity Navigate Exception.. length is " + e.GetLength()));
		}
    }
	
	/**
	 * Handle data from the linear acceleration sensor
	 * @param event
	 */
	private void linearSensorChanged(SensorEvent event) {
		
		try
		{
			if (event.sensor.getType() == Sensor.TYPE_LINEAR_ACCELERATION)
			{        		
				values[24] = String.valueOf(event.values[0]);
				values[25] = String.valueOf(event.values[1]);
				values[26] = String.valueOf(event.values[2]);
			}			
			
			//Time from the last event
			long elapsed = System.currentTimeMillis() - dwQuietStart;
	
			if (elapsed >= minimumInterval)
			{				
				navigate(sensorUrl, NAMES, values);				
				dwQuietStart = System.currentTimeMillis();			
			} 	
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Linear acceleration Navigate Exception.. length is " + e.GetLength()));
		}
    }
	
	/**
	 * handle data from the rotation sensor
	 * @param event
	 */
	private void rotationSensorChanged(SensorEvent event) {
		
		try
		{
			if (event.sensor.getType() == Sensor.TYPE_ROTATION_VECTOR)
			{        		
				values[27] = String.valueOf(event.values[0]);
				values[28] = String.valueOf(event.values[1]);
				values[29] = String.valueOf(event.values[2]);
			}			
			
			//Time from the last event
			long elapsed = System.currentTimeMillis() - dwQuietStart;
	
			if (elapsed >= minimumInterval)
			{				
				navigate(sensorUrl, NAMES, values);				
				dwQuietStart = System.currentTimeMillis();			
			} 	
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Rotation Navigate Exception.. length is " + e.GetLength()));
		}
    }
	
	/**
	 * handle data from the orientation sensor
	 * @param event
	 */
	private void orientationSensorChanged(SensorEvent event) {
		
		try
		{
			if (event.sensor.getType() == Sensor.TYPE_ORIENTATION)
			{        		
				values[30] = String.valueOf(event.values[0]);
				values[31] = String.valueOf(event.values[1]);
				values[32] = String.valueOf(event.values[2]);
			}		
			
			//Time from the last event
			long elapsed = System.currentTimeMillis() - dwQuietStart;
	
			if (elapsed >= minimumInterval)
			{				
				navigate(sensorUrl, NAMES, values);				
				dwQuietStart = System.currentTimeMillis();			
			} 
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Orientation Navigate Exception.. length is " + e.GetLength()));
		}
    }
	
	
	@Override
	public void onSetting(PluginSetting setting) {
		
		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'",
					setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));
		
		if (setting.getName().equalsIgnoreCase("all"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{				
				//NOTE: TYPE_ALL option in the sensor doesn't work
				accelEnabled = true;
				tiltEnabled = true;
				mSensorManager.registerListener(accelerometerListener, 	mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);				
				mSensorManager.registerListener(magnetometerListener, 	mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD),SensorManager.SENSOR_DELAY_NORMAL);
				mSensorManager.registerListener(gyroscopeListener, 		mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE), 	SensorManager.SENSOR_DELAY_NORMAL);
				mSensorManager.registerListener(ambientlightListener, 	mSensorManager.getDefaultSensor(Sensor.TYPE_LIGHT), 		SensorManager.SENSOR_DELAY_NORMAL);
				mSensorManager.registerListener(proximityListener, 		mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY), 	SensorManager.SENSOR_DELAY_NORMAL);
				mSensorManager.registerListener(pressureListener, 		mSensorManager.getDefaultSensor(Sensor.TYPE_PRESSURE), 		SensorManager.SENSOR_DELAY_NORMAL);
				mSensorManager.registerListener(temperatureListener, 	mSensorManager.getDefaultSensor(Sensor.TYPE_TEMPERATURE),	SensorManager.SENSOR_DELAY_NORMAL);
				mSensorManager.registerListener(gravityListener, 		mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY), 		SensorManager.SENSOR_DELAY_NORMAL);
				mSensorManager.registerListener(linearListener, 		mSensorManager.getDefaultSensor(Sensor.TYPE_LINEAR_ACCELERATION),SensorManager.SENSOR_DELAY_NORMAL);
				mSensorManager.registerListener(rotationListener, 		mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR), 		SensorManager.SENSOR_DELAY_NORMAL);
				mSensorManager.registerListener(orientationListener, 	mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION), 	SensorManager.SENSOR_DELAY_NORMAL);
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "deviceOrientation, motion, ecompass,proximitylongrange and humidity Sensors not supported on Android platform"));			
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				accelEnabled = false;
				tiltEnabled = false;
				mSensorManager.unregisterListener(accelerometerListener);				
				mSensorManager.unregisterListener(magnetometerListener);
				mSensorManager.unregisterListener(gyroscopeListener);
				mSensorManager.unregisterListener(ambientlightListener);
				mSensorManager.unregisterListener(proximityListener);
				mSensorManager.unregisterListener(pressureListener);
				mSensorManager.unregisterListener(temperatureListener);	
				mSensorManager.unregisterListener(gravityListener);
				mSensorManager.unregisterListener(linearListener);
				mSensorManager.unregisterListener(rotationListener);	
				mSensorManager.unregisterListener(orientationListener);
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "deviceOrientation, motion, ecompass,proximitylongrange and humidity Sensors not supported on Android platform"));
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}		
		else if (setting.getName().equalsIgnoreCase("accelerometer"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				accelEnabled = true;
				mSensorManager.registerListener(accelerometerListener, mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				accelEnabled = false;
				mSensorManager.unregisterListener(accelerometerListener);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		// There is no tiltangle sensor in Android, so implementing the same algorithm as Sensor API in Windows
		else if ((setting.getName().equalsIgnoreCase("tiltangle")))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				tiltEnabled = true;
				mSensorManager.registerListener(accelerometerListener, mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				tiltEnabled = false;
				mSensorManager.unregisterListener(accelerometerListener);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		// Unsupported sensors on the Enterprise Tablet
		else if ((setting.getName().equalsIgnoreCase("deviceOrientation")) ||
				 (setting.getName().equalsIgnoreCase("motion")) ||
				 (setting.getName().equalsIgnoreCase("ecompass")) ||
				 (setting.getName().equalsIgnoreCase("proximitylongrange")) ||
				 (setting.getName().equalsIgnoreCase("humidity")))
		{			
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, setting + " Sensor not supported on this platform "));			
		}
		else if (setting.getName().equalsIgnoreCase("magnetometer"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{				
				mSensorManager.registerListener(magnetometerListener, mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD), SensorManager.SENSOR_DELAY_NORMAL);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				mSensorManager.unregisterListener(magnetometerListener);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		else if (setting.getName().equalsIgnoreCase("gyroscope"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{				
				mSensorManager.registerListener(gyroscopeListener, mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE), SensorManager.SENSOR_DELAY_NORMAL);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				mSensorManager.unregisterListener(gyroscopeListener);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		else if (setting.getName().equalsIgnoreCase("ambientlight"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				mSensorManager.registerListener(ambientlightListener, mSensorManager.getDefaultSensor(Sensor.TYPE_LIGHT), SensorManager.SENSOR_DELAY_NORMAL);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				mSensorManager.unregisterListener(ambientlightListener);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		else if (setting.getName().equalsIgnoreCase("proximity"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				mSensorManager.registerListener(proximityListener, mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY), SensorManager.SENSOR_DELAY_NORMAL);	
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				mSensorManager.unregisterListener(proximityListener);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		else if (setting.getName().equalsIgnoreCase("pressure"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				mSensorManager.registerListener(pressureListener, mSensorManager.getDefaultSensor(Sensor.TYPE_PRESSURE), SensorManager.SENSOR_DELAY_NORMAL);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				mSensorManager.unregisterListener(pressureListener);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		else if (setting.getName().equalsIgnoreCase("temperature"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{				
				mSensorManager.registerListener(temperatureListener, mSensorManager.getDefaultSensor(Sensor.TYPE_TEMPERATURE), SensorManager.SENSOR_DELAY_NORMAL);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				mSensorManager.unregisterListener(temperatureListener);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		else if (setting.getName().equalsIgnoreCase("gravity"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				mSensorManager.registerListener(gravityListener, mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY), SensorManager.SENSOR_DELAY_NORMAL);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				mSensorManager.unregisterListener(gravityListener);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		else if (setting.getName().equalsIgnoreCase("linearAcceleration"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				mSensorManager.registerListener(linearListener, mSensorManager.getDefaultSensor(Sensor.TYPE_LINEAR_ACCELERATION), SensorManager.SENSOR_DELAY_NORMAL);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				mSensorManager.unregisterListener(linearListener);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		else if (setting.getName().equalsIgnoreCase("rotation"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				mSensorManager.registerListener(rotationListener, mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR), SensorManager.SENSOR_DELAY_NORMAL);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				mSensorManager.unregisterListener(rotationListener);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		else if ((setting.getName().equalsIgnoreCase("orientation")))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				mSensorManager.registerListener(orientationListener, mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION), SensorManager.SENSOR_DELAY_NORMAL);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				mSensorManager.unregisterListener(orientationListener);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}	
		
		else if (setting.getName().equalsIgnoreCase("minimumInterval"))
		{
			try
			{
				minimumInterval = Integer.parseInt(setting.getValue());
				if(minimumInterval < 200)
				{
					minimumInterval = 200; //Default
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Setting default interval 200"));
				}				
			}
			catch (Exception e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid minimumInterval '" + setting.getValue() + "'"));
			}
		}	
		else if (setting.getName().equalsIgnoreCase("sensorevent"))
		{			
			sensorUrl = setting.getValue(); 
		}
		else if (setting.getName().equalsIgnoreCase("getSensorData"))
		{
			// At this point we immediately send all the available data.
			// Note that we dont update the data at this point, the update happens only when 
			// the sensors respond in the listeners
			try
			{
				navigate(sensorUrl, NAMES, values);
			}
			catch (NavigateException e)
			{
				e.printStackTrace();
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
			}
			dwQuietStart = System.currentTimeMillis();			
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + setting.getName()));
		}
	}

	@Override
	public void onPageStarted(String url)
	{
		sensorUrl = null;
		minimumInterval = 1000;
		accelEnabled = false;
		tiltEnabled = false;
		for (int i=0;i<noOfEntries;i++)
		{
			values[i] = "";
		}
		
		try
		{
			mSensorManager.unregisterListener(rotationListener);	
			mSensorManager.unregisterListener(accelerometerListener);				
			mSensorManager.unregisterListener(magnetometerListener);
			mSensorManager.unregisterListener(gyroscopeListener);
			mSensorManager.unregisterListener(ambientlightListener);
			mSensorManager.unregisterListener(proximityListener);
			mSensorManager.unregisterListener(pressureListener);
			mSensorManager.unregisterListener(temperatureListener);	
			mSensorManager.unregisterListener(gravityListener);
			mSensorManager.unregisterListener(linearListener);			
			mSensorManager.unregisterListener(orientationListener);			
		}
		catch (Exception e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Exception.. " + e.getMessage()));
		}
	}
	
	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onShutdown()
	 */
	@Override
	public void onShutdown() {	
		
		// If the user doesn't Disable the sensors then we need to unregister manually
		try
		{
			mSensorManager.unregisterListener(rotationListener);	
			mSensorManager.unregisterListener(accelerometerListener);				
			mSensorManager.unregisterListener(magnetometerListener);
			mSensorManager.unregisterListener(gyroscopeListener);
			mSensorManager.unregisterListener(ambientlightListener);
			mSensorManager.unregisterListener(proximityListener);
			mSensorManager.unregisterListener(pressureListener);
			mSensorManager.unregisterListener(temperatureListener);	
			mSensorManager.unregisterListener(gravityListener);
			mSensorManager.unregisterListener(linearListener);			
			mSensorManager.unregisterListener(orientationListener);		

			mSensorManager = null;
		}
		catch (Exception e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Exception.. " + e.getMessage()));
		}
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
	    return new Version ("RawSensors"); //$NON-NLS-1$
	}
}
