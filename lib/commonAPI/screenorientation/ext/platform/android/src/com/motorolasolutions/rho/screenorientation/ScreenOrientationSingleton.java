package com.motorolasolutions.rho.screenorientation;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;

import com.rho.screenorientation.IScreenOrientationSingleton;
import com.rho.screenorientation.ScreenOrientationSingletonBase;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;

class ScreenOrientationSingleton extends ScreenOrientationSingletonBase implements IScreenOrientationSingleton
{
    private static String TAG = ScreenOrientationSingleton.class.getSimpleName();
    private IMethodResult mScreenOrientationCallback;
    private SensorManager mSensorManager;
    private static boolean isAutoRotate;
    private static String direction;
    private static String lastDirection = "";
    
	/**
     * Monitors the Orientation sensor to determine which way up the device is
     * also takes care of upside down which is not handled by the default Sensors
     * using the Magnetic Field Sensor as well as Orientation.
     */
    private final SensorEventListener mSensorListener = new SensorEventListener() 
    {
	@Override
	public void onAccuracyChanged(Sensor arg0, int arg1) {}

	@Override
	public void onSensorChanged(SensorEvent arg0)
	{
	    setDirection();
	}
    };
    public ScreenOrientationSingleton(ScreenOrientationFactory factory)
    {
        super(factory);
	mSensorManager = (SensorManager) RhodesActivity.safeGetInstance().getSystemService("sensor");
	Logger.D(TAG, "ScreenOrientation -- disable_screen_rotation: " + RhoConf.getString("disable_screen_rotation"));
	isAutoRotate = !(RhoConf.getString("disable_screen_rotation").compareTo("1") == 0);
	ScreenOrientationRhoListener.addScreenOrientationInstance(this);
        Logger.D(TAG, "ScreenOrientationSingleton object constructed correctly");
    }

    @Override
    protected String getInitialDefaultID() {
	// TODO Auto-generated method stub
	return null;
    }
	
    @Override
    public void setAutoRotate(String autoRotate, IMethodResult result)
    {
	Logger.D(TAG, "setAutorotate -- START");
	Logger.D(TAG, "setAutorotate -- autoRotate: " + autoRotate);
	if (autoRotate.equalsIgnoreCase("enabled"))
	{
	    RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
	    isAutoRotate = true;
	    RhoConf.setString("disable_screen_rotation", "0");
	}
	else if (autoRotate.equalsIgnoreCase("disabled"))
	{
	    setDirection();
	    setScreenOrientation(direction);
	    isAutoRotate = false;
	    RhoConf.setString("disable_screen_rotation", "1");
	}
	else
	{
	    Logger.D(TAG, "setAutorotate -- autoRotate value is invalid: " + autoRotate);
	    return;
	}
	Logger.D(TAG, "setAutorotate -- END");
    }

    @Override
    public void getAutoRotate(IMethodResult result) 
    {
	if (isAutoRotate)
	    result.set("enabled");
	else
	    result.set("disabled");
    }

   
    @Override
    public void normal(IMethodResult result)
    {
	setScreenOrientation("normal");
    }

    @Override
    public void rightHanded(IMethodResult result)
    {
	setScreenOrientation("righthanded");
    }

    @Override
    public void leftHanded(IMethodResult result)
    {
	setScreenOrientation("lefthanded");
    }

    @Override
    public void upsideDown(IMethodResult result)
    {
	setScreenOrientation("upsidedown");
    }
	
    private void setScreenOrientation(String orientation)
    {
	Logger.D(TAG, "setScreenOrientation -- START");
	Logger.D(TAG, "setScreenOrientation -- orientation: " + orientation);
	Map<String, Object> values = new HashMap<String, Object>();
	String value = null;
	if (orientation.equalsIgnoreCase("normal"))
	{
	    if (getNaturalDeviceOrientation() == Configuration.ORIENTATION_LANDSCAPE)
		RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	    else
		RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
	    isAutoRotate = false;
	    value = "normal";
	}
	else if (orientation.equalsIgnoreCase("righthanded"))
	{
	    if (getNaturalDeviceOrientation() == Configuration.ORIENTATION_LANDSCAPE)
		RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
	    else
		RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
	    isAutoRotate = false;
	    value = "righthanded";
	}
	else if (orientation.equalsIgnoreCase("lefthanded"))
	{
	    if (getNaturalDeviceOrientation() == Configuration.ORIENTATION_LANDSCAPE)
		RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
	    else
		RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	    isAutoRotate = false;
	    value = "lefthanded";
	}
	else if (orientation.equalsIgnoreCase("upsidedown"))
	{
	    if (getNaturalDeviceOrientation() == Configuration.ORIENTATION_LANDSCAPE)
		RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
	    else
		RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
	    isAutoRotate = false;
	    value = "upsidedown";
	}
	if (value != null)
	{
	    direction = value;
	    values.put("currentOrientation", direction);
	    Logger.D(TAG, "Setting currentOrientation in mScreenOrientationCallback: " + mScreenOrientationCallback + "with value: " + values.get("currentOrientation"));
	    if (mScreenOrientationCallback != null)
		mScreenOrientationCallback.set(values);
	}
    }
	
    private void setDirection()
    {
	try
	{
	    Display display = ((WindowManager) RhodesActivity.safeGetInstance().getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
	    switch (display.getRotation())
	    {
        	case(Surface.ROTATION_0):
       		{
        	    direction = "normal";
        	    break;
        	}
        	case(Surface.ROTATION_90):
        	{
        	    direction = "lefthanded";
        	    break;
        	}
        	case(Surface.ROTATION_180):
        	{
        	    direction = "upsidedown";
        	    break;
        	}
        	case(Surface.ROTATION_270):
        	{
        	    direction = "righthanded";
        	    break;
        	}
        	default:
        	    direction = "normal";
        	}
	    if(lastDirection != direction)
	    {
		lastDirection = direction;
		Map<String, Object> values = new HashMap<String, Object>();
		values.put("currentOrientation", direction);
		Logger.D(TAG, "Setting currentOrientation in mScreenOrientationCallback: " + mScreenOrientationCallback + "with value: " + values.get("currentOrientation"));
		if (mScreenOrientationCallback != null)
		    mScreenOrientationCallback.set(values);
	    }
	}
	catch (NullPointerException e)
	{
	    Logger.E(TAG, e.getMessage());
	}
    }

    @Override
    public void setScreenOrientationEvent(IMethodResult result)
    {
	Logger.D(TAG, "setScreenOrientationEvent() -- Setting screen orientation event to: " + result);
	mScreenOrientationCallback = result;
	if (result == null)
	{
	    cleanUp();
	}
	else
	{
	    // Register SensorListener's for Magnetic Field and Orientation
	    mSensorManager.registerListener(mSensorListener, mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
	}
    }
	
    public void cleanUp()
    {
	Logger.D(TAG, "cleanUp -- START");
	// Un-Register SensorListener's
	mSensorManager.unregisterListener(mSensorListener);
    }
	
    private static int getNaturalDeviceOrientation()
    {
	Logger.D(TAG, "getNaturalDeviceOrientation -- START");
	WindowManager lWindowManager = (WindowManager) RhodesActivity.safeGetInstance().getSystemService(Context.WINDOW_SERVICE);

	Configuration cfg = RhodesActivity.safeGetInstance().getResources().getConfiguration();
	int lRotation = lWindowManager.getDefaultDisplay().getRotation();

	if ( (((lRotation == Surface.ROTATION_0) ||(lRotation == Surface.ROTATION_180)) &&   
		(cfg.orientation == Configuration.ORIENTATION_LANDSCAPE)) ||
		(((lRotation == Surface.ROTATION_90) ||(lRotation == Surface.ROTATION_270)) &&    
			(cfg.orientation == Configuration.ORIENTATION_PORTRAIT)))
	{
	    return Configuration.ORIENTATION_LANDSCAPE;
	}     

	return Configuration.ORIENTATION_PORTRAIT;
    }
	
    @Override
    public void getAllProperties(IMethodResult result) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void setProperty(String propertyName, String propertyValue,
	    IMethodResult result) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void setProperties(Map<String, String> propertyMap,
	    IMethodResult result) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void clearAllProperties(IMethodResult result) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void getProperty(String propertyName, IMethodResult result) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void getProperties(List<String> arrayofNames, IMethodResult result) {
	// TODO Auto-generated method stub
	
    }
}