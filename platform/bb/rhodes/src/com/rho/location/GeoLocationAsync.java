package com.rho.location;

import javax.microedition.location.*;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

public class GeoLocationAsync implements IGeoLocationImpl {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("GeoLocationAsync");

	private LocationProvider m_lp = null;
	private String m_strLog = "";
	private double m_lat = 0.0;
	private double m_lon = 0.0;
	private boolean m_bDetermined = false;
	private long m_nDeterminedTime = 0;
	private LocationListenerImpl m_locListener;
	
	private final String errorStrDontSupport = "Location API doesn't support";
	//private final String errorStrLocationException= "Location could not be determined";
	
	class LocationListenerImpl implements LocationListener{

		public void locationUpdated(LocationProvider provider, Location location) {
			if ( location == null )
			{
				LOG.TRACE("GetLocation - locationUpdated: location is null.");
				return;
			}
			
			if( !location.isValid() )
			{
				LOG.TRACE("GetLocation - locationUpdated: location invalid.");
				return;
			}
			
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
				LOG.TRACE("GetLocation - getQualifiedCoordinates: return null.");
			
		}

		public void providerStateChanged(LocationProvider provider, int newState) {
//http://supportforums.blackberry.com/rim/board/message?board.id=java_dev&thread.id=744
			if ( newState == LocationProvider.TEMPORARILY_UNAVAILABLE ){
				LOG.TRACE("providerStateChanged: TEMPORARILY_UNAVAILABLE");
			//	m_bDetermined = false;
			}else if ( newState == LocationProvider.OUT_OF_SERVICE ){
				LOG.TRACE("providerStateChanged: OUT_OF_SERVICE");
			//	m_bDetermined = false;
			}else
				LOG.TRACE("providerStateChanged: " + newState);
		}
	}
	
	public GeoLocationAsync() {
		//Criteria cr= new Criteria();
		//cr.setHorizontalAccuracy(HORIZONTAL_ACCURANCE);
		
		try{
			LOG.TRACE("GeoLocationImpl constructor");
			m_lp = LocationProvider.getInstance(null);
			checkAlive();
		}catch(LocationException ex)
		{
			LOG.TRACE(errorStrDontSupport + ex.getMessage());
		}
	}

	private void checkAlive(){
		java.util.Date now = new java.util.Date();
		long nNow = now.getTime();
		if ( m_nDeterminedTime == 0 || ((nNow - m_nDeterminedTime)>1000*60*5)){ //5 minutes
			if ( m_lp != null ){
				if ( m_locListener != null )
				{
					LOG.TRACE("Reset LocationListener");
					m_lp.reset();
					m_lp.setLocationListener(null, -1,1, 1 	);
					m_locListener = null;
				}
				
				m_locListener = new LocationListenerImpl();
				LOG.TRACE("setLocationListener");
				m_lp.setLocationListener( m_locListener, 1, 1, 1);
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
