package com.rhomobile.rhodes.camera;

import android.content.Intent;

import com.rhomobile.rhodes.RhodesInstance;

public class Camera {

	//private static CameraListener cameraListener = new CameraListener();
	private static String callbackUrl = "";
	
	public static String BASE_CAMERA_DIR = RhodesInstance.getInstance().getRootPath() + "Camera";

	public static void takePicture(String sourceUrl) {
		callbackUrl = sourceUrl;
		Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(), ImageCapture.class);
		RhodesInstance.getInstance().startActivityForResult(intent, 5);
	}

	public static void choosePicture(String sourceUrl) {
		callbackUrl = sourceUrl;
		Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(), FileList.class);
		RhodesInstance.getInstance().startActivityForResult(intent, 5);
	}
	
	public static void doCallback(String filePath) {
		callback(callbackUrl, filePath, "", filePath == null || filePath.length() == 0);
	}

	public static native void callback(String callbackUrl, String filePath, String error, boolean cancelled);

}
