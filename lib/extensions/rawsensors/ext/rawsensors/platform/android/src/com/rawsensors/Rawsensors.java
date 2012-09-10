package com.rawsensors;



import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.content.Context;
import android.hardware.SensorManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;



public class Rawsensors {

	public final static String RAWSENSORS_SERVICE_ACCELEROMETER = "accelerometer";
	public final static String RAWSENSORS_SERVICE_MAGNETOMETER = "magnetometer";
	public final static String RAWSENSORS_SERVICE_ALL = "all";
	
	public final static String RAWSENSORS_UPDATE_PERIOD = "minimumInterval";
	public final static String RAWSENSORS_CALLBACK_URL = "sensorEvent";
	
	
	private static Rawsensors ourInstance = null;
	
	public static Rawsensors getInstance() {
		if (ourInstance == null) {
			ourInstance = new Rawsensors();
		}
		return ourInstance;
	}
	
	private class AccelerometerSensorEventListener implements SensorEventListener {
		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {
		}

		@Override
		public void onSensorChanged(SensorEvent event) {
			getInstance().onAccelerometer(event.values[0], event.values[1], event.values[2]);
		}
		
	}
	
	private class MagnetometerSensorEventListener implements SensorEventListener {
		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {
		}

		@Override
		public void onSensorChanged(SensorEvent event) {
			getInstance().onMagnetometer(event.values[0], event.values[1], event.values[2]);
		}
		
	}
	
	public Rawsensors() {
		//PerformOnUiThread.exec(new Runnable() {
		//	public void run() {	
				mSensorManager = (SensorManager)RhodesService.getContext().getSystemService(Context.SENSOR_SERVICE);
				mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
				mMagnetometer = mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
				mAccelerometerListener = new AccelerometerSensorEventListener();
				mMagnetometerListener = new MagnetometerSensorEventListener();
		//	}}, true);
	}

	private double mAccelerometerX = 0.0;
	private double mAccelerometerY = 0.0;
	private double mAccelerometerZ = 0.0;

	private double mMagnetometerX = 0.0;
	private double mMagnetometerY = 0.0;
	private double mMagnetometerZ = 0.0;
	
	private boolean mEnableAccelerometer = false;
	private boolean mEnableMagnetometer = false;
	private boolean mEnableAll = false;
	
	private int mUpdatePeriod = 1000;
	
	private String mCallbackURL = null;
	
	private SensorManager mSensorManager;
    private Sensor mAccelerometer;
    private Sensor mMagnetometer;
    
    private SensorEventListener mAccelerometerListener = null;
    private SensorEventListener mMagnetometerListener = null;
    
    private Thread mUpdateThread = null;
    
    private final static String false_string = "false";
	
	public static void setProperty(String property_name, String value) {
		try {
		boolean need_resetup_thread = false;
		if (RAWSENSORS_SERVICE_ACCELEROMETER.equals(property_name) ) {
			need_resetup_thread = true;
			if (false_string.equals(value)) {
				getInstance().mEnableAccelerometer = false;
			}
			else {
				getInstance().mEnableAccelerometer = true;
			}
		}
		if (RAWSENSORS_SERVICE_MAGNETOMETER.equals(property_name) ) {
			need_resetup_thread = true;
			if (false_string.equals(value)) {
				getInstance().mEnableMagnetometer = false;
			}
			else {
				getInstance().mEnableMagnetometer = true;
			}
		}
		if (RAWSENSORS_UPDATE_PERIOD.equals(property_name) ) {
			need_resetup_thread = true;
			int per = Integer.parseInt(value);
			if (per < 200) {
				per = 200;
			}
			getInstance().mUpdatePeriod = per;
		}
		if (RAWSENSORS_SERVICE_ALL.equals(property_name) ) {
			need_resetup_thread = true;
			if (false_string.equals(value)) {
				getInstance().mEnableAll = false;
			}
			else {
				getInstance().mEnableAll = true;
			}
		}
		if (RAWSENSORS_CALLBACK_URL.equals(property_name) ) {
			getInstance().mCallbackURL = value;
		}
		if (need_resetup_thread) {
			getInstance().stopUpdates();
			getInstance().startUpdates();
		}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	// 1 - string
	// 2 - bool
	// 3 - int
	public static int getPropertyType(String property_name) {
		if (RAWSENSORS_SERVICE_ACCELEROMETER.equals(property_name) ) {
			return 2;
		}
		if (RAWSENSORS_SERVICE_MAGNETOMETER.equals(property_name) ) {
			return 2;
		}
		if (RAWSENSORS_UPDATE_PERIOD.equals(property_name) ) {
			return 3;
		}
		if (RAWSENSORS_SERVICE_ALL.equals(property_name) ) {
			return 2;
		}
		if (RAWSENSORS_CALLBACK_URL.equals(property_name) ) {
			return 1;
		}
		return 1;
	}
	
	public static int getIntProperty(String property_name) {
		if (RAWSENSORS_SERVICE_ACCELEROMETER.equals(property_name) ) {
			if (getInstance().mEnableAccelerometer) {
				return 1;
			}
			return 0;
		}
		if (RAWSENSORS_SERVICE_MAGNETOMETER.equals(property_name) ) {
			if (getInstance().mEnableMagnetometer) {
				return 1;
			}
			return 0;
		}
		if (RAWSENSORS_UPDATE_PERIOD.equals(property_name) ) {
			return getInstance().mUpdatePeriod;
		}
		if (RAWSENSORS_SERVICE_ALL.equals(property_name) ) {
			if (getInstance().mEnableAll) {
				return 1;
			}
			return 0;
		}
		return 0;
	}

	public static String getStringProperty(String property_name) {
		if (RAWSENSORS_SERVICE_ACCELEROMETER.equals(property_name) ) {
			if (getInstance().mEnableAccelerometer) {
				return "true";
			}
			return "false";
		}
		if (RAWSENSORS_SERVICE_MAGNETOMETER.equals(property_name) ) {
			if (getInstance().mEnableMagnetometer) {
				return "true";
			}
			return "false";
		}
		if (RAWSENSORS_UPDATE_PERIOD.equals(property_name) ) {
			return String.valueOf(getInstance().mUpdatePeriod);
		}
		if (RAWSENSORS_SERVICE_ALL.equals(property_name) ) {
			if (getInstance().mEnableAll) {
				return "true";
			}
			return "false";
		}
		if (RAWSENSORS_CALLBACK_URL.equals(property_name) ) {
			return getInstance().mCallbackURL;
		}
		return null;
	}
	
	
	public static int isAvailable(String service_name) {
		if (RAWSENSORS_SERVICE_ACCELEROMETER.equals(service_name)) {
			if (getInstance().mAccelerometer != null) {
				return 1;
			}
		}
		if (RAWSENSORS_SERVICE_MAGNETOMETER.equals(service_name)) {
			if (getInstance().mMagnetometer != null) {
				return 1;
			}
		}
		return 0;
	}
	
    public void onAccelerometer(double x, double y, double z ){
		mAccelerometerX = x;
		mAccelerometerY = y;
		mAccelerometerZ = z;
    }
	
	public void onMagnetometer(double x, double y, double z) {
		mMagnetometerX = x;
		mMagnetometerY = y;
		mMagnetometerZ = z;
	}
	
	public void startUpdates() {
		
		//PerformOnUiThread.exec(new Runnable() {
		//	public void run() {	
				if ((mAccelerometer != null) && (mEnableAccelerometer || mEnableAll)) {
					mSensorManager.unregisterListener(mAccelerometerListener, mAccelerometer);
					mSensorManager.registerListener(mAccelerometerListener, mAccelerometer, SensorManager.SENSOR_DELAY_NORMAL);
				}
				if ((mMagnetometer != null) && (mEnableMagnetometer || mEnableAll)) {
					mSensorManager.unregisterListener(mMagnetometerListener, mMagnetometer);
					mSensorManager.registerListener(mMagnetometerListener, mMagnetometer, SensorManager.SENSOR_DELAY_NORMAL);
				}
				if (mUpdateThread != null) {
					//mUpdateThread.stop();
					mUpdateThread.interrupt();
					mUpdateThread = null;
				}
				mUpdateThread = new Thread() {
					public void run() {
						while (true) {
							try {
								getInstance().onUpdate();
								sleep(getInstance().mUpdatePeriod);
							} 
							catch (InterruptedException e) {
								return;
							}
							catch (Exception e) {
								e.printStackTrace();
							}
						}
					}
				};
				mUpdateThread.start();
		//	}
		//});
	}
	
	public void stopUpdates() {
		//PerformOnUiThread.exec(new Runnable() {
		//	public void run() {	
				if ((mAccelerometer != null) ) {
					mSensorManager.unregisterListener(mAccelerometerListener, mAccelerometer);
				}
				if ((mMagnetometer != null) ) {
					mSensorManager.unregisterListener(mMagnetometerListener, mMagnetometer);
				}
				if (mUpdateThread != null) {
					//mUpdateThread.stop();
					mUpdateThread.interrupt();
					mUpdateThread = null;
				}		
		//	}});
	}
	
	public void onUpdate() {
		if ((mAccelerometer == null) || (!mEnableAccelerometer && !mEnableAll)) {
			if ((mMagnetometer == null) || (!mEnableMagnetometer && !mEnableAll)) {
				return;
			}
		}
		StringBuffer strBody = new StringBuffer("&rho_callback=1&status=OK");
		if (mEnableAccelerometer || mEnableAll) {
			strBody = strBody.append("&accelerometerX=");
			strBody = strBody.append(mAccelerometerX);
			strBody = strBody.append("&accelerometerY=");
			strBody = strBody.append(mAccelerometerY);
			strBody = strBody.append("&accelerometerZ=");
			strBody = strBody.append(mAccelerometerZ);
		}
		if (mEnableMagnetometer || mEnableAll) {
			strBody = strBody.append("&magnetometerX=");
			strBody = strBody.append(mMagnetometerX);
			strBody = strBody.append("&magnetometerY=");
			strBody = strBody.append(mMagnetometerY);
			strBody = strBody.append("&magnetometerZ=");
			strBody = strBody.append(mMagnetometerZ);
		}
		fireRhoCallback(mCallbackURL, strBody.toString());
	}
	
	public static void getSensorData() {
		getInstance().onUpdate();
	}
	
	
	private static native void fireRhoCallback(String url, String body);

}
