package com.rho.rhoelements;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import com.rho.rhoelements.plugins.Plugin;

/**
 * Maintains map of instantiated plugins
 * 
 * @author XFH386, NCVT73 (Added convenience method: applySettings)
 */
public class PluginManager
{
	public static final String	PLUGIN_HOURGLASS	= "hourglass";
	public static final String RHOELEMENTS_PACKAGE_NAME = "com.rho.rhoelements";

	// Map of plugin name as per http-equiv to instance of implementation class
	private HashMap<String, Plugin> PluginMap;

	// Map of plugin name as per http-equiv to name of implementation class
	private HashMap<String, String> PluginNamesMap;

	public PluginManager()
	{
		PluginMap = new HashMap<String, Plugin>();
		PluginNamesMap = new HashMap<String, String>();
	}

	/**
	 * Adds a module name/class name pair.
	 * 
	 * @param module_name Module name as used by EMML
	 * @param class_name Class name of corresponding object to construct
	 */
	public void addName(String module_name, String class_name)
	{
		PluginNamesMap.put(module_name.toLowerCase(), class_name);
	}

	/**
	 * Adds a module name/plugin object pair ONLY EXISTS FOR LOADING THE SCANNER MODULE WHICH MUST BE DONE AT START UP FROM THE MAIN ACTIVITY
	 * 
	 * @param module_name Module name as used by EMML
	 * @param plugin Plugin object implementing this module
	 */
	public void addPlugin(String module_name, Plugin plugin)
	{
		PluginMap.put(module_name.toLowerCase(), plugin);
	}

	public Plugin loadPlugin(String module_name)
	{
		String class_name = "";

		try
		{
			// Get the class name from the module name
			class_name = PluginNamesMap.get(module_name.toLowerCase());
			if (class_name == null)
				throw new Exception("Module name not found '" + module_name + "'");

			// Instantiate the class
			@SuppressWarnings("unchecked")
			Class<Plugin> plugin_class = (Class<Plugin>) Class.forName(RHOELEMENTS_PACKAGE_NAME + ".plugins." + class_name);
			Plugin plugin = plugin_class.newInstance();

			// Add to plugin map
			PluginMap.put(module_name.toLowerCase(), plugin);

			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, module_name));

			return plugin;
		}
		catch (ClassNotFoundException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Plugin class not found '" + class_name + "'"));
		}
		catch (Exception e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
		}

		return null;
	}

	/** Finds plugin in map from lowercase name */
	public Plugin getPlugin(String module_name)
	{
		return PluginMap.get(module_name.toLowerCase());
	}

	public void applySetting(PluginSetting setting)
	{
		// Get plugin from map of loaded plugins
		Plugin plugin = getPlugin(setting.getModule());
		if (plugin == null)
		{
			// Plugin not loaded so load it now
			plugin = loadPlugin(setting.getModule());
			if (plugin == null)
			{
				// Plugin can't be loaded so log error and quit
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Cannot load module '" + setting.getModule() + "'"));
				return;
			}
		}

		// Pass the setting to the plugin
		plugin.onSetting(setting);
	}

	/**
	 * Convenience method to apply a list of settings.
	 * 
	 * @param settings the list of plugin settings to apply
	 */
	public void applySettings(List<PluginSetting> settings)
	{
		for (PluginSetting setting : settings)
		{
			applySetting(setting);
		}
	}

	/** Calls onShutdown() for all registered plugins */
	public void shutdown()
	{
		for (Plugin plugin : PluginMap.values())
		{
			plugin.onShutdown();
		}
	}

	public void pageStarted(String url)
	{
		for (Plugin plugin : PluginMap.values())
		{
			plugin.onPageStarted(url);
		}
	}

	public void pageFinished(String url)
	{
		for (Plugin plugin : PluginMap.values())
		{
			plugin.onPageFinished(url);
		}
	}
	
	/**
	 * When the app comes to foreground let all the plugins know
	 * about the event so that they restore the saved attributes
	 * @param url
	 */
	public void onForeground(String url)
	{
		if (!Common.elementsCore.bLaunchingAppHasFocus)
		{
			Common.elementsCore.bLaunchingAppHasFocus = true;
			
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,"Application foreground event received by the plugin manager"));
			for (Plugin plugin : PluginMap.values())
			{
				plugin.onForeground(url);
			}			
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,"Unexpected application foreground event received by the plugin manager"));
		}
	}

	/**
	 * When the app goes background let all the plugins know
	 * about the event so that they restore the saved attributes
	 * @param url
	 */
	public void onBackground(String url)
	{
		if (Common.elementsCore.bLaunchingAppHasFocus)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,"Application background event received by the plugin manager"));
			for (Plugin plugin : PluginMap.values())
			{
				plugin.onBackground(url);
			}		
			
			Common.elementsCore.bLaunchingAppHasFocus = false;
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,"Unexpected application background event received by the plugin manager"));
		}
	}

	/**
	 * Request Version object from each plugin in name map. Static functions are used and reflection so plugin doesn't have to be instantiated.
	 * 
	 * @return List of Version objects
	 */
	public ArrayList<Version> getVersions()
	{
		ArrayList<Version> versions = new ArrayList<Version>();

		// For each plugin name...
		for (String class_name : PluginNamesMap.values())
		{
			try
			{
				// Call static getVersion() using reflection
				@SuppressWarnings("unchecked")
				Class<Plugin> plugin_class = (Class<Plugin>) Class.forName(RHOELEMENTS_PACKAGE_NAME + ".plugins." + class_name);
				Method version_method = plugin_class.getMethod("getVersion", new Class[] {});
				versions.add ((Version)version_method.invoke(null, new Object[] {}));
			}
			catch (Exception e)
			{
				// If there's any problem then use a general UnknownVersion
				versions.add(new UnknownVersion(class_name));
			}
		}

		return versions;
	}

	public void pause()
	{
		for (Plugin plugin : PluginMap.values())
		{
			plugin.onPause();
		}
	}

	public void resume()
	{
		for (Plugin plugin : PluginMap.values())
		{
			plugin.onResume();
		}
	}
}
