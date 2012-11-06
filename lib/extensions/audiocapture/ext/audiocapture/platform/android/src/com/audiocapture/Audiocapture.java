package com.audiocapture;

import java.io.File;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import com.rhomobile.rhodes.RhodesAppOptions;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.media.MediaRecorder;

public class Audiocapture {

	private static Audiocapture ourInstance = null;
	
	public static Audiocapture getInstance() {
		if (ourInstance == null) {
			ourInstance = new Audiocapture();
		}
		return ourInstance;
	}
	
	public Audiocapture() {
		mCallbackURL = "";
		mDuration = 20000;
		mName = "audiocapture.wav";
		String dir = RhodesAppOptions.getBlobPath();
		mDestination = dir + "/" +mName;
	}
	
	public final static String AUDIOCAPTURE_CALLBACK = "audioSaveEvent";
	public final static String AUDIOCAPTURE_DESTINATION = "destination";
	public final static String AUDIOCAPTURE_DURATION = "duration";
	public final static String AUDIOCAPTURE_NAME = "name";

	public final static String AUDIOCAPTURE_TMP_NAME_ADDON = "_tmp";
	
	private String mCallbackURL = null;
	private String mDestination = null;
	private String mName = null;
	private int mDuration = 20000;
	private boolean mIsSaveFile = false; 
	
	private ExtAudioRecorder mRecorder = null;
	
	private Timer mTimer = null;
	
	
	public static void setProperty(String property_name, String value) {
		try {
			if (AUDIOCAPTURE_DURATION.equals(property_name) ) {
				int per = Integer.parseInt(value);
				getInstance().mDuration = per;
			}
			if (AUDIOCAPTURE_CALLBACK.equals(property_name) ) {
				getInstance().mCallbackURL = value;
			}
			if (AUDIOCAPTURE_DESTINATION.equals(property_name) ) {
				getInstance().mDestination = value;
			}
			if (AUDIOCAPTURE_NAME.equals(property_name) ) {
				String dir = RhodesAppOptions.getBlobPath();
				getInstance().mDestination = dir + "/" +value;
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	// 1 - string
	// 3 - int
	public static int getPropertyType(String property_name) {
		if (AUDIOCAPTURE_DURATION.equals(property_name) ) {
			return 3;
		}
		if (AUDIOCAPTURE_CALLBACK.equals(property_name) ) {
			return 1;
		}
		if (AUDIOCAPTURE_DESTINATION.equals(property_name) ) {
			return 1;
		}
		if (AUDIOCAPTURE_NAME.equals(property_name) ) {
			return 1;
		}
		return 1;
	}
	
	public static int getIntProperty(String property_name) {
		if (AUDIOCAPTURE_DURATION.equals(property_name) ) {
			return getInstance().mDuration;
		}
		return 0;
	}

	public static String getStringProperty(String property_name) {
		if (AUDIOCAPTURE_CALLBACK.equals(property_name) ) {
			return getInstance().mCallbackURL;
		}
		if (AUDIOCAPTURE_DESTINATION.equals(property_name) ) {
			return getInstance().mDestination;
		}
		if (AUDIOCAPTURE_NAME.equals(property_name) ) {
			return getInstance().mName;
		}
		return null;
	}
	
	
	
	private class MyTimerTask extends TimerTask {
		private long mDelay = 0;
		
		public MyTimerTask(int delay) {
			mDelay = delay;
		}
		
		public void run() {
			stopCommand();
		}
	}
	
	public void startCommand() {
	    mRecorder = ExtAudioRecorder.getInstanse(false, mDuration);
        //mRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
        //mRecorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);
        mRecorder.setOutputFile(mDestination+AUDIOCAPTURE_TMP_NAME_ADDON);
        //mRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);

        mRecorder.prepare();
        mRecorder.start();
        
        mIsSaveFile = true;
        
		if (mTimer != null) {
			mTimer.cancel();
			mTimer = null;
		}
        //mTimer = new Timer();
        //mTimer.schedule( new MyTimerTask (mDuration), (long)mDuration);
        
	}
	
	public void stopCommand() {
		if (mTimer != null) {
			mTimer.cancel();
			mTimer = null;
		}
		if (mRecorder != null) {
			mRecorder.stop();
	        mRecorder.release();
	        mRecorder = null;
	        
	        if (mIsSaveFile) {
	        	// kill if exist
	        	File dst_file = new File(mDestination);
	        	if (dst_file.exists()) {
	        		dst_file.delete();
	        	}
	        	
	        	// rename tmp to dst
	        	File tmp_file = new File(mDestination+AUDIOCAPTURE_TMP_NAME_ADDON);
	        	if (tmp_file.exists()) {
	        		tmp_file.renameTo(new File(mDestination));
	        	}
	        	fireCallback("OK", "filename", mDestination);
	        }
	        else {
	        	// kill tmp file
	        	File file = new File(mDestination + AUDIOCAPTURE_TMP_NAME_ADDON);
	        	if (file.exists()) {
	        		file.delete();
	        	}
	        	fireCallback("CANCEL", null, null);
	        }
		}
		else {
        	fireCallback("ERROR", "message", "not any record process");
		}
	}

	public void cancelCommand() {
        mIsSaveFile = false;
		stopCommand();
	}
	
	
	public static void onAudioRecorderFinishedByDuration() {
		getInstance().stopCommand();
	}
	
	
	public static void start() {
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				getInstance().startCommand();
			}
		});
	}
	
	public static void stop() {
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				getInstance().stopCommand();
			}
		});
	}

	public static void cancel() {
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				getInstance().cancelCommand();
			}
		});
	}
	
	private void fireCallback(String status, String param_name, String param_value) {
		StringBuffer strBody = new StringBuffer("&rho_callback=1&status=");

		strBody = strBody.append(status);
		if (param_name != null) {
			strBody = strBody.append("&");
			strBody = strBody.append(param_name);
			strBody = strBody.append("=");
			strBody = strBody.append(param_value);
		}
		
		fireRhoCallback(mCallbackURL, strBody.toString());
	}

	private static native void fireRhoCallback(String url, String body);
	
	
}
