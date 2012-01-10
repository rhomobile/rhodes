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

package com.rhomobile.rhodes.signature;

import java.io.File;
import java.util.Map;
import java.util.Vector;

import android.content.Intent;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesAppOptions;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class Signature {

	private static final String TAG = "Signature";
	
	public static final String INTENT_EXTRA_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + "signature.";
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	private static void init() {
		File f = new File(RhodesAppOptions.getBlobPath());
		if (!f.exists())
			f.mkdirs();
	}
	
	private static class Picture implements Runnable {
		private String url;
		private Class<?> klass;
		private String imageFormat;
		private int penColor;
		private float penWidth;
		private int bgColor;
		
		public Picture(String u, Class<?> c, String _imageFormat, int _penColor, float _penWidth, int _bgColor) {
			url = u;
			klass = c;
			imageFormat = _imageFormat;
			penColor = _penColor;
			penWidth = _penWidth;
			bgColor = _bgColor;
		}
		
		public void run() {
			init();
			RhodesActivity ra = RhodesActivity.getInstance();
			Intent intent = new Intent(ra, klass);
			intent.putExtra(INTENT_EXTRA_PREFIX + "callback", url);
			intent.putExtra(INTENT_EXTRA_PREFIX + "imageFormat", imageFormat);
			intent.putExtra(INTENT_EXTRA_PREFIX + "penColor", penColor);
			intent.putExtra(INTENT_EXTRA_PREFIX + "penWidth", penWidth);
			intent.putExtra(INTENT_EXTRA_PREFIX + "bgColor", bgColor);
			ra.startActivity(intent);
		}
	};
	
	public static void takeSignature(String url, Object params) {
		try {
			String imageFormat = "png";
			int penColor = 0xFF66009A;
			float penWidth = 2;
			int bgColor = 0xFFFFFFFF;
			
			if (params instanceof Map<?,?>) {
				Map<Object,Object> settings = (Map<Object,Object>)params;
				
				Object imgFrmtObj = settings.get("imageFormat");
				if ((imgFrmtObj != null) && (imgFrmtObj instanceof String)) {
					imageFormat = new String(((String)imgFrmtObj));
				}
				
				Object penColorObj = settings.get("penColor");
				if ((penColorObj != null) && (penColorObj instanceof String)) {
					penColor = Integer.parseInt((String)penColorObj) | 0xFF000000;
				}
					
				Object penWidthObj = settings.get("penWidth");
				if ((penWidthObj != null) && (penWidthObj instanceof String)) {
					penWidth = Float.parseFloat((String)penWidthObj);
				}

				Object bgColorObj = settings.get("bgColor");
				if ((bgColorObj != null) && (bgColorObj instanceof String)) {
					bgColor = Integer.parseInt((String)bgColorObj) | 0xFF000000;
				}
			}
			
			Runnable runnable = new Picture(url, ImageCapture.class, imageFormat, penColor, penWidth, bgColor);
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
