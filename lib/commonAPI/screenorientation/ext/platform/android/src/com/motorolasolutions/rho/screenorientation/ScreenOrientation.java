package com.motorolasolutions.rho.screenorientation;

import java.util.HashMap;
import java.util.Map;

import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Rect;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;

import com.rho.screenorientation.IScreenOrientation;
import com.rho.screenorientation.ScreenOrientationBase;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

public class ScreenOrientation extends ScreenOrientationBase 
	implements IScreenOrientation, IRhoListener
{
    private String TAG = ScreenOrientation.class.getSimpleName();
    private IMethodResult mScreenOrientationCallback;
    private SensorManager mSensorManager;
    private boolean mIsAutoRotate;
    private String mDirection;
    private String mLastDirection = "Empty";
    
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
    
    public ScreenOrientation(String id)
    {
	super(id);
	mSensorManager = (SensorManager) RhodesActivity.safeGetInstance().getSystemService("sensor");
	
	mIsAutoRotate = true;
        Logger.D(TAG, "ScreenOrientation object constructed correctly");
    }
    
    @Override
    public void setAutoRotate(String autoRotate, IMethodResult result)
    {
	Logger.D(TAG, "setAutorotate -- START");
	Logger.D(TAG, "setAutorotate -- autoRotate: " + autoRotate);
	if (autoRotate.equalsIgnoreCase("enabled"))
	{
	    RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
	    mIsAutoRotate = true;
	}
	else if (autoRotate.equalsIgnoreCase("disabled"))
	{
	    setDirection();
	    setScreenOrientation(mDirection);
	    mIsAutoRotate = false;
	}
	else
	{
	    Logger.D(TAG, "setAutorotate -- autoRotate value is invalid: " + autoRotate);
	    return;
	}
	super.setAutoRotate(autoRotate, result);
	Logger.D(TAG, "setAutorotate -- END");
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
	    RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	    mIsAutoRotate = false;
	    value = "normal";
	}
	else if (orientation.equalsIgnoreCase("righthanded"))
	{
	    RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
	    mIsAutoRotate = false;
	    value = "righthanded";
	}
	else if (orientation.equalsIgnoreCase("lefthanded"))
	{
	    RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
	    mIsAutoRotate = false;
	    value = "lefthanded";
	}
	else if (orientation.equalsIgnoreCase("upsidedown"))
	{
	    RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
	    mIsAutoRotate = false;
	    value = "upsidedown";
	}
	if (value != null)
	{
	    mDirection = value;
	    values.put("currentOrientation", mDirection);
	    Logger.D(TAG, "Setting currentOrientation in mScreenOrientationCallback: " + mScreenOrientationCallback + "with value: " + values.get("currentOrientation"));
	    if (mScreenOrientationCallback != null)
		mScreenOrientationCallback.set(values);
	}
    }
    
    private void setDirection()
    {
	Display display = ((WindowManager) RhodesActivity.safeGetInstance().getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
	switch (display.getRotation())
	{
		case(Surface.ROTATION_0):
		{
		    mDirection = "normal";
		    break;
		}
		case(Surface.ROTATION_90):
		{
		    mDirection = "lefthanded";
		    break;
		}
		case(Surface.ROTATION_180):
		{
		    mDirection = "upsidedown";
		    break;
		}
		case(Surface.ROTATION_270):
		{
		    mDirection = "righthanded";
		    break;
		}
		default:
		    mDirection = "normal";
	}
	if(mLastDirection != mDirection)
	{
	    mLastDirection = mDirection;
	    Map<String, Object> values = new HashMap<String, Object>();
	    values.put("currentOrientation", mDirection);
	    Logger.D(TAG, "Setting currentOrientation in mScreenOrientationCallback: " + mScreenOrientationCallback + "with value: " + values.get("currentOrientation"));
	    if (mScreenOrientationCallback != null)
		mScreenOrientationCallback.set(values);
	}
    }
    
    @Override
    public void setScreenOrientationEvent(IMethodResult result)
    {
	mScreenOrientationCallback = result;
	if (result == null)
	{
	    unregisterSensorListeners();
	}
	else
	{
	    // Register SensorListener's for Magnetic Field and Orientation
	    mSensorManager.registerListener(mSensorListener, mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
	}
	
    } 
    
    private void unregisterSensorListeners()
    {
	// Un-Register SensorListener's
	mSensorManager.unregisterListener(mSensorListener);
    }

    @Override
    public void onCreate(RhodesActivity activity, Intent intent)
    {
	Logger.D(TAG, "onCreate -- START");
    }

    @Override
    public void onStart(RhodesActivity activity)
    {
	Logger.D(TAG, "onStart -- START");
	//mOrientationPlugin = new OrientationPlugin(activity);
    }

    @Override
    public void onResume(RhodesActivity activity) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void onPause(RhodesActivity activity) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void onStop(RhodesActivity activity) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void onDestroy(RhodesActivity activity) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void onNewIntent(RhodesActivity activity, Intent intent) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void onActivityResult(RhodesActivity activity, int requestCode,
	    int resultCode, Intent intent) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public Dialog onCreateDialog(RhodesActivity activity, int id) {
	// TODO Auto-generated method stub
	return null;
    }

    @Override
    public void onConfigurationChanged(RhodesActivity activity,
	    Configuration newConfig) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
	// TODO Auto-generated method stub
	
    }
}