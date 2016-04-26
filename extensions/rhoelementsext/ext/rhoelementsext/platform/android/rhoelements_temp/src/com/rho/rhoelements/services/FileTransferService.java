/**
 * \file FileTransferService.java \brief File containing the FileTransferService class
 */

package com.rho.rhoelements.services;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.channels.FileChannel;
import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.Iterator;

import android.app.Service;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.transferprotocols.ElementsFTP;
import com.rho.rhoelements.transferprotocols.ElementsHTTP;
import com.rho.rhoelements.transferprotocols.ElementsHTTPS;
import com.rhomobile.rhodes.Logger;

/**
 * The FileTransfer Service is used by other classes to transfer files from one location to another.
 * It is built largely around the File Transfer plugin provided in Browser from which the
 * interface is derived. The Service is started via an intent which should contain all necessary
 * information about the transfer (such as source / destination). The service will attempt to obtain
 * the protocol, username, password and port from the provided URL (source or destination) but if
 * these are not specified they should be declared explicitely in the Intent. Static strings are
 * exposed from this class to define the extra data in this intent.
 * @author Darryn Campbell, JRQ768. Initial Creation.
 */
public class FileTransferService extends Service
{
    private static final String FTP_PREFIX = "ftp://";
    private static final String HTTP_PREFIX = "http://";
    private static final String HTTPS_PREFIX = "https://";
    private static final String FILE_PREFIX = "file://";
    
	/**
	 * Enumeration defining the transfer protocols supported by this service. Note additional
	 * services would require extensive refactoring and reworking of this class.
	 * @author Darryn Campbell, JRQ768. Initial Creation.
	 */
	public enum FileTransferProtocols
	{
		/**
		 * File transfer
		 */
		PROTOCOL_FILE,
		/**
		 * Download from or upload to an HTTP server
		 */
		PROTOCOL_FTP,
		/**
		 * Download from or upload to an HTTP server.
		 */
		PROTOCOL_HTTP,
        /**
		 * Download from or upload to an HTTPS server.
		 */
		PROTOCOL_HTTPS
	}

	// Fields of the FileTransferService Intent (Sent to initiate the service)
	/**
	 * The protocol of the transfer to be carried out (solely provided for backwards compatibility
	 * with EMML 1.0)
	 */
	public static final String TransferProtocol = "Protocol";

	/**
	 * Whether or not the destination of the transfer is the file system (Provided for backwards
	 * compatibility with EMML 1.0)
	 */
	public static final String FileDestination = "FileDestination";

	/**
	 * The port number for the transfer. HTTP, HTTPS and FTP only.
	 */
	public static final String Port = "Port";

	/**
	 * Whether or not the folder structure should be created on the local device to enable the
	 * transfer (download from remote or local file-->file only).
	 */
	public static final String CreateFolders = "CreateFolders";

	/**
	 * The source URL to be copied from (SPB2 and PB3 formats are accepted)
	 */
	public static final String Source = "Source";

	/**
	 * The destination URL to be copied from (SPB2 and PB3 formats are accepted)
	 */
	public static final String Destination = "Destination";

	/**
	 * Whether or not the existing file on the local file system can be overwritten (download from
	 * remote and local file --> file only).
	 */
	public static final String Overwrite = "Overwrite";

	/**
	 * The username to use for the HTTP, HTTPS or FTP server
	 */
	public static final String Username = "Username";

	/**
	 * The password to use for the HTTP, HTTPS or FTP server.
	 */
	public static final String Password = "Password";

	/**
	 * Intent filter which will be used to construct the intent returned to the caller. This should
	 * be a uniquely identifiable string so only the caller receives notifications that the transfer
	 * has completed
	 */
	public static final String IntentFilter = "IntentFilter";

	// Fields of the Intent returned from the File Transfer Service
	/**
	 * ReturnValue is whether or not the File Transfer succeeded, it will closely match the values
	 * returned by the FileTransfer plugin in Browser(differences occur where PB returned
	 * Windows error codes)
	 */
	public static final String ReturnValue = "ReturnValue";

	// Fields of the Intent going both to and from the File Transfer Service

	/**
	 * Transfer event is the user specified event associated with this transfer, e.g.
	 * FileTransferPlugin uses the TransferEvent. This event is passed back unchanged from the
	 * original data received by the caller, this is done to prevent the user changing the value of
	 * the transfer event during the transfer.
	 */
	public static final String TransferEvent = "TransferEvent";

	/**
	 * Return ID is the identifier given to this transfer by the caller. This enables callers to
	 * determine between successful transfers, e.g. the Buttons fetching images from an external
	 * site to be painted on them will need distinguishing
	 */
	public static final String ReturnID = "ReturnID";
	
	/**
	 * Copy is a flag to define whether file-to-file transfers should be copy (true) or moves
	 * (false).
	 */
	public static final String Copy = "Copy";

	/**
	 * Android uses the root of the SD card (or USB storage if there is no SD card) for what was
	 * referred to as file:// on Windows.
	 */
	private String rootFileDirectory = Environment.getExternalStorageDirectory().getAbsolutePath()
			+ "/";

	/**
	 * Default FTP port to use in transfers if not specified by the user
	 */
	private static final int g_iDefaultFTPPort = 21;

	/**
	 * Default HTTP port to use in transfers if not specified by the user.
	 */
	private static final int g_iDefaultHTTPPort = 80;

	/**
	 * Default HTTPS port to use in transfers if not specified by the user.
	 */
	private static final int g_iDefaultHTTPSPort = 443;

	/**
	 * Container holding threads started. Threads are added to this array on start and removed after
	 * the transfer is complete. If there are pending threads when the service is stopped they can
	 * be stopped.
	 */
	private ArrayList<RunningThreads> m_threads;
	
	public static final String PID = "pid"; //The process id is required because the response sent by this service would be caught by any process listening for it.
	private static final String TAG = "FileTransferService";

	/*
	 * (non-Javadoc)
	 * @see android.app.Service#onCreate()
	 */
	public void onCreate()
	{
		// Method called when the Service is created,
		m_threads = new ArrayList<RunningThreads>();
	}

	/*
	 * (non-Javadoc)
	 * @see android.app.Service#onStartCommand(android.content.Intent, int, int)
	 */
	public int onStartCommand(Intent intent, int flags, int startId)
	{
		// Overridden method called when the service is started. Data
		// associated with the transfer is stored in the intent.

		// Extract the transfer data from the Intent
		Bundle transferData = intent.getExtras();
		// Transfer attributes
		Boolean fileDestination = transferData.getBoolean(FileDestination);
		int port = transferData.getInt(Port);
		Boolean bCreateFolders = transferData.getBoolean(CreateFolders);
		String source = transferData.getString(Source);
		String destination = transferData.getString(Destination);
		Boolean bOverwrite = transferData.getBoolean(Overwrite);
		String username = transferData.getString(Username);
		String password = transferData.getString(Password);
		String transferEvent = transferData.getString(TransferEvent);
		// Class to receive the broadcast intent when the transfer is finished
		String intentFilter = transferData.getString("IntentFilter");
		// ID caller has associated with the return
		String returnID = transferData.getString(ReturnID);
		Boolean bCopy = transferData.getBoolean(Copy);
		
		Logger.I(TAG, "Starting FileTransfer >> source: " + source + ", destination: " + destination);

		// Start a thread to handle the transfer
		Thread transferThread;
		try
		{
			transferThread = new Thread(new TransferThread(fileDestination, port,
					bCreateFolders, source, destination, bOverwrite, bCopy, username, password, transferEvent,
					startId, intentFilter, returnID));
			
			// Maintain the list of running threads in case we are stopped mid transfer
			transferThread.start();
		}
		catch (Exception e)
		{
			//  An error has occured configuring the File Transfer service
			Logger.W(TAG, "File Transfer Failed, either source or destination were not specified correctly.");			
		}

		// Want this service to remain alive until the transfer is complete therefore
		// start sticky.
		return START_NOT_STICKY;
	}

	/*
	 * (non-Javadoc)
	 * @see android.app.Service#onDestroy()
	 */
	public void onDestroy()
	{
		// Called when the service finishes (either by its self or a client)
		// Stop all currently running threads
		Iterator<RunningThreads> arrIterator = m_threads.iterator();
		while (arrIterator.hasNext())
		{
			RunningThreads runningThread = ((RunningThreads) arrIterator.next());
			runningThread.getThread().interrupt();
		}
	}

	/*
	 * (non-Javadoc)
	 * @see android.app.Service#onBind(android.content.Intent)
	 */
	@Override
	public IBinder onBind(Intent arg0)
	{
		// We do not provide binding to this service
		return null;
	}
	
	/**
	 * Convenience method to parse protocol Strings into the FileTransferProtocols enum equivalent.
	 * (Case insensitive)
	 * @param inputProtocol The string to parse
	 * @return the appropriate FileTransferProtocols enum, or null if not found.
	 */
	public static FileTransferProtocols parseProtocol(String inputProtocol)
	{
		if(inputProtocol != null)
		{
			if(inputProtocol.equalsIgnoreCase("http")) return FileTransferProtocols.PROTOCOL_HTTP;
            else if (inputProtocol.equalsIgnoreCase("https")) return FileTransferProtocols.PROTOCOL_HTTPS;
			else if (inputProtocol.equalsIgnoreCase("ftp")) return FileTransferProtocols.PROTOCOL_FTP;
			else if (inputProtocol.equalsIgnoreCase("file")) return FileTransferProtocols.PROTOCOL_FILE;
		}
		return null;
	}
	
	/**
	 * Copies a file from one local position to another.
	 * @param sourceFile the file to copy.
	 * @param destFile to location to save the copy.
	 * @param overwrite whether the method is allowed to overwrite the destination file if it exists.
	 * @throws IOException if any file exceptions occur in transfer.
	 */
	public static boolean copyFile(File sourceFile, File destFile, boolean overwrite) throws IOException, IllegalArgumentException
	{
		if(!destFile.exists())//If it doesnt exist, create it.
		{
			destFile.createNewFile();
		}
		else//If it does...
		{
			if(overwrite)//And overwrite flag is set...
			{
				if(!destFile.delete())//try and delete it.
				{
					return false; //But error if the delete fails.
				}
				destFile.createNewFile();//create if it passes
			}
			else
			{
				return false; //File exists and we cant overwrite.
			}
		}

		FileChannel source = null;
		FileChannel destination = null;
		try
		{
			source = new FileInputStream(sourceFile).getChannel();
			destination = new FileOutputStream(destFile).getChannel();
			destination.transferFrom(source, 0, source.size());
			return true;
		}
		finally
		{
			if(source != null)
			{
				source.close();
			}
			if(destination != null)
			{
				destination.close();
			}
		}
	}

	private synchronized void addThread(int threadId, Thread transferThread)
	{
		m_threads.add(new RunningThreads(threadId, transferThread));
	}
	
	private synchronized void removeThread(int threadId)
	{
		Iterator<RunningThreads> arrIterator = m_threads.iterator();
		RunningThreads threadToRemove = null;
		while (arrIterator.hasNext())
		{
			RunningThreads runningThread = ((RunningThreads) arrIterator.next());
			if (threadId == runningThread.getStartID())
			{
				threadToRemove = runningThread;
				break;
			}
		}
		if (threadToRemove != null)
			m_threads.remove(threadToRemove);
		stopSelf(threadId);
			Logger.D(TAG, "FileTransfer threads still running: " + m_threads.size());
	}
	
	/**
	 * Class to encapsulate a running thread. This is required as we want to maintain a list of
	 * running threads in the system so we can shut them down if we are destroyed, however we want
	 * threads to be able to remove themselves from the list when they finish executing. startId is
	 * used as a unique identifier.
	 * @author Darryn Campbell, JRQ768. Febuary 2011, Initial Creation.
	 */
	
	private class RunningThreads
	{
		/**
		 * @param startId The Start ID associated with the running thread
		 * @param transferThread The Thread object associated with the transfer
		 */
		public RunningThreads(int startId, Thread transferThread)
		{
			m_startID = startId;
			m_Thread = transferThread;
		}

		public int getStartID()
		{
			return m_startID;
		}

		public Thread getThread()
		{
			return m_Thread;
		}

		private int m_startID;

		private Thread m_Thread;
	}

	/**
	 * Thread on which the file transfer will take place. Each transfer requested will take place on
	 * a separate thread, transfers will not be queued and may happen simultaneously.
	 * @author Darryn Campbell, JRQ768. February 2011, Initial Creation.
	 */
	private class TransferThread extends Thread
	{

		private static final String TAG = "FileTransferThread";

		/**
		 * Constructor for the TransferThread sets the member variables.
		 * @param protocol The Protocol on which the transfer should occur (File / FTP / HTTP / HTTPS)
		 * @param fileDestination Whether or not the destination is the local FS or not, provided
		 *            for compatibility with EMML 1.0 and should not be set explicitely by the user
		 * @param port The port for the transfer (HTTP / HTTPS / FTP only)
		 * @param bCreateFolders Whether or not the folder structure should be created on the local
		 *            FS
		 * @param source The [Fully Qualified] Source to use for the file. If the source starts with
		 * 			  a "/" it is considered absolute (i.e. root) and will be treated as such in
		 * 			  accordance to UNIX standards
		 * @param destination The [Fully Qualified] Destination to send the file to. If the 
		 * 		      destination starts with a "/" it is considered absolute (i.e. root) and will 
		 * 			  be treated as such in accordance to UNIX file naming standards
		 * @param bOverwrite Whether or not it is OK to overwrite a file on the local file system
		 * @param bCopy Whether or not to copy (true) or to move (false) files in file-to-file
		 * 			  transfers.
		 * @param username For FTP, HTTP or HTTPS this is the optional username to use.
		 * @param password For FTP, HTTP or HTTPS this is the optional password to use.
		 * @param transferEvent The event registered to receive the result of the transfer (e.g.
		 *            TransferEvent for the File Transfer Plugin)
		 * @param startID The start ID assigned to the Service by Android in the onStartCommand
		 *            override. Used as an identifier.
		 * @param intentFilter The filter of the intent which will be fired back once the transfer
		 *            has completed.
		 * @param returnID User assigned ID for the transfer so it can be distinguished if multiple
		 *            transfers are initiated.
		 * @throws Exception 
		 */
		TransferThread(boolean fileDestination, int port,
				Boolean bCreateFolders, String source, String destination, Boolean bOverwrite, 
				boolean bCopy, String username, String password, String transferEvent, int startID,
				String intentFilter, String returnID) throws Exception
		{
		    
            if (source == null || destination == null)
                return;
            // Android local file system uses '/' instead of '\\', do a universal replace for
            // compatibility with code written for Windows
            source = source.replace('\\', '/');
            destination = destination.replace('\\', '/');

            // setProtocol performs source/destination analysis in order to determine transfer protocol
            setProtocol(source, destination);

            // Set all the member variables for this transfer

			//this.m_bFileDestination = fileDestination;
			this.m_iPort = port;
			this.m_bCreateFolders = bCreateFolders;
            this.m_bOverwrite = bOverwrite;
			this.m_currentURL = Common.elementsCore.getCurrentUrl();

			m_currentURL = m_currentURL.replace('\\', '/');

			// retrieving the username, password and port if specified.

			this.m_bCopy = bCopy;
			if (username != null)
				this.m_username = username;
			if (password != null)
				this.m_password = password;
			this.m_transferEvent = transferEvent;
			this.m_startID = startID;
			this.m_intentFilter = intentFilter;
			this.m_returnID = returnID;
		}

		/*
		 * (non-Javadoc)
		 * @see java.lang.Runnable#run()
		 */
		@Override
		public void run()
		{
			//Add this thread to the thread list
			addThread(m_startID, this);
			
			String returnValue = "FileTransfer Error";
			// Local named constants to keep track of whether or not we should send an intent
			// back to the caller.
			int TRANSFER_ABORT = 0;
			int TRANSFER_SETUP_ERROR = 1;
			int TRANSFER_PROCEED = 2;

			int iPrerequisitesMet = TRANSFER_PROCEED;

			// If there is no source or destination specified then we can not continue
			if (m_source == null || m_destination == null)
			{
				Logger.W(TAG, "File Transfer Failed, either source or destination was not specified.");
				iPrerequisitesMet = TRANSFER_ABORT;
			}

			// Determine the direction
			if (m_bFileDestination)
			{
			    Logger.I(TAG, "File destination");
				if (m_bCreateFolders)
				{
					if (!CreatePath(m_destination))
					{
						Logger.D(TAG, "Failed to create path on local file system for destination (" + m_destination + ")");
						iPrerequisitesMet = TRANSFER_SETUP_ERROR;
					}
				}
				File existingFile = new File(m_destination);
				if (existingFile.exists() && !m_bOverwrite)
				{
					// Destination file exists and we should not overwrite it,
					// proceed no further
					Logger.D(TAG, "Failed to copy file ("	+ existingFile.getAbsolutePath() + "), File exists and Overwrite is set to False");
					returnValue = "Error: File Exists";
					iPrerequisitesMet = TRANSFER_SETUP_ERROR;
				}
				else if (existingFile.exists() && m_bOverwrite)
				{
					// Delete existing file
					if (!existingFile.delete())
					{
						Logger.D(TAG, "Failed to delete existing file (" + existingFile.getAbsolutePath() + ")");
						returnValue = "Error: Unable to Delete File";
						iPrerequisitesMet = TRANSFER_SETUP_ERROR;
					}
				}
			}

			// Only attempt the file transfer if we have not encountered an error up to this
			// point.
			if (iPrerequisitesMet == TRANSFER_PROCEED)
			{
				// The remaining logic depends on whether this is an FTP, HTTP, HTTPS or FILE transfer
				switch (m_protocol)
				{
					case PROTOCOL_HTTP:
		                Logger.I(TAG, "HTTP protocol");

		                if (m_iPort <= 0)
							m_iPort = g_iDefaultHTTPPort;

						if (!m_bFileDestination)
						{
							// HTTP is an Upload
							ElementsHTTP http = new ElementsHTTP(false, m_source, m_destination, m_iPort,
									m_username, m_password);
							returnValue = http.transfer();
							http.cleanProxySettings();
						}
						else
						{
							// HTTP is a download
							// Directory structure to the destination file has already been created
							// above
							// As well as consideration for whether or not the file should
							// be overwritten.
							ElementsHTTP http = new ElementsHTTP(true, m_source, m_destination, m_iPort,
									m_username, m_password);
							returnValue = http.transfer();
							http.cleanProxySettings();
						}

						break;
                        
                    case PROTOCOL_HTTPS:
                        
                        Logger.I(TAG, "HTTPS protocol");

		                if (m_iPort <= 0)
							m_iPort = g_iDefaultHTTPSPort;

						if (!m_bFileDestination)
						{
							// HTTPS is an Upload
							ElementsHTTPS https = new ElementsHTTPS(false, m_source, m_destination, m_iPort,
									m_username, m_password);
							returnValue = https.transfer();
							https.cleanProxySettings();
						}
						else
						{
							// HTTP is a download
							// Directory structure to the destination file has already been created
							// above
							// As well as consideration for whether or not the file should
							// be overwritten.
							ElementsHTTPS https = new ElementsHTTPS(true, m_source, m_destination, m_iPort,
									m_username, m_password);
							returnValue = https.transfer();
							https.cleanProxySettings();
						}
                    
                        break;

					case PROTOCOL_FTP:
                        Logger.I(TAG, "FTP protocol");

                        if (m_iPort <= 0)
							m_iPort = g_iDefaultFTPPort;

						if (!m_bFileDestination)
						{
							// FTP is an upload
							ElementsFTP ftp = new ElementsFTP(false, m_source, m_destination, m_iPort,
									m_username, m_password);
							returnValue = ftp.transfer();
						}
						else
						{
							// FTP is a download, just as with File and HTTP the
							// directory structure and overwrite exiting file? have already been
							// taken
							// care of above.
							ElementsFTP ftp = new ElementsFTP(true, m_source, m_destination, m_iPort,
									m_username, m_password);
							returnValue = ftp.transfer();
						}

						break;

					case PROTOCOL_FILE:
                        Logger.I(TAG, "File protocol");

                        // Copy/Move the file to the new location
						if ( (m_source != null) && (m_source.startsWith("file://")) )
							m_source = m_source.substring(7);
						File source = new File(m_source);
						if (!source.exists())
						{
							// The source file does not exist
							Logger.I(TAG, "Failed to copy from source file (" + m_source + "), File does not exist");
							returnValue = com.rho.rhoelements.transferprotocols.TransferProtocolInterface.ERROR 
											+ ": " +
											com.rho.rhoelements.transferprotocols.TransferProtocolInterface.FILENOTFOUNDEXCEPTION;
						}
						else
						{
							File existingFile = new File(m_destination);
							if(m_bCopy)
							{
								try
								{
									if(copyFile(source, existingFile, m_bOverwrite))
									{
										returnValue = "OK: File Copied"; 
									}
									else
									{
										returnValue = "Error: failed to write to destination";
									}
								}
								catch(IOException e)
								{
									Logger.W(TAG, "Failed to copy file (" + source.getAbsolutePath() + ") to (" + existingFile.getAbsolutePath() + ")");
									returnValue = "Error: Failed to Copy File";
								}
								catch(IllegalArgumentException e)
								{
									Logger.W(TAG, "Failed to copy file (" + source.getAbsolutePath() + ") to (" + existingFile.getAbsolutePath() + ")");
									returnValue = "Error: Failed to Copy File";
								}
							}
							else
							{
								if (!source.renameTo(existingFile))
								{
									// 	An error has occured moving the file
									Logger.W(TAG, "Failed to move file (" + source.getAbsolutePath() + ") to (" + existingFile.getAbsolutePath() + ")");
									returnValue = "Error: Failed to Move File";
								}
								else
								{
									returnValue = "OK: File Moved";
								}
							}
						}
						break;
				}
			}

			if (iPrerequisitesMet != TRANSFER_ABORT)
			{
				// The return intent is sent back to the caller as a broadcase intent
				// to notify them that we have finished the File Transfer. onReceive
				// in Android will be called within the main thread of the process.
				Intent returnIntent = new Intent(m_intentFilter);
				returnIntent.putExtra(FileTransferService.ReturnValue, returnValue);
				returnIntent.putExtra(FileTransferService.ReturnID, m_returnID);
				returnIntent.putExtra(FileTransferService.TransferEvent, m_transferEvent);
				returnIntent.putExtra(PID, android.os.Process.myPid());
				sendBroadcast(returnIntent);
			}

			// Remove ourself from the list of running threads
			removeThread(m_startID);
		}

        /**
         * Set the member variables m_protocol, m_source, m_destination taking
         * account of the default value (File)
         * 
         * @param protocol
         */
        private void setProtocol(String source, String destination) {
            if (source == null || source.length() == 0 || destination == null || destination.length() == 0) {
                throw new RuntimeException("Filetransfer: no source or destination are specified.");
            }

            // Determine protocol
            if (source.startsWith(FTP_PREFIX) || destination.startsWith(FTP_PREFIX)) {
                m_protocol = FileTransferProtocols.PROTOCOL_FTP;
            } else if ((source.startsWith(HTTPS_PREFIX) || destination.startsWith(HTTPS_PREFIX))) {
                m_protocol = FileTransferProtocols.PROTOCOL_HTTPS;
            } else if (source.startsWith(HTTP_PREFIX) || destination.startsWith(HTTP_PREFIX)) {
                m_protocol = FileTransferProtocols.PROTOCOL_HTTP;
            } else {
                m_protocol = FileTransferProtocols.PROTOCOL_FILE;
            }

            //Fixing a stupid SR saying if the source is "ftp:// then we dont return an error. 
            if (source.equalsIgnoreCase("ftp://")) {
                  m_bFileDestination = true;
                  m_source = "";
            }
    
            // Source may be specified as a relative or an absolute URL,
            // convert relative URLs to absolute URLs.
            // Dereferenced source is the URL to use as the source
            // (accounting for if a relative path was specified)
            String dereferencedSource = null;
            if (IsRelativeURL(source)) {
                  dereferencedSource = DereferenceURL(source, m_currentURL);
                  if (dereferencedSource == null)
                      throw new RuntimeException("Filetransfer: wrong source: " + source);
            }
            else {
                  dereferencedSource = source;
            }
            // Stripped source is a concatenation of the server address and the
            // file name / file path. It is stripped of username / password /
            // protocol & port.
            String strippedSource = null;
            String sourceProtocol = getProtocolFromURL(dereferencedSource);
    
            if (sourceProtocol.equalsIgnoreCase("http") || sourceProtocol.equalsIgnoreCase("https") || sourceProtocol.equalsIgnoreCase("ftp")) {
                  m_bFileDestination = true;
                  strippedSource = ParseURLAttributes(dereferencedSource);
                  if (strippedSource == null)
                      throw new RuntimeException("Filetransfer: wrong source: " + source);
            }
            else if (sourceProtocol.equalsIgnoreCase("file") || sourceProtocol.equals("")) {
            //m_protocol should always be set at this point, PROTOCOL_FILE is the default
                  try
                  {
                      dereferencedSource = Common.parseAndroidURI(dereferencedSource, true).getPath();
                  } 
                  catch (InvalidParameterException e)
                  {
                      Logger.E(TAG, e.getMessage());
                  } 
                  catch (URISyntaxException e)
                  {
                      Logger.E(TAG, e.getMessage());
                  }
                  strippedSource = dereferencedSource;
            }
            else if (sourceProtocol.equals("error")) {
                throw new RuntimeException("Filetransfer: wrong source: " + source);
            }
            else {
                  strippedSource = source;
            }
    
            // Populate the source member variable
            m_source = strippedSource;

            // destination may be specified as a relative or absolute URL,
            // convert relative URLs to absolute URLs.
            // Dereferenced destination is the URL to use as the destination
            // (accounting for if a relative path was specified)
            String dereferencedDestination = null;
            
            //Decode the URL
            dereferencedDestination = Uri.decode(destination);
            
            if (IsRelativeURL(destination))
            {
                dereferencedDestination = DereferenceURL(destination, m_currentURL);
                if (dereferencedDestination == null)
                    throw new RuntimeException("Filetransfer: wrong destination: " + destination);
            }
            
            // Stripped destination is a concatenation of the server address
            // and the filename / file pagh. It is stripped of the username /
            // password / protocol & port
            String strippedDestination = null;
            String destinationProtocol = getProtocolFromURL(dereferencedDestination);
    
            if (destinationProtocol.equalsIgnoreCase("http") || destinationProtocol.equalsIgnoreCase("https") || destinationProtocol.equalsIgnoreCase("ftp")) {
                m_bFileDestination = false;
                strippedDestination = ParseURLAttributes(dereferencedDestination);
                if (strippedDestination == null)
                    throw new RuntimeException("Filetransfer: wrong destination: " + destination);
            }
            else if (destinationProtocol.equalsIgnoreCase("file") || destinationProtocol.equals("")) {
                try {
                    dereferencedDestination = Common.parseAndroidURI(dereferencedDestination, true).getPath();
                } 
                catch (InvalidParameterException e) {
                    Logger.E(TAG, e.getMessage());
                } 
                catch (URISyntaxException e) {
                    //URI does not allow spaces, so process it here 
                    //Strip off File:// protocol string and add sdcard /mnt/sdcard to the destination
                    dereferencedDestination = Environment.getExternalStorageDirectory().getPath() + 
                            dereferencedDestination.substring(7);                   
                }
                strippedDestination = dereferencedDestination;
                //  Paths specified absolutely (/mnt/sdcard) will reach here, therefore set file destination true.
                m_bFileDestination = true;
            }
            else if (destinationProtocol.equals("error")) {
                throw new RuntimeException("Filetransfer: wrong destination: " + destination);
            }
    
            // Set the member variable as the parsed, stripped destination
            m_destination = strippedDestination;
        }

		/**
		 * Creates the directory structure required to create the specified file.
		 * @param filePath Fully qualified file name and file path in which to create the file.
		 * @return Whether or not the file was successfully created.
		 */
		private boolean CreatePath(String filePath)
		{
			// In Android this path is created from the root of SD Card
			// If there are no "/" in the file path then there is nothing to do
			if (filePath.indexOf("/") == -1)
				return true;

			String directoryPath = new String(filePath);
			// Strip the file name from the end
			directoryPath = directoryPath.substring(0, directoryPath.lastIndexOf('/'));
			File createDirectoryHere = new File(directoryPath + "/");
			if (createDirectoryHere.exists())
				return true;
			return createDirectoryHere.mkdirs();
		}

		/**
		 * Returns whether or not the passed URL was a relative relative URL (e.g. '../../')
		 * @param url The URL being queried
		 * @return If the URL was relative
		 */
		private boolean IsRelativeURL(String url)
		{
			if ( (url.startsWith(".")) || (url.contains("//.")) )  
				return true;
			return false;
				
			 
		}

		/**
		 * Given a relative URL this function will convert it into a fully qualified URL using the
		 * directory of the current page in the WebView.
		 * @param relativeURL The relative URL to parse (e.g. ../file.htm)
		 * @param currentURL The current page being displayed in the browser, e.g.
		 *            http://web.com/dir/page.htm
		 * @return A fully qualified version of the passed relative URL, e.g.
		 *         http://web.com/file.htm
		 */
		private String DereferenceURL(String relativeURL, String currentURL)
		{
			// Work out how many lavels we need to navigate up from the current URL
			// (specified as ../)
			int iLevelsUp = 0;
			String relativeURLCpy = relativeURL;
			while (relativeURLCpy.indexOf("../") > -1)
			{
				iLevelsUp++;
				relativeURLCpy = relativeURLCpy.substring(relativeURLCpy.indexOf("../") + 3,
						relativeURLCpy.length());
			}

			// We now know how many levels up we want to go from the current URL.
			// Starting at the end of the current URL search for '/' and
			// work out if we can go up that many levels

			// We want to ignore the protocol specified in the current URL
			String currentURLProtocol = getProtocolFromURL(currentURL);
			if (currentURLProtocol.length() == 0)
			{
				Logger.W(TAG, "Unable to determine current URL protocol, aborting file transfer");
				return null;
			}
			int iLengthOfProtocol = currentURLProtocol.length() + 3;
			// Navigate the current URL backwards counting the number of slashes
			int iPosInCurrentURL;
			for (iPosInCurrentURL = currentURL.length() - 1; iPosInCurrentURL > iLengthOfProtocol; iPosInCurrentURL--)
			{
				if (currentURL.charAt(iPosInCurrentURL) == '/')
					iLevelsUp--;
				if (iLevelsUp == -1)
				{
					// iPosInCurrentURL is the character in the current URL after which we want to
					// paste our relative URL
					break;
				}
			}

			// If we exit the above loop and 'levelsUp' is not -1 then there were
			// insufficient slashes in the current URL to go up the number of
			// levels specified in the relative URL.
			if (iLevelsUp != -1)
			{
				Logger.W(TAG, "Invalid Relative URL given (" + relativeURL + ")");
				return null;
			}

			// The actual URL we require is the juxtaposition of the current URL
			// up to 'iPosInCurrentURL' with the first non . | / | \ character
			// in the relative URL
			int iFirstNonRelativeCharacter = 0;
			for (int i = iFirstNonRelativeCharacter; i < relativeURL.length(); i++)
			{
				if (relativeURL.charAt(i) == '.' || relativeURL.charAt(i) == '/')
					iFirstNonRelativeCharacter++;
				else
					break;
			}
			if (iFirstNonRelativeCharacter == relativeURL.length())
			{
				Logger.W(TAG, "Invalid Relative URL given (" + relativeURL + ")");
				return null;
			}

			// Concatenate the absolute directory with the page / directory from the relative URL
			String output = currentURL.substring(0, iPosInCurrentURL + 1);
			output = output.concat(relativeURL.substring(iFirstNonRelativeCharacter));
			if (output.length() > Common.MAX_URL)
			{
				Logger.W(TAG, "Unable to dereference relative URL, resultant URL would be too long");
				return null;
			}

			return output;
		}

		/**
		 * Extract the protocol from the given URL
		 * @param url the URL to extract the protocol from
		 * @return The protocol being specified, either File, HTTP, HTTPS or FTP
		 */
		private String getProtocolFromURL(String url)
		{
			if (url.length() < FILE_PREFIX.length() &&
					!url.startsWith("/"))
				return "error";
			else if (url.toLowerCase().startsWith(FILE_PREFIX))
				return "file";
            else if (url.toLowerCase().startsWith(HTTPS_PREFIX))
                return "https";
			else if (url.toLowerCase().startsWith(HTTP_PREFIX))
				return "http";
			else if (url.toLowerCase().startsWith(FTP_PREFIX))
				return "ftp";
			else
				return "";
		}

		/**
		 * Extract from a fully qualified URL the username, password and port being specified. Note
		 * this function is fail safe, if some or none of the attributes are specified they will not
		 * be set.
		 * @param fullyQualifiedURL The URL to be cracked, specified as:
		 *            protocol://username:password@hostname:port/directories
		 * @return The cracked URL with any protocol and attributes removed, e.g.
		 *         hostname/directories.
		 */
		private String ParseURLAttributes(String fullyQualifiedURL)
		{
			// Set The Username, Password, Port and return the Server URL
			// Pattern of a URL is protocol://username:password@hostname:port/directories

			// The dereferenced URL is returned.
			String returnURL = null;

			// Advance past the protocol
			if (fullyQualifiedURL.indexOf("://") > -1)
			{
				fullyQualifiedURL = fullyQualifiedURL
						.substring(fullyQualifiedURL.indexOf("://") + 3);
			}

			// Work out if we have a Username and Password
			int iPositionOfAtSymbol = fullyQualifiedURL.indexOf("@");
			if (iPositionOfAtSymbol > -1)
			{
				// Fully qualified URL contains a username and password
				// If the URL contains a username with no password we will not have a ':'
				if (fullyQualifiedURL.substring(0, iPositionOfAtSymbol).indexOf(":") > -1)
				{
					// Fully Qualified URL contains both username and password
					// Format is username:password
					String usernameAndPassword = fullyQualifiedURL
							.substring(0, iPositionOfAtSymbol);
					int iPositionOfColon = usernameAndPassword.indexOf(":");
					m_username = usernameAndPassword.substring(0, iPositionOfColon);
					m_password = usernameAndPassword.substring(iPositionOfColon + 1,
							usernameAndPassword.length());
				}
				else
				{
					// Fully Qualified URL contains only a username
					m_password = null;
					m_username = fullyQualifiedURL.substring(0, iPositionOfAtSymbol);
				}

				// Advance past the username and password and @ symbol
				fullyQualifiedURL = fullyQualifiedURL.substring(iPositionOfAtSymbol + 1);
			}

			// fullyQualifiedURL now contains the hostname, port (if specified) and directories
			// Work out if a port is specified
			int iPositionOfColon = fullyQualifiedURL.indexOf(":");
			if (iPositionOfColon > -1)
			{
				// The port number has been specified, parse this out
				// (A '/' will appear directly after the number)
				int iPositionOfSlash = fullyQualifiedURL.indexOf("/");
				if (iPositionOfSlash < 0)
				{
					Logger.D(TAG, "Whilst parsing URL for port, unable to determine begginning of file name / directory.  Aborting File Transfer");
					returnURL = null;
				}
				else if (iPositionOfSlash < iPositionOfColon)
				{
					// Colon found was part of the filename or directory structure, would not
					// expect to see this
					Logger.D(TAG, "Whilst parsing URL for port, colon found as part of file name / directory.  Aborting file transfer");
					returnURL = null;
				}
				else
				{
					// port number is between ':' and '/'
					String sPort = fullyQualifiedURL.substring(iPositionOfColon + 1,
							iPositionOfSlash);
					try
					{
						m_iPort = Integer.parseInt(sPort);
					}
					catch (NumberFormatException e)
					{
						Logger.D(TAG, "Unable to Parse port " + sPort + " to number, ignoring this attribute");
					}

					returnURL = fullyQualifiedURL.substring(0, iPositionOfColon);
					returnURL += fullyQualifiedURL.substring(iPositionOfSlash,
							fullyQualifiedURL.length());
				}
			}
			else
			{
				// A port number has not been specified, the remainder of the URL is server URL and
				// file name
				returnURL = fullyQualifiedURL;
			}

			return returnURL;
		}
		
		/**
		 * The protocol of the transfer. Note one end of the transfer will always be FILE so this
		 * specifies the other end, i.e you can not do a HTTP --> FTP transfer.
		 */
		private FileTransferProtocols m_protocol;

		/**
		 * Whether or not the destination is the local file system
		 */
		private boolean m_bFileDestination;

		/**
		 * The port to use for the transfer for HTTP / HTTPS / FTP
		 */
		private int m_iPort;

		/**
		 * Whether or not a folder structure should be created when storing the file locally
		 */
		private boolean m_bCreateFolders;

		/**
		 * The source of the file
		 */
		private String m_source;

		/**
		 * The destination to send the file to
		 */
		private String m_destination;

		/**
		 * Whether or not the file can be overwritten on the local file system
		 */
		private boolean m_bOverwrite;

		/**
		 * The username to be used for HTTP / HTTPS / FTP transfers
		 */
		private String m_username;

		/**
		 * The password to be used for HTTP / HTTPS / FTP transfers.
		 */
		private String m_password;
		
		/**
		 * Whether or not the file --> file transfer is a copy (true) or a move (false)
		 */
		private boolean	m_bCopy;

		/**
		 * The current URL of the page being displayed in the browser.
		 */
		private String m_currentURL;

		/**
		 * The filter of the intent to be passed back to the caller
		 */
		private String m_intentFilter;

		/**
		 * The ID specified by the user so they can distinguish which transfer this is if they
		 * started multiple transfers.
		 */
		private String m_returnID;

		/**
		 * The ID of the Android service which started this transfer
		 */
		private int m_startID;

		/**
		 * The Transfer event specified by the user associated with this transfer (e.g.
		 * SignatureSaveEvent for Signature Capture)
		 */
		private String m_transferEvent;
	}
}
