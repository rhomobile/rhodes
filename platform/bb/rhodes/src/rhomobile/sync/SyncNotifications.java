package rhomobile.sync;

import java.util.Hashtable;

public class SyncNotifications {
	private Hashtable m_idToUrlmap = new Hashtable();
	
	private static class Notification{
		String m_url;
		String m_params;
		
		Notification(String url, String params ){
			m_url = url;
			m_params = params;
		}
	}
	
	void setNotification(int nSourceID, String url, String params){
		m_idToUrlmap.put(new Integer(nSourceID), new Notification(url, params) );
	}

	void clearNotification(int nSourceID){
		m_idToUrlmap.remove(new Integer(nSourceID));
	}
	
	void fireNotification(int nSourceID, int success){
		Notification obj = (Notification)m_idToUrlmap.get(new Integer(nSourceID));
		
		String body = "status=" + (success > 0 ?"ok":"error") + "&" + obj.m_params; 
		performNotification(obj.m_url, body );
		clearNotification(nSourceID);
	}
	
	public void performNotification(String url, String body){
		
	}
}
