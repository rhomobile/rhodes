package com.rho.network;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;

public class Network
{
	protected static String TAG = "NetworkJava";
	private ConnectivityManager connectivityManager;
	private static Network network;
	
	private Network()
	{
		connectivityManager = (ConnectivityManager)RhodesActivity.getContext().getSystemService(Context.CONNECTIVITY_SERVICE);
	}
	
	public static Network getNetworkObject()
	{
		if(network == null) network = new Network();
		return network;
	}
	
	public static int hasNetwork()
	{
		return getNetworkObject().doHasNetwork();
	}
	
	public static int hasCellNetwork()
	{
		return getNetworkObject().doHasCellNetwork();
	}
	
	public static int hasWifiNetwork()
	{
		return getNetworkObject().doHasWifiNetwork();
	}
	
	private int doHasNetwork()
	{
		Logger.D(TAG, "doHasNetwork+");
		if (connectivityManager != null)
		{
			NetworkInfo activeNetwork = connectivityManager.getActiveNetworkInfo();
			Logger.D(TAG, "hasNetwork-");
			if (activeNetwork != null)
			{
				// "Connecting" state is probably no good so isConnectedOrConnecting is not used here
				int result = (activeNetwork.isConnected() ? 1 : 0);
				Logger.D(TAG, "doHasNetwork " + result + "-");
				return result;
			}
			Logger.D(TAG, "doHasNetwork noNet-");
			return 0; //No network at all
		}
		Logger.E(TAG, "doHasNetwork- Cannot get connectivity manager");
		return -1;
	}
	
	private int doHasCellNetwork()
	{
		Logger.D(TAG, "doHasCellNetwork+");
		if (connectivityManager != null)
		{
			NetworkInfo mobile = connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
			if (mobile != null)
			{
				// "Connecting" state is probably no good so isConnectedOrConnecting is not used here
				int result = (mobile.isConnected() ? 1 : 0);
				Logger.D(TAG, "doHasCellNetwork " + result + "-");
				return result;
			}
			Logger.D(TAG, "doHasCellNetwork noNet-");
			return 0; //No network at all
		}
		Logger.E(TAG, "doHasCellNetwork- Cannot get connectivity manager");
		return -1;
	}
	
	private int doHasWifiNetwork()
	{
		Logger.D(TAG, "doHasWifiNetwork+");
		if (connectivityManager != null)
		{
			NetworkInfo wifi = connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
			Logger.D(TAG, "doHasWifiNetwork-");
			if (wifi != null)
			{
				// "Connecting" state is probably no good so isConnectedOrConnecting is not used here
				int result = (wifi.isConnected() ? 1 : 0);
				Logger.D(TAG, "doHasWifiNetwork " + result + "-");
				return (wifi.isConnected() ? 1 : 0);
			}
			Logger.D(TAG, "doHasWifiNetwork noNet-");
			return 0; //No network at all
		}
		Logger.E(TAG, "doHasWifiNetwork- Cannot get connectivity manager");
		return -1;
	}
}
