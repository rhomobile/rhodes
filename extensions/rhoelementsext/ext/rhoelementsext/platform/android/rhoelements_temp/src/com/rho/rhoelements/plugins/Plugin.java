package com.rho.rhoelements.plugins;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.util.ArrayList;
import java.util.Arrays;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Base64;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.Navigate;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesService;

import java.net.URL;
import java.net.MalformedURLException;
/**
 * @author XFH386
 *
 */
public abstract class Plugin/* extends BroadcastReceiver*/
{
	public static CallbackHandler CallbackHandlerObj;
	public static final String jsonObjNVPOrderString = "NVPOrder"; 

	/** Method called by core when a meta tag is detected for this plugin */
	public abstract void onSetting(PluginSetting setting);

	/** Method called by core when the plugin is shut down */
	public abstract void onShutdown();
	
	/** Method called when core is paused - so plugins can suspend devices if necessary */
	public void onPause() {}

	/** Method called when core is resumed - so any suspended devices can be re-opened */
	public void onResume() {}
		
	/** Method called when core detects the start of a page navigation */
	public void onPageStarted(String url) {}
	
	/** Method called when core detects the completion of a successful page navigation */
	public void onPageFinished(String url) {}
	
	/** Method called when the App gets foreground focus event */
	public void onForeground(String url) {}
	
	/** Method called when the App gets Background focus event */
	public void onBackground(String url) {}
	
	/**
	 * Instructs core to navigate via IElementsCore interface Substitutes supplied value for %s in url, if any
	 * 
	 * @param url
	 *            URL to navigate to (could be javascript:)
	 * @param sub
	 *            Value to substitute for %s in url
	 */
	/*protected void navigate (String url, String sub)
	{
		if (url == null || url == "")
			return;
		
		elementsCore.navigate(url.replaceFirst("%s", sub));
	}*/
	
    /** Instructs core to navigate via IElementsCore interface */
    protected void navigate(String url) {
        Logger.T("Plugin", "navigate: " + url);

        if (url.startsWith("url:")) {
            url = url.substring(4);
        }
        else {
            try {
              URL validating = new URL(url);
            } catch ( MalformedURLException e ) {
              url = "javascript:" + url;
            }
        }


        Common.getElementsCore().navigate(url);
    }

	protected void navigate(String url, String[] names, String[] values) throws NavigateException
	{
		/**
		 * This is to handle the case the url is a Ruby callback
		 */
		if ( (url == null) || (names == null) || (values == null) )
			return;
    
    /*one more ugly hack*/
    if (url.startsWith("url:/app")) {
      url = url.substring(4);
    }
    
		if (url.startsWith("/app"))
		{
			if ( (names.length <= 0) || (values.length <=0) || (names.length != values.length) )
				return;
      
			String jsonString = new String("{");
			for (int i=0; i<names.length; i++)
			{
				if (values[i] == null)
					return;
				jsonString += "\"" + names[i] + "\"" + ":";
				if ((values[i].startsWith("[")) && (values[i].endsWith("]")))
					jsonString += values[i];
				else
					jsonString += "\"" + values[i] + "\"";
				if (i < names.length - 1)
					jsonString += ",";
			}
			jsonString += "}";
			//Common.getElementsCore().navigate(url + jsonString);
			
			RhodesService.doRequestJson(url, jsonString, "", false);
		}
		else
			navigate(Navigate.embed(url, names, values));
	}

	protected void navigate(String url, String name, String value) throws NavigateException
	{
		navigate(url, new String[] { name }, new String[] { value });
	}

	protected void navigate(String url, String name, JSONArray j) throws NavigateException
	{
		if(url != null)
		{
			if ( (url.toLowerCase().contains("%json")) || (url.startsWith("/app")) )
			{
				navigate(url, name, j.toString());
			}
			else
			{
				navigate(url, name, jsonArrayToJavaScript(j));
			}
		}
		else
		{
			throw new NavigateException(11, "URL is null");
		}
	}

	protected void navigate(String url, String[] names, JSONArray j) throws NavigateException
	{
		if(url != null)
		{
			if ( (url.toLowerCase().contains("%json")) || (url.startsWith("/app")) )
			{
				// convert the JSONArray into an array of JSON strings
				ArrayList<String> namesList = new ArrayList<String>(Arrays.asList(names));
				String[] values = new String[names.length];
				for (int i=0; i<names.length; i++)
				{
					JSONArray a = j.optJSONArray(i);
					if (a != null)
					{
						values[i] = a.toString();
					}
					else
					{
						try
						{
							values[i] = j.toJSONObject(new JSONArray(namesList)).getString(names[i]);
						}
						catch (JSONException e)
						{
							throw new NavigateException(j.length(), "unknown JSONException");
						}
					}
				}				
				navigate(url, names, values);
			}
			else
			{
				navigate(url, names, jsonArrayToJavaScriptArray(j));
			}
		}
		else
		{
			throw new NavigateException(11, "URL is null");
		}
	}
	
	protected String jsonArrayToJavaScript(JSONArray j)
	{
		String javaScriptString = new String("");
		if (j != null)
		{
			javaScriptString += "new Array(";
			for (int i=0; i<j.length();i++)
			{
				if (i > 0) // if not the first element add a separator
				{
					javaScriptString += ",";
				}
				JSONArray array = j.optJSONArray(i);
				if (array != null) // if it's a nested array decode it
				{
					javaScriptString += jsonArrayToJavaScript(array);
				}
				else
				{
					JSONObject o = j.optJSONObject(i);
					if (o != null)
					{
						JSONArray names = o.names();
					
						// JSON objects have no defined order to their N.V.Pairs, so extract the order information from the object, and order the nvps
						try
						{
							JSONArray orderedNames = new JSONArray();
							if(o.has(Plugin.jsonObjNVPOrderString))
							{
								String[] nvpOrder = o.getString(Plugin.jsonObjNVPOrderString).split(",");
								for(int x = 0; x < nvpOrder.length; x++)
								{
									orderedNames.put(x, nvpOrder[x]);
								}
								javaScriptString += jsonArrayToJavaScript(o.toJSONArray(orderedNames));
							}
							else //No ordering information, so continue
							{
								javaScriptString += jsonArrayToJavaScript(o.toJSONArray(names));
							}
						}
						catch (JSONException e)
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "JSON parameter ordering failed."));
						}
					}
					else	// otherwise append it as a string
					{
						javaScriptString += "'" + j.optString(i, "") + "'";
					}
				}
			}
			javaScriptString += ")";
		}
		return javaScriptString;
	}

	protected String[] jsonArrayToJavaScriptArray(JSONArray j)
	{
		String[] jStringArray = new String[j.length()];
		
		for (int i=0; i<j.length(); i++)
		{
			JSONArray array = j.optJSONArray(i);
			if (array != null)
			{
				jStringArray[i] = jsonArrayToJavaScript(array);
			}
			else
			{
				jStringArray[i] = j.optString(i, "");
			}
		}
		
		return jStringArray;
	}
	
	/**
	 * 
	 * @param fileToEncode is the file containing the data to encode to Base64
	 * @return the file content encoded to Base64
	 */
	protected String encodeToBase64(File fileToEncode)
	{
	    String res = null;
	    try
	    {
		InputStream is = new BufferedInputStream(new FileInputStream(fileToEncode));
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		while (is.available() > 0)
		{
		    bos.write(is.read());
		}
		res = Base64.encodeToString(bos.toByteArray(), Base64.NO_WRAP);
	    }
	    catch (FileNotFoundException e)
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
	    } 
	    catch (IOException e)
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
	    }
	    return res;
	}


	//Stuff for turning Plugin into its own BroadcastReceiver//
//	private IntentFilter intentFilter;
//	protected Plugin(IElementsCore core, String[] intentList)
//	{
//	    this(core);
//	    intentFilter = new IntentFilter();
//	    for(String intent: intentList)
//	    {
//		intentFilter.addAction(intent);
//	    }
//	    Common.mainActivity.registerReceiver(this, intentFilter);
//	}
//	
//	public abstract void onReceive(Context context, Intent intent);
}
