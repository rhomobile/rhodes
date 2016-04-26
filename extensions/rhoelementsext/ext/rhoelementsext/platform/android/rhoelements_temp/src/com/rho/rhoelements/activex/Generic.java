package com.rho.rhoelements.activex;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;
import android.os.Handler;
import android.os.Message;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rhomobile.rhodes.Logger;

/**
 * Implements functions handled by Generic ActiveX object in Browser. Added to JavScript context after page load. Calls to class methods from JavaScript come on a different thread, so they are synchronised to the main thread before action if necessary
 * 
 * @author xfh386
 * 
 */
public class Generic extends ActiveX
{
    private static final String TAG = Generic.class.getSimpleName();
    private List<File> soundFileQueue;

    private static Generic sInstance = null;
    
    public static synchronized Generic getInstance() {
        if (sInstance == null) {
            sInstance = new Generic();
        }
        return sInstance;
    }
	
	private Generic()
	{
		soundFileQueue = new ArrayList<File>();
	}

	/**
	 * @throws Exception
	 ************************************************************************/
	// equiv = Args[0], content = Args[1]
	public String[] invokemetafunction(String args[]) throws Exception
	{
		if (args.length != 2)
		{
			// TODO: change to return/error code since this has to go through NPAPI back to user
			throw new Exception("Invalid number of parameters to Generic.InvokeMETAFunction. Expecting 2,  recieved " + args.length);
		}

		// Use the same method to process as is used for actual <meta> tags.
		// It will also synchronise to the main thread.
		Common.metaReceiver.setMeta(args[0], args[1]);

		return null;
	}

	/**************************************************************************/
	//  args[0] comment, args[1] severity
	public String[] log(String args[]) throws Exception
	{
		if (args.length != 2)
		{
			// TODO: change to return/error code since this has to go through NPAPI back to user
			throw new Exception("Invalid number of parameters to Generic.Log. Expecting 2,  recieved " + args.length);
		}

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, args[0] + ", " + args[1]));

		String comment = args[0];

		int severity;
		try
		{
			severity = Integer.parseInt(args[1]);
		}
		catch (Exception e)
		{
			severity = 0;
		}

		// Submit log message to logging system.
		// This is thread-safe, and the actual log output is done in another thread.

		// Translate severity figure to text and use as 'caller' field
		String caller;
		switch (severity)
		{
		case 1:
			caller = "Low";
			break;
		case 2:
			caller = "Medium";
			break;
		case 3:
			caller = "High";
			break;
		default:
			caller = "Unknown";
			break;
		}

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_USER, caller, Common.elementsCore.getCurrentUrl(), comment, 0));

		// Always return true
		String[] result = new String[1];
		result[0] = "true";

		return result;
	}

	/**************************************************************************/
	//  args[0] strImageName, args[1] strCommandLine
	public String[] launchprocessnonblocking(String args[]) throws Exception
	{
		if (args.length != 2)
		{
			// TODO: change to return/error code since this has to go through NPAPI back to user
			throw new Exception("Invalid number of parameters to Generic.LaunchProcessNonBlocking. Expecting 2,  recieved " + args.length);
		}

		return null;
	}
	
	/**************************************************************************/
	//  args[0] strSound, args[1] iFlags
	public String[] playsound(String args[]) throws Exception
	{
		return playwave(args);
	}

	/**************************************************************************/
	//  args[0] strSound, args[1] iFlags
	public String[] playwave(String args[]) throws Exception
	{
		if (args.length != 2)
		{
			// TODO: change to return/error code since this has to go through NPAPI back to user
			throw new Exception("Invalid number of parameters to Generic.PlayWave. Expecting 2,  recieved " + args.length);
		}

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, args[0] + ", " + args[1]));
		
		File soundFile = null;
		int flags;
		boolean flagAsynchronous = false;
		boolean flagRepeat = false;
		boolean flagIsPolite = false;
		//boolean flagNoDefault = false;
		//boolean flagInRam = false;
		
		if(!args[0].equals("") && !args[0].equalsIgnoreCase("null"))
		{
			try//Parse sound file URL
			{
				soundFile = Common.getLocalFile(args[0]);
			}
			catch(URISyntaxException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
				String[] result = new String[1];
				result[0] = "false"; 

				return result;
			}
			catch(InvalidParameterException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
				String[] result = new String[1];
				result[0] = "false"; 

				return result;
			}
		}
		
		try//Decode Flags
		{
			flags = Integer.decode(args[1]);
			flagAsynchronous = ((flags & 0x1) != 0);
			flagRepeat = ((flags & 0x8) != 0);
			flagIsPolite = ((flags & 0x10) != 0);
			//flagNoDefault = ((flags & 0x2) != 0);
			//flagInRam = ((flags & 0x4) != 0); //Ignored
		}
		catch(NumberFormatException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid flags value."));
		}
		
		if(soundFile == null)
		{
			if(Common.mediaPlayer != null)
			{
				synchronized(Common.mediaPlayer)
				{
					if(Common.mediaPlayer != null)
					{
						if(flagIsPolite)
						{
							soundFileQueue.clear();
							Common.mediaPlayer.setLooping(false);
						}
						else
						{
							Common.mediaPlayer.stop();
							soundFileQueue.clear();
							Common.mediaPlayer.reset();
						}
					}
					else
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Sound file set to null, but no sound is playing. Ignoring null request"));
					}
				}
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Sound file set to null, but no sound is playing. Ignoring null request"));
			}
			String[] result = new String[1];
			result[0] = "true"; 

			return result;
		}

		if (!soundFile.exists())
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Sound file does not exist."));
			String[] result = new String[1];
			result[0] = "false"; 

			return result;
		}
		if(!soundFile.canRead())
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot read sound file."));
			String[] result = new String[1];
			result[0] = "false"; 

			return result;
		}

		if(Common.mediaPlayer == null)
		{
			Common.mediaPlayer = new MediaPlayer();
			synchronized(Common.mediaPlayer)
			{
				AudioManager am = (AudioManager) Common.mainActivity.getSystemService(Context.AUDIO_SERVICE);
				Common.mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
				Common.mediaPlayer.setLooping(flagRepeat);
				if (am.getMode() != AudioManager.MODE_NORMAL)
				{
					am.setMode(AudioManager.MODE_NORMAL);
				}
				if (!am.isSpeakerphoneOn())
				{
					am.setSpeakerphoneOn(true);
				}
				Common.mediaPlayer.setOnPreparedListener(new OnPreparedListener()
				{
					@Override
					public void onPrepared(MediaPlayer mp)
					{
						mp.start();
					}
				});
				Common.mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener()
				{
					@Override
					public void onCompletion(MediaPlayer mp)
					{
						synchronized(soundFileQueue)
						{
							if(!soundFileQueue.isEmpty())
							{
								setupMediaPlayer(soundFileQueue.remove(0).getAbsolutePath(), false);
							}
							else
							{
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
		synchronized (Common.mediaPlayer)
		{
			if(Common.mediaPlayer.isPlaying())
			{
				if(!flagIsPolite)
				{
					Common.mediaPlayer.stop();
					soundFileQueue.clear();
					
					setupMediaPlayer(soundFile.getAbsolutePath(), flagRepeat);
					
				}
				else
				{
					soundFileQueue.add(soundFile);//Queue up
				}
			}
			else
			{
				setupMediaPlayer(soundFile.getAbsolutePath(), flagRepeat);
			}
		}
		String[] result = new String[1];
		result[0] = "true"; 

		return result;
	}

	private void setupMediaPlayer(String absolutePath, boolean isLooping)
	{
		try
		{
			Common.mediaPlayer.reset();
			Common.mediaPlayer.setDataSource(absolutePath);
			Common.mediaPlayer.setLooping(isLooping);
			Common.mediaPlayer.prepare();
		}
		catch (IllegalArgumentException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Bad file argument"));
		}
		catch (IllegalStateException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "MediaPlayer in bad state."));
		}
		catch (IOException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "File Access error"));
		}
	}

	/**************************************************************************/
	//  args[0] strSetting, args[1] strValue
	public String[] writeusersetting(String args[]) throws Exception
	{
		if (args.length != 2)
		{
			// TODO: change to return/error code since this has to go through NPAPI back to user
			throw new Exception("Invalid number of parameters to Generic.WriteUserSetting . Expecting 2,  recieved " + args.length);
		}

		return null;
	}

	/**************************************************************************/
	//  args[0] strSetting
	public String[] readusersetting(String args[]) throws Exception
	{
		if (args.length != 1)
		{
			// TODO: change to return/error code since this has to go through NPAPI back to user
			throw new Exception("Invalid number of parameters to Generic.ReadUserSetting . Expecting 1,  recieved " + args.length);
		}

		String[] result = new String[1];
		result[0] = "tralalala";

		return result;
	}

	/**************************************************************************/
	//  
	public String[] get(String args[]) throws Exception
	{
		String[] result = null;

		if (args.length != 1)
		{
			// TODO: change to return/error code since this has to go through NPAPI back to user
			throw new Exception("Invalid number of parameters to Generic.Info . Expecting none,  recieved " + args.length);
		}

		if (args[0].equalsIgnoreCase("oeminfo"))
		{
			result = new String[1];
			result[0] = Common.getOEMInfo();
		}
		else if (args[0].equalsIgnoreCase("uuid"))
		{
			result = new String[1];
			result[0] = Common.getUUID();
		}
		return result;
	}


	/**************************************************************************/
	public void Quit()
	{
		//handler.sendMessage(handler.obtainMessage(0, new QuitRequest()));
	}

	/**************************************************************************/
	class GenericHandler extends Handler
	{
		@Override
		public void handleMessage(Message message)
		{
			// Message is GenericRequest object
			GenericRequest request = (GenericRequest) message.obj;
			request.execute();
		}
	}

	/**
	 * Base class for all Generic requests
	 */
	abstract class GenericRequest
	{
		/**
		 * Overridden by specific request handlers
		 */
		abstract void execute();
	}

	/**************************************************************************/
	class QuitRequest extends GenericRequest
	{
		@Override
		void execute()
		{
			Common.elementsCore.quit();
		}
	}

	/**************************************************************************/
	class LogRequest extends GenericRequest
	{
		int severity;
		String comment;

		public LogRequest(int severity, String comment)
		{
			this.severity = severity;
			this.comment = comment;
		}

		@Override
		void execute()
		{
			// Translate severity figure to text and use as 'caller' field
			String caller;
			switch (severity)
			{
			case 1:
				caller = "Low";
				break;
			case 2:
				caller = "Medium";
				break;
			case 3:
				caller = "High";
				break;
			default:
				caller = "Unknown";
				break;
			}

			Common.logger.add(new LogEntry(LogEntry.PB_LOG_USER, caller, Common.elementsCore.getCurrentUrl(), comment, 0));
		}
	}

	public void clearSoundQueue()
	{
		if(soundFileQueue != null)
		{
			soundFileQueue.clear();
		}
	}
}
