package com.rho.camera;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;

public class CameraFactory extends AbstractRhoListener implements ICameraFactory, IRhoListener {
    private static final String TAG = CameraFactory.class.getSimpleName();
    
    private CameraSingleton mSingleton;
    private Camera[] mCameraList;

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
        CameraFactorySingleton.setInstance(this);
    }

    @Override
    public ICameraSingleton getApiSingleton() {
        if (mSingleton == null) {
            mSingleton = new CameraSingleton();
            
            int count = mSingleton.getCameraCount();
            mCameraList = new Camera[count];
        }
        return mSingleton;
    }

    @Override
    public ICamera getApiObject(String id) {
        int idx = Integer.valueOf(id).intValue();

        if (idx >= mCameraList.length) {
            Logger.E(TAG, "Unknown camera id: " + id);
            return null;
        }

        if (mCameraList[idx] == null) {
            mCameraList[idx] = new Camera(id);
        }
        return mCameraList[idx];
    }

}
