/*
 ============================================================================
 Author	    : Dmitry Soldatenkov
 Version	: 1.5
 Copyright  : Copyright (C) 2010 Rhomobile. All rights reserved.

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
package com.rhomobile.rhodes.signature;

import java.io.FileOutputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.Rhodes;

import android.app.Activity;
import android.content.ContentValues;
import android.graphics.PixelFormat;
import android.graphics.Bitmap;
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
import android.view.View.OnTouchListener;
import android.view.MotionEvent;


public class ImageCapture extends Activity implements OnClickListener
 {
	
	private static final String TAG = "ImageCapture";
	
	private String callbackUrl;
	private Camera camera;
	private boolean isPreviewRunning = false;
	private SimpleDateFormat timeStampFormat = new SimpleDateFormat("yyyyMMddHHmmssSS");

	private SignatureView surfaceView;
	private SurfaceHolder surfaceHolder;
	private ImageButton cancelButton;
	private ImageButton clearButton;
	private ImageButton okButton;

	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		Logger.D(TAG, "$$$$$$$$$$$$$$$$$$$$$$$$  onCreate");
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		//getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().setFormat(PixelFormat.TRANSLUCENT);
		setContentView(AndroidR.layout.signature);
		
		Bundle extras = getIntent().getExtras();
		callbackUrl = extras.getString(com.rhomobile.rhodes.signature.Signature.INTENT_EXTRA_PREFIX + "callback");
		
		surfaceView = (SignatureView) findViewById(AndroidR.id.signature_view);
		surfaceHolder = surfaceView.getHolder();
		surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_NORMAL);
		
		cancelButton = (ImageButton)findViewById(AndroidR.id.sig_cancelButton);
		cancelButton.setOnClickListener(this);
		clearButton = (ImageButton)findViewById(AndroidR.id.sig_clearButton);
		clearButton.setOnClickListener(this);
		okButton = (ImageButton)findViewById(AndroidR.id.sig_okButton);
		okButton.setOnClickListener(this);
	}

	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		Logger.D(TAG, "onRestoreInstanceState");
		super.onRestoreInstanceState(savedInstanceState);
		surfaceView.onRestoreInstanceState(savedInstanceState);
	}

	public boolean onKeyDown(int keyCode, KeyEvent event) {
		switch (keyCode) {
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
		Logger.D(TAG, "onSaveInstanceState");
		surfaceView.onSaveInstanceState(outState);
		super.onSaveInstanceState(outState);
	}

	protected void onStop() {
		Logger.D(TAG, "onStop");
		super.onStop();
	}

	public void onClick(View v) {
		if (v.getId() == AndroidR.id.sig_cancelButton) {
			Signature.doCallback(callbackUrl, "");
			finish();
		}
		if (v.getId() == AndroidR.id.sig_clearButton) {
			surfaceView.doClear();
		}
		if (v.getId() == AndroidR.id.sig_okButton) {
			takeSignature();
			finish();
		}
	}
	

	
	private void takeSignature() {
		try {
			String filename = "Image_" + timeStampFormat.format(new Date());
			ContentValues values = new ContentValues(5);
			values.put(Media.TITLE, filename);
			values.put(Media.DISPLAY_NAME, filename);
			values.put(Media.DATE_TAKEN, new Date().getTime());
			values.put(Media.MIME_TYPE, "image/png");
			values.put(Media.DESCRIPTION, "Handwrited Signature");
	
			Uri uri = getContentResolver().insert(Media.EXTERNAL_CONTENT_URI, values);
			String dir = Rhodes.getBlobPath();
			
			OutputStream osCommon = getContentResolver().openOutputStream(uri);
	
			
			String filePath = dir + "/" + filename + ".png";
			OutputStream osOwn = new FileOutputStream(filePath);
			
			Bitmap bitmap = surfaceView.makeBitmap();
	
			bitmap.compress(Bitmap.CompressFormat.PNG, 100, osCommon);		
			osCommon.flush();
			osCommon.close();
		
			bitmap.compress(Bitmap.CompressFormat.PNG, 100, osOwn);		
			osOwn.flush();
			osOwn.close();
			
			Signature.doCallback(callbackUrl, filePath);
		} catch (Exception ex) {
			Logger.E(TAG, ex.getMessage());
		}
	
	}


}
