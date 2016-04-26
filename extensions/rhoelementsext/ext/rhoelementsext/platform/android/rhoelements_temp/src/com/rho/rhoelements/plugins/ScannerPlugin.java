package com.rho.rhoelements.plugins;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;

import android.content.ComponentName;
import android.content.ServiceConnection;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.AsyncTask;
import android.os.Environment;
import android.os.IBinder;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;

import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.webview.GoogleWebView;
import com.rhomobile.rhodes.webview.WebViewConfig;
import com.rhomobile.rhodes.RhodesActivity;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.Config;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rho.rhoelements.services.ScannerService;
import com.rho.rhoelements.services.ScannerService.IScannerClient;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author Ben Kennedy (NCVT73) A Plugin class for barcode and symbol scanning. Once available, this
 *         plugin will be able to use laser, imager and camera scanners.
 */
public class ScannerPlugin extends Plugin implements IScannerClient
{
	private final static String[] NAMES = { "data", "source", "type", "time", "length", "event" }; //$NON-NLS-1$//$NON-NLS-2$//$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$

	private boolean enabled;
	private ScannerService scannerService;
	private String barcodeUrl;
	private ServiceConnection mConnection;
	private boolean mIsBound;
	private String enumUrl;
	private boolean autoTab;
	private boolean autoEnter;
	private boolean applyInProgress = false;
	private boolean mIsEnabled = false;
	private boolean m_bDisableDuringNavigation = true;
	private float batPercent;
	private boolean bolBroacastRegistred;
	// This Queue is just to store the settings until the scanner service is started
	private ArrayList<PluginSetting> settingQueue;
	// This Queue is to store all the settings that the user applied
	private EmmlSettingsQueue<PluginSetting> emmlSettingsQueue; 
	
	/*
	 * The scanner API doesn't call any callback if a start request fails (no scanning happens) We
	 * need to understand whether there is any redundant start request in the queue as if there is,
	 * we need to erase them to avoid generating not required start requests when, for instance,
	 * onForeground() is called.
	 */
	private int mFailedStartRequests = 0;
	private MediaPlayer mp = null;
	private FileInputStream fis = null;
	private File f = null;
	private String deviceName = null;

	/** Construct plugin with reference to core for navigation callback, etc. */
	public ScannerPlugin()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		reset();
		deviceName = android.os.Build.MODEL;
		enabled = false;
		mIsBound = false;
		settingQueue = new ArrayList<PluginSetting>();
		emmlSettingsQueue = new EmmlSettingsQueue<PluginSetting>();
		mConnection = new ServiceConnection()
		{
			@Override
			public void onServiceConnected(ComponentName className, IBinder service)
			{
				// Returns us the service
				scannerService = ((ScannerService.ScannerBinder) service).getService();
				registerClient();
				synchronized (settingQueue)
				{
					for (PluginSetting setting : settingQueue)
					{
						onSetting(setting);
					}
					settingQueue.clear();
				}
				mIsBound = true;
			}

			@Override
			public void onServiceDisconnected(ComponentName className)
			{
				// TODO do cleanup on service crash
				// This will only happen if the service crashes
				scannerService = null;
			}
		};

		
		if(!Common.isRebradnedDevice() && deviceName.equalsIgnoreCase("TC700H")){
						mp = new MediaPlayer();
						f = new File(Common.mainActivity.getExternalCacheDir(),"decodeWav.wav");
						try {
							fis = new FileInputStream(f);
							mp.setDataSource(fis.getFD());
							mp.setAudioStreamType(AudioManager.STREAM_MUSIC);
							mp.prepare();
						} catch (FileNotFoundException e) {
							e.printStackTrace();
						} catch (IllegalArgumentException e) {
							e.printStackTrace();
						} catch (IllegalStateException e) {
							e.printStackTrace();
						} catch (IOException e) {
							e.printStackTrace();
						}
					}
		boolean isBound = ScannerService.bind(mConnection);
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "end"));
	}
	
	private BroadcastReceiver batteryInfoReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {			
			int currentLevel = intent.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
	                 int scale = intent.getIntExtra(BatteryManager.EXTRA_SCALE, -1);
		             if (currentLevel >= 0 && scale > 0) {
		            	 batPercent = (currentLevel * 100) / scale;
		             }
		}
	};
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("Scanner"); //$NON-NLS-1$
	}

	/**
	 * Registers the ScannerPlugin with the scannerService
	 */
	protected void registerClient()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		scannerService.registerClient(this);
	}

	/**
	 * Resets the user defined scanner plugin settings.
	 */
	private void reset()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		autoEnter = false;
		autoTab = false;
		enumUrl = "";
		barcodeUrl = "";
		if (scannerService != null)
		{
			scannerService.clearSettings(this);
		}
		String szScannerDisabledDuringNavigation = Common.config.getSetting(Config.SETTING_SCANNER_DISABLE_DURING_NAV);
		if (szScannerDisabledDuringNavigation != null)
		{
			m_bDisableDuringNavigation = !(szScannerDisabledDuringNavigation.equals("0"));
		}
	}

	/** Method called when the App gets foreground focus event */
	@Override
	public void onForeground(String url)
	{
		// An Application focus change is received
		// Application has become active.
		// Reset to defaults
		if (scannerService != null)
		{
			applyInProgress = true;
			if (mFailedStartRequests > 0)
			{
				removeSettings("start");
				mFailedStartRequests = 0;
			}
			// The data is already saved in the member variables so apply all
			// the settings
			for (PluginSetting setting : emmlSettingsQueue)
			{
				onSetting(setting);
			}
			// emmlSettingsQueue.clear();
			applyInProgress = false;
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Application foreground event received by Scanner plugin"));

	}

	/** Method called when the App gets Background focus event */
	@Override
	public void onBackground(String url)
	{
		// An Application focus change is received
		// Application has gone into the background.
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Application background event received by Scanner plugin"));

		// Reset to defaults
		if (scannerService != null)
		{
			setEnabled(false, null);// Clears the parameters here
		}
	}

	@Override
	public void onScanEvent(String inputBarcode, String source, String type, String time, String length, String event)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Received Scan at: " + new Date().getTime()));
		mFailedStartRequests = 0;
		removeSettings("start"); // All the start request in the stack need to be cleaned up.
		if(!Common.isRebradnedDevice() && deviceName.equalsIgnoreCase("TC700H")){
						new PlayBeep().execute();
					}
		if (barcodeUrl != null && !barcodeUrl.equals(""))
		{
			try
			{
				navigate(barcodeUrl, NAMES, new String[] { inputBarcode, source, type, time, length, event });
			}
			catch (NavigateException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
			}
		}
		else
		{
			String barcode = inputBarcode;
			if (autoEnter)
			{
				// Check \n or \r\n
				barcode += "\n";
				if (autoTab)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "AutoTab ignored because AutoEnter is active"));
				}
			}
			else if (autoTab)
			{
				barcode += "\t";
			}

			// generate keystrokes
			Common.injectKeyEvents(barcode);
		}
	}
	
	class PlayBeep extends AsyncTask<String, Void, String> {
		
				@Override
				protected String doInBackground(String... params) {
					try {
						mp.start();
					} catch (IllegalArgumentException e) {
						e.printStackTrace();
					} catch (IllegalStateException e) {
						e.printStackTrace();
					}
					return null;
				}
		
			}

	/**
	 * Sends an enumeration of all available scanners to the current webpage.
	 */
	public void doEnumerateEvent()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		try
		{
			navigate(enumUrl, Common.mainActivity.getString(RhoExtManager.getResourceId("string", "scanner_array_name")),
					scannerService.enumerateScanners());
		}
		catch (NavigateException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
		}
		catch (NullPointerException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
		}
	}

	/**
	 * Unbinds the Scanner Service from this plugin and removes the settings from the service.
	 */
	private void doUnbindService()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		if (mIsBound)
		{
			// Remove our user entry
			scannerService.removeScannerUser(this, false);
			// Detach our existing connection.
			try
			{
				Common.mainActivity.getApplicationContext().unbindService(mConnection);
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Plugin unBound"));
			}
			catch (IllegalArgumentException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Did not unbind from scanning service."));
			}
			mIsBound = false;
		}
	}

	@Override
	public void onShutdown()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		setEnabled(false, null);
		doUnbindService();
		if (bolBroacastRegistred) {
			RhodesActivity.getContext().unregisterReceiver(batteryInfoReceiver);
			bolBroacastRegistred = false;
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	}

	@Override
	public void onPageStarted(String url)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, url));
		int disable_scanner_val = 0;
		IRhoConfig rhoelementsGetConfig= null;
        	IRhoWebView vi=RhodesActivity.safeGetInstance().getMainView().getWebView(0);
        	GoogleWebView web=(GoogleWebView)vi;
        	rhoelementsGetConfig=web.getConfig();
        	if(rhoelementsGetConfig.getString(WebViewConfig.DISABLE_SCANNER_ON_NAVIGATION) != null){
        		disable_scanner_val = Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.DISABLE_SCANNER_ON_NAVIGATION));
        	}
		if(disable_scanner_val ==1){
		if(mIsEnabled == true)
		{
			mIsEnabled = false;
			setEnabled(false, null);
		}
		if(!barcodeUrl.isEmpty())
		{
			barcodeUrl="";
		}
		// Clear all the settings as they are non persistent
		emmlSettingsQueue.clear();
		// if (m_bDisableDuringNavigation)
		// {
		// mFailedStartRequests = 0;
		// reset();
		// settingQueue.clear();
		// // Clear all the settings as they are non persistent
		// emmlSettingsQueue.clear();
		// if (scannerService != null)
		// {
		// setEnabled(false, null);// Clears the parameters here
		// }
		// else
		// {
		// onSetting(new PluginSetting("scanner", "disable", null));
		// }
		// }
		}
	}

	/**
	 * This method has the main purpose to rid of redundant calls to enable and disable. For
	 * instance, if there is a chain of calls to enable and disable, only the last call is really
	 * significative, so the previous ones get deleted from the list. This is needed because when an
	 * application is resumed, all the settings in the list are applied, and a chain of
	 * enable/disable calls has a disastrous impact on performances
	 * @param settingName the name of the settings to delete.
	 */
	private void removeSettings(String settingName)
	{
		Iterator<PluginSetting> it = emmlSettingsQueue.iterator();
		while (it.hasNext())
		{
			PluginSetting setting = it.next();
			if (setting.getName().equalsIgnoreCase(settingName))
			{
				it.remove();
			}
		}
	}

	@Override
	public void onSetting(PluginSetting setting)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		bolBroacastRegistred = true;
		RhodesActivity.getContext().registerReceiver(this.batteryInfoReceiver,	new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
		int scanOrNot = 1;
		try
		{	IRhoConfig rhoelementsGetConfig= null;
			IRhoWebView vi=RhodesActivity.safeGetInstance().getMainView().getWebView(0);
			GoogleWebView web=(GoogleWebView)vi;
			rhoelementsGetConfig=web.getConfig();
			if(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_SCAN) != null){
				scanOrNot = Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_SCAN));
			}
		}
		catch(NumberFormatException exp){
			
		}
		// If the scannerService hasnt been started then some EMML calls will crash with a NullPointerException
		// This stores up the settings until they can be used.
		// This way, rogue tags wont bleed over into new pages if scannerService is taking some time.
		if (setting == null)
			return;
		if (scannerService == null)
		{
			settingQueue.add(setting);
			return;
		}
		if (!applyInProgress)
		{
			// Add all the settings in the queue to apply later
			emmlSettingsQueue.add(setting);
		}

		// If we are not the foreground application then just store the settings
		// in a queue and apply later
		if (!Common.elementsCore.bLaunchingAppHasFocus) { return; }

		String name = setting.getName().toLowerCase();
		String value = setting.getValue();

		if (setting.getValue().length() > 0)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "'" + name + "', '" + value + "'"));
		else
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, name));

		if (name.equals("enable") || name.equals("enabled")) // Both because of javascript problems with NPAPI
		{
			mIsEnabled = true;
			setEnabled(true, value);
		}
		else if (name.equals("disabled") || name.equals("disable"))
		{
			mIsEnabled = false;
			setEnabled(false, null);
		}
		else if (name.equals("decodeevent"))
		{
			barcodeUrl = setting.getValue();
		}
		else if (name.equals("enumscannerevent"))
		{
			enumUrl = setting.getValue();
		}
		else if (name.equals("enumsupporteddecodersevent"))
		{
			// TODO do me. Also, what decoder strings want returning?? Neon, EMDK or both?!
			// List<String> decoderParams = params.getSupportedParameters("symbologies"); //<<Thats how to get the supported decoders.
		}
		else if (name.equals("enumerate"))
		{
			doEnumerateEvent();
		}
		else if (name.equals("start"))
		{
			if (enabled && (scanOrNot != 0 || batPercent > 5))
			{
				mFailedStartRequests++;
				scannerService.startScanner(this);
			}
		}
		else if (name.equals("stop"))
		{
			if (enabled)
			{
				scannerService.stopScanner(this);
			}
		}
		else if (name.equals("autoenter"))
		{
			if (barcodeUrl == null || barcodeUrl.equals(""))
			{
				
				if (value.equalsIgnoreCase("enable") || value.equalsIgnoreCase("enabled"))
				{
					autoEnter = true;
				}
				else if (value.equalsIgnoreCase("disabled"))
				{
					autoEnter = false;
				}
				else
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Bad AutoEnter value: " + value));
				}
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "AutoEnter cannot be set when DecodeEvent has been specified"));
			}
		}
		else if (name.equals("autotab"))
		{
			if (barcodeUrl == null || barcodeUrl.equals(""))
			{
				//Both because of javascript problems with NPAPI
				if (value.equalsIgnoreCase("enable") || value.equalsIgnoreCase("enabled")) 
				{
					autoTab = true;
				}
				else if (value.equalsIgnoreCase("disabled"))
				{
					autoTab = false;
				}
				else
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Bad AutoTab value: " + value));
				}
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "AutoTab cannot be set when DecodeEvent has been specified"));
			}
		}
		// //Commented out because its currently not supported. In case the scanner library starts to support it, let them deal with the call.
		// else if(name.equals("rastermode"))
		// {
		// 		Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, name + "\" is not supported on this platform."));
		// 		scannerService.setMode(this, "rastermode", value);//TODO
		// }
		else if (name.equals("aimtype"))
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, name + "\" is not supported on this platform."));
			// TODO
		}
		// else if(name.equals("picklistmode"))
		// {
		// 		if(value.equalsIgnoreCase("disabled"))
		// 		{
		// 			setDecoder("picklist","0");
		// 		}
		// 		else if(value.equalsIgnoreCase("hardwarereticle"))
		// 		{
		// 			setDecoder("picklist","1");
		// 		}
		//		else if(value.equalsIgnoreCase("softwarereticle"))
		// 		{
		// 			setDecoder("picklist","2");
		// 		}
		// 		else
		// 		{
		// 			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Bad PicklistMode value: " + value));
		// 		}
		// }
		else // Guess its a decoder
		{
			setDecoder(name, value.toLowerCase());
		}
	}

	/**
	 * Adds a single decoder setting to the scanner profile currently associated with this plugin.
	 * @param name The name of the decoder (for example ean13 or Code39)
	 * @param value The value of the decoder (i.e. enabled, disabled, true false, 0-59)
	 */
	private void setDecoder(String name, String value)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, name + ", " + value));
		scannerService.addDecodeSetting(this, name, value);
	}

	/**
	 * Changes whether the plugin is enabled or disabled.
	 * @param enabled whether the plugin is enabled (true) or disabled (false)
	 * @param value the scanner to enable, if empty enables the default scanner
	 */
	private void setEnabled(boolean enabled, String value)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		if (enabled)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Enabling Scanner at: " + new Date().getTime()));
			// scannerService.registerClient(this);
			if (value.length() > 0)
			{
				try
				{
					scannerService.setScanner(this, value);
				}
				catch (InvalidParameterException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid scanner selected. Using default scanner."));
					value = "";
				}
			}

			if (value.length() == 0)
			{
				try
				{
					scannerService.setScanner(this, scannerService.getDefaultScannerId());
				}
				catch (InvalidParameterException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Could not select default scanner."));
					return;
				}
			}
			scannerService.enableScanner(this);
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Scanner Enabled at: " + new Date().getTime()));
		}
		else // "Disabled" call
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Disabling Scanner at: " + new Date().getTime()));
			scannerService.disableScanner(this);
			// scannerService.removeScannerUser(this, true);
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Scanner Disabled at: " + new Date().getTime()));
		}

		this.enabled = enabled;
	}
	
	/**
	 * A special ArrayList which is used to store previously set PluginSettings
	 * so that they can be reapplied onForeground (as onBackground disables the 
	 * scanner and thus clears all the decoder/scanner settings).
	 * Calling this.add with a "disable|disabled" PluginSetting will clear the list.
	 * @author Ben Kennedy (NCVT73)
	 * @param <T> Must be extended from or instance of PluginSetting
	 */
	private class EmmlSettingsQueue<T extends PluginSetting> extends ArrayList<T>
	{
		private static final long serialVersionUID = -7394674321822018062L;

		@Override
		public boolean add(T setting)
		{
			String name = setting.getName();
			if(name.equalsIgnoreCase("disable") || name.equalsIgnoreCase("disabled"))
			{
				this.clear();
				return true;
			}
			return super.add(setting);
		}	
	}
}
