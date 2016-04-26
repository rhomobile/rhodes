package com.rho.rhoelements;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.UUID;

import android.app.Activity;
import android.app.Instrumentation;
import android.content.Context;
import android.media.MediaPlayer;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Environment;
import android.util.Log;
import android.view.View;

import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author Geoff Day (XFH386, Initial Creation)
 *
 */
public class Common
{
	/*
	 *  KEYCODE_TRIGGER is the value of the trigger key code on the Nimbus device.
	 *  This constant should live in RhoElements until platform will provide such constant.
	 *  The key code constant should be device independent as long as the platform will map the trigger
	 *  buttons scan codes to this key code.
	 */
	public static final int KEYCODE_TRIGGER_1 = 102;
	public static final int KEYCODE_TRIGGER_2 = 103;
	
	public static final Boolean logFlow = true;
	public static final Boolean logLog = true;
	public static final int MAX_URL = Integer.MAX_VALUE;
	
	public static String packageName = "com.rho.rhoelements";
	static String dataPath;
	public static String internalDataPath;
	public static final String getDataPath() {return dataPath;}
	
	// UUID initialised in static class constructor
	static String uuid = "";
	public static boolean isRhodes;
	public static String getUUID() {return uuid;}
	static public String getOEMInfo() {return Build.PRODUCT;}
	
	public static final String APP_NAME = "RhoElements";
	public static final String LOG_TAG = APP_NAME;
	public static void log(String text)
	{
		android.util.Log.d(LOG_TAG, "<" + String.valueOf(Thread.currentThread().getId()) + "> " + text);
	}
	
	public static Activity mainActivity;
	public static ElementsCore elementsCore;
	public static View mainView;
	public static Config config;
	public static Logger logger;
	public static PluginManager pluginManager;
	public static MetaReceiver metaReceiver;
	public static MediaPlayer mediaPlayer;
	
	
	public static  int ZOOM_IN_KEY = 0;
	public static  int ZOOM_OUT_KEY = 0;
        public static boolean ZOOM_KEY_ENABLE = false;
	
	public static IElementsCore getElementsCore() {return elementsCore;}
	
	public static void init(String packageName)
	{
		Common.packageName = packageName;
		internalDataPath = "/Android/data/" + packageName;
		dataPath = Environment.getExternalStorageDirectory().getAbsolutePath() + internalDataPath;
	}
	
	
	/**
	 * This method is used only for non-Symbol devices as the UUID needs to be computed by other parameters.
	 * @return 32-byte long UUID
	 */
	private static String computeUUID()
	{
	    String res="";
	    WifiManager wifi = (WifiManager) Common.mainActivity.getSystemService(Context.WIFI_SERVICE);
	    // Get WiFi status
	    WifiInfo wifiInfo = wifi.getConnectionInfo();
	    String macAddress = wifiInfo.getMacAddress();
	    macAddress = macAddress.replaceAll(":", "");
	    UUID localUuid = UUID.nameUUIDFromBytes(macAddress.getBytes());
	    res = localUuid.toString().replaceAll("-", "");
	    return res.toUpperCase();
	}
	
	public static void fetchUUID()
	{
		String fileUuid = "";
		BufferedReader reader = null;
		// Get serial number from UUID file built into image
		try
		{
		    if (isEmdkDevice(Build.MODEL))
		    {
		        reader = new BufferedReader(new FileReader("/sys/hardware_id/uuid"));
		        fileUuid = reader.readLine();
		        uuid = fileUuid.toUpperCase();
		    }
		    else
		    {
		        uuid = computeUUID();
		    }
		}
		catch (Exception e)
		{
		    uuid = e.getMessage();
            try {
                if (reader != null) {
                    reader.close();
                }
            } catch (IOException e1) {}
		}
	}
	
	public static ArrayList<Version> getVersions()
	{
		ArrayList<Version> versions = new ArrayList<Version>();
		
		// Add version for core
		// By using this Version constructor we get the project-wide version numbers and build time
		versions.add(new Version("Core"));
		
		// Add versions for the plugins
		versions.addAll(pluginManager.getVersions());
		
		return versions;
	}
	
	/**
	 * Returns a local File object constructed by any legal URI (i.e. Absolute, relative etc).
	 * Converts Windows style paths (\) to valid URI (/) for backwards compatibility.
	 * DOES NOT check if the file exists. Do that yourself.
	 * If the path is relative, it will use Android's external storage directory as a root
	 * for the relative URI. Normally this is /mnt/sdcard/ or /sdcard/.
	 * 
	 * @param fileName The location of the file to parse.
	 * @return The constructed file.
	 * @throws URISyntaxException if the URI has bad syntax.
	 * @throws InvalidParameterException if the URI describes a remote file or if the argument
	 * is null or empty
	 * @author Ben Kennedy (NCVT73)
	 */
	public static File getLocalFile(String fileName) throws URISyntaxException, InvalidParameterException 
	{
		URI resultingURI = parseAndroidURI(fileName);
		if(!resultingURI.getScheme().equalsIgnoreCase("file"))
		{
			throw new InvalidParameterException("File can not be stored in a remote location.");
		}
		return new File(resultingURI);
	}
	
	/**
	 * Returns a local URI object constructed by any legal URI (i.e. Absolute, relative etc).
	 * Converts Windows style paths (\) to valid URI (/) for backwards compatibility.
	 * DOES NOT check if the file exists. Do that yourself.
	 * If the path is relative, it will use Android's external storage directory as a root
	 * for the relative URI. Normally this is /mnt/sdcard/ or /sdcard/.
	 * 
	 * @param fileName The location of the file to parse.
	 * @return The constructed URI.
	 * @throws URISyntaxException if the URI has bad syntax.
	 * @throws InvalidParameterException if the argument is null or empty
	 * @author Ben Kennedy (NCVT73)
	 */
	public static URI parseAndroidURI(String fileName) throws URISyntaxException, InvalidParameterException
	{
		return parseAndroidURI(fileName, false, null );
	}
  
  
  public static URI parseAndroidURI(String fileName, boolean isInternalCall) throws URISyntaxException, InvalidParameterException
	{
		return parseAndroidURI(fileName, isInternalCall, null );
	}
  
	
	/**
	 * Returns a local URI object constructed by any legal URI (i.e. Absolute, relative etc).
	 * Converts Windows style paths (\) to valid URI (/) for backwards compatibility.
	 * DOES NOT check if the file exists. Do that yourself.
	 * If the path is relative, it will use Android's external storage directory as a root
	 * for the relative URI. Normally this is /mnt/sdcard/ or /sdcard/.
	 * 
	 * @param fileName The location of the file to parse.
	 * @param isInternalCall signifies if this has been called by an internal method that has added the special character
	 * ("//") to show that this URI is not on the sdcard.
	 * @return The constructed URI.
	 * @throws URISyntaxException if the URI has bad syntax.
	 * @throws InvalidParameterException if the argument is null or empty
	 * @author Ben Kennedy (NCVT73)
	 */
	public static URI parseAndroidURI(String fileName, boolean isInternalCall, String origin) throws URISyntaxException, InvalidParameterException
	{
		URI resultingURI;
		
		if(fileName == null || fileName.equals(""))
		{
			throw new InvalidParameterException("URL cannot be empty");
		}		
		//below line added to handle space in the Config.xml file path
		//fileName = fileName.replace(" ", "%20");
		
		String deWindowedURLStr = fileName.replaceAll("\\\\", "/");
		
		if(deWindowedURLStr.toLowerCase().startsWith("file://") && !deWindowedURLStr.toLowerCase().startsWith("file:///"))
		{
			deWindowedURLStr = "file:///" + deWindowedURLStr.substring(7);
		}
		
		resultingURI = new URI(deWindowedURLStr);
		String sdcardDir = Environment.getExternalStorageDirectory().getPath() + "/";
		String scheme = resultingURI.getScheme();
		if(scheme != null)
		{
			scheme = scheme.toLowerCase();
		}
		
		if(scheme == null)
		{
			// If this method has been called with an URI that has been generated internally (i.e. not through user input)
			// then the beginning "//" signifies that this URL is referencing from the ROOT OF THE SYSTEM, not the sdcard.
			if(deWindowedURLStr.startsWith("//") & isInternalCall)
			{
				return new URI("file://" + deWindowedURLStr.substring(1));
			}
			if(deWindowedURLStr.startsWith("/storage") || deWindowedURLStr.startsWith("/mnt") || deWindowedURLStr.startsWith("/sdcard")) {
	            com.rhomobile.rhodes.Logger.I("RhoElements", "path: " + deWindowedURLStr + " >>>>>>>>>>>>>>>>>>>>>>>>>>>");
			    return new URI("file://" + deWindowedURLStr);
			}
			if(deWindowedURLStr.startsWith("/"))
			{
      
        if (origin != null) {
          URI originURI = new URI(origin);
          String originScheme = originURI.getScheme();
          if ( (originScheme != null) && !originScheme.equals("file") ) {
            return resultingURI;
          }
        }
      
				deWindowedURLStr = deWindowedURLStr.substring(1);
			}
			return new URI("file://" + sdcardDir + deWindowedURLStr);
		}
		
		if(!scheme.equals("file"))
		{
			//Not applicable for parse
			return resultingURI;
		}
		
		String path = resultingURI.getPath();
		
		// If this method has been called with an URI that has been generated internally (i.e. not through user input)
		// then the beginning "//" signifies that this URL is referencing from the ROOT OF THE SYSTEM, not the sdcard.
		if(path.startsWith("//") && isInternalCall)
		{
			return new URI("file:/" + path); 
		}
		if(path.startsWith(sdcardDir))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Encountered user reference to device root (" + sdcardDir + "). Referencing device root in URLs is not recommended. " + APP_NAME + " treats the root of the sdcard as device root."));
			return resultingURI;
		}
		
		if(path.startsWith(sdcardDir.substring(1)))
		{
			deWindowedURLStr = "/" + deWindowedURLStr;
			resultingURI = new URI("file://" + deWindowedURLStr);
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Encountered user reference to device root (" + sdcardDir + "). Referencing device root in URLs is not recommended. " + APP_NAME + " treats the root of the sdcard as device root."));
			return resultingURI;
		}

		//Doesnt start with file:///mnt/sdcard
		String uriString = "file://" + sdcardDir;
		String path2 = resultingURI.getPath();
		if(path2.charAt(0) == '/')
		{
			uriString = uriString + path2.substring(1);
		}
		else
		{
			uriString = uriString + path2;
		}
		resultingURI = new URI(uriString);
		return resultingURI;
	}
	
	public static void injectKeyEvents(String injection)
	{
		KeyEventGenerator generator = new KeyEventGenerator(injection);
		generator.start();
	}
	
	/**
	 * 
	 */
	/*static public void goToHomePage()
	{
		//TODO: This doesn't load the home page correctly, path problem
		try
		{
			Common.mainActivity.getWebViewInstance().loadUrl(Common.parseAndroidURI(
					Common.config.getSetting(Config.SETTING_START_PAGE)).toString());
		} 
		catch (InvalidParameterException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, e.getMessage()));
		} 
		catch (URISyntaxException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, e.getMessage()));
		}
		catch (NullPointerException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, e.getMessage()));
		}
	}*/
	private static class KeyEventGenerator extends Thread
	{
		private String injection;
		
		public KeyEventGenerator(String injection)
		{
			this.injection = injection;
		}
		
		@Override
		public void run()
		{
			Instrumentation ins = new Instrumentation();
			ins.sendStringSync(injection);
		}
		
	}
	
	/**
	 * Adds the http protocol to an input URL String, if the URL in question has no protocol.
	 * Ignores and returns the original URL String if the protocol has been set.
	 * @return the input url string with the protocol set to HTTP, if the URL did not have a protocol originally
	 * @throws MalformedURLException if the new URL is malformed
	 */
	public static String addDefaultProtocol(String inputUrl) throws MalformedURLException
	{
		try
		{
			URI uri  = new URI(inputUrl);
			String scheme = uri.getScheme();
			if(scheme == null || scheme.equals(""))
			{
				return "http://" + inputUrl;
			}
			// else
			return inputUrl;
		}
		catch (URISyntaxException e)
		{
			throw new MalformedURLException();
		}
	}
	
	public static View getWebView()
	{
		//return mainActivity.findViewById(RhoExtManager.getResourceId("id","elements_webview"));
	    return RhoExtManager.getInstance().getWebView().getView();
	}
	
	public static View getMainView()
	{
		if(isRhodes)
			return mainActivity.getCurrentFocus().getRootView();
		else
			return mainActivity.findViewById(RhoExtManager.getResourceId("id","composed_layout"));
	}
	
	/**
	 * Predicate to understand whether the device supports EMDK or not
	 * @return true if the device supports EMDK, false otherwise.
	 */
public static boolean isEmdkDevice()
	{
	    boolean res = false;
	    try {
			Class.forName("com.symbol.scanning.Scanner", false, Common.mainActivity.getClassLoader());
			Log.d("com.rho.rhoelements.Common","com.symbol.scanning.Scanner loaded");
			res=true;
	     } 
	    catch (ClassNotFoundException e1) 
	    {
	
		e1.printStackTrace();
											
	    }
	    return res;
	}
	public static boolean isRebradnedDevice()
	{
	    boolean res = false;
	    try
	    {
		
	    	Class.forName("com.symbol.scanning.Scanner", false, Common.mainActivity.getClassLoader());
	    	res = true;
	    } 
	    catch (ClassNotFoundException e)
	    {
	    	
	    	res = false;
	    	
		}
	    return res;
	}
	
	public static boolean isEmdkDevice(String modelName)
	{
		 boolean res = false;
		 try {
 			Class.forName("com.symbol.scanning.Scanner", false, Common.mainActivity.getClassLoader());
 			res=true;
 		} 
 	catch (ClassNotFoundException e1) 
 	    {
		
 			e1.printStackTrace();
 											
 		}
 	 if(modelName.compareToIgnoreCase("MC18N0") == 0){
				res = true;
			}
		
		return res;
	}
	public static boolean isEt1()
	{
	    return (Build.MODEL.compareToIgnoreCase("ET1") == 0);
	}
	
//	/**
//	 * Adds the http protocol to an input URL, if the URL in question has no protocol.
//	 * Ignores and returns the original URL if the protocol has been set.
//	 * @return the input url with the protocol set to HTTP, if the URL did not have a protocol originally
//	 * @throws MalformedURLException if the new URL is malformed (should never happen)
//	 */
//	public static URL addDefaultProtocol(URL inputUrl) throws MalformedURLException
//	{
//		if(inputUrl.getProtocol() == null || inputUrl.getProtocol().equals(""))
//		{
//			return new URL("http", inputUrl.getHost(), inputUrl.getPort(), inputUrl.getFile());
//		}
//		return inputUrl;
//	}
}
