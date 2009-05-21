package com.rhomobile.rhodes.geolocation;

import com.rho.location.IGeoLocationImpl;

public class GeoLocation implements IGeoLocationImpl {

	private static GeoLocationImpl locImpl = new GeoLocationImpl();
	
	public double GetLatitude() {
		if (locImpl != null)
			  return locImpl.GetLatitude();
			
		return 0.0;
	}

	public double GetLongitude() {
		if (locImpl != null)
		  return locImpl.GetLongitude();
		
		return 0.0;
	}

	public boolean isKnownPosition() {
		return locImpl != null && locImpl.isKnownPosition();
	}

	public boolean isStarted() {
		return true;
	}

	public void quit() {
	}

	public void wakeUp() {
	}

}
