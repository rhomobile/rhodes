package com.rho.rhoelements.plugins;

import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import android.os.Handler;
import android.os.Message;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;

/**
 * @author hpkx48
 */
public class AlarmPlugin extends Plugin
{

	private final int	NO_OPTION_SET		= 0;
	private final int	INTERVAL_EVENT		= 1;
	private final int	TIME_EVENT			= 2;

	private final int	DEFAULT_INTERVAL	= 30000;
	private String		strInterval;
	private Timer		theTimer			= new Timer();
	private boolean		repeatFlag			= false;
	private String		OneShotEventURL;
	private Date		mdatetofire			= null;
	private long		mtimetofire			= 0;
	private int			mFireOption			= NO_OPTION_SET;

	public AlarmPlugin()
	{
		CallbackHandlerObj = new CallbackHandler(this);
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	}

	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("Alarm");
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
		clearAlarm();
	}

	@Override
	public void onSetting(PluginSetting setting)
	{
		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(),
					setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));

		if (setting.getName().equalsIgnoreCase("Interval"))
		{
			strInterval = setting.getValue();
			mdatetofire = null;
			mFireOption = INTERVAL_EVENT;
			try
			{
				mtimetofire = ConvertToMilliSeconds(strInterval);
			}
			catch (Exception ex)
			{
				// clear timer, since there was an error
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "ERROR Bad Syntax for Interval Command  "));
			}
		}
		else if (setting.getName().equalsIgnoreCase("Repeat"))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Repeat"));

			// Do the String to boolean conversion
			repeatFlag = Boolean.parseBoolean(setting.getValue());
		}
		else if (setting.getName().equalsIgnoreCase("Time"))
		{
			try
			{
				// Reset mtimetofire
				mtimetofire = 0;

				mFireOption = TIME_EVENT;

				// Set Alarm Time
				mdatetofire = setAlarmTime(setting.getValue());
			}
			catch (Exception ex)
			{
				// clear timer, since there was an error
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,
						"ERROR Setting Alarm Fire Time! Please check syntax(Time)."));
				mdatetofire = null;
			}
		}
		else if (setting.getName().equalsIgnoreCase("Set"))
		{
			// Do something
			// Sets an interval after which the alarm will fire.
			// The delay cannot be set to less that 30 seconds,
			// if a value of less than 30 seconds is supplied,
			// the delay will be defaulted to 30 seconds.
			// Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Set: " + strInterval +
			// " repeatFlag=" + repeatFlag));
			if (OneShotEventURL != null && OneShotEventURL.length() > 0)
			{
				switch (mFireOption)
				{
					case INTERVAL_EVENT:
						scheduleTimer(mtimetofire);
						break;
					case TIME_EVENT:
						scheduleTimer(mdatetofire);
						break;
					default:
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,
								"ERROR Invalid Configuration Parameters (Set)."));
				}
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "ERROR Bad URL (Set)."));
			}

		}
		else if (setting.getName().equalsIgnoreCase("AlarmTriggered"))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Alarm Triggered"));
			OneShotEventURL = setting.getValue();
		}
		else if (setting.getName().equalsIgnoreCase("Clear"))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Alarm Clear"));
			clearAlarm();
		}

	}

	private void clearAlarm()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		theTimer.cancel();
		OneShotEventURL = "";
		repeatFlag = false;
		strInterval = "";
		mdatetofire = null;
		mtimetofire = 0;
		mFireOption = NO_OPTION_SET;
	}

	/**
	 * Method to set the Alarm time based on a passed interval string.
	 * 
	 * @param setting
	 *            accepts a string indicating the time interval
	 * @return an object of type Date
	 */
	private Date setAlarmTime(String setting)
	{
		// Sets the alarm to trigger at the specified time.
		String[] parsedarray = setting.split("t");
		String[] filterstr = parsedarray[0].split("-");

		String Year = filterstr[0];
		String Month = filterstr[1];
		String Day = filterstr[2];
		boolean positive = false;

		String time;
		String alarmhours;
		String Hrs;
		String Mins;
		String Secs;
		String AlmHrs;
		String AlmMins;

		// split on plus or minus
		if (parsedarray[1].contains("+"))
		{
			positive = true;
			filterstr = parsedarray[1].split("[+]");
			time = filterstr[0];
			alarmhours = filterstr[1];
			filterstr = time.split("-");
			Hrs = filterstr[0];
			Mins = filterstr[1];
			Secs = filterstr[2];

			filterstr = alarmhours.split("-");
			AlmHrs = filterstr[0];
			AlmMins = filterstr[1];
		}
		else
		{
			positive = false;
			filterstr = parsedarray[1].split("-");
			Hrs = filterstr[0];
			Mins = filterstr[1];
			Secs = filterstr[2];
			AlmHrs = filterstr[3];
			AlmMins = filterstr[4];
		}

		int gmtAdjustmentmilliseconds = ((Integer.parseInt(AlmHrs) * 3600) + Integer.parseInt(AlmMins) * 60) * 1000;

		int iYear = Integer.parseInt(Year) - 1900;
		int iMonth = Integer.parseInt(Month) - 1;
		int iDay = Integer.parseInt(Day);
		int iHour = Integer.parseInt(Hrs);
		int iMinute = Integer.parseInt(Mins);
		int iSecond = Integer.parseInt(Secs);

		Date datetofire = new Date(iYear, iMonth, iDay, iHour, iMinute, iSecond);

		// get time in milliseconds
		long milliseconds = datetofire.getTime();

		// apply gmt adjustment
		if (positive == true)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "timezone is +" + AlmHrs + ":" + AlmMins));
			milliseconds += gmtAdjustmentmilliseconds;
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "timezone is +" + AlmHrs + ":" + AlmMins));
			milliseconds -= gmtAdjustmentmilliseconds;
		}

		datetofire = new Date(milliseconds);

		return (datetofire);
	}

	/**
	 * Method used for firing an event to java script returning the current network state.
	 */
	private void fireOneShotEvent()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "firing fireOneShotEvent!!!"));
		navigate(OneShotEventURL);
	}

	/**
	 * Sets an interval after which the alarm will fire. The delay cannot be set to less that 30
	 * seconds, if a value of less than 30 seconds is supplied, the delay will be defaulted to 30
	 * seconds.
	 */
	private void scheduleTimer(long timetofire)
	{
		theTimer.cancel();
		theTimer = new Timer();

		try
		{
			theTimer.scheduleAtFixedRate(new doTimerFire(repeatFlag), timetofire, timetofire);
		}
		catch (Exception ex)
		{

		}

	}

	/**
	 * Method to convert a formatted string value to milliseconds format is HH-MM-SS
	 * 
	 * @param strValue
	 * @return value in milliseconds
	 */
	private long ConvertToMilliSeconds(String strValue)
	{
		long timetofire = 0;
		String[] newarray;
		newarray = strValue.split("-");
		String Hrs = newarray[0];
		String Mins = newarray[1];
		String Secs = newarray[2];
		timetofire = ((Integer.parseInt(Hrs) * 3600) + (Integer.parseInt(Mins) * 60) + Integer.parseInt(Secs)) * 1000;

		if (timetofire < DEFAULT_INTERVAL)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,
					"WARNING: You tried to set a interval less than the default minimum, setting default minimum "
							+ DEFAULT_INTERVAL / 1000 + " seconds"));
			timetofire = DEFAULT_INTERVAL;
		}

		return (timetofire);
	}

	/**
	 * Method to schedule timer based on a date and time.
	 * 
	 * @param datetofire
	 */
	private void scheduleTimer(Date datetofire)
	{
		theTimer.cancel();
		theTimer = new Timer();

		try
		{
			// Schedule the timer but ignore repeat, this is used by time scheduling
			theTimer.scheduleAtFixedRate(new doTimerFire(false), datetofire, 1);
		}
		catch (Exception ex)
		{

		}
	}

	/**
	 * Method that is scheduled based via timer mechanism.
	 */
	private class doTimerFire extends TimerTask
	{
		private boolean	repeatFlag;

		public doTimerFire(boolean repeatFlag)
		{
			this.repeatFlag = repeatFlag;
		}

		@Override
		public void run()
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "timer firing!!"));
			// if repeatFlag has been set to false then cancel this timer after one shot
			if (repeatFlag == false)
			{
				this.cancel();
			}

			// Fire Network State Changed Event
			mHandler.sendMessage(mHandler.obtainMessage(0, 1, 0));
		}
	} // eof run

	/**
	 * Internal Message Handler to process calls from Timer Task and allow OneShot Event to be
	 * created as Thread on the parents thread context
	 */
	private Handler	mHandler	= new Handler()
	{
		@Override
		public void handleMessage(Message msg)
		{
			fireOneShotEvent();
		}
	};
}
