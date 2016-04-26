package com.rho.rhoelements.plugins;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;
import android.content.res.Configuration;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.Config;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;

/**
 *  <pre>
 * Plugin handles Orientation Monitoring and Event callback into Java Script
 * 
 * The Plugin uses the following API derived from the Browser Help file, additional
 * new API's will have been defined in the Helium Alignment Document.
 * 
 * Upon manually ANY Orientation Automatic ReOrientation will be turned off
 * 
 * Setting Up a Callback in Java script to be called on Orientation Changes
 * 	generic.InvokeMETAFunction('ScreenOrientation',"ScreenOrientationEvent: url('javascript: onOrientationChange('%s');')");
 * 	Values returned will be "Normal", "LeftHanded", "RightHanded", "UpsideDown"
 * 
 * Turning off Event Callback Mechanism is Done as follows:
 * 	generic.InvokeMETAFunction('ScreenOrientation',"ScreenOrientationEvent: url('')");
 * 
 * Setting Orientation Normal ( Portrait )
 *	generic.InvokeMETAFunction('ScreenOrientation', "normal");
 * 
 * Setting Orientation Left Handed ( Landscape )
 * 	generic.InvokeMETAFunction('ScreenOrientation', "lefthanded");
 * 
 * Setting Orientation Right Handed ( Inverse Landscape )
 * 	generic.InvokeMETAFunction('ScreenOrientation', "righthanded");
 *
 * Setting Orientation Upside Down ( Upside Down )
 *	generic.InvokeMETAFunction('ScreenOrientation', "upsidedown");
 *
 * Setting Automatic ReOrientation
 *     	generic.InvokeMETAFunction('ScreenOrientation', "autorotate"); 
 *
 * If the webpage where the orientation plug-in was started is navigated away from
 * the orientation plug-in will reset all configured events and also stop the Orientation
 * event listener since this is no longer required.
 *
 *  </pre>
 */
//@SuppressWarnings("deprecation")
public class OrientationPlugin extends Plugin
{
	private static enum Orientation {
		NORMAL, RIGHT_HANDED, LEFT_HANDED, UPSIDE_DOWN
	}

    private static final String TAG = "Orientation";
    private String mDirection;
    private String mLastDirection = "Empty";
    private String mScreenOrientationEventURL;
    private SensorManager mSensorManager;
    private boolean mIsAutoRotate;
    private String mConfigSetting;
    private int defaultOrientation = -1;


  	public OrientationPlugin()
	{
		CallbackHandlerObj = new CallbackHandler(this);
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		
		//Common.mainActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		
		// Locate the SensorManager using Activity.getSystemService
		mSensorManager = (SensorManager) Common.mainActivity.getSystemService("sensor");
		// read the autorotate config setting
		mConfigSetting = Common.config.getSetting(Config.SETTING_AUTOROTATE);
		if (mConfigSetting == null)
		{
			mConfigSetting = new String("1");
		}
		mIsAutoRotate = !mConfigSetting.equals("0");

	}

	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version ("Orientation");
	}

	@Override
	public void onPageStarted(String url)
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));    
	   
	    // Stop the Event Listeners
	    unregisterSensorListeners();
	    
	    // Wipe the screen orientation Event URL
	    mScreenOrientationEventURL = "";
	    
	}
	
	/**
	 * Monitors the Orientation sensor to determine which way up the device is
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
			setDirection();
		}
	};
	
	private void setDirection()
	{
		Display display = ((WindowManager) Common.mainActivity.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		switch (display.getRotation())
		{
		case(Surface.ROTATION_0):
			mDirection = "Normal";
			break;
		case(Surface.ROTATION_90):
			mDirection = "LeftHanded";
			break;
		case(Surface.ROTATION_180):
			mDirection = "UpsideDown";
			break;
		case(Surface.ROTATION_270):
			mDirection = "RightHanded";
			break;
		default:
			mDirection = "Normal";
		}
	      
	    if(mLastDirection != mDirection)
	    {
			mLastDirection = mDirection;
			fireOrientationEvent();
	    }
	}
	
	/**
	 * Method used for firing an event to java script returning the current network state.
	 */
	private void fireOrientationEvent() 
	{
		fireOrientationEvent(false);
	}
	
	/**
	 * Method used for firing an event to java script returning the current network state.
	 * @param isForced will force resend of the screenOrienationEventURL 
	 */
	private void fireOrientationEvent(boolean isForced) 
	{
		if(mIsAutoRotate || isForced)
		{
		    try
			{
				navigate(mScreenOrientationEventURL, "orientation", mDirection);
			}
			catch (NavigateException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
			}
		}
	}
	
	
	@Override
	public void onShutdown()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		
		// default back to Sensor mode on quiting Helium
		Common.mainActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
		
		// Shutdown the sensor listeners if we get asked to shutdown.
		unregisterSensorListeners();
	}

	@Override
	public void onSetting(PluginSetting setting)
	{
		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));

		String propertyStr = "";

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,setting.getName()));
		
		doParamCheck(setting);
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, propertyStr));
	}

	/**
	 * Method to Unregister Sensor Listeners
	 * Unregisters sensor listeners for ORIENTATION_RAW and MAGNETIC_FIELD
	 * Magnetic field is used to help determine if the device is upside down.
	 */
	void unregisterSensorListeners()
	{
	    	// Un-Register SensorListener's
		mSensorManager.unregisterListener(sl);
	}
	
	/**
	 * Method to check the parameters passed into this plugin and perform the actions associated.
	 * @param setting passed from the main OnSetting method.
	 */
	void doParamCheck(PluginSetting setting)
	{
	    if (setting.getName().equalsIgnoreCase("normal"))
		{
	   		orientDevice(Orientation.NORMAL);
		}
		else if (setting.getName().equalsIgnoreCase("righthanded"))
		{
			orientDevice(Orientation.RIGHT_HANDED);
		}
		else if (setting.getName().equalsIgnoreCase("lefthanded"))
		{
			orientDevice(Orientation.LEFT_HANDED);
		}
		else if (setting.getName().equalsIgnoreCase("upsidedown"))
		{
		    orientDevice(Orientation.UPSIDE_DOWN);
		}
		else if (setting.getName().equalsIgnoreCase("screenorientationevent"))
		{
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,"ScreenOrientationEvent +"));
		    mScreenOrientationEventURL = setting.getValue();
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,mScreenOrientationEventURL));
		    
		    // if we clear the event url then UnRegister the Sensor Listeners
		    if(mScreenOrientationEventURL.equals(""))
		    {
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,"unregister Sensor Listeners"));
				unregisterSensorListeners();
		    }
		    else
		    {
				// Register SensorListener's for Magnetic Field and Orientation
				mSensorManager.registerListener(sl, mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
		    }
		}
		else if (setting.getName().equalsIgnoreCase("autorotate"))
		{
			if (setting.getValue().equalsIgnoreCase("enabled"))
			{
		
				mIsAutoRotate = true;
				Common.mainActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
			}
			else if (setting.getValue().equalsIgnoreCase("disabled"))
			{
				mIsAutoRotate = false;
				setDirection();
				doParamCheck(new PluginSetting(null, mDirection, null));
			}
		}
		else
		{
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unknown setting '" + setting.getName() + "'"));
		}
	}
	
	/**
	 * Orients the device to the inputted Orientation 
	 * @param direction the requested new Orientation of the device 
	 * @author Ben kennedy (NCVT73)
	 */
	public void orientDevice(Orientation direction)
	{
		boolean isDefaultPortrait = getDeviceDefaultOrientation() == 1;
		switch(direction)
		{
			case NORMAL:
				Common.mainActivity.setRequestedOrientation(
					(isDefaultPortrait ? ActivityInfo.SCREEN_ORIENTATION_PORTRAIT : ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE)
				);
				mDirection = "normal";
				break;
			case LEFT_HANDED:
				Common.mainActivity.setRequestedOrientation(
					(isDefaultPortrait ? ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE : ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT)
				);
				mDirection = "lefthanded";
				break;
			case UPSIDE_DOWN:
				Common.mainActivity.setRequestedOrientation(
					(isDefaultPortrait ? ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT : ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE)
				);
				mDirection = "upsidedown";
				break;
			case RIGHT_HANDED: 
				Common.mainActivity.setRequestedOrientation(
					(isDefaultPortrait ? ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE : ActivityInfo.SCREEN_ORIENTATION_PORTRAIT)
				);
				mDirection = "righthanded";
				break;	
		} 	
    	mIsAutoRotate = false;
    	fireOrientationEvent(true);
	}
	
	/**
	 * 
	 * @return Configuration.ORIENTATION_PORTRAIT (1) if the device's "normal" orientation is Portrait, (i.e. phone) 
	 * Configuration.ORIENTATION_LANDSCAPE (2) if the normal is landscape (i.e. tablet)
	 * @author Ben Kennedy (NCVT73) 
	 */
	public int getDeviceDefaultOrientation()
	{
		if(defaultOrientation != -1) return defaultOrientation;
		
	    WindowManager windowManager =  (WindowManager) Common.mainActivity.getSystemService(Activity.WINDOW_SERVICE);
	    Configuration config = Common.mainActivity.getResources().getConfiguration();
	    int rotation = windowManager.getDefaultDisplay().getRotation();

	    if ( ((rotation == Surface.ROTATION_0 || rotation == Surface.ROTATION_180) && config.orientation == Configuration.ORIENTATION_LANDSCAPE)
	        || ((rotation == Surface.ROTATION_90 || rotation == Surface.ROTATION_270) && config.orientation == Configuration.ORIENTATION_PORTRAIT))
	    {
	    	defaultOrientation = Configuration.ORIENTATION_LANDSCAPE;
	    	return Configuration.ORIENTATION_LANDSCAPE;
	    }
	    else
	    { 
	    	defaultOrientation = Configuration.ORIENTATION_PORTRAIT;
	    	return Configuration.ORIENTATION_PORTRAIT;
	    }
	}
}
	
	
	

