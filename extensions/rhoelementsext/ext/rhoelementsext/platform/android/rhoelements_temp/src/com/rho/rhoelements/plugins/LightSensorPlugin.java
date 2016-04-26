/**
 * Light Sensor plugin
 */
package com.rho.rhoelements.plugins;

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
 * Light Sensor plugin measures the light intensity
 */
public class LightSensorPlugin extends Plugin {

	private SensorManager mSensorManager;
	private int minimumInterval = 1000;
	private static String sensorUrl = null;	
	private long dwQuietStart;		///< System tick count when quiet period was started
	
	/// event names corresponding to navigate name value pairs
	private static final String[] NAMES = new String[]
	{
		"LightSensorValue",	// Light Sensor Value as LUX
	};
	private static final String[] values = new String[1];
	
	public LightSensorPlugin()
	{
		// Locate the SensorManager using Activity.getSystemService
		mSensorManager = (SensorManager) Common.mainActivity.getSystemService("sensor");		
		dwQuietStart = System.currentTimeMillis();
	}
	
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
	
	private void sensorChanged(SensorEvent event) {    
		
		try
		{
			if (event.sensor.getType() != Sensor.TYPE_LIGHT)
	            return;
	        
			int lightValue = (int) event.values[0];
			//TODO: Convert to equivalent Sensor IST value (0 - 2000)		
			
			values[0] = String.valueOf(lightValue);		
			
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
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "LightSensor Navigate Exception.. length is " + e.GetLength()));
		}
    }

	@Override
	public void onSetting(PluginSetting setting) {
		
		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'",
					setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));
		
		if (setting.getName().equalsIgnoreCase("status"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				mSensorManager.registerListener(sl, mSensorManager.getDefaultSensor(Sensor.TYPE_LIGHT), SensorManager.SENSOR_DELAY_NORMAL);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				mSensorManager.unregisterListener(sl);
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
				if(minimumInterval < 0)
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
		else if (setting.getName().equalsIgnoreCase("lightsensorevent"))
		{			
			sensorUrl = setting.getValue(); 
		}
		else if (setting.getName().equalsIgnoreCase("getSensorData"))
		{
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
		
		values[0] = null;		
		
		try
		{
			mSensorManager.unregisterListener(sl);	
		}
		catch (Exception e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Exception.. " + e.getMessage()));
		}
	}
	
	@Override
	public void onShutdown() {
		// If the user doesn't Disable the sensors then we need to unregister manually
		try
		{
			mSensorManager.unregisterListener(sl);
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
	    return new Version ("LightSensor"); //$NON-NLS-1$
	}
}
