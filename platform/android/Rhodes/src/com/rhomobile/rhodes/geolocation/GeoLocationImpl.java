package com.rhomobile.rhodes.geolocation;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesInstance;

import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Looper;

public class GeoLocationImpl implements LocationListener {

	private static final String TAG = "GeoLocationImpl";
	private LocationManager locationManager;
	private double longitude = 0;
	private double latitude = 0;
	private boolean determined = false;

	public GeoLocationImpl() {

		Looper.prepare();

		setCurrentGpsLocation(null);
	}

	private void setCurrentGpsLocation(Location location) {
		Logger.T(TAG, "GeoLocationImpl.setCurrentGpsLocation");
		try {
			if (location == null) {
				locationManager = (LocationManager) RhodesInstance
						.getInstance().getSystemService(
								Context.LOCATION_SERVICE);
				locationManager.requestLocationUpdates(
						LocationManager.GPS_PROVIDER, 0, 0, this);
				location = locationManager
						.getLastKnownLocation(LocationManager.GPS_PROVIDER);
			}
			if (location != null) {
				longitude = location.getLongitude();
				latitude = location.getLatitude();
				determined = true;
			} else {
				determined = false;
			}

			Logger.T(TAG, "gps enabled: " + new Boolean(locationManager.isProviderEnabled(
					LocationManager.GPS_PROVIDER)).toString());
			Logger.T(TAG, "determined: " + new Boolean(determined).toString());
			if (determined) {
				Logger.T(TAG, "longitude: " + new Double(longitude).toString());
				Logger.T(TAG, "latitude: " + new Double(latitude).toString());
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

	public synchronized double GetLatitude() {
		return latitude;
	}

	public synchronized double GetLongitude() {
		return longitude;
	}

	public synchronized boolean isKnownPosition() {
		return determined;
	}
}
