package com.rho.mediacapture;

import com.rho.camera.CameraFactory;
import com.motorolasolutions.rho.videocapture.VideocaptureFactory;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;

public class RhoListener extends AbstractRhoListener implements IRhoListener {

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {

    	try
    	{
    		new CameraFactory().onCreateApplication(extManager);
    		new VideocaptureFactory().onCreateApplication(extManager);
    	}
    	catch(NullPointerException e)
    	{
    		Logger.E("RhoListener", "Rho Listener null pointer exception");
    	}

    }

}
