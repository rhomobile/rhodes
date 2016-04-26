package com.rhoelementsext;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;
import android.util.SparseArray;
import android.webkit.JavascriptInterface;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.Config;
import com.rho.rhoelements.LogEntry;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

public class Generic {
    public static final String TAG = Generic.class.getSimpleName();
    
    private static Generic mInstance = null;
    
    public static synchronized Generic getInstance() {
        if (mInstance == null) {
            mInstance = new Generic();
        }
        return mInstance;
    }
    
    private static class ProcessStatus {
    
        public enum ExitCode {
            UNDEFINED(-1),
            SUCCESS(0),
            ERROR(1);
            
            public int code;
            
            ExitCode(int code) {
                this.code = code;
            }
        }
        
        public ExitCode exitCode = ExitCode.UNDEFINED;
    
    }
    
    private SparseArray<ProcessStatus> mProcessArray = new SparseArray<ProcessStatus>();
    
	private List<File> soundFileQueue;
	
	private Generic() {
		soundFileQueue = new ArrayList<File>();
	}

    @JavascriptInterface
    public void InvokeMETAFunction(String equiv, String content) {
        // Use the same method to process as is used for actual <meta> tags.
        // It will also synchronise to the main thread.
        Common.metaReceiver.setMeta(equiv, content);
    }
    
    @JavascriptInterface
    public int LaunchProcessNonBlocking(String appName, String args) {
        int handle = RhoExtManager.getInstance().getActivityResultNextRequestCode(RhoElementsProxy.getInstance());
        synchronized (mProcessArray) {
            mProcessArray.append(handle, new ProcessStatus());
        }
        try {
            RhodesService service = RhodesService.getInstance();

            Intent intent = new Intent();
            service.resolveAppName(appName, intent);
            service.parseAppParams(args, intent);

            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            RhodesActivity.safeGetInstance().startActivityForResult(intent, handle);
        } catch (Throwable e) {
            Logger.E(TAG, "Can't run application " + appName);
            Logger.E(TAG, e);
            return 0;
        }
        
        return handle;
    }
    
    @JavascriptInterface
    public void CloseProcess(int handle) {
        RhoExtManager.getInstance().dropActivityResultRequestCode(handle);
        synchronized (mProcessArray) {
            mProcessArray.delete(handle);
        }
    }
    
    @JavascriptInterface
    public int GetProcessExitCode(int handle) {
        ProcessStatus status = mProcessArray.get(handle);
        if (status != null) {
            return status.exitCode.code;
        }
        else {
            return ProcessStatus.ExitCode.UNDEFINED.code;
        }
    }
    
    @JavascriptInterface
    public String getuuid() {
        return Common.getUUID();
    }
    
    @JavascriptInterface
    public String getOEMInfo() {
        return Common.getOEMInfo();
    }
    
    @JavascriptInterface
    public boolean Log(String logMessage, int severity) {
    	Logger.D(TAG, "Log+");
    	if(Common.config.getSetting(Config.SETTING_LOG_USER).equals("1")) {
			String stringSeverity;
			switch (severity)
			{
				case 1: stringSeverity = "Low"; break;
				case 2:	stringSeverity = "Medium"; break;
				case 3: stringSeverity = "High"; break;
				default: stringSeverity = "Unknown"; break;
			}
	    	Logger.I("User-" + stringSeverity, logMessage + ": " +  Common.elementsCore.getCurrentUrl());
	    	Logger.D(TAG, "Log-");
	    	return true;
    	}
    	Logger.D(TAG, "Log-");
    	return false;
    }
        
    @JavascriptInterface
    public boolean SPBLog(String logMessage, String severity) {
    	Logger.D(TAG, "SPBLog");
    	if(severity == null || severity.equals("")) return false;
    	
    	if(severity.trim().equalsIgnoreCase("high")) {
    		return Log(logMessage, 3);
    	}
    	else if(severity.trim().equalsIgnoreCase("medium")) {
    		return Log(logMessage, 2);
    	}
    	else if(severity.trim().equalsIgnoreCase("low")) {
    		return Log(logMessage, 1);
    	}
    	
    	return false;
    }
    
    @JavascriptInterface
    public boolean PlayWave(String fileName, int flags) {
		File soundFile = null;
		boolean flagAsynchronous = false;
		boolean flagRepeat = false;
		boolean flagIsPolite = false;
		
		if(!fileName.equals("") && !fileName.equalsIgnoreCase("null")) {
			try { //Parse sound file URL
				soundFile = Common.getLocalFile(fileName);
			}
			catch(URISyntaxException e) {
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
				return false;
			}
			catch(InvalidParameterException e) {
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
				return false;
			}
		}
		
		try { //Decode Flags
			flagAsynchronous = ((flags & 0x1) != 0);
			flagRepeat = ((flags & 0x8) != 0);
			flagIsPolite = ((flags & 0x10) != 0);
		}
		catch(NumberFormatException e) {
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid flags value."));
		}
		
		if(soundFile == null) {
			if(Common.mediaPlayer != null) {
				synchronized(Common.mediaPlayer) {
					if(Common.mediaPlayer != null) {
						if(flagIsPolite) {
							soundFileQueue.clear();
							Common.mediaPlayer.setLooping(false);
						}
						else {
							Common.mediaPlayer.stop();
							soundFileQueue.clear();
							Common.mediaPlayer.reset();
						}
					}
					else {
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Sound file set to null, but no sound is playing. Ignoring null request"));
					}
				}
			}
			else {
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Sound file set to null, but no sound is playing. Ignoring null request"));
			}
			return true; 
		}

		if (!soundFile.exists()) {
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Sound file does not exist."));
			return false;
		}
		if(!soundFile.canRead()) {
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot read sound file."));
			return false;
		}

		if(Common.mediaPlayer == null) {
			Common.mediaPlayer = new MediaPlayer();
			synchronized(Common.mediaPlayer) {
				AudioManager am = (AudioManager) Common.mainActivity.getSystemService(Context.AUDIO_SERVICE);
				Common.mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
				Common.mediaPlayer.setLooping(flagRepeat);
				if (am.getMode() != AudioManager.MODE_NORMAL) {
					am.setMode(AudioManager.MODE_NORMAL);
				}
				if (!am.isSpeakerphoneOn()) {
					am.setSpeakerphoneOn(true);
				}
				Common.mediaPlayer.setOnPreparedListener(new OnPreparedListener() {
					@Override
					public void onPrepared(MediaPlayer mp) {
						mp.start();
					}
				});
				Common.mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
					@Override
					public void onCompletion(MediaPlayer mp) {
						synchronized(soundFileQueue) {
							if(!soundFileQueue.isEmpty()) {
								setupMediaPlayer(soundFileQueue.remove(0).getAbsolutePath(), false);
							}
							else {
								Common.mediaPlayer.stop();
								Common.mediaPlayer.release();
								Common.mediaPlayer = null;
								Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Released mediaPlayer"));
							}
						}
					}
				});
			}
		}
		synchronized (Common.mediaPlayer) {
			if(Common.mediaPlayer.isPlaying()) {
				if(!flagIsPolite) {
					Common.mediaPlayer.stop();
					soundFileQueue.clear();
					
					setupMediaPlayer(soundFile.getAbsolutePath(), flagRepeat);
					
				}
				else {
					soundFileQueue.add(soundFile);//Queue up
				}
			}
			else {
				setupMediaPlayer(soundFile.getAbsolutePath(), flagRepeat);
			}
		}
		return true;
    }
    
	private void setupMediaPlayer(String absolutePath, boolean isLooping) {
		try {
			Common.mediaPlayer.reset();
			Common.mediaPlayer.setDataSource(absolutePath);
			Common.mediaPlayer.setLooping(isLooping);
			Common.mediaPlayer.prepare();
		}
		catch (IllegalArgumentException e) {
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Bad file argument"));
		}
		catch (IllegalStateException e) {
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "MediaPlayer in bad state."));
		}
		catch (IOException e) {
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "File Access error"));
		}
	}
	    
    public void onActivityResult(int reqCode, int resCode) {
        ProcessStatus status = mProcessArray.get(reqCode);
        if (status != null) {
        
            Logger.T(TAG, "Activity result: " + resCode);
            
            if (resCode == Activity.RESULT_OK) {
                status.exitCode = ProcessStatus.ExitCode.SUCCESS;
            }
            else {
                status.exitCode = ProcessStatus.ExitCode.ERROR;
            }
        }
        else {
            Logger.D(TAG, "Unknown Activity result, request code: " + reqCode + " - skipping");
        }
    }
}
