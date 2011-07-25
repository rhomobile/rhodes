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
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class GeoLocation {

	private static final String TAG = "GeoLocation";
	private static GeoLocationImpl locImpl = null;
	
	private static int mInactivityTimerId = 0;
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	private static void checkState() throws IllegalAccessException {
		if (!Capabilities.GPS_ENABLED)
			throw new IllegalAccessException("Capability GPS disabled");
	}
	
	private static void updateInactivityTimer() {
		final int id = ++mInactivityTimerId;
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				if (id != mInactivityTimerId)
					return;
				Logger.D(TAG, "Stop geolocation service by timeout");
				GeoLocation.stop();
			}
		}, RhodesService.getGeoLocationInactivityTimeout());
	}
	
	private static GeoLocationImpl getImpl() {
		synchronized (TAG) {
			if (locImpl == null)
				locImpl = new GeoLocationImpl();
			updateInactivityTimer();
			return locImpl;
		}
	}
	
	public static void stop() {
		try {
			Logger.T(TAG, "stop");
			if (locImpl == null)
				return;
			synchronized (TAG) {
				if (locImpl == null)
					return;
				locImpl.stop();
				locImpl = null;
			}
		}
		catch (Exception e) {
			reportFail("stop", e);
		}
	}
	
	public static boolean isAvailable() {
		try {
			Logger.T(TAG, "isAvailable...");
			boolean result = false;
			if (locImpl != null) {
				checkState();
				result = getImpl().isAvailable();
			}
			Logger.T(TAG, "Geo location service is " + (result ? "" : "not ") + "available");
			return result;
		}
		catch (Exception e) {
			reportFail("isAvailable", e);
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
			reportFail("getLatitude", e);
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
			reportFail("getLongitude", e);
		}
		
		return 0.0;
	}

	public static boolean isKnownPosition() {
		try {
			checkState();
			Logger.T(TAG, "isKnownPosition");
			return getImpl().isKnownPosition();
		}
		catch (Exception e) {
			reportFail("isKnownPosition", e);
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
			Logger.T(TAG, "setTimeout");
			getImpl().setTimeout(nsec);
		}
		catch (Exception e) {
			reportFail("setTimeout", e);
		}
	}

}
