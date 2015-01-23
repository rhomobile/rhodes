package com.rho.camera;

import java.io.File;
import java.util.List;

import android.app.Activity;
import android.hardware.Camera;
import android.os.Environment;

import com.rhomobile.rhodes.Logger;

public class CameraGingerbread extends CameraEclair implements ICameraObject {
    private static final String TAG = CameraGingerbread.class.getSimpleName();
    
    private int mCameraUsers = 0;

    CameraGingerbread(String id) { 
        super(id); 

        getPropertiesMap().put("compressionFormat", "jpg");
        getPropertiesMap().put("outputFormat", "image");

        android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
        android.hardware.Camera.getCameraInfo(getCameraIndex(), info);
        switch (info.facing) {
        case android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK:
            getPropertiesMap().put("cameraType", "back");
            break;
        case android.hardware.Camera.CameraInfo.CAMERA_FACING_FRONT:
            getPropertiesMap().put("cameraType", "front");
            break;
        default:
            getPropertiesMap().put("cameraType", "unknown");
            break;
        }
        
        openCamera();
        Camera.Parameters params = getCamera().getParameters();
        closeCamera();
        
        List<android.hardware.Camera.Size> listSize = params.getSupportedPictureSizes();
        ISize maxSize = null;
        for(android.hardware.Camera.Size curSize: listSize) {
            Logger.D(TAG, "Possible picture size: " + curSize.width + "X" + curSize.height);
            if ((maxSize == null) || (curSize.width > maxSize.getWidth())) {
                maxSize = new CameraSize(curSize);
            }
        }

        getPropertiesMap().put("maxWidth", String.valueOf(maxSize.getWidth()));
        getPropertiesMap().put("maxHeight", String.valueOf(maxSize.getHeight()));
    }
    

    @Override synchronized
    protected void openCamera() { 
        if (mCameraUsers == 0) {
            setCamera(Camera.open(getCameraIndex())); 
        }
        mCameraUsers++;
    }

    @Override synchronized
    protected void closeCamera() {
        mCameraUsers--;
        if (mCameraUsers == 0) {
            getCamera().release();
        }
    }


    protected String getTemporaryPath(String targetPath) {
        if(Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            File externalRoot = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
            if (! externalRoot.exists()){
                if (! externalRoot.mkdirs()){
                    Logger.E(TAG, "Failed to create directory: " + externalRoot);
                    return null;
                }
            }
            String filename = new File(targetPath).getName();
            return new File(externalRoot, filename).getAbsolutePath();
        } else {
            return null;
        }
    }
    @Override
    public void doTakePicture(final Activity previewActivity, int rotation) {
        Logger.T(TAG, "doTakePicture: rotation: " + rotation);
        android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
        android.hardware.Camera.getCameraInfo(getCameraIndex(), info);
        openCamera();
        ISize pictureSize = getDesiredSize();
        Camera.Parameters params = getCamera().getParameters();
        if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
            params.setRotation((info.orientation - rotation + 360) % 360);
        } else {
            params.setRotation((info.orientation + rotation) % 360);
        }
        if (pictureSize != null) {
            params.setPictureSize(pictureSize.getWidth(), pictureSize.getHeight());
        }
        getCamera().setParameters(params);
        if (hasAutoFocus()) {
            getCamera().autoFocus(new Camera.AutoFocusCallback() {
                public void onAutoFocus(boolean success, Camera camera) {
                    openCamera();
                    getCamera().takePicture(null, null, new TakePictureCallback(previewActivity));
                    closeCamera();
                }
            });
            
        } else {
            getCamera().takePicture(null, null, new TakePictureCallback(previewActivity));
        }
        closeCamera();
    }
}
