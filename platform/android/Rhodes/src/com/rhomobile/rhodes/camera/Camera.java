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

import java.io.File;
import java.util.Iterator;
import java.util.List;

import android.content.Intent;
import android.hardware.Camera.Size;
import android.os.Build;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesAppOptions;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.util.Utils;

public class Camera {

	private static final String TAG = "Camera";
	
	public static final String INTENT_EXTRA_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + "camera.";
	
	private static int mMainCamera_max_Width = 0;
	private static int mMainCamera_max_Height = 0;
	private static int mFrontCamera_max_Width = 0;
	private static int mFrontCamera_max_Height = 0;
	
	private static CameraService ourCameraService = null;
	
	public static CameraService getCameraService() {
		if (ourCameraService == null) {
			if (Capabilities.CAMERA_ENABLED) {
				int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
				if (sdkVersion >= Build.VERSION_CODES.GINGERBREAD) {
					ourCameraService = new CameraNewService();
				}
				else {
					if (sdkVersion >= Build.VERSION_CODES.ECLAIR) {
						ourCameraService = new CameraSemiService();
					}
					else {
						ourCameraService = new CameraOldService();
					}
				}
			}
			else {
				Logger.I(TAG, "Camera capability is not enabled !");
			}
		}
		return ourCameraService;
	}

	public static boolean ENABLE_DEBUG_LOGGING = false;
	
	public static void logDebug(String tag, String message) {
		if (ENABLE_DEBUG_LOGGING) {
			Utils.platformLog(tag, message);
		}
	}
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	public static void init_from_UI_Thread() {
		int[] s_m = getCameraResolution("main");
		mMainCamera_max_Width = s_m[0];
		mMainCamera_max_Height = s_m[1];
		int[] s_f = getCameraResolution("front");
		mFrontCamera_max_Width = s_f[0];
		mFrontCamera_max_Height = s_f[1];
	}
	
	private static void init() {
		File f = new File(RhodesAppOptions.getBlobPath());
		if (!f.exists())
			f.mkdirs();
	}
	
	private static class CameraDisabled implements Runnable {
		
		private String url;
		
		public CameraDisabled(String u) {
			url = u;
		}
		
		public void run() {
			execute_callback(url, "", "Camera disabled", false, 0, 0, "");
		}
		
	};
	
	private static class Picture implements Runnable {
		private String url;
		private Class<?> klass;
		private CameraSettings settings;
		
		public Picture(String u, Class<?> c, Object settingsObj) {
			url = u;
			klass = c;
			settings = new CameraSettings(settingsObj);
		}
		
		public void run() {
			init();
			RhodesActivity ra = RhodesActivity.getInstance();
			Intent intent = new Intent(ra, klass);
			intent.putExtra(INTENT_EXTRA_PREFIX + "callback", url);
			intent.putExtra(INTENT_EXTRA_PREFIX + "settings", settings);
			ra.startActivity(intent);
		}
	};
	
	public static void takePicture(String url, Object params_obj) {
		try {
			Runnable runnable = Capabilities.CAMERA_ENABLED ? new Picture(url, ImageCapture.class, params_obj) :
				new CameraDisabled(url);
			PerformOnUiThread.exec(runnable, false);
		}
		catch (Exception e) {
			reportFail("takePicture", e);
		}
	}

	public static void choosePicture(String url) {
		try {
			PerformOnUiThread.exec(new Picture(url, FileList.class, null), false);
		}
		catch (Exception e) {
			reportFail("choosePicture", e);
		}
	}
	
	public static void doCallback(String callbackUrl, String filePath, int w, int h, String format) {
		String fp = filePath == null ? "" : filePath;
		int idx = fp.lastIndexOf('/');
		if (idx != -1)
			fp = fp.substring(idx + 1);
		execute_callback(callbackUrl, fp, "", fp.length() == 0, w, h, format);
		
	}
	
	public static void execute_callback(String callbackUrl, String filePath, String error, boolean cancelled, int w, int h, String format) {
		StringBuffer body = new StringBuffer();
		body.append("&rho_callback=1");
		if (cancelled || ((error != null) && (error.length() > 0))) {
			if (cancelled) {
				body.append("&status=cancel&message=User canceled operation.");
			}
			else {
				body.append("&status=error&message=" + error);
			}
		}
		else {
			body.append("&status=ok&image_uri=db%2Fdb-files%2F" + filePath);
			if ((w > 0) && (h > 0)) {
				body.append("&image_width="+String.valueOf(w));
				body.append("&image_height="+String.valueOf(h));
			}
			if ((format != null) && (format.length() > 0)) {
				body.append("&image_format="+format);
			}
		}
		callback(callbackUrl, body.toString());
	}

	//public static native void callback(String callbackUrl, String filePath, String error, boolean cancelled);
	public static native void callback(String callbackUrl, String body);

	public static int[] getCameraResolution(String camera_type) {
		android.hardware.Camera camera = null;
		try {
			if (getCameraService() != null) {
				if ("front".equals(camera_type)) {
					camera = getCameraService().getFrontCamera();
				}
				else {
					if ("default".equals(camera_type) || "main".equals(camera_type)) {
						camera = getCameraService().getMainCamera();
					}
				}
			}
		}
		catch (Exception e) {
			Logger.E(TAG, e.getMessage());
		}

		if (camera == null) {
			
			int[] res = {0,0};
			return res;
		}

		int maxW = 10000;
		int maxH = 10000;
		
		CameraService.Size s = getCameraService().getClosestPictureSize(camera, maxW, maxH);
		if (s == null) {
			int[] ress = {0, 0};
			return ress;
		}
		camera.release();
		int[] res = {s.width, s.height};
		return res;
	}
	
	public static int getMaxCameraWidth(String camera_type) {
		if ("default".equals(camera_type) || "main".equals(camera_type)) {
			return mMainCamera_max_Width;
		}
		if ("front".equals(camera_type)) {
			return mFrontCamera_max_Width;
		}
		return 0;
	}

	public static int getMaxCameraHeight(String camera_type) {
		if ("default".equals(camera_type) || "main".equals(camera_type)) {
			return mMainCamera_max_Height;
		}
		if ("front".equals(camera_type)) {
			return mFrontCamera_max_Height;
		}
		return 0;
	}
	
}
