package com.rho.rhoelements.plugins;

import java.text.SimpleDateFormat;
import java.util.Date;

import android.os.Handler;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;

/**
 * @author Geoff Day (XFH386, Initial Creation)
 * 
 */
public class TimerPlugin extends Plugin
{
	public static Version getVersion()
	{
		return new Version("Timer");
	}

	private int interval;
	private String timeoutUrl;
	private Thread timerThread = null;

	void resetParameters()
	{
		interval = 1000;
		timeoutUrl = null;
	}

	public TimerPlugin()
	{
		resetParameters();

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	}

	@Override
	public void onShutdown()
	{
		stopTimer();

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	}

	@Override
	public void onPageStarted(String url)
	{
		stopTimer();
		resetParameters();
	}

	@Override
	public void onSetting(PluginSetting setting)
	{
		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));

		if (setting.getName().equalsIgnoreCase("interval"))
		{
			try
			{
				interval = Integer.parseInt(setting.getValue());
				if(interval < 500)
				{
					interval = 500;
				}
			}
			catch (Exception e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid interval '" + setting.getValue() + "'"));
			}
		}
		else if (setting.getName().equalsIgnoreCase("timeout"))
		{
			timeoutUrl = setting.getValue();
		}
		else if (setting.getName().equalsIgnoreCase("start"))
		{
			// Stop any running timer
			stopTimer();

			// Start a new timer
			startTimer();
		}
		else if (setting.getName().equalsIgnoreCase("stop"))
		{
			// Stop any running timer
			stopTimer();
		}
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unknown setting '" + setting.getName() + "'"));
	}

	void startTimer()
	{
		// Already started?
		if (timerThread != null)
			return;

		timerThread = new Thread(new Sleeper(new Handler()));
		timerThread.start();

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	}

	void stopTimer()
	{
		if (timerThread == null)
			return;

		// Interrupt timer thread and wait for completion
		try
		{
			timerThread.interrupt();
			timerThread.join(5000);
		}
		catch (Exception e)
		{
		}

		// Allow garbage collection
		timerThread = null;

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	}

	/**
	 * Inner class to fire event Runs in a separate thread
	 */
	class Sleeper implements Runnable
	{
		Handler handler;

		Sleeper(Handler handler)
		{
			this.handler = handler;
		}

		@Override
		public void run()
		{
			try
			{
				Thread.sleep(interval);
			}
			catch (Exception e)
			{
				// Quit without firing an event if there's an exception e.g. interrupted
				return;
			}

			handler.post(new Runnable()
			{
				@Override
				public void run()
				{
					String datetime = new SimpleDateFormat("dd/MM/yy HH:mm:ss").format(new Date());
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Fired at " + datetime));
					try
					{
						navigate(timeoutUrl, "time", datetime);
					}
					catch (NavigateException e)
					{
						e.printStackTrace();
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
					}

					timerThread = null;
				}
			});
		}
	}
}
