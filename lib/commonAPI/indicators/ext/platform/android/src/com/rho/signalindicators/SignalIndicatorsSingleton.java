package com.rho.signalindicators;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;
import android.graphics.Color;
import android.hardware.SensorManager;
import android.net.DhcpInfo;
import android.net.NetworkInfo;
import android.net.NetworkInfo.State;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.text.format.Formatter;
import android.view.Display;
import android.view.OrientationEventListener;

import com.rho.indicators.IndicatorView;
import com.rho.indicators.IndicatorView.Layout;
import com.rho.signalindicators.ISignalIndicatorsSingleton;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.util.ContextFactory;

class SignalIndicatorsSingleton extends BroadcastReceiver implements ISignalIndicatorsSingleton
{
	protected static String TAG = "SignalIndicatorsSingleton";

	private MethodResult statusResult;
	private boolean isReceiverRegistered;

	private SignalIndicatorsView signalIndicatorsView;

	private final int RSSI_MIN = -90;
	private final int RSSI_MAX = -50;
	private final String NET_CONNECTED = "Connected";

	private int signalLevel;
	private int signalStrength;
	private int wifiIpAddress;
	private int rssi;
	private String SSID = "";
	private String macAddress = "";
	private String adapterName = "";
	private int serverAddress;
	private int gateway;
	private int netmask;
	private boolean defaultValues;
	private boolean isPositionDefault;

	private int top = 0; // Default position of icon
	private int left = 0; // Default position of icon
	private IndicatorView.Layout layout = Layout.RIGHT; // Default
	private int color = Color.BLACK; // Default
	private SignalIndicatorsView signalView = null;

	public SignalIndicatorsSingleton(SignalIndicatorsFactory factory)
	{
		super();
		Logger.D(TAG, "+");
		signalView = new SignalIndicatorsView(RhodesActivity.getContext());
		isReceiverRegistered = false;
		defaultValues = true;
		//left = calculateWidth() - SignalIndicatorsView.LONG_SIDE;
		left = calculateWidth() - signalView.LONG_SIDE;
		isPositionDefault = true;
		Logger.D(TAG, "-");
	}

	private int calculateWidth()
	{
		Display display = RhodesActivity.safeGetInstance().getWindowManager().getDefaultDisplay();
		return display.getWidth();
	}

	@Override
	public void getRefreshInterval(IMethodResult result)
	{
		Logger.D(TAG, "getRefreshInterval");
		result.setError("refreshInterval is not supported on Android");
	}

	@Override
	public void setRefreshInterval(int refreshInterval, IMethodResult result)
	{
		Logger.D(TAG, "setRefreshInterval");
		result.setError("refreshInterval is not supported on Android");

	}

	/**
	 * Registers this class from listening to signal status if it is not already registered
	 * @author Shekar Muniven, Malaya Panda
	 */
	private void registerForSignalStatus()
	{
		Logger.D(TAG, "registerForSignalStatus+");
		if (!isReceiverRegistered)
		{
			Logger.D(TAG, "registerForSignalStatus async");
			Activity activity = RhodesActivity.safeGetInstance();
			if (activity != null)
			{
				IntentFilter filter = new IntentFilter();
				filter.addAction(WifiManager.RSSI_CHANGED_ACTION);
				filter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
				filter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
				activity.registerReceiver(this, filter);
				isReceiverRegistered = true;
			}
			else
				Logger.E(TAG, "Could not communicate with main Activity");
		}
		Logger.D(TAG, "registerForSignalStatus-");
	}

	/**
	 * Unregisters this class from listening to signal status, only if the indicator is invisible
	 * and the signalStatus is stopped
	 * @author Ben Kennedy (NCVT73)
	 */
	private void unregisterForSignalStatus()
	{
		Logger.D(TAG, "unregisterForSignalStatus");

		if (isReceiverRegistered && statusResult == null && (signalIndicatorsView == null || !signalIndicatorsView.isShown()))
		{
			Activity activity = RhodesActivity.safeGetInstance();
			if (activity != null)
			{
				activity.unregisterReceiver(this);
				isReceiverRegistered = false;
			}
		}
	}

	@Override
	public void wlanStatus(IMethodResult result)
	{
		Logger.D(TAG, "wlanStatus+");
		if (isCallback(result))
		{
			((MethodResult) result).keepAlive();
			stopWlanStatus(null);
			statusResult = ((MethodResult) result);
			registerForSignalStatus();
		}
		else
		{
			stopWlanStatus(null);
			registerForSignalStatus();
			forceGetSignalInfo(result);
		}
		Logger.D(TAG, "wlanStatus-");

	}

	@Override
	public void stopWlanStatus(IMethodResult result)
	{
		Logger.D(TAG, "stopSignalStatus+");
		if (statusResult != null)
		{
			statusResult.release();
			statusResult = null;
		}
		unregisterForSignalStatus();
		Logger.D(TAG, "stopSignalStatus-");
	}

	@Override
	public void showIcon(Map<String, Object> propertyMap, IMethodResult result)
	{
		Logger.D(TAG, "showIcon+");

		// Parse and set all parameters if they are correct.
		// If the parameters are invalid, then ignore and use the currently set parameters/
		Object newTop = propertyMap.get(HK_TOP);
		Object newLeft = propertyMap.get(HK_LEFT);
		Object newLayout = propertyMap.get(HK_LAYOUT);
		Object newColor = propertyMap.get(HK_COLOR);

		if (newTop != null)
		{
			try
			{
				top = IndicatorView.parseDimension(newTop);
				isPositionDefault = false;
			}
			catch (IllegalArgumentException e)
			{
				Logger.E(TAG, e.getMessage());
			}
		}

		if (newLeft != null)
		{
			try
			{
				left = IndicatorView.parseDimension(newLeft);
				isPositionDefault = false;
			}
			catch (IllegalArgumentException e)
			{
				Logger.E(TAG, e.getMessage());
			}
		}

		if (newLayout != null)
		{
			try
			{
				layout = IndicatorView.parseLayout(newLayout);
				onConfigurationChanged();
			}
			catch (IllegalArgumentException e)
			{
				Logger.E(TAG, e.getMessage());
			}
		}

		if (newColor != null)
		{
			try
			{
				color = IndicatorView.parseColor(newColor);
			}
			catch (IllegalArgumentException e)
			{
				Logger.E(TAG, e.getMessage());
			}
		}
		// End parsing the parameters

		/*
		 * if(signalQuality == 0) { Activity activity = RhodesActivity.safeGetInstance();
		 * if(activity != null) { IntentFilter filter=new IntentFilter();
		 * filter.addAction(WifiManager.RSSI_CHANGED_ACTION); Intent signalIntent =
		 * activity.registerReceiver(null, filter); //signalLevel =
		 * signalIntent.getIntExtra("level", 0); int level =
		 * signalIntent.getIntExtra(WifiManager.EXTRA_NEW_RSSI, 0); signalLevel = level;
		 * signalQuality=getSignalQuality(signalLevel); } }
		 */
		if (signalStrength == 0)
		{
			registerForSignalStatus();
			populateActualValues(null, "Unknown");
		}

		if (signalIndicatorsView == null)
		{
			signalIndicatorsView = new SignalIndicatorsView(ContextFactory.getUiContext());
			signalIndicatorsView.setupView(top, left, layout, color);
			registerForSignalStatus();
		}
		else
		{
			signalIndicatorsView.updateParams(top, left, layout, color);
		}
		signalIndicatorsView.setSignalLevel(signalStrength);
		signalIndicatorsView.show();
		Logger.D(TAG, "showIcon-");

	}

	@Override
	public void hideIcon(IMethodResult result)
	{
		Logger.D(TAG, "hideIcon+");
		if (signalIndicatorsView != null)
		{
			signalIndicatorsView.hide();
			unregisterForSignalStatus();
		}
		else
		{
			Logger.W(TAG, "Erroneous hideIcon call. Signal indicator not visible");
		}
		Logger.D(TAG, "hideIcon-");
	}

	@Override
	public void onReceive(Context context, Intent intent)
	{
		Logger.D(TAG, "onReceive" + intent.getAction() + "+");
		boolean doRefresh = false;
		String action = intent.getAction();
		Logger.D("Wifi", "WLan Plugin --- Intent Action :" + action);

		if (WifiManager.NETWORK_STATE_CHANGED_ACTION.equalsIgnoreCase(action))
		{
			Logger.D(TAG, "WifiManager.NETWORK_STATE_CHANGED_ACTION");
			NetworkInfo mWifiNetworkInfo = (NetworkInfo) intent.getParcelableExtra(WifiManager.EXTRA_NETWORK_INFO);

			if (mWifiNetworkInfo.getState() == State.CONNECTED)
			{
				doRefresh = populateActualValues(intent, NET_CONNECTED);
			}
			else
			{
				doRefresh = populateDefaultValues();
			}
		}
		if (WifiManager.WIFI_STATE_CHANGED_ACTION.equalsIgnoreCase(action))
		{
			int wifiState = intent.getIntExtra("wifi_state", 3);
			switch (wifiState)
			{

				case WifiManager.WIFI_STATE_ENABLED:
					Logger.D(TAG, "Wifi State --- WIFI_STATE_ENABLED");
					doRefresh = populateActualValues(intent, "Wifi_State_Enabled");
					break;

				case WifiManager.WIFI_STATE_ENABLING:

					Logger.D(TAG, "Wifi State --- WIFI_STATE_ENABLING");
					doRefresh = populateDefaultValues();
					break;

				case WifiManager.WIFI_STATE_DISABLED:

					Logger.D(TAG, "Wifi State --- WIFI_STATE_DISABLED");
					doRefresh = populateDefaultValues();
					break;

				case WifiManager.WIFI_STATE_DISABLING:
					Logger.D(TAG, "Wifi State --- WIFI_STATE_DISABLING");
					doRefresh = populateDefaultValues();

					break;

				case WifiManager.WIFI_STATE_UNKNOWN:
					Logger.D(TAG, "Wifi State --- WIFI_STATE_UNKNOWN");
					doRefresh = populateDefaultValues();

					break;
			}
		}
		if (WifiManager.RSSI_CHANGED_ACTION.equalsIgnoreCase(action))
		{
			Logger.D(TAG, "RSSI_CHANGED_ACTION");
			int wLanSignalStrength = 0;
			wLanSignalStrength = intent.getIntExtra(WifiManager.EXTRA_NEW_RSSI, 0);
			if (this.rssi != wLanSignalStrength)
			{
				this.rssi = wLanSignalStrength;
				this.signalLevel = WifiManager.calculateSignalLevel(wLanSignalStrength, 31);
				if (this.signalLevel < 0)
					this.signalLevel = 0;
				if (this.signalLevel > 0)
					this.signalLevel = 30;
				this.signalStrength = /* (int)((float) this.signalLevel*(10f/3f)) */getSignalQuality(wLanSignalStrength);
				doRefresh = true;
			}
		}

		if (doRefresh)
		{
			updateIndicator();
			fireSignalStatus(null);
		}
	}

	private boolean populateDefaultValues()
	{
		Logger.D(TAG, "populateDefaultValues+");
		if (this.defaultValues == false)
		{
			this.defaultValues = true;
			this.signalLevel = 0;
			this.signalStrength = 0;
			this.wifiIpAddress = 0;
			this.rssi = 0;
			this.SSID = "";
			this.macAddress = "00:00:00:00:00:00";
			this.adapterName = "NOT_AVAILABLE";
			this.serverAddress = 0;
			this.gateway = 0;
			this.netmask = 0;
			Logger.D(TAG, "populateDefaultValues-");
			return true;
		}
		Logger.D(TAG, "populateDefaultValues-");
		return false;
	}

	private synchronized boolean populateActualValues(Intent intent, String netConnected)
	{
		Logger.D(TAG, "populateActualValues+");
		boolean doRefresh = false;
		this.defaultValues = false;
		WifiInfo wifiInfo = null;
		WifiManager wifiMgr = (WifiManager) RhodesActivity.safeGetInstance().getSystemService(Context.WIFI_SERVICE);
		if (netConnected.equalsIgnoreCase(NET_CONNECTED))
		{
			wifiInfo = intent.getParcelableExtra(WifiManager.EXTRA_WIFI_STATE);
			if (wifiInfo == null)
				wifiInfo = wifiMgr.getConnectionInfo();
		}
		else
		{
			wifiInfo = wifiMgr.getConnectionInfo();
		}

		DhcpInfo dhcpInfo = wifiMgr.getDhcpInfo();
		int localIp = wifiInfo.getIpAddress();
		String localSSID = wifiInfo.getSSID();
		String localMacAddress = wifiInfo.getMacAddress();
		int localServerAddress = dhcpInfo.serverAddress;
		int localGateway = dhcpInfo.gateway;
		int localRSSI = wifiInfo.getRssi();
		int localNetMask = dhcpInfo.netmask;
		int localSignalLevel = WifiManager.calculateSignalLevel(localRSSI, 31);
		if (localSignalLevel < 0)
			localSignalLevel = 0;
		if (localSignalLevel > 0)
			localSignalLevel = 30;
		int localSignalStrength = /* (int)((float) localSignalLevel*(10f/3f)) */getSignalQuality(localRSSI);

		if (this.wifiIpAddress != localIp)
		{
			this.wifiIpAddress = localIp;
			doRefresh = true;
		}
		if (!(this.SSID.equalsIgnoreCase(localSSID)))
		{
			this.SSID = localSSID;
			doRefresh = true;
		}
		if (!(this.macAddress.equalsIgnoreCase(localMacAddress)))
		{
			this.macAddress = localMacAddress;
			doRefresh = true;
		}
		if (this.serverAddress != localServerAddress)
		{
			this.serverAddress = localServerAddress;
			doRefresh = true;
		}
		if (this.gateway != localGateway)
		{
			this.gateway = localGateway;
			doRefresh = true;
		}
		if (this.rssi != localRSSI)
		{
			this.rssi = localRSSI;
			doRefresh = true;
		}
		if (this.netmask != localNetMask)
		{
			this.netmask = localNetMask;
			doRefresh = true;
		}
		if (this.signalLevel != localSignalLevel)
		{
			this.signalLevel = localSignalLevel;
			doRefresh = true;
		}
		if (this.signalStrength != localSignalStrength)
		{
			this.signalStrength = localSignalStrength;
			doRefresh = true;
		}
		
		boolean adapterRefresh = findAdapterName();
		doRefresh = doRefresh || adapterRefresh;
		Logger.D(TAG, "populateActualValues-");
		return doRefresh;
	}

	private boolean findAdapterName()
	{
		Logger.D(TAG, "findAdapterName+");
		boolean dorefresh = false;
		Enumeration<NetworkInterface> interfaces = null;
		try
		{
			interfaces = NetworkInterface.getNetworkInterfaces();
			int wifiIp = this.wifiIpAddress;
			int reverseWifiIp = Integer.reverseBytes(wifiIp);
			boolean bFoundInterface = false;

			while (interfaces.hasMoreElements())
			{
				if (bFoundInterface)
					break;
				NetworkInterface iface = interfaces.nextElement();
				Enumeration<InetAddress> inetAddresses = iface.getInetAddresses();
				while (inetAddresses.hasMoreElements())
				{
					InetAddress nextElement = inetAddresses.nextElement();
					int byteArrayToInt = byteArrayToInt(nextElement.getAddress(), 0);
					if (byteArrayToInt == wifiIp || byteArrayToInt == reverseWifiIp)
					{
						if (!((this.adapterName).equalsIgnoreCase(iface.getDisplayName())))

						{
							this.adapterName = iface.getDisplayName();
							dorefresh = true;

						}
						bFoundInterface = true;
						break;
					}
				}
			}
		}
		catch (SocketException e)
		{
			Logger.D(TAG, "Socket Exception in adapterName");
			this.adapterName = "Socket Exception";
		}
		Logger.D(TAG, "findAdapterName-");
		return dorefresh;
	}

	private static final int byteArrayToInt(byte[] arr, int offset)
	{
		if (arr == null || arr.length - offset < 4)
			return -1;

		int r0 = (arr[offset] & 0xFF) << 24;
		int r1 = (arr[offset + 1] & 0xFF) << 16;
		int r2 = (arr[offset + 2] & 0xFF) << 8;
		int r3 = (arr[offset + 3] & 0xFF);

		return r0 + r1 + r2 + r3;
	}

	private int getSignalQuality(int wLanSignalStrength)
	{
		Logger.D(TAG, "getSignalQuality+");
		// Changes done with reference to the values received like -33,-49 when the signals were
		// excellent.

		int wLanSignalQuality = 0;

		if ((wLanSignalStrength > RSSI_MAX) && (wLanSignalStrength <= 0))
		{
			wLanSignalQuality = 100;
		}
		else if ((wLanSignalStrength < RSSI_MIN) && (wLanSignalStrength >= -100))
		{
			wLanSignalQuality = 0;
		}
		else if ((wLanSignalStrength <= RSSI_MAX) && (wLanSignalStrength >= RSSI_MIN))
		{
			wLanSignalQuality = (int) ((wLanSignalStrength - RSSI_MIN) * 2.5);
		}
		else
		{
			// wLanSignalQuality = ERROR_CODE_FOR_INVALID_VALUES;
		}

		Logger.D(TAG, "getSignalQuality-");
		return wLanSignalQuality;
	}

	/**
	 * Updates and redraws the signal indicator if it is visible.
	 * @author Ben Kennedy (NCVT73)
	 */
	private void updateIndicator()
	{
		Logger.D(TAG, "updateIndicator+");
		if (signalIndicatorsView != null)
		{
			signalIndicatorsView.setSignalState(this.signalStrength);
		}
		Logger.D(TAG, "updateIndicator-");
	}

	private void forceGetSignalInfo(IMethodResult result)
	{
		Logger.D(TAG, "forceGetSignalInfo+");
		/*
		 * Activity activity = RhodesActivity.safeGetInstance(); if(activity != null) { IntentFilter
		 * filter = new IntentFilter(); filter.addAction(WifiManager.RSSI_CHANGED_ACTION);
		 * filter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
		 * filter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION); Intent signalIntent =
		 * activity.registerReceiver(null, filter); NetworkInfo mWifiNetworkInfo = (NetworkInfo)
		 * signalIntent.getParcelableExtra(WifiManager.EXTRA_NETWORK_INFO); if
		 * (mWifiNetworkInfo.getState() == State.CONNECTED) { PopulateActualValues(signalIntent); }
		 * if(signalIntent.getAction().equalsIgnoreCase(WifiManager.RSSI_CHANGED_ACTION)) {
		 * this.rssi=signalIntent.getIntExtra(WifiManager.EXTRA_NEW_RSSI, 0);
		 * this.signalLevel=WifiManager.calculateSignalLevel(this.rssi, 31); if(this.signalLevel<0)
		 * this.signalLevel=0; if(this.signalLevel>0) this.signalLevel=30;
		 * this.signalStrength=(int)((float) this.signalLevel*(10f/3f)); } fireSignalStatus(
		 * result); } else { Logger.E(TAG, "Could not communicate with main Activity");
		 * result.setError("Internal Error. Could not communicate with Activity"); }
		 */
		if (this.defaultValues == true)
			populateActualValues(null, "Unknown");
		fireSignalStatus(result);
		Logger.D(TAG, "forceGetSignalInfo-");
	}

	/**
	 * Sends a signalStatus callback with signal information if it has been registered.
	 * @param result the method result to which to send the signal data. If null, uses the
	 *            statusResult in its place
	 * @author Ben Kennedy (NCVT73)
	 */
	private synchronized void fireSignalStatus(IMethodResult result)
	{
		Logger.D(TAG, "fireSignalStatus+");
		if (result == null)
			result = statusResult;
		if (result != null)
		{
			Map<String, Object> resultMap = new HashMap<String, Object>();

			resultMap.put(HK_DHCP_SERVER, Formatter.formatIpAddress(this.serverAddress));
			resultMap.put(HK_ESSID, this.SSID);
			resultMap.put(HK_GATEWAY, Formatter.formatIpAddress(this.gateway));
			resultMap.put(HK_IP_ADDRESS, Formatter.formatIpAddress(this.wifiIpAddress));
			resultMap.put(HK_MAC_ADDRESS, this.macAddress);
			resultMap.put(HK_RSSI, String.valueOf(this.rssi));
			resultMap.put(HK_SIGNAL_STRENGTH, String.valueOf(this.signalStrength));
			resultMap.put(HK_SUBNET_MASK, Formatter.formatIpAddress(this.netmask));
			resultMap.put(HK_ADAPTER_NAME, this.adapterName);

			result.set(resultMap);
		}
		Logger.D(TAG, "fireSignalStatus-");
	}

	/**
	 * Checks whether the result has a callback (true) or is empty (false)
	 * @param result the IMethodResult to query
	 * @return true is a callback exists, false if no callback exists
	 * @author Ben Kennedy (NCVT73)
	 */
	private boolean isCallback(IMethodResult result)
	{
		return (result != null && result.hasCallback());
	}

	@Override
	protected void finalize() throws Throwable
	{
		Logger.D(TAG, "finalize+");
		hideIcon(null);
		stopWlanStatus(null);
		Logger.D(TAG, "finalize-");
		super.finalize();
	}

	/**
	 * Called when the device rotates (and possibly other times too). See the Android event.
	 */
	public void onConfigurationChanged()
	{
		Logger.D(TAG, "onConfigurationChanged+");
		if(isPositionDefault)
		{
			if(layout.equals(IndicatorView.Layout.UP) || layout.equals(IndicatorView.Layout.DOWN))
			{
				//left = calculateWidth() - SignalIndicatorsView.SHORT_SIDE;
				left = calculateWidth() - signalView.SHORT_SIDE;
			}
			else if(layout.equals(IndicatorView.Layout.LEFT) || layout.equals(IndicatorView.Layout.RIGHT))
			{
				//left = calculateWidth() - SignalIndicatorsView.LONG_SIDE;
				left = calculateWidth() - signalView.LONG_SIDE;
			}
			if(signalIndicatorsView != null)
			{
				signalIndicatorsView.updateParams(top, left, layout, color);
			}
		}
		Logger.D(TAG, "onConfigurationChanged-");
	}
}
