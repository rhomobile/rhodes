/**
 * Audio Capture plugin
 */
package com.rho.rhoelements.plugins;

import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.MediaRecorder;
import android.media.MediaRecorder.OnErrorListener;
import android.media.MediaRecorder.OnInfoListener;
import android.os.Bundle;
import android.os.Handler;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rho.rhoelements.services.FileTransferService;
import com.rhomobile.rhodes.Logger;

/**
 * @author pxb743
 *
 */
public class AudioCapturePlugin extends Plugin {

	private int duration = 20000; //Default - 20 seconds
	//private int maximumSize = 10000; //Default - 10MB
	private String name = "AudioCapture.mp4";	//Default file name
	private String path; 
	private String userName = null;
	private String password = null;
	private String destination = null;
	private String mCaptureEvent = null;	
	private static final String	CODEC_AAC = "AAC";
	private static final String	CODEC_AMR_NB = "AMR_NB";
	private static final String	CODEC_AMR_WB = "AMR_WB";
	private static final String	CODEC_WAV = "WAV";
	private int format = MediaRecorder.OutputFormat.MPEG_4;
	private int codec = MediaRecorder.AudioEncoder.AAC;		
	private MediaRecorder recorder = null;
    private boolean isPlaying = false;
	
	protected FileTransferService.FileTransferProtocols mProtocol = null;    
    private AudioTransferBroadcastReceiver mReceiver;
    public static final String AUDIO_PLUGIN_TRANSFER_COMPLETE = 
    		"com.rho.rhoelements.plugins.AudioCapturePlugin.TRANSFER_COMPLETE";
    public static final String AUDIO_TRANSFER_RETURN_ID = "AudioReturnID";
	private static final String TAG = "AudioCapturePlugin";
    private Handler mHandler;
   
    /**
     * Audio Capture constructor
     */
    public AudioCapturePlugin() 
    {
        IntentFilter intentFilter = new IntentFilter(AUDIO_PLUGIN_TRANSFER_COMPLETE);
		mReceiver = new AudioTransferBroadcastReceiver();
		Common.mainActivity.registerReceiver(mReceiver, intentFilter);
		mHandler = new Handler();
		
    }
    
    /**
	 * Class to receive the intent sent from the FileTransferService when the File Transfer
	 * has completed.  We configure the intent in such a way so that we are the only recipient.
	 * @author John Heywood, VBC863.  Initial Creation, March 2011
	 * @author Harish Pulimi, PXB743. Modified, October 2012
	 */
	public class AudioTransferBroadcastReceiver extends BroadcastReceiver
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
			
			if (returnID.equals(AUDIO_TRANSFER_RETURN_ID))
			{
				mHandler.post(new Runnable()
				{
					@Override
					public void run()
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, transferEvent + " : " + returnValue));
						//  Navigate to the specified URL
						if (transferEvent != null && returnValue != null)
						{
							try
							{
								navigate(transferEvent, "transferResult", returnValue);
							}
							catch (NavigateException e)
							{
								Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
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
				transferIntent.putExtra(FileTransferService.IntentFilter, AUDIO_PLUGIN_TRANSFER_COMPLETE);
				//  Return ID allows us to distinguish between responses from the FileTransferService
				transferIntent.putExtra(FileTransferService.ReturnID, AUDIO_TRANSFER_RETURN_ID);
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
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Name: " + this.name));		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Destination: " + this.destination));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Username: " + this.userName));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Password: " + this.password));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "captureEvent: " + this.mCaptureEvent));		
	}
	
	/**
	 * Routine to start the audio recording
	 */
	private void start()
	{
    	if (recorder != null)
    	{
    		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Audio Capture already in progress"));
    		return;
    	}
    	
		path = "";
		if (name.contains("."))
		{
			path = Common.getDataPath() + "/" + name;
		}
		else 
		{
			if (format == MediaRecorder.OutputFormat.MPEG_4)
				path = Common.getDataPath() + "/" + name + ".mp4";
			else if (format == MediaRecorder.OutputFormat.THREE_GPP)
				path = Common.getDataPath() + "/" + name + ".3gpp";
		}	

    	recorder = new MediaRecorder();
    	
    	//defaults
    	recorder.setAudioSource(MediaRecorder.AudioSource.MIC);    	
    	recorder.setOutputFormat(format);
        recorder.setOutputFile(path);
        recorder.setAudioEncoder(codec);
        recorder.setMaxDuration(duration); // 20 seconds
        //recorder.setMaxFileSize(maximumSize * 1024); // Approximately 10 megabytes
        
        try {
			recorder.prepare();
			recorder.start();
			isPlaying = true;
		} catch (IllegalStateException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} 
		
        recorder.setOnInfoListener(new OnInfoListener() {
        	@Override
        	public void onInfo(MediaRecorder mr, int what, int extra) {     
        		switch (what)
        		{
        		case MediaRecorder.MEDIA_RECORDER_INFO_MAX_DURATION_REACHED:
        			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Maximum duration reached for the audio capture"));
        			stop(true);
        			break;
        		case MediaRecorder.MEDIA_RECORDER_INFO_MAX_FILESIZE_REACHED:
        			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Maximum file size reached for the audio capture"));
        			stop(true);
        			break;
        		case MediaRecorder.MEDIA_RECORDER_INFO_UNKNOWN:
        			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Unknown Info from Media Recorder"));
        			break;        		
        		}
        	}
        });
        
        recorder.setOnErrorListener(new OnErrorListener() {
        	@Override
        	public void onError(MediaRecorder mr, int what, int extra) {
        		switch(what)
        		{
        		case MediaRecorder.MEDIA_RECORDER_ERROR_UNKNOWN:
        			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Unknown error from Media Recorder"));
        			stop(false);
        			break;
        		}
        	}
        });
		
	}
	
    /**
     * Stop the media player gracefully and transfer the file
     */
    private void stop(boolean bTransfer)
    {
    	if (isPlaying)
    	{
    		try
    		{
		    	recorder.stop();
		    	recorder.reset();
				recorder.release();	
				recorder = null;
				isPlaying = false;
		    	
				if (bTransfer)
				{
					//Transfer the file
					transferFile();
				}
				else
				{					
					File file = new File(path);
					file.delete();
				}
    		}
    		catch(Exception e)
    		{
    			e.printStackTrace();
    			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error in stopping the audio capture process"));		
    		}
    	}
    }
    
	/** Method called when the App gets Background focus event */
	@Override
	public void onBackground(String url)
	{		
		//  An Application focus change is received 

		//  Application has gone into the background.
		
		if (isPlaying)
		{
			stop(false);	
		}	
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,"Application background event received by Audio Capture plugin"));		
	}
	
	@Override
	public void onPageStarted(String str)
	{
		duration = 20000; //Default - 20 seconds
		//maximumSize = 10000; //default - 10MB
		name = "AudioCapture";	//Default file name
		path = Common.getDataPath() + "/" + name;
		userName = null;
		password = null;
		destination = null;
		mCaptureEvent = null;
		mProtocol = null;
		codec = MediaRecorder.AudioEncoder.AAC;		
		format = MediaRecorder.OutputFormat.MPEG_4;
		isPlaying = false;		
		if (recorder != null)
		{
			stop(false);
			recorder = null;
		}
	}
	
	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onShutdown()
	 */
	@Override
	public void onShutdown() {	
		
		//Stop any recoridng in progress
		stop(false);
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
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'",
					setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));
		
		if (setting.getName().equalsIgnoreCase("duration"))
		{
			try
			{
				duration = Integer.parseInt(setting.getValue());
				if(duration < 1000)
				{
					duration = 20000; //Default
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Setting default duration 20000"));
				}
				//recorder.setMaxDuration(duration); 
			}
			catch (Exception e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid duration '" + setting.getValue() + "'"));
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
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Setting default maximumSize 10000"));
				}				
			}
			catch (Exception e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid maximumSize '" + setting.getValue() + "'"));
			}
		}	*/
		else if (setting.getName().equalsIgnoreCase("name"))
		{
			name = setting.getValue();
			if (name != null)
			{
				path = Common.getDataPath() + "/" + name;				
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
					if (format == MediaRecorder.OutputFormat.MPEG_4)
						destination = value + ".mp4";
					else if (format == MediaRecorder.OutputFormat.THREE_GPP)
						destination = value + ".3gpp";					
				}
				else
				{
					destination = value;
				}
			}
			catch (MalformedURLException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Bad destination URL."));	
			}
			catch(IndexOutOfBoundsException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Bad destination URL."));
			}		
		}
		else if (setting.getName().equalsIgnoreCase("codec"))
		{
			String codecS = setting.getValue();			  
			
			if (codecS.equalsIgnoreCase(CODEC_AAC))
			{
				format = MediaRecorder.OutputFormat.MPEG_4;
				codec = MediaRecorder.AudioEncoder.AAC;				
			}
			else if (codecS.equalsIgnoreCase(CODEC_AMR_NB))
			{
				format = MediaRecorder.OutputFormat.THREE_GPP;
				codec = MediaRecorder.AudioEncoder.AMR_NB;				
			}
			else if (codecS.equalsIgnoreCase(CODEC_AMR_WB))
			{
				format = MediaRecorder.OutputFormat.THREE_GPP;
				codec = MediaRecorder.AudioEncoder.AMR_WB;				
			}			
			else if (codecS.equalsIgnoreCase(CODEC_WAV))
			{
				format = MediaRecorder.OutputFormat.DEFAULT;
				codec = MediaRecorder.AudioEncoder.DEFAULT;				
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "WAV Codec is not valid on Android, so using the defaults"));
			}
			else 
			{
				format = MediaRecorder.OutputFormat.DEFAULT;
				codec = MediaRecorder.AudioEncoder.DEFAULT;				
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Invalid Codec specified, so using the defaults"));
			}
		}
		else if (setting.getName().equalsIgnoreCase("audiosaveevent"))
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
			if (Common.elementsCore.bLaunchingAppHasFocus)
			{
				//Stop recording
				stop(true);
			}
		}
		else if (setting.getName().equalsIgnoreCase("cancel"))
		{
			if (Common.elementsCore.bLaunchingAppHasFocus)
			{
				//Cancel recording
				stop(false);
			}
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + setting.getName()));
		}
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
	    return new Version("AudioCapture"); //$NON-NLS-1$
	}
}
