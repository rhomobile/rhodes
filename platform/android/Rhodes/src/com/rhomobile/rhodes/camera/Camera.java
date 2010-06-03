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

import android.content.Intent;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.Rhodes;
import com.rhomobile.rhodes.RhodesInstance;

public class Camera {

	private static final String TAG = "Camera";
	
	public static final String INTENT_EXTRA_PREFIX = Rhodes.INTENT_EXTRA_PREFIX + "camera.";
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	private static void init() {
		File f = new File(Rhodes.getBlobPath());
		if (!f.exists())
			f.mkdirs();
	}
	
	private static class CameraDisabled implements Runnable {
		
		private String url;
		
		public CameraDisabled(String u) {
			url = u;
		}
		
		public void run() {
			callback(url, "", "Camera disabled", false);
		}
		
	};
	
	private static class Picture implements Runnable {
		private String url;
		private Class<?> klass;
		
		public Picture(String u, Class<?> c) {
			url = u;
			klass = c;
		}
		
		public void run() {
			init();
			Rhodes r = RhodesInstance.getInstance();
			Intent intent = new Intent(r, klass);
			intent.putExtra(INTENT_EXTRA_PREFIX + "callback", url);
			r.startActivity(intent);
		}
	};
	
	public static void takePicture(String url) {
		try {
			Runnable runnable = Capabilities.CAMERA_ENABLED ? new Picture(url, ImageCapture.class) :
				new CameraDisabled(url);
			Rhodes.performOnUiThread(runnable, false);
		}
		catch (Exception e) {
			reportFail("takePicture", e);
		}
	}

	public static void choosePicture(String url) {
		try {
			Rhodes.performOnUiThread(new Picture(url, FileList.class), false);
		}
		catch (Exception e) {
			reportFail("choosePicture", e);
		}
	}
	
	public static void doCallback(String callbackUrl, String filePath) {
		String fp = filePath == null ? "" : filePath;
		int idx = fp.lastIndexOf('/');
		if (idx != -1)
			fp = fp.substring(idx + 1);
		callback(callbackUrl, fp, "", fp.length() == 0);
	}

	public static native void callback(String callbackUrl, String filePath, String error, boolean cancelled);

}
