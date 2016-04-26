package com.rho.rhoelements.plugins;

import android.content.Context;
import android.media.AudioManager;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;


/**
 * @author hpkx48
 * Allows for the adjustment of the Audio Stream Volumes.
 * 
 * The volume is specified as a hex value between 0-0xFFFF
 * High order byte is for right channel
 * Low order byte is for left channel
 * 
 * value 0x0000 is no volume , highest volume = 0xFFFF
 *  
 * If stereo balance adjustment is not supported then just use the low order byte.
 * When the plugin is loaded all the current stream settings are stored.
 * Upon unloading/destruction call (onShutdown) to of the plugin all the previously store audio settings
 * are restored. 
 * 
 */
public class VolumePlugin extends Plugin
{	
    
    	// Array to save all sound settings which can be restored after quiting.
	int[] StreamSettings = new int[10];
	
	final int INVALID_VALUE = -1;
	private String mVolumeSetting = "";
	
	public VolumePlugin()
	{
	    CallbackHandlerObj = new CallbackHandler(this);
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	    
	    // Store all existing values for sound channels 
	    StoreAllStreamSettings();
	}

	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version ("Volume");
	}	

	/** Method called when the App gets foreground focus event */
	@Override
	public void onForeground(String url)
	{		
		//  An Application focus change is received 
		
		//  Application has become active. 
					
		// The data is already saved in the member variables so apply all the settings	
		if (mVolumeSetting != "")
		{
			setStream(mVolumeSetting,AudioManager.STREAM_ALARM);
			setStream(mVolumeSetting,AudioManager.STREAM_DTMF);
			setStream(mVolumeSetting,AudioManager.STREAM_MUSIC);
			setStream(mVolumeSetting,AudioManager.STREAM_NOTIFICATION);
			setStream(mVolumeSetting,AudioManager.STREAM_RING);
			setStream(mVolumeSetting,AudioManager.STREAM_SYSTEM);
			setStream(mVolumeSetting,AudioManager.STREAM_VOICE_CALL);
		}
		else
		{
			//The volume was not set previously, so ignore
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,"Application foreground event received by Volume plugin"));
		
	}
	
	/** Method called when the App gets Background focus event */
	@Override
	public void onBackground(String url)
	{		
		//  An Application focus change is received 

		//  Application has gone into the background.					
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,"Application background event received by Volume plugin"));
		
	    // Restore all sound channel settings that where set before any adjustment
	    RestoreAllStreamSettings();
		
	}
	
	@Override
	public void onPageStarted(String url)
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));    
	}
		
	@Override
	public void onShutdown()
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	    
	    // Restore all sound channel settings that where set before any adjustment
	    RestoreAllStreamSettings();
	}

	@Override
	public void onSetting(PluginSetting setting)
	{		

	    if (setting.getValue().length() > 0)
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
	    else
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));
	    
	    
	    // Audio Device Control
	   if (setting.getName().equalsIgnoreCase("setvolume"))
	   { 
	       //Save the EMML value in both the cases  
		   mVolumeSetting = setting.getValue();
		   
		   //Change the hardware settings only when the application has got focus
		   if (Common.elementsCore.bLaunchingAppHasFocus)
		   {
		       setStream(mVolumeSetting,AudioManager.STREAM_ALARM);
		       setStream(mVolumeSetting,AudioManager.STREAM_DTMF);
		       setStream(mVolumeSetting,AudioManager.STREAM_MUSIC);
		       setStream(mVolumeSetting,AudioManager.STREAM_NOTIFICATION);
		       setStream(mVolumeSetting,AudioManager.STREAM_RING);
		       setStream(mVolumeSetting,AudioManager.STREAM_SYSTEM);
		       setStream(mVolumeSetting,AudioManager.STREAM_VOICE_CALL);
		   }
		   else
		   {
			   Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,"The volume will not be set as the app doesn't have focus"));
		   }
	   }
	  
	   Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "-"));
		
	}
	
	/**
	 * Method to store all current channel volume settings
	 */
	public void StoreAllStreamSettings()
	{
	    	Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Storing All Audio Stream Channel Settings"));    
	    	AudioManager am = (AudioManager) Common.mainActivity.getSystemService(Context.AUDIO_SERVICE);

		StreamSettings[AudioManager.STREAM_ALARM] = am.getStreamVolume(AudioManager.STREAM_ALARM);
		StreamSettings[AudioManager.STREAM_DTMF] = am.getStreamVolume(AudioManager.STREAM_DTMF);
		StreamSettings[AudioManager.STREAM_MUSIC] = am.getStreamVolume(AudioManager.STREAM_MUSIC);
		StreamSettings[AudioManager.STREAM_NOTIFICATION] = am.getStreamVolume(AudioManager.STREAM_NOTIFICATION);  
		StreamSettings[AudioManager.STREAM_RING] = am.getStreamVolume(AudioManager.STREAM_RING);  
		StreamSettings[AudioManager.STREAM_SYSTEM] = am.getStreamVolume(AudioManager.STREAM_SYSTEM);  
		StreamSettings[AudioManager.STREAM_VOICE_CALL] = am.getStreamVolume(AudioManager.STREAM_VOICE_CALL);  
	}
	
	/**
	 * Method to restore all previously stored channel volume settings
	 */
	public void RestoreAllStreamSettings()
	{
	    	Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Restoring All Audio Stream Channel Settings"));    
	    	AudioManager am = (AudioManager) Common.mainActivity.getSystemService(Context.AUDIO_SERVICE);
	    
		am.setStreamVolume(AudioManager.STREAM_ALARM,      	StreamSettings[AudioManager.STREAM_ALARM],0);
		am.setStreamVolume(AudioManager.STREAM_DTMF,       	StreamSettings[AudioManager.STREAM_DTMF],0);
		am.setStreamVolume(AudioManager.STREAM_MUSIC, 		StreamSettings[AudioManager.STREAM_MUSIC],0);
		am.setStreamVolume(AudioManager.STREAM_NOTIFICATION, 	StreamSettings[AudioManager.STREAM_NOTIFICATION],0);
		am.setStreamVolume(AudioManager.STREAM_RING, 		StreamSettings[AudioManager.STREAM_RING],0);
		am.setStreamVolume(AudioManager.STREAM_SYSTEM, 		StreamSettings[AudioManager.STREAM_SYSTEM],0);
		am.setStreamVolume(AudioManager.STREAM_VOICE_CALL, 	StreamSettings[AudioManager.STREAM_VOICE_CALL],0);
	}
	  
	/**
	 * Method to set the specified audio stream to a particular volume.
	 * The volume is specified in hex 0-0xFFFF
	 * High order byte is for right channel
	 * Low order byte is for left channel
	 * 
	 * value 0x0000 is no volume , highest volume = 0xFFFF
	 *  
	 * If stereo balance adjustment is not supported then just use the low order byte. 
	 * 
	 * @param strvalue contains the hex string to be used to set the stream volume
	 * @param stream contains the specified stream to be modified.
	 */
	private void setStream(String strvalue, int stream) 
	{
	    	AudioManager am = null;
	    	boolean strAudioLeftRightBal = false; // Android does not currently support audio balance 2.33
	    	
		am = (AudioManager) Common.mainActivity.getSystemService(Context.AUDIO_SERVICE);
		Integer value = parseHex(strvalue,"volume level");
		
		// check if we get an invalid volume level passed , if so do nothing.
		if(value >= 0)
		{
		    // is audio left and right balance available
		    if(strAudioLeftRightBal == false)
		    { 
			// mask off High Order byte, since left and right channel balance is not available
			value = value & 0xFF;   
		    }
		    else
		    {
			// both channels are available
			    
			// currently not supported!
		    }
			
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "value =" + value ));
		    	  
		    
		    //Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "value =" + value ));
		    	
		    // value is ranged between 0 and 255 since left and right is not supported.
		    // since there are all different values for each individual stream we will use a % based
		    // calculation.
		    	
		    // 1. convert value to a % of max 255
		    // 100 / 255 * 100 = %
		    float value2 = (value.floatValue() / 255);
		    //Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "value2 =" + value2 ));
		    	
		    	
		    int maxvolume = am.getStreamMaxVolume(stream);
		    float calculatedVolume = (maxvolume * value2) + 0.5f;
			
		    value = (int)calculatedVolume;
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "maxvolume=" + maxvolume + "calculated value =" +  calculatedVolume + " actual value =" + value));
		    if(value > maxvolume) value = maxvolume;
		    am.setStreamVolume(stream, value, 0);
		}
		else
		{
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Invalid Volume Level Detected Making No Changes"));
		}
				
	}
	
	
	/**
	 * Method to allow the parsing of a text based hex string into an integer.
	 * @param strValue this is the string value formatted as a hex string i.e 0xFFEE
	 * @param strProperty this is the output string that will be printed if an exception occurs
	 * @param iProperty this is the value to use if an exception occurs.
	 * @return returns an integer
	 */
	private int parseHex(String strValue, String strProperty)
	{
	    int value;
	  
	    try
	    {
		value = Integer.decode(strValue);
	    }
	    catch(NumberFormatException e)
	    {
		//Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " + strProperty + " value: " + strValue));
		value = INVALID_VALUE;
	    }
	    return value;
	}
}
	
	
	

