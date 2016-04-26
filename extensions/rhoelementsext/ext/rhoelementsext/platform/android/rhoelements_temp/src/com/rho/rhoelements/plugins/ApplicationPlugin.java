package com.rho.rhoelements.plugins;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

public class ApplicationPlugin extends Plugin
{
    String versionEventURL;
    private static final String[] VERSION_NAMES = new String[] {
	"productVersion",
	"hotFixes",
	"componentVersions" };
    
    String applicationEventURL;
    private static final String[] APPLICATION_NAMES = new String[] {
	"applicationState" };
    public enum ApplicationEvents {
	MINIMIZED,
	RESTORED
    }
    private static final String MINIMIZED = "Minimized";
    private static final String RESTORED = "Restored";
    public static final String APPLICATION_ACTION = "com.rho.rhoelements.APPLICATION_ACTION";
    public static final String APPLICATION_METHOD = "APPLICATION_METHOD";
    
    private final BroadcastReceiver mApplicationReceiver = new BroadcastReceiver()
    {  
        @Override
        public void onReceive(Context context, Intent intent)
        {
            int event = intent.getIntExtra(APPLICATION_METHOD, -1);
            switch (event)
            {
                case 0:
                {
                	fireApplicationEvent(ApplicationEvents.MINIMIZED);
                	break;
                }
                case 1:
                {
                	fireApplicationEvent(ApplicationEvents.RESTORED);
                	break;
                }
                default:
        	{
        	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, event + " is not supported")); 
        	}
            }
        }
    };
	
    public ApplicationPlugin()
    {
	super();
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	versionEventURL = null;
	applicationEventURL = null;
    }

    @Override
    public void onShutdown()
    {
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	reset();
    }

    /**
     * @return The version of this plugin being built
     */
    public static Version getVersion()
    {
	return new Version ("Application");
    }

    @Override
    public void onPageStarted(String url)
    {
	reset();
    }
	
    @Override
    public void onSetting(PluginSetting setting)
    {
	if (setting.getValue().length() > 0)
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", setting.getName(), setting.getValue())));
	else
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, setting.getName()));

	if (setting.getName().equalsIgnoreCase("versionevent"))
	{
	    // Store URL
	    versionEventURL = setting.getValue();
	}
	else if (setting.getName().equalsIgnoreCase("applicationevent"))
	{
	    // Store URL
	    applicationEventURL = setting.getValue();
	    Common.mainActivity.registerReceiver(mApplicationReceiver, new IntentFilter(APPLICATION_ACTION));
	}
	else if (setting.getName().equalsIgnoreCase("getversion"))
	{
	    // Fire event with version info
	    fireVersionEvent();
	}
	else if (setting.getName().equalsIgnoreCase("quit"))
	{
	    Common.elementsCore.quit();
	}
	else if (setting.getName().equalsIgnoreCase("minimize"))
	{
	    Common.mainActivity.moveTaskToBack(true);
	    fireApplicationEvent(ApplicationEvents.MINIMIZED);
	}
	else if (setting.getName().equalsIgnoreCase("restore"))
	{
	    RhodesService.bringToFront();
	    //To restricting from firing an event when restore method is called as it is already fired when activity is brought to foreground.
	    //fireApplicationEvent(ApplicationEvents.RESTORED);
	}
	else
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unknown setting '" + setting.getName() + "'"));
    }
    
    
    private void reset()
    {
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));    
	// Wipe the event URL's
	versionEventURL = null;
	applicationEventURL = null;
	try
	{
	    Common.mainActivity.unregisterReceiver(mApplicationReceiver);
	}
	catch (IllegalArgumentException e)
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Receiver was not registered"));
	}
    }
	
    /**
     * 
     */
    private void fireApplicationEvent(ApplicationEvents event)
    {
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
	try
	{
        	switch (event.ordinal())
        	{
                	case 0:
                	{
                	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "MINIMIZED case"));
                	    String[] value = {MINIMIZED};
                	    navigate(applicationEventURL, APPLICATION_NAMES, value);
                	    break;
                	}
                	case 1:
                	{
                	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "RESTORED case"));
                	    String[] value = {RESTORED};
                	    navigate(applicationEventURL, APPLICATION_NAMES, value);
                	    break;
                	}
                	default:
                	{
                	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, event.ordinal() + " is not supported")); 
                	}
        	}
	}
	catch (NavigateException e)
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
	}
    }
	
    /**
     * Gets version info for the application and returns it via a navigation event
     */
    private void fireVersionEvent()
    {
	// Anything to do?
	if (versionEventURL == null)
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Event URL has not been set"));
	    return;
	}

	// Get the core version to use as overall product version and hotfix
	Version product_version = Common.elementsCore.getVersion();

	// Get plugin versions
//	ArrayList<Version> plugin_versions = Common.pluginManager.getVersions();

	// %1 = product version string without hotfix
	// %2 = array of hotfix numbers - there will be at most one for Android
	// %3 = array of plugin name and version strings
		
	// Create a JSONArray to hold all the version info
	JSONArray j = new JSONArray();
		
	// Add the product version
	j.put(product_version.getString());
	
	// build list of hotfixes - use a HashSet because it ignores duplicates 
	HashSet<Integer> hotfixSet = new HashSet<Integer>();

	// build a list of JSON objects holding the component/version name/values
	JSONArray componentVersions = new JSONArray();

//	for (Version version : plugin_versions)
//	{
	    // For every plugin...
	    // add the hotfix to the hotfix set
//	    hotfixSet.add(version.getHotfix());
	    // create a JSON component object
	    JSONObject component = new JSONObject();
	    try
	    {
				component.put(Plugin.jsonObjNVPOrderString, "module,version");
//				// add the module name/value pair
				component.put("module", "");
				// add the version name/value pair
				component.put("version", "");
			}
			catch (JSONException e)
			{
				e.printStackTrace();
			}

			componentVersions.put(component);
//		}
		
		// remove 0 from the set of hotfixes
//		hotfixSet.remove(0);
		// Add the sorted hotfix list to the JSON Array
		ArrayList<Integer> hotfixList = new ArrayList<Integer>(hotfixSet);
//		Collections.sort(hotfixList);
		j.put(new JSONArray(hotfixList));

		// Add the component versions array to the the JSON array
		j.put(componentVersions);
		
		try
		{
			navigate(versionEventURL, VERSION_NAMES, j);
		}
		catch (NavigateException e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
		}

	}

}
