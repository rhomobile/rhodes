package com.rho.camera;

import com.rhomobile.rhodes.Logger;

public class CameraSingletonEclair extends CameraSingletonObject  implements ICameraSingleton {
    private static final String TAG = CameraSingletonEclair.class.getSimpleName();

    @Override
    public ICameraObject createCameraObject(String id) {
        Logger.T(TAG, "createCameraObject: " + id);
        return new CameraEclair(id);
    }

}
