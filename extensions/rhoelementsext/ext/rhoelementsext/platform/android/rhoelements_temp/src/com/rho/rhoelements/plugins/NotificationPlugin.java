package com.rho.rhoelements.plugins;

import java.nio.ShortBuffer;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.PendingIntent;
import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Vibrator;
import android.os.Build;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

public class NotificationPlugin extends Plugin
{
	public final int LED = 0;
	public final int BEEPER = 1;
	public final int PAGER = 2;
	protected String mEnumNotificationsEventUrl = new String();
	protected int mLedOnDuration = 1000;
	protected int mLedOffDuration = 1000;
	protected int mLedNumberOfCycles = 1;
	protected int mBeeperFrequency = 2000;
	protected int mBeeperVolume = 0;
	protected int mBeeperDuration = 1000;
	protected int mVibrateDuration = 1000;
	protected JSONArray mNotificationObjectArray;
	protected PendingIntent mNotificationIntent;
	protected Vibrator mVibrator = null;
	protected AudioTrack mAudioTrack = null;
	protected NotificationBeep mBeep;
	private boolean isBeeperPlaying = false;
	private boolean isPagerPlaying = false;
	private boolean isBeeperCycle = false;
	private boolean isPagerCycle = false;
	
	public NotificationPlugin()
	{
		mNotificationObjectArray = new JSONArray();
		try
		{
			JSONObject j = new JSONObject();
			// need notification capabilities API - hardcoded for now so no LED support
			
			// hardcode a beeper at index 0
			j.put(Plugin.jsonObjNVPOrderString, Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_index")) + "," +
					Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_type")) + "," +
					Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_name")));//Add the order of the items in the object 
			j.put(Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_index")), "0");
			j.put(Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_type")), Integer.toString(BEEPER));
			j.put(Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_name")), "Beeper");
			mNotificationObjectArray.put(0, j);

			// hardcode a pager at index 1
			if(!((Build.MODEL.equalsIgnoreCase("mc18n0")) || (Build.MODEL.equalsIgnoreCase("mc32n0"))))
			{
				j = new JSONObject();
				j.put(Plugin.jsonObjNVPOrderString, Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_index")) + "," + 
				Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_type")) + "," +
				Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_name"))); //Add the order of the items in the object
				j.put(Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_index")), "1");
				j.put(Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_type")), Integer.toString(PAGER));
				j.put(Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_name")), "Pager");
				mNotificationObjectArray.put(1, j);
			}
		
		}
		catch (JSONException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error creating Notification Object Array: "
							+ e.getMessage()));
			mNotificationObjectArray = new JSONArray();
		}
		mBeep = new NotificationBeep(mBeeperFrequency);
	}
	
	/** Method called when the App gets Background focus event */
	@Override
	public void onBackground(String url)
	{		
		//  An Application focus change is received
		//  Application has gone into the background.					
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,"Application background event received by Notification plugin"));
		
		//Stop any notifications that are going on		
		if (mAudioTrack != null)
		{
			mAudioTrack.stop();
			mAudioTrack = null;
			isBeeperPlaying = true;
		}	
		if (mVibrator != null)
		{
			mVibrator.cancel();
			mVibrator = null;	
			isPagerPlaying = true;
		}		
	}
	
	/** Method called when the App gets foreground focus event */
	@Override
	public void onForeground(String url)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,"Application foreground event received by Notification plugin"));
		
		//Resume any notifications that are going on
		if (isBeeperPlaying)
		{
			if (isBeeperCycle)
			{
				mBeep.play(mBeeperDuration);
			}
			else
			{
				mBeep.play(600000);
			}
			isBeeperPlaying = false;
		}
	
		if (isPagerPlaying)
		{
			mVibrator = (Vibrator) Common.mainActivity.getSystemService(Context.VIBRATOR_SERVICE);
			if (isPagerCycle)
			{
				mVibrator.vibrate(mVibrateDuration);
			}
			else
			{
				mVibrator.vibrate(600000); // vibrate for 10 minutes or until we're told to stop
			}
			isPagerPlaying = false;
		}
			
	}
	
	@Override
	public void onPageStarted(String str)
	{
		mEnumNotificationsEventUrl = "";
		isPagerCycle = false;
		isBeeperCycle = false;
		isBeeperPlaying = false;
		isPagerPlaying = false;
	}

	@Override
	public void onSetting(PluginSetting setting)
	{
		if (setting.getValue().length() < 1)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format(
					"'%s', '%s'", setting.getName(), setting.getValue())));

		if (setting.getName().equalsIgnoreCase("setledonduration"))
		{
			try
			{
				mLedOnDuration = Integer.parseInt(setting.getValue());
			}
			catch (NumberFormatException e)
			{
				mLedOnDuration = 1000;
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error on SetLedOnDuration (" + setting.getValue()
								+ ") - " + e.getMessage()));
			}
		}
		else if (setting.getName().equalsIgnoreCase("setledoffduration"))
		{
			try
			{
				mLedOffDuration = Integer.parseInt(setting.getValue());
			}
			catch (NumberFormatException e)
			{
				mLedOffDuration = 1000;
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error on SetLedOffDuration (" + setting.getValue()
								+ ") - " + e.getMessage()));
			}
		}
		else if (setting.getName().equalsIgnoreCase("setlednumberofcycles"))
		{
			try
			{
				mLedNumberOfCycles = Integer.parseInt(setting.getValue());
				if (mLedNumberOfCycles < 1) mLedNumberOfCycles = 1;
			}
			catch (NumberFormatException e)
			{
				mLedNumberOfCycles = 1;
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error on SetLedNumberOfCycles (" + setting.getValue()
								+ ") - " + e.getMessage()));
			}
		}
		else if (setting.getName().equalsIgnoreCase("setbeeperfrequency"))
		{
			int freq = 2000;
			try
			{
				freq = Integer.parseInt(setting.getValue());
				if (freq < 0)
					freq = 2000;
			}
			catch (NumberFormatException e)
			{
				mBeeperFrequency = 2000;
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error on SetBeeperFrequency (" + setting.getValue()
								+ ") - " + e.getMessage()));
			}
			if (freq != mBeeperFrequency)
			{
				mBeeperFrequency = freq;
				mBeep = new NotificationBeep(mBeeperFrequency);
			}
		}
		else if (setting.getName().equalsIgnoreCase("setbeepervolume"))
		{
			try
			{
				mBeeperVolume = Integer.parseInt(setting.getValue());
				if ((mBeeperVolume < 0) || (mBeeperVolume > 3))
					mBeeperVolume = 0;
			}
			catch (NumberFormatException e)
			{
				mBeeperVolume = 0;
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error on SetBeeperVolume (" + setting.getValue()
								+ ") - " + e.getMessage()));
			}
		}
		else if (setting.getName().equalsIgnoreCase("setbeeperduration"))
		{
			try
			{
				mBeeperDuration = Integer.parseInt(setting.getValue());
				if (mBeeperDuration < 0) mBeeperDuration = 1000;
			}
			catch (NumberFormatException e)
			{
				mBeeperDuration = 1000;
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error on SetBeeperDuration (" + setting.getValue()
								+ ") - " + e.getMessage()));
			}
		}
		else if (setting.getName().equalsIgnoreCase("setvibrateduration"))
		{
			try
			{
				mVibrateDuration = Integer.parseInt(setting.getValue());
				if (mVibrateDuration < 0) mVibrateDuration = 1000;
			}
			catch (NumberFormatException e)
			{
				mVibrateDuration = 1000;
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error on SetVibrateDuration (" + setting.getValue()
								+ ") - " + e.getMessage()));
			}
		}
		else if (setting.getName().equalsIgnoreCase("stateon"))
		{
			if (Common.elementsCore.bLaunchingAppHasFocus)
			{
				try
				{
					JSONObject j = (JSONObject) mNotificationObjectArray.get(Integer.parseInt(setting.getValue()));
					switch (Integer.parseInt(j.getString(Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_type")))))
					{
					case BEEPER:						
						if (mAudioTrack == null)
						{
							//mBeep.play(600000); // beep for 10 minutes or until we're told to stop
						         mBeep.play(mBeeperDuration);
						}
						isBeeperCycle = false;
						break;
	
					case PAGER:	
						if (mVibrator == null)
						{
							mVibrator = (Vibrator) Common.mainActivity.getSystemService(Context.VIBRATOR_SERVICE);
							mVibrator.vibrate(600000); // vibrate for 10 minutes or until we're told to stop
						}
						
						isPagerCycle = false;
						break;	
					default:
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "NotificationType " + setting.getValue()
										+ " not supported"));
						break;
					}
					
				}
				catch (NumberFormatException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
				}
				catch (JSONException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
				}
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot set the notifications from a background application"));
			}
		}
		else if (setting.getName().equalsIgnoreCase("stateoff"))
		{
			if (Common.elementsCore.bLaunchingAppHasFocus)
			{
				try
				{
					JSONObject j = (JSONObject) mNotificationObjectArray
							.get(Integer.parseInt(setting.getValue()));
					switch (Integer.parseInt(j.getString(Common.mainActivity
							.getString(RhoExtManager.getResourceId("string","notification_type")))))
					{
					case BEEPER:
						if (mAudioTrack != null)
						{
							mAudioTrack.stop();
							mAudioTrack = null;							
						}
						isBeeperCycle = false;
						break;
	
					case PAGER:
						if (mVibrator != null)
						{
							mVibrator.cancel();
							mVibrator = null;							
						}
						isPagerCycle = false;
						break;
	
					default:
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "NotificationType " + setting.getValue()
										+ " not supported"));
						break;
					}
					
				}
				catch (NumberFormatException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
				}
				catch (JSONException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
				}
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot set the notifications from a background application"));
			}
		}
		else if (setting.getName().equalsIgnoreCase("statecycle"))
		{
			if (Common.elementsCore.bLaunchingAppHasFocus)
			{
				try
				{
					JSONObject j = (JSONObject) mNotificationObjectArray.get(Integer.parseInt(setting.getValue()));
					switch (Integer.parseInt(j.getString(Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_type")))))
					{
					case BEEPER:
						if (mAudioTrack == null)
						{
							mBeep.play(mBeeperDuration);
						}
						isBeeperCycle = true;
						break;
	
					case PAGER:
						if (mVibrator == null)
						{
							mVibrator = (Vibrator) Common.mainActivity.getSystemService(Context.VIBRATOR_SERVICE);
							mVibrator.vibrate(mVibrateDuration);
						}
						isPagerCycle = true;
						break;
	
					default:
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "NotificationType " + setting.getValue()
										+ " not supported"));
						break;
					}
					
				}
				catch (NumberFormatException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
				}
				catch (JSONException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
				}
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot set the notifications from a background application"));
			}
		}
		else if (setting.getName().equalsIgnoreCase("enumnotificationsevent"))
		{
			mEnumNotificationsEventUrl = setting.getValue();
		}
		else if (setting.getName().equalsIgnoreCase("enumerate"))
		{
			if (mEnumNotificationsEventUrl.length() > 0)
			{
				try
				{
					navigate(mEnumNotificationsEventUrl, 
							Common.mainActivity.getString(RhoExtManager.getResourceId("string","notification_array_name")), mNotificationObjectArray);
				}
				catch (NavigateException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
				}
			}
		}
	}

	@Override
	public void onShutdown()
	{
		if (mAudioTrack != null)
		{
			mAudioTrack.stop();
			mAudioTrack.release();
			mAudioTrack = null;
		}
		if (mVibrator != null)
		{
			mVibrator.cancel();	
			mVibrator = null;
		}
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("NotificationPlugin");
	}

	private class NotificationBeep
	{
		private final int mSampleRate = 44100; 	// 44.1KHz sample rate
		private final int mBytesPerSample = 2; 	// 16 Bits (2 Bytes) per sample
		private double mLoopDuration = 5000;	// maximum loop duration in milliseconds
		private int mSampleCount = (int) ((mLoopDuration * mSampleRate) / 1000); // total number of calculated samples
		private ShortBuffer mAudioBuffer = ShortBuffer.allocate(mSampleCount);
		private short[] mAudioData = 
			(mAudioBuffer.hasArray() ? mAudioBuffer.array() : new short[mSampleCount]); // PCM track data

		public NotificationBeep(double frequency)
		{
			final double maxAmplitude = 32767;

			double wavePeriod = mSampleRate / frequency; // in samples

			// Create a sine wave of the required frequency at maximum amplitude
			for (int i = 0; i < mSampleCount; ++i)
			{
				mAudioData[i] = (short) ((Math.sin(2 * Math.PI * i / wavePeriod)) * maxAmplitude);
				if ((i > 0) && (mAudioData[i] == 0) && (mAudioData[i-1] < 0))
				{
					// we've completed a sine wave that will loop
					mSampleCount = i;
					mLoopDuration = mSampleCount * 1000 / mSampleRate;
					break;
				}
			}			
		}
		
		public void play(int duration)
		{
			int cycles = 0;

			// calculate how many times we need to play the entire buffer
			cycles = (int) (duration / mLoopDuration);
			
			// calculate any part buffers required in samples
			int remainder = (int)((duration % mLoopDuration) * mSampleRate / 1000);
			
			// get the minimum buffer size from AudioTrack
			int bufferSizeBytes = AudioTrack.getMinBufferSize(mSampleRate, 
					AudioFormat.CHANNEL_CONFIGURATION_MONO, AudioFormat.ENCODING_PCM_16BIT);
			
			// calculate how big a rendering buffer to request from AudioTrack
			int reqdBytes = remainder * mBytesPerSample;
			if (cycles > 0)
			{
				reqdBytes += (mSampleCount * mBytesPerSample);
			}
			if (reqdBytes > bufferSizeBytes)
			{
				bufferSizeBytes = reqdBytes;
			}

			// configure AudioTrack
			mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,
					mSampleRate, AudioFormat.CHANNEL_CONFIGURATION_MONO,
					AudioFormat.ENCODING_PCM_16BIT, bufferSizeBytes,
					AudioTrack.MODE_STATIC);

			if (cycles > 0)
			{
				// write the entire PCM data to the AudioTrack rendering buffer
				mAudioTrack.write(mAudioData, 0, mSampleCount);

				if (cycles > 1)
				{
					// tell AudioTrack how many times to repeat the sample
					mAudioTrack.setLoopPoints(0, mSampleCount, cycles - 1);
				}				
			}

			if (remainder > 0)
			{
				// add a part buffer of the size of the remainder
				mAudioTrack.write(mAudioData, 0, remainder);			
			}

			// start playing in the background
			mAudioTrack.flush();
			mAudioTrack.play();
		}
	}
}
