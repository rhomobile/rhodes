package com.rho.notification;

import java.io.InputStream;
import java.nio.ShortBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.app.Dialog;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.media.MediaPlayer;
import android.os.Vibrator;
import androidx.core.app.NotificationCompat;
import android.util.SparseArray;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.rho.notification.INotificationSingleton;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.RhodesApplication;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.util.PerformOnUiThread;



/**
 * Singleton for the Notification extension. Holds all of the implementation for this extension.
 * Uses code from both RhoElements2 and Rhodes3
 * @authors Ben Kennedy (reauthor), Alexey Tikhvinsky (aat103)
 */

public class NotificationSingleton implements INotificationSingleton
{
    protected static String TAG = NotificationSingleton.class.getSimpleName();

    //private static SparseArray<MethodResult> callbackMap = new SparseArray<MethodResult>();
    //private static int callbackIdCount = 0;

    private SparseArray<Notification> notifications = new SparseArray<Notification> ();
    private int lastNotificationId = -1;
    protected AudioTrack audioTrack = null;
	private Vibrator vibrator;
	private MediaPlayer currentMP;
	private Context ctx = null;

	public void setContext(Context c)
	{
		ctx = c;
	}

	@Override
	public void setScheduler(final Map<String, Object> propertyMap, final IMethodResult result)
	{
		Logger.T(TAG, "setScheduler");
		NotificationScheduler.setReminderEx(propertyMap, result);
	}

	@Override
	public void removeScheduler(final IMethodResult result)
	{
		Logger.T(TAG, "removeScheduler");
		NotificationScheduler.cancelReminderEx();	
	}

    @Override
    public void showPopup(final Map<String, Object> propertyMap, final IMethodResult result) {
        Logger.T(TAG, "showPopup");

        Notification notification;
        synchronized (notifications) {
            ++lastNotificationId;
            
            Logger.T(TAG, "Add notification: " + lastNotificationId);
            
            notification = new Notification(lastNotificationId, propertyMap, result, ctx);
            notifications.append(lastNotificationId, notification);
        }
        
        if (notification.isForegroundToastNeeded()) {
            notification.showForegroundToast();
        }
        if (notification.isNotificationAreaNeeded()) {
            notification.showNotification();
        }
        if (notification.isDialogNeeded()) {
            notification.showDialog();
        }
    }

    @Override
    public void hidePopup(final IMethodResult result) {
        synchronized (notifications) {
            if (notifications.size() == 0) {
                throw new RuntimeException("There are no active notifications.");
            }

            int notificationId = notifications.size() - 1;
            
            Notification notification = notifications.valueAt(notificationId);
            
            if (notification != null) {
                notification.dismiss();
                notifications.removeAt(notificationId);
                Logger.T(TAG, "Remove notification: " + notification.id);
            }
        }
    }

    @Override
    public void showStatus(String title, String status, String hideLabel, final IMethodResult result)
    {
        Map<String, Object> propertyMap = new HashMap<String, Object> ();
        if (title != null) {
            propertyMap.put(HK_TITLE, title);
        }
        if (status != null) {
            propertyMap.put(HK_MESSAGE, status);
        }
        if (hideLabel != null) {
            List<String> buttons = new ArrayList<String>();
            buttons.add(hideLabel);
            propertyMap.put(HK_BUTTONS, buttons);
        }
        ArrayList<String> types = new ArrayList<String>();
        types.add(TYPE_DIALOG);
        types.add(TYPE_TOAST);
        propertyMap.put(HK_TYPES, types);
        showPopup(propertyMap, result);
    }
    
    void onAction(int notificationId, int actionIdx) {
        Logger.T(TAG, "Notification action: notificationID: " + notificationId + ", action index: " + actionIdx);
        Notification notification = null;
        synchronized (notifications) {
            notification = notifications.get(notificationId);
            if (notification != null) {
                notification.dismiss();
                notifications.delete(notificationId);
                Logger.I(TAG, "Remove notification: " + notification.id);
            }
        }
        if (notification != null) {
            notification.dismiss();
            notification.onAction(actionIdx);
        }
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
            Context context = null;
            try {
                context = RhodesService.getContext();
            }
            catch (IllegalStateException e) {
                context = null;
                Logger.D(TAG, "Rhodes main Service (com.rhomobile.rhodes.RhodesService) is not run !!!");
            }
            if (context != null) {
                if(vibrator != null) vibrator.cancel();
                vibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);

                if ( duration > 15000 ) {
                    duration = 15000;
                }
                vibrator.vibrate(duration > 0 ? duration : 1000);
            }
            else {
                Logger.D(TAG, "Can not vibrate, because can not get Rhodes main Context !!!");
            }
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
