/**
 * Video Capture Plugin
 */
package com.rho.rhoelements.plugins;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.AssetFileDescriptor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.provider.MediaStore;
import android.view.KeyEvent;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rho.rhoelements.services.FileTransferService;
import com.rhoelementsext.RhoElementsProxy;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author pxb743
 *
 */
public class VideoCapturePlugin extends Plugin {
    
	private int duration = 5000; //Default - 20 seconds
	//private int maximumSize = 10000; //Default - 10MB
	private String name = "VideoCapture";	//Default file name
	private String path = Common.getDataPath() + "/" + name + ".mp4"; 
	private String userName = null;
	private String password = null;
	private String destination = null;
	private String mCaptureEvent = null;
	protected FileTransferService.FileTransferProtocols mProtocol = null;

    private VideoTransferBroadcastReceiver mReceiver;
    public static final String VIDEO_PLUGIN_TRANSFER_COMPLETE = 
    		"com.rho.rhoelements.plugins.VideoCapturePlugin.TRANSFER_COMPLETE";
    public static final String Video_TRANSFER_RETURN_ID = "VideoReturnID";
	private static final String TAG = "VideoCapturePlugin";
    private Handler mHandler;
	
	
    /**
     * Video Capture plugin constructor
     */
    public VideoCapturePlugin()
    {
    	IntentFilter intentFilter = new IntentFilter(VIDEO_PLUGIN_TRANSFER_COMPLETE);
		mReceiver = new VideoTransferBroadcastReceiver();
		Common.mainActivity.registerReceiver(mReceiver, intentFilter);
		mHandler = new Handler();
    }
    
    /**
	 * Class to receive the intent sent from the FileTransferService when the File Transfer
	 * has completed.  We configure the intent in such a way so that we are the only recipient.
	 * @author John Heywood, VBC863.  Initial Creation, March 2011
	 * @author Harish Pulimi, PXB743. Modified, October 2012
	 */
	public class VideoTransferBroadcastReceiver extends BroadcastReceiver
	{
		/* (non-Javadoc)
		 * @see android.content.BroadcastReceiver#onReceive(android.content.Context, android.content.Intent)
		 */
		@Override
		public void onReceive(Context arg0, Intent receivedIntent) {
			Bundle b = receivedIntent.getExtras();
			//  Extract the values associated with the return Intent
			//  File Transfer does not use return ID
			String returnID = b.getString(FileTransferService.ReturnID);
			final String returnValue = b.getString(FileTransferService.ReturnValue);
			final String transferEvent = b.getString(FileTransferService.TransferEvent);			

			//NOTE: To match with Windows behavior
			//If the return was positive (the transaction completed successfully) 
			/*if(returnValue.contains("ok") || returnValue.contains("OK") || returnValue.contains("Ok") || returnValue.contains("oK"))
			{
				File file = new File(path);
				file.delete();
			}*/
			
			if (returnID.equals(Video_TRANSFER_RETURN_ID))
			{
				mHandler.post(new Runnable()
				{
					@Override
					public void run()
					{
						Logger.I(TAG, transferEvent + " : " + returnValue);
						//  Navigate to the specified URL
						if (transferEvent != null && returnValue != null)
						{
							try
							{
								navigate(transferEvent, "transferResult", returnValue);
							}
							catch (NavigateException e)
							{
								Logger.E(TAG, "Navigate Exception.. length is " + e.GetLength());
							}
						}
						else if (transferEvent != null)
							navigate(transferEvent);	
					}
				});
			}
		}
	}
	
	/**
	 * Routine for File Transfer
	 */
	private void transferFile()
	{
		dumpParams();
		//source includes /mnt/sdcard/ which would be prefixed again by the file transfer service, therefore we need to delete it
		String source = path;
		int sdcardPathPos = path.indexOf("/mnt/sdcard/");
		if (sdcardPathPos == 0)
			source = path.substring(12); //12 = length("/mnt/sdcard/")
		
		FileTransferService.FileTransferProtocols protocol = null;
		
		try
		{
			URL destinationUrl = new URL(destination);
			protocol = FileTransferService.parseProtocol(destinationUrl.getProtocol());
		
			if (protocol != null)
			{
				//  Create the intent to initialise the FileTransferService, this will 
				//  contain all attributes applicable to the transfer such as source,
				//  destination etc.  The user does not have to specify all fields but
				//  if they have provided insufficient information the transfer will
				//  not succeed.  The fields which need to be specified are identical to Browser
				Intent transferIntent = new Intent(Common.mainActivity, FileTransferService.class);
				//  Add data to the intent
				transferIntent.putExtra(FileTransferService.TransferProtocol, protocol);
				transferIntent.putExtra(FileTransferService.FileDestination, false);
				transferIntent.putExtra(FileTransferService.CreateFolders, true);
				transferIntent.putExtra(FileTransferService.Port, destinationUrl.getPort());
				transferIntent.putExtra(FileTransferService.Source, source);
				transferIntent.putExtra(FileTransferService.Destination, destination);
				transferIntent.putExtra(FileTransferService.Overwrite, true);
				transferIntent.putExtra(FileTransferService.Copy, true);
				transferIntent.putExtra(FileTransferService.Username, userName);
				transferIntent.putExtra(FileTransferService.Password, password);
				transferIntent.putExtra(FileTransferService.TransferEvent, mCaptureEvent);
				//  Specify our Broadcast receiver to be notified when the File Transfer has finished
				transferIntent.putExtra(FileTransferService.IntentFilter, VIDEO_PLUGIN_TRANSFER_COMPLETE);
				//  Return ID allows us to distinguish between responses from the FileTransferService
				transferIntent.putExtra(FileTransferService.ReturnID, Video_TRANSFER_RETURN_ID);
				//  Start the File Transfer Service
				Common.mainActivity.startService(transferIntent);
			}		
		}
		catch (MalformedURLException e)
		{
			Logger.E(TAG, "Passed URL is not formatted correctly");
		}
	}
	
	/**
	 * Log the parameters
	 */
	private void dumpParams()
	{
		Logger.I(TAG, "Name: " + this.name);		
		Logger.I(TAG, "Destination: " + this.destination);
		Logger.I(TAG, "Username: " + this.userName);
		Logger.I(TAG, "Password: " + this.password);
		Logger.I(TAG, "captureEvent: " + this.mCaptureEvent);		
	}
	
	/**
	 * Using Android Intent to start the video capture
	 */
	private void start()
	{
		//Common.elementsCore.isVideoCapture = true;
		int durationInSeconds = duration / 1000;							
	    try{
		Intent takeVideoIntent = new Intent(MediaStore.ACTION_VIDEO_CAPTURE);				
		takeVideoIntent.putExtra(android.provider.MediaStore.EXTRA_VIDEO_QUALITY, 0.5);	//Medium Quality			
		takeVideoIntent.putExtra(android.provider.MediaStore.EXTRA_DURATION_LIMIT, durationInSeconds);
		//takeVideoIntent.putExtra(android.provider.MediaStore.EXTRA_OUTPUT, path);
		//takeVideoIntent.putExtra(android.provider.MediaStore.EXTRA_SIZE_LIMIT, maximumSize); //KB
		
		Common.elementsCore.RESULT_VIDEO_CODE = RhoExtManager.getInstance().getActivityResultNextRequestCode(RhoElementsProxy.getInstance());
		Common.mainActivity.startActivityForResult(takeVideoIntent, Common.elementsCore.RESULT_VIDEO_CODE);				
		//Check for result in CommonActivity
	       }catch(Exception e)
	       {
	       	 Logger.E(TAG, "While Opening VideoCapture"+ e.getMessage());	
	       }
	}

	/**
     * Stop the media player gracefully and transfer the file
     * This will be called from ElementsActivity -> ElementsCore
     */
    public void stop(Uri intentData)
    {    			
    	// Copy the video contents from the intent to the local file specified
    	try {
		    AssetFileDescriptor videoAsset = Common.mainActivity.getApplicationContext().getContentResolver().openAssetFileDescriptor(intentData, "r");
		    FileInputStream fis = videoAsset.createInputStream();		    
		    File tmpFile = new File(path); 
		    FileOutputStream fos = new FileOutputStream(tmpFile);

		    byte[] buf = new byte[1024];
		    int len;
		    while ((len = fis.read(buf)) > 0) {
		        fos.write(buf, 0, len);
		    }
		    fis.close();
		    fos.close();
		    
		    //Delete the file from the Gallery
		    Common.mainActivity.getApplicationContext().getContentResolver().delete(intentData, null, null);   
		    
		  } catch (IOException io_e) {
		    io_e.printStackTrace();
		  }	
    	
    	//We should transfer the file when we get the callback from the startActivity()
    	transferFile();    	
    }
    

	/** Method called when the App gets Background focus event */
	@Override
	public void onBackground(String url)
	{		
		//  An Application focus change is received 
		//  Application has gone into the background.		
		
		//TODO: Do we need to handle this event?
		Logger.D(TAG, "Application background event received by Video Capture plugin");		
	}
	
	@Override
	public void onPageStarted(String str)
	{		
		duration = 5000; //Default - 5 seconds
		//maximumSize = 10000; //default - 10MB
		name = "VideoCapture";	//Default file name
		path = Common.getDataPath() + "/" + name + ".mp4"; 
		userName = null;
		password = null;
		destination = null;
		mCaptureEvent = null;
		mProtocol = null;
	}
	
	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onShutdown()
	 */
	@Override
	public void onShutdown() {	
		
		if (mReceiver != null)
	    {
			Common.mainActivity.unregisterReceiver(mReceiver);
	    }
	}	
	
	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onSetting(com.rho.rhoelements.PluginSetting)
	 */
	@Override
	public void onSetting(PluginSetting setting) {
		if (setting.getValue().length() > 0)
			Logger.I(TAG, String.format("'%s', '%s'", setting.getName(), setting.getValue()));
		else
			Logger.I(TAG, setting.getName());
		
		if (setting.getName().equalsIgnoreCase("duration"))
		{
			try
			{
				duration = Integer.parseInt(setting.getValue());
				if(duration < 1000)
				{
					duration = 5000; //Default
					Logger.W(TAG, "Setting default duration 5000");
				}				
			}
			catch (Exception e)
			{
				Logger.W(TAG, "Invalid duration '" + setting.getValue() + "'");
			}
		}	
		/*else if (setting.getName().equalsIgnoreCase("maximumSize"))
		{
			try
			{
				maximumSize = Integer.parseInt(setting.getValue());
				if(maximumSize < 0)
				{
					maximumSize = 10000; //Default
					Logger.W(TAG, "Setting default maximumSize 10000");
				}				
			}
			catch (Exception e)
			{
				Logger.W(TAG, "Invalid maximumSize '" + setting.getValue() + "'");
			}
		}*/	
		else if (setting.getName().equalsIgnoreCase("name"))
		{
			name = setting.getValue();
			if (name != null)
			{
				if (name.contains("."))
				{
					path = Common.getDataPath() + "/" + name;
				}
				else
				{
					path = Common.getDataPath() + "/" + name + ".mp4";
				}
			}
		}
		else if (setting.getName().equalsIgnoreCase("username"))
		{
			String uname = setting.getValue();
			if (uname.equals(""))
			{
				userName = null;
			}
			else
			{
				userName = uname;
			}		
		}
		else if (setting.getName().equalsIgnoreCase("password"))
		{
			String pword = setting.getValue();
			if (pword.equals(""))
			{
				password = null;
			}
			else
			{
				password = pword;
			}		
		}
		else if (setting.getName().equalsIgnoreCase("destination"))
		{
			String value = setting.getValue();
			if(value != null && value.toLowerCase().startsWith("url:")) value = value.substring(4); //Remove the new url: prefix
			
			try
			{
				URL uDestination = new URL(value);
				String dpath = uDestination.getPath();
				if(!dpath.substring(Math.max(dpath.lastIndexOf('/'), dpath.lastIndexOf('\\')) + 1).contains("."))
				{
					destination = value + ".mp4";
				}
				else
				{
					destination = value;
				}
			}
			catch (MalformedURLException e)
			{
				Logger.W(TAG, "Bad destination URL.");	
			}
			catch(IndexOutOfBoundsException e)
			{
				Logger.W(TAG, "Bad destination URL.");
			}		
		}		
		else if (setting.getName().equalsIgnoreCase("Videosaveevent"))
		{
		    mCaptureEvent = setting.getValue();
		}
		else if (setting.getName().equalsIgnoreCase("start"))
		{
			if (Common.elementsCore.bLaunchingAppHasFocus)
			{	
				start();
			}
		}
		else if (setting.getName().equalsIgnoreCase("stop"))
		{
			//Ignore as the stop button is displayed on the video recoridng window
		}
		else if (setting.getName().equalsIgnoreCase("cancel"))
		{			
			Common.mainActivity.dispatchKeyEvent(new KeyEvent(KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_BACK));
		}
		else
		{
			Logger.W(TAG, "Unrecognised parameter " + setting.getName());
		}
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
	    return new Version ("VideoCapture"); //$NON-NLS-1$
	}
}
