/**
 * Media Player plugin
 */
package com.rho.rhoelements.plugins;

import java.io.IOException;
import java.net.URISyntaxException;
import java.security.InvalidParameterException;

import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.Logger;
import java.io.File;
import android.util.Patterns;

/**
 * @author pxb743
 *
 */
public class MediaPlayerPlugin extends Plugin {

	private static final String TAG = "MediaPlayer";
	private String filename = null;   
    private boolean isVideo = true;  
    private boolean isAudioPlaying = false;
    private MediaPlayer mPlayer;
	private AudioManager audioManager;
	
	/**
	 * Play audio files
	 */
    private void onAudioPlay()
    {
    	if(mPlayer != null)
		{
			mPlayer.stop();
			mPlayer = null;
		}
		mPlayer = new MediaPlayer();
		mPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
		audioManager = (AudioManager) Common.mainActivity.getSystemService(Context.AUDIO_SERVICE);
		if (audioManager.getMode() != AudioManager.MODE_NORMAL)
		{
			audioManager.setMode(AudioManager.MODE_NORMAL);
		}
		if (!audioManager.isSpeakerphoneOn())
		{
			audioManager.setSpeakerphoneOn(true);
		}
		mPlayer.setOnPreparedListener(new OnPreparedListener()
		{
			@Override
			public void onPrepared(MediaPlayer mp)
			{
				mp.start();
				isAudioPlaying = true;
			}
		});
		
		try
		{
			mPlayer.setDataSource(filename);
			mPlayer.prepare();			
		}
		catch (IllegalArgumentException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));						
		}
		catch (IllegalStateException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
		}
		catch (IOException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
		}    	
    }
    
    /**
     * Stop the media player gracefully 
     */
    private void onAudioStop()
    {
    	if (isAudioPlaying)
    	{
    		if (mPlayer != null)
			{
				mPlayer.stop();
				mPlayer = null;
			}
    	}
    }
    

	/** Method called when the App gets Background focus event */
	@Override
	public void onBackground(String url)
	{		
		//  An Application focus change is received 

		//  Application has gone into the background.	
		
		if (isVideo)
		{
			Common.elementsCore.onStopMedia();
		}
		else
		{	
			onAudioStop();
		}
			
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,"Application background event received by Audio Capture plugin"));		
	}
	
	@Override
	public void onPageStarted(String str)
	{		
		filename = null;	
		isVideo = true;
		isAudioPlaying = false;
		mPlayer = null;
		audioManager = null;
	}
	
	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onShutdown()
	 */
	@Override
	public void onShutdown() {		
		
		//Audio
		onAudioStop();
		
	}
	
	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onSetting(com.rho.rhoelements.PluginSetting)
	 */
	@Override
	public void onSetting(PluginSetting setting) {
		if (setting.getName().equalsIgnoreCase("filename"))
		{
			String file = setting.getValue();
			if(file == null)
			{
				filename = null;
				return;
			}
			
			if(file.startsWith("url:"))
				file=file.substring(4);
			
			try
			{
				file = Common.parseAndroidURI(file).toString();
			}
			catch (InvalidParameterException e)
			{
				Logger.W(TAG, "Invalid Parameter: " + file + ", setting ignored");
				file = "";
			}
			catch (URISyntaxException e)
			{
				Logger.W(TAG, "Invalid URL: " + file + ", setting ignored");
				file = "";
			}
			if(file.contains("http"))
			{
			    if(Patterns.WEB_URL.matcher(file).matches())
			     {
		         	filename = file;
			     }else
			     {
			        filename = "";	
			     }
			}   
		       	else
		        {
			  filename = file;
			}
			//filename = file;
		}
		else if (setting.getName().equalsIgnoreCase("type"))
		{
			String type = setting.getValue();
			if (type.equals("audio"))
			{
				isVideo = false;
			}	
			else if (type.equals("video"))
			{
				isVideo = true;
			}
		}
		else if (setting.getName().equalsIgnoreCase("start"))
		{			
			if (Common.elementsCore.bLaunchingAppHasFocus)
			{
				if (filename == null)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Please specify the file name to be played"));
				}
				if (isVideo)
				{
					//If Video
					Common.elementsCore.onPlayMedia(null, filename);
				}
				else
				{
					onAudioPlay();
				}
			}				
		}
		else if (setting.getName().equalsIgnoreCase("stop"))
		{
			if (Common.elementsCore.bLaunchingAppHasFocus)
			{
				if (isVideo)
				{
					Common.elementsCore.onStopMedia();
				}
				else
				{	
					onAudioStop();
				}
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
	    return new Version("MediaPlayer"); //$NON-NLS-1$
	}

}
