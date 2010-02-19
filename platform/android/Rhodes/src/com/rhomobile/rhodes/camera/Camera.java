package com.rhomobile.rhodes.camera;

import android.content.Intent;

import com.rhomobile.rhodes.RhodesInstance;

public class Camera {

	//private static CameraListener cameraListener = new CameraListener();
	private static String callbackUrl = "";
	
	public static String BASE_CAMERA_DIR = RhodesInstance.getInstance().getRootPath() + "apps/public/db-files";

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
		String fp = filePath == null ? "" : filePath;
		int idx = fp.lastIndexOf('/');
		if (idx != -1)
			fp = fp.substring(idx + 1);
		callback(callbackUrl, fp, "", fp.length() == 0);
	}

	public static native void callback(String callbackUrl, String filePath, String error, boolean cancelled);

}
