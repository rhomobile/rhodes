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

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class GeoLocation {

	private static final String TAG = "GeoLocation";
	private static final String inactivityTimeoutName = "geo_location_inactivity_timeout"; 
	private static final String updatePeriodName = "gps_ping_timeout_sec"; 
	private static volatile GeoLocationImpl locImpl = null;
	private static volatile int inactivityTimerId = 0;
	private static long updatePeriod = -1;
	
	static long getInactivityTimeout() {
		long upd = getUpdatePeriod();
		long ret = 0;
		if (RhoConf.isExist(inactivityTimeoutName))
			ret = RhoConf.getInt(inactivityTimeoutName) * 1000;

		if (ret <= upd) {
			ret = upd*10;
			Logger.W(TAG, "Inactivity time out less then position update period, override configured value: " + ret);
		}
		
		return ret;
	}
	
	static long getUpdatePeriod() {
		if (updatePeriod != -1) {
			return updatePeriod; 
		}
		if (RhoConf.isExist(updatePeriodName)) {
			long ret = RhoConf.getInt(updatePeriodName) * 1000;
			return ret;
		} else
			return 30000; // 30 sec
	}
	
	private static void checkState() throws IllegalAccessException {
		if (!Capabilities.GPS_ENABLED)
			throw new IllegalAccessException("Capability GPS disabled");
	}
	
	private static void updateInactivityTimer() {
		final long inactivityTimeout = getInactivityTimeout();
		Logger.T(TAG, "Updating inactivity timer: " + inactivityTimeout + "ms");
		PerformOnUiThread.exec(new Runnable() {
			int lastId = ++inactivityTimerId;
			public void run() {
				if ((inactivityTimerId != lastId) || (locImpl == null))
					return;

				Logger.T(TAG, "Stop geolocation by inactivity timeout: " + inactivityTimeout + "ms");
				stop();
			}
		}, inactivityTimeout);
	}

	private static GeoLocationImpl getImpl() {
		if (locImpl == null) {
			synchronized (GeoLocation.class) {
				if (locImpl == null) {
					Logger.T(TAG, "Creating GeoLocationImpl instance.");
					locImpl = new GeoLocationImpl(getUpdatePeriod());
					Logger.T(TAG, "GeoLocationImpl instance has created.");
					locImpl.start();
					Logger.T(TAG, "GeoLocation has started.");
				}
			}
		}
		updateInactivityTimer();
		return locImpl;
	}
	
	public static void stop() {
		Logger.T(TAG, "stop");
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

    public static void restart() {
        try {
            checkState();
            Logger.T(TAG, "restart");
            if (updatePeriod != -1)
                getImpl().setTimeout(getUpdatePeriod());
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
			checkState();
			Logger.T(TAG, "getLatitude");
			return getImpl().getLatitude();
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		return 0.0;
	}

	public static double getLongitude() {
		try {
			checkState();
			Logger.T(TAG, "getLongitude");
			return getImpl().getLongitude();
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		return 0.0;
	}
	
	public static float getAccuracy() {
		try {
			checkState();
			Logger.T(TAG, "getAccuracy");
			return getImpl().getAccuracy();
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		return 0;
	}

	public static boolean isKnownPosition() {
		try {
			checkState();
			Logger.T(TAG, "isKnownPosition");
			return getImpl().isKnownPosition();
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		return false;
	}
	
	public static void setTimeout(int nsec) {
		try {
			if (nsec < 0) {
				Logger.E(TAG, "setTimeout: wrong parameter: " + nsec);
				return;
			}
			
			checkState();
			Logger.T(TAG, "setTimeout: " + nsec + "s");
			updatePeriod = nsec * 1000;
			getImpl().setTimeout(getUpdatePeriod());
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
	}

}
