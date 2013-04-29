package com.motorolasolutions.rho.screenorientation;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

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

public class OrientationPlugin
{	
    private String mDirection;
    private String mLastDirection = "Empty";
    private String mScreenOrientationEventURL;
    private SensorManager mSensorManager;
    private boolean mIsAutoRotate;
    
    private String mConfigSetting;
    private Activity mActivity;

    private String TAG = OrientationPlugin.class.getSimpleName();

    public OrientationPlugin(Activity activity)
    {
	mActivity = activity;
	
	/*CallbackHandlerObj = new CallbackHandler(this);
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));*/
		
	//mActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		
	// Locate the SensorManager using Activity.getSystemService
	mSensorManager = (SensorManager) mActivity.getSystemService("sensor");
	// read the autorotate config setting
	//mConfigSetting = Common.config.getSetting(Config.SETTING_AUTOROTATE);
	if (mConfigSetting == null)
	{
	    mConfigSetting = new String("1");
	}
	mIsAutoRotate = !mConfigSetting.equals("0");
    }

    public void onPageStarted(String url)
    {
	Logger.D(TAG, "");    
	   
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
	    Display display = ((WindowManager) mActivity.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
	    switch (display.getRotation())
	    {
	    	case(Surface.ROTATION_0):
	    	{
	    	    mDirection = "Normal";
	    	    break;
	    	}
	    	case(Surface.ROTATION_90):
	    	{
	    	    mDirection = "LeftHanded";
	    	    break;
	    	}
		case(Surface.ROTATION_180):
		{
		    mDirection = "UpsideDown";
		    break;
		}
		case(Surface.ROTATION_270):
		{
		    mDirection = "RightHanded";
		    break;
		}
		default:
		{
		    mDirection = "Normal";
		    break;
		}
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
	    /*if(mIsAutoRotate || isForced)
	    {
		try
		{
		    RhoExtManager.(mScreenOrientationEventURL, "orientation", mDirection);
		}
		catch (NavigateException e)
		{
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
		}
	    }*/
	}
	
	public void onShutdown()
	{
	    Logger.I(TAG, "");
		
	    // default back to Sensor mode on quiting Helium
	    mActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
		
	    // Shutdown the sensor listeners if we get asked to shutdown.
	    unregisterSensorListeners();
	}

	public void onSetting(String setting, String value)
	{
	    if (setting.length() > 0)
		Logger.D(TAG, String.format("'%s', '%s'", setting, value));
	    else
		Logger.D(TAG, setting);

	    String propertyStr = "";

	    Logger.D(TAG, setting);
		
	    doParamCheck(setting, value);
		
	    Logger.D(TAG, propertyStr);
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
	void doParamCheck(String setting, String value)
	{
	    if (setting.equalsIgnoreCase("normal"))
	    {
	    	mActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	    	mIsAutoRotate = false;
	    	mDirection = "normal";
	    	fireOrientationEvent(true);
	    }
	    else if (setting.equalsIgnoreCase("righthanded"))
	    {
		mActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
		mIsAutoRotate = false;
		mDirection = "righthanded";
		fireOrientationEvent(true);
	    }
	    else if (setting.equalsIgnoreCase("lefthanded"))
	    {
		mActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		mIsAutoRotate = false;
		mDirection = "lefthanded";
		fireOrientationEvent(true);
	    }
	    else if (setting.equalsIgnoreCase("upsidedown"))
	    {
		mActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
		mIsAutoRotate = false;
		mDirection = "upsidedown";
		fireOrientationEvent(true);
	    }
	    else if (setting.equalsIgnoreCase("screenorientationevent"))
	    {
		Logger.D(TAG, "ScreenOrientationEvent");
		mScreenOrientationEventURL = value;
		Logger.D(TAG, mScreenOrientationEventURL);
		    
		// if we clear the event url then UnRegister the Sensor Listeners
		if(mScreenOrientationEventURL.equals(""))
		{
		    Logger.D(TAG, "unregister Sensor Listeners");
		    unregisterSensorListeners();
		}
		else
		{
		    // Register SensorListener's for Magnetic Field and Orientation
		    mSensorManager.registerListener(sl, mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
		}
	    }
	    else if (setting.equalsIgnoreCase("autorotate"))
	    {
		if (value.equalsIgnoreCase("enabled"))
		{
		    mIsAutoRotate = true;
		    mActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
		}
		else if (value.equalsIgnoreCase("disabled"))
		{
		    mIsAutoRotate = false;
		    setDirection();
		    doParamCheck(mDirection, null);
		}
	    }
	    else
	    {
		Logger.W(TAG, "Unknown setting");
	    }
	}
	
	/**
	 * @return the mIsAutoRotate
	 */
	 public boolean getAutoRotate()
	 {
	     return mIsAutoRotate;
	 }

	 /**
	  * @param mIsAutoRotate the mIsAutoRotate to set
	  */
	  public void setAutoRotate(boolean autoRotate)
	  {
	      this.mIsAutoRotate = autoRotate;
	  }
}
	
	
	

