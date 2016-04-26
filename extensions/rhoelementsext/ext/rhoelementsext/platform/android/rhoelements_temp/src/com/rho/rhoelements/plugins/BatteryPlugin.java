/**
 * 
 */
package com.rho.rhoelements.plugins;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rho.rhoelements.graphics.BatteryIndicator;

/**
 * @author FPC843
 *
 */
public class BatteryPlugin extends Plugin
{

	//TODO: Has to be tested whether, when non in FG and stopped by Android, if Helium gets back
	//      to the FG if it reloads the meta tags (and then updating the indicator status).

	//TODO: In case asynchronorus doesn't work well, let's implement polling like for the SignalPlugin

	private static final String	PARAM_GRAPHPOSITION	= "graphposition";
	private static final String	PARAM_ICONPOSITION	= "iconposition";
	private static final String	PARAM_LAYOUT		= "layout";
	private static final String	VALUE_UP			= "up";
	private static final String	VALUE_DOWN			= "down";

	private static final short NORMALISATION = 10;

	/*
	 * We only have 3 values for the Battery Event
	 * 		AC Line Status
	 * 		Battery Life Percent
	 * 		Backup Battery life percent
	 */
	private static final short BATTERY_EVENT_VALUES = 3;
	
	/*
	 * No Backup battery for Android
	 */
	private static final String BACKUP_BATTERY_LIFE = "0";

	private String batteryUrl;
	private BatteryIndicator batteryIndicator;
	private int batteryLevel = -1;
	private boolean isPowerConnected = false;

	private static final String[] NAMES = new String[] {
		"acLineStatus",
		"batteryLifePercent",
	"backupBatteryLifePercent"};
	
	private String[] values = new String[BATTERY_EVENT_VALUES];

	private BroadcastReceiver mBatInfoReceiver = new BroadcastReceiver(){
		@Override
		public void onReceive(Context arg0, Intent intent) {
			if (intent.getAction().equals(Intent.ACTION_BATTERY_CHANGED))
			{
				batteryLevel = intent.getIntExtra("level", 0);
				if (intent.getIntExtra("plugged", 0) == 0)
					isPowerConnected = false;
				else
					isPowerConnected = true;
			}
			passValuesToEvent();
			redrawBatteryIndicator();
		}
	};
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version ("Battery");
	}

	public BatteryPlugin()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		//This value never changes as there is no backup battery
		values[2] = BACKUP_BATTERY_LIFE;

		// Get signal indicator via indicator panel
		this.batteryIndicator = Common.elementsCore.getIndicatorPanel().getBatteryIndicator();

		batteryIndicator.setVisible(false);
		batteryIndicator.setPaintColor("#000000");

		Common.mainActivity.registerReceiver(this.mBatInfoReceiver, 
				new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
		getBatteryProperties();
		batteryUrl = null;
	}

	/**************************************************************************/
	@Override
	public void onShutdown()
	{
		batteryIndicator.reset();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		try
		{
		Common.mainActivity.unregisterReceiver(this.mBatInfoReceiver);
		}
		catch (IllegalArgumentException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Battery Receiver wasn't registered"));
		}
	}

	/**************************************************************************/
	@Override
	public void onSetting(PluginSetting setting)
	{
		getBatteryProperties();
		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));

		if (setting.getName().equalsIgnoreCase("batteryevent"))
		{
			batteryUrl = setting.getValue();
			if (batteryUrl.length() > 0)
				passValuesToEvent();
		}
		else if (setting.getName().equalsIgnoreCase("visibility"))
		{
			if(setting.getValue().equalsIgnoreCase("visible"))
			{
				batteryIndicator.setVisible(true);
				batteryIndicator.reDraw();
			}
			else if(setting.getValue().equalsIgnoreCase("hidden"))
			{
				batteryIndicator.setVisible(false);
				batteryIndicator.reDraw();
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid visibility value '" + setting.getValue() + "'"));
			}
		}
		else if (setting.getName().equalsIgnoreCase("left"))
		{
			try
			{
				int xPosition = Integer.parseInt(setting.getValue());
				batteryIndicator.setxPosition(xPosition, true);
				batteryIndicator.reDraw();
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid left value: " + setting.getValue()));
			}
		}
		else if (setting.getName().equalsIgnoreCase("top"))
		{
			try
			{
				int yPosition = Integer.parseInt(setting.getValue());
				batteryIndicator.setyPosition(yPosition, true);
				batteryIndicator.reDraw();
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid top value: " + setting.getValue()));
			}
		}
/*		else if (setting.getName().equalsIgnoreCase("right"))
		{
			try
			{
				int xPosition = Integer.parseInt(setting.getValue());
				batteryIndicator.setxPosition(xPosition, false);
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid right value: " + setting.getValue()));
			}
		}
		else if (setting.getName().equalsIgnoreCase("bottom"))
		{
			try
			{
				int yPosition = Integer.parseInt(setting.getValue());
				batteryIndicator.setyPosition(yPosition, false);
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid bottom value: " + setting.getValue()));
			}
		}*/
		else if (setting.getName().equalsIgnoreCase(PARAM_ICONPOSITION))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, PARAM_ICONPOSITION + " is no longer supported. Please use " + PARAM_LAYOUT + " instead"));
		}
		else if (setting.getName().equalsIgnoreCase(PARAM_GRAPHPOSITION))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, PARAM_GRAPHPOSITION + " has been deprecated but is still usable. In the future please use \"" + PARAM_LAYOUT + "\"."));
			setLayout(setting.getValue());
		}
		else if (setting.getName().equalsIgnoreCase(PARAM_LAYOUT))
		{
			setLayout(setting.getValue());
		}
		else if (setting.getName().equalsIgnoreCase("color"))
			batteryIndicator.setPaintColor(setting.getValue()); //Validation checking for color is in Indicator class
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unknown setting '" + setting.getName() + "'"));

//		redrawBatteryIndicator();

		// TODO Add check interval property (extra to PB3)
	}
	
	/**
	 * Sets the layout of the battery indicator
	 * @param value the layout String: "up"/"down"/"left"/"right"/("top")/("bottom")
	 */
	private void setLayout(String value)
	{
		if (value.equalsIgnoreCase("left"))
		{
			batteryIndicator.setLayout(BatteryIndicator.LEFT);
			batteryIndicator.reDraw();
		}
		else if (value.equalsIgnoreCase("right"))
		{
			batteryIndicator.setLayout(BatteryIndicator.RIGHT);
			batteryIndicator.reDraw();
		}
		else if (value.equalsIgnoreCase("top") || value.equalsIgnoreCase(VALUE_UP))
		{
			batteryIndicator.setLayout(BatteryIndicator.UP);
			batteryIndicator.reDraw();
		}
		else if (value.equalsIgnoreCase("bottom") || value.equalsIgnoreCase(VALUE_DOWN))
		{
			batteryIndicator.setLayout(BatteryIndicator.DOWN);
			batteryIndicator.reDraw();
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid layout '" + value + "'"));
		}
	}

	@Override
	public void onPageStarted(String url)
	{
		batteryUrl = null;
	}

	private void getBatteryProperties()
	{
		Intent batteryIntent = Common.mainActivity.getApplicationContext().registerReceiver(null,
				new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
		batteryLevel = batteryIntent.getIntExtra("level", -1);
		if (batteryIntent.getIntExtra("plugged", 0) == 0)
			isPowerConnected = false;
		else 
			isPowerConnected = true;
	}

	private void redrawBatteryIndicator()
	{
		batteryIndicator.setValue(batteryLevel / NORMALISATION);
		batteryIndicator.setCharging(isPowerConnected);
		batteryIndicator.reDraw();
	}

	private void passValuesToEvent()
	{
		values[1] = String.valueOf(batteryLevel);
		if (isPowerConnected)
			values[0] = "1";
		else
			values[0] = "0";
		if(batteryUrl != null)
		{
			try
			{
				navigate(batteryUrl, NAMES, values);
			}
			catch (NavigateException e)
			{
				e.printStackTrace();
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
			}
		}
	}
}
