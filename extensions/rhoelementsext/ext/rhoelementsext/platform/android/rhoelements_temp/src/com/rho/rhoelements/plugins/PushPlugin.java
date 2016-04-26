package com.rho.rhoelements.plugins;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LocalWebServer;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;

public class PushPlugin extends Plugin {

	private int port = 8081; //default Value
	private String passKey = null;
	private String responsePage = null;  
	private String path = "";	
	private LocalWebServer webServer;
	private Boolean webServerStarted = false;
	private String pushUrl = null;	
	
	public String getResponsePage()
	{
		return responsePage;
	}
	
	public PushPlugin()
	{	
	}
	
	@Override
	public void onSetting(PluginSetting setting) {
		
		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));

		if (setting.getName().equalsIgnoreCase("port"))
		{
			if (webServerStarted)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot set the port while the web server is running"));
			}
			else
			{
				try
				{
					port = Integer.parseInt(setting.getValue());
					if((port < 1025) || (port > 65535))
					{
						port = 8081; //Default
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Setting default port 8081"));
					}
				}
				catch (Exception e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid port '" + setting.getValue() + "'"));
				}
			}
		}
		else if (setting.getName().equalsIgnoreCase("passkey"))
		{
			passKey = setting.getValue();
		}
		else if (setting.getName().equalsIgnoreCase("response"))
		{
			if (!setting.getValue().contains("/mnt/sdcard")) 
				responsePage = "/mnt/sdcard" + setting.getValue();
			else
				responsePage = setting.getValue(); //Assume that the user knows to specify SD CARD path
		}
		else if (setting.getName().equalsIgnoreCase("path"))
		{
			path = setting.getValue();
		}		
		else if (setting.getName().equalsIgnoreCase("unattended"))
		{
			String value = setting.getValue();	
			if (value.equalsIgnoreCase("Enabled"))
			{
				Common.elementsCore.setWifiLock(true);
			}
			else if (value.equalsIgnoreCase("Disabled"))
			{
				Common.elementsCore.setWifiLock(false);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised parameter " + value));
			}
		}
		else if (setting.getName().equalsIgnoreCase("start"))
		{
			if ((port > 1024) && (port < 65536))
			{
				startWebServer();
			}
		}
		else if (setting.getName().equalsIgnoreCase("stop"))
		{
			stopWebServer();
		}
		else if (setting.getName().equalsIgnoreCase("detected"))
		{
			pushUrl = setting.getValue(); 
		}	
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unknown setting '" + setting.getName() + "'"));
		}

	}

	@Override
	public void onShutdown() {	
		
		stopWebServer();
	}
	
	/** Starts the Local web server
	 * This uses the LocalWebServer class 
	 */
	public void startWebServer()
	{
	    /*	if (webServer == null)
		{
			webServer = new LocalWebServer(port, path, true, true);
		}
		
	    */	
		//If not started
		if (!webServerStarted)
		{
			webServer = new LocalWebServer(port, path, true, true);
			try {				
				webServer.start();	
				webServerStarted = true;
			} catch (Exception e) {				
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Unable to start the webserver"));
			}
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Server already running!"));
		}
	}
	
	/** Stops the Jetty web server
	 * This Web server serves both "Config>WebServer" & the "Push plugin" functionality
	 */
	public void stopWebServer()
	{
		if (webServerStarted)
		{			
			try {
				webServer.stop();				
				webServerStarted = false;
				webServer=null;
			} catch (Exception e) {				
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Unable to stop the webserver"));
			}
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Server not yet running!"));
		}
	}


	/**
	 * Handlers that handles the HTTP GET & POST requests
	 * 
	 */
	public boolean handle(String params) 
    {
		Boolean isPassKey = false;	
		Boolean isValidResponse = false;
		
		int i=0;
		int paramsCount = 0;		
		
		if (params == null)
			return false;
		
		String[] paramsSplit = params.split("\\?", -1);
		
		if (paramsSplit == null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "The URL doesn't have any arguments"));	
			return false;
		}		
		
	    // Check for path
		// This is always the first parameter
		if (path != "")
		{
			String pathInfo = paramsSplit[0];
			if (!pathInfo.equalsIgnoreCase(path))
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "The path should include the \"path\" value"));					
				return false;
			}
		}
		
		String arguments = null;
		if (paramsSplit.length == 1)
		{
			//No path specified
			arguments = paramsSplit[0];			
		}
		else if (paramsSplit.length == 2)
		{
			arguments = paramsSplit[1];
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "The URL contains multiple & separators"));	
			return false;
		}
		
		String[] argsSplit = arguments.split("&", -1);
		
		if (argsSplit != null)
			paramsCount = argsSplit.length;
		
		String[] Names  = new String[paramsCount];
		String[] Values = new String[paramsCount];	
		
		//Parse the remaining arguments
		for (String param : argsSplit)
	    {
	    	isValidResponse = true; // To filter out empty requests
	    	String[] pair = param.split("=");	
	    	if (pair.length != 2)
	    	{
	    		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Incorrect URL format for pairs"));
	    		return false;
	    	}
	    	String paramName = null;
	    	String paramValue = null;
			try
			{
				paramName  = URLDecoder.decode(pair[0], "UTF-8");
				paramValue = URLDecoder.decode(pair[1], "UTF-8");
			}
			catch (UnsupportedEncodingException e)
			{				
				e.printStackTrace();
			}  	    	
	    	
			// Check for passKey
	    	if (paramName.equalsIgnoreCase("passkey"))
	    	{
	    		//Special case for passkey
		    	if (passKey != null)
		    	{
		    		if (paramValue.equals/*IgnoreCase*/(passKey))
		    		{	
		    			isPassKey = true;
		    		}
		    		else
		    		{
		    			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "The passKey specified is incorrect"));
		    			return false;
		    		}
		    	}
		    	continue; //Do not add the pass key
	    	}	   
	    	
	    	if (paramName != null && paramValue != null)
	    	{		    		
	    		Names[i]  = paramName;
	    		Values[i] = paramValue;		    		
	    		i++;
	    	}		    	
	    }
	    
	    if (!isValidResponse)
	    {
	    	return false; //Ignore empty/other requests
	    }
	    
	    if ((passKey != null) && (!isPassKey))
	    {		    	
	    	Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "The path should include the passKey"));
	    	return false;	    	
	    }  
	    
	    if(pushUrl != null)
		{
			try {
				navigate(pushUrl, Names, Values);
			} catch (NavigateException e) {					
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
			}
		}	
	    
	    return true;
    }
	
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
	    return new Version ("Push"); //$NON-NLS-1$
	}
}
