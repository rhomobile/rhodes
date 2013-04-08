package com.rhomobile.rhodes.util;

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

public class Config
{
    private static final String TAG = Config.class.getSimpleName();

	private HashMap<String, String> mValuesHash = new HashMap<String, String>();

    public void load(InputStream is, String installDir) throws Exception {
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new InputStreamReader(is,"UTF-8"));
            Xml.parse(reader, new ConfigHandler(installDir));
        }
        catch (Throwable e) {
            Logger.E(TAG, e);
            throw new Exception("Load config.xml error", e);
        }
        finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    //Cant do anything
                }
            }
        }
    }

    public String getValue(String name) {
    	if (name == null) {
    		return null;
    	}
        String value = mValuesHash.get(name.toLowerCase());
        if (value == null) {
        	int pos = name.lastIndexOf("\\");
        	if (pos >= 0) {
        		String fixedName = name.substring(pos + 1);
        		value = mValuesHash.get(fixedName.toLowerCase());
        	}
        }
        Logger.T(TAG, "getValue: " + name + ": " + value);
        return value;
    }

	private class ConfigHandler implements ContentHandler
	{
		private String mInstallDir = "";

		public ConfigHandler(String installDir) {
			mInstallDir = installDir;
		}
		
		@Override
		public void startElement(String uri, String localName, String qName, Attributes atts) throws SAXException {
			String setting = null;
			int length = atts.getLength();
			for (int i = 0; i < length; i++) {
				if (atts.getQName(i).equalsIgnoreCase("value")) {
					setting = atts.getValue(i);
				}
			}
			
			if (setting != null) {
				if (!qName.equalsIgnoreCase("metatag")) {
				    String value = setting.replace("%INSTALLDIR%", mInstallDir);
				    String name = qName.toLowerCase();
				    mValuesHash.put(name, value);
				    Logger.T(TAG, "Get value: " + name + ": " + value);
				}
			} else {
                String name = qName.toLowerCase();
                mValuesHash.put(name, null);
                Logger.T(TAG, "Get value: " + name + ": null");
			}
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
		public void startPrefixMapping(String prefix, String uri) throws SAXException {}
	}
}
