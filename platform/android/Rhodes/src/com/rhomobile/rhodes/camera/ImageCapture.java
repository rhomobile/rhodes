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
import com.rhomobile.rhodes.Rhodes;

import android.app.Activity;
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
import android.view.View.OnClickListener;
import android.widget.ImageButton;

public class ImageCapture extends Activity implements SurfaceHolder.Callback, OnClickListener {
	
	private static final String TAG = "ImageCapture";
	
	private String callbackUrl;
	private Camera camera;
	private boolean isPreviewRunning = false;
	private SimpleDateFormat timeStampFormat = new SimpleDateFormat("yyyyMMddHHmmssSS");

	private SurfaceView surfaceView;
	private SurfaceHolder surfaceHolder;
	private ImageButton cameraButton;

	// private Uri target = Media.EXTERNAL_CONTENT_URI;

	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		Logger.D(TAG, "onCreate");
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(Rhodes.WINDOW_FLAGS, Rhodes.WINDOW_MASK);
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
	}

	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		super.onRestoreInstanceState(savedInstanceState);
	}

	PictureCallback mPictureCallbackRaw = new PictureCallback() {
		public void onPictureTaken(byte[] data, Camera c) {
			Logger.D(TAG, "PICTURE CALLBACK RAW");
			camera.startPreview();
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
			takePicture();
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
		camera = Camera.open();
	}

	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		try {
			Logger.D(TAG, "surfaceChanged");
			if (isPreviewRunning) {
				camera.stopPreview();
			}
			Camera.Parameters p = camera.getParameters();
			p.setPreviewSize(w, h);
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
		camera.stopPreview();
		isPreviewRunning = false;
		camera.release();
	}

	public void onClick(View v) {
		if (v.getId() == AndroidR.id.cameraButton) {
			takePicture();
		}
	}
	
	private void takePicture() {
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
			String dir = Rhodes.getBlobPath();
			
			OutputStream osCommon = getContentResolver().openOutputStream(uri);
			iccb = new ImageCaptureCallback(callbackUrl, osCommon, dir + "/" + filename + ".jpg");
		} catch (Exception ex) {
			Logger.E(TAG, ex.getMessage());
		}
		
		camera.takePicture(mShutterCallback, mPictureCallbackRaw, iccb);
	}
}
