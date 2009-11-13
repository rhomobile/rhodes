package com.rhomobile.rhodes.camera;

import android.content.Intent;
import android.util.Log;

import com.rhomobile.rhodes.RhodesInstance;

public class Camera {

	//private static CameraListener cameraListener = new CameraListener();
	private static String selectPictureUrl = "";
	
	public static String BASE_CAMERA_DIR = null;

	public static String takePicture(String sourceUrl) {

		selectPictureUrl = sourceUrl;
		
		Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(), ImageCapture.class);
		RhodesInstance.getInstance().startActivityForResult(intent, 5);
		
		return sourceUrl;
	}

	public static String selectPicture(String sourceUrl) {
		selectPictureUrl = sourceUrl;
		//open select picture dialog
		Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(), FileList.class);
		RhodesInstance.getInstance().startActivityForResult(intent, 5);
		
		//cameraListener.selectPicture(); // start select file activity
		return sourceUrl;
	}

	public static String getSelectPictureUrl() {
		return selectPictureUrl;
	}
	
	static public void callCallback(String filePath)
	{
		String sourceUrl = com.rhomobile.rhodes.camera.Camera.getSelectPictureUrl();
		String fullUrl = RhodesInstance.getInstance().getCurrentUrl().replaceAll("\\\\", "/");

		String[] paths = sourceUrl.replaceAll("\\\\", "/").split("/");

		for (int i = paths.length - 1; i >= 0; i--) {
			System.out.println(fullUrl);

			if (!paths[i].equals("")) {
				int pos = fullUrl.lastIndexOf("/" + paths[i]);

				if (pos != -1) {
					fullUrl = fullUrl.substring(0, pos);
				}
			}
		}

		if (fullUrl.endsWith("/"))
			fullUrl = fullUrl.substring(0, fullUrl.length() - 2);

		if (sourceUrl.startsWith("/"))
			fullUrl += sourceUrl;
		else
			fullUrl += "/" + sourceUrl;

		System.out.println(fullUrl);

		String selectedFile = filePath;
		String status = (selectedFile == null || "".equals(selectedFile)) ? "ok" : "cancel"; 
		new Thread(new CameraRunnable(fullUrl, selectedFile, 
				status, "User has canceled image selection")).start();

	}

}
