package com.rho.rhoelements.plugins;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import android.content.Context;
import android.net.DhcpInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Handler;
import android.text.format.Formatter;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.Config;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rho.rhoelements.graphics.BatteryIndicator;
import com.rho.rhoelements.graphics.SignalIndicator;

/**
 * SignalPlugin class is responsible for handling Signal meta tags.  The Indicator class 
 * is responsible for drawing the icon using the passed display parameters. 
 * @author Geoff Day (XFH386, Initial Creation)
 * @author Darryn Campbell (JRQ768, Updated to link properties to display interface)
 */
public class SignalPlugin extends Plugin
{
	private int SIGNAL_STRENGTH = 0;
	private int SSID = 1;
	private int MAC_ADDRESS = 2;
	private int ADAPTER_NAME = 3;
	private int SERVER_ADDRESS = 4;
	private int DHCP = 5;
	private int GATEWAY = 6;
	private int IP_ADDRESS = 7;
	private int RSSI = 8;
	private int NETMASK = 9;
	private int WINS = 10;
	private static final String	PARAM_GRAPHPOSITION	= "graphposition";
	private static final String	PARAM_ICONPOSITION	= "iconposition";
	private static final String	PARAM_LAYOUT		= "layout";
	private static final String	VALUE_UP			= "up";
	private static final String	VALUE_DOWN			= "down";

	///  Registered URL via SignalEvent, which will be called back with signal values
	private String m_signalUrl;

	///  Reference to background thread which polls signal state and calls that state back
	private Thread m_checkerThread;

	///  Interval, in milliseconds.  How often the signal state will be polled.
	private int m_checkInterval;

	///  Reference to the SignalIndicator class used to display the indicator.
	private SignalIndicator m_signalIndicator;

	/// event names corresponding to navigate name value pairs
	private static final String[] NAMES = new String[] 
	                                                 {
		"signalStrength",	// Signal strength value as a percentage 
		"essid",			// Current SSID 
		"macAddress",		// Device's MAC address 
		"adapterName",		// Device's adapter name 
		"dhcpServer",		// Current DHCP servers address 
		"dhcpStatic",		// Whether the unit has a static or DHCP address 
		"gateway",			// Current gateway IP address 
		"ipAddress",		// Current IP address 
		"rssi",				// strength in RSSI terms 
		"subnetMask",		// Current subnet mask 
		"wins"				// Current WINs server IP address 

	                                                 };

	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version ("Signal");
	}

	/**
	 * Create a Signal Plugin object using the default signalIndicator and associated indicator panel
	 */
	public SignalPlugin()
	{
		super();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));

		m_signalUrl = null;
		m_checkerThread = null;

		// Get signal indicator via indicator panel
		this.m_signalIndicator = Common.elementsCore.getIndicatorPanel().getSignalIndicator();

		//  Default visibility of signal indicator is hidden
		m_signalIndicator.setVisible(false);
		m_signalIndicator.setPaintColor("#000000");

		try 
		{
			m_checkInterval = Integer.parseInt(Common.config.getSetting(Config.SETTING_SIGNAL_REFRESH));
		}
		catch (NumberFormatException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Unable to retrive Signal Refresh from Configuration"));

			m_checkInterval = 5000;
		}

	}

	/* (non-Javadoc)
	 * @see com.rho.rhoelements.Plugin#onShutdown()
	 */
	@Override
	public void onShutdown()
	{
		m_signalIndicator.reset();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		stopChecker();
	}

	/* (non-Javadoc)
	 * @see com.rho.rhoelements.Plugin#onSetting(com.rho.rhoelements.PluginSetting)
	 */
	@Override
	public void onSetting(PluginSetting setting)
	{
		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));

		//  Parse the META Tag and set attributes as appropriate
		if (setting.getName().equalsIgnoreCase("signalevent"))
		{
			startChecker();
			m_signalUrl = setting.getValue();
		}
		else if (setting.getName().equalsIgnoreCase("visibility"))
		{
			if (setting.getValue().equalsIgnoreCase("visible"))
			{
				//  Show the Indicator and start updating it
				startChecker();
				m_signalIndicator.setVisible(true);
				m_signalIndicator.reDraw();
			}
			else if (setting.getValue().equalsIgnoreCase("hidden"))
			{
				//  Visibility default is hidden in PB3
				stopChecker();
				m_signalIndicator.setVisible(false);
				m_signalIndicator.reDraw();
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid visibility value: '" + setting.getValue() + "'"));
			}
		}
		else if (setting.getName().equalsIgnoreCase("color") || setting.getName().equalsIgnoreCase("colour"))
		{
			m_signalIndicator.setPaintColor(setting.getValue());
			m_signalIndicator.reDraw();
		}
		else if (setting.getName().equalsIgnoreCase("left"))
		{
			try
			{
				m_signalIndicator.setxPosition(Integer.parseInt(setting.getValue()), true);
				m_signalIndicator.reDraw();
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid left value: " + setting.getValue()));
			}
		}
		else if (setting.getName().equalsIgnoreCase("top"))
		{
			try
			{
				m_signalIndicator.setyPosition(Integer.parseInt(setting.getValue()), true);
				m_signalIndicator.reDraw();
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid top value: " + setting.getValue()));
			}
		}
		/*			else if (setting.getName().equalsIgnoreCase("right"))
			{
				try
				{
					m_signalIndicator.setxPosition(Integer.parseInt(setting.getValue()), false);
					m_signalIndicator.reDraw();
				}
				catch(NumberFormatException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid right value: " + setting.getValue()));
				}
			}
			else if (setting.getName().equalsIgnoreCase("bottom"))
			{
				try
				{
					m_signalIndicator.setyPosition(Integer.parseInt(setting.getValue()), false);
					m_signalIndicator.reDraw();
				}
				catch(NumberFormatException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid bottom value: " + setting.getValue()));
				}
			}*/
		else if (setting.getName().equalsIgnoreCase(PARAM_ICONPOSITION))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, PARAM_ICONPOSITION + " is no longer supported. Please use " + PARAM_LAYOUT + " instead"));
		}
		else if (setting.getName().equalsIgnoreCase(PARAM_GRAPHPOSITION))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, PARAM_GRAPHPOSITION + " has been deprecated but is still usable. In the future please use \"" + PARAM_LAYOUT + "\"."));
			setLayout(setting.getValue());
		}
		else if (setting.getName().equalsIgnoreCase(PARAM_LAYOUT))
		{
			setLayout(setting.getValue());
		}
		else if (setting.getName().equalsIgnoreCase("signalrefresh"))
		{
			//  Addition from Browser
			//  Change the refresh rate of the signal updates
			m_checkInterval = Integer.parseInt(setting.getValue());
			//  If the checker thread is currently running, stop it and restart
			//  it with the new check interval.
			if (m_checkerThread != null)
			{
				stopChecker();
				startChecker();
			}
		}
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unknown setting '" + setting.getName() + "'"));
	}
	
	/**
	 * Sets the layout of the battery indicator
	 * @param value the layout String: "up"/"down"/"left"/"right"/("top")/("bottom")
	 */
	private void setLayout(String value)
	{
		if (value.equalsIgnoreCase("left"))
		{
			m_signalIndicator.setLayout(SignalIndicator.LEFT);
			m_signalIndicator.reDraw();
		}
		else if (value.equalsIgnoreCase("right"))
		{
			m_signalIndicator.setLayout(SignalIndicator.RIGHT);
			m_signalIndicator.reDraw();
		}
		else if (value.equalsIgnoreCase("top") || value.equalsIgnoreCase(VALUE_UP))
		{
			m_signalIndicator.setLayout(SignalIndicator.UP);
			m_signalIndicator.reDraw();
		}
		else if (value.equalsIgnoreCase("bottom") || value.equalsIgnoreCase(VALUE_DOWN))
		{
			m_signalIndicator.setLayout(SignalIndicator.DOWN);
			m_signalIndicator.reDraw();
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid layout '" + value + "'"));
		}
	}

	@Override
	public void onPageStarted(String url)
	{
		if(m_signalUrl != null)
		{
			synchronized(m_signalUrl)
			{
				m_signalUrl = null;
			}
		}
	}

	/**
	 * Start the thread which will poll the signal status
	 */
	void startChecker()
	{

		// Quit if already running
		if (m_checkerThread != null)
			return;

		// Create and start thread
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Starting WLAN polling thread"));
		m_checkerThread = new Thread(new Checker(new Handler()));
		m_checkerThread.start();
	}

	/**
	 *  Stop the thread which will poll the signal status
	 */
	void stopChecker()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, ""));

		// Quit if not running
		if (m_checkerThread == null)
			return;

		// Interrupt thread and wait
		try
		{
			m_checkerThread.interrupt();
			m_checkerThread.join(m_checkInterval);
		}
		catch (Exception e)
		{
		}

		m_checkerThread = null;
	}

	/**
	 * Thread which will poll the status of the signal
	 * @author Geoff Day (XFH386, Initial Creation)
	 * @author Darryn Campbell (JRQ768, modified to not require a signal event to update the graphic)
	 */
	public class Checker implements Runnable
	{
		Handler handler;

		/**
		 * @param handler
		 */
		Checker(Handler handler)
		{
			this.handler = handler;
		}

		/* (non-Javadoc)
		 * @see java.lang.Runnable#run()
		 */
		@Override
		public void run()
		{
			// Every checkInterval get the WiFi status and fire event
			while (true)
			{
				// Get the WiFi status and synchronize a CheckerUpdate with it 
				handler.post(new CheckerUpdate());

				// Sleep but quit if interrupted
				try
				{
					Thread.sleep(m_checkInterval);
				}
				catch (InterruptedException e)
				{
					return;
				}
				catch (Exception e)
				{
				}
			}
		}

		/**
		 *  Updates signal indicator with passed WiFi values.
		 *  Checker thread constructs this object then synchronizes it using its Handler
		 */
		class CheckerUpdate implements Runnable
		{
			/* (non-Javadoc)
			 * @see java.lang.Runnable#run()
			 */
			@Override
			public void run()
			{
				// Get the current WiFi status via a WifiManager
				WifiManager manager = (WifiManager) Common.mainActivity.getSystemService(Context.WIFI_SERVICE);
				WifiInfo info = manager.getConnectionInfo();
				DhcpInfo dhcp = manager.getDhcpInfo();

				// Convert RSSI to signal strength 0 - 10 (though Android seems to return 11 sometimes...)
				int level = WifiManager.calculateSignalLevel(info.getRssi(), 31);
				if (level < 0)
					level = 0;
				if (level > 30)
					level = 30;

				m_signalIndicator.setValue(level);
				m_signalIndicator.reDraw();


				//  If there is a registered callback poll the WiFi information

				// Get name value pairs to pass with event
				String[] values = new String[11];

				// Check WiFI is enabled and associated to an AP
				if (manager.isWifiEnabled() && info.getRssi() != 0)
				{
					values[SIGNAL_STRENGTH] = String.valueOf((int) ((float) level * (10f / 3f)));
					values[SSID] = info.getSSID();
					values[MAC_ADDRESS] = info.getMacAddress();

					// Adapter name is not possible under Android.
					// Nearest is display name of NetworkInterface, but that is the Linux name, 
					//  e.g. eth0.
					values[ADAPTER_NAME] = "";
					Enumeration<NetworkInterface> interfaces = null;
					try 
					{
						//  No simple way to obtain our network interface, iterate over all
						//  network interfaces and compare it with our current IP address
						//  to see which one we're associated with.
						interfaces = NetworkInterface.getNetworkInterfaces();
						//  Online solutions advise comparing against both endianness of the 
						//  IP address for device compatibility.
						int wifiIP = info.getIpAddress();
						int reverseWifiIP = Integer.reverseBytes(wifiIP);
						boolean bFoundInterface = false;
						while (interfaces.hasMoreElements())
						{
							if (bFoundInterface)
								break;
							//  For each Network Interface on the device
							NetworkInterface iface = interfaces.nextElement();
							Enumeration<InetAddress> inetAddresses = iface.getInetAddresses();
							while (inetAddresses.hasMoreElements())
							{
								//  For each InetAddress on that Interface
								InetAddress nextElement = inetAddresses.nextElement();
								int byteArrayToInt = byteArrayToInt(nextElement.getAddress(), 0);
								if (byteArrayToInt == wifiIP || byteArrayToInt == reverseWifiIP)
								{
									values[ADAPTER_NAME] = iface.getDisplayName();
									bFoundInterface = true;
									break;
								}
							}
						}
					}
					catch (SocketException e)
					{
						values[ADAPTER_NAME] = "";
					}

					values[SERVER_ADDRESS] = Formatter.formatIpAddress(dhcp.serverAddress);

					// No way to know if using DHCP
					values[DHCP] = "";

					values[GATEWAY] = Formatter.formatIpAddress(dhcp.gateway);
					values[IP_ADDRESS] = Formatter.formatIpAddress(info.getIpAddress());
					values[RSSI] = String.valueOf(info.getRssi());
					values[NETMASK] = Formatter.formatIpAddress(dhcp.netmask);

					// WINS not available under Android
					values[WINS] = "";
				}
				else
				{
//				    // Set blank values if WiFi is disabled
//				    for (int n = 0; n < values.length; n++)
//					values[n] = "";

					//Set values to equal PB
					for (int n = 0; n < values.length; n++)
					{
						values[n] = "";
					}
					values[SIGNAL_STRENGTH] = "0";
					values[MAC_ADDRESS] = "00:00:00:00:00:00";
					//values[DHCP] = "DHCP";
					values[RSSI] = "0";
				}

				// if signal event URL not NULL and length is greater than 0 fire event
				if (m_signalUrl != null && m_signalUrl.length() > 0)
				{
					synchronized(m_signalUrl)
					{
						// Fire signal event
						try
						{
							navigate(m_signalUrl, NAMES, values);
						}
						catch (NavigateException e)
						{
							e.printStackTrace();
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
						}
					}
				}

			}
		}
	}

	/**
	 * Function to convert the byte array format of an IP address to an integer form
	 * @param arr The array to convert
	 * @param offset The offset in the array
	 * @return Integer form of IP address
	 */
	public static final int byteArrayToInt(byte[] arr, int offset) 
	{
		if (arr == null || arr.length - offset < 4)
			return -1;

		int r0 = (arr[offset] & 0xFF) << 24;
		int r1 = (arr[offset + 1] & 0xFF) << 16;
		int r2 = (arr[offset + 2] & 0xFF) << 8;
		int r3 = arr[offset + 3] & 0xFF;
		return r0 + r1 + r2 + r3;
	}
}




