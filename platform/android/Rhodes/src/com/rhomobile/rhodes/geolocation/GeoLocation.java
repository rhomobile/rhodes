package com.rhomobile.rhodes.geolocation;

public class GeoLocation {

	private static GeoLocationImpl locImpl = new GeoLocationImpl();
	
	public static double getLatitude() {
		//LOG.TRACE("GeoLocation.GetLatitude");
		
		if (locImpl != null)
			  return locImpl.GetLatitude();
			
		return 0.0;
	}

	public static double getLongitude() {
		//LOG.TRACE("GeoLocation.GetLongitude");
		
		if (locImpl != null)
		  return locImpl.GetLongitude();
		
		return 0.0;
	}

	public static boolean isKnownPosition() {
		//LOG.TRACE("GeoLocation.isKnownPosition");
		
		return locImpl != null && locImpl.isKnownPosition();
	}

}
