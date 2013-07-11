package com.motorolasolutions.rho.notification;

import java.nio.ShortBuffer;
import java.util.Map;

import android.app.Activity;
import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.media.MediaPlayer;
import android.os.Vibrator;

import com.rho.notification.INotificationSingleton;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesApplication;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.PerformOnUiThread;

/**
 * Singleton for the Notification extension. Holds all of the implementation for this extension.
 * Uses code from both RhoElements2 and Rhodes3
 * @author Ben Kennedy (reauthor)
 */
public class NotificationSingleton implements INotificationSingleton
{
	protected static String TAG = "NotificationSingleton";
	protected AudioTrack audioTrack = null;
	private Vibrator vibrator;
	private MediaPlayer currentMP;

	@Override
	public void showPopup(final Map<String, Object> propertyMap, final IMethodResult result)
	{
		RhodesApplication.runWhen(RhodesApplication.AppState.AppActivated, new RhodesApplication.StateHandler(true)
		{
			@Override
			public void run()
			{
				try
				{
					Logger.T(TAG, "showPopup");
					PopupActivity.showDialog(propertyMap, result);
				}
				catch (Exception e)
				{
					Logger.E(TAG, e);
					result.setError(e.getLocalizedMessage());
				}
			}
		});
	}

	@Override
	public void hidePopup(final IMethodResult result)
	{
		RhodesApplication.runWhen(RhodesApplication.AppState.AppActivated, new RhodesApplication.StateHandler(true)
		{
			@Override
			public void run()
			{
				try
				{
					Logger.T(TAG, "hidePopup");
					PopupActivity.hidePopup();
				}
				catch (Exception e)
				{
					Logger.E(TAG, e);
					result.setError(e.getLocalizedMessage());
				}
			}
		});
	}

	@Override
	public void showStatus(final String titleText, final String statusText, final String hideLabelText, final IMethodResult result)
	{
		RhodesApplication.runWhen(RhodesApplication.AppState.AppActivated, new RhodesApplication.StateHandler(true)
		{
			@Override
			public void run()
			{
				try
				{
					Logger.I(TAG, "showStatusPopup");
					PerformOnUiThread.exec(new Runnable()
					{
						@Override
						public void run()
						{
							PopupActivity.showStatusDialog(titleText, statusText, hideLabelText);
						}
					});
				}
				catch (Exception e)
				{
					Logger.E(TAG, e);
					result.setError(e.getLocalizedMessage());
				}
			}
		});
	}

	@Override
	public void playFile(String path, String mediaType, final IMethodResult result)
	{
		try
		{
			Logger.T(TAG, "playFile: " + path + " (" + mediaType + ")");

			if (currentMP != null)
				currentMP.release();

			currentMP = new MediaPlayer();
			currentMP.setOnErrorListener(new MediaPlayer.OnErrorListener()
			{
				public boolean onError(MediaPlayer mp, int what, int extra)
				{
					Logger.E(TAG, "Error when playing file : " + what + ", " + extra);
					return false;
				}
			});
			currentMP.setDataSource(RhoFileApi.openFd(path));

			currentMP.prepare();
			currentMP.start();
		}
		catch (Exception e)
		{
			Logger.E(TAG, e);
			result.setError(e.getLocalizedMessage());
		}
	}

	@Override
	public synchronized void beep(Map<String, Integer> propertyMap, IMethodResult result)
	{
		if(audioTrack != null)
		{
			try
			{
				audioTrack.stop();
			}
			catch(IllegalStateException e)
			{
				Logger.D(TAG, "AudioTrack did not need to be stopped here");
			}
			finally
			{
				audioTrack.release();
			}
			audioTrack = null;
		}
		Integer userFrequency = propertyMap.get(HK_FREQUENCY);
		Integer userVolume = propertyMap.get(HK_VOLUME);
		Integer userDuration = propertyMap.get(HK_DURATION);
		int frequency = (userFrequency != null ? userFrequency : 2000);
		int volume = (userVolume != null && userVolume >= 0 && userVolume <= 3 ? userVolume : 1);
		int duration = (userDuration != null ? userDuration : 1000);
		(new NotificationBeep(frequency)).play(duration, volume);
	}

    @Override
    public void vibrate(int duration, IMethodResult result)
    {
        Logger.T(TAG, "Vibrate: " + duration);
        
        if (duration != 0)
        {
            Activity activity = RhodesActivity.safeGetInstance();
            if(vibrator != null) vibrator.cancel();
            vibrator = (Vibrator) activity.getSystemService(Context.VIBRATOR_SERVICE);
            vibrator.vibrate(duration > 0 ? duration : 1000);
        }
    }

	/**
	 * @author unknown (From RE1)
	 */
	private class NotificationBeep
	{
		private final int mSampleRate = 44100; // 44.1KHz sample rate
		private final int mBytesPerSample = 2; // 16 Bits (2 Bytes) per sample
		private double mLoopDuration = 5000; // maximum loop duration in milliseconds
		private int mSampleCount = (int) ((mLoopDuration * mSampleRate) / 1000); // total number of calculated samples
		private ShortBuffer mAudioBuffer = ShortBuffer.allocate(mSampleCount);
		private short[] mAudioData = (mAudioBuffer.hasArray() ? mAudioBuffer.array() : new short[mSampleCount]); // PCM track data

		public NotificationBeep(double frequency)
		{
			final double maxAmplitude = 32767;
			double wavePeriod = mSampleRate / frequency; // in samples

			// Create a sine wave of the required frequency at maximum amplitude
			for (int i = 0; i < mSampleCount; ++i)
			{
				mAudioData[i] = (short) ((Math.sin(2 * Math.PI * i / wavePeriod)) * maxAmplitude);
				if ((i > 0) && (mAudioData[i] == 0) && (mAudioData[i - 1] < 0))
				{
					// we've completed a sine wave that will loop
					mSampleCount = i;
					mLoopDuration = mSampleCount * 1000 / mSampleRate;
					break;
				}
			}
		}

		/**
		 * Plays the beep
		 * @param duration the duration of the beep in milliseconds
		 * @param inputVolume the volume to play the beep (0-3)
		 * @author Unknown & Ben Kennedy
		 */
		public void play(int duration, int inputVolume)
		{
			//normalise to make the volume decrease seem linear
			float volume = ((inputVolume + 1.0f) * (inputVolume > 0 ? inputVolume : 1))/12.0f;

			// calculate how many times we need to play the entire buffer
			int cycles = (int) (duration / mLoopDuration);

			// calculate any part buffers required in samples
			int remainder = (int) ((duration % mLoopDuration) * mSampleRate / 1000);

			// get the minimum buffer size from AudioTrack
			int bufferSizeBytes = AudioTrack.getMinBufferSize(mSampleRate, AudioFormat.CHANNEL_CONFIGURATION_MONO, AudioFormat.ENCODING_PCM_16BIT);

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
			audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, mSampleRate, AudioFormat.CHANNEL_CONFIGURATION_MONO,
					AudioFormat.ENCODING_PCM_16BIT, bufferSizeBytes, AudioTrack.MODE_STATIC);

			if (cycles > 0)
			{
				// write the entire PCM data to the AudioTrack rendering buffer
				audioTrack.write(mAudioData, 0, mSampleCount);

				if (cycles > 1)
				{
					// tell AudioTrack how many times to repeat the sample
					audioTrack.setLoopPoints(0, mSampleCount, cycles - 1);
				}
			}

			if (remainder > 0)
			{
				// add a part buffer of the size of the remainder
				audioTrack.write(mAudioData, 0, remainder);
			}

			//TODO do i need to release???
			// start playing in the background
			audioTrack.flush();
			audioTrack.setStereoVolume(volume, volume);
			audioTrack.play();
		}
	}
	
	/**
	 * Stops and releases all resources
	 * @author Ben Kennedy
	 */
	public void cleanUpResources()
	{
		if (audioTrack != null)
		{
			try
			{
				audioTrack.stop();
			}
			catch(IllegalStateException e)
			{
				//No need to deal with
				Logger.D(TAG, "AudioTrack did not need to be stopped here");
			}
			finally
			{
				audioTrack.release();
				audioTrack = null;
			}
		}
		if(vibrator != null)
		{
			vibrator.cancel();
		}
		if(currentMP != null)
		{
			try
			{
				currentMP.stop();
			}
			catch(IllegalStateException e)
			{
				//No need to deal with.
				Logger.D(TAG, "CurrentMP did not need to be stopped here");
			}
			finally
			{
				currentMP.release();
				currentMP = null;
			}
		}
	}

	/**
	 * Android onPause event
	 */
	public void onPause()
	{
		cleanUpResources();
	}

	/**
	 * Android onStop event
	 */
	public void onStop()
	{
		cleanUpResources();
	}

	/**
	 * Android onDestroy event
	 */
	public void onDestroy()
	{
		cleanUpResources();
	}
}
