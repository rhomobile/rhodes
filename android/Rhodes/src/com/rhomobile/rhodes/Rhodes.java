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

import com.rhomobile.rhodes.ui.AboutDialog;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.webkit.WebSettings;
import android.webkit.WebView;

public class Rhodes extends Activity {
	
	private static final int ACTIVITY_CREATE=0;
	private static final String LOG_TAG = "Rhodes";
	
	private WebView webView;
	
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
        
        //start http server
        startService(new Intent(this, RhoHttpService.class));
    }
    
    
    
    @Override
	protected void onDestroy() {
		super.onDestroy();
		
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
    	}
    	
    	return false;
    }
    
    
}