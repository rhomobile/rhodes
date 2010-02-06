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
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Hashtable;
import java.util.Locale;

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
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.graphics.Bitmap;
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
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.os.Process;

public class Rhodes extends Activity {

	private static final String TAG = "Rhodes";
	
	private static final boolean SHOW_PROGRESS_BAR = false;
	private static final int MAX_PROGRESS = 10000;
	
	private static final boolean DB_ON_SDCARD = true;
	private static final int DB_BACKUP_INTERVAL = 60000;
	
	public static final int WINDOW_FLAGS = WindowManager.LayoutParams.FLAG_FULLSCREEN;
	public static final int WINDOW_MASK = WindowManager.LayoutParams.FLAG_FULLSCREEN;
	
	public final Handler uiHandler = new Handler();
	
	private static int screenWidth;
	private static int screenHeight;

	private WebView webView;

	private String sdCardError = "Application can not access the SD card while it's mounted. Please unmount the device and stop the adb server before launching the app.";
		
	private String rootPath = null;
	
	private native void setRootPath(String path);
	
	private String phoneMemoryRootPath() {
		return "/data/data/" + getPackageName() + "/data/";
	}
	
	private String sdcardRootPath() {
		return "/sdcard/rhomobile/" + getPackageName() + "/";
	}
	
	private void initRootPath() {
		rootPath = phoneMemoryRootPath();
		setRootPath(rootPath);
	}
	
	public String getRootPath() {
		return rootPath;
	}
	
	public native void startRhodesApp();
	public native void stopRhodesApp();
	
	private native void doSyncAllSources(boolean v);
	
	public native String getOptionsUrl();
	public native String getStartUrl();
	public native String getCurrentUrl();
	
	private native void dblock();
	private native void dbunlock();
	
	public native static void makeLink(String src, String dst);
	
	private RhoLogConf m_rhoLogConf = new RhoLogConf();
	public RhoLogConf getLogConf() {
		return m_rhoLogConf;
	}
	
	private boolean deleteRecursively(File target) {
		if (target.isDirectory()) {
			String[] children = target.list();
			for(int i = 0; i != children.length; ++i)
				if (!deleteRecursively(new File(target, children[i])))
					return false;
		}
		return target.delete();
	}
	
	
	private Hashtable<String, UriHandler> uriHandlers = new Hashtable<String, UriHandler>();
	
	private class FileSource {
		
		AssetManager amgr;
		
		public FileSource() {
			amgr = null;
		}
		
		public FileSource(AssetManager a) {
			amgr = a;
		}
		
		String[] list(String dir) throws IOException {
			if (amgr != null)
				return amgr.list(dir);
			return new File(dir).list();
		}
		
		InputStream open(String file) throws FileNotFoundException, IOException {
			return amgr != null ? amgr.open(file) : new FileInputStream(file);
		}
	};
	
	private void copyFromBundle(File source, File target, boolean remove) throws IOException {
		copyFromBundle(new FileSource(), source.getAbsolutePath(), target, remove);
	}
	
	private void copyFromBundle(FileSource fs, String source, File target, boolean remove) throws IOException
	{
		if (remove && target.exists() && !deleteRecursively(target))
			throw new IOException("Can not delete " + target.getAbsolutePath());
		
		String[] children = fs.list(source);
		if (children != null && children.length > 0) {
			if (!target.exists())
				target.mkdirs();
			
			for(int i = 0; i != children.length; ++i)
				copyFromBundle(fs, source + "/" + children[i], new File(target, children[i]), false);
		}
		else {
			InputStream in = null;
			OutputStream out = null;
			try {
				in = fs.open(source);
				out = new FileOutputStream(target);
				
				byte[] buf = new byte[1024];
				int len;
				while((len = in.read(buf)) > 0)
					out.write(buf, 0, len);
				
			}
			catch (FileNotFoundException e) {
				if (in != null)
					throw e;
				
				target.createNewFile();
			}
			finally {
				if (in != null)
					in.close();
				if (out != null)
					out.close();
			}
		}
	}
	
	private String getContent(InputStream in) throws IOException {
		String retval = "";
		byte[] buf = new byte[512];
		while(true) {
			int n = in.read(buf);
			if (n <= 0)
				break;
			retval += new String(buf);
		}
		return retval;
	}
	
	private boolean isContentsEquals(String bundleFile, File sdcardFile) throws IOException {
		AssetManager amgr = getResources().getAssets();
		InputStream bundleIn = null;
		InputStream sdcardIn = null;
		try {
			bundleIn = amgr.open(bundleFile);
			sdcardIn = new FileInputStream(sdcardFile);
			
			String newName = getContent(bundleIn);
			String oldName = getContent(sdcardIn);
			return newName.equals(oldName);
		} catch (Exception e) {
			return false;
		}
		finally {
			if (bundleIn != null) bundleIn.close();
			if (sdcardIn != null) sdcardIn.close();
		}
	}
	
	private void checkSDCard() {
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
			return;
		}
		Log.d(TAG, "SD card check passed, going on");
	}
	
	private void copyFilesFromBundle() {
		try {
			String phRootPath = phoneMemoryRootPath();
			String sdRootPath = sdcardRootPath();
			
			boolean removeFiles = true;
			boolean copyFiles = true;
			
			removeFiles = !isContentsEquals("name", new File(sdRootPath, "name"));
			if (!removeFiles)
				copyFiles = !isContentsEquals("hash", new File(sdRootPath, "hash"));
			
			if (copyFiles) {
				Log.d(TAG, "Copying required files from bundle to sdcard");
				
				File phrf = new File(phRootPath);
				if (!phrf.exists())
					phrf.mkdirs();
				phrf = null;
				
				FileSource fs = new FileSource(getResources().getAssets());
				
				String items[] = {"apps", "lib", "db", "RhoLog.txt", "RhoLog.txt_pos", "hash", "name"};
				for (int i = 0; i != items.length; ++i) {
					String item = items[i];
					File phf = new File(phRootPath, item);
					File sdf = new File(sdRootPath, item);
					Log.d(TAG, "Copy '" + item + "' to '" + sdRootPath + "'");
					copyFromBundle(fs, item, sdf, removeFiles);
					if (DB_ON_SDCARD || !item.equals("db")) {
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
			
			if (!DB_ON_SDCARD) {
				// Load DB from SD card to phone memory
				File phdb = new File(phRootPath, "db");
				File sddb = new File(sdRootPath, "db");
				Log.d(TAG, "Load DB to memory");
				copyFromBundle(sddb, phdb, false);
			}
		} catch (IOException e) {
			Log.e(TAG, e.getMessage(), e);
			return;
		}
	}
	
	private void flushDb() throws IOException {
		// Store DB from phone memory to SD card
		Log.d(TAG, "Store DB to SD card");
		File phdb = new File(phoneMemoryRootPath(), "db");
		File sddb = new File(sdcardRootPath(), "db");
		copyFromBundle(phdb, sddb, true);
	}
	
	private boolean handleUrlLoading(String url) {
		Uri uri = Uri.parse(url);
		UriHandler handler = uriHandlers.get(uri.getScheme());
		if (handler == null)
			return false;
		
		handler.handle(uri);
		return true;
	}

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		getWindow().setFlags(WINDOW_FLAGS, WINDOW_MASK);

		this.requestWindowFeature(Window.FEATURE_PROGRESS);

		setContentView(AndroidR.layout.main);
		
		WindowManager wm = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
		Display d = wm.getDefaultDisplay();
		screenHeight = d.getHeight();
		screenWidth = d.getWidth();

		webView = (WebView) findViewById(AndroidR.id.webview);
		
		// Register custom uri handlers here
		uriHandlers.put("mailto", new MailUriHandler(this));
		uriHandlers.put("tel", new TelUriHandler(this));

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
			
			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {
				return handleUrlLoading(url);
			}

		});

		webView.setWebChromeClient(new WebChromeClient() {

			@Override
			public void onProgressChanged(WebView view, int newProgress) {

				onLoadingProgress(newProgress * 100);
				super.onProgressChanged(view, newProgress);
			}

		});
		
		Log.i("Rhodes", "Loading...");
		webView.loadUrl("file:///android_asset/apps/app/loading.html");
		
		RhodesInstance.setInstance(this);
		
		// WARNING!!! This function MUST be called in context of UI thread
		// to be correctly initialized
		// Do not remove this line!!!
		initRootPath();
		
		Thread init = new Thread(new Runnable() {

			public void run() {
				checkSDCard();
				copyFilesFromBundle();
				startRhodesApp();
				
				if (!DB_ON_SDCARD) {
					Thread flush = new Thread(new Runnable() {
	
						public void run() {
							for(;;) {
								try {
									Thread.sleep(DB_BACKUP_INTERVAL);
								} catch (InterruptedException e) {}
								
								dblock();
								try {
									flushDb();
								} catch (IOException e) {
									Logger.E(TAG, e.getMessage());
								}
								finally {
									dbunlock();
								}
							}
						}
						
					});
					flush.start();
				}
			}
			
		});
		init.start();
	}

	protected void onStartLoading() {
		if (!SHOW_PROGRESS_BAR)
			return;
		this.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, 0);
	}

	protected void onStopLoading() {
		if (!SHOW_PROGRESS_BAR)
			return;
		this.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, MAX_PROGRESS);
	}

	protected void onLoadingProgress(int curProgress) {
		if (!SHOW_PROGRESS_BAR)
			return;
		
		if (curProgress < 0)
			curProgress = 0;
		if (curProgress > 10000)
			curProgress = 10000;

		this.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, curProgress);
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
	
	private static class WebviewNavigate implements Runnable {
		private WebView webView;
		private String url;
		
		public WebviewNavigate(WebView w, String u) {
			webView = w;
			url = u;
		}
		public void run() {
			webView.loadUrl(url);
		}
	};
	
	private static class WebviewReload implements Runnable {
		private WebView webView;
		
		public WebviewReload(WebView w) {
			webView = w;
		}
		
		public void run() {
			webView.reload();
		}
	};
	
	public void webview_navigate(String url) {
		if (handleUrlLoading(url))
			return;
		uiHandler.post(new WebviewNavigate(webView, url));
	}
	
	public void webview_refresh() {
		uiHandler.post(new WebviewReload(webView));
	}
	
	public String webview_currentLocation() {
		return this.webView.getUrl();
	}

	public String webview_executeJs(String js) {
		webview_navigate("javascript:" + js);
		return "";
	}
	
	public static void deleteFilesInFolder(String folder) {
		String[] children = new File(folder).list();
		for (int i = 0; i != children.length; ++i)
			delete(new File(folder, children[i]));
	}
	
	public static void delete(File f) {
		if (f.isDirectory()) {
			String[] children = f.list();
			for (int i = 0; i != children.length; ++i)
				delete(new File(f, children[i]));
		}
		f.delete();
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
		if (!DB_ON_SDCARD) {
			try {
				flushDb();
				deleteRecursively(new File(phoneMemoryRootPath(), "db"));
			} catch (IOException e) {
				Logger.E(TAG, e.getMessage());
			}
		}
		Process.killProcess(Process.myPid());
	}
	
	static {
		// Load native implementation of rhodes
		System.loadLibrary("rhodes");
	}
}
