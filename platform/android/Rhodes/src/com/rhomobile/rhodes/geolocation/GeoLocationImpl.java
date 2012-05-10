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

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.location.LocationProvider;
import android.os.Bundle;
import android.os.Looper;

public class GeoLocationImpl {

	private static final String TAG = "GeoLocationImpl";
	
	private static final int UPDATE_PERIOD_IN_MILLISECONDS = 250;
	
	private LocationManager locationManager = null;
	private volatile Location lastLocation = null;
	
	private List<RhoLocationListener> mListeners = new LinkedList<RhoLocationListener>();
    private List<RhoLocationListener> mSwitchedOffListeners = new LinkedList<RhoLocationListener>();
	
	public class RhoLocationListener implements LocationListener {
		private String providerName;
		private LocationManager manager;
		private LocationProvider provider;
		private boolean available = false;
		
		boolean isAvailable() { return available; }
		
		RhoLocationListener(String providerName, LocationManager manager) {
			this.manager = manager;
			this.providerName = providerName;
		}
		
		String getProviderName() { return this.providerName; }
		
		synchronized boolean register() {
			Logger.T(TAG, "Registering location listener for '" + this.providerName + "'.");
			if (this.provider != null) {
				this.manager.removeUpdates(this);
			} else {
				this.provider = this.manager.getProvider(this.providerName);
			}
			if(this.provider != null) {
				this.manager.requestLocationUpdates(providerName, UPDATE_PERIOD_IN_MILLISECONDS, 0, this, Looper.getMainLooper());
				available = this.manager.isProviderEnabled(this.providerName);
			}
			return available;
		}
		
		synchronized boolean unregister() {
			available = false;
			if (this.provider != null && this.manager != null) {
				Logger.T(TAG, "Unregistering location listener for '" + this.providerName + "'.");
				this.manager.removeUpdates(this);
				this.provider = null;
				return true;
			}
			this.provider = null;
			return false;
		}
		
		void requestLastLocation() {
			if (this.provider != null && this.manager != null) {
				Location location = this.manager.getLastKnownLocation(this.providerName);
				onLocationChanged(location);
			}
		}
		
		@Override
		public void onStatusChanged(String provider, int status, Bundle extras) {
			Logger.T(TAG, "onStatusChanged: provider=" + provider + ", status=" + status);
			requestLastLocation();
		}
		
		@Override
		public void onProviderEnabled(String provider) {
			Logger.I(TAG, "onProviderEnabled: provider=" + provider);
			available = true; 
			checkProviderEnabled(this);
		}
		
		@Override
		public void onProviderDisabled(String provider) {
			Logger.I(TAG, "onProviderDisabled: provider=" + provider);
			available = false;
			checkProviderDisabled(this);
		}
		
		@Override
		public void onLocationChanged(Location location) {
			Logger.T(TAG, "onLocationChanged");
			setLocation(location);
		}
	};
	
	GeoLocationImpl() {
		Logger.T(TAG, "GeoLocationImpl instance created");
		Context ctx = ContextFactory.getContext();
		locationManager = (LocationManager)ctx.getSystemService(Context.LOCATION_SERVICE);
		createListeners();
	}
	
	private void createListeners() {
		LocationManager man = locationManager;
		if (man == null) {
			Logger.E(TAG, "createListeners() : Location Manager is NULL !");
			return;
		}

		List<String> providers = man.getAllProviders();
		Iterator<String> it = providers.iterator();
		
		synchronized (mListeners) {
			while (it.hasNext()) {
				String provider = it.next();
				if (provider.equals(LocationManager.PASSIVE_PROVIDER))
					continue;
				
				RhoLocationListener listener = new RhoLocationListener(provider, man);
				mSwitchedOffListeners.add(listener);
			}
		}
		
	}
	private void registerListeners() {
		Iterator<RhoLocationListener> it;
		synchronized (mListeners) {
			it = mSwitchedOffListeners.iterator();
			while (it.hasNext()) {
			    RhoLocationListener listener = it.next();
                
                Logger.T(TAG, "Registering location listener: " + listener.getProviderName());
                
				listener.register();
				mListeners.add(listener);
			}
			mSwitchedOffListeners.removeAll(mListeners);
			
			Logger.T(TAG, "Count of switched off listeners: " + mSwitchedOffListeners.size());
		}
		// Request last location after every listener has registered
		// because last location from GPS will force other listeners to unregister
		it = mListeners.iterator();
		while (it.hasNext()) {
			it.next().requestLastLocation();
		}
	}
	
	private void unregisterListeners(String skipProvider) {
		synchronized (mListeners) {
			Iterator<RhoLocationListener> it = mListeners.iterator();
			while (it.hasNext()) {
				RhoLocationListener listener = it.next(); 
				if ((skipProvider != null) && listener.getProviderName().equals(skipProvider))
					continue;
                
                Logger.T(TAG, "Unregistering location listener: " + listener.getProviderName());
                
				listener.unregister();
				it.remove();
				mSwitchedOffListeners.add(listener);
			}
            Logger.T(TAG, "Count of switched off listeners: " + mSwitchedOffListeners.size());
		}
	}
	

	public void doUpdate() {
		
	}
	
	private void checkProviderEnabled(RhoLocationListener listener) {
	}
	
	private void checkProviderDisabled(RhoLocationListener listener) {
		// Register all listeners back
		// if GPS provider becomes disabled
		if (LocationManager.GPS_PROVIDER.equals(listener.getProviderName())) {
			Logger.T(TAG, "GPS provider has disabled, register others back.");
			registerListeners();
		} else {
			Logger.T(TAG, "Provider is disabled: " + listener.getProviderName());
		}
	}
	
	private synchronized void setLocation(Location location) {
		if (location == null) {
			Logger.T(TAG, "setCurrentGpsLocation: location = null");
			return;
		}
		Logger.T(TAG, "setCurrentGpsLocation: location=" + location);
		try {
			// We've received location update
			Logger.T(TAG, "Received location update from \"" + location.getProvider() + "\" provider");
				
			// If we've received fix from GPS, stop updates from all other providers
			// as we don't need it anymore
			if (LocationManager.GPS_PROVIDER.equals(location.getProvider())) {
					unregisterListeners(LocationManager.GPS_PROVIDER);
			}
			
			LocationManager man = locationManager;
			if (man != null) {
				dumpStatus(man.getAllProviders());
			}

			lastLocation = location;
		} catch (Exception e) {
			lastLocation = null;
			GeoLocation.onGeoCallbackError();
			Logger.E(TAG, e);
		}
	}
	
	private void dumpStatus(List<String> providers) {
		LocationManager man = locationManager;
		if (man == null) {
			Logger.E(TAG, "dumpStatus() : Location Manager is NULL !");
			return;
		}
		StringBuffer log = new StringBuffer();
		log.append("Status of location providers: ");
		Iterator<String> it = providers.iterator();
		boolean first = true;
		while (it.hasNext()) {
			String provider = it.next();
			if (first) 
				first = false;
			else
				log.append(", ");
			log.append("\"");
			log.append(provider);
			log.append("\" - ");
			if (man.isProviderEnabled(provider))
				log.append("enabled");
			else
				log.append("disabled");
		}
		Logger.T(TAG, log.toString());
		log = new StringBuffer();
		log.append("location is");
		Location ll = lastLocation;
		if (ll == null) {
			log.append(" not determined");
		} else {
			log.append(" determined");
			log.append(": longitude=").append(Double.toString(ll.getLongitude()));
			log.append(", latitude=").append(Double.toString(ll.getLatitude()));
			if (lastLocation.hasAccuracy())
				log.append(", accuracy=").append(Float.toString(ll.getAccuracy()));
			if (lastLocation.hasSpeed())
				log.append(", speed=").append(Float.toString(ll.getSpeed()));
			if (lastLocation.hasAltitude())
				log.append(", altitude=").append(Double.toString(ll.getAltitude()));
		}
		Logger.T(TAG, log.toString());
	}
	
	void start() {
		registerListeners();
	}
	
	
	synchronized void stop() {
		unregisterListeners(null);
	}

	synchronized boolean isAvailable() {
		Iterator<RhoLocationListener> it;
		int available = 0;
		synchronized (mListeners) {
			it = mListeners.iterator();
			while (it.hasNext()) {
				if (it.next().isAvailable())
					available++;
			}
		}
		return available > 0;
	}
	
	synchronized Location getLocation() {
		return lastLocation;
	}
	
	synchronized double getLatitude() {
		return lastLocation != null ? lastLocation.getLatitude() : 0;
	}

	synchronized double getLongitude() {
		return lastLocation != null ? lastLocation.getLongitude() : 0;
	}
	
	synchronized float getAccuracy() {
		return (lastLocation != null && lastLocation.hasAccuracy()) ? lastLocation.getAccuracy() : 0;
	}

	synchronized long getTime() {
		return lastLocation != null ? lastLocation.getTime() : 0;
	}

	synchronized float getSpeed() {
		return (lastLocation != null && lastLocation.hasSpeed()) ? lastLocation.getSpeed() : 0;
	}

	synchronized double getAltitude() {
		return (lastLocation != null && lastLocation.hasAltitude()) ? lastLocation.getAltitude() : 0;
	}

	synchronized boolean isKnownPosition() {
		return lastLocation != null;
	}
	

}
