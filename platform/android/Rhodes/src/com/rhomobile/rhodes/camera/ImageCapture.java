package com.rhomobile.rhodes.camera;

import java.text.SimpleDateFormat;
import java.util.Date;

import com.rhomobile.rhodes.AndroidR;

import android.app.Activity;
import android.content.ContentValues;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore.Images.Media;
import android.util.Log;
import android.view.KeyEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class ImageCapture extends Activity implements SurfaceHolder.Callback {
	private Camera camera;
	private boolean isPreviewRunning = false;
	private SimpleDateFormat timeStampFormat = new SimpleDateFormat(
			"yyyyMMddHHmmssSS");

	private SurfaceView surfaceView;
	private SurfaceHolder surfaceHolder;

	// private Uri target = Media.EXTERNAL_CONTENT_URI;

	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		Log.e(getClass().getSimpleName(), "onCreate");
		getWindow().setFormat(PixelFormat.TRANSLUCENT);
		setContentView(AndroidR.layout.camera);
		surfaceView = (SurfaceView) findViewById(AndroidR.id.surface);
		surfaceHolder = surfaceView.getHolder();
		surfaceHolder.addCallback(this);
		surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
	}

	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		super.onRestoreInstanceState(savedInstanceState);
	}

	PictureCallback mPictureCallbackRaw = new PictureCallback() {
		public void onPictureTaken(byte[] data, Camera c) {
			Log.e(getClass().getSimpleName(), "PICTURE CALLBACK RAW: " + data);
			camera.startPreview();
		}
	};

	Camera.PictureCallback mPictureCallbackJpeg = new Camera.PictureCallback() {
		public void onPictureTaken(byte[] data, Camera c) {
			Log.e(getClass().getSimpleName(),
					"PICTURE CALLBACK JPEG: data.length = " + data);
		}
	};

	Camera.ShutterCallback mShutterCallback = new Camera.ShutterCallback() {
		public void onShutter() {
			Log.e(getClass().getSimpleName(), "SHUTTER CALLBACK");
		}
	};

	public boolean onKeyDown(int keyCode, KeyEvent event) {
		ImageCaptureCallback iccb = null;
		switch (keyCode) {
		case KeyEvent.KEYCODE_DPAD_CENTER:
		case KeyEvent.KEYCODE_CAMERA:
			try {
				String filename = "Image_" + timeStampFormat.format(new Date());
				ContentValues values = new ContentValues(5);
				values.put(Media.TITLE, filename);
				values.put(Media.DISPLAY_NAME, filename);
				values.put(Media.DATE_TAKEN, new Date().getTime());
				values.put(Media.MIME_TYPE, "image/jpeg");
				values.put(Media.DESCRIPTION, "Image capture by camera");

				Uri uri = getContentResolver().insert(
						Media.EXTERNAL_CONTENT_URI, values);
				// String filename = timeStampFormat.format(new Date());
				iccb = new ImageCaptureCallback(getContentResolver().openOutputStream(uri),
						com.rhomobile.rhodes.camera.Camera.BASE_CAMERA_DIR + "/" +
						filename + ".jpg");
			} catch (Exception ex) {
				ex.printStackTrace();
				Log.e(getClass().getSimpleName(), ex.getMessage(), ex);
			}
			
			camera.takePicture(mShutterCallback, mPictureCallbackRaw, iccb);
			return true;
		case KeyEvent.KEYCODE_BACK:
			return super.onKeyDown(keyCode, event);
		default:
			return false;
		}
	}

	protected void onResume() {
		Log.e(getClass().getSimpleName(), "onResume");
		super.onResume();
	}

	protected void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
	}

	protected void onStop() {
		Log.e(getClass().getSimpleName(), "onStop");
		super.onStop();
	}

	public void surfaceCreated(SurfaceHolder holder) {
		Log.e(getClass().getSimpleName(), "surfaceCreated");
		camera = Camera.open();
	}

	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		try {
			Log.e(getClass().getSimpleName(), "surfaceChanged");
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
			e.printStackTrace();
		}

	}

	public void surfaceDestroyed(SurfaceHolder holder) {
		Log.e(getClass().getSimpleName(), "surfaceDestroyed");
		camera.stopPreview();
		isPreviewRunning = false;
		camera.release();
	}
}
