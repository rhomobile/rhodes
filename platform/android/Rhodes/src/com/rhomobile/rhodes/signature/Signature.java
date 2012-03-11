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

import android.content.Intent;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.AbsoluteLayout;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesAppOptions;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.extmanager.IRhoExtension;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.util.Utils;

import com.rhomobile.rhodes.extmanager.AbstractRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoExtData;
import com.rhomobile.rhodes.extmanager.IRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

public class Signature extends AbstractRhoExtension implements IRhoExtension {

	static final String TAG = "Signature";
	
	public static final String INTENT_EXTRA_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + "signature.";
	
	private static final String SIGNATURE_EXT = "signature_ext";
	
	private static SignatureView ourInlineSignatureView = null;
	
	private static Signature ourInstance = null;
	
	public SignatureProperties mProperties = null;
	
	private static final boolean ENABLE_LOGGING = true;
	
	public static void reportMsg(String msg) {
		if (ENABLE_LOGGING) {
			Utils.platformLog("SignatureCapture", msg);
		}
	}
	
	private static void init() {
		File f = new File(RhodesAppOptions.getBlobPath());
		if (!f.exists())
			f.mkdirs();
	}
	
	private static class Picture implements Runnable {
		private Class<?> klass;
		
		public Picture(Class<?> c) {
			klass = c;
		}
		
		public void run() {
			init();
			RhodesActivity ra = RhodesActivity.safeGetInstance();
			Intent intent = new Intent(ra, klass);
			intent.putExtra(INTENT_EXTRA_PREFIX + "callback", getSharedInstance().mProperties.callbackUrl);
			intent.putExtra(INTENT_EXTRA_PREFIX + "imageFormat", getSharedInstance().mProperties.imageFormat);
			intent.putExtra(INTENT_EXTRA_PREFIX + "penColor", getSharedInstance().mProperties.penColor);
			intent.putExtra(INTENT_EXTRA_PREFIX + "penWidth", getSharedInstance().mProperties.penWidth);
			intent.putExtra(INTENT_EXTRA_PREFIX + "bgColor", getSharedInstance().mProperties.bgColor);
			ra.startActivity(intent);
		}
	};
	
	private static void processParams(Object params) {
		if (params == null) {
			return;
		}
		if (params instanceof Map<?,?>) {
			Map<Object,Object> settings = (Map<Object,Object>)params;

			int prop_count = SignatureProperties.PROPERTIES_LIST.length;
			int i;
			for (i = 0; i < prop_count; i++) {
				String prop_name = SignatureProperties.PROPERTIES_LIST[i];
				Object valueObj = settings.get(prop_name);
				if (valueObj != null) {
					getSharedInstance().mProperties.setPropertyByName(prop_name, new String(((String)valueObj)));
				}
			}
		}
	}
	
	
	public static void takeSignature(String url, Object params) {
		try {
			processParams(params);
			getSharedInstance().mProperties.penColor = getSharedInstance().mProperties.penColor | 0xFF000000;
			getSharedInstance().mProperties.bgColor = getSharedInstance().mProperties.bgColor | 0xFF000000;
			getSharedInstance().mProperties.setPropertyByName(SignatureProperties.CALLBACK, url);
			Runnable runnable = new Picture(ImageCapture.class);
			PerformOnUiThread.exec(runnable);
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
	}

	
	public static void doCallback(String callbackUrl, String filePath) {
		String fp = filePath == null ? "" : filePath;
		int idx = fp.lastIndexOf('/');
		if (idx != -1)
			fp = fp.substring(idx + 1);
		callback(callbackUrl, fp, "", fp.length() == 0);
	}

    public static void inline_signature_visible(int visible, Object params) {
        inlineSignatureVisible(RhoExtManager.getInstance(), (visible != 0), params);
    }

    public static void inlineSignatureVisible(final IRhoExtManager extManager, boolean visible, Object params) {
		if (!visible) {
			inlineSignatureHide(extManager);
			return;
		}
		
		processParams(params);
		
		PerformOnUiThread.exec( new Runnable () {
			public void run() {
				reportMsg(" $$$ Start make of Signature View");
				
				ViewGroup wv = (ViewGroup)extManager.getWebView();
				if ((wv != null) && (ourInlineSignatureView != null)) {
					wv.removeView(ourInlineSignatureView);
					ourInlineSignatureView = null;
				}
				
				ourInlineSignatureView = new SignatureView(ContextFactory.getUiContext(), null);
				
				{
					if ((getSharedInstance().mProperties.bgColor & 0xFF000000) != 0xFF000000) {
						//ourInlineSignatureView.setVisibility(View.INVISIBLE);
						ourInlineSignatureView.isTransparency = true;
					}
					ourInlineSignatureView.setupView(	getSharedInstance().mProperties.penColor, 
							getSharedInstance().mProperties.penWidth, 
							getSharedInstance().mProperties.bgColor);
					ourInlineSignatureView.setLayoutParams(
							new AbsoluteLayout.LayoutParams(	getSharedInstance().mProperties.width, 
																getSharedInstance().mProperties.height, 
																getSharedInstance().mProperties.left, 
																getSharedInstance().mProperties.top));
					
				}
				
				wv.addView(ourInlineSignatureView);
				wv.bringChildToFront(ourInlineSignatureView);	
				ourInlineSignatureView.requestFocus();
				ourInlineSignatureView.bringToFront();
				ourInlineSignatureView.invalidate();
				
				//wv.invalidate();
				
				reportMsg(" $$$ Finish make of Signature View");
			}
		});
	}
	
	public static void inline_signature_capture(String callback_url) {
		if (ourInlineSignatureView != null) {
			ImageCapture.takeSignature(callback_url, getSharedInstance().mProperties.imageFormat, ourInlineSignatureView.makeBitmap());
		}
		inlineSignatureHide(RhoExtManager.getInstance());
	}
	
	public static void inline_signature_clear() {
	    inlineSignatureClear(RhoExtManager.getInstance());
	}
	
	public static void inlineSignatureClear(IRhoExtManager extmanager) {
		PerformOnUiThread.exec( new Runnable () {
			public void run() {
				if (ourInlineSignatureView != null) {
					ourInlineSignatureView.doClear();
					//RhodesService.getInstance().getMainView().getWebView(-1).getView().invalidate();
					//ViewGroup wv = (WebView)RhodesService.getInstance().getMainView().getWebView(-1).getView();
					//if (wv != null) {
					//	wv.invalidate();
					//}
					
				}
			}
		});
	}
	
	public static void inlineSignatureHide(final IRhoExtManager extManager) {
		PerformOnUiThread.exec( new Runnable () {
			public void run() {
				if (ourInlineSignatureView != null) {
					ViewGroup wv = (ViewGroup)extManager.getWebView();
					if (wv != null) {
						wv.removeView(ourInlineSignatureView);
						ourInlineSignatureView = null;
					}
				}
			}
		});
	}
	
	@Override
	public void onBeforeNavigate(IRhoExtManager extManager, String url, IRhoExtData data) {
		inlineSignatureHide(extManager);
	}
	
	@Override
	public void onSetProperty(IRhoExtManager extManager, String name, String value, IRhoExtData data) {
		if ((name == null) || (value == null)) {
			return;
		}
		mProperties.setPropertyByName(name, value);
		if ("Visibility".equalsIgnoreCase(name)) {
			if ("Visible".equalsIgnoreCase(value)) {
				// show
				inlineSignatureVisible(extManager, true, null);
			}
			else {
				// hide;
				inlineSignatureHide(extManager);
			}
		}
		else if ("Clear".equalsIgnoreCase(name)) {
			// clear
			inlineSignatureClear(extManager);
		}
	}
	
	public static Signature getSharedInstance() {
		if (ourInstance == null) {
			ourInstance = new Signature();
			ourInstance.mProperties = new SignatureProperties();
		}
		return ourInstance;
	}
	
	public static void registerSignatureCaptureExtension() {
		RhoExtManager.getInstance().registerExtension(SIGNATURE_EXT, getSharedInstance());
	}
	
	public static native void callback(String callbackUrl, String filePath, String error, boolean cancelled);

}


