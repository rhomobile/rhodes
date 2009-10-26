package com.rho.net;

import java.io.IOException;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;

import rhomobile.RhodesApplication;
import rhomobile.Utilities;

import net.rim.device.api.servicebook.ServiceBook;
import net.rim.device.api.servicebook.ServiceRecord;
import net.rim.device.api.system.DeviceInfo;
import net.rim.device.api.system.RadioInfo;

import com.rho.BBVersionSpecific;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.net.bb.BBHttpConnection;
import com.rho.net.bb.NativeBBHttpConnection;

import net.rim.device.api.io.http.HttpHeaders;

public class NetworkAccess implements INetworkAccess {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("NetworkAccess");

	private static String URLsuffix = "";
	private static String WIFIsuffix;
	private static boolean networkConfigured = false;
	private static boolean bes = true;
	
	public String getHomeUrl()
	{
		return "http://localhost:8080/";
	}
	
	public void configure() {
		networkConfigured = false;
		bes = false;
		URLsuffix = null;

		String strDeviceside = ";deviceside=true";
		if ( com.rho.RhoConf.getInstance().getInt("no_deviceside_postfix") == 1 )
			strDeviceside = "";

		if (DeviceInfo.isSimulator()) {
			URLsuffix = ";deviceside=true";
			networkConfigured = true;
		}else{
			ServiceBook sb = ServiceBook.getSB();
			if (sb != null) {
				ServiceRecord[] wifis = sb.findRecordsByCid("WPTCP");
				for ( int i = 0; i < wifis.length; i++ ){
					if (/*srs[i].isDisabled() ||*/ !wifis[i].isValid())
						continue;
					
					WIFIsuffix = ";interface=wifi" + strDeviceside; 
						//";deviceside=true;ConnectionUID=" + 
						//wifis[i].getUid();
					
					LOG.INFO("WIFI :" + WIFIsuffix );
					
					break;
				}
				
				ServiceRecord[] srs = sb.getRecords();
				// search for BIS-B transport
				if (URLsuffix == null) {
					for (int i = 0; i < srs.length; i++) {
						if (srs[i].isDisabled() || !srs[i].isValid())
							continue;
						if (srs[i].getCid().equals("IPPP")
								&& srs[i].getName().equals("IPPP for BIBS")) {
							LOG.INFO("SRS: CID: " + srs[i].getCid() + " NAME: " + srs[i].getName());
							
							URLsuffix = ";deviceside=false;ConnectionType=mds-public";
							networkConfigured = true;
							break;
						}
					}
				}
				
				// search for BES transport
				for (int i = 0; i < srs.length; i++) {
					LOG.INFO("SB: " + srs[i].getName() + ";UID: " + srs[i].getUid() +
							";CID: " + srs[i].getCid() +
							";APN: " + srs[i].getAPN() + ";Descr: " + srs[i].getDataSourceId() +
							";Valid: " + (srs[i].isValid() ? "true" : "false") + 
							";Disabled: "+ (srs[i].isDisabled()? "true" : "false") );
					
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
				
			}
		}
		
		String strConfPostfix = com.rho.RhoConf.getInstance().getString("bb_connection_postfix");
		if ( strConfPostfix != null && strConfPostfix.length() > 0 )
		{
			URLsuffix = strConfPostfix;
			networkConfigured = true;
		}else if (networkConfigured == false) {
			URLsuffix = strDeviceside;//";deviceside=true";
			networkConfigured = true;
		}

		LOG.INFO("Postfix: " + URLsuffix);
	}

	/*public IHttpConnection doLocalRequest(String strUrl, String strBody)
	{
		HttpHeaders headers = new HttpHeaders();
		headers.addProperty("Content-Type", "application/x-www-form-urlencoded");
		
		
		HttpConnection http = Utilities.makeConnection(strUrl, headers, strBody.getBytes());
		
//		RhodesApplication.getInstance().postUrl(strUrl, strBody, headers);
		
		return new BBHttpConnection(http);
	}*/
	
	public boolean isWifiActive()
	{
		return BBVersionSpecific.isWifiActive();
	}
	
	public IHttpConnection connect(String url) throws IOException 
	{
		HttpConnection http = null;

		if ( URI.isLocalHost(url) )
		{
			URI uri = new URI(url);
			return new RhoConnection(uri);
		}
		
		int fragment = url.indexOf('#');
		if (-1 != fragment) {
			url = url.substring(0, fragment);
		}

		//Try wifi first
		if ( WIFIsuffix != null && isWifiActive() ){
			try {
				LOG.INFO(url + WIFIsuffix);
				http = (HttpConnection) Connector.open(url + WIFIsuffix);
			} catch (IOException ioe) {
				LOG.INFO("WIFI connection failed: " + ioe.getMessage() );
			}
		}
		
		if ( http == null ){
			/*int nStatus = net.rim.device.api.system.RadioInfo.getNetworkService();
			if ( ( nStatus & net.rim.device.api.system.RadioInfo.NETWORK_SERVICE_DATA) == 0) {
				throw new IOException("Network Data Service Not Available");
			}*/
			
			try {
				LOG.INFO(url + URLsuffix);
				http = (HttpConnection) Connector.open(url + URLsuffix);
			} catch (IOException ioe) {
				
				if ( URLsuffix.length() > 0 )
				{
					try{
						LOG.INFO(url);
						http = (HttpConnection) Connector.open(url);
					} catch (IOException ioe2) {
						LOG.ERROR("Connector.open exception", ioe2 );
						if (http != null)
							http.close();
						http = null;
						throw ioe2;
					}
				}else
				{				
					LOG.ERROR("Connector.open exception", ioe );
					if (http != null)
						http.close();
					http = null;
					throw ioe;
				}
			}catch(Exception exc)
			{
				throw new IOException("Could not open network connection.");
			}
		}
		
		return new BBHttpConnection(http);
	}

	public void close() {
	}

	public boolean isNetworkAvailable() {
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
	
}
