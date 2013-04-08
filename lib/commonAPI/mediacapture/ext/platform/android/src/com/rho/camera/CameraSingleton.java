package com.rho.camera;

import java.util.Map;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;

public class CameraSingleton implements ICameraSingleton {
    private static final String TAG = CameraSingleton.class.getSimpleName();

    private int mId;

    int getCameraCount() {
        return android.hardware.Camera.getNumberOfCameras();
    }

    public CameraSingleton() {
        int camera_count = getCameraCount();
        int i;
        for (i = 0 ; i < camera_count; i++) {
            android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
            android.hardware.Camera.getCameraInfo(i, info);
            if (info.facing == android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK) {
                mId = i;
                return;
            }
        }
    }
    
    @Override
    public String getDefaultID() {
        return String.valueOf(mId);
    }

    @Override
    public void setDefaultID(String id) {
        mId = Integer.valueOf(id).intValue();
    }

    @Override
    public void enumerate(IMethodResult result) {
        int cameraCount = android.hardware.Camera.getNumberOfCameras();
        for (int i = 0 ; i < cameraCount; i++) {
            result.collect(String.valueOf(i));
        }
        result.set();
    }

    @Override
    public void getCameraByType(String cameraType, IMethodResult result) {
        int cameraTypeId = android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK;
        if (cameraType.equalsIgnoreCase("front")) {
            cameraTypeId = android.hardware.Camera.CameraInfo.CAMERA_FACING_FRONT;
        } else if (cameraType.equalsIgnoreCase("front")) {
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
                result.set(String.valueOf(i));
                return;
            }
        }
    }

    @Override
    public void choosePicture(Map<String, String> propertyMap, IMethodResult result) {
        // TODO Auto-generated method stub
        
    }
}
