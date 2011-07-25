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

import org.apache.http.HttpHost;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Proxy;
import android.os.Handler;
import android.os.Message;

public class NetworkStateTracker {

	public final static String HTTP_NETWORK_STATE_CHANGED_INTENT = "android.net.http.NETWORK_STATE";

	public final static String HTTP_NETWORK_STATE_UP = "up";

	final static int EVENT_DATA_STATE_CHANGED = 100;

	private Context context;
	
	private NetworkConnectivityListener connectivityListener;
	
	private NetworkInfo.State lastNetworkState = NetworkInfo.State.CONNECTED;
	
	private int currentNetworkType;
	
	private boolean networkConnected = true;

	private HttpHost proxyHost;
	
	NetworkStateTracker(Context context) {
		this.context = context;
	}
	
	public Context getContext() {
		return context;
	}

	/**
	 * register for updates
	 */
	public void enable() {
		if (this.connectivityListener == null) {
			/*
			 * Initializing the network type is really unnecessary, since as
			 * soon as we register with the NCL, we'll get a CONNECTED event for
			 * the active network, and we'll configure the HTTP proxy
			 * accordingly. However, as a fallback in case that doesn't happen
			 * for some reason, initializing to type WIFI would mean that we'd
			 * start out without a proxy. This seems better than thinking we
			 * have a proxy (which is probably private to the carrier network
			 * and therefore unreachable outside of that network) when we really
			 * shouldn't.
			 */
			this.currentNetworkType = ConnectivityManager.TYPE_WIFI;
			this.connectivityListener = new NetworkConnectivityListener();
			this.connectivityListener.registerHandler(handler,
					EVENT_DATA_STATE_CHANGED);
			this.connectivityListener.startListening(this.context);
		}
	}

	public void disable() {
		if (this.connectivityListener != null) {
			this.connectivityListener.unregisterHandler(handler);
			this.connectivityListener.stopListening();
			this.connectivityListener = null;
		}
	}

	public boolean isNetworkConnected() {
		return this.networkConnected;
	}

	private Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case EVENT_DATA_STATE_CHANGED:
				networkStateChanged();
				break;
			}
		}
	};

	int getCurrentNetworkType() {
		return this.currentNetworkType;
	}

	void networkStateChanged() {
		if (this.connectivityListener == null)
			return;

		NetworkConnectivityListener.State connectivityState = this.connectivityListener
				.getState();
		NetworkInfo info = this.connectivityListener.getNetworkInfo();
		if (info == null) {
			/**
			 * We've been seeing occasional NPEs here. I believe recent changes
			 * have made this impossible, but in the interest of being totally
			 * paranoid, check and log this here.
			 */
			HttpLog.v("NetworkStateTracker: connectivity broadcast"
					+ " has null network info - ignoring");
			return;
		}
		NetworkInfo.State state = info.getState();

		if (HttpLog.LOGV) {
			HttpLog.v("NetworkStateTracker " + info.getTypeName() + " state= "
					+ state + " last= " + this.lastNetworkState
					+ " connectivityState= " + connectivityState.toString());
		}

		boolean newConnection = state != this.lastNetworkState
				&& state == NetworkInfo.State.CONNECTED;

		if (state == NetworkInfo.State.CONNECTED) {
			this.currentNetworkType = info.getType();
			setProxyConfig();
		}

		this.lastNetworkState = state;
		if (connectivityState == NetworkConnectivityListener.State.NOT_CONNECTED) {
			setNetworkState(false);
			broadcastState(false);
		} else if (newConnection) {
			setNetworkState(true);
			broadcastState(true);
		}

	}

	protected void setNetworkState(boolean isNetworkConnected) {
		if (HttpLog.LOGV)
			HttpLog.v("RequestQueue.setNetworkState() " + isNetworkConnected);
		this.networkConnected = isNetworkConnected;
	}

	void broadcastState(boolean connected) {
		Intent intent = new Intent(HTTP_NETWORK_STATE_CHANGED_INTENT);
		intent.putExtra(HTTP_NETWORK_STATE_UP, connected);
		this.context.sendBroadcast(intent);
	}

	private synchronized void setProxyConfig() {
		if (this.currentNetworkType == ConnectivityManager.TYPE_WIFI) {
			proxyHost = null;
		} else {
			String host = Proxy.getHost(context);
			if (HttpLog.LOGV)
				HttpLog.v("RequestQueue.setProxyConfig " + host);
			if (host == null) {
				proxyHost = null;
			} else {
				proxyHost = new HttpHost(host, Proxy.getPort(context), "http");
			}
		}
	}

	public HttpHost getProxyHost() {
		return proxyHost;
	}
}
