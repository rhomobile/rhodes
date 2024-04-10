package com.rho.keycapture;


import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.FileChannel;
import java.nio.channels.ReadableByteChannel;
import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.app.Instrumentation;
import android.os.Environment;
import android.util.Log;
import android.util.SparseArray;
import android.view.KeyEvent;

import com.rho.keycapture.IKeyCaptureSingleton;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.webview.GoogleWebView;
import com.rhomobile.rhodes.webview.WebViewConfig;

public class KeyCaptureSingleton implements IKeyCaptureSingleton
{
	//private static final int KEYCODE_TRIGGER_1 = 102;
	//private static final int KEYCODE_TRIGGER_2 = 103;
	private static final String HEX_CHAR = "\\b0{1}([xX]{1})([a-fA-F\\d]{2})\\b";
	private static final String DEC_CHAR = "\\b([\\d]{1,6})\\b";
	private static final String KEY_VALUE_ALL = "ALL";
	private static final String CAPTURE_KEY_VALUE = "keyValue";
	private static final String CAPTURE_TRIGGER_FLAG = "triggerFlag";
	
	protected static String TAG = "KeyCaptureSingleton";
	private static Pattern hexCharPattern = Pattern.compile(HEX_CHAR);
	private static Pattern decCharPattern = Pattern.compile(DEC_CHAR);
	
	private Matcher hexCharMatcher;
	private Matcher decCharMatcher;
	private SparseArray<CaptureRecord> keyCaptureMap;
	private SparseArray<Integer> remapMap;
	private CaptureRecord allKeyCaptureRecord;
	private MethodResult triggerCaptureResult;
	
	private int mLastAction = KeyEvent.ACTION_UP;
	
	
	private boolean THIS_MODULE_CALLED=false;

	
	/**
	 * Creates a new KeyCaptureSingleton. Contains all the implementation for the KeyCapture extension.
	 */
	public KeyCaptureSingleton()
	{
		Logger.D(TAG, "Constructor");
		hexCharMatcher = hexCharPattern.matcher("");
		decCharMatcher = decCharPattern.matcher("");
		keyCaptureMap = new SparseArray<CaptureRecord>();
		remapMap = new SparseArray<Integer>();
		//copyMappingXML();
		KeyCodeRemapper.init();
	}

	@Override
	public void getHomeKeyValue(IMethodResult result)
	{
		THIS_MODULE_CALLED=true;
		Logger.D(TAG, "getHomeKeyValue");
		result.setError("HomeKey not supported on Android");
	}

	@Override
	public void setHomeKeyValue(String homeKeyValue, IMethodResult result)
	{
		THIS_MODULE_CALLED=true;
		Logger.D(TAG, "setHomeKeyValue");
		result.setError("HomeKey not supported on Android");
	}

	@Override
	public void captureKey(boolean dispatch, String keyValue, IMethodResult result)
	{
		
		Logger.D(TAG, "captureKey: " + keyValue + " dispatch: " + dispatch);
		Log.d("keycapt", "captureKey: " + keyValue + " dispatch: " + dispatch);
		THIS_MODULE_CALLED=true;
		//All case
		if(keyValue.equalsIgnoreCase(KEY_VALUE_ALL))
		{
			Log.d("keycapt", "ALL KEY");
			if(allKeyCaptureRecord != null)
			{
				MethodResult allKeyResult = allKeyCaptureRecord.getResult();
				if(allKeyResult != null) allKeyResult.release();
			}
			
			if(!isCallback(result))
			{
				Log.d("keycapt", "ALL KEY NO CALLBACK");
				if(dispatch)
				{
					Logger.D(TAG, "Clearing all keyCapture callback");
					allKeyCaptureRecord = null;
				}
				else			allKeyCaptureRecord = new CaptureRecord(false, null); //Consume the key, and no callback
			}
			else
			{
					Log.d("keycapt", "ALL KEY  CALLBACK");
				((MethodResult) result).keepAlive();
				allKeyCaptureRecord = new CaptureRecord(dispatch, (MethodResult) result);
			}
			Logger.D(TAG, "captureKey ALL-");
			return;
		}
		
		int keyValueInt;
		try
		{
			//keyValueInt = parseKeyValue(keyValue);
			keyValueInt = KeyCodeRemapper.getProperParameter(keyValue);
				Log.d("keycapt", "After remapping keyValueInt="+keyValueInt);
			
		}
		catch(InvalidParameterException e)
		{
			result.setError("Invalid key value");
			Logger.D(TAG, "captureKey Error-");
				Log.d("keycapt", "captureKey Error-");
			return;
		}
		
		if(!isCallback(result))
		{
			Logger.D(TAG, "Clearing " + keyValueInt + " keyCapture callback");
			keyCaptureMap.remove(keyValueInt);
			if(!dispatch) keyCaptureMap.put(keyValueInt,new CaptureRecord(false, null)); //Consume the key, and no callback
			Logger.D(TAG, "captureKey Clear-");
			return;
		}
		
		//clearKeyRecords(keyValueInt);
		keyCaptureMap.remove(keyValueInt);
		((MethodResult) result).keepAlive();
		MethodResult methodResult = ((MethodResult) result);
		keyCaptureMap.put(keyValueInt, new CaptureRecord(dispatch, methodResult));
		Logger.D(TAG, "captureKey-");
	}

	@Override
	public void remapKey(String keyValue, String remapTo, IMethodResult result)
	{
		Logger.D(TAG, "remapKey: " + keyValue + " to: " +remapTo);
		Log.d("keycapt","remapKey: " + keyValue + " to: " +remapTo);
		THIS_MODULE_CALLED=true;
		int keyValueInt;
		try
		{
			//keyValueInt = parseKeyValue(keyValue);
			keyValueInt = KeyCodeRemapper.getProperParameter(keyValue);
		}
		catch(InvalidParameterException e)
		{
			result.setError("Invalid keyValue");
			Logger.D(TAG, "remapKey ValueError-");
			return;
		}
		
		if(remapTo == null || remapTo.length() == 0) // To be able to remove key mappings
		{
			Logger.D(TAG, "Clearing " + keyValueInt + " remap");
			remapMap.remove(keyValueInt);
			Logger.D(TAG, "remapKey resetKey-");
			return;
		}

		int remapValue;
		try
		{
			//remapValue = parseKeyValue(remapTo);
			remapValue =  KeyCodeRemapper.getProperParameter(remapTo);
		}
		catch(InvalidParameterException e)
		{
			result.setError("Invalid remapTo value");
			Logger.D(TAG, "remapKey RemapError-");
			return;
		}
		
		//clearKeyRecords(keyValueInt);
		remapMap.remove(keyValueInt);
		Log.d("keycapt","FINAL remapKey: " + keyValueInt + " to: " +remapValue);
		remapMap.put(keyValueInt, remapValue);
		Logger.D(TAG, "remapKey-");
	}

	@Override
	public void captureTrigger(IMethodResult result)
	{
		Logger.D(TAG, "captureTrigger+");
		THIS_MODULE_CALLED=true;
		if(triggerCaptureResult != null)	triggerCaptureResult.release();
		triggerCaptureResult = null;

		if(isCallback(result))
		{
			((MethodResult) result).keepAlive();
			triggerCaptureResult = (MethodResult) result;
		}
		Logger.D(TAG, "captureTrigger-: " + triggerCaptureResult);
	}
	 
	/**
	 * Parses a keyValue string and returns its integer value. Can accept decimal and hex formats. 
	 * @param keyValue the String formatted hex or decimal key value to parse.
	 * @return the integer key value of the inputted string
	 * @throws InvalidParameterException if the keyValue could not be parsed.
	 * @author Ben Kennedy (NCVT73)
	 */
	private int parseKeyValue(String keyValue) throws InvalidParameterException
	{
		Logger.D(TAG, "parseKeyValue+");
		hexCharMatcher = hexCharMatcher.reset(keyValue);
		decCharMatcher = decCharMatcher.reset(keyValue);

		int keyCode;
		if (hexCharMatcher.matches())		keyCode = Integer.valueOf(keyValue.substring(2), 16);
		else if (decCharMatcher.matches())	keyCode = Integer.valueOf(keyValue);
		else throw new InvalidParameterException("Cannot parse value");
		Logger.D(TAG, "parseKeyValue-: " + keyCode);
		return keyCode;
	}
	
	/**
	 * Checks whether the result has a callback (true) or is empty (false)
	 * @param result the IMethodResult to query
	 * @return true is a callback exists, false if no callback exists
	 * @author Ben Kennedy (NCVT73)
	 */
	private boolean isCallback(IMethodResult result)
	{
		return (result != null && result.hasCallback());
	}
	private class KeyEventRunnable implements Runnable
	{
		private CaptureRecord record;
		private String str=null;
		private Object value=null;
		
		public KeyEventRunnable(CaptureRecord record,String key,Object obj)
		{
			this.record=record;
			this.str=key;
			this.value=obj;
		}
		
		@Override
		public void run()
		{
			HashMap<String,Object> resultMap=new HashMap<String,Object>(1);
			resultMap.put(str, value);
			if(record.getResult()!=null)
				record.getResult().set(resultMap);
		}
		
	}
	/**
	 * 
	 * @author Davide Schembari
	 */
	private class KeyEventGenerator implements Runnable
	{
		private KeyEvent mKeyEvent;
		
		public KeyEventGenerator(KeyEvent keyEvent)
		{
			mKeyEvent = keyEvent;
		}
		
		@Override
		public void run()
		{
			Logger.D(TAG, "Generating Key:" + mKeyEvent.getKeyCode());
			Instrumentation ins = new Instrumentation();
			ins.sendKeyDownUpSync(mKeyEvent.getKeyCode());
			Logger.D(TAG, "Generated Key");
		}
		
	}
	
	private boolean IsCodeErrorProne(int keyCode, KeyEvent event)
	{
		if(event.getRepeatCount()!=0)
			return true;
		
		boolean isDown1 = (event.getAction() == KeyEvent.ACTION_DOWN);
		if (mLastAction != event.getAction())
		{
			if (isDown1)
			    mLastAction = KeyEvent.ACTION_DOWN;
			else
			    mLastAction = KeyEvent.ACTION_UP;
			
		}
		return false;
	}
	
	/**
	 * Deals with incoming keyEvents. 
	 * @param keyCode the code of the key event
	 * @param event the keyEvent
	 * @return whether the event was consumed/absorbed (true) or whether it should bubble/pass on (false)
	 * @author Ben Kennedy NCVT73
	 */
	public synchronized boolean onKey(int keyCode, KeyEvent event)
	{
		if(!THIS_MODULE_CALLED)
		{
			return false;
		}
		
		
		if(IsCodeErrorProne(keyCode, event))
			return false;
	
	
	    
		Log.d("keycapt","CommonAPI onKey="+keyCode);
		Logger.D(TAG, "onKey+: " + keyCode);
		int disAppCallKey = 1000;
		IRhoConfig rhoelementsGetConfig= null; //RhoExtManager.getInstance().getConfig("rhoelementsext");
		IRhoWebView vi = null;
		try{
		    vi=RhodesActivity.safeGetInstance().getMainView().getWebView(0);
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
		
		
		GoogleWebView web=null;

		try {
			web=(GoogleWebView)vi;
		} catch ( ClassCastException e ) {
        	Logger.W(TAG, "onKey: WebView is not from Google, skipping configuration" );
			web = null;
		}

		if ( web != null ) {
			rhoelementsGetConfig=web.getConfig();
			if(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_APPLICATION_KEY_A1) != null){
				disAppCallKey = Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_APPLICATION_KEY_A1));
			}
		}
		//Trigger capture
		//if(keyCode == KEYCODE_TRIGGER_1 || keyCode == KEYCODE_TRIGGER_2)
		if(KeyCodeRemapper.isHWKeyCode(keyCode))
		{
			if(triggerCaptureResult != null)
			{
				Map<String, Object> returnMap = new HashMap<String, Object>(1);
				
				switch(event.getAction())
				{
					case KeyEvent.ACTION_DOWN: 	Logger.D(TAG, "onKey firing triggerEvent");	Log.d("keycapt","CommonAPI onKey firing triggerEvent="); returnMap.put(CAPTURE_TRIGGER_FLAG, keyCode); triggerCaptureResult.set(returnMap); break;
					case KeyEvent.ACTION_UP:	Logger.D(TAG, "onKey firing triggerEvent");     Log.d("keycapt","CommonAPI onKey firing triggerEvent=");     returnMap.put(CAPTURE_TRIGGER_FLAG, 0);triggerCaptureResult.set(returnMap); break;
					default: /*Ignore for now*/ break;
				}
			}
			else{
				Log.d("keycapt","CommonAPI onKey trigger not being captured=");
				Logger.D(TAG, "onKey- trigger not being captured");}
			return false;
		}
		
		// Added this condition to return windows key code if IS_WINDOWS_KEY flag is enabled in config.xml
		
		if(event.getAction() == KeyEvent.ACTION_DOWN && keyCode > 130 && keyCode < 143) {
			if(!WebViewConfig.IS_FUNCTIONS_KEY_DEF){
			 keyCode = 0; 
			}
		}
		if(keyCode == KeyEvent.KEYCODE_CALL && disAppCallKey == 0){
		  keyCode = 0;
		}
		//Remaps
		Integer remapKey = remapMap.get(keyCode);
		if(remapKey != null)
		{
			if (event.getAction() == KeyEvent.ACTION_DOWN)
			{
				Logger.D(TAG, "onKey remapping key event...");
					Log.d("keycapt","CommonAPI onKey remapping key event...");
				KeyEvent remappedKeyEvent = new KeyEvent(event.getDownTime(), event.getEventTime(), event.getAction(), remapKey, event.getRepeatCount());
				new Thread(new KeyEventGenerator(remappedKeyEvent)).start();
			}
			Logger.D(TAG, "onKey- remap");
			return true;
		}
		else{
				Log.d("keycapt","CommonAPI onKey not being remapped...");
				Logger.D(TAG, "onKey key not being remapped");}
		
		//CaptureEvents
		CaptureRecord record = null;
		record = keyCaptureMap.get(keyCode); //else find a keycapture record
		if(record == null && allKeyCaptureRecord != null) record = allKeyCaptureRecord; //If ALL key capture has been set, ignore specific keyCapture records
		if(record != null) //record found
		{
			Log.d("keycapt","CommonAPI onKey record found...");
			if(record.getResult() != null) //callback registered
			{
				Log.d("keycapt","CommonAPI onKey callback registered...");
				if(event.getAction() == KeyEvent.ACTION_DOWN)
				{
					Logger.D(TAG, "onKey keyEvent callback firing...");
					Log.d("keycapt","CommonAPI onKey  keyEvent callback firing...");
					Map<String, Object> returnMap = new HashMap<String, Object>(1);
					int returnKeyCode=KeyCodeRemapper.getProperReturnValue(keyCode);
					returnMap.put(CAPTURE_KEY_VALUE, returnKeyCode);
					if(keyCode !=0 ){
					//record.getResult().set(returnMap); //fire the callback
						// new Thread(new KeyEventRunnable(record,CAPTURE_KEY_VALUE,returnKeyCode)).start();
						Thread t = new Thread(new KeyEventRunnable(record,CAPTURE_KEY_VALUE,returnKeyCode));
						t.start();
						try {
							t.join(20);	
						} catch (InterruptedException ex) {
							Logger.E(TAG, ex);
						}
					}
				}
			}
			
			if(!record.isDispatch())
			{
				Log.d("keycapt","commonAPI onKey- absorbingEvent");
				Logger.D(TAG, "onKey- absorbingEvent");
				return true; //absorb the keyevent
			}
		}
		else{
			Log.d("keycapt","commonAPI onKey- key not being captured");
			Logger.D(TAG, "onKey- key not being captured");}
		
		return false;
	}
	
	private boolean isAnyAppKeyEnabled(int keyCode) {
		// TODO Auto-generated method stub
		boolean appKeyCode = false;
		try{
			IRhoConfig rhoelementsGetConfig= null;/* RhoExtManager.getInstance().getConfig("rhoelementsext");*/
			IRhoWebView vi=RhodesActivity.safeGetInstance().getMainView().getWebView(0);
			GoogleWebView web=(GoogleWebView)vi;
			rhoelementsGetConfig=web.getConfig();
			if(rhoelementsGetConfig != null){
				switch (keyCode) {
				case KeyEvent.KEYCODE_CALL: 
										if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_APPLICATION_KEY_A1)) == 1){
											appKeyCode = true;
										}
										break;
				case KeyEvent.KEYCODE_CALCULATOR: 
										if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_APPLICATION_KEY_A2)) == 1)
												appKeyCode = true;
										break;
				case KeyEvent.KEYCODE_CALENDAR: 
										if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_APPLICATION_KEY_A3)) == 1)
												appKeyCode = true;
										break;
				case KeyEvent.KEYCODE_CAMERA: 
										if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_APPLICATION_KEY_A4)) == 1)
												appKeyCode = true;
										break;	
				case KeyEvent.KEYCODE_CONTACTS: 
										if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_APPLICATION_KEY_A5)) == 1)
												appKeyCode = true;
										break;	
				case KeyEvent.KEYCODE_EXPLORER: 
										if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_APPLICATION_KEY_A6)) == 1)
												appKeyCode = true;
										break;
				case KeyEvent.KEYCODE_MUSIC: 
										if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_APPLICATION_KEY_A7)) == 1)
												appKeyCode = true;
										break;						
				default:
						break;
				}
			}
		}
		catch(NumberFormatException e){
			e.printStackTrace();
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
					
		return appKeyCode;
	}
	
	private boolean isAnyFuncKeyEnabled(int keyCode) {
		// TODO Auto-generated method stub
		boolean andKeyCode = false;
		try{
			IRhoConfig rhoelementsGetConfig= null;/* RhoExtManager.getInstance().getConfig("rhoelementsext");*/
			IRhoWebView vi=RhodesActivity.safeGetInstance().getMainView().getWebView(0);
			GoogleWebView web=(GoogleWebView)vi;
			rhoelementsGetConfig=web.getConfig();
			if(rhoelementsGetConfig != null){
				switch (keyCode) {
				case KeyEvent.KEYCODE_F1:
					if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_FUNCTION_KEY_F1)) == 1)
						andKeyCode = true;
					break;
				case KeyEvent.KEYCODE_F2:
					if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_FUNCTION_KEY_F2)) == 1){
						andKeyCode = true;
					}
					break;
				case KeyEvent.KEYCODE_F3:
					if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_FUNCTION_KEY_F3)) == 1){
						andKeyCode = true;
					}
					break;
				case KeyEvent.KEYCODE_F4:
					if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_FUNCTION_KEY_F4)) == 1){
						andKeyCode = true;
					}
					break;
				case KeyEvent.KEYCODE_F5:
					if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_FUNCTION_KEY_F5)) == 1){
						andKeyCode = true;
					}
					break;
				case KeyEvent.KEYCODE_F6:
					if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_FUNCTION_KEY_F6)) == 1){
						andKeyCode = true;
					}
					break;
				case KeyEvent.KEYCODE_F7:
					if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_FUNCTION_KEY_F7)) == 1){
						andKeyCode = true;
					}
					break;
				case KeyEvent.KEYCODE_F8:
					if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_FUNCTION_KEY_F8)) == 1){
						andKeyCode = true;
					}
					break;
				case KeyEvent.KEYCODE_F9:
					if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_FUNCTION_KEY_F9)) == 1){
						andKeyCode = true;
					}
					break;
				case KeyEvent.KEYCODE_F10:
					if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_FUNCTION_KEY_F10)) == 1){
						andKeyCode = true;
					}
					break;
				case KeyEvent.KEYCODE_F11:
					if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_FUNCTION_KEY_F11)) == 1){
						andKeyCode = true;
					}
					break;
				case KeyEvent.KEYCODE_F12:
					if(Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_FUNCTION_KEY_F12)) == 1){
						andKeyCode = true;
					}
					break;
				default:
					break;
				}
			}
		}
		catch(NumberFormatException e){
			e.printStackTrace();
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
		
		return andKeyCode;
	}
	

}
