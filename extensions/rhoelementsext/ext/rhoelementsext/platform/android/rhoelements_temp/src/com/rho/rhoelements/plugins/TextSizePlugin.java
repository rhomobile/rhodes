package com.rho.rhoelements.plugins;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.IRhoWebView;


/**
 * @author hpkx48
 * TextSizePlugin provides the ability to adjust the text size on the webview 
 */
public class TextSizePlugin extends Plugin
{	
	public TextSizePlugin()
	{
	    CallbackHandlerObj = new CallbackHandler(this);
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null)); 
	}

	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version ("TextSize");
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
	//TODO: Fix the implementation of this plugin
	public void onSetting(PluginSetting setting)
	{
	    if (setting.getValue().length() > 0)
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
	    else
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));
	    IRhoWebView.TextZoom textZoom = IRhoWebView.TextZoom.NORMAL; //Default
	    if (setting.getName().equalsIgnoreCase("LARGEST"))
	    {
	    	textZoom = IRhoWebView.TextZoom.LARGEST;
	    }
	    else if (setting.getName().equalsIgnoreCase("LARGER"))
	    {
	    	textZoom = IRhoWebView.TextZoom.LARGER;
	    }
	    else if (setting.getName().equalsIgnoreCase("MEDIUM"))
	    {
	    	textZoom = IRhoWebView.TextZoom.NORMAL;   
	    }
	    else if (setting.getName().equalsIgnoreCase("SMALLER"))
	    {
	    	textZoom = IRhoWebView.TextZoom.SMALLER;
	    }
	    else if (setting.getName().equalsIgnoreCase("SMALLEST"))
	    {
	    	textZoom = IRhoWebView.TextZoom.SMALLEST;
	    }
	    else
	    {
		// default
	    	Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, " Invalid value passed to TextSize Plugin=" + setting.getName() + " taking no action." ));
	    	return;
	    }
	    //Common.elementsCore.getWebViewInstance().setTextZoom(scale);
	    Common.elementsCore.getWebViewInstance().setTextZoom(textZoom);
	}
	
	
}
	
	
	

