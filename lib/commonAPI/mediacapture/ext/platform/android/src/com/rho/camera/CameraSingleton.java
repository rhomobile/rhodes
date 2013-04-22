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
        Logger.T(TAG, "enumerate");
        int cameraCount = getCameraCount();
        Logger.T(TAG, "Number of cameras: " + cameraCount);
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
        ((CameraFactory)CameraFactorySingleton.getInstance()).setMethodResult(result);

        RhodesActivity ra = RhodesActivity.safeGetInstance();
        Intent intent = new Intent(ra, FileList.class);
        ra.startActivityForResult(intent, 0);
    }

    @Override
    public void saveImageToDeviceGallery(String pathToImage, IMethodResult result) {
        // TODO Auto-generated method stub

    }
}
