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
	private static final long TIMEOUT_STOP = -1;
	
	private LocationManager locationManager = null;
	private volatile int available = 0;
	private volatile Location lastLocation;
	private long invalidateLocationPeriod;
	private volatile long pingTimeout = Long.MAX_VALUE; // 1 minute
	
	private List<RhoLocationListener> mListeners = new LinkedList<RhoLocationListener>();
	
	public class RhoLocationListener implements LocationListener {
		private String providerName;
		private LocationManager manager;
		private LocationProvider provider;
		private long updatePeriod = 0;
		
		RhoLocationListener(String providerName, LocationManager manager) {
			this.manager = manager;
			this.providerName = providerName;
		}
		
		String getProviderName() { return this.providerName; }
		
		synchronized boolean register(long updatePeriod) {
			Logger.T(TAG, "Registering location listener for '" + this.providerName + "'.");
			if (this.provider != null) {
				this.manager.removeUpdates(this);
			} else {
				this.provider = this.manager.getProvider(this.providerName);
			}
			if(this.provider != null) {
				this.updatePeriod = updatePeriod;
				this.manager.requestLocationUpdates(providerName, this.updatePeriod, 0, this, Looper.getMainLooper());
				return this.manager.isProviderEnabled(this.providerName);
			}
			return false;
		}
		
		synchronized boolean unregister() {
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
				if (location != null) {
					long time = location.getTime();
					StringBuilder message = new StringBuilder();
					message.append("Last known location from ").append(location.getProvider());
					if((System.currentTimeMillis() - time) > (errorTimeout(this.updatePeriod))) {
						message.append(" time is very old: ").append(location.getTime());
					} else {
						onLocationChanged(location);
						message.append(" time os ok: ").append(location.getTime());
					}
					message.append(". Current time: ").append(System.currentTimeMillis());
					message.append(". Inactivity timeout: ").append(this.updatePeriod * 5).append(".");
					Logger.T(TAG,  message.toString());
				}
			}
		}
		
		@Override
		public void onStatusChanged(String provider, int status, Bundle extras) {
			Logger.T(TAG, "onStatusChanged: provider=" + provider + ", status=" + status);
			//setCurrentGpsLocation(null);
		}
		
		@Override
		public void onProviderEnabled(String provider) {
			Logger.T(TAG, "onProviderEnabled: provider=" + provider);
			checkProviderEnabled(this);
		}
		
		@Override
		public void onProviderDisabled(String provider) {
			Logger.T(TAG, "onProviderDisabled: provider=" + provider);
			checkProviderDisabled(this);
		}
		
		@Override
		public void onLocationChanged(Location location) {
			Logger.T(TAG, "onLocationChanged");
			setLocation(location);
		}
	};
	
	private Thread thWatchdog = new Thread(new Runnable() {
		public void run() {
			Logger.I(TAG, "\"watchdog\" thread started");
			for (;;) {
				if (pingTimeout < 0)
					break;
				try {
					long curTimeout = errorTimeout(pingTimeout);
					// Sleep double ping time (to do not interfere with real location updates)
					Logger.T(TAG, "\"watchdog\" thread waits (" + curTimeout + "ms)...");
					Thread.sleep(curTimeout);
				}
				catch (InterruptedException e) {
					Logger.T(TAG, "\"watchdog\" thread interrupted");
					continue;
				}
				if (isKnownPosition()) {
					Logger.T(TAG, "Position became very old, invalidate and inform about this");
					
				} else {
					Logger.T(TAG, "Position is still unknown, inform about this");
				}
				lastLocation = null;
				PerformOnUiThread.exec(new Runnable() {
					public void run() {
						geoCallbackError();
					}
				});
			}
			PerformOnUiThread.exec(new Runnable() { public void run() { geoCallbackStop();} });
			Logger.I(TAG, "\"watchdog\" thread stopped");
		}
	});
	
	private static long errorTimeout(long time) { return time * 5; } 
	
	private static native void geoCallback();
	private static native void geoCallbackError();
	private static native void geoCallbackStop();
	
	GeoLocationImpl(long invalidateLocationPeriod) {
		this.invalidateLocationPeriod = invalidateLocationPeriod; 
		Logger.T(TAG, "GeoLocationImpl instance created");
		Context ctx = ContextFactory.getContext();
		locationManager = (LocationManager)ctx.getSystemService(Context.LOCATION_SERVICE);
		createListeners();
		thWatchdog.start();
	}
	
	private void createListeners() {
		List<String> providers = locationManager.getAllProviders();
		Iterator<String> it = providers.iterator();
		
		synchronized (mListeners) {
			while (it.hasNext()) {
				String provider = it.next();
				if (provider.equals(LocationManager.PASSIVE_PROVIDER))
					continue;
				
				RhoLocationListener listener = new RhoLocationListener(provider, locationManager);
				mListeners.add(listener);
			}
		}
		
	}
	private void registerListeners() {
		Iterator<RhoLocationListener> it;
		synchronized (mListeners) {
			available = 0;
			it = mListeners.iterator();
			while (it.hasNext()) {
				available++;
				it.next().register(invalidateLocationPeriod);
			}
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
				if (listener.unregister())
					available--;
			}
		}
	}
	
	private void checkProviderEnabled(RhoLocationListener listener) {
		available++;
	}
	
	private void checkProviderDisabled(RhoLocationListener listener) {
		// Register all listeners back
		// if GPS provider becomes disabled
		if (LocationManager.GPS_PROVIDER.equals(listener.getProviderName())) {
			Logger.T(TAG, "GPS provider has disabled, register others back.");
			registerListeners();
		} else {
			available--;
			Logger.T(TAG, "Provider is disabled: " + listener.getProviderName() + ", available=" + available);
		}
	}
	
	private static boolean isLocationsEqual(Location prev, Location current) {
		if (prev == null && current != null)
			return false;
		if (prev != null && current == null)
			return true;

		do {
			if (prev.getLatitude() != current.getLatitude() ||
				prev.getLongitude() == current.getLongitude())
				break;
			
			if(current.hasAccuracy()) {
				if(!prev.hasAccuracy())
					break;
				if (prev.getAccuracy() != current.getAccuracy())
					break;
			}
			return true;
		} while(false);
		return false;
	}
	
	private synchronized void setLocation(Location location) {
		Logger.T(TAG, "setCurrentGpsLocation: location=" + location);
		try {
			// We've received location update
			Logger.T(TAG, "Received location update from \"" + location.getProvider() + "\" provider");
				
			// If we've received fix from GPS, stop updates from all other providers
			// as we don't need it anymore
			if (LocationManager.GPS_PROVIDER.equals(location.getProvider())) {
					unregisterListeners(LocationManager.GPS_PROVIDER);
			}
			
			dumpStatus(locationManager.getAllProviders());

			if (!isLocationsEqual(lastLocation, location)) {
				Logger.T(TAG, "Geo location information changed, notify about that");
				lastLocation = location;
				PerformOnUiThread.exec(new Runnable() { public void run() { geoCallback();} }, 10);
			}
			
		} catch (Exception e) {
			lastLocation = null;
			Logger.E(TAG, e.getMessage());
		}
	}
	
	private void dumpStatus(List<String> providers) {
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
			if (locationManager.isProviderEnabled(provider))
				log.append("enabled");
			else
				log.append("disabled");
		}
		Logger.T(TAG, log.toString());
		log = new StringBuffer();
		log.append("location is");
		if (lastLocation == null) {
			log.append(" not determined");
		} else {
			log.append(" determined");
			log.append(": longitude=").append(Double.toString(lastLocation.getLongitude()));
			log.append(", latitude=").append(Double.toString(lastLocation.getLatitude()));
			if (lastLocation.hasAccuracy())
				log.append(", accuracy=").append(Float.toString(lastLocation.getAccuracy()));
			if (lastLocation.hasSpeed())
				log.append(", speed=").append(Float.toString(lastLocation.getSpeed()));
			if (lastLocation.hasAltitude())
				log.append(", altitude=").append(Double.toString(lastLocation.getAltitude()));
		}
		Logger.T(TAG, log.toString());
	}
	
	void start() {
		registerListeners();
	}
	
	
	synchronized void stop() {
		// Stop thCancel thread
		pingTimeout = TIMEOUT_STOP;
		thWatchdog.interrupt();
		
		if (locationManager == null)
			return;

		unregisterListeners(null);
		
		locationManager = null;
	}

	synchronized boolean isAvailable() {
		return available > 0;
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

	synchronized void setTimeout(int nsec) {
		pingTimeout = nsec*1000;
		thWatchdog.interrupt();
		registerListeners();// do reregister for new timeout
		Logger.T(TAG, "Set new ping timeout: " + pingTimeout + "ms");
		
	}
}
