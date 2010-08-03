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

	public ImageCaptureCallback(ImageCapture owner, String u, OutputStream o, String f) {
		mOwner = owner;
		callbackUrl = u;
		osCommon = o;
		filePath = f;
	}

	public void onPictureTaken(byte[] data, Camera camera) {
		try {
			Logger.D(TAG, "PICTURE CALLBACK JPEG: " + data.length + " bytes");

			osCommon.write(data);
			osCommon.flush();
			osCommon.close();
			
			OutputStream osOwn = new FileOutputStream(filePath);
			osOwn.write(data);
			osOwn.flush();
			osOwn.close();

			com.rhomobile.rhodes.camera.Camera.doCallback(callbackUrl, filePath);
			mOwner.finish();

		} catch (Exception e) {
			Logger.E(TAG, e);
			e.printStackTrace();
		}
	}
	
}
