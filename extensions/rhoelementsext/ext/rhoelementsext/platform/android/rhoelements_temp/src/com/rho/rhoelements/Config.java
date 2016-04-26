package com.rho.rhoelements;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.FileChannel;
import java.nio.channels.ReadableByteChannel;
import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import android.os.Environment;
import android.util.Xml;

import com.rhomobile.rhodes.extmanager.RhoExtManager;

// TODO - make access to map thread-safe

/**
 * @author Geoff Day (XFH386, Initial Creation)
 * @author Ben Kennedy (NCVT73, Added resource control to load())
 */
public class Config
{

	public static final String	SETTING_AUTOROTATE				= "autorotate";
	public static final String	SETTING_BADLINK_FILE			= "badlinkuri";		
	public static final String	SETTING_DEBUG_BUTTONS_ENABLED	= "debugbuttonsenabled";
	public static final String	SETTING_SETTINGS_BUTTON_ENABLED	= "settingsbuttonenabled";
	public static final String	SETTING_FULLSCREEN				= "fullscreen";
	public static final String	SETTING_LOG_ERROR				= "logerror";
	public static final String	SETTING_LOG_WARNING				= "logwarning";
	public static final String	SETTING_LOG_INFO				= "loginfo";
	public static final String	SETTING_LOG_USER				= "loguser";
	public static final String	SETTING_LOG_DEBUG				= "logdebug";
	public static final String	SETTING_LOG_URI					= "loguri";
	public static final String	SETTING_LOG_PORT				= "logport";
	public static final String	SETTING_LOG_PROTOCOL			= "logprotocol";
	public static final String	SETTING_LOG_SIZE				= "logmaxsize";
	public static final String	SETTING_NAV_TIMEOUT				= "navtimeout";
	public static final String	SETTING_HTTP_PROXY				= "http_proxy";
	public static final String	SETTING_HTTPS_PROXY				= "https_proxy";
	public static final String	SETTING_PROXY_EXCEPTIONS		= "no_proxy";
	public static final String	SETTING_REGEX_FILE				= "regexfile";		
	public static final String	SETTING_SIGNAL_REFRESH			= "signalrefresh";
	public static final String	SETTING_SIP_ENABLE				= "enablesip";
	public static final String	SETTING_START_PAGE				= "startpage";		
	public static final String	SETTING_HOURGLASS_LEFT			= "hourglassleft";
	public static final String	SETTING_HOURGLASS_TOP			= "hourglasstop";
	public static final String	SETTING_HOURGLASS_ENABLED		= "hourglassenabled";
	public static final String	SETTING_P1_ENABLED				= "enablefunctionkey_f1";
	public static final String	SETTING_P2_ENABLED				= "enablefunctionkey_f2";
	public static final String	SETTING_P3_ENABLED				= "enablefunctionkey_f3";
	public static final String	METATAG							= "metatag";
	public static final String	GEOLOCATION_ENABLED				= "geolocationenabled";
	public static final String	HTTP_AUTHENTICATION_USERNAME	= "username";
	public static final String	HTTP_AUTHENTICATION_PASSWORD	= "password";
	public static final String	APPLICATION_LICENSE		= "licensekey";
	public static final String	WEBSERVER_ENABLED		= "enabled";
	public static final String	WEBSERVER_PORT			= "port";
	public static final String	WEBSERVER_WEBFOLDER		= "webfolder";
	public static final String	WEBSERVER_PUBLIC		= "public";
	public static final String  SETTING_SCANNER_DISABLE_DURING_NAV = "disablescannerduringnavigation";
	public static final String  SETTING_CORS = "access-control-allow-origin";

    public static final String  SETTING_CAFILE      = "cafile";
	public static final String  SETTING_CAPATH      = "capath";
	public static final String  SETTING_VERIFY_PEER = "verifypeercertificate";
	
	public static final String	ERROR_CONFIG_READ 				= "Config.xml read error";
	public static final String	ERROR_BAD_XML 					= "Bad xml in Config.xml: ";
	public static final String	ERROR_FILE_NOT_FOUND 			= "Config.xml cannot be found and attempts in creating Config.xml have failed. Make sure the device is not connected to a PC";
	
	private static ArrayList<String> defaultMetaTag = null; 
	
	HashMap<String, String> settings;
	
	/**
	 * Initialises the config class and calls to load the config file. Also calls to create the regex.xml
	 * in the data directory if the file does not exist. Any logs are returned instead of sent to the logger
	 * because at this point the logger doesn't exist. 
	 * @return any logging messages that need to be logged.
	 */
	public ArrayList<LogEntry> load() throws ConfigException
	{
		boolean writeableStorage = false;
		settings = new HashMap<String, String>();
		
		//As the logger hasnt been started
		ArrayList<LogEntry> messages = new ArrayList<LogEntry>();
		
		String state = Environment.getExternalStorageState();
		
		if (state.equals(Environment.MEDIA_MOUNTED))
		{
		    // We can read and write the media
			writeableStorage = true;
			File directory = new File(Common.dataPath + "/");
			if(!directory.exists())
			{
				if(!directory.mkdirs());
				{
					messages.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Cannot create data directory"));
				}
			}
		}
		else if(state.equals(Environment.MEDIA_MOUNTED_READ_ONLY))
		{
		    // We can only read the media
			writeableStorage = false;
		}
		//Any other environment states are ignored
		
		messages.addAll(loadConfig(Common.internalDataPath + "/Config.xml", writeableStorage));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/keycodemapping.xml", RhoExtManager.getResourceId("raw","keycodemapping")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/RegEx.xml", RhoExtManager.getResourceId("raw","regex")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/menu.html", RhoExtManager.getResourceId("raw","menu")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/settings.html", RhoExtManager.getResourceId("raw","settings")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/style.css", RhoExtManager.getResourceId("raw","style")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/logo.gif", RhoExtManager.getResourceId("raw","logo")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/badlink.html", RhoExtManager.getResourceId("raw","badlink")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/apdconfig.xml", RhoExtManager.getResourceId("raw","apdconfig")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/apd.zip", RhoExtManager.getResourceId("raw","apd")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/background.png", RhoExtManager.getResourceId("raw","background")));
				
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/alert.png", RhoExtManager.getResourceId("raw","alert")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/backActive.png", RhoExtManager.getResourceId("raw","back_active")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/backFocused.png", RhoExtManager.getResourceId("raw","back_focused")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/backgroundTitle.png", RhoExtManager.getResourceId("raw","background_title")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/backNorm.png", RhoExtManager.getResourceId("raw","back_norm")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/blback.png", RhoExtManager.getResourceId("raw","blback")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/buttonDefault.png", RhoExtManager.getResourceId("raw","button_default")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/buttonPressed.png", RhoExtManager.getResourceId("raw","button_pressed")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/pressedItem.png", RhoExtManager.getResourceId("raw","pressed_item")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/spacerNormal.png", RhoExtManager.getResourceId("raw","spacer_normal")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/spacerNormalFocus.png", RhoExtManager.getResourceId("raw","spacer_normal_focus")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/spacerPressed.png", RhoExtManager.getResourceId("raw","spacer_pressed")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/spacerPressedFocus.png", RhoExtManager.getResourceId("raw","spacer_pressed_focus")));
		messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/images/unselectedItem.png", RhoExtManager.getResourceId("raw","unselected_item")));
		
		//Feature demo
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_bnorm.png", RhoExtManager.getResourceId("raw","fd_bnorm")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_bpress.png", RhoExtManager.getResourceId("raw","fd_bpress")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_conttopback.png", RhoExtManager.getResourceId("raw","fd_conttopback")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_conttopborder.png", RhoExtManager.getResourceId("raw","fd_conttopborder")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_expbut.png", RhoExtManager.getResourceId("raw","fd_expbut")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_expbutminus.png", RhoExtManager.getResourceId("raw","fd_expbutminus")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_flash.png", RhoExtManager.getResourceId("raw","fd_flash")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_headborder.png", RhoExtManager.getResourceId("raw","fd_headborder")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_motsol.png", RhoExtManager.getResourceId("raw","fd_motsol")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_rhoelements.png", RhoExtManager.getResourceId("raw","fd_rhoelements")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_rhologosmall.png", RhoExtManager.getResourceId("raw","fd_rhologosmall")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_sectback.png", RhoExtManager.getResourceId("raw","fd_sectback")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_sectborder.png", RhoExtManager.getResourceId("raw","fd_sectborder")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_selectback.png", RhoExtManager.getResourceId("raw","fd_selectback")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_topfade.png", RhoExtManager.getResourceId("raw","fd_topfade")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/images/fd_transferimage.png", RhoExtManager.getResourceId("raw","fd_transferimage")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/fd_prof.emmp", RhoExtManager.getResourceId("raw","fd_prof")));
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/Demos/FeatureDemo/fd.html", RhoExtManager.getResourceId("raw","fd")));
		
		//Temporary JSObjects file
		//messages.addAll(writeConfigurationFile(writeableStorage, Common.dataPath + "/JSobjects.js", RhoExtManager.getResourceId("raw","jsobjects")));
		
		return messages;
	}

	/**
	 * Loads the configuration file into the activity from the data directory. If the file does not exist,
	 * it is created with default values as defined in /res/raw/Config.xml (and a warning message is sent).
	 * If the directory is not writable, the default values in /res/raw/Config.xml are used (and a warning 
	 * message is sent).
	 */
	public ArrayList<LogEntry> loadConfig(String configFilePath, boolean writeableStorage) 
		throws ConfigException
	{
		ArrayList<LogEntry> messages = new ArrayList<LogEntry>();//Logger hasnt started, so save the log messages.
		//File configFile = new File(Common.dataPath + "/Config.xml");
		File configFile = null;
		boolean fileExists = false;
		try
		{
			configFile = new File(Common.parseAndroidURI(configFilePath));
		} 
		catch (InvalidParameterException e1)
		{
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} 
		catch (URISyntaxException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		if (configFile != null)
			fileExists = configFile.exists();
		
		//If the file does not exist, then write a default Config.xml.
		if(!fileExists && writeableStorage)
		{
			messages.add(new LogEntry(LogEntry.PB_LOG_INFO, "Config.xml not found. Generating default config file."));
			//Write a new Config.xml from res
			try
			{
				configFile.createNewFile();
				FileChannel writer = new FileOutputStream(configFile, false).getChannel();
				ReadableByteChannel channel = Channels.newChannel(Common.mainActivity.getResources().openRawResource(RhoExtManager.getResourceId("raw","config")));
				ByteBuffer buffer = ByteBuffer.allocate(1024);
				while(channel.read(buffer) > 0)
				{
					buffer.flip();
					writer.write(buffer);
					buffer.clear();
				}

				writer.close();
			}
			catch (IOException e)
			{
				messages.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Error writing config file"));
				writeableStorage = false;
				configFile.delete();
			}
		}
		if (configFile != null)
		fileExists = configFile.exists();
		BufferedReader reader = null;
		
		try
		{
			if(!fileExists)//Either the attempt at writing the config file failed, or it didnt exist and wasnt writable, so use defaults.
			{
				throw new FileNotFoundException();
			}
			reader = new BufferedReader(new FileReader(configFile));
			Xml.parse(reader, new ConfigHandler());
		}
		catch (SAXException e)
		{
			badConfig(ERROR_BAD_XML  + e.getMessage(), messages);
//			messages.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Error loading Config.xml: " + e.getMessage()));
//			messages.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Using default config settings."));
//			InputStream stream = Common.mainActivity.getResources().openRawResource(RhoExtManager.getResourceId("raw","config"));
//			try
//			{
//				Xml.parse(stream, Xml.Encoding.UTF_8, new ConfigHandler());
//				stream.close();
//			}
//			catch (Exception e1)
//			{
//				//Should never happen.
//				messages.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Last-chance config load failed. Could not read default config."));
//			}
		}
		catch (FileNotFoundException e)
		{
			badConfig(ERROR_FILE_NOT_FOUND, messages);
		}
		catch (IOException e)
		{
			badConfig(ERROR_CONFIG_READ, messages);
		}
		finally
		{
			if(reader != null)
			{
				try
				{
					reader.close();
				}
				catch (IOException e)
				{
					//Cant do anything
				}
			}
		}
		
		return messages;
	}
	
	/**
	 * Ease-of-use method to log an error and throw a ConfigException. Used if the Config.xml is bad.
	 * @param requiredSetting the error message to give the exception.
	 */
	private void badConfig(String errorString, ArrayList<LogEntry> logMessages) throws ConfigException
	{
		logMessages.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error in loading Config.xml: " + errorString + ". " + Common.APP_NAME + " is quitting..."));
		throw new ConfigException(errorString, logMessages);
	}

	/**
	 * Creates a default given file in the data directory if the file does not already exist 
	 * and if the directory is writable. It is created with default values as defined in /res/raw/[FILENAME] (and a warning message is sent).
	 */
	private ArrayList<LogEntry> writeConfigurationFile(boolean writeableStorage, String rawResourceName, int resourceId)
	{
		ArrayList<LogEntry> messages = new ArrayList<LogEntry>();//Logger hasnt started, so save the log messages.
		File resourceFile = new File(rawResourceName);
		boolean fileExists = resourceFile.exists();
		
		if(!fileExists && writeableStorage)
		{
			messages.add(new LogEntry(LogEntry.PB_LOG_INFO, resourceFile.getName() + " not found. Generating default " + resourceFile.getName()));
			try
			{
				resourceFile.getParentFile().mkdirs();
				resourceFile.createNewFile();
				FileChannel writer = new FileOutputStream(resourceFile, false).getChannel();
				ReadableByteChannel channel = Channels.newChannel(Common.mainActivity.getResources().openRawResource(resourceId));
				ByteBuffer buffer = ByteBuffer.allocate(1024);
				while(channel.read(buffer) > 0)
				{
					buffer.flip();
					writer.write(buffer);
					buffer.clear();
				}

				writer.close();
			}
			catch (IOException e)
			{
				messages.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Error writing default " + resourceFile.getName()));
				resourceFile.delete();
			}
		}
		return messages;
	}
	
	/**
	 * Get string setting value from the name
	 * @param name in lowercase
	 * @return
	 */
	public String getSetting(String name)
	{
		return settings.get(name);
	}
	
	/**
	 * Get string setting value from the name
	 * @param name in lowercase
	 * @return
	 */
	public boolean getBooleanSetting(String name)
	{
		try{return getSetting(name).equals("1");}
		catch(Exception e){return false;}
	}
	
	public static void applyDefaultMetaTags()
	{
		if (defaultMetaTag == null)
			return;
		Iterator<String> iterator = defaultMetaTag.iterator();
	    while (iterator.hasNext())
	    {
	    	String metatag = iterator.next();
	    	if (Common.metaReceiver != null)
	    	{
	    		int tildePos = metatag.indexOf('~'); //Separator used by default metatag
	    		if (tildePos > 0)
	    			Common.metaReceiver.setMeta(metatag.substring(0, tildePos), metatag.substring(tildePos + 1));
	    	}
	    }
	}
	
	class ConfigHandler implements ContentHandler
	{
		@Override
		public void characters(char[] ch, int start, int length) throws SAXException {}

		@Override
		public void endDocument() throws SAXException {}

		@Override
		public void endElement(String uri, String localName, String qName) throws SAXException {}

		@Override
		public void endPrefixMapping(String prefix) throws SAXException {}

		@Override
		public void ignorableWhitespace(char[] ch, int start, int length) throws SAXException {}

		@Override
		public void processingInstruction(String target, String data) throws SAXException {}

		@Override
		public void setDocumentLocator(Locator locator) {}

		@Override
		public void skippedEntity(String name) throws SAXException {}

		@Override
		public void startDocument() throws SAXException {}
		
		@Override
		public void startElement(String uri, String localName, String qName, Attributes atts) throws SAXException
		{
			// For now add each setting to the map
			String setting = null;
			int length = atts.getLength();
			for(int i = 0; i < length; i++)
			{		
				if(atts.getQName(i).equalsIgnoreCase("value"))
				{
						
					setting = atts.getValue(i);
				}
			}
			
			//String setting = atts.getValue("value");
			if(setting != null)
			{
					if (qName.equalsIgnoreCase(METATAG))
					{
						if (defaultMetaTag == null)
							defaultMetaTag = new ArrayList<String>();
						defaultMetaTag.add(setting);
					}
					else
						settings.put(localName.toLowerCase(), setting.replace("%INSTALLDIR%", Common.internalDataPath));
			}
			else
			{
				settings.put(localName.toLowerCase(), null);
			}
		}

		@Override
		public void startPrefixMapping(String prefix, String uri) throws SAXException {}
	}
	
	class ConfigException extends Exception
	{
		private ArrayList<LogEntry> logMessages;
		
		private static final long	serialVersionUID	= 262061320448820494L;

		public ConfigException(String detailMessage, ArrayList<LogEntry> logMessages)
		{
			super(detailMessage);
			this.logMessages = logMessages;
		}

		public ArrayList<LogEntry> getLoggerMessages()
		{
			return logMessages;
		}
	}
}
