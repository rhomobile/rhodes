package com.rho.camera;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.hardware.Camera;
import android.view.SurfaceHolder;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;

public class CameraEclair extends CameraObject implements ICameraObject {
    private static final String TAG = CameraEclair.class.getSimpleName();
    
    private List<Camera.Size> mSupportedPictureSizes;

    CameraEclair(String id) { 
        super(id); 

        getPropertiesMap().put("cameraType", "back");
        getPropertiesMap().put("compressionFormat", "jpg");
        getPropertiesMap().put("outputFormat", "image");

        openCamera();
        Camera.Parameters params = getCamera().getParameters();
        closeCamera();

        mSupportedPictureSizes = params.getSupportedPictureSizes();
        ISize maxSize = null;
        for(android.hardware.Camera.Size size: mSupportedPictureSizes) {
            Logger.D(TAG, "Possible picture size: " + size.width + "X" + size.height);
            ISize curSize = new CameraSize(size);
            if ((maxSize == null) || (curSize.D2() > maxSize.D2())) {
                maxSize = curSize;
            }
        }

        getPropertiesMap().put("maxWidth", String.valueOf(maxSize.getWidth()));
        getPropertiesMap().put("maxHeight", String.valueOf(maxSize.getHeight()));
    }

    @Override
    public void startPreview(SurfaceHolder surfaceHolder) {
        try {
            openCamera();
            Camera camera = getCamera();

            Camera.Parameters cameraParams = camera.getParameters();

            List<String> supportedFlashModes = cameraParams.getSupportedFlashModes();
            if (supportedFlashModes != null) {
                String flashMode = getFlashMode();
                if (supportedFlashModes.contains(flashMode)) {
                    cameraParams.setFlashMode(flashMode);
                } else {
                    Logger.W(TAG,"Flash mode is not supported by the camera: " + flashMode);
                }
            }

            List<String> supportedColorEffects = cameraParams.getSupportedColorEffects();
            if (supportedColorEffects != null) {
                String colorEffect = getColorMode();
                if (supportedColorEffects.contains(colorEffect)) {
                    cameraParams.setColorEffect(colorEffect);
                } else {
                    Logger.W(TAG, "Color effect is not supported by the camera: " + colorEffect);
                }
            }

            camera.setParameters(cameraParams);

            camera.setPreviewDisplay(surfaceHolder);
            camera.startPreview();
        } catch (Throwable e) {
            Logger.E(TAG, e);
            return;
        }
    }

    @Override
    public void doTakePicture(final Activity previewActivity, int rotation) {
        Logger.T(TAG, "doTakePicture: rotation: " + rotation);
        openCamera();
        ISize pictureSize = getDesiredSize();
        Camera.Parameters params = getCamera().getParameters();
        params.setRotation((90 + rotation) % 360);
        if (pictureSize != null) {
            params.setPictureSize(pictureSize.getWidth(), pictureSize.getHeight());
        }
        getCamera().setParameters(params);
        if (hasAutoFocus()) {
            getCamera().autoFocus(new Camera.AutoFocusCallback() {
                public void onAutoFocus(boolean success, Camera camera) {
                    openCamera();
                    getCamera().takePicture(null, null, new TakePictureCallback(previewActivity));
                    closeCamera();
                }
            });
            
        } else {
            getCamera().takePicture(null, null, new TakePictureCallback(previewActivity));
        }
        closeCamera();
    }

    protected ISize getDesiredSize() {
        String strDesiredWidth = getActualPropertyMap().get("desiredWidth");
        String strDesiredHeight = getActualPropertyMap().get("desiredHeight");
        
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
	 
	@Override
    public void setDisplayOrientation(int rotate) {
    	// TODO Auto-generated method stub
    }
}
