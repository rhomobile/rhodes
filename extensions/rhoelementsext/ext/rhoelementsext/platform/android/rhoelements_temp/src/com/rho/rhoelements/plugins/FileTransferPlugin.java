package com.rho.rhoelements.plugins;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rho.rhoelements.services.FileTransferService;
import com.rho.rhoelements.services.FileTransferService.FileTransferProtocols;

/**
 * File Transfer plugin is responsible for providing an interface to the user
 * to send and receive files over ftp / http or on the local file system.
 * Identically to the FileTransfer plugin in Browser 3 this plugin should
 * first be configured with all the attributes of the transfer and then 'Transfer()'
 * called before configuring for the next transfer.
 * @author Darryn Campbell (JRQ768, February 2011, Initial Creation)
 */
public class FileTransferPlugin extends Plugin
{	
	/**
	 * The intent filter unique to the File Transfer plugin which we 
	 * will listen to to indicate that our transfer has completed.
	 */
	public static final String FT_PLUGIN_TRANSFER_COMPLETE = 
		"com.rho.rhoelements.plugins.FileTransferPlugin.TRANSFER_COMPLETE";
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		// Sample Version with custom build tag
		return new Version ("FileTransfer");
	}
	
	private static boolean hasNavigatedAway = false; //This flag is true if we 
	
	/**
	 * Create an initialised file transfer plugin
	 */
	public FileTransferPlugin()
	{
		super();
		resetVariables();
		m_transferEvent = null;
		m_transferIntent = null;
		//  Receiver to listen to the intent returned after the File Transfer service has completed.
		IntentFilter intentFilter = new IntentFilter(FT_PLUGIN_TRANSFER_COMPLETE);
		m_receiver = new FileTransferBroadcastReceiver();
		Common.mainActivity.registerReceiver(m_receiver, intentFilter);
		m_handler = new Handler();
	}
	
	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onShutdown()
	 */
	@Override
	public void onShutdown() 
	{
		//  Method called when the runtime is shutting down
		//  Stop all transfers currently in progress.  Note calling stopService will
		//  stop the service for all transfers, not just our own.
		if (m_transferIntent != null)
			Common.mainActivity.stopService(m_transferIntent);
		if (m_receiver != null)
			Common.mainActivity.unregisterReceiver(m_receiver);
	}
	
	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onPageStarted(java.lang.String)
	 */
	@Override
	public void onPageStarted(String url)
	{
		resetVariables();
		m_transferEvent = null;
		hasNavigatedAway = true;
	}
	
	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onSetting(com.rho.rhoelements.PluginSetting)
	 */
	@Override
	public void onSetting(PluginSetting setting) {
		//  Method called to parse META Tags
		if (setting.getValue().length() > 0)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, 
					String.format("Meta tag received: '%s', '%s'", 
							setting.getName(), setting.getValue())));
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, 
					"Meta tag received: " + setting.getName()));
		}

		//  Parse the META tag and set attributes as appropriate.
		String settingName = setting.getName();
		String settingValue = setting.getValue();
		try
		{
			if (settingName.equalsIgnoreCase("Protocol"))
			{
				if (settingValue.equalsIgnoreCase("File"))
					m_protocol = FileTransferProtocols.PROTOCOL_FILE;
				else if (settingValue.equalsIgnoreCase("Http"))
					m_protocol = FileTransferProtocols.PROTOCOL_HTTP;
				else if (settingValue.equalsIgnoreCase("Ftp"))
					m_protocol = FileTransferProtocols.PROTOCOL_FTP;
			}
			else if (settingName.equalsIgnoreCase("SetFileDestination"))
			{
				if (settingValue.equalsIgnoreCase("True"))
					m_bFileDestination = true;
				else
					m_bFileDestination = false;
			}
			else if (settingName.equalsIgnoreCase("Port"))
			{
				m_port = Integer.parseInt(settingValue);
			}
			else if (settingName.equalsIgnoreCase("CreateFolders"))
			{
				if (settingValue.equalsIgnoreCase("True"))
					m_bCreateFolders = true;
				else
					m_bCreateFolders = false;
			}
			else if (settingName.equalsIgnoreCase("Source"))
			{
			    if (settingValue.startsWith("url:")) {
			        m_source = settingValue.substring(4);
			    }
			    else {
			        m_source = settingValue;
			    }
			}
			else if (settingName.equalsIgnoreCase("Destination"))
			{
			    if (settingValue.startsWith("url:")) {
			        m_destination = settingValue.substring(4);
			    }
			    else {
			        m_destination = settingValue;
			    }
			}
			else if (settingName.equalsIgnoreCase("Overwrite"))
			{
				if (settingValue.equalsIgnoreCase("True"))
					m_bOverwrite = true;
				else
					m_bOverwrite = false;
			}
			else if (settingName.equalsIgnoreCase("UserName"))
			{
				m_username = settingValue;
			}
			else if (settingName.equalsIgnoreCase("Password"))
			{
				m_password = settingValue;
			}		
			else if (settingName.equalsIgnoreCase("TransferEvent"))
			{
				m_transferEvent = settingValue;
			}
			else if (settingName.equalsIgnoreCase("Transfer"))
			{
				hasNavigatedAway = false;
				//  Create the intent to initialise the FileTransferService, this will 
				//  contain all attributes applicable to the transfer such as source,
				//  destination etc.  The user does not have to specify all fields but
				//  if they have provided insufficient information the transfer will
				//  not succeed.  The fields which need to be specified are identical to Browser
				m_transferIntent = new Intent(Common.mainActivity, FileTransferService.class);
				//  Add data to the intent
				m_transferIntent.putExtra(FileTransferService.TransferProtocol, m_protocol);
				m_transferIntent.putExtra(FileTransferService.FileDestination, m_bFileDestination);
				m_transferIntent.putExtra(FileTransferService.Port, m_port);
				m_transferIntent.putExtra(FileTransferService.CreateFolders, m_bCreateFolders);
				m_transferIntent.putExtra(FileTransferService.Source, m_source);
				m_transferIntent.putExtra(FileTransferService.Destination, m_destination);
				m_transferIntent.putExtra(FileTransferService.Overwrite, m_bOverwrite);
				m_transferIntent.putExtra(FileTransferService.Username, m_username);
				m_transferIntent.putExtra(FileTransferService.Password, m_password);
				m_transferIntent.putExtra(FileTransferService.TransferEvent, m_transferEvent);
				//  Specify our Broadcast receiver to be notified when the File Transfer has 
				//  finished
				m_transferIntent.putExtra(FileTransferService.IntentFilter, FT_PLUGIN_TRANSFER_COMPLETE);
				//  Return ID allows us to distinguish between responses from the FileTransferService
				//  but the FileTransferPlugin does not need to distinguish.
				m_transferIntent.putExtra(FileTransferService.ReturnID, "FTReturn");
				//  Start the File Transfer Service
				Common.mainActivity.startService(m_transferIntent);
				resetVariables();
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unknown setting '" + setting.getName() + "'"));
			}
		}
		catch (NumberFormatException e)
		{
			//  Port number may have been specified incorrectly
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unable to convert specified setting to a number (" + setting.getName() + ")"));
		}
	}
	
	/**
	 * Each transfer uses default values which reset after the transfer is intiated.  This 
	 * function resets those values.
	 */
	private void resetVariables()
	{
		m_protocol = null;
		m_bFileDestination = false;
		m_port = 0;	//  Set to the appropriate default value in the File Transfer service.
		m_bCreateFolders = false;
		m_source = null;
		m_destination = null;
		m_bOverwrite = false;
		m_username = null;
		m_password = null;
		//m_transferEvent = null;
	}
	
	//  Attributes congifured by the user relating to the transfer, these
	//  will be packaged into an intent and used by the FileTransferService
	///  The protocol of the transfer
	FileTransferProtocols m_protocol;
	///  Only provided for backwards compatibility with EMML 1.0 / SPB2.x, Whether or not the destination is the local file system.
	boolean m_bFileDestination;
	///  The port to use for FTP / HTTP
	int m_port;
	///  Whether folders should be created on the local FS for the transfer
	boolean m_bCreateFolders;
	///  The source of the transfer
	String m_source;
	///  The destination of the trasnfer
	String m_destination;
	///  Whether or not the file on the local FS should be overwritten if it exists
	boolean m_bOverwrite;
	///  The username to use for the HTTP / FTP transfer
	String m_username;
	///  The password to use for the HTTP / FTP transfer
	String m_password;
	///  The registered 'TransferEvent' string.  Note this persists between transfers.
	String m_transferEvent;
	///  Reference to our receiver class which will receive notifications when transfers complete
	FileTransferBroadcastReceiver m_receiver;
	///  The Intent associated with the Service to transfer the file.  We only need a reference to this should we need to stop the service.
	Intent m_transferIntent;
	///  Handler to synchronise with main event queue
	Handler m_handler;
	
	/**
	 * Class to receive the intent sent from the FileTransferService when the File Transfer
	 * has completed.  We configure the intent in such a way so that we are the only recipient.
	 * @author Darryn Campbell, JRQ768.  Initial Creation, February 2011
	 */
	public class FileTransferBroadcastReceiver extends BroadcastReceiver
	{
		/* (non-Javadoc)
		 * @see android.content.BroadcastReceiver#onReceive(android.content.Context, android.content.Intent)
		 */
		@Override
		public void onReceive(Context arg0, Intent receivedIntent)
		{
		    if ( (receivedIntent == null) || (receivedIntent.getIntExtra(FileTransferService.PID, android.os.Process.myPid()) != android.os.Process.myPid()) )
	    		    return;
		    Bundle b = receivedIntent.getExtras();
		    //  Extract the values associated with the return Intent
		    //  File Transfer does not use return ID
		    @SuppressWarnings("unused")
		    String returnID = b.getString(FileTransferService.ReturnID);
		    final String returnValue = b.getString(FileTransferService.ReturnValue);
		    final String transferEvent = b.getString(FileTransferService.TransferEvent);
			
		    m_handler.post(new Runnable()
		    {
			@Override
			public void run()
			{
			    /**
				 * If the user has navigated away the page where the transfer has started, then don't trigger the event
				 */
				if (hasNavigatedAway)
					return;
				//  Navigate to the specified URL
				if (transferEvent != null && returnValue != null)
					try
					{
						navigate(transferEvent, new String[] {"transferResult"}, new String[] {returnValue});
					}
					catch (NavigateException e)
					{
						e.printStackTrace();
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
					}
				else if (transferEvent != null)
					navigate(transferEvent);
				}
			});
		}
	}
}
