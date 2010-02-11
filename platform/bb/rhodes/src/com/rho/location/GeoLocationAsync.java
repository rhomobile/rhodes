package com.rho.location;

import javax.microedition.location.*;

import com.rho.RhoClassFactory;
import com.rho.RhoConf;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhoThread;

public class GeoLocationAsync extends RhoThread implements IGeoLocationImpl {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("GeoLocationAsync");

	private LocationProvider m_lp = null;
	private double m_lat = 0.0;
	private double m_lon = 0.0;
	private boolean m_bDetermined = false;
	private long m_nDeterminedTime = 0;
	private LocationListenerImpl m_locListener;
	static private Object sync = new Object();
	//private int m_nResetTimeoutSec = 2; 
	private int m_nPingTimeoutSec;
	private boolean m_bInCallback = false;
	
	private final String errorStrDontSupport = "Location API doesn't support";
	private final String errorStrLocationException= "Location could not be determined";
	
	static class LocationListenerImpl implements LocationListener{
        
		GeoLocationAsync m_parent;
		LocationListenerImpl(GeoLocationAsync parent)
		{
			m_parent = parent;
		}
		
		public void locationUpdated(LocationProvider provider, Location location) {
			m_parent.m_bInCallback = true;
			try{
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
				
				synchronized(m_parent.sync)
				{
					m_parent.m_nDeterminedTime = now.getTime();
					
					Coordinates coord = location.getQualifiedCoordinates();
			
					if(coord != null ) 
					{
						m_parent.m_lat = coord.getLatitude();
						m_parent.m_lon = coord.getLongitude();
						m_parent.m_bDetermined = true;
						
						LOG.TRACE("GetLocation - latitude: " + Double.toString(m_parent.m_lat));
						LOG.TRACE("GetLocation - longitude: " + Double.toString(m_parent.m_lon));
					}else
						LOG.TRACE("GetLocation - getQualifiedCoordinates: return null.");
				}
			}catch(Exception exc)
			{
				LOG.ERROR("locationUpdated failed.", exc);
			}catch(Throwable exc)
			{
				LOG.ERROR("locationUpdated crashed.", exc);
			}
			
			m_parent.m_bInCallback = false;
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
		super(new RhoClassFactory());
		start(epLow);
	}
	
	public void run()
	{
		LOG.INFO( "Starting main routine..." );

		/*try{
			m_lp = LocationProvider.getInstance(null);
		}catch(LocationException ex)
		{
			LOG.INFO(errorStrDontSupport + ex.getMessage());
		}*/
		/*try{
			Location loc = LocationProvider.getLastKnownLocation();
			if ( loc != null )
			{
				Coordinates coord = loc.getQualifiedCoordinates();
				if ( coord != null )
					LOG.TRACE("getLastKnownLocation - latitude: " + Double.toString(coord.getLatitude()) + "; long: " + Double.toString(coord.getLongitude()));
			}
		}catch(Exception exc)
		{
			LOG.ERROR("getLastKnownLocation failed.", exc);
		}*/
		
		m_nPingTimeoutSec = RhoConf.getInstance().getInt("gps_ping_timeout_sec");
		if (m_nPingTimeoutSec==0)
			m_nPingTimeoutSec = 10;
		
		while(!m_bStop)
		{
			wait( 10*10000);
			
			if (!m_bStop)
				checkAlive();
		}

		synchronized(sync)
		{
			try{
				while(m_bInCallback)
					sync.wait(100);
			}catch(InterruptedException exc)
			{}
			
			if ( m_lp != null ){
				m_lp.setLocationListener(null, 0, 0, 0);
				m_lp.reset();
			}
		}
		
		LOG.INFO( "End main routine..." );
	}

	private void checkAlive(){
		java.util.Date now = new java.util.Date();
		long nNow = now.getTime();
		synchronized(sync)
		{
			boolean bReset = !m_bInCallback;//m_nDeterminedTime == 0 || ((nNow - m_nDeterminedTime)>1000*m_nResetTimeoutSec);
			
			if ( bReset ){
				
				try {
					if ( m_lp != null ){
						if ( m_locListener != null )
						{
							LOG.TRACE("Reset LocationListener");
							m_lp.reset();
							m_lp.setLocationListener(null, 0,0, 0 	);
							m_locListener = null;
						}
						
						m_lp = null;
					}
					
					m_lp = LocationProvider.getInstance(null);
					if ( m_lp != null ){
						m_locListener = new LocationListenerImpl(this);
						LOG.TRACE("setLocationListener");
						m_lp.setLocationListener( m_locListener, 
								m_nDeterminedTime == 0 ? 1 : m_nPingTimeoutSec, -1, -1);
						m_nDeterminedTime = nNow;
					}
				}catch(Exception exc)
				{
					if ( m_lp != null )
						LOG.INFO(errorStrLocationException + exc.getMessage());
					else	
						LOG.INFO(errorStrDontSupport + exc.getMessage());
				}
			}
		}
	}
	
	public double GetLatitude(){
		//stopWait();
		synchronized(sync)
		{
			return m_lat;
		}
	}
	
	public double GetLongitude(){
		//stopWait();
		synchronized(sync)
		{
			return m_lon;
		}
	}
	
	public boolean isStarted(){
		//stopWait();
		synchronized(sync)
		{
			return m_lp != null;
		}
	}

	public boolean isKnownPosition(){
		//stopWait();
		synchronized(sync)
		{
			return m_bDetermined;
		}
	}
	
	public void quit() {
		this.stop(2);
	}
	
	public void wakeUp() {
		stopWait();
	}
	
}
