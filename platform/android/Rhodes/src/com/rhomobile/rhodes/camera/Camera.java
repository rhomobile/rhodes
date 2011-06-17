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

public class Camera {

	private static final String TAG = "Camera";
	
	public static final String INTENT_EXTRA_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + "camera.";
	
	private static int mMainCamera_max_Width = 0;
	private static int mMainCamera_max_Height = 0;
	private static int mFrontCamera_max_Width = 0;
	private static int mFrontCamera_max_Height = 0;
	
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
			int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
			if (sdkVersion >= Build.VERSION_CODES.GINGERBREAD) {
				if ("front".equals(camera_type)) {
					// find front camera
					int camera_count = android.hardware.Camera.getNumberOfCameras();
					int i;
					for (i = 0 ; i < camera_count; i++) {
						android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
						android.hardware.Camera.getCameraInfo(i, info);
						if (info.facing == info.CAMERA_FACING_FRONT) {
							camera = android.hardware.Camera.open(i);
							break;
						}
					}
				}
				else {
					if ("default".equals(camera_type) || "main".equals(camera_type)) {
						camera = android.hardware.Camera.open();
					}
				}
			}
			else {
				if ("default".equals(camera_type) || "main".equals(camera_type)) {
					camera = android.hardware.Camera.open();
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
		
		android.hardware.Camera.Parameters p = camera.getParameters();
		
		List<Size> sizes = p.getSupportedPictureSizes();
		Iterator<Size> iter = sizes.iterator();
		// find max size
		float max_S = -1;
		int maxW = 0;
		int maxH = 0;
		while (iter.hasNext()) {
			Size s = iter.next();
			float cur_S = ((float)s.width)*((float)s.height);
			if (cur_S > max_S) {
				max_S = cur_S;
				maxW = s.width;
				maxH = s.height;
			}
		}
		camera.release();
		int[] res = {maxW, maxH};
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
