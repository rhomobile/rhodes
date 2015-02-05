package com.rho.camera;

import android.hardware.Camera;
import android.os.Build;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.osfunctionality.OsVersionManager;

public class CameraFactory implements ICameraFactory {
    private static final String TAG = CameraFactory.class.getSimpleName();
    
    private ICameraSingletonObject mSingleton;
    private ICameraObject[] mCameraList;
    private CameraRhoListener mRhoListener;
    
    public CameraFactory(CameraRhoListener rhoListener) {
        mRhoListener = rhoListener;

        OsVersionManager.registerSelector(ICameraSingletonObject.class, CameraSingletonObject.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.ECLAIR, ICameraSingletonObject.class, CameraSingletonEclair.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.GINGERBREAD, ICameraSingletonObject.class, CameraSingletonGingerbread.class.getCanonicalName());
    }

    @Override
    public ICameraSingleton getApiSingleton() {
        return getCameraSingleton();
    }

    @Override
    public ICamera getApiObject(String id) {
        return getCameraObject(id);
    }
    
    ICameraSingletonObject getCameraSingleton() {
        if (mSingleton == null) {
            mSingleton = OsVersionManager.getFeature(ICameraSingletonObject.class);
            
            int count = mSingleton.getCameraCount();
            mCameraList = new ICameraObject[count];
        }
        return mSingleton;
    }
    
    ICameraObject getCameraObject(String id) {
        int idx = CameraSingletonObject.getCameraIndex(id);

        if (idx >= mCameraList.length) {
            Logger.E(TAG, "Unknown camera id: " + id);
            return null;
        }

        if (mCameraList[idx] == null) {
            mCameraList[idx] = getCameraSingleton().createCameraObject(id);
        }
        return mCameraList[idx];
    }
    
    CameraRhoListener getRhoListener() {
        return mRhoListener;
    }
}
