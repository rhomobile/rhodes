/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
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
