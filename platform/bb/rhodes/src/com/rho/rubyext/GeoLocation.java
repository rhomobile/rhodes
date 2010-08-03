package com.rho.rubyext;

import javax.microedition.location.*;

import com.rho.RhoClassFactory;
import com.rho.RhoConf;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhoThread;
import com.rho.net.NetResponse;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.rho.net.NetRequest;
import com.rho.RhoRuby;
import j2me.lang.MathEx;

public class GeoLocation extends RhoThread{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("GeoLocation");

	private static GeoLocation m_pInstance;
	
	private LocationProvider m_lp = null;
	private double m_lat = 0.0;
	private double m_lon = 0.0;
	private boolean m_bDetermined = false;
	private long m_nDeterminedTime = 0;
	private LocationListenerImpl m_locListener;
	static private Object sync = new Object();
	static private Object syncLP = new Object();
	private boolean m_bInCallback = false;
	private int m_nPingTimeoutSec = 0;
	
	private final String errorStrDontSupport = "Location API doesn't support";
	private final String errorStrLocationException= "Location could not be determined";
	
	static class LocationListenerImpl implements LocationListener{
        
		GeoLocation m_parent;
		LocationListenerImpl(GeoLocation parent)
		{
			m_parent = parent;
		}
		
		public void locationUpdated(LocationProvider provider, Location location) 
		{
			m_parent.m_bInCallback = true;
			Coordinates coord = null;
			try{
				if ( location == null )
				{
					LOG.TRACE("locationUpdated: location is null.");
					return;
				}
				
				if( !location.isValid() )
				{
					String strExtraInfo = location.getExtraInfo("text/plain");
					LOG.TRACE("locationUpdated: location invalid.Extra info: " + (strExtraInfo!=null ? strExtraInfo :"") );
					return;
				}
				
				coord = location.getQualifiedCoordinates();
				if(coord != null ) 
				{
					LOG.TRACE("locationUpdated - latitude: " + Double.toString(coord.getLatitude()));
					LOG.TRACE("locationUpdated - longitude: " + Double.toString(coord.getLongitude()));
				}else
					LOG.TRACE("locationUpdated - getQualifiedCoordinates: return null.");
				
			}catch(Exception exc)
			{
				LOG.ERROR("locationUpdated failed.", exc);
			}catch(Throwable exc)
			{
				LOG.ERROR("locationUpdated crashed.", exc);
			}finally
			{
				if(coord != null ) 
					m_parent.updateLocation(coord.getLatitude(), coord.getLongitude());
				else
					m_parent.onLocationError();
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
	
	public GeoLocation() {
		super(new RhoClassFactory());
		start(epLow);
	}
	
	int getPingTimeoutSec()
	{
		if (m_nPingTimeoutSec==0)
			m_nPingTimeoutSec = getDefaultPingTimeoutSec();
		
		return m_nPingTimeoutSec;
	}

	int getDefaultPingTimeoutSec()
	{
		int nPingTimeoutSec = RhoConf.getInstance().getInt("gps_ping_timeout_sec");
		if (nPingTimeoutSec==0)
			nPingTimeoutSec = 10;
		
		return nPingTimeoutSec;
	}
	
	void setPingTimeoutSec( int nTimeout )
	{
		int nNewTimeout = nTimeout;
		if (nNewTimeout == 0)
			nNewTimeout = getDefaultPingTimeoutSec();
		
		if ( nNewTimeout != m_nPingTimeoutSec)
		{
			m_nPingTimeoutSec = nNewTimeout;
			stopWait();
		}
	}
	
	public void run()
	{
		LOG.INFO( "Starting main routine..." );

		while(!isStopping())
		{
			if (!isStopping())
				checkAlive();
			
			wait( 10*10000);
		}

		synchronized(sync)
		{
			try{
				while(m_bInCallback)
					sync.wait(100);
			}catch(InterruptedException exc)
			{}
		}
		
		synchronized(syncLP)
		{
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
		boolean bErrorNotify = false;
		boolean bReset = !m_bInCallback;//m_nDeterminedTime == 0 || ((nNow - m_nDeterminedTime)>1000*m_nResetTimeoutSec);
		
		if ( bReset ){
			
			try {
				synchronized(syncLP)
				{
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
					
					if ( getPingTimeoutSec() != -1 )
					{
						m_lp = LocationProvider.getInstance(null);
						if ( m_lp != null ){
							m_locListener = new LocationListenerImpl(this);
							LOG.TRACE("setLocationListener");
							m_lp.setLocationListener( m_locListener, 
									m_nDeterminedTime == 0 ? 1 : getPingTimeoutSec(), -1, -1);
							m_nDeterminedTime = nNow;
						}else
							bErrorNotify = true;
					}
				}
			}catch(Exception exc)
			{
				if ( m_lp != null )
					LOG.INFO(errorStrLocationException + " : " + exc.getMessage());
				else	
					LOG.INFO(errorStrDontSupport + " : " + exc.getMessage());
				
				bErrorNotify = true;
			}finally
			{
				if (bErrorNotify)
					onLocationError();
				else
					checkKnownPosition();
			}
		}
	}
	
    private static class GeoNotification
    {
        String m_strUrl = "", m_strParams = "";
        
        GeoNotification(String strUrl, String strParams)
        { 
        	m_strUrl = strUrl; 
        	m_strParams = strParams; 
        }
        
        void fire(boolean bError)
        {
        	if (m_strUrl != null && m_strUrl.length() == 0)
        		return;
        	
        	NetRequest netRequest = RhoClassFactory.createNetRequest();
        	
        	try {
	        	String strFullUrl = netRequest.resolveUrl(m_strUrl);
	        	String strBody = "rho_callback=1";
	        	if (bError && isAvailable() )
	        		strBody += "&status=error&error_code=" + RhoRuby.ERR_GEOLOCATION;
	        	else	
	        		strBody += "&status=ok";
	        		
        		strBody += "&available=" + (isAvailable() ? 1 : 0);
	        	strBody += "&known_position=" + (isKnownPosition() ? 1 : 0);
	        	strBody += "&latitude=" + GetLatitude();
	        	strBody += "&longitude=" + GetLongitude();
	        	
	            if ( m_strParams != null && m_strParams.length() > 0 )
	                strBody += "&" + m_strParams;
	        	
			    NetResponse resp = netRequest.pushData( strFullUrl, strBody, null );
			    if ( !resp.isOK() )
			        LOG.ERROR( "Fire notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData() );
        	}catch(Exception exc)
        	{
        		LOG.ERROR("fire notification failed.", exc);
        	}
        }
    };
	
    GeoNotification m_ViewNotify, m_Notify; 
    private void setViewNotification( String strUrl, String strParams, int nTimeout )
    {
		synchronized(sync)
		{
			m_ViewNotify = new GeoNotification(strUrl, strParams);
			
			setPingTimeoutSec(nTimeout);
		}
    }

    private void setNotification( String strUrl, String strParams, int nTimeout )
    {
		synchronized(sync)
		{
			m_Notify = new GeoNotification(strUrl, strParams);
			
			setPingTimeoutSec(nTimeout);
		}
    }
    
    private void onLocationError()
    {
		if ( m_Notify != null )
		{
			m_Notify.fire(true);
			m_Notify = null;
		}
		
		if ( m_ViewNotify != null )
		{
			m_ViewNotify.fire(true);
			m_ViewNotify = null;
		}
    }
    
    void checkKnownPosition()
    {
		synchronized(sync)
		{
			if ( m_bDetermined )
				return;

			try{
				Location location = LocationProvider.getLastKnownLocation();
				if ( location != null && location.isValid() )
				{
					long locTime = location.getTimestamp();
					long curTime = java.lang.System.currentTimeMillis();
					long ageInMitutes = (curTime - locTime)/60000;  
					LOG.TRACE("getLastKnownLocation return valid location. Age in minutes from now: " + ageInMitutes);
					Coordinates coord = location.getQualifiedCoordinates();
					if(coord != null ) 
					{
						LOG.TRACE("getLastKnownLocation - latitude: " + Double.toString(coord.getLatitude()));
						LOG.TRACE("getLastKnownLocation - longitude: " + Double.toString(coord.getLongitude()));
						
						if ( ageInMitutes <= 10 )
							updateLocation(coord.getLatitude(), coord.getLongitude());
					}else
						LOG.TRACE("getLastKnownLocation - getQualifiedCoordinates: return null.");
				}
			}catch(Exception exc)
			{
				LOG.ERROR("getLastKnownLocation failed.", exc);
			}
		}
		
    }
    
	private void updateLocation( double dLatitude, double dLongitude )
	{
		synchronized(sync)
		{
			boolean bNotify = m_lat != dLatitude || m_lon != dLongitude || m_bDetermined != true;
			
			m_lat = dLatitude;
			m_lon = dLongitude;
			m_bDetermined = true;
			
			java.util.Date now = new java.util.Date();
			m_nDeterminedTime = now.getTime();
			
			if ( bNotify && m_Notify != null )
			{
				m_Notify.fire(false);
				m_Notify = null;
			}
			
			if ( bNotify && m_ViewNotify != null )
				m_ViewNotify.fire(false);
		}		
	}
	
	public static void stop() {
		if ( m_pInstance != null )
		{
			m_pInstance.stop(2);
			m_pInstance = null;
		}
	}
	
	public static void wakeUp() {
		if ( m_pInstance != null )
			m_pInstance.stopWait();
	}
	
	public static boolean isAvailable(){
		synchronized(sync)
		{
			return m_pInstance != null && m_pInstance.m_lp != null;
		}
	}

	public static double GetLatitude(){
		startSelf();
		synchronized(sync)
		{
			return m_pInstance.m_lat;
		}
	}
	
	public static double GetLongitude(){
		startSelf();
		synchronized(sync)
		{
			return m_pInstance.m_lon;
		}
	}
	
	public static boolean isKnownPosition(){
		startSelf();
		synchronized(sync)
		{
			return m_pInstance.m_bDetermined;
		}
	}
	
	public static String getGeoLocationText()
	{
		String location = "";
		if ( !isAvailable() )
			location = "Unavailable;Unavailable;Unavailable";
		else if( !isKnownPosition() )
			location = "reading...;reading...;reading...";//<br/>" + GeoLocation.getLog();
		else
		{
			double latitude = GetLatitude();
			double longitude = GetLongitude();
		
			location = String.valueOf(Math.abs(latitude)) + "f° " +
				(latitude < 0 ? "South" : "North") + ", " +
				String.valueOf(Math.abs(longitude)) + "f° " +	
				(longitude < 0 ? "West" : "East") + ";" +
				String.valueOf(latitude) + ";" +
				String.valueOf(longitude) + ";";
		}
		
		return location;
	}
	
	private static void startSelf() {
		if ( m_pInstance  == null )
			m_pInstance = new GeoLocation();
	}
	
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("latitude", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try{
					return ObjectFactory.createFloat(GetLatitude());
				}catch(Exception e)
				{
					LOG.ERROR("latitude failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod("longitude", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try{
					return ObjectFactory.createFloat(GetLongitude());
				}catch(Exception e)
				{
					LOG.ERROR("longitude failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod("known_position?", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try{
					
					return ObjectFactory.createBoolean(isKnownPosition());
				}catch(Exception e)
				{
					LOG.ERROR("known_position? failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod("set_view_notification",	new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
				
				if ( args.size() < 1 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in GeoLocation.set_view_notification: wrong number of arguments ( " + args.size() + " for " + 3 + " )");			
				
				try{
					String url = args.get(0) != RubyConstant.QNIL ? args.get(0).toStr() : "";
					String params = "";
					int nTimeout = 0;
					if ( args.size() > 1 )
						params = args.get(1) != RubyConstant.QNIL ? args.get(1).toStr() : "";
					if ( args.size() > 2 )
						nTimeout = args.get(2) != RubyConstant.QNIL ? args.get(2).toInt() : -1;
					
					if ( url != null && url.length() > 0 )
					{
						startSelf();
						m_pInstance.setViewNotification(url, params, nTimeout);
					}else if ( m_pInstance != null )
						m_pInstance.setViewNotification(url, params, nTimeout);
						
				}catch(Exception e)
				{
					LOG.ERROR("set_view_notification failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return RubyConstant.QNIL;
			}
		});
		klass.getSingletonClass().defineMethod("set_notification",	new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
				
				if ( args.size() < 1 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in GeoLocation.set_view_notification: wrong number of arguments ( " + args.size() + " for " + 3 + " )");			
				
				try{
					String url = args.get(0) != RubyConstant.QNIL ? args.get(0).toStr() : "";
					String params = "";
					int nTimeout = 0;
					if ( args.size() > 1 )
						params = args.get(1) != RubyConstant.QNIL ? args.get(1).toStr() : "";
					if ( args.size() > 2 )
						nTimeout = args.get(2) != RubyConstant.QNIL ? args.get(2).toInt() : -1;
					
					if ( url != null && url.length() > 0 )
					{
						startSelf();
						m_pInstance.setNotification(url, params, nTimeout);
					}else if ( m_pInstance != null )
						m_pInstance.setNotification(url, params, nTimeout);
						
				}catch(Exception e)
				{
					LOG.ERROR("set_view_notification failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return RubyConstant.QNIL;
			}
		});
		klass.getSingletonClass().defineMethod("haversine_distance", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
				
				if ( args.size() != 4 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in GeoLocation.haversine_distance: wrong number of arguments ( " + args.size() + " for " + 4 + " )");			
				
				try{
					double lat1 = args.get(0).toFloat();
					double lon1 = args.get(1).toFloat();
					double lat2 = args.get(2).toFloat();
					double lon2 = args.get(3).toFloat();
					
					double RAD_PER_DEG = 0.017453293; //PI/180
					int Rmiles = 3956;                //radius of the great circle in miles

					double dlon = lon2 - lon1;
					double dlat = lat2 - lat1;
					double dlon_rad = dlon * RAD_PER_DEG;
					double dlat_rad = dlat * RAD_PER_DEG;
					double lat1_rad = lat1 * RAD_PER_DEG;
					//double lon1_rad = lon1 * RAD_PER_DEG;
					double lat2_rad = lat2 * RAD_PER_DEG;
					//double lon2_rad = lon2 * RAD_PER_DEG;

					double a = MathEx.pow((Math.sin(dlat_rad/2)), 2) + Math.cos(lat1_rad) * Math.cos(lat2_rad) * MathEx.pow(Math.sin(dlon_rad/2), 2);
					double c = 2 * MathEx.atan2(Math.sqrt(a), Math.sqrt(1-a));
					double dMi = Rmiles * c; //delta between the two points in mile

					return ObjectFactory.createFloat(dMi);
					
				}catch(Exception e)
				{
					LOG.ERROR("haversine_distance failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
	}
	
}
