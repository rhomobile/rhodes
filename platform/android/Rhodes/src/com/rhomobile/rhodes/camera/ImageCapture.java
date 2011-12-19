/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes.camera;

import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Iterator;
import java.util.List;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.RhodesAppOptions;
import com.rhomobile.rhodes.osfunctionality.AndroidFunctionalityManager;
import com.rhomobile.rhodes.util.Utils;

import android.content.ContentValues;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.Size;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.MediaStore.Images.Media;
import android.view.KeyEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.ImageButton;
import android.hardware.SensorManager;
import android.view.OrientationEventListener;

public class ImageCapture extends BaseActivity implements SurfaceHolder.Callback, OnClickListener
 {
	
	private static final String TAG = "ImageCapture";
	
	private String callbackUrl;
	private android.hardware.Camera camera;
	private boolean isPreviewRunning = false;
	private SimpleDateFormat timeStampFormat = new SimpleDateFormat("yyyyMMddHHmmssSS");

	private SurfaceView surfaceView;
	private SurfaceHolder surfaceHolder;
	private ImageButton cameraButton;
    private OrientationEventListener myOrientationEventListener;
    private int m_rotation = 0;
    
    private CameraSettings mSettings = null;
    private boolean mIsFrontCamera = false;
    
    
	// private Uri target = Media.EXTERNAL_CONTENT_URI;

	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		Logger.D(TAG, "onCreate");
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().setFormat(PixelFormat.TRANSLUCENT);
		setContentView(AndroidR.layout.camera);
		
		Bundle extras = getIntent().getExtras();
		callbackUrl = extras.getString(com.rhomobile.rhodes.camera.Camera.INTENT_EXTRA_PREFIX + "callback");
		mSettings = (CameraSettings)extras.getSerializable(com.rhomobile.rhodes.camera.Camera.INTENT_EXTRA_PREFIX + "settings");
		
		surfaceView = (SurfaceView) findViewById(AndroidR.id.surface);
		surfaceHolder = surfaceView.getHolder();
		surfaceHolder.addCallback(this);
		surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		
		cameraButton = (ImageButton)findViewById(AndroidR.id.cameraButton);
		cameraButton.setOnClickListener(this);
		
        myOrientationEventListener = new OrientationEventListener(this, SensorManager.SENSOR_DELAY_NORMAL)
        {
            @Override
            public void onOrientationChanged(int orientation) 
            { 
            	Logger.D(TAG, "onOrientationChanged("+String.valueOf(orientation)+")");
                //Logger.D(TAG, "onOrientationChanged: " + orientation); 
                if (orientation == ORIENTATION_UNKNOWN) {
                	Logger.D(TAG, "orientation == UNKNOWN !");
                    return; 
                }
                 
                m_rotation = orientation;    
             }   
        };
    
        if (myOrientationEventListener.canDetectOrientation())
        {
           Logger.I(TAG, "myOrientationEventListener.enable()"); 
           myOrientationEventListener.enable();
        }
        else
        {
         	Logger.I(TAG, "orientation detect is not worked !!!");
           myOrientationEventListener = null;
        }		
	}

	@Override
	public void finish() 
	{
	    Logger.D(TAG, "finish");
	    if ( myOrientationEventListener != null )
	        myOrientationEventListener.disable();
	        
        myOrientationEventListener = null;	        
		super.finish();
	}

	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		super.onRestoreInstanceState(savedInstanceState);
	}

	PictureCallback mPictureCallbackRaw = new PictureCallback() {
		public void onPictureTaken(byte[] data, Camera c) {
			Logger.D(TAG, "PICTURE CALLBACK RAW");
		}
	};

	Camera.ShutterCallback mShutterCallback = new Camera.ShutterCallback() {
		public void onShutter() {
			Logger.D(TAG, "SHUTTER CALLBACK");
		}
	};

	public boolean onKeyDown(int keyCode, KeyEvent event) {
		switch (keyCode) {
		case KeyEvent.KEYCODE_DPAD_CENTER:
		case KeyEvent.KEYCODE_CAMERA:
			takePictureWithAutofocus();
			cameraButton.setVisibility(View.INVISIBLE);
			return true;
		case KeyEvent.KEYCODE_BACK:
			return super.onKeyDown(keyCode, event);
		default:
			return false;
		}
	}

	protected void onResume() {
		Logger.D(TAG, "onResume");
		super.onResume();
	}

	protected void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
	}

	protected void onStop() {
		Logger.D(TAG, "onStop");
		super.onStop();
	}

	public void surfaceCreated(SurfaceHolder holder) {
		Logger.D(TAG, "surfaceCreated");
		try {
			
			if (mSettings.getCameraType() == mSettings.CAMERA_TYPE_FRONT) {
				camera = com.rhomobile.rhodes.camera.Camera.getCameraService().getFrontCamera();
			}
			else {
				camera = com.rhomobile.rhodes.camera.Camera.getCameraService().getMainCamera();
			}
			
		}
		catch (Exception e) {
			Logger.E(TAG, e);
			finish();
		}
	}

	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		try {
			Logger.D(TAG, "surfaceChanged");
			if (camera == null) {
				Logger.E(TAG, "Camera was not opened");
				return;
			}
			
			if (isPreviewRunning) {
				camera.stopPreview();
			}
			Camera.Parameters p = camera.getParameters();
			
			int newW = (w >> 3) << 3;
			int newH = (h >> 3) << 3;

			CameraService.Size s = com.rhomobile.rhodes.camera.Camera.getCameraService().getClosestPreviewSize(camera, newW, newH);
			if (s == null) {
				p.setPreviewSize(newW, newH);
			}
			else {
				p.setPreviewSize(s.width, s.height);
			}
			if (mSettings != null) {
	            if ((mSettings.getWidth() > 0) && (mSettings.getHeight() > 0)) {
	                p.setPictureSize(mSettings.getWidth(), mSettings.getHeight());
	            }
	            if (mSettings.getColorModel() == mSettings.CAMERA_COLOR_MODEL_GRAYSCALE) {
	            	p.set("effect", Camera.Parameters.EFFECT_MONO);//p.setColorEffect(Camera.Parameters.EFFECT_MONO);
	            }
	            if (mSettings.getFlashMode() != null) {
	            	p.set("flash-mode", mSettings.getFlashMode());
	            	
	            }
			}
			camera.setParameters(p);
			camera.setPreviewDisplay(holder);
			camera.startPreview();
			isPreviewRunning = true;
		} catch (Exception e) {
			Logger.E(TAG, e.getMessage());
		}

	}

	public void surfaceDestroyed(SurfaceHolder holder) {
		Logger.D(TAG, "surfaceDestroyed");
		if (camera != null) {
			camera.stopPreview();
			isPreviewRunning = false;
			camera.release();
			camera = null;
		}
	}

	public void onClick(View v) {
		if (v.getId() == AndroidR.id.cameraButton) {
			takePictureWithAutofocus();
			cameraButton.setVisibility(View.INVISIBLE);
		}
	}

	private void takePictureWithAutofocus() {
		if (camera == null) {
			Logger.E(TAG, "Attempt of auto focus while camera was not opened");
			return;
		}
		
		//this only from API v.5 and higher
		//String focus_mode = camera.getParameters().getFocusMode();
		//if ((focus_mode != Camera.Parameters.FOCUS_MODE_FIXED) && (focus_mode != Camera.Parameters.FOCUS_MODE_INFINITY)) {
		if (com.rhomobile.rhodes.camera.Camera.getCameraService().isAutoFocusSupported(camera)) {
			camera.autoFocus(new Camera.AutoFocusCallback() {
				public void onAutoFocus(boolean success, Camera camera) {
					takePicture();
				}
			});
		}
		else {
			takePicture();
		}
		//}
	}

	
	private void takePicture() {
		
		com.rhomobile.rhodes.camera.Camera.logDebug(TAG, "takePicture()");
		
		if (camera == null) {
			Logger.E(TAG, "Attempt of take picture while camera was not opened");
			return;
		}
		
		ImageCaptureCallback iccb = null;
		try {
			String filename = "Image_" + timeStampFormat.format(new Date());
			ContentValues values = new ContentValues(5);
			values.put(Media.TITLE, filename);
			values.put(Media.DISPLAY_NAME, filename);
			values.put(Media.DATE_TAKEN, new Date().getTime());
			values.put(Media.MIME_TYPE, "image/jpeg");
			values.put(Media.DESCRIPTION, "Image capture by camera");

			Uri uri = null;
			try {
				uri = getContentResolver().insert(Media.EXTERNAL_CONTENT_URI, values);
			}
			catch (Exception ex) {
				uri = null;
				Logger.E(TAG, ex.getMessage());
			}
			try {
				if (uri == null) {
					uri = getContentResolver().insert(Media.INTERNAL_CONTENT_URI, values);
				}
			}
			catch (Exception ex) {
				uri = null;
				Logger.E(TAG, ex.getMessage());
			}
			// String filename = timeStampFormat.format(new Date());
			String dir = RhodesAppOptions.getBlobPath();

			
			OutputStream osCommon = null;
			try {
				if (uri != null) {
					osCommon = getContentResolver().openOutputStream(uri);
				}
			}
			catch (Exception ex) {
				uri = null;
				Logger.E(TAG, ex.getMessage());
			}

			Camera.Parameters parameters = camera.getParameters();
	        
	        int imgW = 0;
	        int imgH = 0;
	        
            //int nOrient = RhodesService.getInstance().getScreenOrientation();
            int nCamRotate = 90;
            if ( (m_rotation > 45 && m_rotation < 135) || (m_rotation > 225 && m_rotation < 315) ) {
                nCamRotate = 0;
            }
            if (mIsFrontCamera) {
                nCamRotate = 0;
                parameters.set("rotation", nCamRotate );//.setRotation(270);
            }
	        Logger.D(TAG, "Camera rotation: " + nCamRotate );
            parameters.set("rotation", nCamRotate );
            
            int deviceRotation = AndroidFunctionalityManager.getAndroidFunctionality().getDeviceRotation();
            if (deviceRotation >= 0) {
            	// platform from 2.2
            	parameters.set("rotation", deviceRotation );
    	        Logger.D(TAG, "Camera rotation resetup for platforms >= 2.2: " + deviceRotation );
            }
            
            if ((mSettings != null) && (mSettings.getWidth() > 0) && (mSettings.getHeight() > 0)) {

            	
    			int newW = mSettings.getWidth();
    			int newH = mSettings.getHeight();

        		com.rhomobile.rhodes.camera.Camera.logDebug(TAG, "    use custom size: ["+String.valueOf(newW)+"x"+String.valueOf(newH)+"]");
    			
    			CameraService.Size s = com.rhomobile.rhodes.camera.Camera.getCameraService().getClosestPictureSize(camera, newW, newH);
 
    			if (s != null) {
    				if ((s.width >= 0) && (s.height >= 0)) {
    					newW = s.width;
    					newH = s.height;
    					parameters.setPictureSize(newW, newH);
    				}
    			}
            	imgW = newW;
            	imgH = newH;
        		com.rhomobile.rhodes.camera.Camera.logDebug(TAG, "    final size: ["+String.valueOf(imgW)+"x"+String.valueOf(imgH)+"]");
            }
            else {
            	// detect camera resolution
    			CameraService.Size s = com.rhomobile.rhodes.camera.Camera.getCameraService().getClosestPictureSize(camera, 10000, 10000);
    			if (s != null) {
    				imgW = s.width;
    				imgH = s.height;
    			}
    			else {
    				imgW = 0;
    				imgH = 0;
    			}
            }
            if (mSettings != null) {
	            if (mSettings.getColorModel() == mSettings.CAMERA_COLOR_MODEL_GRAYSCALE) {
	            	parameters.set("effect", Camera.Parameters.EFFECT_MONO);//setColorEffect(Camera.Parameters.EFFECT_MONO);
	            }
	            if (mSettings.getFlashMode() != null) {
	            	parameters.set("flash-mode", mSettings.getFlashMode());
	            	
	            }
            }
			iccb = new ImageCaptureCallback(this, callbackUrl, osCommon, dir + "/" + filename + ".jpg", imgW, imgH, "jpg");
            camera.setParameters(parameters);

		} catch (Exception ex) {
			Logger.E(TAG, ex.getMessage());
		}
		
		camera.takePicture(mShutterCallback, mPictureCallbackRaw, iccb);
	}

}
