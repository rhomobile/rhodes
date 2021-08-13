package com.rho.camera;

import android.hardware.Camera;
import android.os.Build;
import java.util.HashMap;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.osfunctionality.OsVersionManager;

public class CameraFactory implements ICameraFactory {
    private static final String TAG = CameraFactory.class.getSimpleName();
    
    static private CameraSingleton singleton = null;
    private HashMap<String, ICamera> camerasMap = new HashMap<String, ICamera>();
    private CameraRhoListener mRhoListener;
    
    public CameraFactory(CameraRhoListener rhoListener) {
        mRhoListener = rhoListener;
        OsVersionManager.registerSelector(ICameraSingleton.class, CameraSingleton.class.getCanonicalName());
    }

    @Override
    public ICameraSingleton getApiSingleton() {
        return getCameraSingleton();
    }

    @Override
    public ICamera getApiObject(String id) {
        return getCameraObject(id);
    }
    
    CameraSingleton getCameraSingleton() {
        if (singleton == null) {
            singleton = new CameraSingleton();
        }
        return singleton;
    }
    
    ICamera getCameraObject(String id) {
        if (!camerasMap.containsKey(id)){
            ICamera c = null;
            try{
                c = new CameraObject(id);
            }catch(Exception e){
                Logger.E(TAG, "Can't create camera with id: " + id);
                return null;
            }
            camerasMap.put(id, c);
        }

        return camerasMap.get(id);
    }
    
    CameraRhoListener getRhoListener() {
        return mRhoListener;
    }
}
