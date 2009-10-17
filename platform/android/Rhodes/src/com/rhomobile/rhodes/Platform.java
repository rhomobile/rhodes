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

import com.rho.RhoConf;
import com.rho.RhoLogger;
import com.rho.RhoRuby;
import com.rho.location.GeoLocation;
import com.rho.sync.SyncThread;
import com.rhomobile.rhodes.http.HttpHeader;
import com.rhomobile.rhodes.http.HttpServer;
import com.rhomobile.rhodes.ui.AboutDialog;
import com.rhomobile.rhodes.ui.LogOptionsDialog;
import com.rhomobile.rhodes.ui.LogViewDialog;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
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

public class Platform extends Activity {

	private static final String LOG_TAG = "Rhodes";
	private static String HOME_URL = "http://127.0.0.1:8080";

	public static final int HTTP_SERVER_STARTED = 1;

	private static final int MAX_PROGRESS = 10000;

	private WebView webView;

	private String startPage = HOME_URL + "/";

	private boolean isStarted = false;

	private String sdCardError = "Application can not access the SD card while it's mounted. Please unmount the device and stop the adb server before launching the app.";

	//private NetworkStateTracker networkStateTracker;
	
	public String getHomeUrl() {
		return HOME_URL;
	}
	
	//private void stopServices() {
	//	stopService(new Intent(this, RhoSyncService.class));
	//	stopService(new Intent(this, RhoHttpService.class));
	//}
	
	public void stopSelf() {
		//stopServices();
		Process.killProcess(Process.myPid());
	}

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

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

		RhodesInstance.setInstance(this);
		
        try{
		    RhoLogger.InitRhoLog();
		}catch(Exception exc)
        {
            //TODO: we should stop if InitRhoLog failed
        }
		
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
		
		/*
		try {
			networkStateTracker = new NetworkStateTracker(RhodesInstance.getInstance());
		}catch (Exception e){
			Log.e( "NetworkAccessImpl", e.getMessage());
		}
		*/
		
		Log.i(LOG_TAG, "Loading...");
		webView.loadUrl("file:///android_asset/apps/loading.html");

		HttpServer.DEFAULT_PORT = HttpServer.findFreePort(); 
		
		HOME_URL = "http://127.0.0.1:" + HttpServer.DEFAULT_PORT.toString();
		startPage = HOME_URL + "/";

		// start http server
		startService(new Intent(this, RhoHttpService.class));
	}
	
	void saveCurrentLocation(String url) {
		url = url.replace(HOME_URL, "");
    	if (RhoConf.getInstance().getBool("KeepTrackOfLastVisitedPage")) {
			RhoConf.getInstance().setString("LastVisitedPage",url);
			RhoConf.getInstance().saveToFile();
			Log.d(LOG_TAG,"Saved LastVisitedPage: " + url);
		}   	
    }

    boolean restoreLocation() {
    	Log.d(LOG_TAG,"Restore Location to LastVisitedPage");
    	if (RhoConf.getInstance().getBool("KeepTrackOfLastVisitedPage")) {
			String url = RhoConf.getInstance().getString("LastVisitedPage");
			if (url.length()>0) {
				Log.d(LOG_TAG,"Navigating to LastVisitedPage: " + url);
				this.navigateUrl(url);
				return true;
			}
		} 
		return false;
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
		
		GeoLocation.stop();
		RingtoneManager.stop();
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
			this.webView.loadUrl(startPage);
			return true;

		case AndroidR.id.sync:
			SyncThread.doSyncAllSources(true);
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
			String curUrl = RhoRuby.getOptionsPage();
			curUrl = HOME_URL
					+ "/"
					+ curUrl.substring(curUrl.charAt(0) == '\\'
							|| curUrl.charAt(0) == '/' ? 1 : 0);
			this.webView.loadUrl(curUrl);
			return true;

		case AndroidR.id.refresh:
			webView.reload();
			return true;
		}

		return false;
	}

	public void startSyncEngine() {
		Log.d(LOG_TAG, "startSyncEngine...");

		//SyncEngine.setNotificationImpl(new SyncNotificationsImpl());

		// start sync engine
		startService(new Intent(this, RhoSyncService.class));

		isStarted = true;

		// load start page
		startPage = HOME_URL + RhoRuby.getStartPage();
		
		if ( !restoreLocation() )
			this.webView.loadUrl(startPage);
	}

	/*public class SyncNotificationsImpl extends SyncNotifications {
		public void performNotification(String url, String body){

    		HttpHeader headers = new HttpHeader();
    		headers.setHeader("Content-Type", "application/x-www-form-urlencoded");
    		postUrl(url, body, headers);

		}
	}*/

	String canonicalizeURL( String url ){
		if ( url == null || url.length() == 0 )
			return "";

		url.replace('\\', '/');
		if ( !url.startsWith(HOME_URL) ){
    		if ( url.charAt(0) == '/' )
    			url = HOME_URL.substring(0, HOME_URL.length()) + url;
    		else
    			url = HOME_URL + "/" + url;
		}

		return url;
    }

	void navigateUrl(String url){
		String fullUrl = canonicalizeURL(url);
        ResourceFetchThread thread = new ResourceFetchThread(fullUrl, null, null);
        thread.start();        
        this.webView.loadUrl(fullUrl);
    }

    public void postUrl(String url, String body, HttpHeader headers){
        ResourceFetchThread thread = new ResourceFetchThread(
        		canonicalizeURL(url), headers, body.getBytes());
        thread.start();                       
    }

    public void executeJs(String js) {
    	this.webView.loadUrl("javascript:" + js);
    }
	
	public String getStartPage() {
		return startPage;
	}

	public String getCurrentUrl() {
		return this.webView.getUrl();
	}
	
	public void refreshCurrentPage() {
		this.webView.reload();
	}

	public synchronized boolean isNetworkAvailable() {
		/*
		if ( networkStateTracker != null )
			return networkStateTracker.isNetworkConnected();
		else
			return true;
		*/
		return true;
	}

}