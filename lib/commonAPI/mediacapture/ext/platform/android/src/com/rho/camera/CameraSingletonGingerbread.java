package com.rho.camera;

import java.util.Map;

import android.content.Intent;
import android.net.Uri;
import android.provider.MediaStore;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.ui.FileList;
import com.rhomobile.rhodes.util.PerformOnUiThread;

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
                setDefaultID(i);
                return;
            }
        }
    }
    

    @Override
    public void getCameraByType(String cameraType, IMethodResult result) {
        int cameraTypeId = android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK;
        if (cameraType.equalsIgnoreCase("front")) {
            cameraTypeId = android.hardware.Camera.CameraInfo.CAMERA_FACING_FRONT;
        } else if (cameraType.equalsIgnoreCase("back")) {
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
    public ICameraObject createCameraObject(String id) {
        Logger.T(TAG, "createCameraObject: " + id);
        return new CameraGingerbread(id);
    }
}
