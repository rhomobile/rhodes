package com.rhomobile.rhodes.camera;

import android.content.Intent;
import android.util.Log;

import com.rho.RhoClassFactory;
import com.rhomobile.rhodes.RhodesInstance;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyOneArgMethod;
import com.xruby.runtime.lang.RubyValue;

public class Camera extends RubyBasic {

	//private static CameraListener cameraListener = new CameraListener();
	private static String selectPictureUrl = "";
	
	public static String BASE_CAMERA_DIR = null;

	Camera(RubyClass c) {

		super(c);
	}

	public static RubyValue takePicture(RubyValue arg) {
		return ObjectFactory.createString(takePictureImpl(arg.asString()));
	}

	public static RubyValue selectPicture(RubyValue arg) {
		return ObjectFactory.createString(selectPictureImpl(arg.asString()));
	}

	public static void initMethods(RubyClass klass) {

		try {
			BASE_CAMERA_DIR = RhoClassFactory.createFile().getDirPath("Camera");
		} catch (Exception e) {
			Log.e("FileList", e.getMessage());
			RhodesInstance.getInstance().stopSelf();
		}
		
		klass.getSingletonClass().defineMethod("take_picture",
				new RubyOneArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0,
							RubyBlock block) {
						return Camera.takePicture(arg0);
					}
				});

		klass.getSingletonClass().defineMethod("choose_picture",
				new RubyOneArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0,
							RubyBlock block) {
						return Camera.selectPicture(arg0);
					}
				});
	}

	private static String takePictureImpl(String sourceUrl) {

		selectPictureUrl = sourceUrl;
		
		Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(), ImageCapture.class);
		RhodesInstance.getInstance().startActivityForResult(intent, 5);
		
		return sourceUrl;
	}

	private static String selectPictureImpl(String sourceUrl) {
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
		String fullUrl = RhodesInstance.getInstance().getCurrentUrl()
		.replaceAll("\\\\", "/");

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
