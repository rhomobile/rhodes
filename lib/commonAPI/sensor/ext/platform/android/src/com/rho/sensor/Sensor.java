package com.rho.sensor;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import com.rho.sensor.ISensor;
import com.rho.sensor.ISensorSingleton;
import com.rho.sensor.SensorBase;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;

public class Sensor extends SensorBase implements ISensor {
    
	private String TAG = "Rho::" + Sensor.class.getSimpleName(); 

	/**
	 * Sensor Manager variable for enabling/disabling all the sensors
	 */
	private SensorManager mSensorManager;	
	/**
	 * Sensor Navigation URLs for all the sensors
	 */
	private IMethodResult sensorUrl;

	/**
	 * Minimum Interval between any two sensor event updates
	 */
	private int minimumInterval;

	/**
	 * Type of the Sensor
	 */
	private String mType;
	
	/**
	 * Status of the sensor
	 */
	private String mStatus = ISensorSingleton.SENSOR_STATUS_NOT_READY;
	
	/**
	 * Variable that records the event sending data to avoid sending another event before minimum interval
	 */
	private long dwQuietStart;
	/**
	 *	The default minimem gap specified in the sensor.xml
	 *  Cannot be set to any value lower than 200.
	*/

	private Map<String, Object> lastRefreshedProps = null;

	static private final int DEFAULT_MINIMUM_GAP = 200;
	
	@Override
	public void getMinimumGap(IMethodResult result)
	{
		result.set(minimumInterval);		
	}

	@Override
	public void setMinimumGap(int duration, IMethodResult result)
	{		
		minimumInterval = duration >= DEFAULT_MINIMUM_GAP ? duration : DEFAULT_MINIMUM_GAP;		
	}	
	
	@Override
	public void getType(IMethodResult result)
	{
		result.set(mType);		
	}
	
	@Override
	public void getStatus(IMethodResult result)
	{
		result.set(mStatus);		
	}

	@Override
    public void getAllProperties(IMethodResult result) {
       
	   Logger.D(TAG,  "getProperty API [" + minimumInterval + ", " + mType + ", " + mStatus + "]");
	   
	   Map<String, Object> props = new HashMap<String, Object>();
	  
	   props.put("minimumGap", Integer.toString(minimumInterval));
	   props.put("type", mType);
	   props.put("status", mStatus);	   
	   
	   result.set(props);
    }
	
    @Override
    public void getProperty(String name, IMethodResult result) {
        
    	Logger.D(TAG,  "getProperty API");
    	if (name.equalsIgnoreCase("minimumGap"))
    		result.set(Integer.toString(minimumInterval));
    	else if (name.equalsIgnoreCase("type"))
    		result.set(mType);    	
    	else if (name.equalsIgnoreCase("status"))
    		result.set(mStatus);
    	else
    		result.setError("Invalid attribute");
    }

    @Override
    public void getProperties(List<String> names, IMethodResult result) {
       
    	Logger.D(TAG,  "getProperties API");
    	Map<String, Object> props = new HashMap<String, Object>();
    	for (String name: names)
    	{
    		if (name.equalsIgnoreCase("minimumGap"))
    			props.put("minimumGap", Integer.toString(minimumInterval));        		
        	else if (name.equalsIgnoreCase("type"))
        		props.put("type", mType);
        	else if (name.equalsIgnoreCase("status"))
        		props.put("status", mStatus);
        	else
        		result.setError("Invalid attribute");
    	}
    	result.set(props);
    }

    @Override
    public void setProperty(String name, String val, IMethodResult result) {
        
    	Logger.D(TAG,  "setProperty API");
    	if (name.equalsIgnoreCase("minimumGap"))
    	{
    		int minGap = DEFAULT_MINIMUM_GAP;
    		try
    		{
    			minGap = Integer.parseInt(val);
    			minimumInterval = (minGap >= DEFAULT_MINIMUM_GAP) ? minGap : DEFAULT_MINIMUM_GAP;
    		}
    		catch(NumberFormatException nfex)
    		{
    			Logger.E(TAG,  "setProperties API, Invalid minimumGap " + val);
    			result.setError("Invalid minimumGap " + val + " , must be greater than " + DEFAULT_MINIMUM_GAP);
    		}    		
    	}
    	else 
    		result.setError("Invalid attribute");
    }

    @Override
    public void setProperties(Map<String, String> props, IMethodResult result) {
        
    	Logger.D(TAG,  "setProperties API");
    	for (Map.Entry<String, String> entry: props.entrySet())
    	{
    		if (entry.getKey().equalsIgnoreCase("minimumGap"))
    		{
    			int minGap = DEFAULT_MINIMUM_GAP;
	    		try
	    		{
	    			minGap = Integer.parseInt(entry.getValue());
	    			minimumInterval = (minGap >= DEFAULT_MINIMUM_GAP) ? minGap : DEFAULT_MINIMUM_GAP;
	    		}
	    		catch(NumberFormatException nfex)
	    		{
	    			Logger.E(TAG,  "setProperties API, Invalid minimumGap " + entry.getValue());
	    			result.setError("Invalid minimumGap " + entry.getValue() + " , must be greater than " + DEFAULT_MINIMUM_GAP);
	    		}
    		}
        	else 
        		result.setError("Invalid attribute");
    	}
    }
    
//    @Override
//    public void clearAllProperties(IMethodResult result) 
//    {
//    	Logger.D(TAG,  "clearAllProperties API");
//    	  
//    	resetProperties();	    	
//    }
    
    public void resetProperties()
    {
    	sensorUrl = null;
		dwQuietStart = System.currentTimeMillis();        	
		minimumInterval = DEFAULT_MINIMUM_GAP; //Default - 200 milli seconds
		mStatus = ISensorSingleton.SENSOR_STATUS_NOT_READY;
    }

	private final SensorEventListener sensorListener = new SensorEventListener() 
	{		
		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			sensorChanged(arg0);
		}

		@Override
		public void onAccuracyChanged(android.hardware.Sensor sensor, int accuracy)
		{
			// TODO Auto-generated method stub			
		}
	};
	
	/**
	 * Handle the data from Accelerometer sensor
	 * @param event
	 */
	private void sensorChanged(SensorEvent event) {

		Map<String, Object> props = new HashMap<String, Object>();
		double accel_x = event.values[0];
		double accel_y = event.values[1];
		double accel_z = event.values[2];	

		int sensorType = event.sensor.getType();
		long currentTimeMillis = System.currentTimeMillis();
		
		//Logger.D(TAG, "Sensor event fired for type:" + sensorType);
		switch (sensorType)
		{
			case android.hardware.Sensor.TYPE_ACCELEROMETER:

				//Logger.D(TAG,  ISensorSingleton.SENSOR_TYPE_ACCELEROMETER + ">>>" + mType + " [" + accel_x + ", " + accel_y + ", " + accel_z + "]");
	
				//Logger.D(TAG, "Accelerometer Sensor change detected");
				if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_ACCELEROMETER))
				{
					//Logger.D(TAG, "Acceleromert fire the event");
					//get time from the last event					
					long elapsed = currentTimeMillis - dwQuietStart;

					Logger.D(TAG,  "Accelerometer>>> elapsed " + elapsed + ", " + currentTimeMillis);
	
					if (elapsed >= minimumInterval)
					{
						//Logger.D(TAG, "Accelerometer minimum interval surpassed");
						props.put("accelerometer_x", accel_x);
						props.put("accelerometer_y", accel_y);
						props.put("accelerometer_z", accel_z);
						props.put("status", "ok");
	
						if ((sensorUrl != null) && isCallback(sensorUrl)) sensorUrl.set(props);
						//Logger.T(TAG, "Accelerometer event fired: "+ sensorUrl +":" + props);
						dwQuietStart = currentTimeMillis;	
					}				
				}		
	
				if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_TILT_ANGLE))
				{
					//get time from the last event
					long elapsed = currentTimeMillis - dwQuietStart;
	
					if (elapsed >= minimumInterval)
					{
						double PI = 3.14159265;				
						// This algorithm is taken from the Windows Sensor API
						double tilt_y =  (Math.atan( accel_x/Math.sqrt( accel_y*accel_y + accel_z*accel_z ) ) * 180 / PI);
						double tilt_x =  (Math.atan( accel_y/Math.sqrt( accel_x*accel_x + accel_z*accel_z ) ) * 180 / PI);        
						double tilt_z =  (Math.atan( accel_z/Math.sqrt( accel_x*accel_x + accel_y*accel_y ) ) * 180 / PI);  
	
						props.put("tiltangle_x", String.valueOf(tilt_x));
						props.put("tiltangle_y", String.valueOf(tilt_y));
						props.put("tiltangle_z", String.valueOf(tilt_z));
						props.put("status", "ok");
	
						if ((sensorUrl != null) && isCallback(sensorUrl)) sensorUrl.set(props);

						dwQuietStart = currentTimeMillis;
					}					
				}
				break;
				
			case android.hardware.Sensor.TYPE_MAGNETIC_FIELD:
				
				if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_MAGNETOMETER))
				{
					//get time from the last event
					long elapsed = currentTimeMillis - dwQuietStart;
	
					if (elapsed >= minimumInterval)
					{
						props.put("magnetometer_x", event.values[0]);
						props.put("magnetometer_y", event.values[1]);
						props.put("magnetometer_z", event.values[2]);	
						props.put("status", "ok");
	
						if ((sensorUrl != null) && isCallback(sensorUrl)) sensorUrl.set(props);

						dwQuietStart = currentTimeMillis;	
					}
				}	
				break;
				
			case android.hardware.Sensor.TYPE_GYROSCOPE:		
		      		
				if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_GYROSCOPE))
				{
					//get time from the last event
					long elapsed = currentTimeMillis - dwQuietStart;
	
					if (elapsed >= minimumInterval)
					{
						props.put("gyroscope_x", event.values[0]);
						props.put("gyroscope_y", event.values[1]);
						props.put("gyroscope_z", event.values[2]);	
						props.put("status", "ok");
	
						if ((sensorUrl != null) && isCallback(sensorUrl)) sensorUrl.set(props);

						dwQuietStart = currentTimeMillis;	
					}				
				}	
				break;
				
			case android.hardware.Sensor.TYPE_LIGHT:		
	      		
				if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_AMBIENT_LIGHT))
				{
					//get time from the last event
					long elapsed = currentTimeMillis - dwQuietStart;
	
					if (elapsed >= minimumInterval)
					{
						props.put("ambientlight_value", event.values[0]);						
						props.put("status", "ok");
	
						if ((sensorUrl != null) && isCallback(sensorUrl)) sensorUrl.set(props);

						dwQuietStart = currentTimeMillis;	
					}				
				}	
				break;
				
			case android.hardware.Sensor.TYPE_PROXIMITY:		
	      		
				if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_PROXIMITY))
				{
					//get time from the last event
					long elapsed = currentTimeMillis - dwQuietStart;
	
					if (elapsed >= minimumInterval)
					{
						props.put("proximity_value", event.values[0]);						
						props.put("status", "ok");
	
						if ((sensorUrl != null) && isCallback(sensorUrl)) sensorUrl.set(props);

						dwQuietStart = currentTimeMillis;	
					}				
				}	
				break;
				
			case android.hardware.Sensor.TYPE_PRESSURE:		
	      		
				if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_PRESSURE))
				{
					//get time from the last event
					long elapsed = currentTimeMillis - dwQuietStart;
	
					if (elapsed >= minimumInterval)
					{
						props.put("pressure_value", event.values[0]);						
						props.put("status", "ok");
	
						if ((sensorUrl != null) && isCallback(sensorUrl)) sensorUrl.set(props);

						dwQuietStart = currentTimeMillis;	
					}				
				}	
				break;
				
			case android.hardware.Sensor.TYPE_TEMPERATURE:		
	      		
				if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_TEMPERATURE))
				{
					//get time from the last event
					long elapsed = currentTimeMillis - dwQuietStart;
	
					if (elapsed >= minimumInterval)
					{
						props.put("temperature_value", event.values[0]);						
						props.put("status", "ok");
	
						if ((sensorUrl != null) && isCallback(sensorUrl)) sensorUrl.set(props);

						dwQuietStart = currentTimeMillis;	
					}				
				}	
				break;
				
			case android.hardware.Sensor.TYPE_GRAVITY:		
	      		
				if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_GRAVITY))
				{
					//get time from the last event
					long elapsed = currentTimeMillis - dwQuietStart;
	
					if (elapsed >= minimumInterval)
					{
						props.put("gravity_x", event.values[0]);
						props.put("gravity_y", event.values[1]);
						props.put("gravity_z", event.values[2]);	
						props.put("status", "ok");
	
						if ((sensorUrl != null) && isCallback(sensorUrl)) sensorUrl.set(props);

						dwQuietStart = currentTimeMillis;	
					}				
				}	
				break;
				
			case android.hardware.Sensor.TYPE_LINEAR_ACCELERATION:		
	      		
				if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_LINEAR_ACCELERATION))
				{
					//get time from the last event
					long elapsed = currentTimeMillis - dwQuietStart;
	
					if (elapsed >= minimumInterval)
					{
						props.put("linearacceleration_x", event.values[0]);
						props.put("linearacceleration_y", event.values[1]);
						props.put("linearacceleration_z", event.values[2]);	
						props.put("status", "ok");
	
						if ((sensorUrl != null) && isCallback(sensorUrl)) sensorUrl.set(props);

						dwQuietStart = currentTimeMillis;	
					}				
				}	
				break;
				
			case android.hardware.Sensor.TYPE_ROTATION_VECTOR:		
	      		
				if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_ROTATION))
				{
					//get time from the last event
					long elapsed = currentTimeMillis - dwQuietStart;
	
					if (elapsed >= minimumInterval)
					{
						props.put("rotation_x", event.values[0]);
						props.put("rotation_y", event.values[1]);
						props.put("rotation_z", event.values[2]);	
						props.put("status", "ok");
	
						if ((sensorUrl != null) && isCallback(sensorUrl)) sensorUrl.set(props);

						dwQuietStart = currentTimeMillis;	
					}				
				}	
				break;
				
			case android.hardware.Sensor.TYPE_ORIENTATION:		
	      		
				if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_ORIENTATION))
				{
					//get time from the last event
					long elapsed = currentTimeMillis - dwQuietStart;
	
					if (elapsed >= minimumInterval)
					{
						props.put("orientation_x", event.values[0]);
						props.put("orientation_y", event.values[1]);
						props.put("orientation_z", event.values[2]);	
						props.put("status", "ok");
	
						if ((sensorUrl != null) && isCallback(sensorUrl)) sensorUrl.set(props);

						dwQuietStart = currentTimeMillis;	
					}				
				}	
				break;
				
			default:
				break;
		}		
		if (!props.isEmpty()) lastRefreshedProps = props;	
	}
	
    public Sensor(String id) {
        super(id);
        
        mSensorManager = (SensorManager) RhodesActivity.safeGetInstance().getSystemService("sensor");		
		sensorUrl = null;
		mType = id; //ISensorSingleton.SENSOR_TYPE_ACCELEROMETER; 
		// initial the start time to zero as the first event will get propgated to the JS/Ruby layer
		dwQuietStart = 0;//System.currentTimeMillis();        	
		minimumInterval = DEFAULT_MINIMUM_GAP; //Default - 200 milli seconds
		
		// List the available sensors
		List<android.hardware.Sensor> sensors = mSensorManager.getSensorList(android.hardware.Sensor.TYPE_ALL);
		for (android.hardware.Sensor sensor : sensors) 
		{
			Logger.I(TAG, "Available Sensor: " + sensor.getName());	    		       
		}
		mStatus = ISensorSingleton.SENSOR_STATUS_READY;
		Logger.I(TAG, "Creating object for Sensor type: " + mType);	    
    }

	@Override
	public void start(IMethodResult result)
	{		
		sensorUrl = result;	

		if (mStatus == ISensorSingleton.SENSOR_STATUS_STARTED)
		{
			Logger.I(TAG, "Sensor type " + mType + " is already started");
			return;
		}

		// reset the start time to zero as the first event will get propgated to the JS/Ruby layer
		dwQuietStart = 0;
		
		if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_ACCELEROMETER))
		{
			mSensorManager.registerListener(sensorListener, 
					mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
		}
		else if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_TILT_ANGLE))
		{
			mSensorManager.registerListener(sensorListener, 
					mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
		}
		else if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_MAGNETOMETER))
		{	
			mSensorManager.registerListener(sensorListener, 
					mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_MAGNETIC_FIELD), SensorManager.SENSOR_DELAY_NORMAL);
		}
		else if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_GYROSCOPE))
		{
			mSensorManager.registerListener(sensorListener, 
					mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_GYROSCOPE), SensorManager.SENSOR_DELAY_NORMAL);
		}
		else if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_AMBIENT_LIGHT))
		{	
			mSensorManager.registerListener(sensorListener, 
					mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_LIGHT), SensorManager.SENSOR_DELAY_NORMAL);
		}
		else if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_PROXIMITY))
		{	
			mSensorManager.registerListener(sensorListener, 
					mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_PROXIMITY), SensorManager.SENSOR_DELAY_NORMAL);
		}
		else if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_PRESSURE))
		{
			mSensorManager.registerListener(sensorListener,
					mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_PRESSURE), SensorManager.SENSOR_DELAY_NORMAL);
		}
		else if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_TEMPERATURE))
		{	
			mSensorManager.registerListener(sensorListener, 
					mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_TEMPERATURE), SensorManager.SENSOR_DELAY_NORMAL);
		}
		else if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_GRAVITY))
		{	
			mSensorManager.registerListener(sensorListener, 
					mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_GRAVITY), SensorManager.SENSOR_DELAY_NORMAL);
		}
		else if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_LINEAR_ACCELERATION))
		{	
			mSensorManager.registerListener(sensorListener, 
					mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_LINEAR_ACCELERATION), SensorManager.SENSOR_DELAY_NORMAL);
		}
		else if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_ROTATION))
		{	
			mSensorManager.registerListener(sensorListener, 
					mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_ROTATION_VECTOR), SensorManager.SENSOR_DELAY_NORMAL);
		}
		else if (mType.equalsIgnoreCase(ISensorSingleton.SENSOR_TYPE_ORIENTATION))
		{	
			mSensorManager.registerListener(sensorListener, 
					mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_ORIENTATION), SensorManager.SENSOR_DELAY_NORMAL);
		}			
		else
		{
			Logger.E(TAG, "Sensor type " + mType + " not yet set or not supported on this platform");
		}
					
		
		mStatus = ISensorSingleton.SENSOR_STATUS_STARTED;
		Logger.D(TAG, "Sensor registered successfully for type: " + mType);
	}

	@Override
	public void readData(IMethodResult result)
	{
		Map<String, Object> props = null;
		
		if (mStatus != ISensorSingleton.SENSOR_STATUS_STARTED){
			props = new HashMap<String, Object>();
			props.put("status", "error");
			props.put("message", "the sensor has not been started");
		}else if (lastRefreshedProps != null){
			props = lastRefreshedProps;
		}else{
			props = new HashMap<String, Object>();
		props.put("status", "error");
			props.put("message", "the sensor has not been refreshed from the last time of calling this function or starting the sensor");
		}

		result.set(props);		
	}

	@Override
	public void stop(IMethodResult result)
	{
		if (mStatus == ISensorSingleton.SENSOR_STATUS_STARTED)
		{
			mSensorManager.unregisterListener(sensorListener);
			Logger.D(TAG, "Sensor unregistered successfully");		
			mStatus = ISensorSingleton.SENSOR_STATUS_READY;
			lastRefreshedProps = null;
		}
	}

	private boolean isCallback(IMethodResult result)
	{
		return (result != null && result.hasCallback());
	}

}
