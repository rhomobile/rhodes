package com.rho.videocapture;

import java.util.LinkedList;
import java.util.List;

import com.rho.videocapture.IVideocaptureSingleton;
import com.rho.videocapture.VideocaptureSingletonBase;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;

public class VideocaptureSingleton extends VideocaptureSingletonBase implements IVideocaptureSingleton {
    private static final String TAG = VideocaptureSingleton.class.getSimpleName();
    private static final String ID = "_DEFAULT_CAMERA"; //This is the default Video capture object
    
    public VideocaptureSingleton(VideocaptureFactory factory)
    {
    	super(factory);
    }
    
    @Override
    public void enumerate(IMethodResult result) {
        result.set(getIDs());
    }
    
    List<Object> getIDs() {
    	Logger.D(TAG, "VideocaptureSingleton getIDs");
        List<Object> ids = new LinkedList<Object>();
        ids.add(ID); 
        return ids;
    }

	@Override
	protected String getInitialDefaultID()
	{
		return (String) getIDs().get(0);
	}

}
