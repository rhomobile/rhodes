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
import android.media.MediaPlayer;
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

public class CameraObject extends CameraBase implements ICamera{
    private static final String TAG = CameraObject.class.getSimpleName();
    private Map<String, String> mActualPropertyMap;
    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyyMMdd_hhmmss");

    void setActualPropertyMap(Map<String, String> props) { 
        mActualPropertyMap = props; 
    }

    Map<String, String> getActualPropertyMap() { 
        return mActualPropertyMap; 
    }

    private Camera mCamera = null;
    private int mCameraUsers;   
    public static String userFilePath = null;
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
        getPropertiesMap().put("compressionFormat", "jpg");
        getPropertiesMap().put("outputFormat", "image");
        getPropertiesMap().put("colorModel", "rgb");
        getPropertiesMap().put("useRealBitmapResize", "true");
        getPropertiesMap().put("useRotationBitmapByEXIF", "true");
        getPropertiesMap().put("saveToDeviceGallery", "false");

        storageDir = new File(Environment.getExternalStorageDirectory(), "RhoImages");
        createRhoCacheFolder();

        android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
        android.hardware.Camera.getCameraInfo(getCameraIndex(), info);
        switch (info.facing) {
        case android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK:
            getPropertiesMap().put("cameraType", "back");
            break;
        case android.hardware.Camera.CameraInfo.CAMERA_FACING_FRONT:
            getPropertiesMap().put("cameraType", "front");
            break;
        default:
            getPropertiesMap().put("cameraType", "unknown");
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

            getPropertiesMap().put("maxWidth", String.valueOf(maxSize.getWidth()));
            getPropertiesMap().put("maxHeight", String.valueOf(maxSize.getHeight()));
        }
        else {
            Logger.E(TAG, "Application has no permission to Camera access !!!");
            getPropertiesMap().put("maxWidth", String.valueOf(0));
            getPropertiesMap().put("maxHeight", String.valueOf(0));
        }

        getPropertiesMap().put("desiredWidth", "0");
        getPropertiesMap().put("desiredHeight", "0");
    }

    public void createRhoCacheFolder(){
        storageDir.mkdirs();
    }

    
    @Override
    public void setCompressionFormat(String compressionFormat, IMethodResult result) {
        if (!compressionFormat.equalsIgnoreCase("jpg")) {
            Logger.E(TAG, "Android does not support the compression format: " + compressionFormat);
            result.setError("Android does not support the compression format: " + compressionFormat);
        }
    }

    private File createImageFile() throws IOException {
        String timeStamp = dateFormat.format(new Date(System.currentTimeMillis()));
        String imageFileName = "JPEG_" + timeStamp + "_";
        File storageDir = RhodesActivity.getContext().getExternalFilesDir(Environment.DIRECTORY_PICTURES);
        File image = File.createTempFile(imageFileName, ".jpg", storageDir);
        return image;
    }


    @Override
    public void takePicture(Map<String, String> propertyMap, IMethodResult result) {
        if (!hasPermission()) {
            Logger.E(TAG, "Application has no permission to Camera access !!!");
            HashMap<String, Object> inResultMap = new HashMap<String,Object>();
            inResultMap.put("message", "No CAMERA permission !");
            inResultMap.put("status", "error");
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
            setActualPropertyMap(actualPropertyMap);

            String outputFormat = actualPropertyMap.get("outputFormat");

            String fileDir = storageDir.getAbsolutePath();
            String fileName = actualPropertyMap.get("fileName");
            if (fileName == null || fileName.isEmpty()){
               fileName = "IMG_" + dateFormat.format(new Date(System.currentTimeMillis()));
            }
            String filePath = fileDir + "/" + fileName + ".jpg";

            Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
            if (outputFormat.equalsIgnoreCase("image")) {
                ContentValues values = new ContentValues();
                if (!Boolean.parseBoolean(propertyMap.get("saveToDeviceGallery"))) {
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

                propertyMap.put("dataURI", "");
                
                intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            } else if (outputFormat.equalsIgnoreCase("dataUri")) {}


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
            resSize.put("width", Integer.valueOf(size.width));
            resSize.put("height", Integer.valueOf(size.height));
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
        String strDesiredWidth = getActualPropertyMap().get("desiredWidth");
        String strDesiredHeight = getActualPropertyMap().get("desiredHeight");

        if (Boolean.parseBoolean(getActualPropertyMap().get("useRealBitmapResize"))) {
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
        String flashParam = getActualPropertyMap().get("flashMode");
        String flashMode = Camera.Parameters.FLASH_MODE_AUTO;
        if (flashParam != null) {
            if (flashParam.equals("on")) {
                flashMode = Camera.Parameters.FLASH_MODE_ON;
            } else
                if (flashParam.equals("off")) {
                    flashMode = Camera.Parameters.FLASH_MODE_OFF;
                } else
                    if (flashParam.equals("redEye")) {
                        flashMode = Camera.Parameters.FLASH_MODE_RED_EYE;
                    } else
                        if (flashParam.equals("torch")) {
                            flashMode = Camera.Parameters.FLASH_MODE_TORCH;
                        }
        }
        Logger.T(TAG, "Flash mode: " + flashMode);
        return flashMode;
    }

    protected String getColorMode() {
        String colorParam = getActualPropertyMap().get("colorModel");
        String colorMode = Camera.Parameters.EFFECT_NONE;
        if (colorParam != null) {
            if (colorParam.equals("grayscale")) {
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



    @Override
    public void getCameraType(IMethodResult result){
        //TODO!!!
    }

    @Override
    public void getMaxWidth(IMethodResult result){
        //TODO!!!
    }

    @Override
    public void getMaxHeight(IMethodResult result){
        //TODO!!!
    }

    @Override
    public void getDesiredWidth(IMethodResult result){
        //TODO
    }
    @Override
    public void setDesiredWidth(int desiredWidth, IMethodResult result){
        //TODO
    }
    @Override
    public void getDesiredHeight(IMethodResult result){
        //TODO
    }
    @Override
    public void setDesiredHeight(int desiredHeight, IMethodResult result){
        //TODO
    }

    @Override
    public void getFileName(IMethodResult result){
        //TODO
    }
    @Override
    public void setFileName(String fileName, IMethodResult result){
        //TODO
    }

    @Override
    public void getCompressionFormat(IMethodResult result){
        //TODO
    }

    @Override
    public void getOutputFormat(IMethodResult result){
        //TODO
    }

    @Override
    public void setOutputFormat(String outputFormat, IMethodResult result){
        //TODO
    }

    
    @Override
    public void getColorModel(IMethodResult result){
        //TODO
    }
    
    @Override
    public void setColorModel(String colorModel, IMethodResult result){
        //TODO
    }

    @Override
    public void getFlashMode(IMethodResult result){
        //TODO
    }

    @Override
    public void setFlashMode(String flashMode, IMethodResult result){
        //TODO
    }


    @Override
    public void getSaveToDeviceGallery(IMethodResult result){
        //TODO
    }

    @Override
    public void setSaveToDeviceGallery(boolean saveToDeviceGallery, IMethodResult result){
        //TODO
    }

    @Override
    public void getCaptureSound(IMethodResult result){
        //TODO
    }

    @Override
    public void setCaptureSound(String captureSound, IMethodResult result){
        //TODO
    }

    @Override
    public void getUseRealBitmapResize(IMethodResult result){
        //TODO
    }

    @Override
    public void setUseRealBitmapResize(boolean useRealBitmapResize, IMethodResult result){
        //TODO
    }

    @Override
    public void getUseRotationBitmapByEXIF(IMethodResult result){
        //TODO
    }

    @Override
    public void setUseRotationBitmapByEXIF(boolean useRotationBitmapByEXIF, IMethodResult result){
        //TODO
    }



    /* Here comes the stubs */

    @Override
    public void capture(IMethodResult result){
        //NOT SUPPORTED
    }

    @Override
    public void showPreview(Map<String, String> propertyMap, IMethodResult result){
        //NOT SUPPORTED
    }

    @Override
    public void hidePreview(IMethodResult result){
        //NOT SUPPORTED
    }

    @Override
    public void getEnableEditing(IMethodResult result){
        //NOT SUPPORTED
    }

    @Override
    public void setEnableEditing(boolean enableEditing, IMethodResult result){
        //NOT SUPPORTED
    }

    @Override
    public void getPreviewWidth(IMethodResult result){
        //NOT SUPPORTED
    }

    @Override
    public void setPreviewWidth(int previewWidth, IMethodResult result){
        //NOT SUPPORTED
    }

    @Override
    public void getPreviewHeight(IMethodResult result){
        //NOT SUPPORTED
    }

    @Override
    public void setPreviewHeight(int previewHeight, IMethodResult result){
        //NOT SUPPORTED
    }

    
    @Override
    public void getAimMode(IMethodResult result){
        //NOT SUPPORTED
    }

    @Override
    public void setAimMode(String aimMode, IMethodResult result){
        //NOT SUPPORTED
    }

    @Override
    public void getPreviewLeft(IMethodResult result) {
        //NOT SUPPORTED
    }

    @Override
    public void setPreviewLeft(int previewLeft, IMethodResult result) {
        //NOT SUPPORTED
    }

    @Override
    public void getPreviewTop(IMethodResult result) {
        //NOT SUPPORTED
    }

    @Override
    public void setPreviewTop(int previewTop, IMethodResult result) {
        //NOT SUPPORTED
    }

    

}
