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
import java.io.IOException;
import java.util.Hashtable;
import java.util.Locale;

import com.rhomobile.rhodes.Utils.AssetsSource;
import com.rhomobile.rhodes.Utils.FileSource;
import com.rhomobile.rhodes.ui.AboutDialog;
import com.rhomobile.rhodes.ui.LogOptionsDialog;
import com.rhomobile.rhodes.ui.LogViewDialog;
import com.rhomobile.rhodes.uri.MailUriHandler;
import com.rhomobile.rhodes.uri.TelUriHandler;
import com.rhomobile.rhodes.uri.UriHandler;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.Configuration;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.Log;
import android.view.Display;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.view.ViewGroup.LayoutParams;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;
import android.os.Process;

public class Rhodes extends Activity {

	private static final String TAG = "Rhodes";
	
	public static final String INTENT_EXTRA_PREFIX = "com.rhomobile.rhodes.";
	
	public static final int WINDOW_FLAGS = WindowManager.LayoutParams.FLAG_FULLSCREEN;
	public static final int WINDOW_MASK = WindowManager.LayoutParams.FLAG_FULLSCREEN;
	
	private long uiThreadId;
	public long getUiThreadId() {
		return uiThreadId;
	}
	
	private final Handler uiHandler = new Handler();
	
	private static int screenWidth;
	private static int screenHeight;

	private FrameLayout outerFrame;
	private MainView mainView;
	
	private Hashtable<String, UriHandler> uriHandlers = new Hashtable<String, UriHandler>();

	private String sdCardError = "Application can not access the SD card while it's mounted. Please unmount the device and stop the adb server before launching the app.";
		
	private String rootPath = null;
	
	private native void setRootPath(String path);
	
	public native void startRhodesApp();
	public native void stopRhodesApp();
	
	private native void doSyncAllSources(boolean v);
	
	public native String getOptionsUrl();
	public native String getStartUrl();
	public native String getCurrentUrl();
	
	public native String normalizeUrl(String url);
	
	public native static void makeLink(String src, String dst);
	
	private void initRootPath() {
		rootPath = phoneMemoryRootPath();
		setRootPath(rootPath);
	}
	
	public String getRootPath() {
		return rootPath;
	}
	
	private String phoneMemoryRootPath() {
		return "/data/data/" + getPackageName() + "/data/";
	}
	
	private String sdcardRootPath() {
		return "/sdcard/rhomobile/" + getPackageName() + "/";
	}
	
	private RhoLogConf m_rhoLogConf = new RhoLogConf();
	public RhoLogConf getLogConf() {
		return m_rhoLogConf;
	}
	
	private boolean checkSDCard() {
		Log.d(TAG, "Check if the SD card is mounted...");
		String state = Environment.getExternalStorageState();
		Log.d(TAG, "Storage state: " + state);
		if(!Environment.MEDIA_MOUNTED.equals(state)) {
			new AlertDialog.Builder(this)
				.setTitle("SD card error")
				.setMessage(sdCardError)
				.setCancelable(false)
				.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						Log.e(this.getClass().getSimpleName(), "Exit - SD card is not accessible");
						Process.killProcess(Process.myPid());
					}					
				})
				.create()
				.show();
			return false;
		}
		Log.d(TAG, "SD card check passed, going on");
		return true;
	}
	
	private void copyFilesFromBundle() {
		try {
			String phRootPath = phoneMemoryRootPath();
			String sdRootPath = sdcardRootPath();
			
			boolean nameChanged = false;
			boolean contentChanged = true;
			
			FileSource as = new AssetsSource(getResources().getAssets());
			FileSource fs = new FileSource();
			
			nameChanged = !Utils.isContentsEquals(as, "name", fs, new File(sdRootPath, "name").getPath());
			if (nameChanged)
				contentChanged = true;
			else
				contentChanged = !Utils.isContentsEquals(as, "hash", fs, new File(sdRootPath, "hash").getPath());
			
			if (contentChanged) {
				Log.d(TAG, "Copying required files from bundle to sdcard");
				
				File phrf = new File(phRootPath);
				if (!phrf.exists())
					phrf.mkdirs();
				phrf = null;
				
				String items[] = {"apps", "lib", "db", "RhoLog.txt", "RhoLog.txt_pos", "hash", "name"};
				for (int i = 0; i != items.length; ++i) {
					String item = items[i];
					File phf = new File(phRootPath, item);
					File sdf = new File(sdRootPath, item);
					Log.d(TAG, "Copy '" + item + "' to '" + sdRootPath + "'");
					Utils.copyRecursively(as, item, sdf, nameChanged);
					if (!item.equals("db")) {
						Log.d(TAG, "Make symlink from '" + sdf.getAbsolutePath() + "' to '" +
								phf.getAbsolutePath() + "'");
						makeLink(sdf.getAbsolutePath(), phf.getAbsolutePath());
					}
				}
				
				File dbfiles = new File(rootPath + "apps/public/db-files");
				if (!dbfiles.exists())
					dbfiles.mkdirs();
				dbfiles = null;
				
				Log.d(TAG, "All files copied");
			}
			else
				Log.d(TAG, "No need to copy files to SD card");
		} catch (IOException e) {
			Log.e(TAG, e.getMessage(), e);
			return;
		}
	}
	
	private boolean handleUrlLoading(String url) {
		Uri uri = Uri.parse(url);
		UriHandler handler = uriHandlers.get(uri.getScheme());
		if (handler == null)
			return false;
		
		handler.handle(uri);
		return true;
	}
	
	public WebView createWebView() {
		WebView w = new WebView(getApplicationContext());
		
		w.loadUrl("file:///android_asset/apps/app/loading.html");
		
		WebSettings webSettings = w.getSettings();
		webSettings.setSavePassword(false);
		webSettings.setSaveFormData(false);
		webSettings.setJavaScriptEnabled(true);
		webSettings.setJavaScriptCanOpenWindowsAutomatically(false);
		webSettings.setSupportZoom(false);
		webSettings.setCacheMode(WebSettings.LOAD_NO_CACHE);
		webSettings.setSupportMultipleWindows(false);
		// webSettings.setLoadsImagesAutomatically(true);

		w.setVerticalScrollBarEnabled(true);
		w.setHorizontalScrollBarEnabled(true);

		w.setWebViewClient(new WebViewClient() {

			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {
				return handleUrlLoading(url);
			}

		});

		return w;
	}
	
	public void setMainView(MainView v) {
		outerFrame.removeAllViews();
		mainView = v;
		outerFrame.addView(mainView.getView());
	}
	
	public MainView getMainView() {
		return mainView;
	}
	
	private static class PerformOnUiThread implements Runnable {
		
		private Runnable runnable;
		
		public PerformOnUiThread(Runnable r) {
			runnable = r;
		}
		
		public void run() {
			try {
				runnable.run();
			}
			catch (Exception e) {
				Logger.E("Rhodes", "PerformOnUiThread failed: " + e.getMessage());
			}
			finally {
				synchronized (runnable) {
					runnable.notify();
				}
			}
		}
	};
	
	public static void performOnUiThread(Runnable r) {
		try {
			long thrId = Thread.currentThread().getId();
			Rhodes rhodes = RhodesInstance.getInstance();
			if (rhodes.getUiThreadId() == thrId) {
				// We are already in UI thread
				r.run();
			}
			else {
				// Post request to UI thread and wait while it would be done
				synchronized (r) {
					rhodes.uiHandler.post(new PerformOnUiThread(r));
					r.wait();
				}
			}
		}
		catch (Exception e) {
			Logger.E("Rhodes", "performOnUiThread failed: " + e.getMessage());
		}
	}
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		uiThreadId = Thread.currentThread().getId();
		RhodesInstance.setInstance(this);
		
		getWindow().setFlags(WINDOW_FLAGS, WINDOW_MASK);

		this.requestWindowFeature(Window.FEATURE_PROGRESS);

		outerFrame = new FrameLayout(this);
		this.setContentView(outerFrame, new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
		
		setMainView(new SimpleMainView());
		
		Log.i("Rhodes", "Loading...");
		
		WindowManager wm = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
		Display d = wm.getDefaultDisplay();
		screenHeight = d.getHeight();
		screenWidth = d.getWidth();
		
		// Register custom uri handlers here
		UriHandler[] handlers = {
				new MailUriHandler(this),
				new TelUriHandler(this)
		};
		for (int i = 0; i < handlers.length; ++i)
			uriHandlers.put(handlers[i].scheme(), handlers[i]);
		
		// WARNING!!! This function MUST be called in context of UI thread
		// to be correctly initialized
		// Do not remove this line!!!
		initRootPath();
		
		Thread init = new Thread(new Runnable() {

			public void run() {
				if (!checkSDCard())
					return;
				copyFilesFromBundle();
				startRhodesApp();
			}
			
		});
		init.start();
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}
	
	@Override
	protected void onPause() {
		super.onPause();
		
		saveCurrentLocation(getCurrentUrl());
	}
	
	@Override
	protected void onResume() {
		restoreLocation();
		
		super.onResume();
	}
	
	@Override
	protected void onDestroy() {
		stopSelf();
		
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
			mainView.back(mainView.activeTab());
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
			mainView.back(mainView.activeTab());
			return true;

		case AndroidR.id.navigation_forward:
			mainView.forward(mainView.activeTab());
			return true;

		case AndroidR.id.navigation_home:
			mainView.navigate(getStartUrl(), mainView.activeTab());
			return true;

		case AndroidR.id.sync:
			doSyncAllSources(true);
			return true;
			
		case AndroidR.id.logview:
			performOnUiThread(new Runnable() {

				public void run() {
					final LogViewDialog logViewDialog = new LogViewDialog(RhodesInstance.getInstance());
					logViewDialog.setTitle("Log View");
					logViewDialog.setCancelable(true);
					logViewDialog.show();
				}
				
			});
			
			return true;
			
		case AndroidR.id.logoptions:
			performOnUiThread(new Runnable() {

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
			mainView.navigate(curUrl, mainView.activeTab());
			return true;

		case AndroidR.id.refresh:
			mainView.reload(mainView.activeTab());
			return true;
		}

		return false;
	}
	
	// Called from native code
	public static void deleteFilesInFolder(String folder) {
		String[] children = new File(folder).list();
		for (int i = 0; i != children.length; ++i)
			Utils.deleteRecursively(new File(folder, children[i]));
	}
		
	public static boolean hasNetwork() {
		// TODO:
		return true;
	}
	
	public static void showNetworkIndicator(boolean v) {
		// TODO:
	}
	
	public static String getCurrentLocale() {
		String locale = Locale.getDefault().getLanguage();
		if (locale.length() == 0)
			locale = "en";
		return locale;
	}
	
	public static int getScreenWidth() {
		return screenWidth;
	}
	
	public static int getScreenHeight() {
		return screenHeight;
	}
	
	private void saveCurrentLocation(String url) {
		// TODO:
	}
	
	private boolean restoreLocation() {
		// TODO:
		return false;
	}
	
	public void stopSelf() {
		stopRhodesApp();
		Process.killProcess(Process.myPid());
	}
	
	static {
		// Load native implementation of rhodes
		System.loadLibrary("rhodes");
	}
}
