package com.rhomobile.rhodes.geolocation;

public class GeoLocation {

	private static GeoLocationImpl locImpl = new GeoLocationImpl();
	
	public double GetLatitude() {
		//LOG.TRACE("GeoLocation.GetLatitude");
		
		if (locImpl != null)
			  return locImpl.GetLatitude();
			
		return 0.0;
	}

	public double GetLongitude() {
		//LOG.TRACE("GeoLocation.GetLongitude");
		
		if (locImpl != null)
		  return locImpl.GetLongitude();
		
		return 0.0;
	}

	public boolean isKnownPosition() {
		//LOG.TRACE("GeoLocation.isKnownPosition");
		
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
