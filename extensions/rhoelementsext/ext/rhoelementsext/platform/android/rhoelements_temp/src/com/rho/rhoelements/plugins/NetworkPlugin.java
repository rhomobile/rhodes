package com.rho.rhoelements.plugins;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.Timer;
import java.util.TimerTask;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Handler;
import android.os.Message;
import android.telephony.PhoneStateListener;
import android.telephony.ServiceState;
import android.telephony.SignalStrength;
import android.telephony.TelephonyManager;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/** //TODO change to page non persistent? 
 * @author Barry Piper (HPKX48)
 * @author Ben Kennedy (NCVT73, Alignment to Windows, tidied)
 * 
 * The following is an example of some JavaScript code that can be used to setup triggering 
 * to help determine whether a network connection is available. Available is defined as able 
 * to connect to a specified web server via the normal TCPIP protocol.
 * 
 *         <pre>
 *  	This function is required and called on a change of network state , specified in NetworkEvent 
 *  	function onNetwork(connected)
 *   	{
 * 	alert(connected);
 *   	}
 * 
 *  
 *  	Setting the URL/IP address to be polled
 *      IP addresses or URL's are valid
 *      	generic.InvokeMETAFunction('Network', "host:url('www.google.com');"); 
 *        or
 *      	generic.InvokeMETAFunction('Network', "host:url('74.125.230.112')"); 
 *      
 *      Setting the Port to be used for the specified IP 
 *      The following is not required unless a custom port is needed default is set to port 80
 *      	generic.InvokeMETAFunction('Network', "port:1234"); 
 *      
 *      Setting the polling interval that a connection to the specified URL/IP address will attempted 
 *      units are in Milliseconds this is not required the default setting is 5000.
 * 	generic.InvokeMETAFunction('Network', "networkcheckinterval:5000");
 * 
 *      Setting the timeout value for the socket. This is the amount of time the connection attempt will wait
 *      before giving up and timing out units are in Milliseconds. This is not required to be specified and the 
 *      default setting is 10000.
 * 	generic.InvokeMETAFunction('Network', "connectiontimeoutinterval:1000");
 * 
 *      Setting the method to be called in Java Script on Network State Change. 
 *      If a change of status occurs this is required for any of that event information to be passed to java script.
 *      Below shows an example where OnNetwork java script function will be called on Network State Change. The 
 *      network event callback needs to be setup before "Start" or "isnetworkavailable" are called.
 *      	generic.InvokeMETAFunction('Network', "networkevent:url('javascript:onNetwork('%s');')"); 
 *      
 *      Start network checking
 *      This starts the process of periodically checking the network state. 
 *      	generic.InvokeMETAFunction('Network',"Start");
 * 
 *      Stop network checking    
 *      This stops repeated network checking
 *      	generic.InvokeMETAFunction('Network',"Stop");
 *      
 *      
 *      
 *      Checking if network is available ( without running repeated mechanism )
 *      This starts the networking checking mechanism one time only 
 *      Returns "CONNECTED", "DISCONNECTED", "ERROR" ( not currently used )
 *      	generic.InvokeMETAFunction('Network', "isnetworkavailable:url('javascript:onNetwork('%s');')"); 
 *      Note: 
 *      This command also takes the event URL to setup call back to javascript.
 *      Use of this function does not affect the periodic checking mechanism.
 *      
 *      Resetting settings to Default
 *      Resets Network Port , Check Interval and Socket Timeout to defaults. 
 *      	generic.InvokeMETAFunction('Network', "resetdefaults"); 
 *      
 *      
 *      Check the current status of the Network State Checking Mechanism
 *      Gives the status of the mechanism as to whether it is currently running or not
 *      Returns "true" or "false"
 *      	generic.InvokeMETAFunction('Network', "isnetworkcheckrunning" );
 *      The "isnetworkcheckrunning" running functionality requires a event callback to be setup
 *      therefore before calling "isnetworkcheckrunning" the following needs to be setup:
 *       	generic.InvokeMETAFunction('Network', "checkerrunningevent:url('javascript:onNetwork('%s');')"); 
 *      Note "onNetwork" can be any javascript function of your choosing.
 *      
 *      
 *      Important Note: A change to any properties only takes affect to upon next run of  
 *      isnetworkavailable ( Single Shot Checking ) or the periodic checking mechanism started with
 *      Start.  
 *      To update the properties of the periodic checking mechanism the mechamism must be stopped with 
 *      Stop and then restarted with Start
 *      
 *      If the webpage where the network plugin was started is navigated away from
 *      the network plugin will stop all currently outstanding oneshot and repeated network checks
 *      and also reset all configured events.
 * 
 * </pre>
 */
public class NetworkPlugin extends Plugin
{
	private final int	DEFAULT_INTERVAL			= 5000;
	private final int	DEFAULT_CONNECTION_TIMEOUT_INTERVAL	= 10000;
	private final int	DEFAULT_NETWORK_PORT			= 80;
	private final String	DEFAULT_NETWORK_URL_ADDRESS		= "www.symbol.com";
	private final String	METHOD_START  				= "Start";//TODO change to Start/Enable?
	private final String	METHOD_STOP  				= "Stop";//TODO change to Stop/Disable?
	private final String	PARAMETER_HOST 				= "Host";
	private final String	PARAMETER_PORT				= "Port";
	private final String	PARAMETER_INTERVAL			= "NetworkPollInterval";//TODO change to PollInterval?
	private final String	PARAMETER_TIMEOUT			= "ConnectionTimeout";
	private final String	EVENT_NAME				= "NetworkEvent";
	private final String	EVENT_TITLE				= "connectionInformation";

	private int		port					= DEFAULT_NETWORK_PORT;
	private int		iCheckInterval				= DEFAULT_INTERVAL;
	private int		iConnectionTimeoutInterval		= DEFAULT_CONNECTION_TIMEOUT_INTERVAL;

	private String		networkEventURL;
	private String 		mWanEventURL;
	private String		strCheckURL;

	private Timer		checkNetworkTimer			= new Timer();
	private Timer		oneShotCheckTimer			= new Timer();
//	private String		isCheckerRunningEventURL;
	
	private WanEventsReceiver mWanEventsReceiver = new WanEventsReceiver();
	private WanCellEventsReceiver mPhoneStateListener = new WanCellEventsReceiver();
	private TelephonyManager mTelephonyManager = (TelephonyManager) Common.mainActivity.getSystemService(Context.TELEPHONY_SERVICE);
	public static WanStatus wanStatus = null;
	private final static String[] WAN_NAMES = {"phoneSignalStrength", 
	    						"networkOperator", 
	    						"connectionTypeAvailable", 
	    						"connectionTypeConnected", 
	    						"connectionManagerMessage"};

	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("Network");
	}

	@Override
	public void onPageStarted(String url)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));

		// Stop the service running
		checkNetworkTimer.cancel();
		oneShotCheckTimer.cancel();

		// reset all event URL's
		networkEventURL = "";
		mWanEventURL = null;
		// Reset NetworkConnectionStatus
		NetworkConnectionStatus.resetStatus();
		unregisterWanListeners();
	}

	/**
	 * Method used for firing an event to java script returning the current network state.
	 */
	private void fireNetworkEvent()
	{
		try
		{
			navigate(networkEventURL, EVENT_TITLE, NetworkConnectionStatus.isNetworkAvailable());
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
		}
	}

	/*
	 * (non-Javadoc)
	 * @see
	 * com.rho.rhoelements.plugins.Plugin#onSetting(com.rho.rhoelements.PluginSetting)
	 */
	@Override
	public void onSetting(PluginSetting setting)
	{
	    	if (setting == null)
	    	{
	    	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "setting param is null"));
	    	    return;
	    	}
		if (setting.getValue().length() > 0)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "'" + setting.getName() + "', '" + setting.getValue()
					+ "'"));
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));
		}

		// Parse the META Tag and set attributes as appropriate
		if (setting.getName().equalsIgnoreCase(PARAMETER_HOST))
		{
			strCheckURL = setting.getValue();
		}
		else if (setting.getName().equalsIgnoreCase(EVENT_NAME))
		{
			networkEventURL = setting.getValue();
		}
		else if (setting.getName().equalsIgnoreCase(PARAMETER_INTERVAL))
		{
			try
			{
				iCheckInterval = Integer.parseInt(setting.getValue());
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " +  PARAMETER_INTERVAL + " value: " + setting.getValue()));
			}
		}
		else if (setting.getName().equalsIgnoreCase(PARAMETER_PORT))
		{
			try
			{
				port = Integer.parseInt(setting.getValue());
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " +  PARAMETER_PORT + " value: " + setting.getValue()));
			}
		}
		else if (setting.getName().equalsIgnoreCase(PARAMETER_TIMEOUT))
		{
			try
			{
				iConnectionTimeoutInterval = Integer.parseInt(setting.getValue());
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " +  PARAMETER_TIMEOUT + " value: " + setting.getValue()));
			}
		}
		else if (setting.getName().equalsIgnoreCase(METHOD_START))
		{
			scheduleChecker(strCheckURL);
		}
		else if (setting.getName().equalsIgnoreCase(METHOD_STOP))
		{
			stopScheduleChecker();
		}
		else if (setting.getName().equalsIgnoreCase("connectWan"))
		{
		    setMobileDataEnabled(Common.mainActivity, true);
		}
		else if (setting.getName().equalsIgnoreCase("disconnectWan"))
		{
		    setMobileDataEnabled(Common.mainActivity, false);
		}
		else if (setting.getName().equalsIgnoreCase("wanStatusEvent"))
		{
		    mWanEventURL = setting.getValue();
		    registerWanListeners();
		}
	}

	/**
	 * Method resets Network Port , Check Interval and Socket Timeout to defaults.
	 */
	void resetToDefaults()
	{
		port = DEFAULT_NETWORK_PORT;
		iCheckInterval = DEFAULT_INTERVAL;
		iConnectionTimeoutInterval = DEFAULT_CONNECTION_TIMEOUT_INTERVAL;
	}

	/**
	 * Method checks simple URL format checks if URL is zero length or null if so use the default
	 * URL address if http:// has been defined as part of the URL this is also removed.
	 * 
	 * @param URLtoCheck
	 * @return The parsed URL string.
	 */
	String checkURLFormat(String URLtoCheck)
	{
		// if passed URL contains http:// then strip off header
		if (URLtoCheck == null || URLtoCheck.length() == 0)
		{
			URLtoCheck = DEFAULT_NETWORK_URL_ADDRESS;
		}
		else if (URLtoCheck.contains("http://"))
		{
			URLtoCheck = URLtoCheck.replace("http://", "");
		}
		return (URLtoCheck);
	}



	/**
	 * Method to handle the scheduling of NetworkCheck at a specific interval.
	 * @param checkURL the URL to be checked.
	 */
	private void scheduleChecker(String checkURL)
	{
		checkURL = checkURLFormat(checkURL);

		if (networkEventURL != null && networkEventURL.length() > 0)
		{
			checkNetworkTimer.cancel();
			NetworkConnectionStatus.Network_Checker_Running = false;
			checkNetworkTimer = new Timer();

			// GD 11/3/2011 - change to fixed period scheduling
			checkNetworkTimer.schedule/* AtFixedRate */(new NetworkCheck(checkURL/*, false*/), 1, iCheckInterval);
			NetworkConnectionStatus.Network_Checker_Running = true;
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "networkevent:url is null or has a length of 0 "));

			// Cant use this code because there is no where to route the event!
			// WANConnectionStatus.WAN_Connection_Available = WANConnectionStatus.NETWORK_ERROR;
			// fireNetworkEvent();
		}
	}

	/**
	 * Method to handle the scheduling of doNetworkCheck at a specific interval.
	 * 
	 * @param checkURL
	 *            of type String which contains the URL to be checked.
	 */
	private void stopScheduleChecker()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Network Connection Checking Stopped"));
		checkNetworkTimer.cancel();
		NetworkConnectionStatus.Network_Checker_Running = false;
		checkNetworkTimer = new Timer();
	}

	@Override
	public void onShutdown()
	{
		checkNetworkTimer.cancel();
		unregisterWanListeners();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	}

	/**
	 * Method performs a WAN by performing a TCP Socket connect on a specified port this method is
	 * repeatly scheduled and a java script event is fired if the status has changed from last time
	 * the status was checked.
	 */
	private class NetworkCheck extends TimerTask
	{
		private String				checkURL;
		private boolean				bFirstTimeAround		= false;
//		private boolean				bForceFireEvent;

		public NetworkCheck(String checkURL/*, boolean bForceFireEvent*/)
		{
			this.checkURL = checkURL;
//			this.bForceFireEvent = bForceFireEvent;
			bFirstTimeAround = true;
		}

		@Override
		public void run()
		{
			int network_enabled_test = NetworkConnectionStatus.NETWORK_DISCONNECTED;
			InetAddress iNetAddress = null;

			// Every checkInterval get the internet status and fire event
//			BufferedReader in = null;

			/*
			 * Open a socket connection to the given server on the given port. This method currently
			 * sets the socket timeout value to 10 seconds. (A second version of this method could
			 * allow the user to specify this timeout.)
			 */

			// create a socket
			Socket socket = new Socket();

			// create a socket with a timeout
			try
			{
				iNetAddress = InetAddress.getByName(checkURL);
				SocketAddress socketAddress = new InetSocketAddress(iNetAddress, port);

				// this method will block no more than timeout ms.
				socket.connect(socketAddress, iConnectionTimeoutInterval);

				// if we get to here without erroring then we have a good network
				network_enabled_test = NetworkConnectionStatus.NETWORK_CONNECTED;
				mWanEventsReceiver.onReceive(Common.mainActivity, new Intent("android.net.conn.CONNECTIVITY_CHANGE"));
			}
			catch (Exception e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Problem when trying to connect to : "
						+ iNetAddress + " on Port:" + port + " " + e.getMessage()));
			}
			finally
			{
				try
				{
					if (socket.isConnected())
					{
						socket.close();
					}
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}

				// Fire Network State Changed Event
				if ((NetworkConnectionStatus.Network_Connection_Available != network_enabled_test) || /*bForceFireEvent == true
						||*/ bFirstTimeAround == true)
				{
					// update the status right away
					NetworkConnectionStatus.Network_Connection_Available = network_enabled_test;
					bFirstTimeAround = false;
					mHandler.sendMessage(mHandler.obtainMessage(0, 0, 0));
				}
			}
		} // eof run
	} // eof doNetworkCheck class

	/**
	 * Internal Message Handler to process calls from Timer Task and allow OneShot Event to be
	 * created as Thread on the parents thread context
	 */
	private Handler	mHandler = new Handler()
	{
		@Override
		public void handleMessage(Message msg)
		{
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
		    if (msg.arg1 == 0)
		    {
			fireNetworkEvent();
		    }
		    else
		    {
			fireWanEvent();
		    }
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
		}
	};
	
	/**
	 * Method used for firing an event to java script returning the current wan state.
	 */
	private void fireWanEvent()
	{
		try
		{
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
		    navigate(mWanEventURL, WAN_NAMES, new String[] { new Integer(wanStatus.phoneSignalStrength).toString(), 
				wanStatus.networkOperator,
				wanStatus.connectionTypeAvailable,
				wanStatus.connectionTypeConnected,
				wanStatus.connectionManagerMessage});
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
		}
		catch (NullPointerException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Initialization error"));
		}
	}
	
	private void registerWanListeners()
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
	    IntentFilter wanIntentFilter = new IntentFilter("android.net.conn.CONNECTIVITY_CHANGE");
	    Common.mainActivity.registerReceiver(mWanEventsReceiver, wanIntentFilter);
	    TelephonyManager telephonyManager = (TelephonyManager) Common.mainActivity
	            .getSystemService(Context.TELEPHONY_SERVICE);
	    telephonyManager.listen(mPhoneStateListener, PhoneStateListener.LISTEN_SIGNAL_STRENGTHS | PhoneStateListener.LISTEN_DATA_ACTIVITY | PhoneStateListener.LISTEN_DATA_CONNECTION_STATE);
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
	}
	
	private void unregisterWanListeners()
	{
		try
		{
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));	    
		    Common.mainActivity.unregisterReceiver(mWanEventsReceiver);
		    TelephonyManager telephonyManager = (TelephonyManager) Common.mainActivity
		            .getSystemService(Context.TELEPHONY_SERVICE);
		    telephonyManager.listen(mPhoneStateListener, PhoneStateListener.LISTEN_NONE);
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
		}
		catch(Exception e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Unable to unregister the WAN receivers"));	  
		}
	}
	
	private static String translateMobileNetworkType(int networkType)
	{
	    switch (networkType)
	    {
	    /*case 0:
	        return "Unknown";*/
	    case 1:
	        return "GPRS";  
	    case 2:
		return "EDGE";
	    case 3:
	        return "UMTS";
	    /*case 4:
	        return "CDMA";*/
	    case 5:
	    case 6:
	    case 12:
	        return "EVDO";  
	    case 7:
	        return "1xRTT";
	    case 8:
	    case 9:
	    case 10:
	        return "HSDPA";
	    /*case 9:
	        return "HSUPA";*/
	    /*case 10:
	        return "HSPA";*/
	    /*case 11:
	        return "iDen";*/
	    /*case 13:
	        return "LTE";
	    case 14:
	        return "eHRPD";
	    case 15:
	        return "HSPA+";*/
	    default:
		break;
	    }
	    return Common.mainActivity.getString(RhoExtManager.getResourceId("string","network_unavailable"));
	}
	
	private void setMobileDataEnabled(Context context, boolean enabled)
	{
	    try
	    {
    	    final ConnectivityManager conman = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
    	    Class conmanClass = Class.forName(conman.getClass().getName());
    	    final Field iConnectivityManagerField = conmanClass.getDeclaredField("mService");
    	    iConnectivityManagerField.setAccessible(true);
    	    final Object iConnectivityManager = iConnectivityManagerField.get(conman);
    	    final Class iConnectivityManagerClass = Class.forName(iConnectivityManager.getClass().getName());
    	    final Method setMobileDataEnabledMethod = iConnectivityManagerClass.getDeclaredMethod("setMobileDataEnabled", Boolean.TYPE);
    	    setMobileDataEnabledMethod.setAccessible(true);
    	    setMobileDataEnabledMethod.invoke(iConnectivityManager, enabled);
	    }
	    catch (ClassNotFoundException e)
	    {
		// TODO Auto-generated catch block
		e.printStackTrace();
	    } catch (SecurityException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	    } catch (NoSuchFieldException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	    } catch (IllegalArgumentException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	    } catch (IllegalAccessException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	    } catch (NoSuchMethodException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	    } catch (InvocationTargetException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	    }
	}
	
	private class WanStatus
	{
	    public int phoneSignalStrength = 0;
	    public String networkOperator = "";
	    public String connectionTypeAvailable = "";
	    public String connectionTypeConnected = "";
	    public String connectionManagerMessage = "";
	}
	
	private class WanEventsReceiver extends BroadcastReceiver
	{
	    @Override
	    public void onReceive(Context context, Intent intent)
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
		if (intent == null)
		    return;
		if (wanStatus == null)
		    wanStatus = new WanStatus();
		if (intent.getAction().compareToIgnoreCase("android.net.conn.CONNECTIVITY_CHANGE") == 0)
		{
		    ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService( Context.CONNECTIVITY_SERVICE );
		    NetworkInfo mobNetInfo = connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
		    wanStatus.connectionTypeAvailable = translateMobileNetworkType(mobNetInfo.getSubtype());
		    if (NetworkConnectionStatus.Network_Connection_Available == NetworkConnectionStatus.NETWORK_CONNECTED)
		    {
			    if (mobNetInfo.getType() == ConnectivityManager.TYPE_MOBILE)
				wanStatus.connectionTypeConnected = translateMobileNetworkType(mobNetInfo.getSubtype());
			    else
				wanStatus.connectionTypeConnected = mobNetInfo.getTypeName();
		    }
		    else
		    {
			wanStatus.connectionTypeConnected = Common.mainActivity.getString(RhoExtManager.getResourceId("string","network_notconnected"));
			wanStatus.connectionManagerMessage = "No Connection";
		    }
		    mHandler.sendMessage(mHandler.obtainMessage(0, 1, 0));
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
	    }
	}
	
	private class WanCellEventsReceiver extends PhoneStateListener
	{
	    /**
	     * @see android.telephony.PhoneStateListener#onSignalStrengthsChanged(android.telephony.SignalStrength)
	     */
	    @Override
	    public void onSignalStrengthsChanged(SignalStrength signalStrength)
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
		/*
		 * From Android doc: getGsmSignalStrength gets the GSM Signal Strength, valid values are (0-31, 99) as defined in TS 27.007 8.5
		 * 99 means that the strength is not detectable, ie. the antenna is not functional. This can be normalised to 0 
		 */
		int normalisedSignalStrength = signalStrength.getGsmSignalStrength();
		if (normalisedSignalStrength == 99) //Antenna doesn't work
		{
		    normalisedSignalStrength = 0;
		}
		else
		{
		    normalisedSignalStrength = (int) (normalisedSignalStrength * 100 / 31); 
		}
		wanStatus.phoneSignalStrength = normalisedSignalStrength;
		wanStatus.networkOperator = mTelephonyManager.getNetworkOperatorName();
		mHandler.sendMessage(mHandler.obtainMessage(0, 1, 0));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
	    }

	    /**
	     * @see android.telephony.PhoneStateListener#onDataConnectionStateChanged(int)
	     */
	    @Override
	    public void onDataConnectionStateChanged(int state)
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
		if (state == TelephonyManager.DATA_CONNECTED)
		{
		    NetworkConnectionStatus.Network_Connection_Available = NetworkConnectionStatus.NETWORK_CONNECTED;
		    wanStatus.connectionManagerMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","network_connman_connected"));
		}
		else if (state == TelephonyManager.DATA_DISCONNECTED)
		{
		    NetworkConnectionStatus.Network_Connection_Available = NetworkConnectionStatus.NETWORK_DISCONNECTED;
		    wanStatus.connectionManagerMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","network_connman_notconnected"));
		}
		else if (state == TelephonyManager.DATA_CONNECTING)
		{
		    NetworkConnectionStatus.Network_Connection_Available = NetworkConnectionStatus.NETWORK_DISCONNECTED;
		    wanStatus.connectionManagerMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","network_connman_connecting"));
		}
		mHandler.sendMessage(mHandler.obtainMessage(0, 1, 0));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
	    }

	    /**
	     * @see android.telephony.PhoneStateListener#onServiceStateChanged(android.telephony.ServiceState)
	     */
	    @Override
	    public void onServiceStateChanged(ServiceState serviceState)
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));	
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
	    }
	    
	}
}
