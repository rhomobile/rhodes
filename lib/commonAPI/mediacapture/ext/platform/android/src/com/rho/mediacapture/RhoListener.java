package com.rho.mediacapture;

import com.rho.videocapture.VideocaptureFactory;
import com.rho.camera.CameraFactory;
import com.rho.camera.CameraRhoListener;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;

public class RhoListener extends AbstractRhoListener implements IRhoListener {

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
        try {
            new CameraRhoListener().onCreateApplication(extManager);
            new VideocaptureFactory().onCreateApplication(extManager);
        }
        catch(NullPointerException e) {
            Logger.E("MediacaptureRhoListener", e);
        }
    }
}
