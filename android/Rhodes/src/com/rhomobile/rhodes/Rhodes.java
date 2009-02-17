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

import com.rho.RhoRuby;
import com.rhomobile.rhodes.ui.AboutDialog;
import com.xruby.runtime.builtin.IRubyPlatformUtils;
import com.xruby.runtime.builtin.RubyPlatformUtils;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.webkit.WebSettings;
import android.webkit.WebView;

public class Rhodes extends Activity implements IRubyPlatformUtils {
	
	private static final int ACTIVITY_CREATE=0;
	private static final String LOG_TAG = "Rhodes";
	private static final String HOME_URL = "http://127.0.0.1:8080";
	
	private WebView webView;
	
	private String startPage = HOME_URL + "/";
	
	/**
	 * The invoked service
	 */
	private IService httpService;
	
	/**
	 * Service bind flag
	 */
	private boolean httpServiceBind = false;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        webView = (WebView) findViewById(R.id.webview);

        WebSettings webSettings = webView.getSettings();
        webSettings.setSavePassword(false);
        webSettings.setSaveFormData(false);
        webSettings.setJavaScriptEnabled(true);
        webSettings.setSupportZoom(false);
        webSettings.setCacheMode(WebSettings.LOAD_NO_CACHE );
        webSettings.setLoadsImagesAutomatically(true);
        
        webView.setVerticalScrollBarEnabled(true);
        webView.setHorizontalScrollBarEnabled(true);
        
        Log.i(LOG_TAG, "Loading...");
        webView.loadUrl("file:///android_asset/apps/loading.html");       
        
		RubyPlatformUtils.setRubyPlatformUtilsImpl(this);
		
        //start http server
		
		httpServiceBind = bindService(new Intent(this, RhoHttpService.class), 
    			httpServiceConnection, Context.BIND_AUTO_CREATE);
		
        startService(new Intent(this, RhoHttpService.class));
    }
    
    
    
    @Override
	protected void onDestroy() {
		super.onDestroy();
		
		Intent svcSync = new Intent(this, RhoSyncService.class);
	    stopService(svcSync);
		
	    try {
			httpService.unregisterCallback(httpServiceCallback);
		} catch (RemoteException e) {
			Log.e( LOG_TAG,  e.getMessage());
		}
	    
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
    public boolean onOptionsItemSelected(MenuItem item)
    {
    	switch ( item.getItemId() )
    	{
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
    	}
    	
    	return false;
    }
    
	public InputStream loadFile(String path, String mode) {

		//load resource from assets
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

	/**
	 * The call back message handler
	 */
	public ServiceCallbackHandler callbackHandler = new ServiceCallbackHandler(){

		@Override
		public void handleInputRecived(int value) {
		
			//Load start page and start sync thread

			startSyncEngine();
		}
	};

	/**
	 * The callback object that will return from the service
	 */
	private  ICallback httpServiceCallback = new ICallback.Stub(){

		public void valueChanged(int value) throws RemoteException {
			callbackHandler.sendInputRecieved(value);
		}
	};

	
    /**
     * The service connection inteface with our binded service
     * {@link http://code.google.com/android/reference/android/content/ServiceConnection.html}
     */
    private ServiceConnection httpServiceConnection = new ServiceConnection(){

		public void onServiceConnected(ComponentName name, IBinder service) {
			try {
				httpService = IService.Stub.asInterface(service);

				httpService.registerCallback(httpServiceCallback);
			} catch (RemoteException e) {
				e.printStackTrace();
			}
		}

		public void onServiceDisconnected(ComponentName name) {
		}
    	
    };


    void startSyncEngine()
    {
    	//start sync engine
    	startService(new Intent(this, RhoSyncService.class));
    	
    	//load start page
    	startPage = HOME_URL + RhoRuby.getStartPage();
    	this.webView.loadUrl(startPage);
    }
	
}