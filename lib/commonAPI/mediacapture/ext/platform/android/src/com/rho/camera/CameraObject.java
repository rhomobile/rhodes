package com.rho.camera;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Camera;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.view.SurfaceHolder;

import com.rhomobile.rhodes.Base64;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.util.ContextFactory;

public class CameraObject extends CameraBase implements ICameraObject {
    private static final String TAG = CameraObject.class.getSimpleName();
    public static boolean deprecated_take_pic;
    private Map<String, String> mActualPropertyMap;
    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyyMMdd_hhmmss");
    void setActualPropertyMap(Map<String, String> props) { mActualPropertyMap = props; }
    Map<String, String> getActualPropertyMap() { return mActualPropertyMap; }
    
    
    private Camera mCamera;
    private int mCameraUsers;
    private Uri fileUri;
    String mCurrentPhotoPath = null;
    private ContentValues values = null;
    
    int getCameraIndex() {
        return CameraSingletonObject.getCameraIndex(getId());
    }
    
    static class CameraSize implements ICameraObject.ISize {
        private Camera.Size mSize;
        CameraSize(Camera.Size size) { mSize = size; }
        @Override public int getWidth() { return mSize.width; }
        @Override public int getHeight() { return mSize.height; }
        @Override public int D2() { return mSize.width * mSize.width + mSize.height * mSize.height; }
        @Override public String toString() { return "" + mSize.width + "X" + mSize.height; }
    }
    
    static class RawSize implements ICameraObject.ISize {
        private int width;
        private int height;
        RawSize(int width, int height) { this.width = width; this.height = height; }
        @Override public int getWidth() { return this.width; }
        @Override public int getHeight() { return this.height; }
        @Override public int D2() { return width * width + height * height; }
        @Override public String toString() { return "" + width + "X" + height; }
    }

    protected class TakePictureCallback implements Camera.PictureCallback {
        private Activity mPreviewActivity;
        private CameraActivity mcameraActivity;
        TakePictureCallback(Activity previewActivity) {
            mPreviewActivity = previewActivity;
            mcameraActivity = (CameraActivity) previewActivity;
        }
        @Override
        public void onPictureTaken(byte[] data, Camera camera) {        	
            Intent intent = new Intent();
            OutputStream stream = null;
            Bitmap bitmap = null;
            try {
            	
                Map<String, String> propertyMap = getActualPropertyMap();
                if (propertyMap == null) {
                    throw new RuntimeException("Camera property map is undefined");
                }
             
                String outputFormat = propertyMap.get("outputFormat");
                if(propertyMap.get("deprecated") == null || propertyMap.get("deprecated").equalsIgnoreCase("false")){   
    		  propertyMap.put("deprecated", "false");
    		  deprecated_take_pic = false;    	
    	        }
    	        else
    		   deprecated_take_pic = true;
                if(propertyMap.containsKey("captureSound")){
                	mcameraActivity.playMusic(propertyMap.get("captureSound"));
                }
                
                String filePath = null;
                SimpleDateFormat dateFormat = new SimpleDateFormat("yyyyMMdd_hhmmss");                
                if(!propertyMap.containsKey("fileName")){
                   filePath = "/sdcard/DCIM/Camera/IMG_"+ dateFormat.format(new Date(System.currentTimeMillis()));
         	}
         	else{
                   filePath = propertyMap.get("fileName");
         	}
                Uri resultUri = null;
                BitmapFactory.Options options=new BitmapFactory.Options();
		options.inPurgeable = true;
                bitmap = BitmapFactory.decodeByteArray(data, 0, data.length, options);
                 if (outputFormat.equalsIgnoreCase("dataUri")) {
                    Logger.T(TAG, "outputFormat: " + outputFormat);                    
                    StringBuilder dataBuilder = new StringBuilder();
                    dataBuilder.append("data:image/jpeg;base64,");
                    dataBuilder.append(Base64.encodeToString(data, false));
                    propertyMap.put("captureUri", dataBuilder.toString());
                    Logger.T(TAG, dataBuilder.toString());
                    intent.putExtra("IMAGE_WIDTH", bitmap.getWidth());                    
                    intent.putExtra("IMAGE_HEIGHT", bitmap.getHeight());                  
                    mPreviewActivity.setResult(Activity.RESULT_OK, intent);                    
                } else
                if (outputFormat.equalsIgnoreCase("image")) {
                    filePath = getTemporaryPath(filePath)+ ".jpg";
                    Logger.T(TAG, "outputFormat: " + outputFormat + ", path: " + filePath);                    
                    if (Boolean.parseBoolean(propertyMap.get("saveToDeviceGallery"))) 
                    {                        
                        ContentResolver contentResolver = ContextFactory.getContext().getContentResolver();
                        Logger.T(TAG, "Image size: " + bitmap.getWidth() + "X" + bitmap.getHeight());                        
                        String strUri = null;
			if (!propertyMap.containsKey("fileName")) 
				strUri = MediaStore.Images.Media.insertImage(contentResolver, bitmap, "IMG_"+ dateFormat.format(new Date(System.currentTimeMillis())), "Camera");
			else
				strUri = MediaStore.Images.Media.insertImage(contentResolver, bitmap, new File(propertyMap.get("fileName")).getName(), "Camera");
			if (strUri != null) {
                            resultUri = Uri.parse(strUri);                           
                        } else {
                            throw new RuntimeException("Failed to save camera image to Gallery");
                        }                  
                    } 
                    else
                    {                    	
                        stream = new FileOutputStream(filePath);                        
                        resultUri = Uri.fromFile(new File(filePath));                        
                        stream.write(data);                       
                        stream.flush();                        
                        stream.close();                       
                    }
                       
                    intent.putExtra(MediaStore.EXTRA_OUTPUT, resultUri);
                    intent.putExtra("IMAGE_WIDTH", bitmap.getWidth());                    
                    intent.putExtra("IMAGE_HEIGHT", bitmap.getHeight());                   
                    mPreviewActivity.setResult(Activity.RESULT_OK, intent);        
                }              
             
            } catch (Throwable e) {
                Logger.E(TAG, e);                
                if (stream != null) {
                    try {
                        stream.close();
                    } catch (Throwable e1) {
                        //Do nothing
                    }
                }
                intent.putExtra("error", e.getMessage());              
                mPreviewActivity.setResult(Activity.RESULT_CANCELED, intent);                
            }
        if(bitmap != null){
            bitmap.recycle();
            bitmap = null;
	    System.gc();
	}
            mPreviewActivity.finish();
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

        getPropertiesMap().put("cameraType", "back");
        getPropertiesMap().put("compressionFormat", "jpg");
        getPropertiesMap().put("outputFormat", "image");
        getPropertiesMap().put("colorModel", "rgb");
	getPropertiesMap().put("useSystemViewfinder", "false");
	getPropertiesMap().put("saveToDeviceGallery", "false");
        openCamera();
        Camera.Parameters params = getCamera().getParameters();
        closeCamera();
        
        getPropertiesMap().put("maxWidth", String.valueOf(params.getPictureSize().width));
        getPropertiesMap().put("maxHeight", String.valueOf(params.getPictureSize().height));
    }

    @Override
    public void startPreview(SurfaceHolder surfaceHolder) {
        try {
            openCamera();
            getCamera().setPreviewDisplay(surfaceHolder);
            getCamera().startPreview();
        } catch (Throwable e) {
            Logger.E(TAG, e);
            return;
        }
    }

    @Override
    public void stopPreview() {
        // Surface will be destroyed when we return, so stop the preview.
        if (getCamera() != null) {
            try {
                getCamera().stopPreview();
                getCamera().setPreviewDisplay(null);
                closeCamera();
            } catch (Throwable e) {
                Logger.E(TAG, e);
                return;
            }
        } 
    }

    @SuppressWarnings("deprecation")
    @SuppressLint("NewApi")
	@Override
    public ISize setPreviewSize(int width, int height) {
    	Camera camera = getCamera();
        Camera.Parameters parameters = camera.getParameters();       
        List<android.hardware.Camera.Size> sizes = camera.getParameters().getSupportedPictureSizes();        
        android.hardware.Camera.Size maxSize=sizes.get(0);       
        if(getActualPropertyMap().containsKey("desiredWidth") || getActualPropertyMap().containsKey("desiredHeight")){
        	int desired_width = Integer.parseInt(getActualPropertyMap().get("desiredWidth"));
            int desired_height = Integer.parseInt(getActualPropertyMap().get("desiredHeight"));               
        	if(desired_width > 0 && desired_width <= maxSize.width && desired_height > 0 && desired_height <= maxSize.height){        	
        	 Camera.Size previewSize = getOptimalPreviewSize(parameters.getSupportedPictureSizes(), desired_width, desired_height);
        	 Logger.T(TAG, "Selected size: " + previewSize.width + "x" + previewSize.height);             
                 parameters.setPreviewSize(previewSize.width, previewSize.height);
        	}
        	else if(desired_width > maxSize.width || desired_height > maxSize.height){        		
        		parameters.setPreviewSize(maxSize.width , maxSize.height);       
        	}
        	else{ 
        		parameters.setPreviewSize(320, 240);
        	}
        }
        else{
        Camera.Size previewSize = getOptimalPreviewSize(parameters.getSupportedPictureSizes(), width, height);
        Logger.T(TAG, "Selected size: " + previewSize.width + "x" + previewSize.height);        
        parameters.setPreviewSize(previewSize.width, previewSize.height);
       }
        camera.stopPreview();
        try{
           camera.setParameters(parameters);
        }
        catch(RuntimeException e){        	
        	e.printStackTrace();
        }
        camera.startPreview();
        
        return new CameraSize(camera.getParameters().getPreviewSize());
    }

    private Camera.Size getOptimalPreviewSize(List<Camera.Size> sizes, int w, int h) {
        final double ASPECT_TOLERANCE = 0.2;
        final double SCALE_TOLERANCE = 0.2;
        double targetRatio = (double) w / h;
        if (sizes == null) return null;

        Camera.Size optimalSize = null;
        double minDiff = Double.MAX_VALUE;

        int targetHeight = h;

        // Try to find an size match aspect ratio and size
        for (Camera.Size size : sizes) {
            Logger.T(TAG, "Size: " + size.width + "x" + size.height);

            double scale = (double) size.width / w;
            double ratio = (double) size.width / size.height;
            
            Logger.T(TAG, "Target ratio: " + targetRatio + ", ratio: " + ratio + ", scale: " + scale);
            
            if (size.width > w || size.height > h) continue;
            if (Math.abs(1 - scale) > SCALE_TOLERANCE) continue;
            if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE) continue;

            Logger.T(TAG, "Try size: " + size.width + "x" + size.height);

            if (Math.abs(size.height - targetHeight) < minDiff) {
                optimalSize = size;
                minDiff = Math.abs(size.height - targetHeight);
            }
        }

        // Cannot find the one match the aspect ratio, ignore the requirement
        if (optimalSize == null) {
            Logger.T(TAG, "There is no match for preview size!");
            minDiff = Double.MAX_VALUE;
            for (Camera.Size size : sizes) {
                if (size.width > w || size.height > h) continue;

                Logger.T(TAG, "Try size: " + size.width + "x" + size.height);

                if (Math.abs(size.height - targetHeight) < minDiff) {
                    optimalSize = size;
                    minDiff = Math.abs(size.height - targetHeight);
                }
            }
        }
        return optimalSize;
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
            Map<String, String> actualPropertyMap = new HashMap<String, String>();
            actualPropertyMap.putAll(getPropertiesMap());
            actualPropertyMap.putAll(propertyMap);
            setActualPropertyMap(actualPropertyMap);
            
            String outputFormat = actualPropertyMap.get("outputFormat");
            String filePath = null;
            if(!actualPropertyMap.containsKey("fileName")){
            	filePath = "/sdcard/DCIM/Camera/IMG_"+ dateFormat.format(new Date(System.currentTimeMillis())) + ".jpg";
     	   }
     	   else{
     		filePath = actualPropertyMap.get("fileName");
     	   }
            if (outputFormat.equalsIgnoreCase("image")) {
            	filePath = actualPropertyMap.get("fileName") + ".jpg";
                Logger.T(TAG, "outputFormat: " + outputFormat + ", path: " + filePath);               
            } else
            if (outputFormat.equalsIgnoreCase("dataUri")) {
                Logger.T(TAG, "outputFormat: " + outputFormat);                
            } else {
                throw new RuntimeException("Unknown 'outputFormat' value: " + outputFormat);
            }
            
            Intent intent = null;
            if (Boolean.parseBoolean(actualPropertyMap.get("useSystemViewfinder"))) {
                if (outputFormat.equalsIgnoreCase("image")) {
                    values = new ContentValues();
            		fileUri = RhodesActivity.getContext().getContentResolver().insert(
            				MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);
                	intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE); 
                	actualPropertyMap.put("captureUri", fileUri.toString());
                	// intent is null with MediaStore.EXTRA_OUTPUT so adding fileuri to map and get it with same key
                	// if instead of MediaStore.EXTRA_OUTPUT any other key is used then the bitmap is null though the file is getting created
            		intent.putExtra(MediaStore.EXTRA_OUTPUT, fileUri);
                } else
                if (outputFormat.equalsIgnoreCase("dataUri")) {
                    
                }
            } else {
                intent = new Intent(ContextFactory.getUiContext(), CameraActivity.class);
                intent.putExtra(CameraExtension.INTENT_EXTRA_PREFIX + "CAMERA_ID", getId());
            }
            ((CameraFactory)CameraFactorySingleton.getInstance()).getRhoListener().setMethodResult(result);
            ((CameraFactory)CameraFactorySingleton.getInstance()).getRhoListener().setActualPropertyMap(actualPropertyMap);
            
            RhodesActivity.safeGetInstance().startActivityForResult(intent, RhoExtManager.getInstance().getActivityResultNextRequestCode(CameraRhoListener.getInstance()));
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
    }

    @SuppressLint("NewApi")
	@Override
    public void doTakePicture(Activity previewActivity, int rotation) {
        Logger.T(TAG, "doTakePicture: rotation: " + rotation);
        openCamera();
        Camera.Parameters params = getCamera().getParameters();
        params.setRotation((90 + rotation) % 360);
        getCamera().setParameters(params);
        getCamera().takePicture(null, null, new TakePictureCallback(previewActivity));
        closeCamera();
    }
    
    public void finalize() {
        if (getCamera() != null) {
            getCamera().release();
        }
    }
	@Override
	public void showPreview(Map<String, String> propertyMap,
			IMethodResult result) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void hidePreview(IMethodResult result) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void capture(IMethodResult result) {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public void setDisplayOrientation(int rotate) {
	    Camera camera = getCamera();
	    camera.setDisplayOrientation(rotate);
	}
}
