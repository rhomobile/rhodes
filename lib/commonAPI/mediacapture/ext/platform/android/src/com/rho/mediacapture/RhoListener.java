package com.rho.mediacapture;

import com.rho.camera.CameraFactory;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;

public class RhoListener extends AbstractRhoListener implements IRhoListener {

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {

        new CameraFactory().onCreateApplication(extManager);

    }

}
