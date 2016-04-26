/**
 * Tilt gesture plugin
 */
package com.rho.rhoelements.plugins;

import com.rho.rhoelements.Common;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

/**
 * @author pxb743
 * This plugin measures the tilt angle 
 */
public class TiltGesture extends ElementsGesture  {

	private int nXTarget;		///< Target X sensor value; values range from -90 to 90.
	private int nYTarget;		///< Target Y sensor value; values range from -90 to 90.
	private int nZTarget;		///< Target Z sensor value; values range from -90 to 90.
	private int nTolerance;		///< Allowable deviation from target values 
	private int nHysteresis;	///< To prevent false re-triggering 
	private Boolean bInRange;	///< Flag to indicate that reported Sensor values are in allowable range of target
	
	private SensorManager mSensorManager;
	
	public TiltGesture(String id, int x, int y, int z, int tolerance, int hysteresis, Object plugin)
	{
		nXTarget = x;
		nYTarget = y;
		nZTarget = z;

		nTolerance = tolerance;
		nHysteresis = hysteresis;
		
		bInRange = false;
		
		mGestureId = new String(id);
		mPlugin = (GesturePlugin) plugin;
		
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
	    // There is no Tilt Angle sensor for Android, so following the algorithm from
	    // Sensor API in WIndows
        if (event.sensor.getType() != Sensor.TYPE_ACCELEROMETER)
            return;        
       
        double PI = 3.14159265;
        
        double x = event.values[0];
        double y = event.values[1];
        double z = event.values[2];
     
        int tilt_x =  (int) (Math.atan( x/Math.sqrt( y*y + z*z ) ) * 180 / PI);
        int tilt_y =  (int) (Math.atan( y/Math.sqrt( x*x + z*z ) ) * 180 / PI);        
        int tilt_z =  (int) (Math.atan( z/Math.sqrt( x*x + y*y ) ) * 180 / PI);               
        
        Boolean x_detected = false, y_detected = false, z_detected = false;
    	
		if (bInRange)
    	{
    		// Wait for one value to be out of range allowing for hysteresis
    		if (tilt_x > (nXTarget + nTolerance + nHysteresis) || tilt_x < (nXTarget - nTolerance - nHysteresis))
    			bInRange = false;
    	
    		if (tilt_y > (nYTarget + nTolerance + nHysteresis) || tilt_y < (nYTarget - nTolerance - nHysteresis))
    			bInRange = false;

    		if (tilt_z > (nZTarget + nTolerance + nHysteresis) || tilt_z < (nZTarget - nTolerance - nHysteresis))
    			bInRange = false;
    	}
    	else
    	{
    		// Wait for all values to be in range
    		if (tilt_x < (nXTarget + nTolerance) && tilt_x > (nXTarget - nTolerance))
    			x_detected = true;

    		if (tilt_y < (nYTarget + nTolerance) && tilt_y > (nYTarget - nTolerance))
    			y_detected = true;

    		if (tilt_z < (nZTarget + nTolerance) && tilt_z > (nZTarget - nTolerance))
    			z_detected = true;

    		bInRange = (x_detected && y_detected && z_detected);

    		if (bInRange)
    		{  
    			mActivationCounter = 0;        
    			onDetected();		
    		} 
    	}		
    }

}
