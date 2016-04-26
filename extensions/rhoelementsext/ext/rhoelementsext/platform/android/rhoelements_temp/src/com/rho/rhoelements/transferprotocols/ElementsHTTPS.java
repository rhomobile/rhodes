/**
 * \file ElementsHTTP.java
 * \brief File containing the ElementsHTTP class
 */

package com.rho.rhoelements.transferprotocols;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.ConnectException;
import java.net.MalformedURLException;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;

import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLSocketFactory;
import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.SSLSession;

import org.apache.commons.net.util.Base64;
import org.apache.http.HttpEntity;
import org.apache.http.HttpHost;
import org.apache.http.HttpResponse;
import org.apache.http.HttpVersion;
import org.apache.http.client.ResponseHandler;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.conn.ConnectTimeoutException;
import org.apache.http.conn.params.ConnRoutePNames;
import org.apache.http.entity.mime.MultipartEntity;
import org.apache.http.entity.mime.content.FileBody;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.CoreProtocolPNames;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.params.HttpProtocolParams;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.util.EntityUtils;
import org.apache.http.client.HttpClient;
import org.apache.http.conn.scheme.SchemeRegistry;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.ClientConnectionManager;
import org.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager;
import org.apache.http.conn.scheme.SocketFactory;
import org.apache.http.conn.ssl.X509HostnameVerifier;

import android.content.Context;
import android.net.wifi.WifiManager;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.Config;
import com.rho.rhoelements.LogEntry;
	
/**
 * Class encapsulating an HTTPS transfer, to avoid naming conflicts with Android this class
 * has been so named.  This class wraps an HTTPS client.
 * @author To Be Defined
 */
public class ElementsHTTPS implements TransferProtocolInterface
{
	private final static int HTTP_TIMEOUT = 30000;
	
	private boolean m_bHttpDownload;
	private String sourceFile;
	private String destinationUrl;
	private int m_iPort;
	private String m_username;
	private String m_password;
	private static int mClientCounter = 0; //This is used to handle the wifi lock more efficiently
	private static WifiManager.WifiLock mWifiLock = null;
	
	/*
    * Used by download
    * Define InputStreams to read from the URLConnection.
    */
	private InputStream is = null;
	private FileOutputStream fos = null;
	private File tempFile = null;
	
	/**
	 * Used by upload
	 */
	private HttpClient httpClient = null;
    
    private final SSLSocketFactory mUrlConnectionSecureSocketFactory = createUrlConnectionSecureSocketFactory();
    private final org.apache.http.conn.scheme.SocketFactory mHttpClientSecureSocketFactory = createHttpClientSecureSocketFactory();
	
	/**
	 * @param isDestinationLocal true the file should be downloaded from an HTTP server, false the file should be uploaded to the server.
	 * @param source The source of the file to be uploaded
	 * @param destination The destination the file should be uploaded to
	 * @param iPort The port on which the transfer should take place
	 * @param username The username to use for the transfer (or null if no username is set)
	 * @param password The password to use for the transfer (or null if no password is set)
	 */
	public ElementsHTTPS(boolean isDestinationLocal, String source, String destination, int iPort,
			String username, String password)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		m_bHttpDownload = isDestinationLocal;
		sourceFile = source;
		destinationUrl = destination;
		m_iPort = iPort;
		m_username = username != null ? username : "";
		m_password = password != null ? password : "";
    }
	
	private void setProxy()
	{
		URL proxy = null;
		if (Common.config.getSetting(Config.SETTING_HTTP_PROXY) == null )
			return;
		try
		{
			proxy = new URL(Common.config.getSetting(Config.SETTING_HTTP_PROXY));
		}
		catch (MalformedURLException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "HTTP Proxy url could not be parsed. Proceeding with no proxy"));
		}
		if (proxy == null)
			return;
		String settingProxyExceptions = Common.config.getSetting(Config.SETTING_PROXY_EXCEPTIONS);
		if (proxy.getHost() != null)
			System.setProperty("http.proxyHost", proxy.getHost());
		System.setProperty("http.proxyPort", String.valueOf(proxy.getPort()));
		if (settingProxyExceptions != null)
			System.setProperty("http.nonProxyHosts", settingProxyExceptions.replaceAll(" ", "|"));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, 
				"Proxy host set to: " +  System.getProperty("http.proxyHost") + ":" + System.getProperty("http.proxyPort")));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, 
				"Proxy exceptions: " +  System.getProperty("http.nonProxyHosts")));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	/**
	 * Resets the proxy settings read from Config.xml in order not to interfere with other
	 * applications which don't want to use proxy
	 */
	public void cleanProxySettings()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		System.clearProperty("http.proxyHost");
		System.clearProperty("http.proxyPort");
		System.clearProperty("http.nonProxyHosts");
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	/**
	 * Method to perform the HTTP transfer, as configured in the HTTP constructor.
	 * This will never be called in the main activity queue therefore the method can
	 * block.
	 * @return String to specify if the transfer was successful, conforming to the Browser FileTransfer format
	 * @throws IOException 
	 */

	public String transfer()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		String result = ERROR;
		WifiManager wifiManager = null;
		try
		{
			if (mClientCounter == 0) //There are no threads currently transferring
			{
				wifiManager = (WifiManager) Common.mainActivity.getSystemService(Context.WIFI_SERVICE);
				mWifiLock = wifiManager.createWifiLock(WifiManager.WIFI_MODE_FULL, Common.APP_NAME + "/FileTransferService");
			    mWifiLock.acquire();
			}
			mClientCounter++;
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "WifiLock used by " + mClientCounter + " HTTP clients"));
			if (m_bHttpDownload) //Download case
				result = downloadFile(sourceFile, destinationUrl);
			else //Upload
				result = uploadFile(sourceFile, destinationUrl);
		}
		catch (StringIndexOutOfBoundsException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
				MALFORMEDURLEXCEPTION));
			result = ERROR + ": " + MALFORMEDURLEXCEPTION;
		}
		catch (MalformedURLException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
				MALFORMEDURLEXCEPTION));
			result = ERROR + ": " + MALFORMEDURLEXCEPTION;
		}
		catch (FileNotFoundException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
					FILENOTFOUNDEXCEPTION));
			result = ERROR + ": " + FILENOTFOUNDEXCEPTION;
		}
		catch (ConnectException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
				CONNECTEXCEPTION));
			result = ERROR + ": " + CONNECTEXCEPTION;
		}
		catch (ConnectTimeoutException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
				CONNECTIONTIMEOUTEXCEPTION));
			result = ERROR + ": " + CONNECTIONTIMEOUTEXCEPTION;
		}
		catch (SocketException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
				CONNECTIONTIMEOUTEXCEPTION));
			result = ERROR + ": " + CONNECTIONTIMEOUTEXCEPTION;
		}
		catch (SocketTimeoutException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
				CONNECTIONTIMEOUTEXCEPTION));
			result = ERROR + ": " + CONNECTIONTIMEOUTEXCEPTION;
		}
		catch (IOException e) {
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
				e.getMessage()));
			String error = IOEXCEPTION;
			if (e.getMessage().compareToIgnoreCase("No such file or directory") == 0)
				error = PATHNOTFOUNDEXCEPTION;
			result = ERROR + ": " + error;
		}	
		catch (URISyntaxException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
					MALFORMEDURLEXCEPTION));
			result = ERROR + ": " + MALFORMEDURLEXCEPTION;
		}
		finally
		{
			if (mClientCounter > 0)
				--mClientCounter;
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "WifiLock used by " + mClientCounter + " HTTP clients"));
			if ( (mWifiLock != null) && (mWifiLock.isHeld()) && (mClientCounter == 0) )
			{
				mWifiLock.release();
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "WifiLock released by HTTP file transfer"));
			}
			try
			{
				//Download cleaning
				if (fos != null)
				{
	        		fos.close();
	        		fos = null;
				}
	        	if (is != null)
	        	{
	        		is.close();
	        		is = null;
	        	}
	        	if (tempFile != null)
	        	{
	        		tempFile.delete();
	        		tempFile = null;
	        	}
	        	
	        	//Upload cleaning
	        	if (httpClient != null)
	        	{
	        		httpClient.getConnectionManager().shutdown();
	        		httpClient = null;
	        	}
			}
			catch (IOException e)
			{
				e.printStackTrace();
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
					IOEXCEPTION));
			}
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return result;
	}	
	
	private String uploadFile(String sourceFile, String destinationUrl) throws IOException, URISyntaxException
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		String result = ERROR;
		httpClient = createHttpsClient();
		HttpParams params = httpClient.getParams();
		
		if (isUrlDirect(destinationUrl) == false)
		{
			setProxy();
			String proxyHost = System.getProperty("http.proxyHost");
			String proxyPort = System.getProperty("http.proxyPort");
			if ( (proxyHost != null) && (proxyPort != null) )
			{
				try
				{
					HttpHost proxy = new HttpHost(proxyHost, Integer.parseInt(proxyPort));
					httpClient.getParams().setParameter(ConnRoutePNames.DEFAULT_PROXY, proxy);
				}
				catch (NumberFormatException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
				}
			}
		}
		params.setParameter(CoreProtocolPNames.PROTOCOL_VERSION, HttpVersion.HTTP_1_1);
		HttpConnectionParams.setConnectionTimeout(params, HTTP_TIMEOUT);
		HttpConnectionParams.setSoTimeout(params, HTTP_TIMEOUT);
		URL url = new URL(fixUrl(destinationUrl));
		HttpPost httpPost = new HttpPost(url.toURI());
		File sourceFileObject = new File(sourceFile);
		if (sourceFileObject.exists() == false)
			throw (new IOException("source file not found"));
	    FileBody fileContent = new FileBody(new File(sourceFile));
	    
	    MultipartEntity reqEntity = new MultipartEntity();
	    
	 // Insert Authorization header if required
		if (m_username.length() > 0)
		{
			byte[] encodedPasswordBuffer = ( m_username + ":" + m_password ).getBytes();
		    Base64 encoder = new Base64();
		    String encodedPassword = encoder.encodeToString(encodedPasswordBuffer);
		    encodedPassword = encodedPassword.substring(0, encodedPassword.length() - 2 );
		    httpPost.setHeader("Authorization",
		                       "Basic " + encodedPassword);
		}
        
		reqEntity.addPart("SpbImagerFile", fileContent);
        httpPost.setHeader("Cache-Control", "no-cache");
        httpPost.setHeader("User-Agent", "Symbol RhoElements");
        httpPost.getParams().setBooleanParameter( "http.protocol.expect-continue", false);
        Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "executing request " + httpPost.getRequestLine()));
        httpPost.setEntity(reqEntity);
	    result = httpClient.execute(httpPost, new CustomHttpResponse());
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return result;
	}
	
	private String downloadFile(String sourceUrl, String destinationFile) throws IOException 
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Begin"));
		String result = ERROR;
		
		URL url = new URL(fixUrl(sourceUrl));
		
		if (isUrlDirect(url.toString()) == false)
			setProxy();
        /* Open a connection to that URL. */
        
        HttpsURLConnection httpConnection = (HttpsURLConnection) url.openConnection();
        httpConnection.setRequestMethod("GET");
        httpConnection.setDoInput(true);
        httpConnection.setSSLSocketFactory(mUrlConnectionSecureSocketFactory);
        
     // Insert Authorization header if required
		if (m_username.length() > 0)
		{
			byte[] encodedPasswordBuffer = ( m_username + ":" + m_password ).getBytes();
		    Base64 encoder = new Base64();
		    String encodedPassword = encoder.encodeToString(encodedPasswordBuffer);
		    //Get rid of the trailing \r\n in the encoded password
		    encodedPassword = encodedPassword.substring(0, encodedPassword.length() - 2);
		    httpConnection.setRequestProperty( "Authorization",
		                            "Basic " + encodedPassword);
		}
        
        httpConnection.setConnectTimeout(HTTP_TIMEOUT);
		String tempDirectory = destinationFile.substring(0, destinationFile.lastIndexOf("/") + 1);
		//String suffix = String.valueOf(Calendar.getInstance().getTimeInMillis()); //Never read
		tempFile = File.createTempFile(destinationFile.substring(destinationFile.lastIndexOf("/") + 1), 
				"temp",
				new File(tempDirectory));
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "downloading to " + tempFile.getAbsolutePath()));
		is = httpConnection.getInputStream();
        BufferedInputStream bis = new BufferedInputStream(is);

        /*
         * Read bytes to the Buffer until there is nothing more to read(-1).
         */
        byte[] buffer = new byte[5120];
        int length = 0;
        fos = new FileOutputStream(tempFile);
        while ((length = bis.read(buffer)) > 0)
            fos.write(buffer, 0, length);

        // Convert the Bytes read to a String. 
        if (tempFile != null)
			tempFile.renameTo(new File(destinationFile));

        result = OK_FILERECEIVED;
		    
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	    return result;
	}
    
    static private SSLSocketFactory createUrlConnectionSecureSocketFactory() {
        return RESecureSocketFactory.createJavaXFactory();
    }
    
    static private org.apache.http.conn.scheme.SocketFactory createHttpClientSecureSocketFactory() {
        return RESecureSocketFactory.createApacheFactory();
    }
    
    private HttpClient createHttpsClient() {
    
        HttpParams params = new BasicHttpParams();
        HttpProtocolParams.setVersion(params, HttpVersion.HTTP_1_1);
//        HttpProtocolParams.setContentCharset(params, HTTP.UTF_8);

        SchemeRegistry registry = new SchemeRegistry();
        registry.register(new Scheme("https", mHttpClientSecureSocketFactory, 443));

        ClientConnectionManager ccm = new ThreadSafeClientConnManager(params, registry);

        return new DefaultHttpClient(ccm, params);
    }
	
	private String fixUrl(String url) throws MalformedURLException
	{
		StringBuffer destinationBuffer = new StringBuffer(url);
		// Check whether the protocol is already part of the destination url
		String currentPrefix = url.substring(0, 8); //"http://" if exists
		currentPrefix = currentPrefix.toLowerCase();
		if (currentPrefix.compareTo("https://") != 0)
			destinationBuffer.insert(0, "https://");
		// Add a forward slash if there isn't any (upload to the destination root)
		if (destinationBuffer.substring(8).indexOf("/") < 0 )
			destinationBuffer.append("/");
		//Insert the port number
		destinationBuffer.insert(destinationBuffer.indexOf("/", 8), ":" + String.valueOf(m_iPort));
		// Carry out some test on the url
		URL urlObject = new URL(destinationBuffer.toString());
		Pattern userNameRegex = Pattern.compile(USERNAMEPASSWORDREGEX);
		//Pattern 
		if (urlObject.getHost().length()==0)
			throw (new MalformedURLException());
		/*
		 * This is required as URL object would parse correctly a url with no username but with password
		 * (ie. http://:root@192.168.1.1:8080/Folder/upload.aspx
		 */
		if (urlObject.getUserInfo() != null)
		{
			Matcher userNameRegexMatcher = userNameRegex.matcher(urlObject.getUserInfo());
			if (!userNameRegexMatcher.find())
				throw (new MalformedURLException());
		}
		return destinationBuffer.toString();
	}
	
	private boolean isUrlDirect(String url)
	{
		String exSetting = Common.config.getSetting(Config.SETTING_PROXY_EXCEPTIONS);
		if (exSetting == null)
			return false;
		String[] proxyExceptions = exSetting.split(" ");
		for (int i=0; i<proxyExceptions.length; i++)
	    {
	    	if (proxyExceptions[i].length() == 0)
	    		continue;
	    	try
	    	{
				Pattern exceptionPattern = Pattern.compile(proxyExceptions[i]);
		    	Matcher exceptionMatcher = exceptionPattern.matcher(url);
		    	if (exceptionMatcher.find())
		    		return true;
	    	}
	    	catch (PatternSyntaxException e)
	    	{
	    		//Do nothing
	    	}
	    }
		return false;
	}
	/**
	 * This class allows to read custom http server responses also when the http response != 2xx
	 * @author FPC843
	 *
	 */
	private class CustomHttpResponse implements ResponseHandler<String>
	{
		@Override
		public String handleResponse(HttpResponse response) throws IOException
		{
			HttpEntity entity = response.getEntity();
			String responseString = entity == null ? null : EntityUtils.toString(entity);
			if (responseString != null)
			{
				int firstNewLinePos = responseString.indexOf("\r\n");
				if (firstNewLinePos > 0)
					responseString = responseString.substring(0,responseString.indexOf("\r\n"));
			}
	        return responseString;
		} 
	}
}