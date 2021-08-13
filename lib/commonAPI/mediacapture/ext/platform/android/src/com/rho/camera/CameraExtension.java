package com.rho.camera;

import com.rhomobile.rhodes.extmanager.AbstractRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoExtension;

public class CameraExtension extends AbstractRhoExtension implements IRhoExtension {
    
    private static final String TAG = CameraExtension.class.getSimpleName();
    
    @Override
    public String onGetProperty(IRhoExtManager extManager, String name) {
        if (name.equalsIgnoreCase("has_camera")) {
            if (((CameraFactory)(CameraFactorySingleton.getInstance())).getCameraSingleton().getCameraCount() > 0) {
                return "true";
            }
        }
        return null;
    }
    
}
