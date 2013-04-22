package com.rho.camera;

import java.io.File;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.provider.MediaStore;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesAppOptions;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.util.Utils;

public class CameraFactory extends AbstractRhoListener implements ICameraFactory, IRhoListener {
    private static final String TAG = CameraFactory.class.getSimpleName();
    
    private CameraSingleton mSingleton;
    private Camera[] mCameraList;
    private IMethodResult mMethodResult;

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
        CameraFactorySingleton.setInstance(this);
        extManager.addRhoListener(this);
        extManager.registerExtension("RhoCameraApi", new CameraExtension());
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
        if (mSingleton == null) {
            mSingleton = new CameraSingleton();
            
            int count = mSingleton.getCameraCount();
            mCameraList = new Camera[count];
        }
        return mSingleton;
    }
    
    Camera getCameraObject(String id) {
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

    void setMethodResult(IMethodResult result) {
        mMethodResult = result;
    }
    
    void releaseMethodResult() {
        mMethodResult = null;
    }

    @Override
    public void onActivityResult(RhodesActivity activity, int requestCode, int resultCode, Intent intent) {
        Logger.I(TAG, "Result intent: " + intent + ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
        try {
            if (resultCode == Activity.RESULT_OK) {
                Uri uri = (Uri)intent.getParcelableExtra(MediaStore.EXTRA_OUTPUT);
                Logger.T(TAG, "Photo is capltured: " + uri);
                String curPath = uri.getPath();
                File curFile = new File(curPath);
                String filename = Utils.getBaseName(curPath);
                if (!curFile.isFile()) {
                    Logger.E(TAG, "Captured photo file does not exist: " + curPath);
                    mMethodResult.setError("Captured photo file does not exist: " + curPath);
                } else {
                    if (!curPath.startsWith(RhodesAppOptions.getBlobPath())) {
                        String dstPath = RhodesAppOptions.getBlobPath() + "/" + filename;
                        Utils.copy(curPath, dstPath);
                        Logger.T(TAG, "File copied to " + dstPath);
                    }
                    mMethodResult.collect("image_uri", "db%2Fdb-files%2F/" + filename);
                    mMethodResult.collect("status", "ok");
                    mMethodResult.set();
                }
            } else if (resultCode == Activity.RESULT_CANCELED) {
                mMethodResult.setError("Cancelled by user");
            } else {
                mMethodResult.setError("Unknown error");
            }
        } catch (Throwable err) {
            Logger.E(TAG, err);
            mMethodResult.setError(err.getMessage());
        }
        releaseMethodResult();
    }
    
}
