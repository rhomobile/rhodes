package rhomobile;

import java.io.IOException;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;

import net.rim.device.api.servicebook.ServiceBook;
import net.rim.device.api.servicebook.ServiceRecord;
import net.rim.device.api.system.DeviceInfo;
import net.rim.device.api.system.RadioInfo;

//import net.rim.device.api.ui.component.Dialog;

public class NetworkAccess {

	private static String URLsuffix;
	private static String WAPsuffix;
	private static boolean networkConfigured = false;
	private static boolean bes = true;

	public static String getSuffix() {
		if (!networkConfigured)
			autoConfigure();
		if (!networkConfigured)
			return "";
		if (WAPsuffix != null)
			return WAPsuffix;
		else if (URLsuffix != null)
			return URLsuffix;
		else
			return "";
	}

	public static void autoConfigure() {
		networkConfigured = false;
		bes = false;
		URLsuffix = null;
		WAPsuffix = null;
		String wip = null;// Configuration.getGatewayIP();

		if (wip != null) {
			String apn = null;// Configuration.getGatewayAPN();
			WAPsuffix = ";WapGatewayAPN=" + apn;
			WAPsuffix += ";WapGatewayIP=" + wip;
			String wpt = null;// Configuration.getGatewayPort();
			if (wpt != null && wpt.length() > 0) {
				WAPsuffix += ";WapGatewayPort=" + wpt;
			}
			networkConfigured = true;
		} else {
			AUTO: {
				if (DeviceInfo.isSimulator()) {
					URLsuffix = ";deviceside=true";
					networkConfigured = true;
					break AUTO;
				}

				ServiceBook sb = ServiceBook.getSB();
				if (sb != null) {
					ServiceRecord[] srs = sb.getRecords();
					// search for BES transport
					for (int i = 0; i < srs.length; i++) {
						if (srs[i].isDisabled() || !srs[i].isValid())
							continue;
						if (srs[i].getCid().equals("IPPP")
								&& srs[i].getName().equals("Desktop")) {
							URLsuffix = "";
							networkConfigured = true;
							bes = true;
							break;
						}
					}
					// search for BIS-B transport
					if (URLsuffix == null) {
						for (int i = 0; i < srs.length; i++) {
							if (srs[i].isDisabled() || !srs[i].isValid())
								continue;
							if (srs[i].getCid().equals("IPPP")
									&& srs[i].getName().equals("IPPP for BIBS")) {
								log("SRS: CID: " + srs[i].getCid() + " NAME: " + srs[i].getName());
								URLsuffix = ";deviceside=false;ConnectionType=mds-public";
								networkConfigured = true;
								break;
							}
						}
					}
				}
			}
			if (networkConfigured == false) {
				URLsuffix = ";deviceside=true";
				networkConfigured = true;
			}
		}

	}

	public static boolean isNetworkAvailable() {
		if (!(RadioInfo.getState() == RadioInfo.STATE_ON))
			return false;
		if ((RadioInfo.getNetworkService() & RadioInfo.NETWORK_SERVICE_DATA) == 0)
			return false;
		if (bes)
			return true;
		if (URLsuffix == null)
			return false;
		return networkConfigured;
	}

	public static HttpConnection connect(String server) throws IOException {
		HttpConnection http = null;
		// Check the network status before continuing.
		if (((net.rim.device.api.system.RadioInfo.getNetworkService() & net.rim.device.api.system.RadioInfo.NETWORK_SERVICE_DATA) == 0)
				|| (URLsuffix == null && WAPsuffix == null)) {
			throw new IOException("Network Data Service Not Available");
		}
		// Setup connection parameters
		String param = null;
		if (WAPsuffix != null && WAPsuffix.length() > 0) {
			param = WAPsuffix;
		} else if (URLsuffix != null && URLsuffix.length() > 0) {
			param = URLsuffix;
		} else {
			param = "";
		}
		// Connect
		try {
			int fragment = server.indexOf('#');
			if (-1 != fragment) {
				server = server.substring(0, fragment);
			}

			System.out.println(server + param);
			http = (HttpConnection) Connector.open(server + param);
		} catch (IOException ioe) {
			System.out.println("Open exception: " + ioe.getMessage());
			if (http != null)
				http.close();
			http = null;
			throw ioe;
		}
		return http;
	}

	private static void log(String txt) {
		 System.out.println(txt);
	}
	
	private NetworkAccess() {
	}
	/*
	 * public static void promptForConfiguration() { final Mobilet app =
	 * Mobilet.getInstance(); app.invokeLater(new Runnable() { public void run() {
	 * 
	 * int autoconf = Dialog.ask( "Unable to connect to the network. Do you want
	 * to try auto" + "configuration?", new String[] { "Yes", "No" }, new int[] {
	 * 0, 1 }, 0);
	 * 
	 * if (autoconf == 0) { autoConfigure(); if (isNetworkAvailable() == false) {
	 * Dialog .inform("We are having difficulties with network access for" +
	 * "this blackberry. We will try manual configuration."); } else {
	 * app.networkStateChanged(true); return; } }
	 * 
	 * int _bes = Dialog .ask( "Are you connected through a Blackberry
	 * Enterprise Server?", new String[] { (bes ? "Yes" + " " + "(Current
	 * Choice)" : "Yes"), (bes ? "No" : "No" + " " + "(Current Choice)"), "I
	 * don't know" }, new int[] { 0, 1, 2 }, 2);
	 * 
	 * if (_bes == 0) { networkConfigured = true; Dialog .inform("We have
	 * configured the network with BES settings." + "Please contact your device
	 * administrator if the problem persists."); URLsuffix = ""; } else if (_bes ==
	 * 1) { // Default to BIS configuration URLsuffix =
	 * ";deviceside=false;ConnectionType=mds-public"; ; networkConfigured =
	 * true; Dialog .inform("We have configured the network with BIS settings." +
	 * "Please contact your device administrator if the problem persists."); }
	 * else { // Default to BIS configuration URLsuffix =
	 * ";deviceside=false;ConnectionType=mds-public"; ; networkConfigured =
	 * true; Dialog .inform("We have configured the network with default" +
	 * "settings. Please contact your device administrator if the problem" +
	 * "persists."); } app.networkStateChanged(URLsuffix != null); } }); }
	 */
}
