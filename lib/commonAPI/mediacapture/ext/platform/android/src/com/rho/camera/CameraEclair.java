package com.rho.camera;

import java.util.List;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;

public class CameraEclair extends CameraObject implements ICameraObject {
    private static final String TAG = CameraEclair.class.getSimpleName();

    CameraEclair(String id) { super(id); }

    @Override
    public void getMaxWidth(IMethodResult result) {
        android.hardware.Camera hwCamera = android.hardware.Camera.open();
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
        android.hardware.Camera hwCamera = android.hardware.Camera.open();
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
