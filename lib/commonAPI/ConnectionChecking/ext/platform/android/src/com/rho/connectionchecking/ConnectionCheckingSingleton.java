package com.rho.connectionchecking;

import java.util.LinkedList;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;

class ConnectionCheckingSingleton   implements IConnectionCheckingSingleton {
	
	public static String HOST="192.168.7.129";//DUmmy
	public static String MESSAGE="Establishing Connection";
	public static int TIMEOUT=30000;
	public static int POLLI_INTERVAL=5000;
	public static int PORT=80;
	public static int TRACK_CONNECTION=0;
	public static String BADLINK_URI="";
	
	
	
	
    public ConnectionCheckingSingleton(ConnectionCheckingFactory factory) {
       // super(factory);
    	super();
    }

    List<Object> getIDs() {
        List<Object> ids = new LinkedList<Object>();
        ids.add("SCN1");
        ids.add("SCN2");
        return ids;
    }
    
  
	@Override
	public void getHostURL(IMethodResult result) {
		
		result.set(ConnectionCheckingSingleton.HOST);
	}

	

	@Override
	public void getTrackConnection(IMethodResult result) {
		result.set(ConnectionCheckingSingleton.TRACK_CONNECTION);
		
	}

	

	@Override
	public void getTimeout(IMethodResult result) {
		result.set(ConnectionCheckingSingleton.TIMEOUT);
		
	}

	

	@Override
	public void getPollInterval(IMethodResult result) {
		result.set(ConnectionCheckingSingleton.POLLI_INTERVAL);
		
	}

	

	@Override
	public void getMessage(IMethodResult result) {
		result.set(ConnectionCheckingSingleton.MESSAGE);
		
	}

	

	
    
}
