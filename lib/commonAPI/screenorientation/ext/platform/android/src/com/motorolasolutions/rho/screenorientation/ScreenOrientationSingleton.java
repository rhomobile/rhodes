package com.motorolasolutions.rho.screenorientation;

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
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;

class ScreenOrientationSingleton implements IScreenOrientationSingleton
{
	private static final String UPSIDE_DOWN = "upsideDown";
	private static final String LEFT_HANDED = "leftHanded";
	private static final String RIGHT_HANDED = "rightHanded";
	private static final String NORMAL = "normal";
	private static String TAG = ScreenOrientationSingleton.class.getSimpleName();
	private IMethodResult mScreenOrientationCallback;
	private SensorManager mSensorManager;
	private static boolean isAutoRotate;
	private static String direction;
	private static String lastDirection = "";
	public static boolean m_Paused=false;
	/**
	 * Monitors the Orientation sensor to determine which way up the device is also takes care of
	 * upside down which is not handled by the default Sensors using the Magnetic Field Sensor as
	 * well as Orientation.
	 */
	private final SensorEventListener mSensorListener = new SensorEventListener()
	{
		@Override
		public void onAccuracyChanged(Sensor arg0, int arg1)
		{
		}

		@Override
		public void onSensorChanged(SensorEvent arg0)
		{
			setDirection();
		}
	};

	public ScreenOrientationSingleton()
	{
		super();
		mSensorManager = (SensorManager) RhodesActivity.safeGetInstance().getSystemService("sensor");
		Logger.D(TAG, "ScreenOrientation -- disable_screen_rotation: " + RhoConf.getString("disable_screen_rotation"));
		isAutoRotate = !(RhoConf.getString("disable_screen_rotation").compareTo("1") == 0);
		ScreenOrientationRhoListener.addScreenOrientationInstance(this);
		Logger.D(TAG, "ScreenOrientationSingleton object constructed correctly");
	}

	@Override
	public void setAutoRotate(boolean autoRotate, IMethodResult result)
	{
	    Logger.D(TAG, "setAutorotate -- START");
	    Logger.D(TAG, "setAutorotate -- autoRotate: " + autoRotate);
	    if (RhodesActivity.safeGetInstance() == null)
		return;
	    if (autoRotate)
	    {
		RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
		// This is because when used with SignatureCapture, enabling does not take effect after taking signature
		RhodesActivity.safeGetInstance().setScreenAutoRotateMode(true);
		isAutoRotate = true;
		RhoConf.setString("disable_screen_rotation", "0");
	    }
	    else
	    {
		setDirection();
		setScreenOrientation(direction);
		isAutoRotate = false;
		RhoConf.setString("disable_screen_rotation", "1");
	    }
	    Logger.D(TAG, "setAutorotate -- END");
	}

	@Override
	public void getAutoRotate(IMethodResult result)
	{
	    Logger.D(TAG, "getAutorotate -- START");
	    boolean ret = false;
	    if (isAutoRotate)
		ret = true;
	    else
		ret = false;
	    result.set(ret);
	    Logger.D(TAG, "getAutorotate -- return: " + ret);
	}

	@Override
	public void normal(IMethodResult result)
	{
		setScreenOrientation(NORMAL);
	}

	@Override
	public void rightHanded(IMethodResult result)
	{
		setScreenOrientation(RIGHT_HANDED);
	}

	@Override
	public void leftHanded(IMethodResult result)
	{
		setScreenOrientation(LEFT_HANDED);
	}

	@Override
	public void upsideDown(IMethodResult result)
	{
		setScreenOrientation(UPSIDE_DOWN);
	}

	private void setScreenOrientation(final String orientation)
	{
		Logger.D(TAG, "setScreenOrientation -- START");
		Logger.D(TAG, "setScreenOrientation -- orientation: " + orientation);
		RhodesActivity.safeGetInstance().runOnUiThread(new Runnable() {
		    
		    @Override
		    public void run() {
			String value = null;
			if (orientation.equalsIgnoreCase(NORMAL))
			{
				if (getNaturalDeviceOrientation() == Configuration.ORIENTATION_LANDSCAPE)
					RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
				else
					RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
				isAutoRotate = false;
				value = NORMAL;
			}
			else if (orientation.equalsIgnoreCase(RIGHT_HANDED))
			{
				if (getNaturalDeviceOrientation() == Configuration.ORIENTATION_LANDSCAPE)
					RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
				else
					RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
				isAutoRotate = false;
				value = RIGHT_HANDED;
			}
			else if (orientation.equalsIgnoreCase(LEFT_HANDED))
			{
				if (getNaturalDeviceOrientation() == Configuration.ORIENTATION_LANDSCAPE)
					RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
				else
					RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
				isAutoRotate = false;
				value = LEFT_HANDED;
			}
			else if (orientation.equalsIgnoreCase(UPSIDE_DOWN))
			{
				if (getNaturalDeviceOrientation() == Configuration.ORIENTATION_LANDSCAPE)
					RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
				else
					RhodesActivity.safeGetInstance().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
				isAutoRotate = false;
				value = UPSIDE_DOWN;
			}
			Logger.D(TAG, "setScreenOrientation -- value to return: " + value);
			Logger.D(TAG, "setScreenOrientation -- callback: " + mScreenOrientationCallback);
			if (value != null)
			{
				direction = value;
				
				//EMBPD00154917 fix start-Resolve 2 callbacks being fired when screenorientatation is set
				//Logger.D(TAG, "Setting currentOrientation in mScreenOrientationCallback: " + mScreenOrientationCallback + "with value: " + direction);
				
				//if (mScreenOrientationCallback != null)
					//mScreenOrientationCallback.set(direction);
					
					
				//EMBPD00154917 fix end-Resolve 2 callbacks being fired when screenorientatation is set	
			}
		    }
		});
		
	}

	private void setDirection()
	{
		try
		{
			Display display = ((WindowManager) RhodesActivity.safeGetInstance().getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
			switch (display.getRotation())
			{
				case (Surface.ROTATION_0):
				{
					direction = NORMAL;
					break;
				}
				case (Surface.ROTATION_90):
				{
					direction = LEFT_HANDED;
					break;
				}
				case (Surface.ROTATION_180):
				{
					direction = UPSIDE_DOWN;
					break;
				}
				case (Surface.ROTATION_270):
				{
					direction = RIGHT_HANDED;
					break;
				}
				default:
					direction = NORMAL;
			}
			if (lastDirection.compareToIgnoreCase(direction) != 0)
			{
				lastDirection = direction;
				if(m_Paused==false)//
				{
				Logger.D(TAG, "Setting currentOrientation in mScreenOrientationCallback: " + mScreenOrientationCallback + "with value: " + direction);
				if (mScreenOrientationCallback != null)
					mScreenOrientationCallback.set(direction);
				}
				else
				{
				Logger.I(TAG, "Activiy is paused.Do not fire screenorientation event");	
				}
					
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
		if (result == null || !result.hasCallback()) // No callback is set
		{
			cleanUp();
		}
		else
		{
			// Register SensorListener's for Magnetic Field and Orientation
			mSensorManager.registerListener(mSensorListener, mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
					SensorManager.SENSOR_DELAY_NORMAL);
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
		if (RhodesActivity.safeGetInstance() == null)
			return 0; // No valid natural orientation returned
		WindowManager lWindowManager = (WindowManager) RhodesActivity.safeGetInstance().getSystemService(Context.WINDOW_SERVICE);

		Configuration cfg = RhodesActivity.safeGetInstance().getResources().getConfiguration();
		int lRotation = lWindowManager.getDefaultDisplay().getRotation();

		if ((((lRotation == Surface.ROTATION_0) || (lRotation == Surface.ROTATION_180)) && (cfg.orientation == Configuration.ORIENTATION_LANDSCAPE))
				|| (((lRotation == Surface.ROTATION_90) || (lRotation == Surface.ROTATION_270)) && (cfg.orientation == Configuration.ORIENTATION_PORTRAIT))) { return Configuration.ORIENTATION_LANDSCAPE; }

		return Configuration.ORIENTATION_PORTRAIT;
	}
}
