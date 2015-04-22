package com.rhomobile.rhodes.webview;

import android.webkit.WebSettings;
import android.webkit.WebView;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class WebSettingsProviderKitKat extends
        WebSettingsProviderJellyBean implements IWebSettingsProvider {

    @Override
    public void fillSettings(WebSettings settings, IRhoConfig config) {
        super.fillSettings(settings, config);
       PerformOnUiThread.exec(new Runnable(){

				@Override
				public void run() {
					                if (Capabilities.DEBUG_ENABLED) {
				                                                                WebView.setWebContentsDebuggingEnabled(true);
				                                                        }
					
				                  }
				
			});
    }

}
