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

package com.rhomobile.rhodes.signature;

import java.io.FileOutputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.RhodesAppOptions;

import android.content.ContentValues;
import android.graphics.PixelFormat;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.provider.MediaStore.Images.Media;
import android.view.KeyEvent;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.ImageButton;

public class ImageCapture extends BaseActivity implements OnClickListener
 {

	private static final String TAG = "ImageCapture";

	private String callbackUrl;
	private String imageFormat;
	private SimpleDateFormat timeStampFormat = new SimpleDateFormat("yyyyMMddHHmmssSS");

	private SignatureView surfaceView;
	private SurfaceHolder surfaceHolder;
	private ImageButton cancelButton;
	private ImageButton clearButton;
	private ImageButton okButton;

	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		//Logger.D(TAG, "$$$$$$$$$$$$$$$$$$$$$$$$  onCreate");
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		//getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().setFormat(PixelFormat.TRANSLUCENT);
		setContentView(AndroidR.layout.signature);

		Bundle extras = getIntent().getExtras();
		callbackUrl = extras.getString(com.rhomobile.rhodes.signature.Signature.INTENT_EXTRA_PREFIX + "callback");
		imageFormat = extras.getString(com.rhomobile.rhodes.signature.Signature.INTENT_EXTRA_PREFIX + "format");

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
			String file_ext = "jpg";
			String file_type = "image/jpeg";
			Bitmap.CompressFormat compress_format = Bitmap.CompressFormat.JPEG;
			if (imageFormat.equals("png")) {
				file_ext = "png";
				file_type = "image/png";
				compress_format = Bitmap.CompressFormat.PNG;
			}
			String filename = "Image_" + timeStampFormat.format(new Date());
			ContentValues values = new ContentValues(5);
			values.put(Media.TITLE, filename);
			values.put(Media.DISPLAY_NAME, filename);
			values.put(Media.DATE_TAKEN, new Date().getTime());
			values.put(Media.MIME_TYPE, file_type);
			values.put(Media.DESCRIPTION, "Handwrited Signature");

			String dir = RhodesAppOptions.getBlobPath();

			//Uri uri = getContentResolver().insert(Media.EXTERNAL_CONTENT_URI, values);
			//OutputStream osCommon = getContentResolver().openOutputStream(uri);

			String filePath = dir + "/" + filename + "." + file_ext;
			OutputStream osOwn = new FileOutputStream(filePath);

			Bitmap bitmap = surfaceView.makeBitmap();

			//bitmap.compress(compress_format, 100, osCommon);
			//osCommon.flush();
			//osCommon.close();

			bitmap.compress(compress_format, 100, osOwn);
			osOwn.flush();
			osOwn.close();

			Signature.doCallback(callbackUrl, filePath);
		} catch (Exception ex) {
			Logger.E(TAG, ex.getMessage());
		}

	}


}
