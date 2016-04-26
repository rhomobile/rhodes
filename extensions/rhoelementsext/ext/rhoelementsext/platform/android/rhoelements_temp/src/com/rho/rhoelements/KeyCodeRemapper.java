package com.rho.rhoelements;


import java.io.File;
import java.io.IOException;
import java.security.InvalidParameterException;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;


//import org.w3c.dom.Document;
//import org.w3c.dom.Element;
//import org.w3c.dom.Node;
//import org.w3c.dom.NodeList;
//import org.xml.sax.InputSource;
//import org.xml.sax.SAXException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.webview.GoogleWebView;



import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.Environment;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.Toast;

public class KeyCodeRemapper {
	
		//private static Context ctx=null;
		
	
	
		
		public static final HashMap<Integer, Integer> remapperHashMap = new HashMap<Integer, Integer>();
		
		private static String KEYCODEMAPPING_XML=Environment.getExternalStorageDirectory()+"/Android/data/"+RhodesActivity.safeGetInstance().getPackageName()+"/keycodemapping.xml";
		
		private static boolean IS_WINDOWS_KEY=false;
		public static int DOES_NOT_EXIST=-5000;
		
		private static Matcher hexCharMatcher;
		private static Matcher decCharMatcher;
		private static final String HEX_CHAR = "\\b0{1}([xX]{1})([a-fA-F\\d]{2})\\b";
		private static final String DEC_CHAR = "\\b([\\d]{1,6})\\b";
		private static Pattern hexCharPattern = Pattern.compile(HEX_CHAR);
		private static Pattern decCharPattern = Pattern.compile(DEC_CHAR);
		
		private static final String FROM_STR="from";
		private static final String TO_STR="to";
		
		/**
		 * This method initializes the key event maps and has to be called before using them.
		 */
		
		public static void init()
		{
			setPlatform();
			hexCharMatcher = hexCharPattern.matcher("");
			decCharMatcher = decCharPattern.matcher("");
			initHashMap();
			
			
		}
		private static void initHashMap()
		{
			remapperHashMap.clear();
			putDefaultHWKeys();
			if(IS_WINDOWS_KEY)
				fillLocallyAndToWinHashMap();
			
			 File fXmlFile = new File(KEYCODEMAPPING_XML);
			 Log.d("mapper","fXmlFile exists="+fXmlFile.exists());
			 if(fXmlFile.exists() && fXmlFile.canRead())
			 {
				 fillMapperKeyCodesFromXML();
			 }
			
			
			
		}
		
		private static void putDefaultHWKeys()
		{
			remapperHashMap.put(102, 102);//TRIGGER_1
			remapperHashMap.put(103, 103);//Trigger_2
			remapperHashMap.put(104, 104);//Trigger_2
		}
		
		
		
		private static void setPlatform()
		{
		
			try{
			IRhoConfig rhoelementsGetConfig= null;
	        IRhoWebView vi=RhodesActivity.safeGetInstance().getMainView().getWebView(0);
	        GoogleWebView web=(GoogleWebView)vi;
	        rhoelementsGetConfig=web.getConfig();
	        if(rhoelementsGetConfig.getString("iswindowskey") != null){
	        	IS_WINDOWS_KEY = Integer.parseInt(rhoelementsGetConfig.getString("iswindowskey"))==1 ? true:false;
	        }
			}
			catch(Exception ex)
			{
				IS_WINDOWS_KEY=false;
			}
 
		}
		
	
		public static boolean isHWKeyCode(int keyCode)
		{
		boolean hwKey=false;
			
			if(keyCode==102 || keyCode==103 || keyCode==104 )
				hwKey=true;
			
			return hwKey;
		}
	private static void  fillMapperKeyCodesFromXML()
	{
			File fXmlFile = new File(KEYCODEMAPPING_XML);
            DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder dBuilder = null;
            Document doc = null;
				try {
					dBuilder = dbFactory.newDocumentBuilder();
					doc = dBuilder.parse(fXmlFile);
				} catch (ParserConfigurationException e1) {
					 Log.d("mapper","ParserConfigurationException ="+e1.getMessage());
					e1.printStackTrace();
				} catch (SAXException e) {
					 Log.d("mapper","SAXException ="+e.getMessage());
					e.printStackTrace();
				} catch (IOException e) {
					 Log.d("mapper","IOException ="+e.getMessage());
					e.printStackTrace();
				}
            doc.getDocumentElement().normalize();
            
            NodeList parent = doc.getElementsByTagName("KeyCodes");
          
            NodeList nList=parent.item(0).getChildNodes();
            Element el=(Element) parent.item(0);
            NodeList children=el.getElementsByTagName("KEYCODE");
            for (int i = 0; i < children.getLength(); i++) {
            	
           	 String k=children.item(i).getAttributes().getNamedItem(FROM_STR).getTextContent();
           	 String v=children.item(i).getAttributes().getNamedItem(TO_STR).getTextContent();
           	  k=k.trim();
           	 v=v.trim();
           	Log.d("mapper","name="+k+" value="+v);
           	int key=0;
           	int value=0;
           	try{
           		key=parseKeyValue(k);
           		value=parseKeyValue(v);
           	}
           	catch(InvalidParameterException ex)
           	{
           		continue;
           	}
           	catch(NumberFormatException ex)
           	{
           		continue;
           	}
           	
           	deleteFromHashMapIfValueExists(value);
           	remapperHashMap.put(key, value);
           	
           }
            for (Entry<Integer, Integer> entry : remapperHashMap.entrySet()) {
		            	Log.d("mapper","key="+entry.getKey()+" value="+entry.getValue());
		            }
		   
		}
		
		private static void deleteFromHashMapIfValueExists(int value)
		{
			int key=-1000;
			for (Entry<Integer, Integer> entry : remapperHashMap.entrySet()) {
	            if (entry.getValue()==value) {
	            	key=entry.getKey();
	            	break;
	            }
	        }
			if(key!=-1000)
			{
				remapperHashMap.remove(key);
			}
		}
		
		private static int parseKeyValue(String keyValue) throws InvalidParameterException
		{
			
			hexCharMatcher = hexCharMatcher.reset(keyValue);
			decCharMatcher = decCharMatcher.reset(keyValue);

			int keyCode;
			if (hexCharMatcher.matches())		keyCode = Integer.valueOf(keyValue.substring(2), 16);
			else if (decCharMatcher.matches())	keyCode = Integer.valueOf(keyValue);
			else throw new InvalidParameterException("Cannot parse value");
			
			return keyCode;
		}
		private static void fillLocallyAndToWinHashMap()
		{
			//numbers
			remapperHashMap.put(7, 0x30);
			remapperHashMap.put(8, 0x31);
			remapperHashMap.put(9, 0x32);
			remapperHashMap.put(10, 0x33);
			remapperHashMap.put(11, 0x34);
			remapperHashMap.put(12, 0x35);
			remapperHashMap.put(13, 54);
			remapperHashMap.put(14, 55);
			remapperHashMap.put(15, 56);
			remapperHashMap.put(16, 57);
			
			//alphabates
			
			remapperHashMap.put(29, 0x41);
			remapperHashMap.put(30, 0x42);
			remapperHashMap.put(31, 0x43);
			remapperHashMap.put(32, 0x44);
			remapperHashMap.put(33, 0x45);
			remapperHashMap.put(34, 0x46);
			remapperHashMap.put(35, 0x47);
			remapperHashMap.put(36, 0x48);
			remapperHashMap.put(37, 0x49);
			remapperHashMap.put(38, 0x4A);
			remapperHashMap.put(39, 0x4B);
			remapperHashMap.put(40, 0x4C);
			remapperHashMap.put(41, 0x4D);
			remapperHashMap.put(42, 0x4E);
			remapperHashMap.put(43, 0x4F);
			remapperHashMap.put(44, 0x50);
			remapperHashMap.put(45, 0x51);
			remapperHashMap.put(46, 0x52);
			remapperHashMap.put(47, 0x53);
			remapperHashMap.put(48, 0x54);
			remapperHashMap.put(49, 0x55);
			remapperHashMap.put(50, 0x56);
			remapperHashMap.put(51, 0x57);
			remapperHashMap.put(52, 0x58);
			remapperHashMap.put(53, 0x59);
			remapperHashMap.put(54, 0x5A);
		
			
			remapperHashMap.put(19, 0x26);
			remapperHashMap.put(20, 0x28);
			remapperHashMap.put(21, 0x25);
			remapperHashMap.put(22, 0x27);
		
			
			remapperHashMap.put(131, 0x70);
			remapperHashMap.put(132, 0x71);
			remapperHashMap.put(133, 0x72);
			remapperHashMap.put(134, 0x73);
			remapperHashMap.put(135, 0x74);
			remapperHashMap.put(136, 0x75);
			remapperHashMap.put(137, 0x76);
			remapperHashMap.put(138, 0x77);
			remapperHashMap.put(139, 0x78);
			remapperHashMap.put(140, 0x79);
			remapperHashMap.put(141, 0x7A);
			remapperHashMap.put(142, 0x7B);
			remapperHashMap.put(143, 0x7C);
			
			
			remapperHashMap.put(24, 0xAF);
			remapperHashMap.put(25, 0xAE);
			remapperHashMap.put(164, 0xAD);
			
			
			remapperHashMap.put(113, 0x11);
			remapperHashMap.put(114, 0x11);
			remapperHashMap.put(111, 0x1B);
			remapperHashMap.put(61, 0x09);
			remapperHashMap.put(59, 0x10);
			remapperHashMap.put(60, 0x10);
			remapperHashMap.put(62, 0x20);
			remapperHashMap.put(64, 0x08);
			remapperHashMap.put(66, 0x0D);
			remapperHashMap.put(67, 0x2E);
			remapperHashMap.put(119, 0);
			remapperHashMap.put(57, 0x12);
			remapperHashMap.put(58, 0x12);
			
			
			remapperHashMap.put(144, 0x60);
			remapperHashMap.put(145, 0x61);
			remapperHashMap.put(146, 0x62);
			remapperHashMap.put(147, 0x63);
			remapperHashMap.put(148, 0x64);
			remapperHashMap.put(149, 0x65);
			remapperHashMap.put(150, 0x66);
			remapperHashMap.put(151, 0x67);
			remapperHashMap.put(152, 0x68);
			remapperHashMap.put(153, 0x69);
			remapperHashMap.put(154, 0x6A);
			remapperHashMap.put(155, 0x6B);
			remapperHashMap.put(156, 0x6C);
			remapperHashMap.put(157, 0x6D);
			remapperHashMap.put(158, 0x6E);
			
			
			remapperHashMap.put(102,102);
			remapperHashMap.put(103,103);
			remapperHashMap.put(104,104);

			
			
			
		
	
						
		}
		
		@SuppressLint("ShowToast") 
		public static int getProperParameter(String keyCodeStr)
		{
			int keyCode=parseKeyValue(keyCodeStr);
			int retCode=DOES_NOT_EXIST;
			
			if(remapperHashMap.containsValue(keyCode))
			{
				for (Entry<Integer, Integer> entry : remapperHashMap.entrySet()) {
		            if (entry.getValue()==keyCode) {
		            	retCode=entry.getKey();
		            	break;
		            }
		        }
			}
			else
				retCode=keyCode;
			
			
			return retCode;
		}
		public static int getProperParameter(int keyCode)
		{
			int retCode=DOES_NOT_EXIST;
			
			if(remapperHashMap.containsValue(keyCode))
			{
				for (Entry<Integer, Integer> entry : remapperHashMap.entrySet()) {
		            if (entry.getValue()==keyCode) {
		            	retCode=entry.getKey();
		            	break;
		            }
		        }
			}
			else
				retCode=keyCode;
			
			
			return retCode;
		}
	
		public static int getProperReturnValue(int retValue)
		{
			int propRetValue=retValue;
			
			if(remapperHashMap.containsKey(retValue))
			{
				propRetValue=remapperHashMap.get(retValue);
			}
			
			return propRetValue;
			
			
		}

		public static void  set_is_windows_key(boolean value)
		{
			IS_WINDOWS_KEY=value;
		}
		public static boolean get_is_windows_key()
		{
			return IS_WINDOWS_KEY;
		}
		public static void setKeyCodeMappingFileLoc(String location)
		{
			KEYCODEMAPPING_XML=location;
		}
		public static String getKeyCodeMappingFileLoc()
		{
			return KEYCODEMAPPING_XML;
		}
		

}
