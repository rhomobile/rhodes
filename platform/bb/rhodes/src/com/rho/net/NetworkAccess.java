package com.rho.net;

import java.io.IOException;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import javax.microedition.io.Connection;
import javax.microedition.io.SocketConnection;

import net.rim.device.api.system.DeviceInfo;
import net.rim.device.api.system.RadioInfo;
import com.rho.BBVersionSpecific;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhoConf;
import com.rho.RhodesApp;
import com.rho.net.bb.BBHttpConnection;
import net.rim.device.api.servicebook.ServiceRecord;
import net.rim.device.api.servicebook.ServiceBook;

public class NetworkAccess implements INetworkAccess {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("NetworkAccess");

	private static String URLsuffix = "";
	private static String WIFIsuffix;
	private static boolean networkConfigured = false;
	private static boolean bes = true;
	private static long  m_nMaxPacketSize = 0;
	
	public void configure() 
	{
		networkConfigured = false;
		bes = false;
		URLsuffix = null;

		String strDeviceside = ";deviceside=true";
		if ( com.rho.RhoConf.getInstance().getInt("no_deviceside_postfix") == 1 )
			strDeviceside = "";

		if (DeviceInfo.isSimulator()) {
			if (com.rho.RhoConf.getInstance().getInt("no_deviceside_postfix") == 1) {
				URLsuffix = "";
				WIFIsuffix = ";interface=wifi";
			} else {
				URLsuffix = ";deviceside=true";
				WIFIsuffix = ";interface=wifi;deviceside=true";
			}
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
					
					LOG.TRACE("WIFI :" + WIFIsuffix );
					
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
	
	public long getMaxPacketSize()
	{
		if ( WIFIsuffix != null && isWifiActive() )
			return 0;
		
		m_nMaxPacketSize = RhoConf.getInstance().getInt("bb_net_maxpacketsize_kb")*1024;
		if ( (DeviceInfo.isSimulator() || URLsuffix.indexOf(";deviceside=true") < 0) && m_nMaxPacketSize == 0 )
		{
			//avoid 403 error on BES/BIS
			//http://supportforums.blackberry.com/t5/Java-Development/HTTP-Error-413-when-downloading-small-files/m-p/103918
			m_nMaxPacketSize = 1024*250;
		}
		
		return m_nMaxPacketSize;
	}
	
	public boolean isWifiActive()
	{
		return BBVersionSpecific.isWifiActive();
	}
	
	public IHttpConnection connect(String url, boolean ignoreSuffixOnSim) throws IOException 
	{
		if ( RhodesApp.getInstance().isRhodesAppUrl(url) )
		{
			URI uri = new URI(url);
			return new RhoConnection(uri);
		}
		
		int fragment = url.indexOf('#');
		if (-1 != fragment) {
			url = url.substring(0, fragment);
		}

		boolean ignoreSuffix = !URI.isLocalHost(url) && ignoreSuffixOnSim && DeviceInfo.isSimulator();
		HttpConnection http = (HttpConnection)baseConnect(url, ignoreSuffix );
		return new BBHttpConnection(http);
	}

	public SocketConnection socketConnect(String strHost, int nPort) throws IOException
	{
		return socketConnect("socket", strHost, nPort);
	}

	public SocketConnection socketConnect(String proto, String strHost, int nPort) throws IOException 
	{
		boolean ignoreSuffix = DeviceInfo.isSimulator();// && proto.equals("ssl") &&
			//URLsuffix.indexOf("deviceside=true") != -1;
		String strUrl = proto + "://" + strHost + ":" + Integer.toString(nPort);
		
		return (SocketConnection)baseConnect(strUrl, ignoreSuffix);
	}
	
	public Connection baseConnect(String strUrl, boolean ignoreSuffix) throws IOException 
	{
		Connection conn = null;
		
		//Try wifi first
		if ( WIFIsuffix != null && isWifiActive() ){
			try {
				LOG.INFO(strUrl + WIFIsuffix);
				conn = Connector.open(strUrl + WIFIsuffix);
			} catch (IOException ioe) {
				LOG.INFO("WIFI connection failed: " + ioe.getMessage() );
			}
		}
		
		if ( conn == null ){
			/*int nStatus = net.rim.device.api.system.RadioInfo.getNetworkService();
			if ( ( nStatus & net.rim.device.api.system.RadioInfo.NETWORK_SERVICE_DATA) == 0) {
				throw new IOException("Network Data Service Not Available");
			}*/
			
			try {
				String url = strUrl;
				//if ( RhoConf.getInstance().getBool("bb_sim_use_deviceside") || !ignoreSuffix )
					url += URLsuffix;
                if (url.startsWith("https"))
					url += ";EndToEndDesired;RdHTTPS";
					
				LOG.INFO(url);
                conn = Connector.open(url, Connector.READ_WRITE, true);
			} catch (java.io.InterruptedIOException ioe) {
				LOG.ERROR("Connector.open InterruptedIOException", ioe );
				if (conn != null)
					conn.close();
				conn = null;
				throw ioe;			
			} catch (IOException ioe) {
                String strMsg = ioe.getMessage(); 
				
		    	boolean bTimeout = strMsg != null && (strMsg.indexOf("timed out") >= 0 || strMsg.indexOf("Timed out") >= 0);
				
				if ( !bTimeout && URLsuffix.length() > 0 )				{
					LOG.ERROR("Connector.open exception", ioe );
					
					try{
                        String url = strUrl;
						if (url.startsWith("https"))
							url += ";EndToEndDesired;RdHTTPS";
						
						LOG.INFO(url);
						conn = Connector.open(url, Connector.READ_WRITE, true);					
					} catch (IOException ioe2) {
						LOG.ERROR("Connector.open exception", ioe2 );
						if (conn != null)
							conn.close();
						conn = null;
						throw ioe2;
					}
				}else
				{				
					LOG.ERROR("Connector.open exception", ioe );
					if (conn != null)
						conn.close();
					conn = null;
					throw ioe;
				}
			}catch(Exception exc)
			{
				throw new IOException("Could not open network connection.");
			}
		}
		
		return conn;
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
