/**
 * \file ElementsFTP.java
 * \brief File containing the ElementsFTP class
 */

package com.rho.rhoelements.transferprotocols;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.ConnectException;
import java.net.MalformedURLException;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPConnectionClosedException;
import org.apache.commons.net.ftp.FTPReply;
import org.apache.http.auth.AuthenticationException;
import org.apache.http.conn.ConnectTimeoutException;

import android.content.Context;
import android.net.wifi.WifiManager;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;

/**
 * Class encapsulating an FTP transfer, to avoid naming conflicts with Android this class
 * has been so named.  This class wraps an FTP client.
 * @author To Be Defined
 */
public class ElementsFTP extends FTPClient implements TransferProtocolInterface
{
	private boolean m_bFtpDownload;
	private String sourceFile;
	private String destinationUrl;
	private int m_iPort;
	private String m_username;
	private String m_password;
	private final static int FTP_TIMEOUT = 10000;
	private static int mClientCounter = 0; //This is used to handle the wifi lock more efficiently
	private static WifiManager.WifiLock mWifiLock = null;
	/**
	 * stream used by upload
	 */
	FileInputStream in = null;
	
	/**
	 * streams used by download
	 */
	FileOutputStream out = null;
	File tempFile = null;
	
	/**
	 * @param isDestinationLocal If True the file should be uploaded to an FTP server, if false the file should be downloaded from the server.
	 * @param source The source of the file to be uploaded
	 * @param destination The destination the file should be uploaded to
	 * @param iPort The port on which the transfer should take place
	 * @param username The username to use for the transfer (or null if no username is set)
	 * @param password The password to use for the transfer (or null if no password is set)
	 */
	public ElementsFTP(boolean isDestinationLocal, String source, String destination, int iPort, 
			String username, String password)
	{
		m_bFtpDownload = isDestinationLocal;
		sourceFile = source;
		destinationUrl = destination;
		m_iPort = iPort;
		m_username = username;
		m_password = password;
	}
	
	/**
	 * Method to perform the FTP transfer, as configured in the FTP constructor.
	 * This will never be called in the main activity queue therefore the method can
	 * block.
	 * @return String to specify if the transfer was successful, conforming to the Browser FileTransfer format
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
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "WifiLock used by " + mClientCounter + " FTP clients"));
			String remoteHostString = m_bFtpDownload ? sourceFile : destinationUrl;
			// We need to get the host name from the whole url
			URL remoteHost = new URL(fixUrl(remoteHostString));
			boolean loginSuccess = connectAndLogin(remoteHost.getHost(), m_username, m_password);
			if (!loginSuccess)
				throw (new AuthenticationException());
			setFileType(FTP.BINARY_FILE_TYPE);
			enterLocalPassiveMode();
			if (m_bFtpDownload) //Download case
				result = downloadFile(remoteHost.getFile(), destinationUrl);
			else //Upload case
				result = uploadFile(sourceFile, remoteHost.getFile());
		}
		catch (FileNotFoundException e) {
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
				FILENOTFOUNDEXCEPTION));
			result = ERROR + ": " + FILENOTFOUNDEXCEPTION;
		}
		catch (java.net.MalformedURLException e) {
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
				MALFORMEDURLEXCEPTION));
			result = ERROR + ": " + MALFORMEDURLEXCEPTION;
		}
		catch (AuthenticationException e) {
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
				IOEXCEPTION));
			result = ERROR + ": " + FTPAUTHENTICATIONEXCEPTION;
		}
		catch (FTPConnectionClosedException e) {
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
				CONNECTIONCLOSED));
			result = ERROR + ": " + CONNECTIONCLOSED;
		}
		catch (UnknownHostException e) {
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, 
					HOSTUNKNOWN));
			result = ERROR + ": " + HOSTUNKNOWN;
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
		finally
		{
			try
			{
				if (mClientCounter > 0)
					--mClientCounter;
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "WifiLock used by " + mClientCounter + " FTP clients"));
				if ( (mWifiLock != null) && (mWifiLock.isHeld()) && (mClientCounter == 0) )
				{
					mWifiLock.release();
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "WifiLock released by FTP file transfer"));
				}
				//Upload cleaning
				if (in != null)
				{
					in.close();
					in = null;
				}
				//Download cleaning
				if (out != null)
				{
					out.close();
					out = null;
				}
				if (tempFile != null)
				{
					tempFile.delete();
					tempFile = null;
				}
				logout();
				disconnect();
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
	
	private String uploadFile(String sourceUrl, String destinationUrl) 
		throws FileNotFoundException, IOException
	{
		String result = null;
		in = new FileInputStream(sourceUrl);
		result = storeFile(destinationUrl, in) ? OK_FILESENT : ERROR;
		return result;
	}
	
	private String downloadFile(String sourceUrl, String destinationFile)
		throws FileNotFoundException, IOException
	{
		String result = ERROR;
		String tempDirectory = destinationFile.substring(0, destinationFile.lastIndexOf("/") + 1);
		tempFile = File.createTempFile(destinationFile.substring(destinationFile.lastIndexOf("/") + 1), 
				"tmp",
				new File(tempDirectory));
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "downloading to " + tempFile.getAbsolutePath()));
		out = new FileOutputStream(tempFile);
		/* 
		 * Download by writing to a temporary file. Such temporary file will be therefore renamed as
		 * destinationFile
		 */
		result = retrieveFile(sourceUrl, out) ? OK_FILERECEIVED : ERROR + ": " + SERVERERRORMESSAGE;
		if ( (tempFile != null) && (result.compareTo(OK_FILERECEIVED) == 0) )
			tempFile.renameTo(new File(destinationFile));
		return result;
	}
	
	/** A convenience method for connecting and logging in */
    private boolean connectAndLogin(String host, String userName, String password)
	    throws IOException, UnknownHostException, FTPConnectionClosedException
    {
		boolean success = false;
		setDefaultPort(m_iPort);
		setDefaultTimeout(FTP_TIMEOUT);
		connect(host);
		setSoTimeout(FTP_TIMEOUT);
		int reply = getReplyCode();
		if (FTPReply.isPositiveCompletion(reply))
		    success = login(userName, password);
		if (!success) 
			disconnect();
		return success;
    }
      
    private String fixUrl(String url) throws MalformedURLException
	{
		StringBuffer destinationBuffer = new StringBuffer(url);
		// Check whether the protocol is already part of the url
//		String currentPrefix = null;
		// url never comes with the protocol, so we just insert it.
		destinationBuffer.insert(0, "ftp://");
		// Add a forward slash if there isn't any (upload to the destination root)
		if (destinationBuffer.substring(6).indexOf("/") < 0 )
			destinationBuffer.append("/");
		//Insert the port number
		destinationBuffer.insert(destinationBuffer.indexOf("/", 6), ":" + String.valueOf(m_iPort));
		// Carry out some test on the url
		URL urlObject = new URL(destinationBuffer.toString());
		Pattern userNameRegex = Pattern.compile(USERNAMEPASSWORDREGEX);
		//Pattern 
		if (urlObject.getHost().length()==0)
			throw (new MalformedURLException());
		/*
		 * This is required as URL object would parse correctly a url with no username but with password
		 * (ie. ftp://:root@192.168.1.1:8080/Folder/upload.aspx
		 */
		if (urlObject.getUserInfo() != null)
		{
			Matcher userNameRegexMatcher = userNameRegex.matcher(urlObject.getUserInfo());
			if (!userNameRegexMatcher.find())
				throw (new MalformedURLException());
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, 
				"fixed ftp url is: " + destinationBuffer.toString()));
		return destinationBuffer.toString();
	}
}