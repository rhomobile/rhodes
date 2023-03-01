package com.rho.rhoelements;

import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.IntentFilter;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Rect;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
//import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;

import android.text.InputType;
import android.text.method.PasswordTransformationMethod;
import android.util.Log;
import android.view.inputmethod.EditorInfo;
import android.widget.AbsoluteLayout;
import android.widget.RelativeLayout;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.WindowManager.BadTokenException;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.AbsoluteLayout;
import android.widget.EditText;
import android.widget.MediaController;
import android.widget.RelativeLayout;

import com.rho.rhoelements.Config.ConfigException;
import com.rho.rhoelements.activex.Generic;
import com.rho.rhoelements.graphics.IndicatorPanel;
import com.rho.rhoelements.npapi.NPAPIServer;
import com.rho.rhoelements.plugins.ApplicationPlugin;
import com.rho.rhoelements.plugins.HourglassPlugin;
import com.rho.rhoelements.plugins.KeyCapturePlugin;
import com.rho.rhoelements.plugins.SipPlugin;
import com.rho.rhoelements.plugins.VideoCapturePlugin;
import com.rho.rhoelements.plugins.SipPlugin.SipMode;
import com.rho.rhoelements.services.LockService;
import com.rho.rhoelements.services.ScannerService;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.R.string;
import com.rhomobile.rhodes.extmanager.IRhoWebView;




/////////////////////////////////////////////////////
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import com.rho.intent.IntentReceiver;

/**
 * @author xfh386
 * 
 */
public class ElementsCore implements IActivityListener, IElementsCore,
		OnClickListener
{
	public static int RECEIVER_REGISTER_STATUS = 0;
	IntentReceiver mReceiver=null;
	
    private static final String TAG = ElementsCore.class.getSimpleName();
	public static final String	HISTORY_BACK_HREF			= "history:back";
	private static final String	RHOELEMENTSPACKAGENAME		= "com.rho.rhoelements";
	private static final long	DEFAULT_NAV_TIMEOUT			= 0;															// default
																																// value
																																// for
																																// navigation
																																// timeout
	private static final String	RHODES						= "rhodes";
	public static final String	RHOMOBILE_NAVIGATE_ACTION	= "com.rho.rhoelements.RHOMOBILE_NAVIGATE_ACTION";
	public static final String	RHOMOBILE_QUIT_ACTION		= "com.rho.rhoelements.RHOMOBILE_QUIT_ACTION";
	public static final String	RHOELEMENTS_SHORTCUT_ACTION	= "com.rho.rhoelements.SHORTCUT_ACTION";

	// This is the current home page. It may be the one configured by
	// config.xml, or set by a home screen shortcut
	// private String mCurrentUrl;
	private String				mStartPage;
	private int					mCurrentShortcutId			= 0;
    
	// Flag if we're quitting so close down stuff is only called once
	boolean						isShuttingDown;

	// Flag to store the foreground status from Rhodes APIs
	public boolean				bLaunchingAppHasFocus;
	
	// Invisible Edit Text field for Numeric keyboard
	private WebEditText mWebEditText = null;
	
	// Reasons for bad link
	public enum BadLinkReason {
		STOPPED, TIMEOUT, ERROR, UNLICENSED
	};

	// The main WebView component
	//private IRhoWebView			mWebView;
	// To play video files
	private RhoVideoView mVideoView;
	private String				webViewTitle				= "";
	private String				webViewLastStart			= "";
	private String				webViewLastEnd				= "";
    private String              m_Text                      = "";
	// Dialog id's
	public static final int		APD_WAIT_DIALOG				= 0;
	public static final int		APD_PROGRESS_DIALOG			= 1;
	public static final int		CHARACTER_PICKER_DIALOG		= 2;
	public static final int		HTTP_AUTHENTICATION_DIALOG	= 3;
	private ProgressDialog		mProgressDialog;
	public Handler				mProgressHandler			= new Handler() {
																@Override
																public void handleMessage(
																		Message msg)
																{
																	if (msg.what >= mProgressDialog
																			.getMax())
																		mProgressDialog
																				.dismiss();
																	else
																		mProgressDialog
																				.setProgress(msg.what);
																}
															};
	private Handler				mNavigationHandler			= new Handler();

	private Bundle				mSavedInstanceState			= null;

	// Scanner Service objects
	private Intent				mScannerBindIntent;
	private ServiceConnection	mScannerConnection;
	private ScannerService		mScannerService;
	private boolean				mScannerPaused;

	// Lock Service objects
	private Intent				mLockBindIntent				= null;
	private LockService			mLockService				= null;

	// Boolean variable to disable application focus events when Video recoridng
	// intent is active
	// public boolean isVideoCapture = false;

	// Thread to receive requests from the NPAPI plugin
	private NPAPIServer			npapiServer					= null;

	// Used to reduce the amount of intents sent when the trigger is pressed.
	private boolean				scannerEnabled;

	private ArrayList<LogEntry>	mMessages;
	private String				configErrorMessage;

	private LocalWebServer		localWebServer;

	// call to Plugin to set the port
	static private native void setTcpPort(int port);

	URI						badLinkUri;

	// Result code from video capture activity
	public int				RESULT_VIDEO_CODE	= 1;

	private long			m_navigationTimeout;
	private Timer			m_pageLoadTimer;

	boolean					mIsNewTask			= false;	// true if the
															// activity has
															// been created from
															// scratch, false
															// otherwise

	private boolean			mIsFirstTime		= true;
		
	public String			base64Data			= null;
		

	public void addPluginUi(Activity activity)
	{		
		LayoutInflater inflater = activity.getLayoutInflater();
		View pluginView = inflater.inflate(RhoExtManager.getResourceId("layout","plugin_ui"), null);
		activity.addContentView(pluginView, new ViewGroup.LayoutParams(
				ViewGroup.LayoutParams.FILL_PARENT,
				ViewGroup.LayoutParams.FILL_PARENT));
	}

	public void onCreate(Activity activity, View mainView,
			Bundle savedInstanceState)
	{
		Common.init(activity.getApplicationInfo().packageName);

		// Setup variables
		Common.mainActivity = activity;
		Common.elementsCore = this;
		Common.fetchUUID();

		// Check to see if we've come from Rhodes
		if (!savedInstanceState.containsKey("source")
				|| !savedInstanceState.getString("source").equals(RHODES))
		{
			Common.isRhodes = false;
		} else
		{
			Common.isRhodes = true;
		}

		/**
		 * Add the plugin ui to the Activity main view
		 */
		LayoutInflater inflater = activity.getLayoutInflater();
		View pluginView = inflater.inflate(RhoExtManager.getResourceId("layout","plugin_ui"), null);
		activity.addContentView(pluginView, new ViewGroup.LayoutParams(
				ViewGroup.LayoutParams.FILL_PARENT,
				ViewGroup.LayoutParams.FILL_PARENT));

		setIdForMainView(mainView);

		isShuttingDown = false;
		bLaunchingAppHasFocus = true;

		this.mSavedInstanceState = savedInstanceState;


		mMessages = new ArrayList<LogEntry>(); // Used as a message buffer while
												// the logger is not started
		configErrorMessage = new String();
		boolean configPassed = startConfig();
		startLogger();
		
		com.rhomobile.rhodes.Logger.I(TAG, "RhoElements initializing...");
		
		//Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,
		//		"ElementsCore used by: "
		//				+ savedInstanceState.getString("source")));
		
		mIsNewTask = true;

		if (!configPassed)
		{
			startBadConfigDialog();
		} else
		{
			// startValidationCheck();
			initializeActivity();
	        com.rhomobile.rhodes.Logger.I(TAG, "RhoElements initialized.");
		}
	}

	private void setIdForMainView(View mainView)
	{
		if (mainView.getId() == View.NO_ID)
			mainView.setId(RhoExtManager.getResourceId("id","elements_main_view"));
	}

	/*
	 * @Override public void onCreate(Bundle savedInstanceState) {
	 * isShuttingDown = false; this.mSavedInstanceState = savedInstanceState; }
	 */

	public void onNewIntent(Intent intent, boolean updateIntentId)

	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Start"));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "webView: " + getWebViewInstance()));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "intent: " + intent));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "updateIntentId: " + updateIntentId));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "mCurrentShortcutId: " + mCurrentShortcutId));
		/*
		 * Problem: This method is called when we come from a shortcut. Suppose
		 * the shortcut is set with http://google.co.uk Now the user navigates
		 * to other pages until he minimises RE (put it to the background). When
		 * the user will bring RE back to the foreground, exactly the same
		 * intent sent from the shortcut will be sent, and therefore RE would
		 * load the shortcut url rather than the last visited url. Obviously
		 * this could be apparently fixed by storing the last visited url and
		 * ignoring what it comes from the shortcut intent if a variable
		 * containing the last url is set. However this wouldn't work because we
		 * could get here from another shortcut and in such case we don't want
		 * to ignore the new url. Solution: Every shortcut contains an id which
		 * is calculated from its url. The activity keeps track of the received
		 * ids. If we have Shortcut1 with Id1 and Shortcut2 with Id2 and we get
		 * here from Shortcut1, the activity will store such id. The user
		 * navigates to other pages and minimise; but when he brings the
		 * activity back to the foreground and it will receive the Shortcut1
		 * intent, the id will be the same as stored and the activity can
		 * therefore ignore the url and will keep the last visited url. If we
		 * get here via Shortcut2, this id will be different than the current
		 * one and therefore the activity will load the new url.
		 */
		if ((intent != null) && (intent.getAction() != null) && (intent.getAction().compareTo(RHOELEMENTS_SHORTCUT_ACTION) == 0)) // Getting here from a shortcut
		{
			if (intent.getExtras() != null)
			{
				if (intent.getExtras().getInt("id") != mCurrentShortcutId) // Id is different, then take actions
				{
					if ((intent.getDataString() != null)
							&& (intent.getDataString().length() > 0))
					{
						// Let's check which kind of shortcut this is (url or path to config.xml)
						if (intent.getType().compareTo(Common.mainActivity.getString(string.shortcut_mime_url)) == 0)
						{
							try
							{
								mStartPage = Common.addDefaultProtocol(intent.getDataString());
							} catch (MalformedURLException e1)
							{
								if (Common.logger != null)
									Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Malformed Shortcut URL"));

								mStartPage = null;
							}
						} else if (intent.getType().compareTo(Common.mainActivity.getString(string.shortcut_mime_configpath)) == 0)
						{
							try
							{
								Common.config.loadConfig(intent.getDataString(), false);

								if (mIsNewTask == false)
									applyNewSettings();

								mStartPage = Common.config.getSetting(Config.SETTING_START_PAGE);
							} catch (ConfigException e)
							{
								e.printStackTrace();
								showBadLink(BadLinkReason.ERROR);
								return;
							}
						}
						if (updateIntentId)
							mCurrentShortcutId = intent.getExtras().getInt("id");
					}
					if (RhoExtManager.getInstance().onStartNewConfig()) {
					    loadStartPage();
					}
				}
			}
		} else if ((intent == null) || (intent.getAction() == null) || (intent.getAction().compareTo(Intent.ACTION_MAIN) == 0)) // Getting here from the app grid or if starting by URI
		{
			try
			{
				if ((mIsNewTask == false) && (mCurrentShortcutId != 0)) // We get here from the application grid
				{
					Common.config.loadConfig(Common.internalDataPath + "/Config.xml", false);
					applyNewSettings();
				}
				
				if (RhoExtManager.getInstance().onStartNewConfig()) {
				    com.rhomobile.rhodes.Logger.T(TAG, "Looking for start page.");
				    mStartPage = Common.config.getSetting(Config.SETTING_START_PAGE);
				    loadStartPage();
				}
			} catch (ConfigException e)
			{
				com.rhomobile.rhodes.Logger.E(TAG, e);
			}
			mCurrentShortcutId = 0;
		}
		mIsNewTask = false;
	}

	/**
	 * In essence, a bootstrapper. Starts all the parts of Neon that are
	 * required to run at first startup.
	 */
	public void initializeActivity()
	{
		startLocalWebServer();
		setStartPage();
		startNPAPI();
		startScannerService();
		startLockService();
		startPluginManagerAndLogVersions();
		Common.metaReceiver = new MetaReceiver();

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Intent! "
				+ Common.mainActivity.getIntent().getDataString()));
	}

	public void configureWebKit()
	{
		// Initialise the navigation timeout
        String navTimeoutStr = Common.config.getSetting(Config.SETTING_NAV_TIMEOUT);
        if (navTimeoutStr != null) {
            try
            {
                m_navigationTimeout = Long.parseLong(navTimeoutStr);
            } catch (Exception e)
            {
                Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, String
                        .format("Invalid NavTimeout (%s), setting to default",
                                Common.config.getSetting(Config.SETTING_NAV_TIMEOUT))));
                m_navigationTimeout = DEFAULT_NAV_TIMEOUT;
            }
        }

		if (Common.pluginManager.getPlugin("KeyCapture") == null)
		{
			Common.pluginManager.loadPlugin("KeyCapture");
			getWebViewInstance().getView().setOnKeyListener((View.OnKeyListener) Common.pluginManager.getPlugin("KeyCapture"));
		}
		setBadLink();
	}

	// Called when new settings are loaded from a shortcut
	private void applyNewSettings()
	{
		startFullscreen();
		startAutoRotate();
		startSipSettings();
		startDebugButtons();
		startSettingsButton();

		// re-initialise navigation timeout
		// Initialise the navigation timeout
		String navTimeoutStr = Common.config.getSetting(Config.SETTING_NAV_TIMEOUT);
		if (navTimeoutStr != null) {
		    try
		    {
		        m_navigationTimeout = Long.parseLong(navTimeoutStr);
		    } catch (Exception e)
		    {
		        Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, String
		                .format("Invalid NavTimeout (%s), setting to default",
		                        Common.config.getSetting(Config.SETTING_NAV_TIMEOUT))));
		        m_navigationTimeout = DEFAULT_NAV_TIMEOUT;
		    }
		}
	}

	/* **************************************************** */
	/* Begin start methods */
	/* **************************************************** */

	private void setStartPage()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		onNewIntent(Common.mainActivity.getIntent(), false);
	}

	private void startLocalWebServer()
	{
		String isWebServerRequested = Common.config
				.getSetting(Config.WEBSERVER_ENABLED);
		if (isWebServerRequested != null && isWebServerRequested.equals("1"))
		{
			try
			{
				int webServerPort=Integer.parseInt(Common.config.getSetting(Config.WEBSERVER_PORT));
				if (webServerPort==80) 
					webServerPort=8082;// port 80 is NOT supported in android.
				localWebServer = new LocalWebServer(
						webServerPort,
						Common.parseAndroidURI(
								Common.config
										.getSetting(Config.WEBSERVER_WEBFOLDER))
								.toString(), Common.config.getSetting(
								Config.WEBSERVER_PUBLIC).compareTo("1") == 0,
						false);
				localWebServer.start();
			} catch (NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e
						.getMessage()));
			} catch (InvalidParameterException e)
			{
				// TODO Auto-generated catch block
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e
						.getMessage()));
			} catch (URISyntaxException e)
			{
				// TODO Auto-generated catch block
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e
						.getMessage()));
			}
		}
	}

	private void startHourglass()
	{
		// Required to use the hourglass
		Common.pluginManager.loadPlugin(PluginManager.PLUGIN_HOURGLASS);
	}

	private void startDebugButtons()
	{
		// Check whether DebugButtonsEnabled is true
		String debugButtonSetting = Common.config
				.getSetting(Config.SETTING_DEBUG_BUTTONS_ENABLED);
		if ((debugButtonSetting != null)
				&& (debugButtonSetting.compareToIgnoreCase("1") == 0))
			enableDebugButtons(true);
		else
			enableDebugButtons(false);
	}
	
	public void startSettingsButton()
	{
		// Check whether SettingsButtonEnabled is true
		String settingsButtonSetting = Common.config
				.getSetting(Config.SETTING_SETTINGS_BUTTON_ENABLED);
		if ((settingsButtonSetting != null)
				&& (settingsButtonSetting.compareToIgnoreCase("1") == 0))
			enableSettingsButton(true);
		else
			enableSettingsButton(false);
	}

	/**
	 * Intercepts all key presses in WebView and ignores or processes them now
	 * using this method instead of OnKeyListener method due to compatibility
	 * issues with the SoftInputMethod.
	 * 
	 * @author hpkx48, fpc843
	 * 
	 */
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String
				.valueOf(event.getKeyCode())));

		switch (event.getKeyCode())
		{
		case KeyEvent.KEYCODE_BACK:
			break;
		case KeyEvent.KEYCODE_SEARCH:
			break;

		default:
			break;
		}
		/*
		 * Check whether there is an instance of the KeyCapturePlugin we can
		 * pass the key events to
		 */
		KeyCapturePlugin keyCapturer = (KeyCapturePlugin) Common.pluginManager
				.getPlugin("KeyCapture");
		if ((keyCapturer != null) && (KeyCapturePlugin.beingHere == false))
		{
			keyCapturer.onKey(null, keyCode, event);
		}
		KeyCapturePlugin.beingHere = false;

		return true;
	}
	public boolean onKeyDispatchFromActivity(int keyCode,KeyEvent event)
	{
		//Log.d("keycapt","ElementsCore->onKeyDispatchFromActivity,keyCode="+keyCode);
		if (Common.pluginManager.getPlugin("KeyCapture") == null)
		{
			//Log.d("keycapt","ElementsCore->LOADING"+keyCode);
			Common.pluginManager.loadPlugin("KeyCapture");
			//getWebViewInstance().getView().setOnKeyListener((View.OnKeyListener) Common.pluginManager.getPlugin("KeyCapture"));
			
		}
		KeyCapturePlugin keyCapturer = (KeyCapturePlugin) Common.pluginManager.getPlugin("KeyCapture");
		return keyCapturer.onKeyDispatchFromActivity( keyCode, event);
	}
	/**
	 * Intercepts all key releases in WebView and ignores or processes them now
	 * using this method instead of OnKeyListener method due to compatibility
	 * issues with the SoftInputMethod.
	 * 
	 * @author fpc843
	 */
	public boolean onKeyUp(int keyCode, KeyEvent event)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));

		/*
		 * Check whether there is an instance of the KeyCapturePlugin we can
		 * pass the key events to
		 */
		KeyCapturePlugin keyCapturer = (KeyCapturePlugin) Common.pluginManager
				.getPlugin("KeyCapture");
		if ((keyCapturer != null) && (KeyCapturePlugin.beingHere == false))
		{
			keyCapturer.onKey(null, keyCode, event);
		}
		KeyCapturePlugin.beingHere = false;
		return true;
	}

	/**
	 * Sets the user agent string for the WebView according to the pattern in
	 * the config file.
	 */
	private void setUserAgent()
	{
		// Set the user agent string
		String uaPattern = Common.config.getSetting("useragent");
		if (uaPattern != null && !uaPattern.equals(""))
		{
			String oldUserAgent = "";// webView.getSettings().getUserAgentString();
			Pattern androidPat = Pattern.compile("Android (.*?);");
			Pattern webkitPat = Pattern.compile("AppleWebKit/(.*?) ");
			Matcher androidMat = androidPat.matcher(oldUserAgent);
			Matcher webkitMat = webkitPat.matcher(oldUserAgent);
			String androidVer = null;
			String webkitVer = null;

			if (androidMat.find())
			{
				androidVer = androidMat.group(1);
			}
			if (webkitMat.find())
			{
				webkitVer = webkitMat.group(1);
			}

			// TODO add webkit version number here.
			String newUserAgent = uaPattern
					.replace("%p", "Android " + androidVer)/* platform name */
					.replace("%w", webkitVer)/* Webkit Version */
					.replace("%e", "NONE")/*Webkit Version */;

			// webView.getSettings().setUserAgentString(newUserAgent);
		}
	}


	private void startPluginManagerAndLogVersions()
	{
		// Create the plugin manager
		Common.pluginManager = new PluginManager();

		// Add the plugin module and class names
		Common.pluginManager.addName("Timer", "TimerPlugin");
		Common.pluginManager.addName("SignatureCapture", "SignaturePlugin");
		Common.pluginManager.addName("EMMLProfile", "ProfilePlugin");
		Common.pluginManager.addName("FileTransfer", "FileTransferPlugin");
		Common.pluginManager.addName("Scanner", "ScannerPlugin");
		Common.pluginManager.addName("Signal", "SignalPlugin");
		Common.pluginManager.addName("Battery", "BatteryPlugin");
		Common.pluginManager.addName("Network", "NetworkPlugin");
		Common.pluginManager.addName("ScreenOrientation", "OrientationPlugin");
		Common.pluginManager.addName("Application", "ApplicationPlugin");
		Common.pluginManager.addName("Imager", "ImagerPlugin");
		Common.pluginManager.addName("Volume", "VolumePlugin");
		Common.pluginManager.addName("Reboot", "RebootPlugin");
		Common.pluginManager.addName("Stylus", "StylusPlugin");
		Common.pluginManager.addName("TextSize", "TextSizePlugin");
		Common.pluginManager.addName("Alarm", "AlarmPlugin");
		Common.pluginManager.addName("Gesture", "GesturePlugin");
		Common.pluginManager.addName("Backlight", "BacklightPlugin");
		Common.pluginManager.addName("QuitButton", "QuitButtonPlugin");
		Common.pluginManager.addName("SettingsButton", "SettingsButtonPlugin");
		Common.pluginManager.addName("BackButton", "BackButtonPlugin");
		Common.pluginManager.addName("ForwardButton", "ForwardButtonPlugin");
		Common.pluginManager.addName("HomeButton", "HomeButtonPlugin");
		Common.pluginManager.addName("ReloadButton", "ReloadButtonPlugin");
		Common.pluginManager.addName("GoButton", "GoButtonPlugin");
		Common.pluginManager.addName("MinimizeButton", "MinimizeButtonPlugin");
		Common.pluginManager.addName("StopButton", "StopButtonPlugin");
		Common.pluginManager.addName("ZoomTextButton", "ZoomTextButtonPlugin");
		Common.pluginManager.addName("SIPButton", "SipButtonPlugin");
		Common.pluginManager.addName("AddressBar", "AddressBarPlugin");
		Common.pluginManager.addName("TopCommandArea", "TopCommandAreaPlugin");
		Common.pluginManager.addName("BottomCommandArea", "BottomCommandAreaPlugin");
		Common.pluginManager.addName("Notification", "NotificationPlugin");
		Common.pluginManager.addName("KeyCapture", "KeyCapturePlugin");
		Common.pluginManager.addName("Hourglass", "HourglassPlugin");
		Common.pluginManager.addName("PowerOn", "PowerOnPlugin");
		Common.pluginManager.addName("SIP", "SipPlugin");
		Common.pluginManager.addName("Zoom", "ZoomPlugin");
		Common.pluginManager.addName("MPM", "MPMPlugin");
		Common.pluginManager.addName("CardReader", "CardReaderPlugin");
		Common.pluginManager.addName("Device", "DevicePlugin");
		Common.pluginManager.addName("Wake", "WakePlugin");
		Common.pluginManager.addName("Push", "PushPlugin");
		Common.pluginManager.addName("LightSensor", "LightSensorPlugin");
		Common.pluginManager.addName("RawSensors", "RawSensorsPlugin");
		Common.pluginManager.addName("audioCapture", "AudioCapturePlugin");
		Common.pluginManager.addName("videoCapture", "VideoCapturePlugin");
		Common.pluginManager.addName("mediaPlayer", "MediaPlayerPlugin");

		// Write an entry to the log file for each component
		logVersions();
	}

	private void startSipSettings()
	{
		// Retrieve configuration for setting
		final String configSetting = Common.config
				.getSetting(Config.SETTING_SIP_ENABLE);

		Common.mainActivity.runOnUiThread(new Runnable() {

			@Override
			public void run()
			{
				Window window = Common.mainActivity.getWindow();

				if (configSetting == null
						|| configSetting.equalsIgnoreCase("1"))
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,
							"EnableSIP=1"));
					window.clearFlags(WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM);
				} else
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,
							"EnableSIP=0"));
					window.setFlags(
							WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM,
							WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM);
				}
			}
		});
	}

	private void startAutoRotate()
	{
		String configSetting = Common.config
				.getSetting(Config.SETTING_AUTOROTATE);

		// Turn auto-rotate off if requested in the config file
		if ((configSetting != null) && (configSetting.equalsIgnoreCase("0")))
		{
			Common.mainActivity
					.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_NOSENSOR);
		} else
		{
			Common.mainActivity
					.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
		}
	}

	private void startBadConfigDialog()
	{
		AlertDialog.Builder builder = new AlertDialog.Builder(
				Common.mainActivity);
		builder.setMessage(configErrorMessage)
				.setIcon(RhoExtManager.getResourceId("drawable","alert_dialog_icon"))
				.setTitle(RhoExtManager.getResourceId("string","config_error_dialog_title"))
				.setNegativeButton(RhoExtManager.getResourceId("string","alert_dialog_quit"), this);
		AlertDialog alert = builder.create();
		alert.setCancelable(false);
		alert.show();
	}

	private void startLogger()
	{
		// Create the logger and start it
		Common.logger = new Logger();
		Common.logger.start();

		// Log all messages in the message queue
		for (LogEntry log : mMessages)
		{
			Common.logger.add(log);
		}

		mMessages.clear();
		mMessages = null;
	}

	private boolean startConfig()
	{
		// Create the config object and read the config
		Common.config = new Config();
		boolean configPassed = true;

		try
		{
			mMessages.addAll(Common.config.load());
		} catch (ConfigException e1)
		{
			// Loading the config has failed and should close Neon after
			// starting the logger.
			// This allows all log entries to be written before exiting.
			configPassed = false;
			mMessages.addAll(e1.getLoggerMessages());
			configErrorMessage = e1.getMessage();
		}
		return configPassed;
	}

	private void startNPAPI()
	{
		try
		{
			// Create the NPAPI server - it will bind to a port and start a
			// listener thread
			npapiServer = new NPAPIServer();

			// Get the port it's bound to and pass it to the NPAPI plugin
			int port = npapiServer.getPort();
			setTcpPort(port);
		}
		catch (UnsatisfiedLinkError e) {
            Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "NPAPI is not implemented"));
		}
		catch (Exception e) {
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "NPAPI server failed to start: " + e.getMessage()));
		}

		// TODO Replace sleep with sync to NPAPIServer so we only launch the
		// WebView once the server is waiting
		/*
		 * try{Thread.sleep(2000, 0);} catch(Exception e){}
		 */

		// TODO Add code to gracefully shutdown NPAPIServer on app exit
	}

	private void startValidationCheck()
	{
		// Check if the device is a Symbol device
		ValidationChecker platformValidator = new ValidationChecker();
		platformValidator.validate();
	}

    private void startFullscreen() {
        Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
        
        // Retrieve full screen setting from config file
        final String configSetting = Common.config.getSetting(Config.SETTING_FULLSCREEN);

        if (configSetting != null && configSetting.equalsIgnoreCase("0")) {
            RhodesService.setFullscreen(false);
        }
        else {
            RhodesService.setFullscreen(true);
        }
    }

	/**
	 * Starts the Lock service, giving the main activity access to the service.
	 */
	private void startLockService()
	{

		try
		{
			mLockBindIntent = new Intent(Common.mainActivity, LockService.class);
			Common.mainActivity.startService(mLockBindIntent);
			// The service object might not have been created at this point, so
			// that will be
			// set to mLockService once the plugin starts setting the variables
		} catch (Exception e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR,
					"Unable to start the lock service"));
		}
	}

	/**
	 * Starts the scanner service, giving the main activity access to the
	 * service. This allows neon to correctly shutdown or pause the scanner
	 * service.
	 */
	private void startScannerService()
	{
		scannerEnabled = false;
		mScannerPaused = false;
		mScannerBindIntent = new Intent(Common.mainActivity,
				ScannerService.class);
		mScannerConnection = new ServiceConnection() {
			@Override
			public void onServiceConnected(ComponentName className,
					IBinder service)
			{
				// Returns us the service
				mScannerService = ((ScannerService.ScannerBinder) service)
						.getService();
			}

			@Override
			public void onServiceDisconnected(ComponentName className)
			{
				// TODO do cleanup on service crash
				// This will only happen if the service crashes
				mScannerService = null;
			}
		};
		Common.mainActivity.bindService(mScannerBindIntent, mScannerConnection,
				Activity.BIND_AUTO_CREATE);
	}

	/* **************************************************** */
	/* End start methods */
	/* **************************************************** */

	public void restoreState()
	{
		// Use saved state, if any, to restore the previous URL
		// Otherwise load the default start page
		try
		{
			// TODO Re-enable state restore
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,
					"State restoration disabled"));
			throw new Exception("State restoration disabled");

			/*
			 * String url = mSavedInstanceState.getString("url"); if (url ==
			 * null) throw new Exception();
			 * 
			 * webView.loadUrl(url);
			 * 
			 * // Reset the saved URL so if we quit by design we don't reload it
			 * on next start up mSavedInstanceState.putString("url", null);
			 * 
			 * Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,
			 * "Previous state restored"));
			 */
		} catch (Exception e)
		{
			loadStartPage();
		} finally
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "END"));
		}
	}

	public void loadStartPage()
	{
		String url;
		try
		{
			url = Common.parseAndroidURI(mStartPage).toString();
			navigate(url);
		} catch (InvalidParameterException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Bad "
					+ Config.SETTING_START_PAGE
					+ " value in Config.xml. Cannot find Start Page."));
		} catch (URISyntaxException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Malformed "
					+ Config.SETTING_START_PAGE
					+ " URL value in Config.xml. Cannot find Start Page."));
		} catch (RuntimeException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Exception "
					+ Config.SETTING_START_PAGE + e.getMessage()));
		}
	}

	
	/*new implementation for loading settings button*/
	
	public void loadSettingsPage()
	{   navigate(Environment.getExternalStorageDirectory()+"/Android/data/com.symbol.rhoelements/settings.html");
	    enableSettingsButton(false);	
	}
	/**
	 * Writes a log entry for a Version, including the build tag
	 * 
	 * @param version
	 *            The Version to log
	 */
	private void logVersion(Version version)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format(
				"%s %s %s", version.getModuleName(),
				version.getStringWithHotfix(), version.getBuildTag())));
	}

	/**
	 * Writes core and plugin versions to log file
	 */
	private void logVersions()
	{
		logVersion(Common.elementsCore.getVersion());

		for (Version version : Common.pluginManager.getVersions())
		{
			logVersion(version);
		}
	}

	/**
	 * Gets the indicator panel layer which holds the battery and signal
	 * indicators
	 * 
	 * @return The indicator panel layer
	 */
	public IndicatorPanel getIndicatorPanel()
	{
		return (IndicatorPanel) Common.mainActivity
				.findViewById(RhoExtManager.getResourceId("id","indicator_panel"));
	}

	private void shutdown()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		// Log that we're quitting and encourage the log thread to notice before
		// we stop the logger
		// The logger won't stop while a log write is in progress
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));

		// Close down NPAPI server
		if (npapiServer != null)
			npapiServer.close();

		// Shut down media player
		stopMediaPlayer();

		// Stop media play
		onStopMedia();

		// Shut down all loaded plugins
		/*
		 * The null check on the plugin manager is because the program might get
		 * here after the user has pressed quit from the platform validation
		 * dialog. In such case the plugin manager is not initialized and there
		 * would be a null pointer exception thrown.
		 */
		if (Common.pluginManager != null)
			Common.pluginManager.shutdown();


		// Lock Service
		// Release the wakeLock & wifiLock
		try
		{
			if (mLockService != null)
			{
				mLockService.setWakeLockEnabled(false);
				mLockService.setWifiLockEnabled(false);
			}
			Common.mainActivity.stopService(mLockBindIntent);
			mLockService = null;
			// Enable Android Lock Screen
			// Not sure if this is needed as the device immediately goes to Lock
			// Screen
			// mLockService.setKeyguardLockEnabled(true);
		} catch (IllegalArgumentException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,
					"Lock service wasn't created"));
		} catch (Exception e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR,
					"Unable to set the lock service"));
		}

		// Stop the scanner service
		try
		{
			Common.mainActivity.unbindService(mScannerConnection);
			Common.mainActivity.stopService(mScannerBindIntent);
		} catch (IllegalArgumentException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,
					"Scanner connection wasn't created"));
		}

		// TODO : Log thread does not stop if there are too many pending write
		// entries
		// e.g. if you have specified an invalid IP address for the HTTP logger
		if (Common.logger != null)
		{
			Common.logger.stop();
		}

		// Destroys the webview so Javascript doesnt bleed into the next
		// instantiation.
		// TODO check that this wont cause problems, because in theory the
		// webView should be removed from view first.
		//if (mWebView != null)
		//{
		    // Commented out because causes crash since GoogleWebView is already paused at this moment
			//mWebView.stopLoad();
			// webView.destroy();
			//mWebView = null;
		//}

		if (localWebServer != null)
		{
			Log.d("lw","localWebServer="+localWebServer);
			localWebServer.stop();
			localWebServer = null;
		}

		if (Common.logger != null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "END"));
		}
	}

	@Override
	public void onDestroy(Activity activity)
	{
		if (Common.logger != null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		}

		if (!isShuttingDown)
		{
			shutdown();
			isShuttingDown = true;
		}

		if (Common.logger != null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "END"));
		}
	}

	@Override
	public void onPause(Activity activity)
	{
		if (Common.logger != null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		}

		// Inform all the plugins with the background event
		Common.metaReceiver.focusEvent(MetaReceiver.focusEvent.ON_BACKGROUND, "");

		// Use onPause with an isFinishing check instead of onDestroy,
		// since onDestroy isn't always called by Android
		if (Common.mainActivity.isFinishing())
		{
			if (Common.logger != null)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,
						"isFinishing"));
			}

			if (!isShuttingDown)
			{
				shutdown();
				isShuttingDown = true;
			}
		} 
		else
		{
			if (mScannerService != null)
				mScannerPaused = mScannerService.pauseScanner();
			if (Common.pluginManager != null)
				Common.pluginManager.pause();
		}

		if (Common.logger != null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "END"));
		}
	}

	public void onStart(Activity activity)
	{
		if (Common.logger != null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		}
	}

	@Override
	public void onResume(Activity activity)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));

		// Inform all the plugins with the foreground event
		Common.metaReceiver.focusEvent(MetaReceiver.focusEvent.ON_FOREGROUND,
				"");

		if (Common.pluginManager != null)
			Common.pluginManager.resume();
		if (mScannerService != null && mScannerPaused)
		{
			mScannerPaused = false;
			mScannerService.resumeScanner();
		}

		// Send the restore event to the Application plugin
		Intent msgIntent = new Intent(ApplicationPlugin.APPLICATION_ACTION);
		msgIntent.putExtra(ApplicationPlugin.APPLICATION_METHOD,
				ApplicationPlugin.ApplicationEvents.RESTORED.ordinal());
		Common.mainActivity.sendBroadcast(msgIntent);

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "END"));
	}

	@Override
	public void onStop(Activity activity)
	{
		// TODO Auto-generated method stub
	}

	// Save current state on exit
	public void onSaveInstanceState(Bundle bundle)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		if (getWebViewInstance() != null)
		{
			bundle.putString("url", getCurrentUrl());
		}
	}

	/**
	 * Implement INeonCore interface
	 */
	@Override
	public void navigate(String url)
	{
		// Ignore empty URL
		if (url == null || url == "")
			return;

		String parsedURL = null;

		try
		{
			parsedURL = Common.parseAndroidURI(url, false, getWebViewInstance().getUrl()).toString();
		} 
		catch (InvalidParameterException e)
		{
			// Will never happen
			parsedURL = url;
		} 
		catch (URISyntaxException e)
		{
			parsedURL = url; // can happen if not a normal URI (i.e a Javascript call)
		}

		// Ensure any initial javascript: is in lowercase
		parsedURL = parsedURL.replaceFirst("^(?i)javascript:", "javascript:");

        com.rhomobile.rhodes.Logger.T(TAG, "parsed URL:" + parsedURL);

		/**
		 * Check whether the user activity is RhoElements or Rhodes if it's
		 * Rhodes (RE2), we send an intent to the RE extension with the url
		 */
		if (Common.isRhodes)
		{
			Intent intent = new Intent(RHOMOBILE_NAVIGATE_ACTION);
			intent.putExtra("url", parsedURL);
			intent.putExtra("process_id", android.os.Process.myPid());
			Common.mainActivity.sendBroadcast(intent);
		} 
		else if (getWebViewInstance() != null)
			getWebViewInstance().loadUrl(parsedURL);
	}

	@Override
	public String getCurrentUrl()
	{
		return webViewLastEnd;
	}

	@Override
	public Version getVersion()
	{
		// By using this Version constructor we get the project-wide version
		// numbers and build time
		return (new Version("Core"));
	}

	public String getLastUrl()
	{
		return webViewLastStart;
	}

	public void browserReload()
	{
		try
		{
			getWebViewInstance().reload();
		} 
		catch (Exception e)
		{
			// Ignore exception, can only be because engine not yet loaded
		}
	}

	public void browserStop()
	{
		try
		{
			// Instead of calling stop() just show bad link page
			showBadLink(BadLinkReason.STOPPED);
		} 
		catch (Exception e)
		{
			// Ignore exception, can only be because engine not yet loaded
		}
	}

	public void browserBack()
	{
		try
		{
			getWebViewInstance().goBack();
		} 
		catch (Exception e)
		{
			// Ignore exception, can only be because engine not yet loaded
		}
	}

	public void browserForward()
	{
		try
		{
			getWebViewInstance().goForward();
		} 
		catch (Exception e)
		{
			// Ignore exception, can only be because engine not yet loaded
		}
	}

	public void browserHome()
	{
		loadStartPage();
	}

	public void browserSetZoom(double factor)
	{
		try
		{
			getWebViewInstance().setZoom(factor);
		} 
		catch (Exception e)
		{
			// Ignore exception, can only be because engine not yet loaded
		}
	}

	/**
	 * Set the text zoom factor
	 * 
	 * @param textZoom
	 *            Value from 0 (smallest) to 4 (largest)
	 */
	public boolean browserSetTextZoom(int textZoom)
	{
		switch (textZoom)
		{
		case 0:
			getWebViewInstance().setTextZoom(IRhoWebView.TextZoom.SMALLEST);
			break;
		case 1:
			getWebViewInstance().setTextZoom(IRhoWebView.TextZoom.SMALLER);
			break;
		case 2:
			getWebViewInstance().setTextZoom(IRhoWebView.TextZoom.NORMAL);
			break;
		case 3:
			getWebViewInstance().setTextZoom(IRhoWebView.TextZoom.LARGER);
			break;
		case 4:
			getWebViewInstance().setTextZoom(IRhoWebView.TextZoom.LARGEST);
			break;
		default:
			getWebViewInstance().setTextZoom(IRhoWebView.TextZoom.NORMAL);
			break;
		}
		return true;
	}

	/**
	 * Sets the flag to tell whether the scanner is enabled.
	 * 
	 * @param scannerEnabled
	 *            whether the scanner is enabled.
	 */
	public void setScannerEnabled(boolean scannerEnabled)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,
				"scannerEnabled: " + scannerEnabled));
		this.scannerEnabled = scannerEnabled;
	}

	
	class UrlTimerTask extends TimerTask
	{
		public void run()
		{
			mNavigationHandler.post(new Runnable() {
				public void run()
				{
					try
					{
						com.rhomobile.rhodes.Logger.I("ElementsCore", "Page timeout");
						getWebViewInstance().stopLoad();
						showBadLink(BadLinkReason.TIMEOUT);
					} catch (NullPointerException e)
					{
					    com.rhomobile.rhodes.Logger.E("ElementsCore", e);
					}
				}
			});
		}
	}

	@Override
	public void onClick(DialogInterface dialog, int buttonId)
	{
		if (buttonId == DialogInterface.BUTTON_POSITIVE)
			initializeActivity();
		else
			quit();
		dialog.cancel();

	}

	public Dialog onCreateDialog(int id)
	{
		Dialog dialog;
		switch (id)
		{
		case APD_WAIT_DIALOG:
		{
			dialog = new ProgressDialog(Common.mainActivity);
			dialog.setTitle(RhoExtManager.getResourceId("string","apd_title"));
			((ProgressDialog) dialog).setMessage(Common.mainActivity
					.getResources().getString(RhoExtManager.getResourceId("string","apd_connecting")));
			((ProgressDialog) dialog)
					.setProgressStyle(ProgressDialog.STYLE_SPINNER);
			dialog.setCancelable(false);
			break;
		}
		case APD_PROGRESS_DIALOG:
		{
			dialog = mProgressDialog = new ProgressDialog(Common.mainActivity);
			dialog.setTitle(RhoExtManager.getResourceId("string","apd_title"));
			((ProgressDialog) dialog).setMessage(Common.mainActivity
					.getResources().getString(RhoExtManager.getResourceId("string","apd_printing")));
			((ProgressDialog) dialog)
					.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
			((ProgressDialog) dialog).setMax(100);
			dialog.setCancelable(false);
			((ProgressDialog) dialog).setProgress(0);
			break;
		}
		default:
			dialog = null;
		}
		return dialog;
	}

	public void quit()
	{   
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
		if (!Common.isRhodes)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,
					"Quitting from RhoElements"));
			Common.mainActivity.finish();
		} else
		{
			// Send Rhodes the intent to quit
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG,
					"Quitting from Rhodes...sending quit intent"));
			Intent intent = new Intent(RHOMOBILE_QUIT_ACTION);
			intent.putExtra("process_id", android.os.Process.myPid());
			Common.mainActivity.sendBroadcast(intent);
		}
	  
	}


	/**
	 * Method returns an instance of webview for use by plugins
	 */
	public IRhoWebView getWebViewInstance()
	{
		//return mWebView; //BenKen
		return RhoExtManager.getInstance().getWebView();
	}

	/**
	 * Return the reference to the progress dialog
	 * 
	 * @return
	 */
	public ProgressDialog getProgressDialog()
	{
		return mProgressDialog;
	}
	
	
	public void enableSettingsButton(boolean enabled)
	{
		String visibility = (enabled) ? "visible" : "hidden";
		Common.metaReceiver.setMeta("SettingsButton", "Visibility:" + visibility);
	}

	private void enableDebugButtons(boolean enabled)
	{
		String visibility = (enabled) ? "visible" : "hidden";
		// Common.metaReceiver.setMeta("TopCommandArea", "height:120;Visibility:" + visibility);
		// Common.metaReceiver.setMeta("BottomCommandArea", "Visibility:" + visibility);
		Common.metaReceiver.setMeta("QuitButton", "Visibility:" + visibility);
		Common.metaReceiver.setMeta("BackButton", "Visibility:" + visibility);
		Common.metaReceiver.setMeta("ForwardButton", "Visibility:" + visibility);
		Common.metaReceiver.setMeta("HomeButton", "Visibility:" + visibility);
		Common.metaReceiver.setMeta("ReloadButton", "Visibility:" + visibility);
		Common.metaReceiver.setMeta("ZoomTextButton", "Visibility:" + visibility);
		Common.metaReceiver.setMeta("SIPButton", "Visibility:" + visibility);
		Common.metaReceiver.setMeta("GoButton", "Visibility:" + visibility);
		Common.metaReceiver.setMeta("StopButton", "Visibility:" + visibility);
		Common.metaReceiver.setMeta("MinimizeButton", "Visibility:" + visibility);
		Common.metaReceiver.setMeta("AddressBar", "top:50;visibility:" + visibility);
	}

	private void setBadLink()
	{
		badLinkUri = null;

		// Get config setting
		String setting = Common.config.getSetting(Config.SETTING_BADLINK_FILE);

		// If there is a setting then try converting to URI, warn otherwise
		if (setting == null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,
					"Bad link setting missing from configuration"));
		} 
		else
		{
			try
			{
				badLinkUri = Common.parseAndroidURI(setting);
			} catch (URISyntaxException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,
						"Bad link URI invalid: " + setting));
				badLinkUri = null;
			}
		}

		// If bad link is still null then use the default
		if (badLinkUri == null)
			try
			{
				badLinkUri = new URI("file://" + Common.getDataPath()
						+ "/badlink.html");
			} 
			catch (Exception e)
			{
			}

		// Check that the bad link we've ended up with exists
		if (!new File(badLinkUri).exists())
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR,
					"Bad link URI not found: " + badLinkUri.toString()));

			// If it doesn't then set back to null so we don;t bother trying to
			// navigate to it later
			badLinkUri = null;
		}
	}

	/*************************************************************************/
	public void showBadLink(BadLinkReason reason)
	{
		// Check there's a bad link to navigate to
		if (badLinkUri == null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR,
					"No valid bad link set"));
			return;
		}

		if (m_pageLoadTimer != null)
			m_pageLoadTimer.cancel();

		String badLinkUrl = badLinkUri + "?BadLink="
				+ Uri.encode(webViewLastStart);

		switch (reason)
		{
		case STOPPED:
			badLinkUrl += "&Stopped=true";
			break;

		case TIMEOUT:
			badLinkUrl += "&Timeout=true";
			break;
		case UNLICENSED:
			badLinkUrl += "&Unlicensed=true";
			break;
		case ERROR:
			// Nothing to add
			break;
		}

		if (getWebViewInstance() != null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "url: "
					+ badLinkUrl));
			getWebViewInstance().loadUrl(badLinkUrl);
		}
	}
	
	@SuppressWarnings("deprecation")
	public void onInputMethod(IRhoWebView view, 
			boolean enabled, 
			String type,
			Rect area)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String
				.valueOf(enabled)));	
		
		InputMethodManager imm = (InputMethodManager) Common.mainActivity
				.getSystemService(Context.INPUT_METHOD_SERVICE);	
		
		if (SipPlugin.currentSipMode == SipMode.AUTOMATIC) {
			if (enabled)
			{				
				//TODO: Need to add logic for other input types as well
				
				//Logic for displaying Numeric keyboard as the current method
				//always defaults to Alphanumeric keyboard
				if (type.equalsIgnoreCase("number") ||
					type.equalsIgnoreCase("tel"))
				{
					if (mWebEditText == null)
					{
					    mWebEditText = new WebEditText(Common.mainActivity, view.getView());
					}
					
					mWebEditText.setInputType(InputType.TYPE_CLASS_NUMBER);
					mWebEditText.setTransformationMethod(PasswordTransformationMethod.getInstance());
					mWebEditText.setImeOptions(EditorInfo.IME_ACTION_DONE);
				    
				    AbsoluteLayout webEditTextPanel = (AbsoluteLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","editext_panel"));
					webEditTextPanel.removeAllViews();					
					
					//Create the control off the screen since this is hidden					
					int width = Common.mainActivity.getWindowManager().getDefaultDisplay().getWidth();
					int height = Common.mainActivity.getWindowManager().getDefaultDisplay().getHeight();
					AbsoluteLayout.LayoutParams lp = new AbsoluteLayout.LayoutParams(area.width(), area.height(), -200, -200);					
					webEditTextPanel.addView(mWebEditText, lp);
				    
					//TODO: Need to populate mWebEditText with any pre-allocated value through the "value" tag
					//but how do we get that value? We dont know the Id or tag or name or class.
					//mWebEditText.setText(id.value);
					mWebEditText.setVisibility(View.VISIBLE); //Need to be visible for keyboard to pop up	
					mWebEditText.setCursorVisible(false); //Otherwise an extra cursor gets displayed
					mWebEditText.setTextSize((float)0.001); //Make it invisible
					mWebEditText.setBackgroundColor(0x00FFFFFF); // Make it invisible
					mWebEditText.setTextColor(0x00FFFFFF);		
					mWebEditText.requestFocus(); //Otherwise the keyboard wont get displayed	
					
					imm.showSoftInput(mWebEditText, 0);	
				}
				else
				{
					//Hide the edit text box
					if (mWebEditText != null)
					{
						mWebEditText.setVisibility(View.INVISIBLE);	
						mWebEditText = null;
					}
					imm.showSoftInput(view.getView(), 0);
				}						
			}
			else
			{				
				//Hide the numeric keyboard 
				if (mWebEditText != null)
				{
					imm.hideSoftInputFromWindow(mWebEditText.getWindowToken(), 0);		
					//Hide the edit text box
					mWebEditText.setVisibility(View.INVISIBLE);	
					mWebEditText = null;
				}				
				imm.hideSoftInputFromWindow(view.getView().getApplicationWindowToken(), 0);	
			}
		}
	}

	/**
	 * @return the scannerEnabled
	 */
	public boolean isScannerEnabled()
	{
		return this.scannerEnabled;
	}

	/**
	 * Interface for the plugins to set the wifilock from Plugins
	 * 
	 * @param bEnabled
	 *            - To Enable/disable
	 */
	public void setWifiLock(boolean bEnabled)
	{
		try
		{
			// Initialize the Lock service
			if (mLockService == null)
			{
				mLockService = LockService.getInstance();
			}
			if (mLockService != null)
			{
				mLockService.setWifiLockEnabled(bEnabled);
			}
		} catch (Exception e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR,
					"Unable to set the lock service"));
		}
	}

	/**
	 * Interface for the plugins to set the wakelock from Plugins
	 * 
	 * @param bEnabled
	 *            - To Enable/disable
	 */
	public void setWakeLock(boolean bEnabled)
	{
		// Initialize the Lock service
		try
		{
			if (mLockService == null)
			{
				mLockService = LockService.getInstance();
			}
			if (mLockService != null)
			{

				mLockService.setWakeLockEnabled(bEnabled);
			}
		} catch (Exception e)
		{
			e.printStackTrace();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR,
					"Unable to set the lock service"));
		}
	}

	/*
	 * public void setKeyguradLock(boolean bEnabled) { // Initialize the Lock
	 * service try { if (mLockService == null) { mLockService =
	 * LockService.getInstance(); } if (mLockService != null) {
	 * mLockService.setKeyguardLockEnabled(bEnabled); } } catch (Exception e) {
	 * e.printStackTrace(); Common.logger.add(new
	 * LogEntry(LogEntry.PB_LOG_ERROR, "Unable to set the lock service")); }
	 * 
	 * }
	 */

	/*************************************************************************/
	/**
	 * Send the captured video buffer to the video capture plugin
	 * 
	 * @param intentData
	 */
	public void writeVideoCapture(Uri intentData)
	{
		VideoCapturePlugin videoCapturer = (VideoCapturePlugin) Common.pluginManager.getPlugin("VideoCapture");
		if (videoCapturer != null)
		{
			videoCapturer.stop(intentData);
		}
	}

	/*************************************************************************/
	public void onLoadError(IRhoWebView view)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));

        if (m_pageLoadTimer != null)
            m_pageLoadTimer.cancel();

        showBadLink(BadLinkReason.ERROR);
	}

	/*************************************************************************/
	public void onLoadStart(IRhoWebView webView, String url, long arg1, long arg2)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, url));

		if (mIsFirstTime)
		{
			startHourglass();
			startDebugButtons();
			startSettingsButton();
			mIsFirstTime = false;
		}

		// Clear page title
		webViewTitle = "";

		// Set most recently started URL
		webViewLastStart = url;

		// Clear most recently ended URL
		webViewLastEnd = "";

        if (m_pageLoadTimer != null)
            m_pageLoadTimer.cancel();

        // Check for history:back
		if (HISTORY_BACK_HREF.equalsIgnoreCase(url))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "history:back detected"));

			// Got it, so call back() instead
			browserBack();
		} else
		{
		    if (m_navigationTimeout > 0) {
                m_pageLoadTimer = new Timer();
                m_pageLoadTimer.schedule(new UrlTimerTask(), m_navigationTimeout);
		    } else {
		        m_pageLoadTimer = null;
		    }

            // To match the behavior in Windows continue the Media play even if the
            // page is navigated
            // stopMediaPlayer();
            Common.metaReceiver.pageEvent(MetaReceiver.PageEvent.PAGE_STARTED, "");

            Config.applyDefaultMetaTags();  
		}
	}

	/*************************************************************************/
	public void onLoadProgress(IRhoWebView webView, long arg1, long arg2)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("onLoadProgress: %d/%d", arg1, arg2)));

        if (m_pageLoadTimer != null)
            m_pageLoadTimer.cancel();
	}

	/*************************************************************************/
	public void onLoadEnd(IRhoWebView webView, String url, long arg1, long arg2)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, url));
		// Set most recently ended URL
		webViewLastEnd = url;

        // Update the address bar history if it's instantiated
        /*
         * if (Common.pluginManager.getPlugin("AddressBar") != null)
         * ((AddressBarPlugin)
         * (Common.pluginManager.getPlugin("AddressBar"))).loadWebHistory();
         */

        if (m_pageLoadTimer != null)
            m_pageLoadTimer.cancel();

        Common.metaReceiver.pageEvent(MetaReceiver.PageEvent.PAGE_FINISHED, "");

	}

	/**
	 * @return the webViewLastStart
	 */
	public String getWebViewLastStart()
	{
		return this.webViewLastStart;
	}

	/**
	 * @return the webViewLastEnd
	 */
	public String getWebViewLastEnd()
	{
		return this.webViewLastEnd;
	}

	public void onPlayMedia(IRhoWebView view, String urlArg)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Playing the media file: " + urlArg ));
		final String url = urlArg;
		if(urlArg!=null && urlArg.isEmpty())
			return;
		try
		{
			mVideoView = new RhoVideoView(Common.mainActivity.getApplicationContext());
			mVideoView.setZOrderMediaOverlay(true); // Stops WebView from
													// trying to show ontop (as
													// its presumeably a
													// SurfaceView)
			mVideoView.setClickable(true);
			mVideoView.setDimensions(1, 1);// This is required for stock browser. Otherwise we have to rotate and play.
			Common.mainActivity.addContentView(mVideoView,
					new ViewGroup.LayoutParams(
							ViewGroup.LayoutParams.WRAP_CONTENT,
							ViewGroup.LayoutParams.WRAP_CONTENT));

			// video finish listener
			mVideoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {

						// Cleanup function once the video is played
						@Override
						public void onCompletion(MediaPlayer mp)
						{
							mVideoView.stopPlayback();
							mVideoView.clearFocus();
							// This is the only way to refresh the screen
							// Android doesn't support removing a view once it
							// is created\
							// so hiding is the only option
							mVideoView.setVisibility(View.GONE);
							// Common.mainActivity.setContentView(view);
						}
					});

			// For now full screen view
			android.widget.FrameLayout.LayoutParams vvLayoutParams = (android.widget.FrameLayout.LayoutParams) mVideoView.getLayoutParams();
			vvLayoutParams.gravity = 0x11;
			mVideoView.setLayoutParams(vvLayoutParams);

			// Play URL
			mVideoView.setVideoURI(Uri.parse(url));
			mVideoView.setMediaController(new MediaController(Common.mainActivity));
			mVideoView.requestFocus();
			final HourglassPlugin hourglass = new HourglassPlugin();
			hourglass.show();

			mVideoView
					.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {

						@Override
						public void onPrepared(MediaPlayer mp)
						{

							// The video is ready to be played
							if (mp.getVideoWidth() > 0
									&& mp.getVideoHeight() > 0)
							{
								// We got a size, resize and start the video.
								mVideoView.resizeMedia(mp.getVideoWidth(),
										mp.getVideoHeight());
								mVideoView.start();
								hourglass.hide();
							}
						}
					});

			mVideoView.setOnErrorListener(new MediaPlayer.OnErrorListener() {

				@Override
				public boolean onError(MediaPlayer mp, int what, int extra)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,
							"Unable to play the media file: " + url
									+ " Error: " + what));
					if (mVideoView != null)
					{
						mVideoView.clearFocus();
						mVideoView.setVisibility(View.GONE);
						hourglass.hide();
					}
					return true;
				}
			});

			// Another way to start the video without OnPrepareListener
			// mVideoView.start();
		} 
		catch (BadTokenException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,
					"Unable to play the media file: " + url));
		} 
		catch (Exception e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR,
					"Unable to set the lock service"));
		}
	}

	/**
	 * Stops the media file that is being played actively
	 */
	public void onStopMedia()
	{
		if ((mVideoView != null) && mVideoView.isPlaying())
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO,
					"Stopping media on Back button"));
			mVideoView.stopPlayback();
			mVideoView.clearFocus();
			mVideoView.setVisibility(View.GONE);
			mVideoView = null;
		}
	}

	/**
	 * Clears up the media player created by Generic.PlayWave.
	 */
	public void stopMediaPlayer()
	{
		// Shut down media player
		if (Common.mediaPlayer != null)
		{
			Generic.getInstance().clearSoundQueue();
			try
			{
				Common.mediaPlayer.stop();
			} 
			catch (IllegalStateException e)
			{
				try
				{
					Common.mediaPlayer.release();
				} 
				catch (IllegalStateException x)
				{/* Do Nothing */
				}
				Common.mediaPlayer = null;
				Common.logger
						.add(new LogEntry(LogEntry.PB_LOG_WARNING,
								"MediaPlayer was in an illegal state but has been cleaned up"));
			}
		}
	}

	/**
	 * Video player should detect the orientation change and resize accordingly
	 */
	public void onConfigurationChanged(Configuration newConfig)
	{
		// Resize the video
		if (mVideoView != null)
		{
			mVideoView.resizeMedia(-1, -1);
		}
	}
	
	public void startLocationUpdates(View view, boolean highAccuracy)
	{
		//GeolocationWrapper.getInstance().startLocationUpdate(view, highAccuracy);
	}

	public void stopLocationUpdates(View view)
	{
		//GeolocationWrapper.getInstance().stopLocationUpdate(view);
	}
	public void unRegisterCustomReceiver()
	{
		if(RECEIVER_REGISTER_STATUS == 1){
    		try {
        		RhodesActivity.safeGetInstance().getApplicationContext().unregisterReceiver(mReceiver);
        		RECEIVER_REGISTER_STATUS =0;
    		} catch (IllegalArgumentException e) {
    		}
    	}
	}
	public void RegisterCustomReciever(IRhoExtManager extManager, IRhoConfig config, String name, boolean res)
	{   
		if (config.isExist("enablereceiver") && name.equalsIgnoreCase("rhoelementsext") && RECEIVER_REGISTER_STATUS == 0) {
			String EnableReceiver = config.getString("enablereceiver");
			int reg_receiver = 0;
			IntentFilter intentfilter = new IntentFilter();
			 mReceiver = new IntentReceiver();
			if (EnableReceiver != null) {
				reg_receiver = Integer.parseInt(EnableReceiver);
				if(reg_receiver == 1){
					String externalSharedPath = Environment.getExternalStorageDirectory().toString();
                    String configPath = new File(externalSharedPath, "/Android/data/com.symbol.rhoelements/Config.xml").getAbsolutePath();
                    File fXmlFile = new File(configPath);
                    DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
                    DocumentBuilder dBuilder = null;
                    Document doc = null;
					try {
						dBuilder = dbFactory.newDocumentBuilder();
						doc = dBuilder.parse(fXmlFile);
					} catch (ParserConfigurationException e1) {
						e1.printStackTrace();
					} catch (SAXException e) {
						e.printStackTrace();
					} catch (IOException e) {
						e.printStackTrace();
					}
					if ( doc != null )
					{
						doc.getDocumentElement().normalize();
						NodeList nList = doc.getElementsByTagName("IntentAction");
						for (int temp = 0; temp < nList.getLength(); temp++) {
							intentfilter.addAction(nList.item(temp).getAttributes().getNamedItem("value").getTextContent());
						}
						NodeList nList_cat = doc.getElementsByTagName("IntentCategory");
						for (int i = 0; i < nList_cat.getLength(); i++) {
							intentfilter.addCategory(nList_cat.item(i).getAttributes().getNamedItem("value").getTextContent());
						}
					}
			RhodesActivity.getContext().registerReceiver(mReceiver, intentfilter);
                    	RECEIVER_REGISTER_STATUS = 1;
				}
			}
			
		}
		
	}
	public void initKeyHashMap()
	{
		KeyCodeRemapper.init();
	} 

}
