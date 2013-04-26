package com.rho.camera;

import java.io.File;
import java.util.Map;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.provider.MediaStore;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesAppOptions;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import com.rhomobile.rhodes.util.Utils;

public class CameraRhoListener extends AbstractRhoListener implements IRhoListener{

    private static final String TAG = CameraRhoListener.class.getSimpleName();

    private IMethodResult mMethodResult;
    private Map<String, String> mActualPropertyMap;

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
        CameraFactorySingleton.setInstance(new CameraFactory(this));
        extManager.addRhoListener(this);
        extManager.registerExtension("RhoCameraApi", new CameraExtension());
    }
    @Override
    public void onActivityResult(RhodesActivity activity, int requestCode, int resultCode, Intent intent) {
        try {
            if (resultCode == Activity.RESULT_OK) {
                String curPath = null;
                if (intent != null && intent.hasExtra(MediaStore.EXTRA_OUTPUT)) {
                    Logger.T(TAG, "Intent extras: " + intent.getExtras().keySet());
                    
                    Uri uri = (Uri)intent.getParcelableExtra(MediaStore.EXTRA_OUTPUT);
                    if (uri == null) {
                        uri = intent.getData();
                    }
                    Logger.T(TAG, "Photo is capltured: " + uri);
                    curPath = uri.getPath();
                }
                else {
                    Logger.T(TAG, "Wrong intent: " + intent);
                    curPath = getActualPropertyMap().get("tempPath");
                    Logger.T(TAG, "Use fallback path: " + curPath);
                }
                String targetPath = getActualPropertyMap().get("fileName") + ".jpg";
                File curFile = new File(curPath);

                if (!curFile.isFile()) {
                    Logger.E(TAG, "Captured photo file does not exist: " + curPath);
                    mMethodResult.setError("Captured photo file does not exist: " + curPath);
                } else {
                    if (!curPath.equals(targetPath)) {
                        Utils.copy(curPath, targetPath);
                        curFile.delete();
                        Logger.T(TAG, "File copied to " + targetPath);
                    }
                    mMethodResult.collect("imageUri", targetPath);
                    mMethodResult.collect("image_uri", targetPath);
                    mMethodResult.collect("status", "ok");
                    mMethodResult.set();
                }
            } else if (resultCode == Activity.RESULT_CANCELED) {
                if (intent.hasExtra("error")) {
                    mMethodResult.setError(intent.getStringExtra("error"));
                } else {
                    mMethodResult.collect("status", "cancel");
                    mMethodResult.set();
                }
            } else {
                mMethodResult.setError("Unknown error");
            }
        } catch (Throwable err) {
            Logger.E(TAG, err);
            mMethodResult.setError(err.getMessage());
        }
        releaseMethodResult();
    }
    
    void setMethodResult(IMethodResult result) {
        mMethodResult = result;
    }
    
    void releaseMethodResult() {
        mMethodResult = null;
    }

    void setActualPropertyMap(Map<String, String> propertyMap) {
        mActualPropertyMap = propertyMap;
    }

    Map<String, String> getActualPropertyMap() {
        return mActualPropertyMap;
    }
}
