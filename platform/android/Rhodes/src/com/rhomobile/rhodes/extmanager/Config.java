package com.rhomobile.rhodes.extmanager;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;

import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import android.util.Xml;

import com.rhomobile.rhodes.Logger;


// TODO - make access to map thread-safe

/**
 * @author Geoff Day (XFH386, Initial Creation)
 * @author Ben Kennedy (NCVT73, Added resource control to load())
 */
public class Config
{
    private static final String TAG = Config.class.getSimpleName();

	public static final String	SETTING_AUTOROTATE				= "autorotate";
	public static final String	SETTING_BADLINK_FILE			= "badlinkuri";		
	public static final String	SETTING_DEBUG_BUTTONS_ENABLED	= "debugbuttonsenabled";
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

	public static final	String[] REQUIRED_OPTIONS				= { };

	public static final String	ERROR_TAG_NOT_FOUND				= "Required tag: \"%1$s\" not found.";
	public static final String	ERROR_TAG_EMPTY					= "Required tag: \"%1$s\" cannot be empty.";
	public static final String	ERROR_CONFIG_READ 				= "Config.xml read error";
	public static final String	ERROR_BAD_XML 					= "Bad xml in Config.xml: ";
	public static final String	ERROR_FILE_NOT_FOUND 			= "Config.xml cannot be found and attempts in creating Config.xml have failed. Make sure the device is not connected to a PC";
	
	private static ArrayList<String> defaultMetaTag = null; 
	
	private HashMap<String, String> settings;

	/**
	 * Initialises the config class
	 */
	public Config() {
		settings = new HashMap<String, String>();
	}

    /**
     * Loads the configuration file from the input stream
     */
    public void load(InputStream is, String installDir) throws ConfigException
    {
        BufferedReader reader = null;
        try
        {
            reader = new BufferedReader(new InputStreamReader(is,"UTF-8"));
            Xml.parse(reader, new ConfigHandler(installDir));
        }
        catch (SAXException e)
        {
            e.printStackTrace();
            throw new ConfigException(ERROR_BAD_XML);
        }
        catch (FileNotFoundException e)
        {
            e.printStackTrace();
            throw new ConfigException(ERROR_FILE_NOT_FOUND);
        }
        catch (IOException e)
        {
            e.printStackTrace();
            throw new ConfigException(ERROR_CONFIG_READ);
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
        
        //Check to see if start page and bad link are there.
        for(String requiredSetting: REQUIRED_OPTIONS)
        {
            if(settings.get(requiredSetting) == null)
            {
                throw new ConfigException(String.format(ERROR_TAG_NOT_FOUND, requiredSetting));
            }
            else if(settings.get(requiredSetting).equals(""))
            {
                throw new ConfigException(String.format(ERROR_TAG_EMPTY, requiredSetting));
            }
        }
    }
	/**
	 * Get string setting value from the name
	 * @param name in lowercase
	 * @return setting string value
	 */
    public String getSetting(String name) {
    	if (name == null) {
    		return null;
    	}
        String value = settings.get(name.toLowerCase());
        if (value == null) {
        	int slashindex = name.lastIndexOf("\\");
        	if (slashindex >= 0) {
        		String fixedname = name.substring(slashindex+1);
        		value = settings.get(fixedname.toLowerCase());
        	}
        }
        Logger.T(TAG, "getSettings: " + name + "=>" + value);
        return value;
    }

	/**
	 * Get string setting value from the name
	 * @param name in lowercase
	 * @return setting boolean value
	 */
    boolean getBooleanSetting(String name) {
        try {
            return getSetting(name).equals("1");
        }
        catch(Exception e){return false;}
    }

	private class ConfigHandler implements ContentHandler
	{
		private String mInstallDir = "";

		public ConfigHandler(String installDir) {
			mInstallDir = installDir;
		}
		
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
		public void startElement(String uri, String localName, String qName, Attributes atts) throws SAXException {
			// For now add each setting to the map
			String setting = null;
			int length = atts.getLength();
			for (int i = 0; i < length; i++) {
				if (atts.getQName(i).equalsIgnoreCase("value")) {
					setting = atts.getValue(i);
				}
			}
			
			if (setting != null) {
				if (qName.equalsIgnoreCase(METATAG)) {
					if (defaultMetaTag == null) {
						defaultMetaTag = new ArrayList<String>();
					}
					defaultMetaTag.add(setting);
				} else {
				    String value = setting.replace("%INSTALLDIR%", mInstallDir);
				    String name = qName.toLowerCase();//localName.toLowerCase(); 
				    settings.put(name, value);
				    Logger.T(TAG, "Set RhoElements config value: " + name + "=>" + value);
				}
			} else {
                String name = qName.toLowerCase();//localName.toLowerCase(); 
                settings.put(name, null);
                Logger.T(TAG, "Set RhoElements config value: " + name + "=> null");
			}
		}

		@Override
		public void startPrefixMapping(String prefix, String uri) throws SAXException {}
	}
	
	public class ConfigException extends Exception {
		private static final long	serialVersionUID	= 262061320448820494L;

		public ConfigException(String detailMessage) {
			super(detailMessage);
		}

	}
}
