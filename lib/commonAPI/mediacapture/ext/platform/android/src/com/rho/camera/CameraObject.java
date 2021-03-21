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
    void setActualPropertyMap(Map<String, String> props) { mActualPropertyMap = props; }
    Map<String, String> getActualPropertyMap() { return mActualPropertyMap; }


    private Camera mCamera = null;
    private int mCameraUsers;   
    public static String userFilePath = null;
    private File storageDir = null;

    public static boolean CURRENT_SCREEN_AUTO_ROTATE_MODE;
    public static boolean CURRENT_FULL_SCREEN_MODE;

    int getCameraIndex() {
        return CameraSingleton.getCameraIndex(getId());
    }
    @Override
    public void setProperties(Map<String, String> propertyMap, IMethodResult result) {
        // TODO Auto-generated method stub
        Map<String, String> temp = getPropertiesMap();
        temp.putAll(propertyMap);
        result.set(true);
    }
    @Override
    public void getProperties(List<String> arrayofNames, IMethodResult result) {
        //super.getProperties(arrayofNames, result);
        Map<String, Object> props = new HashMap<String, Object>();
        for (String name: arrayofNames)
        {
            props.put(name, cameraPropGet(name));
        }
        result.set(props);

    }

    private String cameraPropGet(String name)
    {
        String propValue="";
        Map<String, String> temp=getPropertiesMap();
        if(temp.containsKey(name))
        {

            try{
                propValue = String.valueOf(temp.get(name));
            }
            catch(Exception ex)
            {

            }
        }
        return propValue;
    }

    @Override
	public void getAllProperties(IMethodResult result) {
		// TODO Auto-generated method stub
    	Map<String, Object> props = new HashMap<String, Object>();
    	for (String key: getPropertiesMap().keySet()) {
    		 props.put(key, cameraPropGet(key));
        }
    	result.set(props);
	}
	
   
    /*private void saveToDeviceGallery(Bitmap bitmap){
        ContentResolver contentResolver = ContextFactory.getContext().getContentResolver();
        Logger.T(TAG, "Image size: " + bitmap.getWidth() + "X" + bitmap.getHeight());
        propertyMap.put("DeviceGallery_Key", "DeviceGallery_Value");
        String strUri = null;
        if (!propertyMap.containsKey("fileName")){
            strUri = MediaStore.Images.Media.insertImage(contentResolver, bitmap, 
                "IMG_"+ dateFormat.format(new Date(System.currentTimeMillis())), "Camera");
        }else{
            strUri = MediaStore.Images.Media.insertImage(contentResolver, bitmap, 
                new File(propertyMap.get("fileName")).getName(), "Camera");
        }
        if (strUri != null) {
            resultUri = Uri.parse(strUri);
        } else {
            throw new RuntimeException("Failed to save camera image to Gallery");
        }
    }*/

    private void playMusic(String musicPath) {
        MediaPlayer mp = new MediaPlayer();
        try {
            mp.setDataSource(RhoFileApi.openFd(musicPath));
            mp.prepare();
            mp.start();
            Thread.sleep(3000);
            mp.stop();
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        if(mp != null){
            mp.release();
            mp = null;
        }
    }
    

    protected Camera getCamera() { return mCamera; }
    protected void setCamera(Camera camera) { mCamera = camera; }

    synchronized protected void openCamera() {
        if (mCameraUsers == 0) {
            setCamera(android.hardware.Camera.open());
        }
        mCameraUsers++;
    }
    synchronized protected void closeCamera() {
        mCameraUsers--;
        if (mCameraUsers == 0) {
            getCamera().release();
        }
    }

    CameraObject(String id) {
        super(id);

        getPropertiesMap().put("ChoosePicture_Key", "");
        getPropertiesMap().put("cameraType", "back");
        getPropertiesMap().put("compressionFormat", "jpg");
        getPropertiesMap().put("outputFormat", "image");
        getPropertiesMap().put("colorModel", "rgb");
        getPropertiesMap().put("useRealBitmapResize", "true");
        getPropertiesMap().put("useRotationBitmapByEXIF", "true");
        getPropertiesMap().put("saveToDeviceGallery", "false");
        if (hasPermission()) {
            openCamera();
            Camera.Parameters params = getCamera().getParameters();
            closeCamera();

            getPropertiesMap().put("maxWidth", String.valueOf(params.getPictureSize().width));
            getPropertiesMap().put("maxHeight", String.valueOf(params.getPictureSize().height));
        }
        else {
            Logger.E(TAG, "Application has no permission to Camera access !!!");
            getPropertiesMap().put("maxWidth", String.valueOf(0));
            getPropertiesMap().put("maxHeight", String.valueOf(0));
        }
        getPropertiesMap().put("desiredWidth", "0");
        getPropertiesMap().put("desiredHeight", "0");

        storageDir = new File(Environment.getExternalStorageDirectory(), "RhoImages");
        createRhoCacheFolder();

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
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date(System.currentTimeMillis()));
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
               fileName = "IMG_"+ dateFormat.format(new Date(System.currentTimeMillis()));
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
                                                          RhodesActivity.getContext().getPackageName() + ".fileprovider",
                                                          photoFile);
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
        //TODO
    }

    public void finalize() {
        if (getCamera() != null) {
            getCamera().release();
        }
    }


    public boolean hasPermission() {
        //Logger.E(TAG, "Application has permission to Camera access DAFAULT !!!");
        return true;
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
