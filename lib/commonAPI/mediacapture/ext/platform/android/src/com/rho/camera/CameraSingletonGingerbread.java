package com.rho.camera;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;

public class CameraSingletonGingerbread extends CameraSingletonEclair implements ICameraSingleton {
    private static final String TAG = CameraSingletonGingerbread.class.getSimpleName();

    @Override
    public int getCameraCount() {
        Logger.T(TAG, "getCameraCount");
        return android.hardware.Camera.getNumberOfCameras();
    }

    public CameraSingletonGingerbread() {
        int camera_count = getCameraCount();
        for (int i = 0 ; i < camera_count; i++) {
            android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
            android.hardware.Camera.getCameraInfo(i, info);
            if (info.facing == android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK) {
                setDefaultIndex(i);
                return;
            }
        }
    }
    

    @Override
    public void getCameraByType(String cameraType, IMethodResult result) {
        int cameraTypeId = android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK;
        if (cameraType.equalsIgnoreCase("front")) {
            Logger.T(TAG, "Requesting front camera.");
            cameraTypeId = android.hardware.Camera.CameraInfo.CAMERA_FACING_FRONT;
        } else if (cameraType.equalsIgnoreCase("back")) {
            Logger.T(TAG, "Requesting back camera.");
            cameraTypeId = android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK;
        } else {
            Logger.E(TAG, "Unknown camera type requested: " + cameraType);
            result.setArgError("Unknown camera type requested: " + cameraType);
            return;
        }

        int cameraCount = getCameraCount();
        int i;
        for (i = 0 ; i < cameraCount; i++) {
            android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
            android.hardware.Camera.getCameraInfo(i, info);
            if (info.facing == cameraTypeId) {
                result.set(getCameraId(i));
                return;
            }
        }
    }

    @Override
    public ICameraObject createCameraObject(String id) {
        Logger.T(TAG, "createCameraObject: " + id);
        return new CameraGingerbread(id);
    }
}
