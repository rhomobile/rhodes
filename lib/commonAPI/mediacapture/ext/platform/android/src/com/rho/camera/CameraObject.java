package com.rho.camera;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.ArrayList;
import java.util.UUID;
 
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.hardware.Camera;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.view.SurfaceHolder;
import android.database.Cursor;
import android.content.pm.PackageManager;

import androidx.core.content.PermissionChecker;
import androidx.core.content.ContextCompat;
import android.Manifest;

import androidx.core.content.FileProvider;

import com.rhomobile.rhodes.Base64;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rho.camera.ICameraSingleton;

public class CameraObject extends CameraBase implements ICamera{
    private static final String TAG = CameraObject.class.getSimpleName();
    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyyMMdd_hhmmss");

    private Camera mCamera = null;
    private int mCameraUsers;   
    private File storageDir = null;
    private List<Camera.Size> mSupportedPictureSizes;

    public static boolean CURRENT_SCREEN_AUTO_ROTATE_MODE;
    public static boolean CURRENT_FULL_SCREEN_MODE;

    interface ISize {
        int getWidth();
        int getHeight();
        int D2();
    }

    static class CameraSize implements ISize {
        private Camera.Size mSize;
        CameraSize(Camera.Size size) { mSize = size; }
        @Override public int getWidth() { return mSize.width; }
        @Override public int getHeight() { return mSize.height; }
        @Override public int D2() { return mSize.width * mSize.width + mSize.height * mSize.height; }
        @Override public String toString() { return "" + mSize.width + "X" + mSize.height; }
    }

    static class RawSize implements ISize {
        private int width;
        private int height;
        RawSize(int width, int height) { this.width = width; this.height = height; }
        @Override public int getWidth() { return this.width; }
        @Override public int getHeight() { return this.height; }
        @Override public int D2() { return width * width + height * height; }
        @Override public String toString() { return "" + width + "X" + height; }
    }

    int getCameraIndex() {
        return CameraSingleton.getCameraIndex(getId());
    }

    @Override
    public void setProperties(Map<String, String> propertyMap, IMethodResult result) {
        getPropertiesMap().putAll(propertyMap);
        result.set(true);
    }

    @Override
    public void getProperties(List<String> arrayofNames, IMethodResult result) {
        Map<String, Object> props = new HashMap<String, Object>();
        for (String name: arrayofNames)
        {
            props.put(name, cameraPropGet(name));
        }
        result.set(props);
    }

    private String cameraPropGet(String name)
    {
        return getPropertiesMap().getOrDefault(name, "");
    }

    @Override
	public void getAllProperties(IMethodResult result) {
    	Map<String, Object> props = new HashMap<String, Object>();
    	for (String key: getPropertiesMap().keySet()) {
    		 props.put(key, cameraPropGet(key));
        }
    	result.set(props);
	}
	 
    protected Camera getCamera() { return mCamera; }
    protected void setCamera(Camera camera) { mCamera = camera; }

    CameraObject(String id) {
        super(id);

        getPropertiesMap().put("ChoosePicture_Key", "");
        getPropertiesMap().put(ICameraSingleton.PROPERTY_COMPRESSION_FORMAT, ICameraSingleton.COMPRESSION_FORMAT_JPG);
        getPropertiesMap().put(ICameraSingleton.PROPERTY_OUTPUT_FORMAT, ICameraSingleton.OUTPUT_FORMAT_IMAGE);
        getPropertiesMap().put(ICameraSingleton.PROPERTY_COLOR_MODEL, ICameraSingleton.COLOR_MODEL_RGB);
        getPropertiesMap().put(ICameraSingleton.PROPERTY_USE_REAL_BITMAP_RESIZE, "true");
        getPropertiesMap().put(ICameraSingleton.PROPERTY_USE_ROTATION_BITMAP_BY_EXIF, "true");
        getPropertiesMap().put(ICameraSingleton.PROPERTY_SAVE_TO_DEVICE_GALLERY, "false");

        storageDir = new File(Environment.getExternalStorageDirectory(), "RhoImages");
        createRhoCacheFolder();

        android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
        android.hardware.Camera.getCameraInfo(getCameraIndex(), info);
        switch (info.facing) {
        case android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK:
            getPropertiesMap().put(ICameraSingleton.PROPERTY_CAMERA_TYPE, ICameraSingleton.CAMERA_TYPE_BACK);
            break;
        case android.hardware.Camera.CameraInfo.CAMERA_FACING_FRONT:
            getPropertiesMap().put(ICameraSingleton.PROPERTY_CAMERA_TYPE, ICameraSingleton.CAMERA_TYPE_FRONT);
            break;
        default:
            getPropertiesMap().put(ICameraSingleton.PROPERTY_CAMERA_TYPE, "unknown");
            break;
        }

        if (hasPermission()) {
            openCamera();
            Camera.Parameters params = getCamera().getParameters();
            closeCamera();

            mSupportedPictureSizes = params.getSupportedPictureSizes();
            ISize maxSize = null;
            for(android.hardware.Camera.Size curSize: mSupportedPictureSizes) {
                Logger.D(TAG, "Possible picture size: " + curSize.width + "X" + curSize.height);
                if ((maxSize == null) || (curSize.width > maxSize.getWidth())) {
                    maxSize = new CameraSize(curSize);
                }
            }

            getPropertiesMap().put(ICameraSingleton.PROPERTY_MAX_WIDTH, String.valueOf(maxSize.getWidth()));
            getPropertiesMap().put(ICameraSingleton.PROPERTY_MAX_HEIGHT, String.valueOf(maxSize.getHeight()));
        }
        else {
            Logger.E(TAG, "Application has no permission to Camera access !!!");
            getPropertiesMap().put(ICameraSingleton.PROPERTY_MAX_WIDTH, String.valueOf(0));
            getPropertiesMap().put(ICameraSingleton.PROPERTY_MAX_HEIGHT, String.valueOf(0));
        }

        getPropertiesMap().put(ICameraSingleton.PROPERTY_DESIRED_WIDTH, "0");
        getPropertiesMap().put(ICameraSingleton.PROPERTY_DESIRED_HEIGHT, "0");
    }

    public void createRhoCacheFolder(){
        storageDir.mkdirs();
    }

    
    @Override
    public void setCompressionFormat(String compressionFormat, IMethodResult result) {
        if (!compressionFormat.equalsIgnoreCase(ICameraSingleton.COMPRESSION_FORMAT_JPG)) {
            Logger.E(TAG, "Android does not support the compression format: " + compressionFormat);
            result.setError("Android does not support the compression format: " + compressionFormat);
        }
    }

    public static String createFileName(){
        return dateFormat.format(new Date(System.currentTimeMillis())) + "_" + UUID.randomUUID().toString();
    }

    private File createImageFile() throws IOException {
        String imageFileName = "JPEG_" + createFileName() + "_";
        File storageDir = RhodesActivity.getContext().getExternalFilesDir(Environment.DIRECTORY_PICTURES);
        File image = File.createTempFile(imageFileName, ".jpg", storageDir);
        return image;
    }


    @Override
    public void takePicture(Map<String, String> propertyMap, IMethodResult result) {
        if (!hasPermission()) {
            Logger.E(TAG, "Application has no permission to Camera access !!!");
            HashMap<String, Object> inResultMap = new HashMap<String,Object>();
            inResultMap.put(ICameraSingleton.HK_MESSAGE, "No CAMERA permission !");
            inResultMap.put(ICameraSingleton.HK_STATUS, ICameraSingleton.STATUS_ERROR);
            result.set(inResultMap);
            return;
        }

        CURRENT_SCREEN_AUTO_ROTATE_MODE = RhodesActivity.safeGetInstance().getScreenAutoRotateMode();
        CURRENT_FULL_SCREEN_MODE = RhodesActivity.safeGetInstance().getFullScreenMode();

        Logger.T(TAG, "takePicture");
        try {
            Map<String, String> actualPropertyMap = new HashMap<String, String>();
            actualPropertyMap.putAll(getPropertiesMap());
            actualPropertyMap.putAll(propertyMap);

            String outputFormat = actualPropertyMap.get(ICameraSingleton.PROPERTY_OUTPUT_FORMAT);

            String fileDir = storageDir.getAbsolutePath();
            String fileName = actualPropertyMap.get(ICameraSingleton.PROPERTY_FILE_NAME);
            if (fileName == null || fileName.isEmpty()){
               fileName = "IMG_" + createFileName();
            }
            String filePath = fileDir + "/" + fileName + ".jpg";

            Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
            if (outputFormat.equalsIgnoreCase(ICameraSingleton.OUTPUT_FORMAT_IMAGE)) {
                ContentValues values = new ContentValues();
                if (!Boolean.parseBoolean(propertyMap.get(ICameraSingleton.PROPERTY_SAVE_TO_DEVICE_GALLERY))) {
                    values.put( MediaStore.Images.ImageColumns.DATA, filePath);
                    Logger.T(TAG, "Output filePath: " + filePath);
                }

                File photoFile = null;
                try {
                    photoFile = createImageFile();
                } catch (IOException ex) {
                    Logger.E(TAG, "$$$ Failed to create temporary file !!!");
                }
                
                if (photoFile != null) {
                    Uri fileUri = FileProvider.getUriForFile(RhodesActivity.getContext(),
                        RhodesActivity.getContext().getPackageName() + ".fileprovider", photoFile);
                    intent.putExtra(MediaStore.EXTRA_OUTPUT, fileUri);
                    actualPropertyMap.put("captureUri", fileUri.toString());
                    Logger.T(TAG, "Output fileUri: " + fileUri.toString());
                    actualPropertyMap.put("captureUriFilePath", photoFile.getAbsolutePath());
                }

                propertyMap.put(ICameraSingleton.OUTPUT_FORMAT_DATAURI, "");
                
                intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            } else if (outputFormat.equalsIgnoreCase(ICameraSingleton.OUTPUT_FORMAT_DATAURI)) {}


            ((CameraFactory)CameraFactorySingleton.getInstance()).getRhoListener().setMethodResult(result);
            ((CameraFactory)CameraFactorySingleton.getInstance()).getRhoListener().setActualPropertyMap(actualPropertyMap);

            RhodesActivity.safeGetInstance().startActivityForResult(intent, 
                RhoExtManager.getInstance().getActivityResultNextRequestCode(CameraRhoListener.getInstance()));
        } catch (RuntimeException e) {
            Logger.E(TAG, e);
            result.setError(e.getMessage());
        }
    }

    @Override
    public void getSupportedSizeList(IMethodResult result) {
        List<Object> res = new ArrayList<Object>();
        for(Camera.Size size: mSupportedPictureSizes) {
            Map<String, Integer> resSize = new HashMap<String, Integer>();
            resSize.put(ICameraSingleton.HK_WIDTH, Integer.valueOf(size.width));
            resSize.put(ICameraSingleton.HK_HEIGHT, Integer.valueOf(size.height));
            res.add(resSize);
        }
        result.set(res);
    }

    public void finalize() {
        if (getCamera() != null) {
            getCamera().release();
        }
    }


    public boolean hasPermission() {
        boolean res = (PermissionChecker.checkSelfPermission(RhodesActivity.getContext(), Manifest.permission.CAMERA) == 
            PackageManager.PERMISSION_GRANTED);
        if (!res) {
            Logger.E(TAG, "Application has no permission to Camera access !!!");
        }
        return res;
    }

    protected ISize getDesiredSize() {
        String strDesiredWidth = getPropertiesMap().get(ICameraSingleton.PROPERTY_DESIRED_WIDTH);
        String strDesiredHeight = getPropertiesMap().get(ICameraSingleton.PROPERTY_DESIRED_HEIGHT);

        if (Boolean.parseBoolean(getPropertiesMap().get(ICameraSingleton.PROPERTY_USE_REAL_BITMAP_RESIZE))) {
            return null;
        }

        if (Integer.valueOf(strDesiredWidth) <= 0) {
            strDesiredWidth = null;
        }
        if (Integer.valueOf(strDesiredHeight) <= 0) {
            strDesiredHeight = null;
        }

        int minDiff = Integer.MAX_VALUE;
        ISize selectedSize = null;

        if (strDesiredWidth != null && strDesiredHeight != null) {
            ISize desiredSize = new RawSize(Integer.valueOf(strDesiredWidth),
                                            Integer.valueOf(strDesiredHeight));
            Logger.T(TAG, "Desired picture size: " + desiredSize);
            for(Camera.Size size: mSupportedPictureSizes) {
                ISize curSize = new CameraSize(size);
                int curDiff = Math.abs(curSize.D2() - desiredSize.D2());
                if (curDiff < minDiff) {
                    minDiff = curDiff;
                    selectedSize = curSize;
                }
            }
        } else if (strDesiredWidth != null) {
            int desiredWidth = Integer.valueOf(strDesiredWidth);
            Logger.T(TAG, "Desired picture width: " + desiredWidth);
            for(Camera.Size size: mSupportedPictureSizes) {
                int curDiff = Math.abs(size.width - desiredWidth);
                if (curDiff < minDiff) {
                    minDiff = curDiff;
                    selectedSize = new CameraSize(size);
                }
            }
        } else if (strDesiredHeight != null) {
            int desiredHeight = Integer.valueOf(strDesiredHeight);
            Logger.T(TAG, "Desired picture width: " + desiredHeight);
            for(Camera.Size size: mSupportedPictureSizes) {
                int curDiff = Math.abs(size.height - desiredHeight);
                if (curDiff < minDiff) {
                    minDiff = curDiff;
                    selectedSize = new CameraSize(size);
                }
            }
        }
        Logger.T(TAG, "Selected picture size: " + selectedSize);
        return selectedSize;
    }

    protected boolean hasAutoFocus() {
        if (!hasPermission()) {
            Logger.D(TAG, "Application has no permission to Camera access !!!");
            return false;
        }
        String focusMode = getCamera().getParameters().getFocusMode();
        boolean supported = false;
        if (focusMode != null) {
            supported = (focusMode.equals(android.hardware.Camera.Parameters.FOCUS_MODE_AUTO)) || (focusMode.equals(android.hardware.Camera.Parameters.FOCUS_MODE_MACRO));
        }
        return supported;

    }

    protected String getFlashMode() {
        String flashParam = getPropertiesMap().get(ICameraSingleton.PROPERTY_FLASH_MODE);
        String flashMode = Camera.Parameters.FLASH_MODE_AUTO;
        if (flashParam != null) {
            if (flashParam.equals(ICameraSingleton.FLASH_ON)) {
                flashMode = Camera.Parameters.FLASH_MODE_ON;
            } else
                if (flashParam.equals(ICameraSingleton.FLASH_OFF)) {
                    flashMode = Camera.Parameters.FLASH_MODE_OFF;
                } else
                    if (flashParam.equals(ICameraSingleton.FLASH_RED_EYE)) {
                        flashMode = Camera.Parameters.FLASH_MODE_RED_EYE;
                    } else
                        if (flashParam.equals(ICameraSingleton.FLASH_TORCH)) {
                            flashMode = Camera.Parameters.FLASH_MODE_TORCH;
                        }
        }
        Logger.T(TAG, "Flash mode: " + flashMode);
        return flashMode;
    }

    protected String getColorMode() {
        String colorParam = getPropertiesMap().get(ICameraSingleton.PROPERTY_COLOR_MODEL);
        String colorMode = Camera.Parameters.EFFECT_NONE;
        if (colorParam != null) {
            if (colorParam.equals(ICameraSingleton.COLOR_MODEL_GRAYSCALE)) {
                colorMode = Camera.Parameters.EFFECT_MONO;
            }
        }
        Logger.T(TAG, "Color effect: " + colorMode);
        return colorMode;
    }

    synchronized protected void openCamera() {
        if (hasPermission()) {
            if (mCameraUsers == 0) {
                setCamera(Camera.open(getCameraIndex()));
            }
            mCameraUsers++;
        }
    }

    synchronized protected void closeCamera() {
        if (hasPermission()) {
            mCameraUsers--;
            if (mCameraUsers == 0) {
                getCamera().release();

            }
        }
    }


    /* Here comes the stubs */

    void notSupported(IMethodResult result){
        result.setError("Not supported");
    }

    @Override
    public void capture(IMethodResult result){
        notSupported(result);
    }

    @Override
    public void showPreview(Map<String, String> propertyMap, IMethodResult result){
        notSupported(result);
    }

    @Override
    public void hidePreview(IMethodResult result){
        notSupported(result);
    }

    @Override
    public void getEnableEditing(IMethodResult result){
        notSupported(result);
    }

    @Override
    public void setEnableEditing(boolean enableEditing, IMethodResult result){
        notSupported(result);
    }

    @Override
    public void getPreviewWidth(IMethodResult result){
        notSupported(result);
    }

    @Override
    public void setPreviewWidth(int previewWidth, IMethodResult result){
        notSupported(result);
    }

    @Override
    public void getPreviewHeight(IMethodResult result){
        notSupported(result);
    }

    @Override
    public void setPreviewHeight(int previewHeight, IMethodResult result){
        notSupported(result);
    }
    
    @Override
    public void getAimMode(IMethodResult result){
        notSupported(result);
    }

    @Override
    public void setAimMode(String aimMode, IMethodResult result){
        notSupported(result);
    }

    @Override
    public void getPreviewLeft(IMethodResult result) {
        notSupported(result);
    }

    @Override
    public void setPreviewLeft(int previewLeft, IMethodResult result) {
        notSupported(result);
    }

    @Override
    public void getPreviewTop(IMethodResult result) {
        notSupported(result);
    }

    @Override
    public void setPreviewTop(int previewTop, IMethodResult result) {
        notSupported(result);
    }

    

}
