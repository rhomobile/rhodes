package com.rho.camera;

import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.util.ContextFactory;

public class CameraGingerbread extends CameraEclair implements ICameraObject {
    private static final String TAG = CameraGingerbread.class.getSimpleName();
    
    private Map<String, String> mActualPropertyMap;
    public Map<String, String> getActualPropertyMap() { return mActualPropertyMap; }

    CameraGingerbread(String id) {
        super(id);
    }
    
    @Override
    public void getCameraType(IMethodResult result) {
        android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
        android.hardware.Camera.getCameraInfo(Integer.valueOf(getId()).intValue(), info);
        switch (info.facing) {
        case android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK:
            result.set("back");
            break;
        case android.hardware.Camera.CameraInfo.CAMERA_FACING_FRONT:
            result.set("front");
            break;
        default:
            result.setError("Camera with unknown type: " + info.facing);
            break;
        }
    }

    @Override
    public void getMaxWidth(IMethodResult result) {
        android.hardware.Camera hwCamera = android.hardware.Camera.open(Integer.valueOf(getId()).intValue());
        List<android.hardware.Camera.Size> listSize = hwCamera.getParameters().getSupportedPictureSizes();
        int maxWidth = 0;
        for(android.hardware.Camera.Size curSize: listSize) {
            Logger.D(TAG, "Possible picture width: " + curSize.width);
            if (curSize.width > maxWidth) {
                maxWidth = curSize.width;
            }
        }
        hwCamera.release();
        result.set(maxWidth);
    }

    @Override
    public void getMaxHeight(IMethodResult result) {
        android.hardware.Camera hwCamera = android.hardware.Camera.open(Integer.valueOf(getId()).intValue());
        List<android.hardware.Camera.Size> listSize = hwCamera.getParameters().getSupportedPictureSizes();
        int maxHeight = 0;
        for(android.hardware.Camera.Size curSize: listSize) {
            Logger.D(TAG, "Possible picture height: " + curSize.height);
            if (curSize.height > maxHeight) {
                maxHeight = curSize.height;
            }
        }
        hwCamera.release();
        result.set(maxHeight);
    }
}
