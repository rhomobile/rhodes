/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
package com.rhomobile.rhodes.camera;

import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.RhodesAppOptions;

import android.content.ContentValues;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.net.Uri;
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
	private Camera camera;
	private boolean isPreviewRunning = false;
	private SimpleDateFormat timeStampFormat = new SimpleDateFormat("yyyyMMddHHmmssSS");

	private SurfaceView surfaceView;
	private SurfaceHolder surfaceHolder;
	private ImageButton cameraButton;
    private OrientationEventListener myOrientationEventListener;
    private int m_rotation = 0;
    
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
                //Logger.D(TAG, "onOrientationChanged: " + orientation); 
                if (orientation == ORIENTATION_UNKNOWN) 
                    return; 
                 
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
           Logger.I(TAG, "cannot detect!"); 
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
			camera = Camera.open();
		}
		catch (Exception e) {
			Logger.E(TAG, e.getMessage());
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
			p.setPreviewSize(newW, newH);
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
		camera.autoFocus(new Camera.AutoFocusCallback() {
			@Override
			public void onAutoFocus(boolean success, Camera camera) {
				takePicture();
			}
		});
		//}
	}

	
	private void takePicture() {
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

			Uri uri = getContentResolver().insert(Media.EXTERNAL_CONTENT_URI, values);
			// String filename = timeStampFormat.format(new Date());
			String dir = RhodesAppOptions.getBlobPath();
			
			OutputStream osCommon = getContentResolver().openOutputStream(uri);
			iccb = new ImageCaptureCallback(this, callbackUrl, osCommon, dir + "/" + filename + ".jpg");
			
	        Camera.Parameters parameters = camera.getParameters();
            //int nOrient = RhodesService.getInstance().getScreenOrientation();
            int nCamRotate = 90;
            if ( (m_rotation > 45 && m_rotation < 135) || (m_rotation > 225 && m_rotation < 315) )
                nCamRotate = 0;
                
	        Logger.D(TAG, "Camera rotation: " + nCamRotate );
            parameters.set("rotation", nCamRotate );
            camera.setParameters(parameters);

		} catch (Exception ex) {
			Logger.E(TAG, ex.getMessage());
		}
		
		camera.takePicture(mShutterCallback, mPictureCallbackRaw, iccb);
	}

}
