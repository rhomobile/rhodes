package com.rhomobile.rhodes.geolocation;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.location.IGeoLocationImpl;

public class GeoLocation implements IGeoLocationImpl {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Android GeoLocation");
	
	private static GeoLocationImpl locImpl = new GeoLocationImpl();
	
	public double GetLatitude() {
		LOG.TRACE("GeoLocation.GetLatitude");
		
		if (locImpl != null)
			  return locImpl.GetLatitude();
			
		return 0.0;
	}

	public double GetLongitude() {
		LOG.TRACE("GeoLocation.GetLongitude");
		
		if (locImpl != null)
		  return locImpl.GetLongitude();
		
		return 0.0;
	}

	public boolean isKnownPosition() {
		LOG.TRACE("GeoLocation.isKnownPosition");
		
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
