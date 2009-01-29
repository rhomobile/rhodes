package rhomobile.location;

import javax.microedition.location.*;

public class GeoLocationAsync {

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
				}else
					log("GetLocation - getQualifiedCoordinates: return null.");
			}else
				log("GetLocation - getLocation: return null.");
			
		}

		public void providerStateChanged(LocationProvider provider, int newState) {
//http://supportforums.blackberry.com/rim/board/message?board.id=java_dev&thread.id=744
			if ( newState == LocationProvider.TEMPORARILY_UNAVAILABLE ){
				log("providerStateChanged:" + "TEMPORARILY_UNAVAILABLE");
			//	m_bDetermined = false;
			}else if ( newState == LocationProvider.OUT_OF_SERVICE ){
				log("providerStateChanged:" + "OUT_OF_SERVICE");
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
			log("GeoLocation error:" + ex.getMessage() + ";" + errorStrDontSupport );
		}
	}

	private void log(String strMsg ){
		System.out.println(strMsg);
		//m_strLog += strMsg;
		//m_strLog += "\r\n<br/>";
	}

	private void checkAlive(){
		java.util.Date now = new java.util.Date();
		long nNow = now.getTime();
		if ( m_nDeterminedTime == 0 || (nNow - m_nDeterminedTime>1000*60*5)){ //5 minutes
			if ( m_lp != null ){
				log("setLocationListener");
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
