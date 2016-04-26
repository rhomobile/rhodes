package com.rho.rhoelements.plugins;

import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsoluteLayout;
import android.widget.AutoCompleteTextView;
import android.widget.Button;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;


/**
 * @author hpkx48
 * Stylus Plugin, Plug-in Simply provides the ability to enable and disable on touch events 
 */
public class StylusPlugin extends Plugin
{	
	public StylusPlugin()
	{
		CallbackHandlerObj = new CallbackHandler(this);
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null)); 
	}

	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version ("Stylus");
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
		// turn on touch events 
		// window.clearFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
	}

	@Override
	public void onSetting(PluginSetting setting)
	{
		String name = setting.getName().toLowerCase();
		String value = setting.getValue();

		if (value.length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "'" + name + "', '" + value + "'"));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, name));

		if (name.equals("enabled") || name.equals("enable"))
		{
			// turn on touch events 
			enableOnClickEvents(true);
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Stylus Enabled"));
		}
		else if (name.equals("disabled") || name.equals("disable"))
		{
			// turn off touch events 
			enableOnClickEvents(false);
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Stylus Disabled"));
		}
	}

	/**
	 * Method to disable the touch screen , it works by consuming WebView touch events
	 */
	public void enableOnClickEvents(final boolean isEnabled)
	{
		Common.getWebView().setOnTouchListener(new View.OnTouchListener() {
			
			@Override
			public boolean onTouch(View arg0, MotionEvent arg1)
			{
				return !isEnabled;
			}
		});
		ViewGroup mainView = (ViewGroup)Common.getMainView();
		for (int i=0; i<mainView.getChildCount(); i++)
		{
			if (mainView.getChildAt(i) instanceof AbsoluteLayout)
			{
				ViewGroup buttonContainer = (ViewGroup)mainView.getChildAt(i);
				for (int j=0; j<buttonContainer.getChildCount(); j++)
				{
					View childView = buttonContainer.getChildAt(j);
					if ( (childView instanceof Button) || //button case
						 (childView instanceof AutoCompleteTextView) ) //address bar case
					{
						childView.setClickable(isEnabled); //set all the (debug) buttons or the address bar
					}
				}
			}
		}
	}
}