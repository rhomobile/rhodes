package com.rho.location;

import javax.microedition.location.Coordinates;
import javax.microedition.location.Criteria;
import javax.microedition.location.Location;
import javax.microedition.location.LocationException;
import javax.microedition.location.LocationListener;
import javax.microedition.location.LocationProvider;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.location.GeoLocationAsync.LocationListenerImpl;

public class GeoLocationImpl implements Runnable, IGeoLocationImpl {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("GeoLocationImpl");
	
	private final int HORIZONTAL_ACCURANCE = 500;
	private final int REQUEST_TIMEOUT = 60;
	private double m_lat = 0.0;
	private double m_lon = 0.0;
	private LocationProvider m_lp = null;
	private boolean m_bDetermined = false;
	
	private final String errorStrDontSupport = "Location API doesn't support";
	private final String errorStrLocationException= "Location could not be determined";
	
	private static final long WAIT_INTERVAL = 60000L;
	private static final long WAIT_START_INTERVAL = 1000L;

	private static final int STATE_NONE = 0;
	private static final int STATE_LOCATING = 1;
	private static final int STATE_PAUSE = 2;	
	private static final int STATE_DOSTOP = 3;
	
	private String sync = "sync";
	private int m_nState = STATE_NONE;
	private Thread m_thread;
	
	public GeoLocationImpl() {
		Criteria cr= new Criteria();
//		cr.setHorizontalAccuracy(HORIZONTAL_ACCURANCE);
		
		try{
			m_lp = LocationProvider.getInstance(cr);
			
			if ( m_lp!=null ){
				m_thread = new Thread(this);
				m_thread.start();
			}			
		}catch(LocationException ex)
		{
			LOG.TRACE(errorStrDontSupport, ex);
		}
		
	}

	void determineLocation(){
		// Request the location, setting a one-minute timeout
		Location loc = null;
		try{
			LOG.TRACE("Start getLocation");
			loc = m_lp.getLocation(REQUEST_TIMEOUT); //seconds
		}catch(InterruptedException ex){ //Interrupted by user
			LOG.TRACE("getLocation:InterruptedException", ex);
		}catch(Exception ex)
		{
			LOG.TRACE("getLocation:LocationException: errorStrLocationException", ex );
		}
		
		if ( loc != null ){
			Coordinates coord = loc.getQualifiedCoordinates();
	
			if(coord != null ) {
				synchronized (sync) {
				  m_lat = coord.getLatitude();
				  m_lon = coord.getLongitude();
				  m_bDetermined = true;
				  
				  LOG.TRACE("determineLocation lat : " + m_lat + "; lon : " + m_lon );
				}
			}else
				LOG.TRACE("GetLocation - getQualifiedCoordinates: return null.");
		}else
			LOG.TRACE("GetLocation - getLocation: return null.");
	}
	
	public synchronized double GetLatitude(){
	  return m_lat;
	}
	public synchronized double GetLongitude(){
	  return m_lon;
	}
	
	public synchronized boolean isStarted(){
		return m_lp != null;
	}

	public synchronized boolean isKnownPosition(){
		return m_bDetermined;
	}
	
	public void quit() {
		synchronized (sync) {
			if ( getState() != STATE_NONE ){
				setState(STATE_DOSTOP);
				sync.notify();
			}
		}
		
		if ( m_lp != null )
			m_lp.reset();
			
		try{
			int nTry = 0;
			while( nTry < 10 && getState() != STATE_NONE ){
				Thread.sleep(100);
				nTry++;
			}
			
			if ( getState() != STATE_NONE )
				m_thread.interrupt();
			
		}catch(Exception exc){
			
		}
	}
	
	public void run() {
		
		LOG.INFO("GeoLocation is started...");
		Thread.currentThread().setPriority(Thread.MIN_PRIORITY);
		
		int nFirstTry = 0;
		while (!isStop()) {
			setState(STATE_LOCATING);
			determineLocation();
			
			synchronized (sync) {
				try {
					if (!isStop()){
						setState(STATE_PAUSE);
						long nTimeout = WAIT_INTERVAL;
						if ( nFirstTry < 40 && !m_bDetermined )
						{
							nFirstTry ++;
							nTimeout = WAIT_START_INTERVAL;
						}
						LOG.TRACE("Wait :" + nTimeout/1000 + " sec.");
						sync.wait(nTimeout);
					}
				} catch (Exception e) {
					LOG.TRACE("Wait exception:" + e.getMessage());
				}
			}
		}
		LOG.INFO("GeoLocation is shutdown...");
		
		setState(STATE_NONE);
	}
	
	public void wakeUp() {
		if ( getState() == STATE_PAUSE ){
			synchronized (sync) {	
				sync.notify(); 
			}
		}
	}

	public synchronized int getState() {
		return m_nState;
	}

	public boolean isStop() {
		return getState()==STATE_DOSTOP;
	}
	
	private synchronized void setState(int state) {
		m_nState = state;
	}	
	
}
