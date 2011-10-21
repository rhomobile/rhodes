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

package com.rhomobile.rhodes.mainview;

import java.io.IOException;
import java.io.InputStream;
import java.util.Map;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.util.Utils;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.View;
import android.webkit.WebView;
import android.widget.ImageView;

public class SplashScreen implements MainView {

	private static final String TAG = SplashScreen.class.getSimpleName();

	private static final boolean DEBUG = false;

	private static final String LOADING_ANDROID_PNG = "apps/app/loading.android.png";
	private static final String LOADING_PNG = "apps/app/loading.png";
	private static final String LOADING_PAGE = "apps/app/loading.html";

	private View mContentView;

	private WebView mWebView;

	private native void nativeStart();
	private native void nativeHide();
	private native int howLongWaitMs();

	private boolean mFirstNavigate = true;

	public SplashScreen(Context context) {
		AssetManager am = context.getResources().getAssets();
		mContentView = createImageView(context, am);
		if (mContentView == null)
			mContentView = createHtmlView(context, am);
		mFirstNavigate = true;
	}

	private View createImageView(Context context, AssetManager am) {
		String[] imageFiles = {LOADING_ANDROID_PNG, LOADING_PNG};
		for (String imageFile : imageFiles) {
			InputStream is = null;
			try {
				is = am.open(imageFile);
				Bitmap bitmap = BitmapFactory.decodeStream(is);

				ImageView view = new ImageView(context);
				view.setImageBitmap(bitmap);
				view.setAdjustViewBounds(false);

				return view;
			} catch (IOException e) {
				if (DEBUG)
					Log.d(TAG, "Can't load " + imageFile, e);
			}
			finally {
				if (is != null)
					try {
						is.close();
					} catch (IOException e) {}
			}
		}

		return null;
	}

	private View createHtmlView(Context context, AssetManager am) {
		boolean hasNeededPage;

		String page = LOADING_PAGE;
		InputStream is = null;
		try {
			is = am.open(page);
			hasNeededPage = true;
		} catch (IOException e) {
			if (DEBUG)
				Log.d(TAG, "Can't load " + page, e);
			hasNeededPage = false;
		}
		finally {
			if (is != null)
				try {
					is.close();
				} catch (IOException e) {}
		}

		// Now create WebView and load appropriate content there
		WebView view = new WebView(context);

		if (hasNeededPage)
			view.loadUrl("file:///android_asset/" + page);
		else
			view.loadData("<html><title>Loading</title><body>Loading...</body></html>", "text/html", "utf-8");

		return view;
	}

	public void start() {
		nativeStart();
	}

	@Override
	public View getView() {
		return mContentView;
	}

	@Override
	public WebView getWebView(int index) {
		if (mWebView == null) {
			RhodesActivity ra = RhodesActivity.getInstance();
			mWebView = ra.createWebView();
		}
		return mWebView;
	}

	@Override
	public void goBack() {
		// Nothing here
	}

	@Override
	public void navigate(String url, int index) {

		final String _url = url;
		final int _index = index;
		if (DEBUG)
			Log.d(TAG, "navigate: url=" + url);

		int delay = howLongWaitMs();
		if (delay < 0) {
			delay = 0;
		}

		Utils.platformLog(TAG, "$$$$$$$$$$$$$$$$$$$$$$$     DELAY for SplashScreen = "+String.valueOf(delay));

		PerformOnUiThread.exec(new Runnable() {
			private String mUrl = _url;
			private int mIndex = _index;
			public void run() {
				if (mFirstNavigate) {
					mFirstNavigate = false;
			        RhodesService r = RhodesService.getInstance();
					MainView mainView = r.getMainView();
					SimpleMainView v = new SimpleMainView(mainView);
					r.setMainView(v);
					//getWebView(0).loadUrl(url);
					v.navigate(mUrl,0);
				}
				else {
			        RhodesService r = RhodesService.getInstance();
					MainView mainView = r.getMainView();
					if (mainView != null) {
						mainView.navigate(mUrl,mIndex);
					}
				}
			}

		}, delay);

		//nativeHide();


	}

	@Override
	public WebView detachWebView() {
		return getWebView(0);
	}
	@Override
	public void back(int index)
	{
        RhodesActivity ra = RhodesActivity.getInstance();
        if ( ra != null )
            ra.moveTaskToBack(true);
	}

	@Override
	public void forward(int index) {
	}
	@Override
	public void reload(int index) {
		// TODO Auto-generated method stub

	}
	@Override
	public String currentLocation(int index) {
		// TODO Auto-generated method stub
		return null;
	}
	@Override
	public void switchTab(int index) {
	}
	@Override
	public int activeTab() {
		return 0;
	}
	@Override
	public int getTabsCount() {
		return 1;
	}
	@Override
	public void loadData(String data, int index) {
	}
	@Override
	public void addNavBar(String title, Map<Object, Object> left,
			Map<Object, Object> right) {
	}
	@Override
	public void removeNavBar() {
	}

	/*
	public void start(ViewGroup outer) {
		outer.removeAllViews();
		if (view instanceof SplashImageView) {
			((SplashImageView)view).setupExecuted = false;
		}
		outer.addView(view);
		RhoService.platformLog(TAG, " view was showed on screen");
	}

	public void rho_start() {
		nativeStart();
		RhoService.platformLog(TAG, " rho native loading splash screen started");
	}

	public void hide(ViewGroup outer) {
		nativeHide();
		outer.removeView(view);
		RhoService.platformLog(TAG, " splash screen closed");
	}
	*/
}
