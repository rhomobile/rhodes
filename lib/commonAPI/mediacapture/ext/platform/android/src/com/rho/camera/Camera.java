package com.rho.camera;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Map;

import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesAppOptions;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.util.ContextFactory;

public class Camera implements ICamera {
    private static final String TAG = Camera.class.getSimpleName();
  
    private String mId;
    
    Camera(String id) {
        mId = id;
    }

    @Override
    public String getId() {
        return mId;
    }

    @Override
    public void getAllProperties(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getProperty(String name, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getProperties(List<String> names, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setProperty(String name, String val, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setProperties(Map<String, String> props, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void clearAllProperties(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getCameraType(IMethodResult result) {
        android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
        android.hardware.Camera.getCameraInfo(Integer.valueOf(mId).intValue(), info);
        switch (info.facing) {
        case android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK:
            result.set("back");
            break;
        case android.hardware.Camera.CameraInfo.CAMERA_FACING_FRONT:
            result.set("front");
            break;
        default:
            result.setError("Camera with unknown type: " + info.facing);
            break;
        }
    }

    @Override
    public void getMaxWidth(IMethodResult result) {
        android.hardware.Camera hwCamera = android.hardware.Camera.open(Integer.valueOf(mId).intValue());
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
        android.hardware.Camera hwCamera = android.hardware.Camera.open(Integer.valueOf(mId).intValue());
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
    public void getDesiredWidth(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setDesiredWidth(int desiredWidth, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getDesiredHeight(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setDesiredHeight(int desiredHeight, IMethodResult result) {
        // TODO Auto-generated method stub

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
    public void getOutputFormat(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setOutputFormat(String outputFormat, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getColorModel(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setColorModel(String colorModel, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getEnableEditing(IMethodResult result) {
        //iOS only
    }

    @Override
    public void setEnableEditing(boolean enableEditing, IMethodResult result) {
        //iOS only
    }

    @Override
    public void getFlashMode(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setFlashMode(String flashMode, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getSaveToDeviceGallery(IMethodResult result) {
        //iOS only
    }

    @Override
    public void setSaveToDeviceGallery(boolean saveToSharedGallery, IMethodResult result) {
        //iOS only
    }

    @Override
    public void getCaptureSound(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setCaptureSound(String captureSound, IMethodResult result) {
        // TODO Auto-generated method stub

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
    public void getPreviewWidth(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setPreviewWidth(int previewWidth, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getPreviewHeight(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setPreviewHeight(int previewHeight, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void takePicture(Map<String, String> propertyMap, IMethodResult result) {
        ((CameraFactory)CameraFactorySingleton.getInstance()).setMethodResult(result);
        
        SimpleDateFormat timeStampFormat = new SimpleDateFormat("yyyyMMddHHmmssSS");
        String filename = "Image_" + timeStampFormat.format(new Date()) + ".jpg";

        File file = new File(Environment.getExternalStorageDirectory(), filename);
        
        //String dir = RhodesAppOptions.getBlobPath();
        
        Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(file));
        RhodesActivity.safeGetInstance().startActivityForResult(intent, Integer.valueOf(mId).intValue());
    }

    @Override
    public void getSupportedSizeList(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void getUseSystemViewfinder(IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setUseSystemViewfinder(boolean useSystemViewfinder,
            IMethodResult result) {
        // TODO Auto-generated method stub
        
    }

}
