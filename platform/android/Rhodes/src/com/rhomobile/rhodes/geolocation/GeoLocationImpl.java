/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
package com.rhomobile.rhodes.geolocation;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.location.LocationProvider;
import android.os.Bundle;
import android.os.Looper;

public class GeoLocationImpl implements LocationListener {

	private static final String TAG = "GeoLocationImpl";
	private static final String PROVIDER = LocationManager.GPS_PROVIDER;
	private LocationManager locationManager = null;
	private boolean available = false;
	private double longitude = 0;
	private double latitude = 0;
	private boolean determined = false;
	
	private static int TIMEOUT_STOP = -1;
	
	private int timeout = 10*60*1000; // 10 minutes
	private Thread thCancel = new Thread(new Runnable() {
		public void run() {
			Logger.T(TAG, "Cancel thread started");
			for (;;) {
				if (timeout < 0)
					break;
				try {
					Logger.T(TAG, "Waiting (" + timeout + "ms)...");
					Thread.sleep(timeout);
				}
				catch (InterruptedException e) {
					Logger.T(TAG, "Cancel thread interrupted");
					continue;
				}
				if (!isKnownPosition()) {
					Logger.T(TAG, "Position is still unknown, inform about this");
					PerformOnUiThread.exec(new Runnable() {
						public void run() {
							geoCallbackError();
						}
					}, false);
				}
				timeout = 2147483647;
			}
			Logger.T(TAG, "Cancel thread stopped");
		}
	});
	
	private native void geoCallback();
	private native void geoCallbackError();
	
	public GeoLocationImpl() {
		setCurrentGpsLocation(null);
		thCancel.start();
	}

	private void setCurrentGpsLocation(Location location) {
		Logger.T(TAG, "GeoLocationImpl.setCurrentGpsLocation");
		try {
			if (locationManager == null) {
				Context ctx = RhodesService.getInstance().getContext();
				locationManager = (LocationManager)ctx.getSystemService(Context.LOCATION_SERVICE);
				LocationProvider gpsProvider = locationManager.getProvider(PROVIDER);
				available = gpsProvider != null && locationManager.isProviderEnabled(PROVIDER);
				locationManager.requestLocationUpdates(PROVIDER, 0, 0, this, Looper.getMainLooper());
			}
			
			if (location == null)
				location = locationManager.getLastKnownLocation(PROVIDER);
			
			boolean prevDetermined = determined;
			double prevLat = latitude;
			double prevLon = longitude;
			
			if (location != null) {
				longitude = location.getLongitude();
				latitude = location.getLatitude();
				determined = true;
			} else {
				determined = false;
			}

			Logger.T(TAG, "gps enabled: " + Boolean.toString(locationManager.isProviderEnabled(PROVIDER)));
			Logger.T(TAG, "determined: " + Boolean.toString(determined));
			if (determined) {
				Logger.T(TAG, "longitude: " + Double.toString(longitude));
				Logger.T(TAG, "latitude: " + Double.toString(latitude));
			}
			
			if (determined != prevDetermined || latitude != prevLat || longitude != prevLon) {
				geoCallback();
			}
		} catch (Exception e) {
			determined = false;
			Logger.E(TAG, e.getMessage());
		}
	}
	
	public void onLocationChanged(Location location) {
		Logger.T(TAG, "onLocationChanged");
		setCurrentGpsLocation(location);
	}

	public void onProviderDisabled(String provider) {
		Logger.T(TAG, "onProviderDisabled");
		setCurrentGpsLocation(null);
	}

	public void onProviderEnabled(String provider) {
		Logger.T(TAG, "onProviderEnabled");
		setCurrentGpsLocation(null);
	}

	public void onStatusChanged(String provider, int status, Bundle extras) {
		Logger.T(TAG, "onStatusChanged");
		setCurrentGpsLocation(null);
	}
	
	public synchronized void stop() {
		// Stop thCancel thread
		timeout = TIMEOUT_STOP;
		thCancel.interrupt();
		
		if (locationManager == null)
			return;
		locationManager.removeUpdates(this);
		locationManager = null;
	}

	public synchronized boolean isAvailable() {
		return available;
	}
	
	public synchronized double getLatitude() {
		return latitude;
	}

	public synchronized double getLongitude() {
		return longitude;
	}

	public synchronized boolean isKnownPosition() {
		return determined;
	}

	public synchronized void setTimeout(int nsec) {
		timeout = nsec*1000;
		thCancel.interrupt();
	}
}
