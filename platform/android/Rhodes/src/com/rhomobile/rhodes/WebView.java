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

package com.rhomobile.rhodes;

import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.net.Uri;
import android.webkit.CookieManager;

public class WebView {
	
	private static final String TAG = "WebView";
	
	///*** NavigateTask ***
	///
	private static class NavigateTask implements Runnable {
		private String url;
		private int index;
		
		public NavigateTask(String u, int i) {
			url = u;
			index = i;
		}
		
		public void run() {
			RhodesService r = RhodesService.getInstance();
			r.getMainView().navigate(url, index);
		}
	};
	
	///*** NavigateBackTask ***
	///
	private static class NavigateBackTask implements Runnable {
		public NavigateBackTask(int index) {
			mIndex = index;
		}
		public void run() {
			RhodesService r = RhodesService.getInstance();
			r.getMainView().back(mIndex);
		}
		private int mIndex;
	};
	
    ///*** NavigateForwardTask ***
    ///
    private static class NavigateForwardTask implements Runnable {
        public NavigateForwardTask(int index) {
            mIndex = index;
        }
        public void run() {
            RhodesService r = RhodesService.getInstance();
            r.getMainView().forward(mIndex);
        }
        private int mIndex;
    };
    
	///*** RefreshTask ***
	///
	private static class RefreshTask implements Runnable {
		private int index;
		
		public RefreshTask(int i) {
			index = i;
		}
		
		public void run() {
			RhodesService r = RhodesService.getInstance();
			r.getMainView().reload(index);
		}
	};
	
	///*** SetCookieTask ***
	///
	private static class SetCookieTask implements Runnable {
		private String url;
		private String cookie;
		
		public SetCookieTask(String u, String c) {
			Uri uri = Uri.parse(u);
			url = uri.getScheme() + "://" + uri.getHost() + "/" + uri.getPath();
			cookie = c;
		}
		
		public void run() {
			CookieManager mgr = CookieManager.getInstance();
			mgr.setCookie(url, cookie);
		}
	};

    ///*** StopNavigateTask ***
    ///
    private static class StopNavigateTask implements Runnable {
        private int index;
        
        public StopNavigateTask(int i) {
            index = i;
        }
        
        public void run() {
            RhodesService r = RhodesService.getInstance();
            r.getMainView().stopNavigate(index);
        }
    };
    
	
	public static void navigate(String url, int index) {
		try {
			PerformOnUiThread.exec(new NavigateTask(url, index));
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
	}
	
	public static void navigateBack() {
		try {
			PerformOnUiThread.exec(new NavigateBackTask(activeTab()));
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
	}
	
    public static void navigateForward() {
        try {
            PerformOnUiThread.exec(new NavigateForwardTask(activeTab()));
        }
        catch (Exception e) {
            Logger.E(TAG, e);
        }
    }
    
	public static void refresh(int index) {
		try {
			PerformOnUiThread.exec(new RefreshTask(index));
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
	}
	
	public static int activeTab() {
		try {
			RhodesService r = RhodesService.getInstance();
			return r.getMainView().activeTab();
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
		
		return 0;
	}
	
	public static void executeJs(String js, int index) {
		try {
			PerformOnUiThread.exec(new NavigateTask("javascript:" + js, index));
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
	}
	
	public static void setCookie(String url, String cookie) {
		try {
			PerformOnUiThread.exec(new SetCookieTask(url, cookie));
		}
		catch (Exception e) {
            Logger.E(TAG, e);
		}
	}

    public static void stopNavigate() {
        try {
            PerformOnUiThread.exec(new StopNavigateTask(activeTab()));
        }
        catch (Exception e) {
            Logger.E(TAG, e);
        }
    }
}
