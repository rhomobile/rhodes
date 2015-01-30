package com.rho.camera;

import java.io.File;
import java.util.Map;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.provider.MediaStore;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.util.Utils;

public class CameraRhoListener extends AbstractRhoListener implements IRhoListener{

    private static final String TAG = CameraRhoListener.class.getSimpleName();

    private IMethodResult mMethodResult;
    private Map<String, String> mActualPropertyMap;
    private static CameraRhoListener sInstance;
    static CameraRhoListener getInstance() {
        return sInstance;
    }

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
        sInstance = this;
        CameraFactorySingleton.setInstance(new CameraFactory(this));
        extManager.addRhoListener(this);
        extManager.registerExtension("RhoCameraApi", new CameraExtension());
    }
    @Override
    public void onActivityResult(RhodesActivity activity, int requestCode, int resultCode, Intent intent) {
        RhoExtManager.getInstance().dropActivityResultRequestCode(requestCode);
        if (mMethodResult == null) {
            return;
        }
        try {
            if (resultCode == Activity.RESULT_OK) {
                String curPath = null;
                Uri captureUri = null;
                String strCaptureUri = getActualPropertyMap().get("captureUri");
                if (strCaptureUri != null) {
                    captureUri = Uri.parse(getActualPropertyMap().get("captureUri"));
                }
                Uri curUri = null;
                if (intent != null && intent.hasExtra(MediaStore.EXTRA_OUTPUT)) {
                    Logger.T(TAG, "Intent extras: " + intent.getExtras().keySet());
                    
                    curUri = (Uri)intent.getParcelableExtra(MediaStore.EXTRA_OUTPUT);
                    if (curUri == null) {
                        curUri = intent.getData();
                    }
                    Logger.T(TAG, "Photo is captured: " + curUri);
                }
                else if (captureUri != null){
                    Logger.T(TAG, "Use stored captureUri: " + captureUri);
                    curUri = captureUri;
                }
                else {
                    curUri = intent.getData();
                    Logger.T(TAG, "Check intent data: " + curUri);
                }

                if (curUri.getScheme().equals("file")) {
                    curPath = curUri.getPath();
                    String targetPath = getActualPropertyMap().get("fileName") + ".jpg";
                    File curFile = new File(curPath);
                    if (!curFile.isFile()) {
                        throw new RuntimeException("Captured photo file does not exist: " + curPath);
                    }
                    if (!curPath.equals(targetPath)) {
                        Utils.copy(curPath, targetPath);
                        curFile.delete();
                        Logger.T(TAG, "File copied to " + targetPath);
                        curUri = Uri.fromFile(new File(targetPath));
                    }
                }

                mMethodResult.collect("imageUri", curUri.toString());
                mMethodResult.collect("image_uri", curUri.toString());
                mMethodResult.collect("status", "ok");
                mMethodResult.set();
            } else if (resultCode == Activity.RESULT_CANCELED) {
                if (intent != null && intent.hasExtra("error")) {
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
