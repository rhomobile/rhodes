package com.rho.rhoelements.plugins;

public class NetworkConnectionStatus
{
	public static final int		NETWORK_CONNECTED				= 1;
	private static final String	NETWORK_CONNECTED_STRING			= "Connected";
	public static final int		NETWORK_DISCONNECTED				= 0;
	private static final String	NETWORK_DISCONNECTED_STRING			= "Disconnected";
	public static final int		NETWORK_ERROR					= -1;
	private static final String	NETWORK_ERROR_STRING				= "ERROR"; //TODO Check alignment here, not sure if windows has this.

	public static final String	NETWORK_CHECK_RUNNING_STRING		= "true";
	public static final String	NETWORK_CHECK_NOT_RUNNING_STRING	= "false";

	public static int		Network_Connection_Available		= ~NETWORK_DISCONNECTED;
	public static boolean		Network_Checker_Running			= false;
	
	
	public static String isNetworkAvailable()
	{
		String strNetworkStatus;

		switch (Network_Connection_Available)
		{
			case NETWORK_CONNECTED:
				strNetworkStatus = NETWORK_CONNECTED_STRING;
				break;
			case NETWORK_DISCONNECTED:
				strNetworkStatus = NETWORK_DISCONNECTED_STRING;
				break;
			case NETWORK_ERROR:
				// fall through on purpose
			default:
				strNetworkStatus = NETWORK_ERROR_STRING;
				break;
		}

		return (strNetworkStatus);
	}

	public static String isNetworkCheckerRunning()
	{
		String strNetworkStatus;

		if (Network_Checker_Running)
		{
			strNetworkStatus = NETWORK_CHECK_RUNNING_STRING;
		}
		else
		{
			strNetworkStatus = NETWORK_CHECK_NOT_RUNNING_STRING;
		}

		return (strNetworkStatus);
	}

	public static void resetStatus()
	{
		// reset back to defaults
		Network_Checker_Running = false;
		Network_Connection_Available = NETWORK_DISCONNECTED;
	}
}
