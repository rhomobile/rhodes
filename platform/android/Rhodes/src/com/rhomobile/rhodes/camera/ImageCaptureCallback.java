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

import java.io.FileOutputStream;
import java.io.OutputStream;

import com.rhomobile.rhodes.Logger;

import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;

public class ImageCaptureCallback implements PictureCallback {

	private static final String TAG = "ImageCapture";

	private ImageCapture mOwner;
	private String callbackUrl;
	private OutputStream osCommon;
	private String filePath;

	private int mImgWidth;
	private int mImgHeight;
	private String mImgFormat;

	public ImageCaptureCallback(ImageCapture owner, String u, OutputStream o, String f, int w, int h, String format) {
		mOwner = owner;
		callbackUrl = u;
		osCommon = o;
		filePath = f;
		mImgWidth = w;
		mImgHeight = h;
		mImgFormat = format;
	}

	public void onPictureTaken(byte[] data, Camera camera) {
		try {
			Logger.D(TAG, "PICTURE CALLBACK JPEG: " + data.length + " bytes");

			if (osCommon != null) {
				osCommon.write(data);
				osCommon.flush();
				osCommon.close();
			}
			OutputStream osOwn = new FileOutputStream(filePath);
			osOwn.write(data);
			osOwn.flush();
			osOwn.close();

			com.rhomobile.rhodes.camera.Camera.doCallback(callbackUrl, filePath, mImgWidth, mImgHeight, mImgFormat);
			mOwner.finish();

		} catch (Exception e) {
			Logger.E(TAG, e);
		}
	}

}
