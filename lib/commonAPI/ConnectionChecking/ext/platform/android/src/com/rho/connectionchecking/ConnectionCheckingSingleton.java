package com.rho.connectionchecking;

import java.util.LinkedList;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;

class ConnectionCheckingSingleton extends ConnectionCheckingSingletonBase implements IConnectionCheckingSingleton {
	
	public static String HOST="192.168.7.129";//DUmmy
	public static String MESSAGE="Establishing Connection";
	public static int TIMEOUT=30000;
	public static int POLLI_INTERVAL=5000;
	public static int PORT=80;
	public static int TRACK_CONNECTION=0;
	public static String BADLINK_URI="";
	
	
	
	
    public ConnectionCheckingSingleton(ConnectionCheckingFactory factory) {
        super(factory);
    }

    List<Object> getIDs() {
        List<Object> ids = new LinkedList<Object>();
        ids.add("SCN1");
        ids.add("SCN2");
        return ids;
    }
    
    @Override
    protected String getInitialDefaultID() {
        return (String)(getIDs().get(0));
    }

	@Override
	public void getHostURL(IMethodResult result) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setHostURL(String hostURL, IMethodResult result) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void getTrackConnection(IMethodResult result) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setTrackConnection(int trackConnection, IMethodResult result) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void getTimeout(IMethodResult result) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setTimeout(int timeout, IMethodResult result) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void getPollInterval(IMethodResult result) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setPollInterval(int pollInterval, IMethodResult result) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void getMessage(IMethodResult result) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setMessage(String message, IMethodResult result) {
		// TODO Auto-generated method stub
		
	}

    
}