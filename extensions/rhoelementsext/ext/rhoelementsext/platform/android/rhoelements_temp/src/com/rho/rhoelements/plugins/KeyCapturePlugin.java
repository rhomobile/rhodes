/**
 * 
 */
package com.rho.rhoelements.plugins;

import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.rho.rhoelements.KeyCodeRemapper;
import com.rho.rhoelements.Common;
import com.rho.rhoelements.Config;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rho.rhoelements.common.VKeyCodes;
import com.rho.rhoelements.services.ScannerService;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.webview.GoogleWebView;
import com.rhomobile.rhodes.webview.WebViewConfig;
import com.rhomobile.rhodes.extmanager.IRhoWebView.TextZoom;

import android.app.Instrumentation;
import android.content.Intent;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;

/**
 * @author FPC843
 *
 */
public class KeyCapturePlugin extends Plugin implements View.OnKeyListener
{
	public static final String HEX_CHAR = "\\b0{1}([xX]{1})([a-fA-F\\d]{2})\\b";
	public static final String DEC_CHAR = "\\b([\\d]{1,6})\\b";
	private static final String KEY_VALUE = "keyValue";
	private static final String TRIGGER_FLAG = "triggerFlag";
	public static boolean beingHere = false;
	private HashMap<Integer, KeyEventStructure> mKeyMap = null;
	private HashMap<Integer, String> mEventMap = null;
	private boolean mDispatch = false; //This is used when "All" keys are captured
	private boolean mCaptureAllKey = false;
	private int mLastKeyMapped = -1; //We need to remember what's the last key passed to KeyValue in order to carry out Remap correctly
	private int mHomeKeyCode = -1;
//	private String mKeyEventUrl = null;
	private String mTriggerEventUrl = null;
	Handler mHandler = null;
	private int zoomInOutLevelVar = 1;
	/**
	 * Workaround for a platform issue on MC40 where there are times where the trigger keys may generate inconsisten events, ie. multiple subsequent
	 * keydown events with counter 0. 
	 */
	private int mLastAction = KeyEvent.ACTION_UP;
	
	
	private boolean THIS_MODULE_CALLED=false;
	
	/**
	 * Constructor
	 */
	public KeyCapturePlugin()
	{
		mKeyMap = new HashMap<Integer, KeyEventStructure>();
		mEventMap = new HashMap<Integer, String>();
		mHandler = new Handler();
		VKeyCodes.init();
		
		 //AndroidToWindowsParser.init(Common.mainActivity.getApplicationContext());
		//AndroidToWindowsParser.init();
	}
	
	private boolean PerformZoomIfEnabled(int keyCode,KeyEvent event)
	{
		if (Common.ZOOM_KEY_ENABLE
				&& event.getAction() == KeyEvent.ACTION_DOWN) {
			
			if (keyCode == Common.ZOOM_IN_KEY && zoomInOutLevelVar >= 0) {
				zoomInOutLevelVar = getZoomSetValue();
				zoomInOutLevelVar = zoomInOutLevelVar + 1;
				
				if(zoomInOutLevelVar <= 4)
					Common.elementsCore.browserSetTextZoom(zoomInOutLevelVar);
				
				return true;

			} else if (keyCode == Common.ZOOM_OUT_KEY) {
				zoomInOutLevelVar = getZoomSetValue();
				if(zoomInOutLevelVar > 0)
					zoomInOutLevelVar = zoomInOutLevelVar - 1;
				Common.elementsCore.browserSetTextZoom(zoomInOutLevelVar);
				if(zoomInOutLevelVar < 0)
					zoomInOutLevelVar = 0;
				return true;
			}
			
		}
		return false;
	}
	public synchronized boolean onKeyDispatchFromActivity(int keyCode,KeyEvent event)
	
	{
		
		Log.d("keycapt","KeyCapturePlugin->onKeyDispatchFromActivity,keyCode="+keyCode+" repeat count="+event.getRepeatCount()+"Action IsDown="+(event.getAction() == KeyEvent.ACTION_DOWN));
		
		boolean isDown1 = (event.getAction() == KeyEvent.ACTION_DOWN);
		if (mLastAction != event.getAction())
		{
			if (isDown1)
			    mLastAction = KeyEvent.ACTION_DOWN;
			else
			    mLastAction = KeyEvent.ACTION_UP;
		}
		
		        //If the application is in background simply ignore
				if (!Common.elementsCore.bLaunchingAppHasFocus)
					return false;
				
				//change to a queued thread
				if ( ( KeyCodeRemapper.isHWKeyCode(keyCode) ) && event.getRepeatCount() == 0)
				{
					Log.d("keycapt","Caught Trigger for the Scanner");
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Caught Trigger for the Scanner"));
					boolean isDown = (event.getAction() == KeyEvent.ACTION_DOWN);
					sendTriggerToScanner(isDown);
					/*if (mLastAction != event.getAction())
					    sendTriggerToScanner(isDown);
					if (isDown)
					    mLastAction = KeyEvent.ACTION_DOWN;
					else
					    mLastAction = KeyEvent.ACTION_UP;
					    */
					    
				}
				
		
		if(PerformZoomIfEnabled(keyCode,event))
			return true;
		
		//If this module is NOT called return false.....else process it
		if(!THIS_MODULE_CALLED)
			{
				Log.d("keycapt","KeyCapturePlugin->THIS MODULE IS NOT CALLED ...HENCE RETURNING FALSE");
				return false;
			}
		if(event.getRepeatCount()!=0)
			return false;
		
		
		
		int disAppCallKey = 1000;
		IRhoConfig rhoelementsGetConfig= null; //RhoExtManager.getInstance().getConfig("rhoelementsext");
		IRhoWebView vi=RhodesActivity.safeGetInstance().getMainView().getWebView(0);

		GoogleWebView web=null;
		try {
			web=(GoogleWebView)vi;
		} catch ( ClassCastException e ) {
			//Logger.W(TAG, "onKeyDispatchFromActivity: WebView is not from Google, skipping configuration" );
			web = null;
		}

		if ( web != null ) {
			rhoelementsGetConfig=web.getConfig();
			if((rhoelementsGetConfig!=null) && (rhoelementsGetConfig.getString(WebViewConfig.ENABLE_APPLICATION_KEY_A1) != null)){
 				disAppCallKey = Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_APPLICATION_KEY_A1));
 			}
 		}
		
		
		if(event.getAction() == KeyEvent.ACTION_DOWN && keyCode > 130 && keyCode < 143) {
			if(!WebViewConfig.IS_FUNCTIONS_KEY_DEF){
			 keyCode = 0; 
			}
		}
		if(keyCode == KeyEvent.KEYCODE_CALL && disAppCallKey == 0){
			  keyCode = 0;
			}
		boolean res = false;
		beingHere = true;
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, " OS KeyCode: " + keyCode));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "KeyEvent: " + event.toString()));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Action: " + event.getAction()));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Return KeyCode: " + KeyCodeRemapper.getProperReturnValue(keyCode)));
		
		/**
		 * Check whether the event comes from one of the Px keys, as they must be enabled from the config file
		 * Such keys are P1 (Volume Down), P2 (Volume Up) and P3 (Search)
		 */
		/*if ( (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN) && 
				(Common.config.getSetting(Config.SETTING_P1_ENABLED) != null) &&
				(Common.config.getSetting(Config.SETTING_P1_ENABLED).compareTo("0") == 0) ) //P1 is disabled
			return true;
		else if ( (keyCode == KeyEvent.KEYCODE_VOLUME_UP) && 
				(Common.config.getSetting(Config.SETTING_P2_ENABLED) != null) &&
				(Common.config.getSetting(Config.SETTING_P2_ENABLED).compareTo("0") == 0) ) //P2 is disabled
			return true;
		else if ( (keyCode == KeyEvent.KEYCODE_SEARCH) && 
				(Common.config.getSetting(Config.SETTING_P3_ENABLED) != null) &&
				(Common.config.getSetting(Config.SETTING_P3_ENABLED).compareTo("0") == 0) ) //P3 is disabled
			return true;
		*/
		Integer keyCodeObject = new Integer(keyCode);
		if ( (event.getAction() == KeyEvent.ACTION_DOWN) && (!KeyCodeRemapper.isHWKeyCode(keyCode)) )
		{
			if ( (keyCode == mHomeKeyCode) && (mKeyMap.get(keyCode) == null) )
			{
				Log.d("keycapt","is mapped as home button");
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, keyCode + " is mapped as home button"));
				Common.elementsCore.browserHome();
				res = true; //consume the event
			}
			else if (mKeyMap.containsKey(keyCodeObject) == true)
			{
				KeyEventStructure mappedKeyEvent = mKeyMap.get(keyCode);
				if (mappedKeyEvent != null)
				{
					if (mappedKeyEvent.keyCode == -1 ) //keyCode is not remapped
					{
						res = !mappedKeyEvent.dispatch;
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Is " + keyCode + " going to be dispatched: " + res));
						Log.d("keycapt","##CAPTURE## KeyCode Is " + keyCode + " going to be dispatched: " + res);
						mHandler.post(new KeyEventRunnable(KEY_VALUE, keyCode, mEventMap.get(keyCode)));
						return res;
					}	
					else
					{
						KeyEventStructure remappedKeyCode = mKeyMap.get(new Integer(keyCode));
						Log.d("keycapt","##REMAP## From  " + keyCode + " TO " + remappedKeyCode.keyCode);
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, keyCode + " is remapped to 0x" + remappedKeyCode.keyCode));
						KeyEvent remappedKeyEvent = new KeyEvent(event.getDownTime(), 
																 event.getEventTime(), 
																 event.getAction(), 
																 remappedKeyCode.keyCode, 
																 event.getRepeatCount());
						/*At the moment only the key code of the created key event is uses, as it looks like the only API able to emulate correctly the key event is
						 * sendKeyDownUpSync(int key) from the Instrumentation class
						 */
						if (event.getAction() == KeyEvent.ACTION_DOWN)
							new Thread(new KeyEventGenerator(remappedKeyEvent)).start();
						return true; ///Remap is not compatible with Dispatch:True, therefore the key event has to be consumed.
					}
				}
			}		
			else if ( (mCaptureAllKey) && !KeyCodeRemapper.isHWKeyCode(keyCode) /*&& keyCode != 0*/)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "All key use case"));
				
				KeyEventStructure allKeysEvent = mKeyMap.get(Integer.MAX_VALUE);
				res = !allKeysEvent.dispatch;
				Log.d("keycapt","##CAPTURE ALL##,keyCode="+keyCode+" dispath="+allKeysEvent.dispatch);
				mHandler.post(new KeyEventRunnable(KEY_VALUE, keyCode, mEventMap.get(Integer.MAX_VALUE)));
			}
		}
		else if ( KeyCodeRemapper.isHWKeyCode(keyCode) )
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Trigger use case"));
			Log.d("keycapt","Trigger use case");
			if (event.getAction() == KeyEvent.ACTION_DOWN)
			{
			    if (event.getRepeatCount() == 0) 
			    	mHandler.post(new KeyEventRunnable(TRIGGER_FLAG, keyCode, mTriggerEventUrl));
			    mLastAction = KeyEvent.ACTION_DOWN;
			}
			else if (event.getAction() == KeyEvent.ACTION_UP)
			{
			    if (event.getRepeatCount() == 0) 
			    	mHandler.post(new KeyEventRunnable(TRIGGER_FLAG, 0, mTriggerEventUrl));
			    mLastAction = KeyEvent.ACTION_UP;
			}
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Returning " + res));
		Log.d("keycapt","Returnng res="+res);
		if(event.getAction() == KeyEvent.ACTION_DOWN && keyCode == KeyEvent.KEYCODE_CALL && disAppCallKey == 1){
			return true;
		}
		return res;
	}

	@Override
	public boolean onKey(View view, int keyCode, KeyEvent event)
	{
		return false;
		
	}
	
	private boolean isAnyAppKeyEnabled(int keyCode) {
		// TODO Auto-generated method stub
		boolean appKeyCode = false;
		try{
			IRhoConfig rhoelementsGetConfig= null;
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
					
		return appKeyCode;
	}
	private boolean isAnyFuncKeyEnabled(int keyCode) {
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
		
		return andKeyCode;
	}
	/**
	 * @see com.rho.rhoelements.plugins.Plugin#onSetting(com.rho.rhoelements.PluginSetting)
	 */
	@Override
	public void onSetting(PluginSetting setting)
	{
		//Log.d("keycapttest","######## Settings Called for 2.x");
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		
		if (setting == null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "setting is null"));
			return;
		}
		
		if (setting.getName().equalsIgnoreCase("Dispatch"))
		{
			THIS_MODULE_CALLED=true;
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Dispatch"));
			if (setting.getValue().compareToIgnoreCase("true") == 0)
				mDispatch = true;
			else if (setting.getValue().compareToIgnoreCase("false") == 0)
				mDispatch = false;
			if (mLastKeyMapped >= 0)
			{
				KeyEventStructure remappedKeyCode = mKeyMap.get(new Integer(mLastKeyMapped));
				if (remappedKeyCode != null)
					remappedKeyCode.dispatch = mDispatch;
			}
		}
		else if (setting.getName().equalsIgnoreCase("KeyValue"))
		{
			THIS_MODULE_CALLED=true;
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "KeyValue	"));
			Pattern hexCharPattern = Pattern.compile(HEX_CHAR);
			Matcher hexCharMatcher = hexCharPattern.matcher(setting.getValue());
			Pattern decCharPattern = Pattern.compile(DEC_CHAR);
			Matcher decCharMatcher = decCharPattern.matcher(setting.getValue());
			int keyCode = -1;
			mCaptureAllKey = false;
			if (hexCharMatcher.matches())
				keyCode = Integer.valueOf(setting.getValue().substring(2), 16);
			else if (decCharMatcher.matches())
				keyCode = Integer.valueOf(setting.getValue());
			else if (setting.getValue().compareToIgnoreCase("All") == 0)
			{
				mCaptureAllKey = true;
				if(mKeyMap.get(Integer.MAX_VALUE) == null)//Dont reset if already exists
				{
					mKeyMap.put(Integer.MAX_VALUE, new KeyEventStructure(Integer.MAX_VALUE, false));
				}
				mLastKeyMapped = Integer.MAX_VALUE;
				return;
			}
			if ( keyCode > -1)
			{
				/******TATJ****/
				//if ( VKeyCodes.windowsToAndroidEventMap.get(keyCode) != null )
				//	mLastKeyMapped = VKeyCodes.windowsToAndroidEventMap.get(keyCode);
				/******TATJ****/
				if(KeyCodeRemapper.getProperParameter(keyCode)!=KeyCodeRemapper.DOES_NOT_EXIST)
					mLastKeyMapped=KeyCodeRemapper.getProperParameter(keyCode);
				if(mKeyMap.get(mLastKeyMapped) == null)//Dont reset if already exists
					mKeyMap.put(mLastKeyMapped, new KeyEventStructure(-1, false));
			}
			else
			{
				mLastKeyMapped = -1;
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Passed value is not a valid numeric code"));
			}
		}
		else if (setting.getName().equalsIgnoreCase("Remap"))
		{
			THIS_MODULE_CALLED=true;
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Remap"));
			if (mLastKeyMapped > -1)
			{
				Pattern hexCharPattern = Pattern.compile(HEX_CHAR);
				Matcher hexCharMatcher = hexCharPattern.matcher(setting.getValue());
				Pattern decCharPattern = Pattern.compile(DEC_CHAR);
				Matcher decCharMatcher = decCharPattern.matcher(setting.getValue());
				int keyCode = -1;
				if (hexCharMatcher.matches())
					keyCode = Integer.valueOf(setting.getValue().substring(2), 16);
				else if (decCharMatcher.matches())
					keyCode = Integer.valueOf(setting.getValue());
				if ( keyCode > -1 )
				{
					/******TATJ*******/
					//if ( VKeyCodes.windowsToAndroidEventMap.get(keyCode) != null )
					//	mKeyMap.put(mLastKeyMapped, new KeyEventStructure(VKeyCodes.windowsToAndroidEventMap.get(keyCode), false));
					/******TATJ*******/
					if(KeyCodeRemapper.getProperParameter(keyCode)!=KeyCodeRemapper.DOES_NOT_EXIST)
						mKeyMap.put(mLastKeyMapped, new KeyEventStructure(KeyCodeRemapper.getProperParameter(keyCode), false));
				}
				else
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Passed value is not a valid numeric code"));
			}
		}
		else if (setting.getName().equalsIgnoreCase("AccelerateKey"))
		{
			THIS_MODULE_CALLED=true;
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "AccelerateKey"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Not supported"));
		}
		else if (setting.getName().equalsIgnoreCase("HomeKeyValue"))
		{
			THIS_MODULE_CALLED=true;
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "HomeKeyValue"));
			if (setting.getValue().compareToIgnoreCase("disabled") == 0)
			{
				mHomeKeyCode = -1;
				return;
			}
			Pattern hexCharPattern = Pattern.compile(HEX_CHAR);
			Matcher hexCharMatcher = hexCharPattern.matcher(setting.getValue());
			Pattern decCharPattern = Pattern.compile(DEC_CHAR);
			Matcher decCharMatcher = decCharPattern.matcher(setting.getValue());
			int keyCode = -1;
			if (hexCharMatcher.matches())
				keyCode = Integer.valueOf(setting.getValue().substring(2), 16);
			else if (decCharMatcher.matches())
				keyCode = Integer.valueOf(setting.getValue());
			if (keyCode > -1)
			{
				/****TATJ****/
				//if ( VKeyCodes.windowsToAndroidEventMap.get(keyCode) != null )
				//	mHomeKeyCode = VKeyCodes.windowsToAndroidEventMap.get(keyCode);
				/****TATJ****/
				if(KeyCodeRemapper.getProperParameter(keyCode)!=KeyCodeRemapper.DOES_NOT_EXIST)
					mHomeKeyCode=KeyCodeRemapper.getProperParameter(keyCode);
				
			}
			else
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Passed value is not a valid numeric code"));
		}
		else if (setting.getName().equalsIgnoreCase("KeyEvent"))
		{
			THIS_MODULE_CALLED=true;
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "KeyEvent"));
			if(mLastKeyMapped != -1)
			{
				if (setting.getValue().length() > 0)
				{
					mEventMap.put(mLastKeyMapped, setting.getValue());
				}
				else
					mEventMap.remove(mLastKeyMapped);
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Attempted to set keyEvent, but keyValue has not been set. Ignoring."));
			}
			mLastKeyMapped = -1;
		}
		else if (setting.getName().equalsIgnoreCase("TriggerEvent"))
		{
			THIS_MODULE_CALLED=true;
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "TriggerEvent"));
			if (setting.getValue().length() > 0)
				mTriggerEventUrl = setting.getValue();
			else
				mTriggerEventUrl = null;
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}

	/**
	 * @see com.rho.rhoelements.plugins.Plugin#onShutdown()
	 */
	@Override
	public void onShutdown()
	{
		// TODO Auto-generated method stub
		
	}

	/** (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onPageStarted(java.lang.String)
	 */
	@Override
	public void onPageStarted(String url)
	{
		//KeyCapture current status has to be reset when another page is loaded
		reset();
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("KeyCapturePlugin");
	}
	
	private void reset()
	{
		if (mKeyMap != null)
			mKeyMap.clear();
		if (mEventMap != null)
			mEventMap.clear();
		mDispatch = false;
		mCaptureAllKey = false;
		mLastKeyMapped = -1;
		//mHomeKeyCode = -1;
//		mKeyEventUrl = null;
		mTriggerEventUrl = null;
		mLastAction = KeyEvent.ACTION_UP;
	}
	
	class KeyEventStructure
	{
		public int keyCode; /**keycode of the key event to generate*/
		public boolean dispatch; /**true if the keyevent has to be dispatched, false otherwise */
		
		public KeyEventStructure(int keyCode, boolean dispatch)
		{
			this.keyCode = keyCode;
			this.dispatch = dispatch;
		}
	}
	
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
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Keycode:" + mKeyEvent.getKeyCode()));
			Instrumentation ins = new Instrumentation();
			ins.sendKeyDownUpSync(mKeyEvent.getKeyCode());
		}
		
	}
	
	private class KeyEventRunnable implements Runnable
	{
		int mKeyCode;
		String mEventUrl;
		String mName;
		
		public KeyEventRunnable(String name, int keyCode, String eventUrl)
		{
			mName = name;
			mKeyCode = keyCode;
			mEventUrl = eventUrl;
		}
		
		@Override
		public void run()
		{
			if ( (mEventUrl != null) && (mKeyCode >= 0) )
				try
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "mEventUrl: " + mEventUrl));
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "mName: " + mName));
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "OS mKeyCode: " + mKeyCode));
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Return keycode: " + KeyCodeRemapper.getProperReturnValue(mKeyCode)));
					/*****TATJ*****/
					/*if ( VKeyCodes.androidToWindowsEventMap.get(mKeyCode) != null )
					{
						Log.d("keycapt","WINDOWSs keycode="+VKeyCodes.androidToWindowsEventMap.get(mKeyCode));
						navigate(mEventUrl, mName, String.valueOf(VKeyCodes.androidToWindowsEventMap.get(mKeyCode)));
					}
					else
					{
						Log.d("keycapt","ANDROID keycode="+VKeyCodes.androidToWindowsEventMap.get(mKeyCode));
						navigate(mEventUrl, mName, String.valueOf(mKeyCode)); //Just in case the event is valid but not mapped to any Windows V-Key code
					}
					*/
					/*****TATJ*****/
					if (KeyCodeRemapper.getProperReturnValue(mKeyCode)!=KeyCodeRemapper.DOES_NOT_EXIST )
					{
						Log.d("keycapt","##From parser## WINDOWS/ANDROIDs keycode="+KeyCodeRemapper.getProperReturnValue(mKeyCode));
						navigate(mEventUrl, mName, String.valueOf(KeyCodeRemapper.getProperReturnValue(mKeyCode)));
					}
					else
					{
						Log.d("keycapt","##No parser## ANDROID keycode="+mKeyCode);
						navigate(mEventUrl, mName, String.valueOf(mKeyCode)); //Just in case the event is valid but not mapped to any Windows V-Key code
					}
				}
				catch (NavigateException e)
				{
					e.printStackTrace();
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
				}
				catch (NullPointerException e)
				{
					e.printStackTrace();
				}
			else if (mEventUrl != null)
				navigate(mEventUrl);
		}
	}
	
	/**
	 * Sends a trigger event to the scanner if the scanner is enabled
	 * 
	 * @param isKeyDown whether to send a keyDown (true) event or a keyUp (false) event
	 * @return whether the trigger event was acted upon.
	 */
	public boolean sendTriggerToScanner(boolean isKeyDown)
	{
		if (Common.elementsCore.isScannerEnabled())
		{
		    Intent intent = new Intent(ScannerService.SCANNER_TRIGGER_EVENT);
		    intent.putExtra("isKeyDown", isKeyDown);
		    intent.putExtra("pid", android.os.Process.myPid());
		    Common.mainActivity.sendBroadcast(intent);
		    return true;
		}
		return false;
	}
	
	public int getZoomSetValue(){
		TextZoom textZoom = Common.elementsCore.getWebViewInstance().getTextZoom();
		
		if (textZoom == TextZoom.SMALLEST)
			return 0;
		else if (textZoom == TextZoom.SMALLER)
		 return 1;
		else if (textZoom == TextZoom.NORMAL)
			return 2;
		else if (textZoom == TextZoom.LARGER)
			return 3;
		else if (textZoom == TextZoom.LARGEST)
			return 4;
		else 
			return 0;
	}


}
