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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import com.rhomobile.rhodes.ui.AboutDialog;
import com.rhomobile.rhodes.ui.LogOptionsDialog;
import com.rhomobile.rhodes.ui.LogViewDialog;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.os.Process;

public class Rhodes extends Activity {

	private static final String LOG_TAG = "Rhodes";

	private static final int MAX_PROGRESS = 10000;

	private WebView webView;

	private boolean isStarted = false;

	private String sdCardError = "Application can not access the SD card while it's mounted. Please unmount the device and stop the adb server before launching the app.";
	
	public static String getRootPath() {
		// TODO:
		return "/sdcard/rhomobile/Rhodes/";
	}

	public native void startRhodesApp();
	public native void stopRhodesApp();
	
	private native void saveCurrentLocation(String url);
    private native boolean restoreLocation();
    
    private native void doSyncAllSources(boolean v);
    
    public native String getOptionsUrl();
    public native String getStartUrl();
    public native String getCurrentUrl();
    
    private RhoLogConf m_rhoLogConf = new RhoLogConf();
    public RhoLogConf getLogConf() {
    	return m_rhoLogConf;
    }
    
    private void copyFromBundle(AssetManager amgr, String source, File target) throws IOException
    {
    	String[] children = amgr.list(source);
    	if (children.length > 0) {
    		if (!target.exists())
    			target.mkdirs();
    		
    		for(int i = 0; i != children.length; ++i)
    			copyFromBundle(amgr, source + "/" + children[i], new File(target, children[i]));
    	}
    	else {
    		InputStream in = null;
    		OutputStream out = null;
    		try {
    			in = amgr.open(source);
    			out = new FileOutputStream(target);
	    		
	    		byte[] buf = new byte[1024];
	    		int len;
	    		while((len = in.read(buf)) > 0)
	    			out.write(buf, 0, len);
	    		
    		}
    		finally {
    			if (in != null)
    				in.close();
    			if (out != null)
    				out.close();
    		}
    	}
    }

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		// Load native implementation of rhodes
		System.loadLibrary("rhodes");
		
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		this.requestWindowFeature(Window.FEATURE_PROGRESS);

		setContentView(AndroidR.layout.main);

		webView = (WebView) findViewById(AndroidR.id.webview);

		WebSettings webSettings = webView.getSettings();
		webSettings.setSavePassword(false);
		webSettings.setSaveFormData(false);
		webSettings.setJavaScriptEnabled(true);
		webSettings.setJavaScriptCanOpenWindowsAutomatically(false);
		webSettings.setSupportZoom(false);
		webSettings.setCacheMode(WebSettings.LOAD_NO_CACHE);
		webSettings.setSupportMultipleWindows(false);
		// webSettings.setLoadsImagesAutomatically(true);

		webView.setVerticalScrollBarEnabled(true);
		webView.setHorizontalScrollBarEnabled(true);

		webView.setWebViewClient(new WebViewClient() {
			/*
			 * @Override public boolean shouldOverrideUrlLoading(WebView view,
			 * String url) {
			 * 
			 * view.loadUrl(url); // return true to handle the click yourself
			 * return true; }
			 */

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

				onLoadingProgress(newProgress * 100);
				super.onProgressChanged(view, newProgress);
			}

		});
		
		Log.i(LOG_TAG, "Loading...");
		webView.loadUrl("file:///android_asset/apps/loading.html");
		
		Log.d(this.getClass().getSimpleName(), "Check if the SD card is mounted...");
		String state = Environment.getExternalStorageState();
		Log.d(this.getClass().getSimpleName(), "Storage state: " + state);
		if(!Environment.MEDIA_MOUNTED.equals(state)) {
			new AlertDialog.Builder(this)
				.setTitle("SD card error")
				.setMessage(sdCardError)
				.setCancelable(false)
				.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						Log.e(this.getClass().getSimpleName(), "Exit - SD card is not accessible");
						stopSelf();
					}					
				})
				.create()
				.show();
			return;
		}
		Log.d(this.getClass().getSimpleName(), "SD card check passed, going on");
		
		try {
			Log.d(this.getClass().getSimpleName(), "Copying required files from bundle to sdcard");
			String rootPath = getRootPath();
			AssetManager amgr = getResources().getAssets();
			copyFromBundle(amgr, "apps", new File(rootPath, "apps"));
			copyFromBundle(amgr, "db", new File(rootPath, "db"));
			copyFromBundle(amgr, "lib", new File(rootPath, "lib"));
			File dbfiles = new File(rootPath + "apps/public/db-files");
			if (!dbfiles.exists())
				dbfiles.mkdirs();
			Log.d(this.getClass().getSimpleName(), "All files copied");
		} catch (IOException e) {
			Log.e(this.getClass().getSimpleName(), e.getMessage(), e);
			return;
		}
		
		RhodesInstance.setInstance(this);

		startRhodesApp();
	}

	protected void onStartLoading() {
		if (isStarted)
			this.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, 0);
	}

	protected void onStopLoading() {
		if (isStarted)
			this.getWindow().setFeatureInt(Window.FEATURE_PROGRESS,
					MAX_PROGRESS);
	}

	protected void onLoadingProgress(int curProgress) {
		if (isStarted) {
			if (curProgress < 0)
				curProgress = 0;
			if (curProgress > 10000)
				curProgress = 10000;

			this.getWindow()
					.setFeatureInt(Window.FEATURE_PROGRESS, curProgress);
		}
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}
	
	@Override
	protected void onPause() {
		super.onPause();
		
		saveCurrentLocation(getCurrentUrl());
		
		/*
		GeoLocation.stop();
		RingtoneManager.stop();
		*/
	}
	
	@Override
	protected void onResume() {
		restoreLocation();
		
		super.onResume();
	}
	
	@Override
	protected void onDestroy() {
		//stopServices();
		
		super.onDestroy();
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		MenuInflater mi = new MenuInflater(getApplication());
		mi.inflate(AndroidR.menu.options, menu);
		return true;
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		switch (keyCode) {
		case KeyEvent.KEYCODE_BACK:
			this.webView.goBack();
			return true;
		case KeyEvent.KEYCODE_HOME:
			stopSelf();
			return true;
		}
		
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case AndroidR.id.about:
			AboutDialog aboutDialog = new AboutDialog(this);
			aboutDialog.setTitle("About");
			aboutDialog.setCanceledOnTouchOutside(true);
			aboutDialog.setCancelable(true);
			aboutDialog.show();

			return true;
		case AndroidR.id.navigation_back:
			this.webView.goBack();
			return true;

		case AndroidR.id.navigation_forward:
			this.webView.goForward();
			return true;

		case AndroidR.id.navigation_home:
			this.webView.loadUrl(getStartUrl());
			return true;

		case AndroidR.id.sync:
			doSyncAllSources(true);
			return true;
			
		case AndroidR.id.logview:
			this.runOnUiThread(new Runnable() {

				public void run() {
					final LogViewDialog logViewDialog = new LogViewDialog(RhodesInstance.getInstance());
					logViewDialog.setTitle("Log View");
					logViewDialog.setCancelable(true);
					logViewDialog.show();
				}
		    	
		    });
			
			return true;
			
		case AndroidR.id.logoptions:
			this.runOnUiThread(new Runnable() {

				public void run() {
					final LogOptionsDialog logOptionsDialog = new LogOptionsDialog(RhodesInstance.getInstance());
					logOptionsDialog.setTitle("Logging Options");
					logOptionsDialog.setCancelable(true);
					logOptionsDialog.show();
				}
				
		    });

		return true;	

		case AndroidR.id.exit:
			stopSelf();
			return true;

		case AndroidR.id.options:
			String curUrl = getOptionsUrl();
			this.webView.loadUrl(curUrl);
			return true;

		case AndroidR.id.refresh:
			webView.reload();
			return true;
		}

		return false;
	}
	
	//private void stopServices() {
	//	stopService(new Intent(this, RhoSyncService.class));
	//	stopService(new Intent(this, RhoHttpService.class));
	//}
	
	public void stopSelf() {
		//stopServices();
		Process.killProcess(Process.myPid());
	}
}