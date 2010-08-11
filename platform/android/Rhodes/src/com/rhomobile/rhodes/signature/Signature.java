/*
 ============================================================================
 Author	    : Dmitry Soldatenkov
 Version	: 1.5
 Copyright  : Copyright (C) 2010 Rhomobile. All rights reserved.

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
package com.rhomobile.rhodes.signature;

import java.io.File;

import android.content.Intent;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class Signature {

	private static final String TAG = "Signature";
	
	public static final String INTENT_EXTRA_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + "signature.";
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	private static void init() {
		File f = new File(RhodesService.getBlobPath());
		if (!f.exists())
			f.mkdirs();
	}
	
	private static class Picture implements Runnable {
		private String url;
		private Class<?> klass;
		private String format;
		
		public Picture(String u, Class<?> c, String form) {
			url = u;
			klass = c;
			format = form;
		}
		
		public void run() {
			init();
			RhodesService r = RhodesService.getInstance();
			Intent intent = new Intent(r.getContext(), klass);
			intent.putExtra(INTENT_EXTRA_PREFIX + "callback", url);
			intent.putExtra(INTENT_EXTRA_PREFIX + "format", format);
			r.startActivity(intent);
		}
	};
	
	public static void takeSignature(String url, String format) {
		try {
			Runnable runnable = new Picture(url, ImageCapture.class, format);
			PerformOnUiThread.exec(runnable, false);
		}
		catch (Exception e) {
			reportFail("takeSignature", e);
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
