package com.rho.camera;

import java.io.File;
import java.util.Map;

import android.content.Intent;
import android.net.Uri;
import android.provider.MediaStore;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.ui.FileList;

public class CameraSingletonObject implements ICameraSingletonObject {
    private static final String TAG = CameraSingletonObject.class.getSimpleName();

    private int mId;
    
    static int getCameraIndex(String id) {
        return Integer.valueOf(id.substring(7)).intValue();
    }
    static String getCameraId(int idx) {
        return "camera#" + String.valueOf(idx);
    }

    @Override
    public int getCameraCount() {
        Logger.T(TAG, "getCameraCount");
        return 1;
    }

    public CameraSingletonObject() {
        mId = 0;
    }
    
    @Override
    public String getDefaultID() {
        return getCameraId(mId);
    }

    @Override
    public void setDefaultID(String id) {
        mId = getCameraIndex(id);
    }

    public void setDefaultIndex(int id) {
        mId = id;
    }

    @Override
    public void enumerate(IMethodResult result) {
        Logger.T(TAG, "enumerate");
        int cameraCount = getCameraCount();
        Logger.T(TAG, "Number of cameras: " + cameraCount);
        for (int i = 0 ; i < cameraCount; i++) {
            result.collect(getCameraId(i));
        }
        result.set();
    }

    @Override
    public void getCameraByType(String cameraType, IMethodResult result) {
        result.set(getDefaultID());
    }

    @Override
    public void choosePicture(Map<String, String> propertyMap, IMethodResult result) {
        //((CameraFactory)CameraFactorySingleton.getInstance()).getRhoListener().setActualPropertyMap(propertyMap);
        String fileName = propertyMap.get("fileName");
        if (fileName != null && fileName.length() > 0) {
            CameraFactory factory = (CameraFactory)CameraFactorySingleton.getInstance();
            factory.getRhoListener().setMethodResult(result);
            factory.getRhoListener().setActualPropertyMap(propertyMap);

            RhodesActivity ra = RhodesActivity.safeGetInstance();
            Intent intent = new Intent(ra, FileList.class);
            intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(new File(fileName + ".jpg")));
            ra.startActivityForResult(intent, 0);
        } else {
            result.setArgError("'fileName' parameter is missed");
        }
    }

    @Override
    public ICameraObject createCameraObject(String id) {
        Logger.T(TAG, "createCameraObject: " + id);
        return new CameraObject(id);
    }
    @Override
    public void copyImageToDeviceGallery(String pathToImage, IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

}
