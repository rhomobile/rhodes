package com.rho.location;

import javax.microedition.location.*;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

public class GeoLocationAsync {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("GeoLocationAsync");

	private LocationProvider m_lp = null;
	private String m_strLog = "";
	private double m_lat = 0.0;
	private double m_lon = 0.0;
	private boolean m_bDetermined = false;
	private long m_nDeterminedTime = 0;
	
	private final String errorStrDontSupport = "Location API doesn't support";
	private final String errorStrLocationException= "Location could not be determined";
	
	class LocationListenerImpl implements LocationListener{

		public void locationUpdated(LocationProvider provider, Location location) {
			if ( location != null ){
				java.util.Date now = new java.util.Date();
				m_nDeterminedTime = now.getTime();
				
				Coordinates coord = location.getQualifiedCoordinates();
		
				if(coord != null ) {
					synchronized (this) {
					  m_lat = coord.getLatitude();
					  m_lon = coord.getLongitude();
					  m_bDetermined = true;
					}
					LOG.TRACE("GetLocation - latitude: " + Double.toString(m_lat));
					LOG.TRACE("GetLocation - longitude: " + Double.toString(m_lon));
				}else
					LOG.INFO("GetLocation - getQualifiedCoordinates: return null.");
			}else
				LOG.INFO("GetLocation - getLocation: return null.");
			
		}

		public void providerStateChanged(LocationProvider provider, int newState) {
//http://supportforums.blackberry.com/rim/board/message?board.id=java_dev&thread.id=744
			if ( newState == LocationProvider.TEMPORARILY_UNAVAILABLE ){
				LOG.INFO("providerStateChanged: TEMPORARILY_UNAVAILABLE");
			//	m_bDetermined = false;
			}else if ( newState == LocationProvider.OUT_OF_SERVICE ){
				LOG.INFO("providerStateChanged: OUT_OF_SERVICE");
			//	m_bDetermined = false;
			}
		}
	}
	
	public GeoLocationAsync() {
		Criteria cr= new Criteria();
//		cr.setHorizontalAccuracy(HORIZONTAL_ACCURANCE);
		
		try{
			m_lp = LocationProvider.getInstance(cr);
			checkAlive();
		}catch(LocationException ex)
		{
			LOG.ERROR(errorStrDontSupport,ex);
		}
	}

	private void checkAlive(){
		java.util.Date now = new java.util.Date();
		long nNow = now.getTime();
		if ( m_nDeterminedTime == 0 || (nNow - m_nDeterminedTime>1000*60*5)){ //5 minutes
			if ( m_lp != null ){
				LOG.INFO("setLocationListener");
				m_lp.setLocationListener(null, 0, 0, 0);
				m_lp.reset();
				m_lp.setLocationListener(new LocationListenerImpl(), 50, 10, 20);
				m_nDeterminedTime = nNow;
			}
		}
	}
	
	public synchronized double GetLatitude(){
		checkAlive();
		return m_lat;
	}
	
	public synchronized double GetLongitude(){
		checkAlive();
		return m_lon;
	}
	
	public synchronized boolean isStarted(){
		checkAlive();
		return m_lp != null;
	}

	public synchronized String getLog(){
		return m_strLog;
	}
	
	public synchronized boolean isKnownPosition(){
		checkAlive();
		return m_bDetermined;
	}
	
	public synchronized void quit() {
		if ( m_lp != null ){
			m_lp.setLocationListener(null, 0, 0, 0);
			m_lp.reset();
		}
	}
	
	public void wakeUp() {
	}
	
}
