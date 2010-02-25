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

import android.content.Intent;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesInstance;

public class Camera {

	private static final String TAG = "Camera";
	
	//private static CameraListener cameraListener = new CameraListener();
	private static String callbackUrl = "";
	
	public static String BASE_CAMERA_DIR = RhodesInstance.getInstance().getRootPath() + "apps/public/db-files";
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}

	public static void takePicture(String sourceUrl) {
		try {
			callbackUrl = sourceUrl;
			Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(), ImageCapture.class);
			RhodesInstance.getInstance().startActivityForResult(intent, 5);
		}
		catch (Exception e) {
			reportFail("takePicture", e);
		}
	}

	public static void choosePicture(String sourceUrl) {
		try {
			callbackUrl = sourceUrl;
			Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(), FileList.class);
			RhodesInstance.getInstance().startActivityForResult(intent, 5);
		}
		catch (Exception e) {
			reportFail("choosePicture", e);
		}
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
