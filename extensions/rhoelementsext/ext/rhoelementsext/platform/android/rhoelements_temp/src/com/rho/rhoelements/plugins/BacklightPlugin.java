package com.rho.rhoelements.plugins;

import java.util.Arrays;

import org.json.JSONArray;
import org.json.JSONObject;

import android.content.Context;
import android.os.PowerManager;
import android.view.View;
import android.view.WindowManager.LayoutParams;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author Ben Kennedy (NCVT73)
 * A plugin to control the display backlight as well as setting the intensity of the backlight.
 */
public class BacklightPlugin extends Plugin
{
	private static final String METHOD_ON = "On";
	private static final String METHOD_OFF = "Off";
	private static final String METHOD_SETTINGS = "getBacklightSettings";
	private static final String PARAMETER_INTENSITY = "Intensity";
	private static final String EVENT_SETTINGS = "backlightSettingsEvent";
	private static final String[] INTENSITY_RANGE = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59","60","61","62","63","64","65","66","67","68","69","70","71","72","73","74","75","76","77","78","79","80","81","82","83","84","85","86","87","88","89","90","91","92","93","94","95","96","97","98","99","100"};
	private static final int	MAX_INTENSITY	= 100;
	private static final int	MIN_INTENSITY	= 0;
	private static final int 	INTENSITY_VALUE = 0;
	private static final int 	INTENSITY_RANGE_VALUE = 1;
	private static final int 	STATE_VALUE = 2;
	
	private int intensity;
	private float initialIntensity;
	private boolean isOn;
	private String settingsUrl;
	
	private String[] settingsNames = {"intensity","intensityRange","state"};
	
	public BacklightPlugin()
	{
		isOn = true;
		LayoutParams lp = Common.mainActivity.getWindow().getAttributes();
		initialIntensity = lp.screenBrightness;
		intensity = (int) (initialIntensity * 100);
		settingsUrl = null;		
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Minimum intensity: " + (MIN_INTENSITY -1) + ", Maximum intensity: " + (MAX_INTENSITY - 1)));
	}
	
	@Override
	public void onSetting(PluginSetting setting)
	{
		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));

		if (setting.getName().equalsIgnoreCase(METHOD_ON))
		{			
			Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","backlight_layout")).setVisibility(View.GONE);			
			isOn = true;
		}
		else if (setting.getName().equalsIgnoreCase(METHOD_OFF))
		{
			Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","backlight_layout")).setVisibility(View.VISIBLE);			
			isOn = false;
		}
		else if (setting.getName().equalsIgnoreCase(PARAMETER_INTENSITY))
		{
			try
			{
				boolean doSet = true;
				//Validate check and normalise
				int intIntensity = Integer.parseInt(setting.getValue());
				if(intIntensity == 0)
				{
					Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","backlight_layout")).setVisibility(View.VISIBLE); //set to off if 0
					isOn = false;
					doSet = false;
					intensity = 0;
				}
				if(intIntensity > MAX_INTENSITY)
				{
					intIntensity = MAX_INTENSITY;
				}
				else if(intIntensity < MIN_INTENSITY)
				{
					doSet = false;//Done to match weird PB behaviour where negative numbers are ignored
//					intIntensity = MIN_INTENSITY;
				}
				
				if(doSet)
				{
					if(intensity == 0)
					{
						Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","backlight_layout")).setVisibility(View.GONE);
						isOn = true;
					}
					intensity = intIntensity;
					setIntensity(intIntensity);
				}
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid intensity value."));
			}
		}
		else if (setting.getName().equalsIgnoreCase(METHOD_SETTINGS))
		{
			LayoutParams lp = Common.mainActivity.getWindow().getAttributes();
			PowerManager pm = (PowerManager) Common.mainActivity.getSystemService(Context.POWER_SERVICE);
			
			JSONArray array = new JSONArray();
			JSONArray intensityRange = new JSONArray(Arrays.asList(INTENSITY_RANGE));
			
			array.put("" + (lp.screenBrightness * 100));
			array.put(intensityRange);
			
			//TODO add wakelock=on clause if we add extra functionality to set timeouts
			if(pm.isScreenOn() && isOn)
			{
				array.put("timeout");
			}
			else
			{
				array.put("off");
			}
			
			try
			{
				if(settingsUrl != null)
				{
					navigate(settingsUrl, settingsNames, array);
				}
			}
			catch (NavigateException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
			}

		}
		else if (setting.getName().equalsIgnoreCase(EVENT_SETTINGS))
		{
			settingsUrl = setting.getValue();
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unknown method or parameter: " + setting.getName()));
		}
	}
	
	@Override
	public void onShutdown()
	{
		settingsUrl = null;
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	}
	
	private void setIntensity(int intensity)
	{		
		LayoutParams lp = Common.mainActivity.getWindow().getAttributes();
		lp.screenBrightness = (float) intensity / (float) MAX_INTENSITY;
		Common.mainActivity.getWindow().setAttributes(lp);		
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("BacklightPlugin");
	}
}
