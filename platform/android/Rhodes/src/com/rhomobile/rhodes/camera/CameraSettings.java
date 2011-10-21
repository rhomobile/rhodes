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

import java.io.Serializable;
import java.util.Map;
import java.util.Vector;

import android.graphics.Color;


class CameraSettings implements Serializable {

	public static final int CAMERA_TYPE_MAIN = 1;
	public static final int CAMERA_TYPE_FRONT = 2;

	public static final int CAMERA_FORMAT_JPG = 1;
	public static final int CAMERA_FORMAT_PNG = 2;

	public static final int CAMERA_COLOR_MODEL_RGB = 1;
	public static final int CAMERA_COLOR_MODEL_GRAYSCALE = 2;


	public CameraSettings(Object obj) {
		mCameraType = CAMERA_TYPE_MAIN;
		mFormat = CAMERA_FORMAT_JPG;
		mWidth = 0;
		mHeight = 0;
		mColorModel = CAMERA_COLOR_MODEL_RGB;
		mFlashMode = null;

		if (obj != null) {
			if (obj instanceof Map<?,?>) {
				Map<Object,Object> settings = (Map<Object,Object>)obj;

				Object typeObj = settings.get("camera_type");
				Object formatObj = settings.get("format");
				Object widthObj = settings.get("desired_width");
				Object heightObj = settings.get("desired_height");
				Object cmodelObj = settings.get("color_model");
				Object flashModeObj = settings.get("flash_mode");

				if (typeObj != null && (typeObj instanceof String)) {
					String typeStr = (String)typeObj;
					if ((typeStr.equals("default")) || (typeStr.equals("main"))) {
						mCameraType = CAMERA_TYPE_MAIN;
					}
					if (typeStr.equals("front")) {
						mCameraType = CAMERA_TYPE_FRONT;
					}
				}

				if (flashModeObj != null && (flashModeObj instanceof String)) {
					mFlashMode = (String)flashModeObj;
				}

				if (formatObj != null && (formatObj instanceof String)) {
					String formatStr = (String)formatObj;
					if (formatStr.equals("jpg")) {
						mFormat = CAMERA_FORMAT_JPG;
					}
					if (formatStr.equals("png")) {
						mFormat = CAMERA_FORMAT_PNG;
					}
				}

				if (cmodelObj != null && (cmodelObj instanceof String)) {
					String cmodelStr = (String)cmodelObj;
					if (cmodelStr.equals("RGB")) {
						mColorModel = CAMERA_COLOR_MODEL_RGB;
					}
					if (cmodelStr.equals("Grayscale")) {
						mColorModel = CAMERA_COLOR_MODEL_GRAYSCALE;
					}
				}

				if (widthObj != null && (widthObj instanceof String)) {
					String widthStr = (String)widthObj;
					try {
						int width = Integer.parseInt(widthStr);
						mWidth = width;
					}
					catch (NumberFormatException e) {
						// Do nothing here
					}
				}

				if (heightObj != null && (heightObj instanceof String)) {
					String heightStr = (String)heightObj;
					try {
						int height = Integer.parseInt(heightStr);
						mHeight = height;
					}
					catch (NumberFormatException e) {
						// Do nothing here
					}
				}

			}
		}
	}

	int getCameraType() {
		return mCameraType;
	}

	int getFormat() {
		return mFormat;
	}

	int getWidth() {
		return mWidth;
	}

	int getHeight() {
		return mHeight;
	}

	int getColorModel() {
		return mColorModel;
	}

	String getFlashMode() {
		return mFlashMode;
	}


	private int mCameraType;
	private int mFormat;
	private int mWidth;
	private int mHeight;
	private int mColorModel;
	private String mFlashMode;
}