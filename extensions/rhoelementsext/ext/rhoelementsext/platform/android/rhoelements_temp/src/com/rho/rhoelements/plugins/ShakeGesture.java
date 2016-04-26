/**
 * Shake Gesture
 */
package com.rho.rhoelements.plugins;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import com.rho.rhoelements.Common;

/**
 * @author pxb743
 * Shake Gesture detects if the device is in motion
 */
public class ShakeGesture extends ElementsGesture  {

	private int nThreshold;			///< Threshold for accelerometer values to detect gesture
	private int nQuietPeriod;		///< Quiet period before gesture is detected again
	private Boolean bWaitingQuiet;	///< Flag to indicate that quiet period is in progress
	private long dwQuietStart;		///< System tick count when quiet period was started
	
	private SensorManager mSensorManager;
	
	private float mAccel; 			///< acceleration apart from gravity
	private float mAccelCurrent; 	///< current acceleration including gravity
	private float mAccelLast; 		///< last acceleration including gravity
	
	public ShakeGesture(String id, int threshold, int quiet_period, Object plugin)
	{
		nThreshold = threshold;
		nQuietPeriod = quiet_period;
		
		mGestureId = new String(id);
		mPlugin = (GesturePlugin) plugin;	
		
		bWaitingQuiet = false;
		mAccel = 0.00f;
	    mAccelCurrent = SensorManager.GRAVITY_EARTH;
	    mAccelLast = SensorManager.GRAVITY_EARTH;
	    
		// Locate the SensorManager using Activity.getSystemService
		mSensorManager = (SensorManager) Common.mainActivity.getSystemService("sensor");
		
		// Register SensorListener's for Magnetic Field and Orientation
		mSensorManager.registerListener(sl, mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);	
	}
	
	/**
	 * Monitors the sensor to determine which way up the device is
	 * also takes care of upside down which is not handled by the default Sensors
	 * using the Magnetic Field Sensor as well as Orientation.
	 */
	private final SensorEventListener sl = new SensorEventListener() 
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1) {}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			sensorChanged(arg0);
		}
	};	
	
	public void onUnregisterSensor()
	{
		mSensorManager.unregisterListener(sl);
	}
	
	private void sensorChanged(SensorEvent event) {    
		
		if (event.sensor.getType() != Sensor.TYPE_ACCELEROMETER)
            return;
        		
        float accel_x = event.values[0];
        float accel_y = event.values[1];
        float accel_z = event.values[2];      

        mAccelLast = mAccelCurrent;         
        mAccelCurrent = (float) Math.sqrt((double) (accel_x*accel_x + accel_y*accel_y + accel_z*accel_z));
        
        float delta = mAccelCurrent - mAccelLast;        
        mAccel = mAccel * 0.9f + delta; // perform low-cut filter
        
        // Here the algorithm is different from Windows 
        Boolean any_active = (mAccel >= 1) && (mAccel * 100 <= nThreshold);
        
        long elapsed;
        
        if (bWaitingQuiet)
    	{
    		if (!any_active)
    		{
    			// Get time since quiet period started - ignore rollover
    			elapsed = System.currentTimeMillis() - dwQuietStart;

    			if (elapsed >= nQuietPeriod)
    				bWaitingQuiet = false;
    		}
    		else
    		{
    			dwQuietStart = System.currentTimeMillis();
    		}
    	}
    	else
    	{
    		// Out of quiet period
    		if (any_active)
    		{
    			mActivationCounter = 0;         		
        		onDetected ();
    			dwQuietStart = System.currentTimeMillis();
    			bWaitingQuiet = true;
    		}
    	}       
    }
}
