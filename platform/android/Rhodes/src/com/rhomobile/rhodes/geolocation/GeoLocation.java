/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/
package com.rhomobile.rhodes.geolocation;

import java.util.Map;

import android.graphics.Color;
import android.location.Location;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.util.Utils;

public class GeoLocation {

	private static final String TAG = "GeoLocation";
	private static volatile GeoLocationImpl locImpl = null;
	
	private static final String CALLBACK_UPDATE_INTERVAL = "gps_ping_timeout_sec";
	
	private static double ourLatitude = 0;
	private static double ourLongitude = 0;
	private static double ourAltitude = 0;
	private static double ourAccuracy = 0;
	private static double ourSpeed = 0;
	private static int ourSatellities = 0;
	
	private static boolean ourIsKnownPosition = false;
	private static boolean ourIsEnable = true;
	private static boolean ourIsErrorState = false;
	
	public static native void geoCallback();
	public static native void geoCallbackError();
	
	
	public static void onUpdateLocation() {
		Location loc = getImpl().getLocation();
		synchronized (GeoLocation.class) {
			if (loc != null) {
				ourLatitude = loc.getLatitude();
				ourLongitude = loc.getLongitude();
				ourAltitude = loc.getAltitude();
				ourAccuracy = loc.getAccuracy();
				ourSpeed = loc.getSpeed();
				//The  way of getiing satellites is changed
				//ourSatellities = getImpl().getSatellities();
				android.os.Bundle extras = loc.getExtras();
				if (extras != null) {
					ourSatellities = extras.getInt("satellites");
				} else {
					ourSatellities = 0;
				}
				ourIsKnownPosition = true;
			}
			else {
				ourLatitude = 0;
				ourLongitude = 0;
				ourAltitude = 0;
				ourAccuracy = 0;
				ourSpeed = 0;
				ourSatellities = 0;
				ourIsKnownPosition = false;
			}
		}
	}
	
	public static void onGeoCallback() {
		Logger.T(TAG, "onGeoCallback()");
		if (ourIsEnable && !ourIsErrorState) {
			Logger.T(TAG, "onGeoCallback() run native Callback");
			geoCallback();
		}
		else {
			Logger.T(TAG, "onGeoCallback() SKIP");
		}
	}
	
	public static void onGeoCallbackError() {
		if (ourIsEnable && !ourIsErrorState) {
			Logger.T(TAG, "onGeoCallbackError() run native Callback");
			ourIsErrorState = true;
			geoCallbackError();
			ourIsErrorState = false;
		}
		else {
			Logger.T(TAG, "onGeoCallbackError() SKIP");
		}
	}
	
	private static Thread ourCallbackThread = null;
	
	private static void checkState() throws IllegalAccessException {
		if (!Capabilities.GPS_ENABLED)
			throw new IllegalAccessException("Capability GPS disabled");
	}
	
	private static GeoLocationImpl getImpl() {
		if (locImpl == null) {
			synchronized (GeoLocation.class) {
				if (locImpl == null) {
					Logger.T(TAG, "Creating GeoLocationImpl instance.");
					locImpl = new GeoLocationImpl();
					Logger.T(TAG, "GeoLocationImpl instance has created.");
					//ourIsFirstUpdate = true;
					locImpl.start();
					Logger.T(TAG, "GeoLocation has started.");
				}
			}
		}
		return locImpl;
	}
	
	public static void stop() {
		Logger.T(TAG, "stop");
		ourIsEnable = false;
		resetCallbackThread(0);
		try {
			if (locImpl == null)
				return;
			synchronized(GeoLocation.class) {
				if (locImpl == null)
					return;
				locImpl.stop();
				locImpl = null;
			}
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
	}

	public static boolean isAvailable() {
		Logger.T(TAG, "isAvailable...");
		try {
			boolean result = false;
			if (locImpl != null) {
				checkState();
				result = getImpl().isAvailable();
			}
			Logger.T(TAG, "Geo location service is " + (result ? "" : "not ") + "available");
			return result;
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		return false;
	}
	
	public static double getLatitude() {		
		try {
			onUpdateLocation();
			checkState();
			Logger.T(TAG, "getLatitude");
			return ourLatitude;
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		return 0.0;
	}
	
	public static double getSpeed() {		
		try {
			onUpdateLocation();
			checkState();
			Logger.T(TAG, "getSpeed");
			return ourSpeed;
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		return 0.0;
	}
	
	public static int getSatellities() {		
		try {
			onUpdateLocation();
			checkState();
			Logger.T(TAG, "getSatellities");
			return ourSatellities;
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		return 0;
	}
	

	public static double getLongitude() {		
		try {
			onUpdateLocation();
			checkState();
			Logger.T(TAG, "getLongitude");
			return ourLongitude;
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		return 0.0;
	}
	
	public static double getAltitude() {		
		try {
			onUpdateLocation();
			checkState();
			Logger.T(TAG, "getAltitude");
			return ourAltitude;
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		return 0.0;
	}
	
	public static float getAccuracy() {		
		try {
			onUpdateLocation();
			checkState();
			Logger.T(TAG, "getAccuracy");
			return (float)ourAccuracy;
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		return 0;
	}

	public static boolean isKnownPosition() {		
		try {
			onUpdateLocation();
			checkState();
			Logger.T(TAG, "isKnownPosition");
			return ourIsKnownPosition;
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		return false;
	}
	
	private static void resetCallbackThread(int period) {
		Logger.T(TAG, "resetCallbackThread: " + period + "s");
		if (ourCallbackThread != null) {
			ourCallbackThread.interrupt();
			ourCallbackThread = null;
		}
		if (period > 0) {
			final int sleep_period = period;
			ourCallbackThread = new Thread(new Runnable() {
				private boolean ourLastCommandProcessed = true;
				public void run() {
					Logger.I(TAG, "\"callback\" thread started");
					for (;;) {
						if (!ourIsEnable) {
							break;
						}
						try {
							if (ourLastCommandProcessed) {
								ourLastCommandProcessed = false;
								Logger.T(TAG, "callback thread: perform callback in UI thread");
								PerformOnUiThread.exec(new Runnable() {
									public void run() {
										Logger.T(TAG, "callback thread: callback in UI thread START");
										onGeoCallback();
										Logger.T(TAG, "callback thread: callback in UI thread FINISH");
										ourLastCommandProcessed = true;
									}
								});
							}
							else {
								Logger.T(TAG, "callback thread: previous command not processed - skip current callback");
							}
							Thread.sleep(sleep_period);
						}
						catch (InterruptedException e) {
							Logger.T(TAG, "\"callback\" thread interrupted");
							break;
						}
					}
				}
			});	
			ourCallbackThread.start();
		}
		else {
			Logger.T(TAG, "resetCallbackThread: zero period - not make any thread");
		}
		
	}
	
	public static void setNotificationEx(Object options) {
		if ((options != null) && (options instanceof Map<?,?>)) 
		{
			Map<Object,Object> settings = (Map<Object,Object>)options;
			
			double minDistance = 10;
			int minTime = 1000;
			
			
			Object minDistObj = settings.get("minimumDistance");
			Object minTimeObj = settings.get("minimumTimeInterval");
			
			
			try {
				checkState();
			}
			catch (Exception e) {
	            Logger.E(TAG, e);
			}			ourIsEnable = true;

			if ((minDistObj != null) && (minDistObj instanceof String)) {
				minDistance = Double.parseDouble(((String)minDistObj));
			}
			if ((minTimeObj != null) && (minTimeObj instanceof String)) {
				minTime = Integer.parseInt(((String)minTimeObj));
			}
			
			getImpl().restartEx(minDistance, minTime);
			
		}		
	}
	
	public static void setTimeout(int nsec) {
		try {
			int p = nsec*1000;
			if (p < 0) {
				p = RhoConf.getInt(CALLBACK_UPDATE_INTERVAL) * 1000;
			}
			
			checkState();
			Logger.T(TAG, "setTimeout: " + nsec + "s");
		
			ourIsEnable = true;
			if (p <= 0) {
				p = 250;
			}
			resetCallbackThread(p);
			getImpl().restartNormal();
		
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
	}

}
