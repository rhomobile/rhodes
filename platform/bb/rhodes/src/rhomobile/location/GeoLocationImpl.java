package rhomobile.location;

import javax.microedition.location.Coordinates;
import javax.microedition.location.Criteria;
import javax.microedition.location.Location;
import javax.microedition.location.LocationException;
import javax.microedition.location.LocationProvider;

public class GeoLocationImpl implements Runnable {

	private final int HORIZONTAL_ACCURANCE = 500;
	private final int REQUEST_TIMEOUT = 60;
	private double m_lat = 0.0;
	private double m_lon = 0.0;
	private LocationProvider m_lp = null;
	private boolean m_bDetermined = false;
	private String m_strLog = "";
	
	private final String errorStrDontSupport = "Location API doesn't support";
	private final String errorStrLocationException= "Location could not be determined";
	
	private static final long WAIT_INTERVAL = 60000L;

	private static final int STATE_NONE = 0;
	private static final int STATE_LOCATING = 1;
	private static final int STATE_PAUSE = 2;	
	private static final int STATE_DOSTOP = 3;
	
	private String sync = "sync";
	private int m_nState = STATE_NONE;
	
	public GeoLocationImpl() {
		Criteria cr= new Criteria();
//		cr.setHorizontalAccuracy(HORIZONTAL_ACCURANCE);
		
		try{
			m_lp = LocationProvider.getInstance(cr);
			
			if ( m_lp!=null ){
				Thread thread = new Thread(this);
				thread.setPriority(Thread.MIN_PRIORITY);
				thread.start();
			}			
		}catch(LocationException ex)
		{
			log("GeoLocation error:" + ex.getMessage() + ";" + errorStrDontSupport );
		}
		
	}

	void determineLocation(){
		// Request the location, setting a one-minute timeout
		Location loc = null;
		try{
			loc = m_lp.getLocation(REQUEST_TIMEOUT); //seconds
		}catch(InterruptedException ex){ //Interrupted by user
			log("GetLocation - getLocation:InterruptedException " + ex.getMessage());
		}catch(Exception ex)
		{
			log("GetLocation - getLocation:LocationException: "+ ex.getMessage() + ";" + errorStrLocationException );
		}
		
		if ( loc != null ){
			Coordinates coord = loc.getQualifiedCoordinates();
	
			if(coord != null ) {
				synchronized (sync) {
				  m_lat = coord.getLatitude();
				  m_lon = coord.getLongitude();
				  m_bDetermined = true;
				}
			}else
				log("GetLocation - getQualifiedCoordinates: return null.");
		}else
			log("GetLocation - getLocation: return null.");
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

	public synchronized String getLog(){
		return m_strLog;
	}
	
	public synchronized boolean isKnownPosition(){
		return m_bDetermined;
	}
	
	public void quit() {
		synchronized (sync) {
			setState(STATE_DOSTOP);
			sync.notify();
			
			if ( m_lp != null )
				m_lp.reset();
		}
	}
	
	private void log(String strMsg ){
		System.out.println(strMsg);
		//m_strLog += strMsg;
		//m_strLog += "\r\n<br/>";
	}
	
	public void run() {
		
		log("GeoLocation is started...");
		
		while (!isStop()) {
			setState(STATE_LOCATING);
			determineLocation();
			
			synchronized (sync) {
				try {
					if (!isStop()){
						setState(STATE_PAUSE);
						sync.wait(WAIT_INTERVAL);
					}
				} catch (Exception e) {
					log("Wait exception:" + e.getMessage());
				}
			}
		}
		setState(STATE_NONE);
		log("GeoLocation is shutdown...");
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
