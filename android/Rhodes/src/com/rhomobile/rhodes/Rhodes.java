/*
 ============================================================================
 Author	    : Anton Antonov
 Version	: 1.0
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
package com.rhomobile.rhodes;

import java.io.IOException;
import java.io.InputStream;

import com.rho.NetworkAccess;
import com.rho.RhoRuby;
import com.rho.sync.SyncEngine;
import com.rhomobile.rhodes.ui.AboutDialog;
import com.xruby.runtime.builtin.IRubyPlatformUtils;
import com.xruby.runtime.builtin.RubyPlatformUtils;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.Window;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class Rhodes extends Activity implements IRubyPlatformUtils {

	private static final int ACTIVITY_CREATE = 0;
	private static final String LOG_TAG = "Rhodes";
	private static final String HOME_URL = "http://127.0.0.1:8080";

	public static final int HTTP_SERVER_STARTED = 1;

	private static final int MAX_PROGRESS = 10000;
	
	private WebView webView;

	private String startPage = HOME_URL + "/";

	private boolean isStarted = false;
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		this.requestWindowFeature(Window.FEATURE_PROGRESS);
		
		setContentView(R.layout.main);

		webView = (WebView) findViewById(R.id.webview);

		WebSettings webSettings = webView.getSettings();
		webSettings.setSavePassword(false);
		webSettings.setSaveFormData(false);
		webSettings.setJavaScriptEnabled(true);
		webSettings.setJavaScriptCanOpenWindowsAutomatically(false);
		webSettings.setSupportZoom(false);
		webSettings.setCacheMode(WebSettings.LOAD_NO_CACHE);
		webSettings.setSupportMultipleWindows(false);
		//webSettings.setLoadsImagesAutomatically(true);

		webView.setVerticalScrollBarEnabled(true);
		webView.setHorizontalScrollBarEnabled(true);

		webView.setWebViewClient(new WebViewClient() {
			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {

				view.loadUrl(url);
				// return true to handle the click yourself
				return true;
			}

			@Override
			public void onPageFinished(WebView view, String url) {
				onStopLoading();
				super.onPageFinished(view, url);
			}

			@Override
			public void onPageStarted(WebView view, String url, Bitmap favicon) {
				
				onStartLoading();
				super.onPageStarted(view, url, favicon);
			}
			
			
		});
		
		webView.setWebChromeClient(new WebChromeClient() {

			@Override
			public void onProgressChanged(WebView view, int newProgress) {
				
				onLoadingProgress(newProgress*100);
				super.onProgressChanged(view, newProgress);
			}
			
		});
		
		Log.i(LOG_TAG, "Loading...");
		webView.loadUrl("file:///android_asset/apps/loading.html");

		RubyPlatformUtils.setRubyPlatformUtilsImpl(this);
		NetworkAccess.setNetworkAccessImpl(new NetworkAccessImpl(
				getBaseContext()));

		RhodesInstance.setInstance(this);

		// start http server
		startService(new Intent(this, RhoHttpService.class));
	}
	
	protected void onStartLoading()
	{
		if ( isStarted )
			this.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, 0 );
	}
	
	protected void onStopLoading()
	{
		if ( isStarted )
			this.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, MAX_PROGRESS );
	}

	protected void onLoadingProgress(int curProgress)
	{
		if ( isStarted )
		{
			if ( curProgress < 0 )
				curProgress = 0;
			if ( curProgress > 10000)
				curProgress = 10000;
			
			this.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, curProgress );
		}
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();

		Intent svcSync = new Intent(this, RhoSyncService.class);
		stopService(svcSync);

		Intent svc = new Intent(this, RhoHttpService.class);
		stopService(svc);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		MenuInflater mi = new MenuInflater(getApplication());
		mi.inflate(R.menu.options, menu);
		return true;
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		switch (keyCode) {
		case KeyEvent.KEYCODE_BACK:
			this.webView.goBack();
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.about:
			Intent i = new Intent(this, AboutDialog.class);
			startActivityForResult(i, ACTIVITY_CREATE);

			return true;
		case R.id.navigation_back:
			this.webView.goBack();
			return true;

		case R.id.navigation_forward:
			this.webView.goForward();
			return true;

		case R.id.navigation_home:
			this.webView.loadUrl(startPage);
			return true;

		case R.id.sync:
			SyncEngine.wakeUp();
			return true;
		}

		return false;
	}

	public InputStream loadFile(String path, String mode) {

		// load resource from assets
		try {
			if (path.startsWith("/")) {
				return this.getAssets().open(path.substring(1));
			} else {
				return this.getAssets().open(path);
			}
		} catch (IOException e) {
			Log.w(LOG_TAG, e.getMessage());
		}

		return null;
	}

	public String getPlatform() {
		return "Android";
	}

	public void startSyncEngine() {
		Log.d(LOG_TAG, "startSyncEngine...");
		// start sync engine
		startService(new Intent(this, RhoSyncService.class));

		isStarted = true;
		
		// load start page
		startPage = HOME_URL + RhoRuby.getStartPage();
		this.webView.loadUrl(startPage);
	}
}