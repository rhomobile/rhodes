package com.rho.rhoelements.plugins;

import java.security.InvalidParameterException;

import android.graphics.drawable.AnimationDrawable;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.FrameLayout;
import android.widget.ImageView;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.Config;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author Ben Kennedy (NCVT73)
 * Plugin to control the "Hourglass" (Wait cursor). Reads data from the config file that
 * sets whether the hourglass is shown on page load (default) or not.
 */
public class HourglassPlugin extends Plugin
{
	private static final String PARAM_VISBILITY	= "visibility";
	private static final String PARAM_LEFT		= "left";
	private static final String PARAM_TOP		= "top";
	private static final String	VALUE_VISIBLE	= "visible";
	private static final String	VALUE_HIDDEN	= "hidden";
	private boolean setInConfig;
	private FrameLayout spinnerPanel;
	private ImageView spinnerView;
	private AnimationDrawable spinnerAnim;
	private int left;
	private int top;

	public HourglassPlugin()
	{
		setInConfig = true;
		spinnerPanel = (FrameLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","hourglass_panel"));
		spinnerView = (ImageView) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","spinnerView"));
		spinnerAnim = (AnimationDrawable) spinnerView.getDrawable();
		top = -1;
		left = -1;
		
		boolean	changed = false;
		
		changed |= setTop(Common.config.getSetting(Config.SETTING_HOURGLASS_TOP));
		changed |= setLeft(Common.config.getSetting(Config.SETTING_HOURGLASS_LEFT));
		
		String enabledString = Common.config.getSetting(Config.SETTING_HOURGLASS_ENABLED);
		if(enabledString != null)
		{
			if(enabledString.equals("0"))
			{
				setInConfig = false;
			}
			else if(enabledString.equals("1"))
			{
				setInConfig = true;
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Config.xml incorrect: Unrecognised value for " + Config.SETTING_HOURGLASS_ENABLED));
				setInConfig = true;
			}
		}
		else
		{
			setInConfig = true;
		}
		if(changed)
		{
			position();
		}
	}

	@Override
	public void onSetting(PluginSetting setting)
	{
		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));
		
		if(setting.getName().equals(PARAM_VISBILITY))
		{
			if(setting.getValue().equalsIgnoreCase(VALUE_VISIBLE))
			{
				show();
			}
			else if(setting.getValue().equalsIgnoreCase(VALUE_HIDDEN))
			{
				hide();
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised " + PARAM_VISBILITY + " value: " + setting.getValue()));
			}
		}
		else if(setting.getName().equals(PARAM_LEFT))
		{
			if(setLeft(setting.getValue()))
			{
				position();
			}
		}
		else if(setting.getName().equals(PARAM_TOP))
		{
			if(setTop(setting.getValue()))
			{
				position();
			}
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter" + setting.getName()));
		}
	}
	
	/**
	 * Sets the top coordinate of the hourglass graphic
	 * @param topString The unparsed sting of the top value
	 * @return whether the top value was changed
	 */
	private boolean setTop(String topString)
	{
		// Ignore empty string so it doesn't complain because of the default config		
		if(topString != null && topString.length() != 0)
		{
			try
			{
				int tempTop = Integer.parseInt(topString);
				if(tempTop < 0)
				{
					throw new InvalidParameterException();
				}
				top = tempTop;
				if(left == -1)
				{
					left = 0;
				}
				return true;
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Could not parse value for " + PARAM_TOP));
			}
			catch(InvalidParameterException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Negative values are not allowed for " + PARAM_TOP));
			}
		}
		return false;
	}
	
	/**
	 * Sets the left coordinate of the hourglass graphic
	 * @param topString The unparsed sting of the left value
	 * @return whether the left value was changed
	 */
	private boolean setLeft(String leftString)
	{
		// Ignore empty string so it doesn't complain because of the default config
		if(leftString != null && leftString.length() != 0)
		{
			try
			{
				int tempLeft = Integer.parseInt(leftString);
				if(tempLeft < 0)
				{
					throw new InvalidParameterException();
				}
				left = tempLeft;
				if(top == -1)
				{
					top = 0;
				}
				return true;
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Could not parse value for " + PARAM_LEFT));
			}
			catch(InvalidParameterException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Negative values are not allowed for " + PARAM_LEFT));
			}
		}
		return false;
	}

	@Override
	public void onShutdown()
	{
		
	}

	@Override
	public void onPageStarted(String url)
	{
		if(setInConfig)
		{
			show();
		}
	}
	
	@Override
	public void onPageFinished(String url)
	{
		hide();
	}
	
	/**
	 * Shows the hourglass.
	 */
	public void show()
	{
		spinnerPanel.setVisibility(View.VISIBLE);
		spinnerView.setVisibility(View.VISIBLE);
		spinnerAnim.setVisible(true,false);
		spinnerAnim.start();
	}

	/**
	 * Hides the hourglass.
	 */
	public void hide()
	{
		spinnerAnim.stop();
		spinnerAnim.setVisible(false, false);
		spinnerView.setVisibility(View.GONE);
		spinnerPanel.setVisibility(View.GONE);
	}
	
	/**
	 * Repositions the Signature area. This is called when the position or the dimensions change
	 * and should only be called when you know that these have been changed. Otherwise you'll waste
	 * time and power. 
	 */
	private void position()
	{
		if(top != -1 && left != -1)
		{
			spinnerView.setLayoutParams(new FrameLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, Gravity.TOP));
			spinnerPanel.setForegroundGravity(Gravity.TOP);
			spinnerPanel.setPadding(left, top, 0, 0);
		}
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("HourglassPlugin");
	}
}
