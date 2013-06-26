package com.rho.mediaplayer;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.database.Cursor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.view.View;
import android.view.ViewGroup;
import android.widget.MediaController;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.api.IMethodResult;

class MediaplayerSingleton extends MediaplayerSingletonBase implements IMediaplayerSingleton {

	private static final String TAG = "Rho::Mediaplayer";

	private MediaPlayer mPlayer = null;
	private RingtoneManager rtManager = null;
	private static Context context = null;
	private static Ringtone current = null;
	private RhoVideoView mVideoView = null;

	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}

	public MediaplayerSingleton(MediaplayerFactory factory) {
		super();
		Logger.I(TAG, "Creating Mediaplayer extension");
		init();
	}

	/**
	 * 
	 * The start method plays a media file, specified by the filename. If a
	 * media file is currently being played, then the media player is stopped.
	 * 
	 * @see com.rho.mediaplayer.IMediaplayerSingleton#start(java.lang.String,
	 *      com.rhomobile.rhodes.api.IMethodResult)
	 * 
	 */
	@Override
	public void start(String filename, IMethodResult result) {
		Logger.I(TAG, "start called with parameter " + filename);
		if (mPlayer != null) {
			Logger.I(TAG, "calling stop on MediaPlayer");
			mPlayer.stop();
			mPlayer = null;
		}

		mPlayer = new MediaPlayer();
		mPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);

		// Create the PreparedListener
		mPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
			@Override
			public void onPrepared(MediaPlayer mp) {
				// Start the play back
				Logger.I(TAG, "Starting playback of selected media");
				mp.start();
			}
		});

		try {
			mPlayer.setDataSource(filename);
			// This call will block. As it only returns, when there is enough
			// data
			// to play back.
			mPlayer.prepare();
		} catch (IOException ioe) {
			reportFail("start", ioe);
		}
	}

	/**
	 * Stop the playback of the currently playing media file.
	 * 
	 * @see com.rho.mediaplayer.IMediaplayerSingleton#stop(com.rhomobile.rhodes.api
	 *      .IMethodResult)
	 */
	@Override
	public void stop(IMethodResult result) {
		if (mPlayer != null) {
			Logger.I(TAG, "Calling stop on MediaPlayer");
			mPlayer.stop();
			mPlayer = null;
		}
	}

	private void init() {
		if (context == null) {
			context = RhodesService.getContext();
			Logger.I(TAG, "init -- Received context from RhodesService " + context.toString());
		}
		if (rtManager == null) {
			rtManager = new RingtoneManager(context);
			Logger.I(TAG, "init -- Retrieved RingtoneManager from Android " + rtManager.toString());
		}
	}

	/**
	 * 
	 * @see com.rho.mediaplayer.IMediaplayerSingleton#getAllRingtones(com.rhomobile
	 *      .rhodes.api.IMethodResult)
	 */
	@Override
	public void getAllRingtones(IMethodResult result) {
		Logger.I(TAG, "getAllRingtones called");
		Cursor cursor = rtManager.getCursor();

		Map<String, Object> ringtones = new HashMap<String, Object>();
		Logger.I(TAG, "getAllRingtones -- Created ringtones hash " + ringtones.size());
		if (cursor != null) {
			String name = null;
			String value = null;
			Logger.I(TAG, "getAllRingtones -- moving to first record in database");
			if (cursor.moveToFirst()) {
				for (int i = 0; i < cursor.getCount(); i++) {
					name = rtManager.getRingtone(i).getTitle(context);
					value = rtManager.getRingtoneUri(i).toString();
					Logger.I(TAG, "Retrieved ringtone: " + name + " " + value);
					// Add to the result ringtones
					ringtones.put(name, value);
				}
			}
		}

		result.set(ringtones);
		Logger.I(TAG, "getAllRingtones -- Retrieved " + ringtones.size() + " ringtones");
	}

	/**
	 * 
	 * @see com.rho.mediaplayer.IMediaplayerSingleton#playRingTone(java.lang.String,
	 *      com.rhomobile.rhodes.api.IMethodResult)
	 */
	@Override
	public void playRingTone(String name, IMethodResult result) {
		init();

		stop();

		Logger.I(TAG, "playRingTone -- " + name);

		current = RingtoneManager.getRingtone(context, Uri.parse(name));

		if (current != null) {
			current.play();
		} else {
			Logger.I(TAG, "Can't play current ring tone");
		}
	}

	/**
	 * Stop the playback of the 'current' ringtone.
	 */
	private void stop() {
		init();

		if (current != null) {
			current.stop();
			current = null;
		}
	}

	/**
	 * 
	 * 
	 * @see com.rho.mediaplayer.IMediaplayerSingleton#stopRingTone(com.rhomobile.
	 *      rhodes.api.IMethodResult)
	 */
	@Override
	public void stopRingTone(IMethodResult result) {
		init();

		stop();
		Logger.I(TAG, "stopRingTone");
	}

	protected String getInitialDefaultID() {
		return null;
	}

	@Override
	public void startvideo(final String filename, IMethodResult result) {
		Logger.I(TAG, "startvideo called with filename: " + filename);
		mVideoView = new RhoVideoView(RhodesService.getContext());
		mVideoView.setZOrderMediaOverlay(true);

		mVideoView.setClickable(true);
		Logger.I(TAG, "startvideo -- Created instance of RhoVideoView");
		
		RhodesActivity.safeGetInstance().addContentView(mVideoView,
				new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT));
		
		Logger.I(TAG, "startvideo -- Added content view to current activity");

		mVideoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {

			// Cleanup function once the video is played
			@Override
			public void onCompletion(MediaPlayer mp) {
				mVideoView.stopPlayback();
				mVideoView.clearFocus();
				// This is the only way to refresh the screen
				// Android doesn't support removing a view once it
				// is created\
				// so hiding is the only option
				mVideoView.setVisibility(View.GONE);
			}
		});

		// For now full screen view
		android.widget.FrameLayout.LayoutParams vvLayoutParams = (android.widget.FrameLayout.LayoutParams) mVideoView
				.getLayoutParams();
		vvLayoutParams.gravity = 0x11;
		mVideoView.setLayoutParams(vvLayoutParams);

		// Play URL
		mVideoView.setVideoURI(Uri.parse(filename));
		Logger.I(TAG, "startvideo -- setting video url to " + Uri.parse(filename));
		mVideoView.setMediaController(new MediaController(RhodesActivity.safeGetInstance()));
		// Required for consumer devices.  The view doesn't appear otherwise
		mVideoView.setDimensions(1, 1);
		mVideoView.requestFocus();
		// final HourglassPlugin hourglass = new HourglassPlugin();
		// hourglass.show();

		Logger.I(TAG, "startvideo -- Calling setOnErrorListener");
		mVideoView.setOnErrorListener(new MediaPlayer.OnErrorListener() {

			@Override
			public boolean onError(MediaPlayer mp, int what, int extra) {
				Logger.E(TAG, "Unable to play media file: " + filename + " Error: " + what);
				if (mVideoView != null) {
					mVideoView.clearFocus();
					mVideoView.setVisibility(View.GONE);
					// hourglass.hide();
				}
				return true;
			}
		});

		Logger.I(TAG, "startvideo -- Calling setOnPreparedListener");
		mVideoView.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {

			@Override
			public void onPrepared(MediaPlayer mp) {
				Logger.I(TAG, "onPrepared -- " + mp.getVideoWidth() + " " + mp.getVideoHeight());
				// The video is ready to be played
				if (mp.getVideoWidth() > 0 && mp.getVideoHeight() > 0) {
					// We got a size, resize and start the video.
					mVideoView.resizeMedia(mp.getVideoWidth(), mp.getVideoHeight());
					Logger.I(TAG, "Video width: " + mp.getVideoWidth() + " Video height: " + mp.getVideoHeight());
					mVideoView.start();
					Logger.I(TAG, "onPrepared -- called start");
					// hourglass.hide();
				}
			}
		});

		// Another way to start the video without OnPrepareListener
		//mVideoView.start();
	}

	@Override
	public void stopvideo(IMethodResult result) {
		if ((mVideoView != null) && (mVideoView.isPlaying())) {
			mVideoView.stopPlayback();
			mVideoView.clearFocus();
			mVideoView.setVisibility(View.GONE);
			mVideoView = null;
		}
	}
}