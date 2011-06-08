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
		
		if (obj != null) {
			if (obj instanceof Map<?,?>) {
				Map<Object,Object> settings = (Map<Object,Object>)obj;

				Object typeObj = settings.get("camera_type");
				Object formatObj = settings.get("format");
				Object widthObj = settings.get("desired_width");
				Object heightObj = settings.get("desired_height");
				Object cmodelObj = settings.get("color_model");
				
				if (typeObj != null && (typeObj instanceof String)) {
					String typeStr = (String)typeObj;
					if ((typeStr.equals("default")) || (typeStr.equals("main"))) {
						mCameraType = CAMERA_TYPE_MAIN;
					}
					if (typeStr.equals("front")) {
						mCameraType = CAMERA_TYPE_FRONT;
					}
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
	
	
	private int mCameraType;
	private int mFormat;
	private int mWidth;
	private int mHeight;
	private int mColorModel;
}