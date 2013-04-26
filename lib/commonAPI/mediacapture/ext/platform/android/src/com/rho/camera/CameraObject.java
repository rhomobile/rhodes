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

public class CameraObject extends CameraBase implements ICameraObject {
    private static final String TAG = CameraObject.class.getSimpleName();
    
    private Map<String, String> mActualPropertyMap;
    public Map<String, String> getActualPropertyMap() { return mActualPropertyMap; }

    CameraObject(String id) { super(id); }

    @Override
    public void getCameraType(IMethodResult result) {
            result.set("back");
    }

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

    @Override
    public void getCompressionFormat(IMethodResult result) {
        result.set("jpg");
    }

    @Override
    public void setCompressionFormat(String compressionFormat, IMethodResult result) {
        if (!compressionFormat.equalsIgnoreCase("jpg")) {
            Logger.E(TAG, "Android does not support the compression format: " + compressionFormat);
            result.setError("Android does not support the compression format: " + compressionFormat);
        }
    }

    @Override
    public void getPreviewLeft(IMethodResult result) {
        //WM only
    }

    @Override
    public void setPreviewLeft(int previewLeft, IMethodResult result) {
        //WM only
    }

    @Override
    public void getPreviewTop(IMethodResult result) {
        //WM only
    }

    @Override
    public void setPreviewTop(int previewTop, IMethodResult result) {
        //WM only
    }

    @Override
    public void takePicture(Map<String, String> propertyMap, IMethodResult result) {
        Logger.T(TAG, "takePicture");
        try {
            mActualPropertyMap = new HashMap<String, String>();
            mActualPropertyMap.putAll(getPropertiesMap());
            mActualPropertyMap.putAll(propertyMap);
            
            String outputFormat = mActualPropertyMap.get("outputFormat");
            String filePath = null;
            if (outputFormat.equalsIgnoreCase("image")) {
                filePath = mActualPropertyMap.get("fileName") + ".jpg";
            } else
            if (outputFormat.equalsIgnoreCase("dataUri")) {
                
            } else {
                throw new RuntimeException("Unknown 'outputFormat' value: " + outputFormat);
            }
            
            ((CameraFactory)CameraFactorySingleton.getInstance()).getRhoListener().setMethodResult(result);
            ((CameraFactory)CameraFactorySingleton.getInstance()).getRhoListener().setActualPropertyMap(mActualPropertyMap);

            boolean useSystemViewfinder = Boolean.parseBoolean(mActualPropertyMap.get("useSystemViewfinder"));
            Intent intent = null;
            if (useSystemViewfinder) {
                intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
    
                if (outputFormat.equalsIgnoreCase("image")) {
                    String tmpPath = getTemporaryPath(filePath);
                    if (tmpPath == null) {
                        throw new RuntimeException("Failed to access shared temporary folder");
                    }
                    mActualPropertyMap.put("tempPath", tmpPath);
                    File file = new File(tmpPath);
                    intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(file));
                } else
                if (outputFormat.equalsIgnoreCase("dataUri")) {
                    
                }
            } else {
                intent = new Intent(ContextFactory.getUiContext(), CameraActivity.class);
                intent.putExtra(CameraExtension.INTENT_EXTRA_PREFIX + "CAMERA_ID", getId());
            }
            RhodesActivity.safeGetInstance().startActivityForResult(intent, Integer.valueOf(getId()).intValue());
        } catch (RuntimeException e) {
            Logger.E(TAG, e);
            result.setError(e.getMessage());
        }
    }

    protected String getTemporaryPath(String targetPath) {
        if(Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            File externalRoot = Environment.getExternalStorageDirectory();
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
    public void getSupportedSizeList(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

}
