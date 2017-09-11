package com.rho.mediaplayer;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import android.app.ProgressDialog;
import android.content.Context;
import android.database.Cursor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.MediaController;
import android.content.pm.PackageManager;
import android.content.pm.PackageInfo;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class MediaplayerSingleton extends MediaplayerSingletonBase implements IMediaplayerSingleton
{
	private static final String TAG = "RhoMediaplayer";

	private MediaPlayer mPlayer = null;
	private RingtoneManager rtManager = null;
	private static Context context = null;
	private static Ringtone current = null;
	private RhoVideoView mVideoView = null;
	private HashMap<String, Uri> ringtonesMap = null;
	private ProgressDialog dialog = null;

	private RhoAudioPlayerListener audioPlayerListener;

	public MediaplayerSingleton(MediaplayerFactory factory)
	{
		super();
		Logger.D(TAG, "+");
		context = RhodesService.getContext();
		rtManager = new RingtoneManager(context);
		audioPlayerListener = new RhoAudioPlayerListener();
		Logger.D(TAG, "-");
	}

	/**
	 * The start method plays a media file, specified by the filename. If a media file is currently
	 * being played, then the media player is reset and re-prepared.
	 * @params filename the file URI or path + filename of the file to play. Can be file, http, https.
	 * @params the Rho Extension MethodResult.
	 * @author Ben Kennedy (NCVT73)
	 */
	@Override
	public void start(String filename, IMethodResult result)
	{
		Logger.D(TAG, "start+ " + filename);
		if (mPlayer != null)
		{
			Logger.D(TAG, "stopping AudioPlayer");
			mPlayer.stop();
			mPlayer.reset();
		}
		else
		{
			createAudioPlayer();
		}

		try
		{
			if(isPackageFile(filename))
			{
				FileDescriptor fileDescriptor = RhoFileApi.openFd(filename);
				if(fileDescriptor != null && fileDescriptor.valid())
				{
					mPlayer.setDataSource(fileDescriptor);
				}
				else
				{
					Logger.E(TAG, "Bad filename");
					throw new FileNotFoundException();
				}
			}
			else
			{
				Logger.D(TAG, "NOT /data/data file");
				mPlayer.setDataSource(filename);
			}
			mPlayer.prepare(); // This call will block. As it only returns, when there is enough data to play back.
		}
		catch (PackageManager.NameNotFoundException e)
        {
            e.printStackTrace();
			Logger.E(TAG, "start(): package name not found: " + e.getMessage());
			result.setError("Could not play due to IO error");
        }
		catch(FileNotFoundException e)
		{
			Logger.W(TAG, "start(): File not found: " + filename);
			result.setError("File not found");
		}
		catch(IllegalStateException e)
		{
			e.printStackTrace();
			Logger.E(TAG, "start(): Mediaplayer is in an illegal state: " + e.getMessage());
			result.setError("Could not play due to internal error");
		}
		catch (IOException e)
		{
			e.printStackTrace();
			Logger.E(TAG, "start(): failed due to IO error: " + e.getMessage());
			result.setError("Could not play due to IO error");
		}
		
		Logger.D(TAG, "start-");
	}
	
	private boolean isPackageFile(String filename) throws PackageManager.NameNotFoundException
	{    
		 PackageManager manager = context.getPackageManager();
         PackageInfo info = manager.getPackageInfo(context.getPackageName(), 0);
         String dir = info.applicationInfo.dataDir;
         return filename.toLowerCase().contains(dir.toLowerCase());
		//return filename.matches("^(file:)?[/]{0,3}data/data.*$");
	}

	/**
	 * Stop the playback of the currently playing media file and release the MediaPlayer
	 * @author Ben Kennedy (NCVT73)
	 */
	@Override
	public void stop(IMethodResult result)
	{
		if (mPlayer != null && mPlayer.isPlaying())
		{
			Logger.D(TAG, "Calling stop on MediaPlayer");
			mPlayer.stop();
			mPlayer.release();
			mPlayer = null;
		}
	}
	
	/**
	 * Creates a MediaPlayer object to use for audio. Will log any errors, does not null check the filename.
	 * @author Ben Kennedy (NCVT73)
	 */
	private void createAudioPlayer()
	{
		Logger.D(TAG, "createAudioPlayer+");
		mPlayer = new MediaPlayer();
		mPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
		mPlayer.setOnErrorListener(audioPlayerListener);
		mPlayer.setOnCompletionListener(audioPlayerListener);
		mPlayer.setOnPreparedListener(audioPlayerListener);
		Logger.D(TAG, "createAudioPlayer-");
	}

	/**
	 * Queries the device for the list of available ringtones.
	 * @author Ben Kennedy (NCVT73)
	 */
	@Override
	public void getAllRingtones(IMethodResult result)
	{
		Logger.D(TAG, "getAllRingtones+");
		generateRingtonesMap();
		ArrayList<Object> ringtones = new ArrayList<Object>(ringtonesMap.size());
		
		for(Entry<String, Uri> entry: ringtonesMap.entrySet())
		{
			Map<String, String> entryMap = new HashMap<String, String>(2);
			entryMap.put("name", entry.getKey());
			entryMap.put("fullname", entry.getValue().toString());
			ringtones.add((Object) entryMap);
		}
		
		result.set(ringtones);
		Logger.D(TAG, "getAllRingtones- Found " + ringtones.size() + " ringtones");
	}

	/**
	 * Generates the ringtonesMap field from ringtones on the device
	 * @author Ben Kennedy (NCVT73)
	 */
	private void generateRingtonesMap()
	{
		//TODO deal with duplicate names?
		Logger.D(TAG, "generateRingtonesMap+");
		Cursor cursor = rtManager.getCursor();

		if (cursor != null && cursor.moveToFirst())
		{
			ringtonesMap = new HashMap<String, Uri>(cursor.getCount());
			for (int i = 0; i < cursor.getCount(); i++)
			{
				String name = rtManager.getRingtone(i).getTitle(context);
				Uri ringtoneUri = rtManager.getRingtoneUri(i);
				ringtonesMap.put(name, ringtoneUri); //store to play later
			}
		}
	}

	/**
	 * @see com.rho.mediaplayer.IMediaplayerSingleton#playRingTone(java.lang.String,
	 *      com.rhomobile.rhodes.api.IMethodResult)
	 * @author Unknown
	 */
	@Override
	public void playRingTone(String name, IMethodResult result)
	{
		Logger.D(TAG, "playRingTone+ " + name);

		stop();
		if(ringtonesMap == null) generateRingtonesMap();
		if(ringtonesMap != null)
		{
			Uri selectedRingtoneUri = ringtonesMap.get(name);
			if(selectedRingtoneUri != null) current = RingtoneManager.getRingtone(context, selectedRingtoneUri);
			if (current != null)
			{
				current.play();
			}
			else
			{
				Logger.W(TAG, "Could not play ringtone");
			}
		}
		else
		{
			Logger.D(TAG, "Could not find ringtone: " + name);
			result.setError("Could not find ringtone");
		}
	}

	/**
	 * Stop the playback of the 'current' ringtone.
	 */
	private void stop()
	{
		if (current != null)
		{
			current.stop();
			current = null;
		}
	}

	/**
	 * @see com.rho.mediaplayer.IMediaplayerSingleton#stopRingTone(com.rhomobile.
	 *      rhodes.api.IMethodResult)
	 */
	@Override
	public void stopRingTone(IMethodResult result)
	{
		stop();
		Logger.D(TAG, "stopRingTone-");
	}

	protected String getInitialDefaultID()
	{
		return null;
	}

	@Override
	public void startvideo(final String filename, IMethodResult result)
	{
		Logger.D(TAG, "startvideo called with filename: " + filename);
		if(filename == null || filename.length() == 0)
		{
			Logger.W(TAG, "startvideo was called with an invalid filename");
			result.setError("Invalid filename");
			return;
		}
		
		if(mVideoView == null)
		{
			createVideoView();
		}

		mVideoView.stopPlayback();
		
		// Play URL
		// If this is Rho Package file, force Rhodes to copy it to /data/data, then copy this to a local file and play.
		try
		{
			if(isPackageFile(filename))
			{
				FileDescriptor rhoFileDesc = RhoFileApi.openFd(filename);
				if(rhoFileDesc != null && rhoFileDesc.valid())
				{
					FileInputStream srcStream = null;
					FileOutputStream dstStream = null;
					try
					{
						File internalVideoFile = new File(filename);
						File temporaryVideoFile = File.createTempFile("video", "tmp");
		
						if (internalVideoFile.exists())
						{
							srcStream = new FileInputStream(internalVideoFile);
							dstStream = new FileOutputStream(temporaryVideoFile);
							FileChannel src = srcStream.getChannel();
							FileChannel dst = dstStream.getChannel();
							dst.transferFrom(src, 0, src.size());
							src.close();
							dst.close();
							
							dialog.show();
							mVideoView.setVideoURI(Uri.parse(temporaryVideoFile.getAbsolutePath()));
							mVideoView.setVisibility(View.VISIBLE);
							mVideoView.requestFocus();
						}
						else
						{
							result.setError("Video not found");
						}
					}
					catch (FileNotFoundException e)
					{
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					catch (IOException e)
					{
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					finally
					{
						if(srcStream != null) try{srcStream.close();}catch(IOException e){e.printStackTrace();}
						if(dstStream != null) try{dstStream.close();}catch(IOException e){e.printStackTrace();}
					}
				}
				else
				{
					result.setError("Video not found");
				}
			}
			else
			{
				dialog.show();
				mVideoView.setVideoURI(Uri.parse(filename));
				mVideoView.setVisibility(View.VISIBLE);
				mVideoView.requestFocus();
			}
		}
		catch(PackageManager.NameNotFoundException e)
		{
		    e.printStackTrace();
			Logger.E(TAG, "start(): package name not found: " + e.getMessage());
			result.setError("Could not play due to IO error");
		}
	}

	/**
	 * Creates the RhoVideoView in which the videos will be played. This only needs to be happen once.
	 * @author Ben Kennedy (NCVT73)
	 */
	private void createVideoView()
	{
		mVideoView = new RhoVideoView(RhodesService.getContext());
		dialog = new ProgressDialog(RhodesActivity.safeGetInstance());
		dialog.setMessage("Loading Video");
		dialog.setIndeterminate(true);
		dialog.setCancelable(false);
		mVideoView.setZOrderMediaOverlay(true);
		mVideoView.setClickable(true);
		mVideoView.setMediaController(new MediaController(RhodesActivity.safeGetInstance()));
		mVideoView.setDimensions(1, 1); 		// Required for consumer devices. The view doesn't appear otherwise

		FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
		layoutParams.gravity = 0x11;
		RhodesActivity.safeGetInstance().addContentView(mVideoView, layoutParams);
		
		//(android.widget.FrameLayout.LayoutParams) mVideoView.getLayoutParams();
		//RhodesActivity.safeGetInstance().addContentView(mVideoView,	new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT));
		
		mVideoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener()
		{
			@Override
			public void onCompletion(MediaPlayer mp)
			{
				stopVideoView();
			}
		});

		mVideoView.setOnErrorListener(new MediaPlayer.OnErrorListener()
		{
			@Override
			public boolean onError(MediaPlayer mp, int what, int extra)
			{
				Logger.E(TAG, "Unable to play media file. Error: " + what);
				stopVideoView();
				return true;
			}
		});

		mVideoView.setOnPreparedListener(new MediaPlayer.OnPreparedListener()
		{
			@Override
			public void onPrepared(MediaPlayer mp)
			{
				Logger.D(TAG, "onPrepared+ w:" + mp.getVideoWidth() + " h:" + mp.getVideoHeight());
				// The video is ready to be played
				if (mp.getVideoWidth() > 0 && mp.getVideoHeight() > 0)
				{
					dialog.hide();
					// We got a size, resize and start the video.
					mVideoView.resizeMedia(mp.getVideoWidth(), mp.getVideoHeight());
					mVideoView.start();
					Logger.D(TAG, "onPrepared- start");
				}
				Logger.D(TAG, "onPrepared- no start");
			}
		});
	}

	/**
	 * Stops the video if playing and hides the video view.
	 * @author Ben Kennedy (NCVT73)
	 */
	private void stopVideoView()
	{
		if(mVideoView != null)
		{
			if(dialog.isShowing()) dialog.hide();
			mVideoView.stopPlayback();
			//mVideoView.clearFocus();
			//mVideoView.setVisibility(View.GONE);.
			PerformOnUiThread.exec(new Runnable(){

						@Override
						public void run() {
							try{
							mVideoView.clearFocus();
							mVideoView.setVisibility(View.GONE);
							}
							catch(Exception ex)
							{
								Logger.E(TAG,ex.getMessage());
							}
						}});
		}
	}
	
	@Override
	public void stopvideo(IMethodResult result)
	{
		if ((mVideoView != null) && (mVideoView.isPlaying()))
		{
			stopVideoView();
		}
	}
	
	/**
	 * Helper class to collect the Audio MediaPlayer's events. Used to encapsulate the use of MediaPlayer better
	 * and ease cleanup.
	 * @author Ben Kennedy (NCVT73)
	 */
	private class RhoAudioPlayerListener implements OnCompletionListener, OnPreparedListener, OnErrorListener
	{
		@Override
		public boolean onError(MediaPlayer mp, int what, int extra)
		{
			Logger.E(TAG, "onError+: Audio player error: " + what);
			mp.release();
			mPlayer = null;
			Logger.D(TAG, "onError-");
			return false;
		}

		@Override
		public void onPrepared(MediaPlayer mp)
		{
			Logger.D(TAG, "onPrepared+: Audio prepared, playing sound");
			mp.start();
			Logger.D(TAG, "onPrepared-");
		}

		@Override
		public void onCompletion(MediaPlayer mp)
		{
			Logger.D(TAG, "onCompletion+: Audio Playback Complete");
			mp.stop();
			mp.release();
			mPlayer = null;
			Logger.D(TAG, "onCompletion-");
		}
	}

	/**
	 * Activity.onResume()
	 */
	public void onResume()
	{
		// TODO resume playing files
	}

	/**
	 * Activity.onPause()
	 */
	public void onPause()
	{
		// TODO Cleanup and release MediaPlayers
	}

	/**
	 * Activity.onDestroy()
	 */
	public void onDestroy()
	{
		if(mPlayer != null)
		{
			mPlayer.stop();
			mPlayer.release();
			mPlayer = null;
		}
	}

	/**
	 * Activity.onStop()
	 */
	public void onStop()
	{
		// TODO Cleanup and release MediaPlayers
	}

	/**
	 * Deals with incoming keyEvents (used in this extension to catch the back key for video view). 
	 * @param keyCode the code of the key event
	 * @param event the keyEvent
	 * @return whether the event was consumed/absorbed (true) or whether it should bubble/pass on (false)
	 * @author Ben Kennedy (NCVT73)
	 */
	public boolean onKey(int keyCode, KeyEvent event)
	{
		if(keyCode == KeyEvent.KEYCODE_BACK && mVideoView != null && mVideoView.getVisibility() != View.GONE)
		{
			stopVideoView();
			return true;
		}
		return false;
	}
}
