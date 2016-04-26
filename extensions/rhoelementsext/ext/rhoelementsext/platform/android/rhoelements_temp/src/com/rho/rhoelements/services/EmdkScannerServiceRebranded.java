package com.rho.rhoelements.services;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.net.URISyntaxException;
import java.security.InvalidParameterException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.net.Uri;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;




import com.symbol.scanning.ScanResult;
import com.symbol.scanning.Scanner;
import com.symbol.scanning.Scanner.DecodeCallback;
import com.symbol.scanning.Scanner.Parameters;
import com.symbol.scanning.Scanner.ScanEvent;
import com.symbol.scanning.Scanner.ScanEventCallback;
import com.symbol.scanning.Scanner.ScanStatus;
import com.symbol.scanning.Scanner.ScannerConnection;
import com.symbol.scanning.Scanner.ScannerExceptions;
import com.symbol.scanning.Scanner.ScannerInfo;



import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.common.ToneFileFactory;
import com.rho.rhoelements.plugins.Plugin;
import com.rho.rhoelements.services.ScannerService.IScannerClient;

/**
 * A service that controls the various types of scanners available on the device.
 * Is capable of having multiusers, the last user to enable a scanner is the one that controls
 * the scanner interface, once that user has finished with the scanner, the previous users
 * settings are restored and it regains control.
 * @author Ben Kennedy (NCVT73)
 */
@SuppressWarnings("deprecation")
public class EmdkScannerServiceRebranded implements IScannerImplementation, DecodeCallback, ScanEventCallback
{
	/**
	 * @author NCVT73
	 *
	 */
	public enum Type { INTEGER, BOOLEAN, STRING, DEPRECATED, UNSUPPORTED }
	public enum Task { ENABLE, DISABLE, START, STOP, SETPARAMS }
	/**
	 * The intent filter unique to the File Transfer plugin which we 
	 * will listen to to indicate that our transfer has completed.
	 */
	public static final String SCNR_SCAN_RECEIVED		= "com.rho.rhoelements.services.ScannerService.SCAN_RECEIVED";
	//public static final String SCANNER_TRIGGER_EVENT	= "com.rho.rhoelements.services.ScannerService.SCAN_TRIGGER";
	private static final String TAG_PARAM	= "param";
	private static final String TAG_TRANSLATE	= "translate";
	private static final String TAG_ALL_DECODERS = "all_decoders";
	public static final Object	TAG_ALL_DECODERS2	= "alldecoders";
	
	public static HashMap<String, Alias> aliasMap;
	
	private BroadcastReceiver triggerReceiver;

	// This is the object that receives interactions from clients.
	//private final IBinder mBinder = new ScannerBinder();

	private ArrayList<ScannerUser> userStack;
	private ArrayList<ScannerUser> openStack;

	//	private ScannerBroadcastReceiver	receiver;
	private Scanner scanner;
	private ScannerUser	pausedUser;
	
	private boolean isScanning;
	private StartScannerThread	startScannerThread;
	
	private LinkedBlockingQueue<ScannerInstruction> scannerInstructionQueue;
	private boolean	isShuttingDown;
	private String	scanDecodeWav;
	private boolean scannerActive;
	private Object globalLockObject;
	
	private Handler handler;
	
	private ScannerService mScannerService;
	
	/**
	 * This hashmap is used to map the barcode symbologies idendified by hex codes to the same description
	 * as on RE WinMo/WinCE
	 */
	private class WinScannerResult
	{
		String typeLabel;
		String typeCode;
		
		public WinScannerResult(String typeLabel, String typeCode)
		{
			this.typeLabel = typeLabel;
			this.typeCode = typeCode;
		}
	}
	public static HashMap<String, WinScannerResult> winTypeToString;
	

	/**
	 * Creates the scanner service.
	 */
	public EmdkScannerServiceRebranded(ScannerService scannerService)
	{
		Log.d("mc18","EmdkScannerServiceRebranded constructor");
	    mScannerService = scannerService;
	    scanDecodeWav = null;
		isShuttingDown = false;
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Creating user stack"));
		userStack = new ArrayList<ScannerUser>();
		openStack = new ArrayList<ScannerUser>();
		scannerInstructionQueue = new LinkedBlockingQueue<ScannerInstruction>();
		pausedUser = null;
		globalLockObject = new Object();
		if(aliasMap == null)
		{
			try
			{
				generateAliasMap();
			}
			catch (XmlPullParserException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Cannot Start the Scanner: Elements Parameter file corrupted"));
			}
			catch (IOException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Cannot Start the Scanner: Elements Parameter file corrupted"));
			}
		}
		handler = new Handler();
		startScannerThread = new StartScannerThread();
		startScannerThread.start();
		initWinCompatibilityTable();
	}
	
	/**
	 * Generates the mapping between Elements "alias" scan parameter names and EMDK scan parameter names.
	 * Also generates the mapping for String based values (i.e. none/auto/always). 
	 * @throws XmlPullParserException thrown if the scannerparams.xml is invalid
	 * @throws IOException thrown if the scannerparams.xml is invalid
	 */
	private void generateAliasMap() throws XmlPullParserException, IOException
	{
	    aliasMap = new HashMap<String, Alias>(150);
	    Alias currentAlias = null;
	    String currentAliasName = null;
	    String currentPbValue = null;
	    Integer currentEmdkIntValue = null;
	    String currentEmdkStringValue = null;
		
	    /* Removed due to RE2 R.java issues. Hardcoded instead above*/
	    // XmlResourceParser parser = Common.mainActivity.getResources().getXml(RhoExtManager.getResourceId("xml","scannerparams"));
		
	    //This is the hardcoded version of the parser above, the string involved is at the bottom of the class
	    XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
	    XmlPullParser parser = factory.newPullParser();
	    parser.setInput(new StringReader(scannerParamsDotXml));
	    //End
        
	    parser.next();
		
	    for(int eventType = parser.getEventType();eventType != XmlPullParser.END_DOCUMENT; eventType = parser.next())
	    {
		if(eventType == XmlPullParser.START_TAG)
		{
		    String tagName = parser.getName();
		    if(tagName.equals(TAG_PARAM))
		    {
			currentAliasName = parser.getAttributeValue(null, "alias");
			String id = parser.getAttributeValue(null, "id");
			Type type = parseType(parser.getAttributeValue(null, "type"));
			currentAlias = new Alias(id, type);
		    }
		    else if(tagName.equals(TAG_TRANSLATE))
		    {
			currentPbValue = parser.getAttributeValue(null, "id");
		    }
		}
		else if(eventType == XmlPullParser.END_TAG)
		{
		    String tagName = parser.getName();
		    if(tagName.equals(TAG_PARAM))
		    {
			aliasMap.put(currentAliasName, currentAlias);
			currentAlias = null;
			currentAliasName = null;
		    }
		    else if(tagName.equals(TAG_TRANSLATE))
		    {
			if(currentAlias.getType().equals(Type.INTEGER))
			{
			    currentAlias.addIntTranslation(currentPbValue, currentEmdkIntValue);
			}
			else if(currentAlias.getType().equals(Type.STRING))
			{
			    currentAlias.addStringTranslation(currentPbValue, currentEmdkStringValue);				
			}
			currentEmdkIntValue = null;
			currentEmdkStringValue = null;
			currentPbValue = null;
		    }
		}
		else if(eventType == XmlPullParser.TEXT)
		{
		    if(currentPbValue != null)
		    {
			if(currentAlias.getType().equals(Type.INTEGER))
			{
			    currentEmdkIntValue = Integer.parseInt(parser.getText());
			}
			else if(currentAlias.getType().equals(Type.STRING))
			{
			    currentEmdkStringValue = parser.getText();
			}
		    }
		}
	    }
	}

	/**
	 * @see IScannerImplementation
	 */
	@Override
	public void registerClient(IScannerClient client)
	{
        Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "registerClient client: " + client.toString()));
		boolean alreadyRegistered = false;
//		Common.mainActivity.setScannerEnabled(true);
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Getting scanner user"));
		synchronized(userStack)
		{
			for(ScannerUser user: userStack)
			{
				if(user.client.equals(client))
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Found scanner user"));
					alreadyRegistered = true;
//					user.setScannerEnabled(true);
			        Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "already registered client: " + client.toString()));
					break;
				}
			}
			if (!alreadyRegistered)
			{
                Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "adding to stack client: " + client.toString()));
                Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "adding scanner user"));
				userStack.add(new ScannerUser(client));
			}
		}
	}
	
	/**
	 * @See IScannerImplementation
	 */
	@Override
	public void enableScanner(IScannerClient client)
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "enableScanner client: " + client.toString()));
	    scannerInstructionQueue.add(new ScannerInstruction(client, Task.ENABLE));	    
	}
	
	/**
	 * @See IScannerImplementation
	 */
	@Override
	public void disableScanner(IScannerClient client)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "enableScanner client: " + client.toString()));
		scannerInstructionQueue.add(new ScannerInstruction(client, Task.DISABLE));
	}
	
	private void enable(ScannerUser currentUser) throws ScannerExceptions
	{
		if(scanner != null && !currentUser.isScannerChanged())//If they're trying to enable the same scanner that is already enabled...
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Extraneous call to scanner.enable. Ignoring"));
			return;
		}
		else if(scanner != null && currentUser.isScannerChanged()) //else if theyre enabling a different scanner...
		{
			disable(currentUser);
			currentUser.resetScannerChanged();
		}
		try
		{
			
			Class scannerClass = Class.forName("com.symbol.scanning.Scanner");
			if(currentUser.getScannerId() >= 0)
			{
				try
				{
					Method openMethod = scannerClass.getMethod("open", Context.class, int.class);
					scanner = (Scanner) openMethod.invoke(null, Common.mainActivity, currentUser.getScannerId());
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "opening Scanner object"));
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "New BSP, using new open command"));
				}
				catch (SecurityException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Cannot access scanner class"));
				}
				catch (NoSuchMethodException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Old BSP, using old open command"));
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "opening Scanner object"));
					scanner = Scanner.open(Common.mainActivity);
				}
				catch (IllegalArgumentException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "1 Cannot instantiate scanner"));
				}
				catch (IllegalAccessException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "2 Cannot instantiate scanner"));
				}
				catch (InvocationTargetException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "3 Cannot instantiate scanner"));
				}
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "opening Scanner object"));
				scanner = Scanner.open(Common.mainActivity);
			}
			
			currentUser.setScannerEnabled(true);
			Scanner.registerScanEventCallback(EmdkScannerServiceRebranded.this, scanner);
			Common.elementsCore.setScannerEnabled(true);
			
			try
			{
				Method enableMethod = scannerClass.getMethod("enable", null);
				enableMethod.invoke(scanner, new Object[] {});
			}
			catch(NoSuchMethodException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Old BSP. Ignoring internal enable"));
			}
			catch (IllegalArgumentException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "1 Cannot enable scanner"));
			}
			catch (IllegalAccessException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "2 Cannot enable scanner"));
			}
			catch (InvocationTargetException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "3 Cannot enable scanner"));
			}
			catch (NullPointerException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "4 Cannot enable scanner"));
			}
			
		}
		catch (ClassNotFoundException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Cannot find scanner class"));
		}
	}
	
	private void disable(ScannerUser currentUser)
	{
		//If its on the openStack
		synchronized(openStack)
		{
			for(int j = 0; j < openStack.size(); j++)
			{
				IScannerClient testClient = openStack.get(j).getClient(); 
				if(testClient.equals(currentUser.getClient()))
				{
					currentUser.setScannerEnabled(false);
					
					int stackSize = openStack.size();
					if(stackSize == 1)
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Removing open scanner user: clear"));
						openStack.clear();
						try {scanner.stopScanning(); /*TODO is this sensible?*/}
						catch (NullPointerException e) {Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));} 
						Common.elementsCore.setScannerEnabled(false);
					}
					else if(stackSize > 1)
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Removing open scanner user"));
						openStack.remove(stackSize - 1);
						try
						{
							scanner.setParameters(openStack.get(stackSize -2).getParameters()); //TODO why -2?
						}
						catch (ScannerExceptions e)
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unsuccessful scanner parameter change: " + e.getMessage()));
						}
					}
					
					try
					{
						
						Class scannerClass = Class.forName("com.symbol.scanning.Scanner");
						try
						{
							Method disableMethod = scannerClass.getMethod("disable", null);
							disableMethod.invoke(scanner, new Object[] {});
							//TODO Do we need to POP here?
						}
						catch(NoSuchMethodException e)
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Old BSP. Ignoring internal disable"));
							//TODO Do we need to POP here?
						}
						catch (IllegalArgumentException e)
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "1 Cannot disable scanner"));
						}
						catch (IllegalAccessException e)
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "2 Cannot disable scanner"));
						}
						catch (InvocationTargetException e)
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "3 Cannot disable scanner"));
						}
						catch (NullPointerException e)
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "4 cannot disable scanner"));
						}
					}
					catch (ClassNotFoundException e)
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Cannot find scanner class"));
					}
					
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "closing Scanner object"));
					try {scanner.release();}
					catch (NullPointerException e) {Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));}
					scanner = null;
					
					return;
				}
			}
			//if it wasnt on the stack, do nothing
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Extraneous scanner.disable call. Ignoring"));
			return;
		}
	}

	/**
	 * @See IScannerImplementation
	 */
	@Override
	public IBinder onBind(Intent intent)
	{
		return null;
	}

	/**
	 * @See IScannerImplementation
	 */
	@Override
	public boolean onUnbind(Intent intent)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		return false;
	}

	/**
	 * @See IScannerImplementation
	 */
	@Override
	public void onDestroy()
	{
	    if(scanner != null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "closing Scanner object"));
			scanner.stopScanning();
			scanner.release();
			scanner = null;
		}
		try
		{
			isShuttingDown = true;
			Scanner.unRegisterScanEventCallback(this);
			Common.elementsCore.setScannerEnabled(false);
			mScannerService.unregisterReceiver(triggerReceiver);
			startScannerThread.interrupt();
		}
		catch (IllegalArgumentException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Scanner Trigger Receiver wasn't registered"));
		}

		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Clearing user stack"));
		userStack.clear();
		openStack.clear();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
	}
	
	/**
	 * @See IScannerImplementation
	 */
	@Override
	public void onCreate()
	{
		isScanning = false;
		
		scanDecodeWav = Common.config.getSetting("scandecodewav");
		
		//startScannerThread = new StartScannerThread();
		
		triggerReceiver = new BroadcastReceiver()
		{
			@Override
			public void onReceive(Context context, Intent intent)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Trigger Received"));
				if ( (intent == null) || (intent.getIntExtra("pid", android.os.Process.myPid()) != android.os.Process.myPid()) )
		    		    return;
				boolean isKeyDown = intent.getExtras().getBoolean("isKeyDown");
				
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Searching user stack"));
				synchronized(userStack)
				{
					ScannerUser startScannerUser = null;
					for(int i = userStack.size() - 1; i >= 0; i--)
					{
						ScannerUser currentUser = userStack.get(i);
						if(currentUser.scannerEnabled)
						{
							if(startScannerUser == null)
							{
								startScannerUser = currentUser;
							}
//							if (currentUser.getClient() instanceof License) // if the license scanning is enabled, override all others.
//							{
//								startScannerUser = currentUser;
//								break;
//							}
                            if (Proxy.isProxyClass(currentUser.getClient().getClass())
                                    && Proxy.getInvocationHandler(currentUser.getClient()).getClass().getCanonicalName().equals("com.rho.rhoelements.license.ScannerClient.ScannerClientHandler")) // if the license scanning is enabled, override all others.
                            {
                                Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "RhoElements 2 LicenseClient: " + Proxy.getInvocationHandler(currentUser.getClient()).getClass().getCanonicalName()));
                                startScannerUser = currentUser;
                                break;
                            }
						}
					}
					if(isKeyDown && startScannerUser != null)
					{
						startScanner(startScannerUser.getClient());
					}
					else if(startScannerUser != null)
					{
						stopScanner(startScannerUser.getClient());
					}
				}
			}
		};
		
		if(scanDecodeWav == null || scanDecodeWav.equals(""))
		{
			try
			{
				String volume = Common.config.getSetting("decodevolume");
				String frequency = Common.config.getSetting("decodefrequency");
				String duration = Common.config.getSetting("decodeduration");
				
				if(volume == null) volume = "";
				if(frequency == null) frequency = "";
				if(duration == null) duration = "";
				
				Integer decodeVolume = Integer.decode(volume);
				Integer decodeFrequency = Integer.decode(frequency);
				Integer decodeDuration = Integer.decode(duration);
				
				if(decodeVolume != null && decodeFrequency != null && decodeDuration != null)
				{
					ToneFileFactory toneWavGenerator = new ToneFileFactory(decodeFrequency, decodeVolume, decodeDuration);
					File wavFile = toneWavGenerator.generateWavFile();
					if(wavFile != null && wavFile.exists())
					{
						scanDecodeWav = Uri.fromFile(wavFile).toString();
					}
				}
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Could not parse a config entry for the decode beep."));
			}
		}
		IntentFilter intentFilter = new IntentFilter(ScannerService.SCANNER_TRIGGER_EVENT);
		mScannerService.registerReceiver(triggerReceiver, intentFilter);
	}
	
	public void onPause()
	{
		
	}
	
	public void onResume()
	{
		
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId)
	{
		return Service.START_STICKY;
	}
	
	/**
	 * @See IScannerImplementation
	 */
	public void addDecodeSetting(IScannerClient client, String decoder, String setting)
	{
	    if((startScannerThread == null || !startScannerThread.isAlive()) && !isShuttingDown)
		{
			startScannerThread = new StartScannerThread();
			startScannerThread.start();
		}
	    scannerInstructionQueue.add(new ScannerInstruction(client, Task.SETPARAMS, decoder, setting));
	}

	void setDecodeSetting(IScannerClient client, String decoder, String setting)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Getting scanner user"));
		if(!userStack.isEmpty())
		{
			for(ScannerUser user: userStack)
			{
				if(user.getClient().equals(client))
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Found scanner user"));
					user.addDecoderSetting(decoder, setting);
					//If the client has currently activated the scanner...
					if(user.getScannerEnabled() && openStack.size() > 0 && openStack.get(openStack.size() - 1).getClient().equals(client))
					{
						try
						{
						    scanner.setParameters(user.getParameters());
						}
						catch (ScannerExceptions e)
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Unsuccessful scanner parameter change: " + e.getMessage()));
						}
						catch (NullPointerException e)
						{
						    Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
						}
					}
					break;
				}
			}
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Attempted to add scanner setting \"" + decoder + "\" but scanner is not in use"));
		}
	}

	/**
	 * @See IScannerImplementation 
	 */
	@Override
	public void clearSettings(IScannerClient client)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Getting scanner user"));
		if(!userStack.isEmpty())
		{
			synchronized(userStack)
			{
				for(ScannerUser user: userStack)
				{
					if(user.getClient().equals(client))
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Found scanner user"));
						user.clearScannerSettings();
						//If the client has currently activated the scanner...
						if(openStack.size() > 0 && openStack.get(openStack.size() - 1).getClient().equals(client))
						{
							try
							{
								scanner.setParameters(user.getParameters());
							}
							catch (ScannerExceptions e)
							{
								Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unsuccessful scanner parameter change: " + e.getMessage()));
							}
						}
					}
				}
			}
		}
	}

	/**
	 * @See IScannerImplementation
	 */
	@Override
	public JSONArray enumerateScanners()
	{
		JSONArray j = new JSONArray();
		for(int i=0; i < Scanner.getNumberOfScanners(); i++)
		{
			try
			{
				//Change to a proper accessor when fixed.
				ScannerInfo scanner = Scanner.getScannerInfo(i);
				JSONObject a = new JSONObject();
				a.put(Plugin.jsonObjNVPOrderString, "deviceName,friendlyName");
				a.put("deviceName", "SCN" + (i + 1));
				a.put("friendlyName", scanner.scannerType.toString());
				j.put(a);
			}
			catch (JSONException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
			}
			catch (ScannerExceptions e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
			}
		}
		return j;
	}

	/**
	 * @See IScannerImplementation
	 */
	@Override
	public void removeScannerUser(IScannerClient scannerClient, boolean isReset)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "removeScannerUser client: " + scannerClient.toString()));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Getting scanner user"));
		synchronized(userStack)
		{
			if(!userStack.isEmpty())
			{
				for(int i = 0; i < userStack.size(); i++)
				{
					if(userStack.get(i).getClient().equals(scannerClient))
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Found scanner user"));
						//Check to see if its using the scanner
						synchronized(openStack)
						{
							for(int j = 0; j < openStack.size(); j++)
							{
								IScannerClient testClient = openStack.get(j).getClient(); 
								if(testClient.equals(scannerClient))
								{
									if(openStack.size() == j + 1)
									{	//CurrentUser
										stopScanner(scannerClient);
									}
									else
									{	//Not the current user
										Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Removing open scanner user"));
										openStack.remove(j);
										break;
									}
								}
							}
						}
	
						//If it is a call to scanner.disable
						if(isReset)
						{
							userStack.get(i).setScannerEnabled(false);
							boolean scannerInUse = false;
							for(ScannerUser user: userStack)
							{
								scannerInUse |= user.getScannerEnabled();
							}
							Common.elementsCore.setScannerEnabled(scannerInUse);
						}
						else //The user is closing, remove all traces.
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Removing scanner user"));
							userStack.remove(i);
							if(userStack.isEmpty())
							{
								Common.elementsCore.setScannerEnabled(false);
							}
						}
						return;
					}
				}
			}
			else
			{
				Common.elementsCore.setScannerEnabled(false);
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Attempted to remove scanner user but scanner is not in use"));
			}
		}
	}
	
//	private void popActiveScannerUser()
//	{
//		synchronized(openStack)
//		{
//			int size = openStack.size();
//			if(size > 0)
//			{
//				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Removing open scanner user"));
//				openStack.remove(size -1);
//			}
//		}
//	}

	/**
	 * @See IScannerImplementation
	 */
	@Override
	public void setScanner(IScannerClient client, String scanner) throws InvalidParameterException
	{
		int scannerNumber = -1;
		try
		{
			//WARNING this presumes all valid scanner id's are of the 
			//form ???x where x is a single or multi digit integer.
			scannerNumber = Integer.parseInt(scanner.substring(3)) -1;
		}
		catch (IndexOutOfBoundsException e)
		{
			throw new InvalidParameterException();
		}
		catch(NumberFormatException e)
		{
			throw new InvalidParameterException();
		}

		synchronized(userStack)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Finding scanner user"));
			if(!userStack.isEmpty())
			{
				for(ScannerUser user: userStack)
				{	
					if(user.getClient().equals(client)) //Found the client...
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Found scanner user"));
						if(user.getScannerId() == -1 || user.getScannerId() != scannerNumber) //If it needs changing...
						{
							user.setScannerId(scannerNumber);
//							user.clearScannerSettings();
							//If it is the current user...
							if(openStack.size() > 0 && openStack.get(openStack.size() - 1).getClient().equals(client))
							{
								stopScanner(client);
								startScanner(client);
							}
						}
						return; //Found it so finish.
					}
				}
				//TODO add code to deal with non existent scanners
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Attempted to set scanner but scanner is not in use"));
			}
		}
	}
	
	/**
	 * @See IScannerImplementation 
	 */
	@Override
	public void stopScanner(IScannerClient client)
	{
		if((startScannerThread == null || !startScannerThread.isAlive()) && !isShuttingDown)
		{
			startScannerThread = new StartScannerThread();
			startScannerThread.start();
		}
		scannerInstructionQueue.add(new ScannerInstruction(client, Task.STOP));
	}
	
	/**
	 * @See IScannerImplementation
	 */
	@Override
	public boolean pauseScanner()
	{
		synchronized(openStack)
		{
			if(openStack.size() > 0)
			{
				pausedUser = openStack.get(openStack.size() - 1);
				stopScanner(pausedUser.getClient());
			}
		}
		return isScanning;
	}
	
	/**
	 * @See IScannerImplementation
	 */
	@Override
	public void resumeScanner()
	{
		startScanner(pausedUser.getClient());
	}

	/**
	 * @See IScannerImplementation
	 */
	@Override
	public void startScanner(IScannerClient client) throws InvalidParameterException
	{ 
		if((startScannerThread == null || !startScannerThread.isAlive()) && !isShuttingDown)
		{
			startScannerThread = new StartScannerThread();
			startScannerThread.start();
		}
		scannerInstructionQueue.add(new ScannerInstruction(client, Task.START));
	}

	/**
	 * @See IScannerImplementation
	 */
	@Override
	public String getDefaultScannerId()
	{
		int defaultNum = getDefaultScannerNumber();
		if(defaultNum == -1)
		{
			return null;
		}
		else
		{
			return "SCN" + defaultNum;
		}
	}
	
	/**
	 * Returns the ID of the default scanner, specific to the device
	 * @return the scannerID, with the format "SCNx" where x is a single or multi digit number
	 */
	public int getDefaultScannerNumber()
	{
		int n = Scanner.getNumberOfScanners();
		for (int i = 0; i < n; ++i)
		{
			ScannerInfo si = null;
			try
			{
				si = Scanner.getScannerInfo(i);
			}
			catch (ScannerExceptions e)
			{
				e.printStackTrace();
			}
			if(isDefaultScanner(si))
			{
				return i;
			}
		}
		if(Scanner.getNumberOfScanners() > 0)
		{
			return 0;
		}
		return -1;
	}
	
	public static boolean isDefaultScanner(ScannerInfo si) 
	{
		Class sic = ScannerInfo.class;
		try
		{
			Field f = sic.getField("defaultScanner");
			return f.getBoolean(si);
		}
		catch (SecurityException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Security Exception"));
		}
		catch (NoSuchFieldException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Could not find defaultScanner field: 1"));
		}
		catch (IllegalArgumentException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Could not find defaultScanner field: 2"));
		}
		catch (IllegalAccessException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Could not find defaultScanner field: 3"));
		}
		return true;
	}

	@Override
	public void onDecode(List<ScanResult> scanBuffer)
	{
		//TODO currently only supports 1 barcode per scan.
		//TODO Extend to do multiple barcodes when available.
		Log.d("mc18","scanBuffer Rebranded..."+scanBuffer.size());
		if(scanBuffer.size() > 0)
		{
			ScanResult result = scanBuffer.get(0);
			String contents = result.toString();
			String type = result.getLabelType();
			String time = (new SimpleDateFormat("hh:mm:ss")).format(new Date()); //$NON-NLS-1$
			String length = Integer.toString(contents.length());
			try
			{
				synchronized(openStack)
				{
					ScannerUser currentUser = openStack.get(openStack.size() -1);
					IScannerClient client = currentUser.getClient();
					WinScannerResult scannerType = winTypeToString.get(type);
					if(scannerType == null)
					{
						scannerType = winTypeToString.get("Unknown"); //$NON-NLS-1$
					}
					client.onScanEvent(contents, "SCN" + (currentUser.getScannerId() + 1) + ":" + scannerType.typeLabel, 
							scannerType.typeCode, time, length, "Decode");
				}
			}
			catch(IndexOutOfBoundsException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Scan complete but lost scanner user."));
			}
		}
	}

	public void onScanEvent(ScanEvent scanEvent, int scanEventInfo, Scanner scanner)
	{
		switch(scanEvent)
		{
			case SCAN_EVENT_STATUS_CHANGE: 
				if(scanEventInfo == Scanner.SCAN_STATUS_IDLE)
				{
					isScanning = false;
//					popActiveScannerUser();
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "ScanEvent Stopped"));
				}
				else if(scanEventInfo == Scanner.SCAN_STATUS_SCANNING)
				{
					isScanning = true;
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "ScanEvent Started"));
				}
				break;
			default: break;
		}
	}
	
	public void onScanEvent(Scanner arg0, ScanStatus scanStatus)
	{
	    // ordinal() is used as opposed to the ScanStatus directly to workaround the crash issue with RE2 on RevB ET1 BSP's
	    switch(scanStatus.ordinal())
	    {
	      	case 1: //SCAN_STATUS_IDLE
	        {
	            isScanning = false;
	            ////   popActiveScannerUser();
	            Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "ScanEvent Stopped"));
	            break;
	        }
	        case 2: //SCAN_STATUS_SCANNING
	        {
	            isScanning = true;
	            Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "ScanEvent Started"));
	            break;
	        }
	        case 0: //SCAN_STATUS_ENABLED
	            //TODO Unknown what to do here, new ability of SDK 1.3.3.
	        default: break;
	    }  
	}
	
	public ScannerUser getCurrentUser(IScannerClient client) throws InvalidParameterException
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Getting scanner user"));
		synchronized(userStack)
		{
			for(ScannerUser user: userStack)
			{
				if(user.getClient().equals(client))
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Found scanner user"));
					return user;
				}
			}
		}
		throw new InvalidParameterException("Could not find scanner user");
	}
	
	/**
	 * @author Ben Kennedy (NCVT73)
	 * A ScannerUser is a internal class to ScannerService. It is used to store the object upon which scanEvents
	 * should be called, and also the scanner that each user wants to use, and the modes and decoder settings
	 * of each.
	 */
	private class ScannerUser
	{
		private IScannerClient client;
		private int scannerId;
		private Parameters params;
		private boolean scannerEnabled;
		private ArrayList<ArrayList<String>> decoderQueue;
		private boolean isScannerChanged;

		/**
		 * Creates a new ScannerUser.
		 * @param client the identifier of each scanner user. The object to call 
		 * onScanEvent upon
		 * @param params the parameters to use when scanning of this scannerUser.
		 */
		public ScannerUser(IScannerClient client)
		{
			this.client = client;
			this.decoderQueue = new ArrayList<ArrayList<String>>();
			clearScannerSettings();
			this.scannerEnabled = false;
			this.scannerId = getDefaultScannerNumber();
			isScannerChanged = false;
		}

		public boolean getScannerEnabled()
		{
			return scannerEnabled;
		}

		/**
		 * @param scannerEnabled the scannerEnabled to set
		 */
		public void setScannerEnabled(boolean scannerEnabled)
		{
			this.scannerEnabled = scannerEnabled;
			
			if(params == null)
			{
			    try
			    {
				params = scanner.getDefaultParameters();
			    }
			    catch (NullPointerException e)
			    {
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
			    }
			}
			
			if(!scannerEnabled)
			{
				//Resets the parameters
				clearScannerSettings();
			}
			else
			{
				
				synchronized(decoderQueue)
				{

//					try
//					{
//						ScannerInfo scanner = Scanner.getScannerInfo(scannerId);
//						switch(scanner.scannerType)
//						{
//							case SCANNER_INTERNAL:
//								addDecoderSetting("picklistmode", "softwarereticle");break;
//							case SCANNER_USB:
//								addDecoderSetting("picklistmode", "hardwarereticle");break;
//							default:break;
//						}
//					}
//					catch (ScannerExceptions e)
//					{
//						//Never!
//					}					
					for(ArrayList<String> command: decoderQueue)
					{
						addDecoderSetting(command.get(0), command.get(1));
					}
					decoderQueue.clear();
				}
			}
		}

		public Parameters getParameters()
		{
			return params;
		}

		/**
		 * Adds a named value pair of a decoder and a setting to the user.
		 * @param decoder the name of the decoder
		 * @param setting the value of the setting
		 */
		public void addDecoderSetting(String decoder, String setting)
		{
			if (scanner != null) //If the scanner has been enabled
			{
			    	if(decoder.equals(TAG_ALL_DECODERS) || decoder.equals(TAG_ALL_DECODERS2)) //For all decoders
				{
					boolean setDecoders;
					if(setting.equalsIgnoreCase("enabled"))
					{
						setDecoders = true;
					}
					else if(setting.equalsIgnoreCase("disabled"))
					{
						setDecoders = false;
					}
					else
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value: \"" + setting + "\" for parameter \"" + decoder +"\" is not recognised."));
						return;
					}
					
					List<String> decoderParams = params.getSupportedParameters("symbologies");
					for(String paramString: decoderParams)
					{
						try
						{
							params.setValue(paramString, new Boolean(setDecoders));
						}
						catch (ScannerExceptions e)
						{
							switch(e.getError())
							{
								case ScannerExceptions.SCAN_ERROR_PARAM_READ_ONLY:
									Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, decoder + " cannot be written (Read Only).")); break;
								case ScannerExceptions.SCAN_ERROR_PARAM_VALUE_NOT_SUPPORTED: 
									Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value: \"" + setting + "\" for parameter \"" + decoder +"\" is not supported.")); break;
								case ScannerExceptions.SCAN_ERROR_PARAM_MISMATCH: 
									Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value: \"" + setting + "\" for parameter \"" + decoder +"\" is not recognised.")); break;
								case ScannerExceptions.SCAN_ERROR_PARAM_NOT_FOUND: 
									Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, decoder + " parameter not recognised")); break;
								default: Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Could not set" + decoder + ". An unknown error occurred.")); break;
							}
						}
						catch(NumberFormatException e)//TODO Remove once fixed
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value: \"" + setting + "\" for parameter \"" + decoder +"\" is not recognised."));
						}
						catch(NullPointerException e)//TODO very very bad practice remove immediately once scanner.java setValueAsString has been properly exceptioned.
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value: \"" + setting + "\" for parameter \"" + decoder +"\" is not recognised."));
						}
					}
					//Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Although " + decoder + " has been set successfully, it has been deprecated. It is strongly advised that you do not use this parameter."));
					return;
				}
				try
				{
					Alias alias = aliasMap.get(decoder);
					if(alias != null)//If statement for Elements Aliases
					{
						if(alias.getType() == Type.DEPRECATED)
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "\"" + decoder + "\" has been deprecated. Please change to an alternative. \"" + decoder + "\" has not been set."));
							return;
						}
						else if(alias.getType() == Type.INTEGER)
						{
							Integer translatedSetting = alias.getIntTranslation(setting);
							if(translatedSetting != null)
							{
								if(translatedSetting.intValue() == -1)//Not Supported
								{
									Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value: \"" + setting + "\" for parameter \"" + decoder + "\" is not supported on this platform."));
									return;
								}
								else if(translatedSetting.intValue() == -2)//Deprecated
								{
									Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value: \"" + setting + "\" for parameter \"" + decoder + "\" has been deprecated. Please change to an alternative. \"" + decoder + "\" has not been changed."));
									return;
								}
								params.setValue(alias.getEmdkId(), translatedSetting);
								return;
							}
						}
						else if(alias.getType() == Type.BOOLEAN)
						{
							if(setting.equalsIgnoreCase("enabled") || setting.equalsIgnoreCase("enable"))
							{
								params.setValueAsString(alias.getEmdkId(), "true");
								return;
							}
							else if(setting.equalsIgnoreCase("disabled") || setting.equalsIgnoreCase("disable"))
							{
								params.setValueAsString(alias.getEmdkId(), "false");
								return;	
							}
						}
						else if(alias.getType() == Type.STRING)
						{
							params.setValueAsString(alias.getEmdkId(), alias.getStringTranslation(setting));
							return;
						}
						params.setValueAsString(alias.getEmdkId(), setting);
					}
					else //Not a recognised Elements Alias, pass straight thru
					{
						params.setValueAsString(decoder, setting);
					}
				}
				catch (ScannerExceptions e)
				{
					switch(e.getError())
					{
						case ScannerExceptions.SCAN_ERROR_PARAM_READ_ONLY:
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, decoder + " cannot be written (Read Only).")); break;
						case ScannerExceptions.SCAN_ERROR_PARAM_VALUE_NOT_SUPPORTED: 
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value: \"" + setting + "\" for parameter \"" + decoder +"\" is not supported.")); break;
						case ScannerExceptions.SCAN_ERROR_PARAM_MISMATCH: 
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value: \"" + setting + "\" for parameter \"" + decoder +"\" is not recognised.")); break;
						case ScannerExceptions.SCAN_ERROR_PARAM_NOT_FOUND: 
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, decoder + " parameter not recognised")); break;
						default: Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Could not set" + decoder + ". An unknown error occurred.")); break;
					}
				}
				catch(NumberFormatException e)//TODO Remove once fixed
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value: \"" + setting + "\" for parameter \"" + decoder +"\" is not recognised."));
				}
				catch(NullPointerException e)//TODO very very bad practice remove immediately once scanner.java setValueAsString has been properly exceptioned.
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Value: \"" + setting + "\" for parameter \"" + decoder +"\" is not recognised."));
				}
			}
			else //If scanner.enable or scanner.enabled hasnt been called yet
			{
				//add the request to the queue of "addDecoderSettings" requests.
				ArrayList<String> command = new ArrayList<String>();
				command.add(decoder);
				command.add(setting);
				decoderQueue.add(command);
			}
		}

		/**
		 * Gets the scanner ID to which this user is related.
		 */
		public int getScannerId()
		{
			return scannerId;
		}

		/**
		 * Clears all decoder settings from the user.
		 */
		public void clearScannerSettings()
		{
			if(scanner != null)
			{
				params = scanner.getDefaultParameters();
			}
			decoderQueue.clear();
			if(scanDecodeWav != null && !scanDecodeWav.equals(""))
			{
				try
				{
					String scanDecodeWavUri = Common.parseAndroidURI(scanDecodeWav).toString();
					addDecoderSetting("decode_audio_feedback_uri", scanDecodeWavUri);
				}
				catch (InvalidParameterException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
				}
				catch (URISyntaxException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, e.getMessage()));
				}
			}
		}

		/**
		 * @return the identifier
		 */
		public IScannerClient getClient()
		{
			return client;
		}

		/**
		 * @param scannerId the scannerId to set
		 */
		public void setScannerId(int scannerId)
		{
			if(this.scannerId != scannerId)
			{
				this.scannerId = scannerId;
				isScannerChanged = true;
			}
		}

		public boolean isScannerChanged()
		{
			return isScannerChanged;
		}

		public void resetScannerChanged()
		{
			this.isScannerChanged = false;
		}
		
	}
	
	/**
	 * @author Ben Kennedy (NCVT73)
	 * A Class that holds information about a Elements Alias. This object holds the EMDK parameter
	 * string that needs to be passed to the EMDK scanning library, the type of data that the 
	 * EMDK will expect to receive when this parameter is applied, and a list of translations
	 * that convert the inputted Elements/PB string into the EMDK Integer.
	 */
	public class Alias
	{	
		private String emdkStr;
		private Type type;
		private HashMap<String, Integer> intTranslations;
		private HashMap<String, String> stringTranslations;
		
		/**
		 * Constructs an Alias.
		 * @param emdkStr EMDK string of this alias.
		 * @param type Enum content type of the alias.
		 */
		public Alias(String emdkStr, Type type)
		{
			this.emdkStr = emdkStr;
			this.type = type;
			this.intTranslations = new HashMap<String, Integer>(3);
			this.stringTranslations = new HashMap<String, String>(2);
		}
		
		/**
		 * Returns the EMDK string of this alias.
		 * @return the EMDK string of this alias.
		 */
		public String getEmdkId()
		{
			return emdkStr;
		}
		
		/**
		 * Returns the content type of the alias.
		 * @return the Enum content type of the alias.
		 */
		public Type getType()
		{
			return type;
		}
		
		/**
		 * Adds a Elements/PB setting string to EMDK integer translation.
		 * @param pbValue the Elements/PB setting string to translate.
		 * @param emdkValue the EMDK Integer that the string represents.
		 */
		public void addIntTranslation(String pbValue, int emdkValue)
		{
			this.intTranslations.put(pbValue, new Integer(emdkValue));
		}

		/**
		 * Adds a Elements/PB setting string to EMDK integer translation.
		 * @param pbValue the Elements/PB setting string to translate.
		 * @param emdkValue the EMDK Integer that the string represents.
		 */
		public void addStringTranslation(String pbValue, String emdkValue)
		{
			this.stringTranslations.put(pbValue, emdkValue);
		}
		
		
		/**
		 * Returns the EMDK readable integer value of a Elements/PB string value for this alias.
		 * @param pbValue the Elements/PB string as defined in the helpfile that will be translated. 
		 * @return the integer value of the inputted Elements/PB string
		 */
		public Integer getIntTranslation(String pbValue)
		{
			Integer returnValue = this.intTranslations.get(pbValue);
			if(returnValue != null)
			{
				//If a translation exists, use it.
				return returnValue;
			}
			try
			{
				//else, try parsing the string
				return Integer.parseInt(pbValue);
			}
			catch(NumberFormatException e)
			{
				//This means it was a bad argument.
				return null;
			}
		}
		
		/**
		 * Returns the EMDK readable string value of a Elements/PB string value for this alias.
		 * @param pbValue the Elements/PB string as defined in the helpfile that will be translated. 
		 * @return the string value of the inputted Elements/PB string
		 */
		public String getStringTranslation(String pbValue)
		{
			if(pbValue != null)
			{
				return this.stringTranslations.get(pbValue);
			}
			return null;
		}

	}
	
	/**
	 * Parses the inputted string into a content type for a EMDK/Elements parameter.
	 * @param type the String representation of the content type for a parameter
	 * @return the Enum representation of the inputted string
	 */
	public static Type parseType(String type)
	{
		if(type.equalsIgnoreCase("boolean"))
		{
			return Type.BOOLEAN;
		}
		else if(type.equalsIgnoreCase("integer"))
		{
			return Type.INTEGER;
		}
		else if(type.equalsIgnoreCase("string"))
		{
			return Type.STRING;
		}
		else if(type.equalsIgnoreCase("deprecated"))
		{
			return Type.DEPRECATED;
		}
		return Type.UNSUPPORTED; //Not being used atm
	}
	
	private void initWinCompatibilityTable()
	{
		Class scannerClass;
		try
		{
			
			String className = "com.symbol.scanning.Scanner";
			scannerClass = Class.forName(className);
		}
		catch (ClassNotFoundException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Cannot load Scanner class"));
			return;
		}
		
		winTypeToString = new HashMap<String, WinScannerResult>();
		winTypeToString.put(Scanner.LABEL_TYPE_UPCE0, new WinScannerResult("UPCE0", "0x30"));	
		winTypeToString.put(Scanner.LABEL_TYPE_UPCE1, new WinScannerResult("UPCE1", "0x31"));
		winTypeToString.put(Scanner.LABEL_TYPE_UPCA, new WinScannerResult("UPCA", "0x32"));
		winTypeToString.put(Scanner.LABEL_TYPE_MSI, new WinScannerResult("MSI", "0x33"));
		winTypeToString.put(Scanner.LABEL_TYPE_EAN8, new WinScannerResult("EAN8", "0x34"));
		winTypeToString.put(Scanner.LABEL_TYPE_EAN13, new WinScannerResult("EAN13", "0x35"));
		winTypeToString.put(Scanner.LABEL_TYPE_CODABAR, new WinScannerResult("CODABAR", "0x36"));
		winTypeToString.put(Scanner.LABEL_TYPE_CODE39, new WinScannerResult("CODE 3 OF 9", "0x37"));
		winTypeToString.put(Scanner.LABEL_TYPE_D2OF5, new WinScannerResult("DISCRETE 2 OF 5", "0x38"));
		winTypeToString.put(Scanner.LABEL_TYPE_I2OF5, new WinScannerResult("INTERLEAVED 2 OF 5", "0x39"));
		winTypeToString.put(Scanner.LABEL_TYPE_CODE11, new WinScannerResult("CODE 11", "0x3a"));
		winTypeToString.put(Scanner.LABEL_TYPE_CODE93, new WinScannerResult("CODE 93", "0x3b"));
		winTypeToString.put(Scanner.LABEL_TYPE_CODE128, new WinScannerResult("CODE 128", "0x3c"));
		winTypeToString.put(Scanner.LABEL_TYPE_IATA2OF5, new WinScannerResult("IATA 2 OF 5", "0x3e"));
		winTypeToString.put(Scanner.LABEL_TYPE_EAN128, new WinScannerResult("EAN 128", "0x3f"));
		winTypeToString.put(Scanner.LABEL_TYPE_PDF417, new WinScannerResult("PDF 417", "0x40"));
		winTypeToString.put(Scanner.LABEL_TYPE_ISBT128, new WinScannerResult("ISBT 128", "0x41"));
		winTypeToString.put(Scanner.LABEL_TYPE_TRIOPTIC39, new WinScannerResult("TRIOPTIC 3 OF 9", "0x42"));
		winTypeToString.put(Scanner.LABEL_TYPE_COUPON, new WinScannerResult("COUPON CODE", "0x43"));
		winTypeToString.put(Scanner.LABEL_TYPE_BOOKLAND, new WinScannerResult("BOOKLAND EAN", "0x44"));
		winTypeToString.put(Scanner.LABEL_TYPE_MICROPDF, new WinScannerResult("MICRO PDF", "0x45"));
		winTypeToString.put(Scanner.LABEL_TYPE_CODE32, new WinScannerResult("CODE 32", "0x46"));
		winTypeToString.put(Scanner.LABEL_TYPE_MAXICODE, new WinScannerResult("MAXICODE", "0x48"));
		winTypeToString.put(Scanner.LABEL_TYPE_DATAMATRIX, new WinScannerResult("DATAMATRIX", "0x49"));
		winTypeToString.put(Scanner.LABEL_TYPE_QRCODE, new WinScannerResult("QRCODE", "0x4a"));
		//TODO fix this string when the new EMDK addon arrives
//		winTypeToString.put("LABEL-TYPE-GS1-DATABAR", new WinScannerResult("RSS-14", "0x4c"));
//		
//		winTypeToString.put("LABEL-TYPE-GS1-DATABAR-LIM", new WinScannerResult("RSS LIMITED", "0x4d"));
		winTypeToString.put(Scanner.LABEL_TYPE_SIGNATURE, new WinScannerResult("SIGNATURE", "0x52"));
		winTypeToString.put(Scanner.LABEL_TYPE_WEBCODE, new WinScannerResult("SCANLET WEBCODE", "0x54"));
		winTypeToString.put(Scanner.LABEL_TYPE_COMPOSITE_AB, new WinScannerResult("COMPOSITE AB", "0x56"));
		winTypeToString.put(Scanner.LABEL_TYPE_COMPOSITE_C, new WinScannerResult("COMPOSITE C", "0x57"));
		winTypeToString.put(Scanner.LABEL_TYPE_TLC39, new WinScannerResult("TCIF LINKED CODE 39", "0x58"));
		winTypeToString.put(Scanner.LABEL_TYPE_USPOSTNET, new WinScannerResult("US POSTNET", "0x61"));
		winTypeToString.put(Scanner.LABEL_TYPE_USPLANET, new WinScannerResult("US PLANET", "0x62"));
		winTypeToString.put(Scanner.LABEL_TYPE_UKPOSTAL, new WinScannerResult("UK POSTAL", "0x63"));
		winTypeToString.put(Scanner.LABEL_TYPE_JAPPOSTAL, new WinScannerResult("JAPANESE POSTAL", "0x64"));
		winTypeToString.put(Scanner.LABEL_TYPE_AUSPOSTAL, new WinScannerResult("AUSTRALIAN POSTAL", "0x65"));
		winTypeToString.put(Scanner.LABEL_TYPE_DUTCHPOSTAL, new WinScannerResult("DUTCH POSTAL", "0x66"));
		winTypeToString.put(Scanner.LABEL_TYPE_CANPOSTAL, new WinScannerResult("CANADA POSTBAR", "0x67"));
		winTypeToString.put(Scanner.LABEL_TYPE_CHINESE_2OF5, new WinScannerResult("CHINESE 2 Of 5", "0x70"));
		winTypeToString.put(Scanner.LABEL_TYPE_AZTEC, new WinScannerResult("AZTEC", "0x74"));
		winTypeToString.put(Scanner.LABEL_TYPE_MICROQR, new WinScannerResult("MicroQR", "0x75"));
		winTypeToString.put(Scanner.LABEL_TYPE_KOREAN_3OF5, new WinScannerResult("Korean 3 Of 5", "0x76"));
		winTypeToString.put(Scanner.LABEL_TYPE_US4STATE, new WinScannerResult("US 4-State Postal", "0x77"));
		winTypeToString.put(Scanner.LABEL_TYPE_US4STATE_FICS, new WinScannerResult("US 4-State Postal FICS", "0x79"));
		winTypeToString.put(Scanner.LABEL_TYPE_MATRIX_2OF5, new WinScannerResult("MATRIX 2 OF 5", "0x7a"));
		winTypeToString.put(Scanner.LABEL_TYPE_OCR, new WinScannerResult("OCR", "0x7b"));
		winTypeToString.put("Unknown", new WinScannerResult("Unknown", "0xff"));
		
		/*
		 * The following section is the Reflection required for backwards compatibility with Non-attachment supporting
		 * BSPs. It only adds the particular labels if they exist in the devices SDK version.
		 */
		if(!addCompatibilityEntryReflect(scannerClass, "LABEL_TYPE_GS1_DATABAR", new WinScannerResult("GS1 DATABAR", "0x4c")))
		{
			winTypeToString.put("LABEL-TYPE-GS1-DATABAR", new WinScannerResult("RSS-14", "0x4c"));
		}
		if(!addCompatibilityEntryReflect(scannerClass, "LABEL_TYPE_GS1_DATABAR_LIM", new WinScannerResult("GS1 DATABAR LIMITED", "0x4d")))
		{
			winTypeToString.put("LABEL-TYPE-GS1-DATABAR-EXP", new WinScannerResult("RSS EXPANDED", "0x4e"));
		}
		if(!addCompatibilityEntryReflect(scannerClass, "LABEL_TYPE_GS1_DATABAR_EXP", new WinScannerResult("GS1 DATABAR EXPANDED", "0x4e")))
		{
			winTypeToString.put("LABEL-TYPE-GS1-DATABAR-LIM", new WinScannerResult("RSS LIMITED", "0x4d"));
		}
		
		addCompatibilityEntryReflect(scannerClass, "LABEL_TYPE_MACROPDF", new WinScannerResult("MACRO PDF", "0x47"));
		addCompatibilityEntryReflect(scannerClass, "LABEL_TYPE_MACROMICROPDF", new WinScannerResult("MACRO MICRO PDF", "0x4b"));
		
		/* These are old versions of the GS1 string, which will still exist on old BSP versions */
		addCompatibilityEntryReflect(scannerClass, "LABEL_TYPE_RSS14", new WinScannerResult("GS1 DATABAR", "0x4c"));
		addCompatibilityEntryReflect(scannerClass, "LABEL_TYPE_RSSLIM", new WinScannerResult("GS1 DATABAR LIMITED", "0x4d"));
		addCompatibilityEntryReflect(scannerClass, "LABEL_TYPE_RSSEXP", new WinScannerResult("GS1 DATABAR EXPANDED", "0x4e"));
		
		/* This is what is replaced by the above code. Should, in the future, we decide to remove the
		 * reflection, certain of these entries will need to be uncommented.
		 */ 
//		winTypeToString.put(Scanner.LABEL_TYPE_GS1_DATABAR, new WinScannerResult("RSS-14", "0x4c"));
//		winTypeToString.put(Scanner.LABEL_TYPE_GS1_DATABAR_LIM, new WinScannerResult("RSS LIMITED", "0x4d"));
//		winTypeToString.put(Scanner.LABEL_TYPE_GS1_DATABAR_EXP, new WinScannerResult("RSS EXPANDED", "0x4e"));
//		winTypeToString.put(Scanner.LABEL_TYPE_MACROPDF, new WinScannerResult("MACRO PDF", "0x47"));
//		winTypeToString.put(Scanner.LABEL_TYPE_MACROMICROPDF, new WinScannerResult("MACRO MICRO PDF", "0x4b"));
//		winTypeToString.put(Scanner.LABEL_TYPE_RSS14, new WinScannerResult("RSS-14", "0x4c"));
//		winTypeToString.put(Scanner.LABEL_TYPE_RSSLIM, new WinScannerResult("RSS LIMITED", "0x4d"));
//		winTypeToString.put(Scanner.LABEL_TYPE_RSSEXP, new WinScannerResult("RSS EXPANDED", "0x4e"));
	}
	
	private boolean addCompatibilityEntryReflect(Class scannerClass, String fieldName, WinScannerResult result)
	{
		if(scannerClass == null || fieldName == null || result == null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Null input"));
			return false;
		}
		
		try
		{
			Field field = scannerClass.getField(fieldName);
			String labelString = new String();
			labelString = (String) field.get(labelString);
			winTypeToString.put(labelString, result);
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, fieldName + " added successfully"));
			return true;
		}
		catch(NoSuchFieldException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, fieldName + " does not exist: 1"));
		}
		catch (IllegalArgumentException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, fieldName + " does not exist: 2"));
		}
		catch (IllegalAccessException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, fieldName + " does not exist: 3"));
		}
		return false;
	}
	
	public class StartScannerThread extends Thread
	{
		public void run()
		{
			while(true)
			{
				try
				{
					ScannerInstruction entry = scannerInstructionQueue.take();
					IScannerClient client = entry.getClient();
					
					//Get the client from the userStack
					//TODO catch exception
					ScannerUser currentUser;
					
					switch(entry.getTask())
					{
						case ENABLE:
							//*****START SCANNER****//
							currentUser = getCurrentUser(client);
							handler.post(new ScannerTask(currentUser, openStack, entry));
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Scanner thread Sleeping..."));
							synchronized (globalLockObject)
							{
								Thread.sleep(1000); //TODO: Awful. Had to be done to sidestep the issue with callign start right after enable.
								Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Scanner thread Waking..."));
							}

							//TODO: change that last call when EMDK is fixed (event fired when enabling completes).
							break;
							//*****END START SCANNER****//
					
						case DISABLE:
							//*****STOP SCANNER ******//
							currentUser = getCurrentUser(client);
							handler.post(new ScannerTask(currentUser, openStack, entry));
							break;
							//****END STOP SCANNER ****//
							
						case START:
						    currentUser = getCurrentUser(client);
						    handler.post(new ScannerTask(currentUser, openStack, entry));
						    break;
							
						case STOP:
						    currentUser = getCurrentUser(client);
						    handler.post(new ScannerTask(currentUser, openStack, entry));
						    break;
						case SETPARAMS:
						    currentUser = getCurrentUser(client);
						    handler.post(new ScannerTask(currentUser, openStack, entry));
						    break;
						default: break;
					}
				}
				catch (InterruptedException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Scanner-Starting Thread interrupted."));
				}
				catch (InvalidParameterException e)
				{
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, e.getMessage()));
				}
			}
		}
	}
	
	/**
	 * An container to hold instructions for the scanner library. This could be either start scanner or
	 * stop scanner.
	 * @author NCVT73
	 */
	public class ScannerInstruction
	{
		private Task task;
		private IScannerClient	client;
		private String arg1;
		private String arg2;

		public ScannerInstruction(IScannerClient client, Task task)
		{
			this.client = client;
			this.task = task;
			this.arg1 = null;
			this.arg2 = null;
		}
		
		public ScannerInstruction(IScannerClient client, Task task, String arg1, String arg2)
		{
			this.client = client;
			this.task = task;
			this.arg1 = arg1;
			this.arg2 = arg2;
		}
		
		public Task getTask()
		{
			return task;
		}
		
		/**
		 * @return the client
		 */
		public IScannerClient getClient()
		{
			return this.client;
		}
	}
	
	private String scannerParamsDotXml =
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + 
					 "<scanner-params version=\"1.0\">" + 
					 "	<!-- SUPPORTED DECODERS AND DECODER SETTINGS -->" + 
					 "	<param alias=\"auspostal\" id=\"decoder_australian_postal\" type=\"boolean\"></param>" + 
					 "	<param alias=\"aztec\" id=\"decoder_aztec\" type=\"boolean\"></param>" + 
					 "	<param alias=\"canpostal\" id=\"decoder_canadian_postal\" type=\"boolean\"></param>" + 
					 "	<param alias=\"chinese_2of5\" id=\"decoder_chinese_2of5\" type=\"boolean\"></param>" + 
					 "	<param alias=\"chinese2of5\" id=\"decoder_chinese_2of5\" type=\"boolean\"></param>" + 
					 "	<param alias=\"codabar\" id=\"decoder_codabar\" type=\"boolean\"></param>" + 
					 "	<param alias=\"codabarclsiediting\" id=\"decoder_codabar_clsi_editing\" type=\"boolean\"></param>" + 
					 "	<param alias=\"codabarmaxlength\" id=\"decoder_codabar_length2\" type=\"integer\"></param>" + 
					 "	<param alias=\"codabarminlength\" id=\"decoder_codabar_length1\" type=\"integer\"></param>" + 
					 "	<param alias=\"codabarnotisediting\" id=\"decoder_codabar_notis_editing\" type=\"boolean\"></param>" + 
					 "	<param alias=\"codabarredundancy\" id=\"decoder_codabar_redundancy\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code11\" id=\"decoder_code11\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code11-reportcheckdigit\" id=\"decoder_code11_report_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code11reportcheckdigit\" id=\"decoder_code11_report_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code11redundancy\" id=\"decoder_code11_redundancy\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code11checkdigitcount\" id=\"decoder_code11_verify_check_digit\" type=\"integer\">" + 
					 "		<translate id=\"none\">0</translate>" + 
					 "		<translate id=\"one\">1</translate>" + 
					 "		<translate id=\"two\">2</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"code11minlength\" id=\"decoder_code11_length1\" type=\"integer\"></param>" + 
					 "	<param alias=\"code11maxlength\" id=\"decoder_code11_length2\" type=\"integer\"></param>" + 
					 "	<param alias=\"code128\" id=\"decoder_code128\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code128checkisbttable\" id=\"decoder_code128_check_isbt_table\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code128ean128\" id=\"decoder_code128_enable_ean128\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code128isbt128\" id=\"decoder_code128_enable_isbt128\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code128isbt128concatmode\" id=\"decoder_code128_isbt128_concat_mode\" type=\"integer\">" + 
					 "		<translate id=\"never\">0</translate>" + 
					 "		<translate id=\"always\">1</translate>" + 
					 "		<translate id=\"auto\">2</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"code128maxlength\" id=\"decoder_code128_length2\" type=\"integer\"></param>" + 
					 "	<param alias=\"code128minlength\" id=\"decoder_code128_length1\" type=\"integer\"></param>" + 
					 "	<param alias=\"code128redundancy\" id=\"decoder_code128_redundancy\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code128securitylevel\" id=\"decoder_code128_security_level\" type=\"integer\"></param>" + 
					 "	<param alias=\"code39\" id=\"decoder_code39\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code39converttocode32\" id=\"decoder_code39_convert_to_code32\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code39fullascii\" id=\"decoder_code39_full_ascii\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code39maxlength\" id=\"decoder_code39_length2\" type=\"integer\"></param>" + 
					 "	<param alias=\"code39minlength\" id=\"decoder_code39_length1\" type=\"integer\"></param>" + 
					 "	<param alias=\"code39redundancy\" id=\"decoder_code39_redundancy\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code39reportcheckdigit\" id=\"decoder_code39_report_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code39code32prefix\" id=\"decoder_code39_report_code32_prefix\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code39verifycheckdigit\" id=\"decoder_code39_verify_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code39securitylevel\" id=\"decoder_code39_security_level\" type=\"integer\"></param>" + 
					 "	<param alias=\"code93\" id=\"decoder_code93\" type=\"boolean\"></param>" + 
					 "	<param alias=\"code93maxlength\" id=\"decoder_code93_length2\" type=\"integer\"></param>" + 
					 "	<param alias=\"code93minlength\" id=\"decoder_code93_length1\" type=\"integer\"></param>" + 
					 "	<param alias=\"code93redundancy\" id=\"decoder_code93_redundancy\" type=\"boolean\"></param>" + 
					 "	<param alias=\"composit_ab\" id=\"decoder_composite_ab\" type=\"boolean\"></param>" + 
					 "	<param alias=\"compositeab\" id=\"decoder_composite_ab\" type=\"boolean\"></param>" + 
					 "	<param alias=\"compositeabucclinkmode\" id=\"decoder_composite_ab_ucc_link_mode\" type=\"integer\">" + 
					 "		<translate id=\"never\">0</translate>" + 
					 "		<translate id=\"always\">1</translate>" + 
					 "		<translate id=\"auto\">2</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"composit_c\" id=\"decoder_composite_c\" type=\"boolean\"></param>" + 
					 "	<param alias=\"compositec\" id=\"decoder_composite_c\" type=\"boolean\"></param>" + 
					 "	<param alias=\"d2of5\" id=\"decoder_d2of5\" type=\"boolean\"></param>" + 
					 "	<param alias=\"d2of5maxlength\" id=\"decoder_d2of5_length2\" type=\"integer\"></param>" + 
					 "	<param alias=\"d2of5minlength\" id=\"decoder_d2of5_length1\" type=\"integer\"></param>" + 
					 "	<param alias=\"d2of5redundancy\" id=\"decoder_d2of5_redundancy\" type=\"boolean\"></param>" + 
					 "	<param alias=\"datamatrix\" id=\"decoder_datamatrix\" type=\"boolean\"></param>" + 
					 "	<param alias=\"dutchpostal\" id=\"decoder_dutch_postal\" type=\"boolean\"></param>" + 
					 "	<param alias=\"ean13\" id=\"decoder_ean13\" type=\"boolean\"></param>" + 
					 "	<param alias=\"ean8\" id=\"decoder_ean8\" type=\"boolean\"></param>" + 
					 "	<param alias=\"gs1databar\" id=\"decoder_gs1_databar\" type=\"boolean\"></param>" +
					 "	<param alias=\"gs1databarexpanded\" id=\"decoder_gs1_databar_exp\" type=\"boolean\"></param>" +
					 "	<param alias=\"gs1databarlimited\" id=\"decoder_gs1_databar_lim\" type=\"boolean\"></param>" +
					 	//RSS Values
					 	"<param alias=\"rss14\" id=\"decoder_gs1_databar\" type=\"boolean\"></param>" +
					 	"<param alias=\"rssexp\" id=\"decoder_gs1_databar_exp\" type=\"boolean\"></param>" + 
					 	"<param alias=\"rsslim\" id=\"decoder_gs1_databar_lim\" type=\"boolean\"></param>" +
					 	//END RSS Values
					 "	<param alias=\"i2of5\" id=\"decoder_i2of5\" type=\"boolean\"></param>" + 
					 "	<param alias=\"i2of5verifycheckdigit\" id=\"decoder_i2of5_check_digit\" type=\"integer\">" + 
					 "		<translate id=\"none\">0</translate>" + 
					 "		<translate id=\"uss\">1</translate>" + 
					 "		<translate id=\"opcc\">2</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"i2of5maxlength\" id=\"decoder_i2of5_length2\" type=\"integer\"></param>" + 
					 "	<param alias=\"i2of5minlength\" id=\"decoder_i2of5_length1\" type=\"integer\"></param>" + 
					 "	<param alias=\"i2of5redundancy\" id=\"decoder_i2of5_redundancy\" type=\"boolean\"></param>" + 
					 "	<param alias=\"i2of5converttoean13\" id=\"decoder_itf14_convert_to_ean13\" type=\"boolean\"></param>" + 
					 "	<param alias=\"i2of5reportcheckdigit\" id=\"decoder_i2of5_report_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"jappostal\" id=\"decoder_japanese_postal\" type=\"boolean\"></param>" + 
					 "	<param alias=\"korean_3of5\" id=\"decoder_korean_3of5\" type=\"boolean\"></param>" + 
					 "	<param alias=\"korean3of5\" id=\"decoder_korean_3of5\" type=\"boolean\"></param>" + 
					 "	<param alias=\"maxicode\" id=\"decoder_maxicode\" type=\"boolean\"></param>" + 
					 "	<param alias=\"micropdf\" id=\"decoder_micropdf\" type=\"boolean\"></param>" + 
					 "	<param alias=\"microqr\" id=\"decoder_microqr\" type=\"boolean\"></param>" + 
					 "	<param alias=\"msi\" id=\"decoder_msi\" type=\"boolean\"></param>" + 
					 "	<param alias=\"msicheckdigits\" id=\"decoder_msi_check_digit\" type=\"integer\">" + 
					 "		<translate id=\"one\">0</translate>" + 
					 "		<translate id=\"two\">1</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"msicheckdigitscheme\" id=\"decoder_msi_check_digit_scheme\" type=\"integer\">" + 
					 "		<translate id=\"mod_11_10\">0</translate>" + 
					 "		<translate id=\"mod_10_10\">1</translate>" + 
					 "		<translate id=\"mod11\">0</translate>" + 
					 "		<translate id=\"mod10\">1</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"msimaxlength\" id=\"decoder_msi_length2\" type=\"integer\"></param>" + 
					 "	<param alias=\"msiminlength\" id=\"decoder_msi_length1\" type=\"integer\"></param>" + 
					 "	<param alias=\"msiredundancy\" id=\"decoder_msi_redundancy\" type=\"boolean\"></param>" + 
					 "	<param alias=\"msireportcheckdigit\" id=\"decoder_msi_report_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"pdf417\" id=\"decoder_pdf417\" type=\"boolean\"></param>" + 
					 "	<param alias=\"qrcode\" id=\"decoder_qrcode\" type=\"boolean\"></param>" + 
					 "	<param alias=\"tlc39\" id=\"decoder_tlc39\" type=\"boolean\"></param>" + 
					 "	<param alias=\"trioptic39\" id=\"decoder_trioptic39\" type=\"boolean\"></param>" + 
					 "	<param alias=\"ukpostal\" id=\"decoder_uk_postal\" type=\"boolean\"></param>" + 
					 "	<param alias=\"ukpostalreportcheckdigit\" id=\"decoder_uk_postal_report_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upc_eanbookland\" id=\"upcean_bookland\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upceanbookland\" id=\"upcean_bookland\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upc_eanconvertrsstoupcean\" id=\"databar_to_upc_ean\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upceanconvertrsstoupcean\" id=\"databar_to_upc_ean\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upc_eancoupon\" id=\"upcean_coupon\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upceancoupon\" id=\"upcean_coupon\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upc_eanretrycount\" id=\"upcean_retry_count\" type=\"integer\"></param>" + 
					 "	<param alias=\"upceanretrycount\" id=\"upcean_retry_count\" type=\"integer\"></param>" + 
					 "	<param alias=\"upc_eansecuritylevel\" id=\"upcean_security_level\" type=\"integer\">" + 
					 "		<translate id=\"none\">-2</translate>" + 
					 "		<translate id=\"ambiguous\">-2</translate>" + 
					 "		<translate id=\"all\">-2</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"upceansecuritylevel\" id=\"upcean_security_level\" type=\"integer\">" + 
					 "		<translate id=\"none\">-2</translate>" + 
					 "		<translate id=\"ambiguous\">-2</translate>" + 
					 "		<translate id=\"all\">-2</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"upc_eansupplemental2\" id=\"upcean_supplemental2\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upc_eansupplemental5\" id=\"upcean_supplemental5\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upc_eansupplementalmode\" id=\"upcean_supplemental_mode\" type=\"integer\">" + 
					 "		<translate id=\"none\">0</translate>" + 
					 "		<translate id=\"always\">1</translate>" + 
					 "		<translate id=\"auto\">2</translate>" + 
					 "		<translate id=\"smart\">3</translate>" + 
					 "		<translate id=\"378_379\">4</translate>" + 
					 "		<translate id=\"378or379\">4</translate>" + 
					 "		<translate id=\"978_979\">5</translate>" + 
					 "		<translate id=\"978or979\">5</translate>" + 
					 "		<translate id=\"414_419_434_439\">6</translate>" + 
					 "		<translate id=\"414or419or434or439\">6</translate>" + 
					 "		<translate id=\"977\">7</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"upceansupplemental2\" id=\"upcean_supplemental2\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upceansupplemental5\" id=\"upcean_supplemental5\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upceansupplementalmode\" id=\"upcean_supplemental_mode\" type=\"integer\">" + 
					 "		<translate id=\"none\">0</translate>" + 
					 "		<translate id=\"always\">1</translate>" + 
					 "		<translate id=\"auto\">2</translate>" + 
					 "		<translate id=\"smart\">3</translate>" + 
					 "		<translate id=\"378_379\">4</translate>" + 
					 "		<translate id=\"378or379\">4</translate>" + 
					 "		<translate id=\"978_979\">5</translate>" + 
					 "		<translate id=\"978or979\">5</translate>" + 
					 "		<translate id=\"414_419_434_439\">6</translate>" + 
					 "		<translate id=\"414or419or434or439\">6</translate>" + 
					 "		<translate id=\"977\">7</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"upca\" id=\"decoder_upca\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upcapreamble\" id=\"decoder_upca_preamble\" type=\"integer\">" + 
					 "		<translate id=\"none\">0</translate>" + 
					 "		<translate id=\"system_char\">1</translate>" + 
					 "		<translate id=\"country_and_system_chars\">2</translate>" + 
					 "		<translate id=\"none\">0</translate>" + 
					 "		<translate id=\"systemchar\">1</translate>" + 
					 "		<translate id=\"countryandsystemchars\">2</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"upcareportcheckdigit\" id=\"decoder_upca_report_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upce0\" id=\"decoder_upce0\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upce0converttoupca\" id=\"decoder_upce0_convert_to_upca\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upce0preamble\" id=\"decoder_upce0_preamble\" type=\"integer\">" + 
					 "		<translate id=\"none\">0</translate>" + 
					 "		<translate id=\"system_char\">1</translate>" + 
					 "		<translate id=\"country_and_system_chars\">2</translate>" + 
					 "		<translate id=\"none\">0</translate>" + 
					 "		<translate id=\"systemchar\">1</translate>" + 
					 "		<translate id=\"countryandsystemchars\">2</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"upce0reportcheckdigit\" id=\"decoder_upce0_report_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upce1\" id=\"decoder_upce1\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upce1converttoupca\" id=\"decoder_upce1_convert_to_upca\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upce1preamble\" id=\"decoder_upce1_preamble\" type=\"integer\">" + 
					 "		<translate id=\"none\">0</translate>" + 
					 "		<translate id=\"system_char\">1</translate>" + 
					 "		<translate id=\"country_and_system_chars\">2</translate>" + 
					 "		<translate id=\"none\">0</translate>" + 
					 "		<translate id=\"systemchar\">1</translate>" + 
					 "		<translate id=\"countryandsystemchars\">2</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"upce1reportcheckdigit\" id=\"decoder_upce1_report_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"us4state\" id=\"decoder_us4state\" type=\"boolean\"></param>" + 
					 "	<param alias=\"us4statefics\" id=\"decoder_us4state_fics\" type=\"boolean\"></param>" + 
					 "	<param alias=\"usplanet\" id=\"decoder_usplanet\" type=\"boolean\"></param>" + 
					 "	<param alias=\"usplanetreportcheckdigit\" id=\"decoder_usplanet_report_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"uspostnet\" id=\"decoder_uspostnet\" type=\"boolean\"></param>" + 
					 "	<param alias=\"uspostnetreportcheckdigit\" id=\"decoder_uspostnet_report_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"webcode\" id=\"decoder_webcode\" type=\"boolean\"></param>" + 
					 "	<param alias=\"webcodedecodegtsubtype\" id=\"decoder_webcode_subtype\" type=\"boolean\"></param>" + 
					 "	" + 
					 "	<!-- UNSUPPORTED DECODERS AND DECODER SETTINGS -->" + 
					 "	<param alias=\"code128other128\" id=\"decoder_code128_enable_plain\" type=\"boolean\"></param>" + 
					 "	<param alias=\"composit_abuseupcpreamblecheckdigitrules\" id=\"\" type=\"deprecated\"></param>" + 
					 "	<param alias=\"compositeabuseupcpreamblecheckdigitrules\" id=\"\" type=\"deprecated\"></param>" + 
					 "	<param alias=\"ean8converttoean13\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"korean_3of5redundancy\" id=\"decoder_korean_3of5_redundancy\" type=\"boolean\"></param>" + 
					 "	<param alias=\"korean3of5redundancy\" id=\"decoder_korean_3of5_redundancy\" type=\"boolean\"></param>" + 
					 "	<param alias=\"macromicropdf\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"macropdf\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"signature\" id=\"decoder_signature\" type=\"boolean\"></param>" + 
					 "	<param alias=\"trioptic39redundancy\" id=\"decoder_trioptic39_redundancy\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upc_eanbooklandformat\" id=\"upcean_bookland_format\" type=\"integer\">" + 
					 "		<translate id=\"isbn_10\">0</translate>" + 
					 "		<translate id=\"isbn_13\">1</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"upc_eanlineardecode\" id=\"upcean_linear_decode\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upc_eanrandomweightcheckdigit\" id=\"upcean_random_weight_check_digit\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upceanbooklandformat\" id=\"upcean_bookland_format\" type=\"integer\">" + 
					 "		<translate id=\"isbn_10\">0</translate>" + 
					 "		<translate id=\"isbn_13\">1</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"upceanlineardecode\" id=\"upcean_linear_decode\" type=\"boolean\"></param>" + 
					 "	<param alias=\"upceanrandomweightcheckdigit\" id=\"upcean_random_weight_check_digit\" type=\"boolean\"></param>" + 
					 "	" + 
					 "	<param alias=\"macromicropdfbufferlabels\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"macromicropdfexclusive\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"macromicropdfreportappendedinfo\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"macropdfbufferlabels\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"macropdfconverttopdf417\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"macropdfexclusive\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"code39concatenation\" id=\"\" type=\"deprecated\"></param>" + 
					 "	" + 
					 "	<!-- SUPPOERTED MODES -->" + 
					 "	<param alias=\"illuminationmode\" id=\"illumination_mode\" type=\"string\">" + 
					 "		<translate id=\"alwaysoff\">off</translate>" + 
					 "		<translate id=\"alwayson\">torch</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"linearsecuritylevel\" id=\"linear_security_level\" type=\"integer\">" + 
					 "		<translate id=\"redundancyandlength\">0</translate>" + 
					 "		<translate id=\"shortorcodabar\">1</translate>" + 
					 "		<translate id=\"longandshort\">3</translate>" + 
					 "		<translate id=\"alltwice\">2</translate>" + 
					 "		<translate id=\"allthrice\">4</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"picklistmode\" id=\"picklist\" type=\"integer\">" + 
					 "		<translate id=\"disabled\">0</translate>" + 
					 "		<translate id=\"hardwarereticle\">1</translate>" + 
					 "		<translate id=\"softwarereticle\">2</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"scantimeout\" id=\"beam_timer\" type=\"integer\"></param>" + 
					 "	<param alias=\"viewfindermode\" id=\"viewfinder_mode\" type=\"integer\">" + 
					 "		<translate id=\"disabled\">0</translate>" + 
					 "		<translate id=\"enabled\">1</translate>" + 
					 "		<translate id=\"staticreticle\">2</translate>" + 
					 "		<translate id=\"dynamicreticle\">-1</translate>" + 
					 "	</param>" + 
					 "	" + 
					 "	<!-- UNSUPPORTED MODES -->" + 
					 "	<param alias=\"viewfinderfeedbacktime\" id=\"viewfinder_feedback_time\" type=\"integer\"></param>" + 
					 "	<param alias=\"viewfinderfeedback\" id=\"viewfinder_feedback_mode\" type=\"integer\">" + 
					 "		<translate id=\"enabled\">1</translate>" + 
					 "		<translate id=\"disabled\">0</translate>" + 
					 "		<translate id=\"reticle\">-1</translate>" + 
					 "	</param>" + 
					 "	<param alias=\"inverse1dmode\" id=\"inverse_1d_mode\" type=\"integer\">" + 
					 "		<translate id=\"disabled\">0</translate>" + 
					 "		<translate id=\"enabled\">1</translate>" + 
					 "		<translate id=\"auto\">2</translate>" + 
					 "	</param>" + 
					 "	" + 
					 "	<param alias=\"aimmode\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"aimtype\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"bidirectionalredundancy\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"connectionidletimeout\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"dbpmode\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"differentsymboltimeout\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"disconnectbtondisable\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"displaybtaddressbarcodeonenable\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"dpmmode\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"focusmode\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"klasseeins\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"narrowbeam\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"poorquality1dmode\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"rasterheight\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"rastermode\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"samesymboltimeout\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"signatureimageheight\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"signatureimagequality\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"signatureimagesize\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"signatureimagewidth\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"timedaimduration\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"viewfinderheight\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"viewfinderwidth\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"viewfinderx\" id=\"\" type=\"\"></param>" + 
					 "	<param alias=\"viewfindery\" id=\"\" type=\"\"></param>" + 
					 "</scanner-params>";

	public void onConnectionChange(Scanner arg0, ScannerConnection connection, ScannerInfo info, int arg3)
	{
		//Required for SDK 1.3.3
//		switch (connection)
//		{
//			case SCANNER_CONNECTED:
//			{
//					/*TODO Dunno what to do here yet*/
				//Test Code
//				if ( mScanStatus == ScanStatus.SCAN_STATUS_IDLE)
//				{
//					try
//					{
//						mScanner.enable();
//					}
//					catch (ScannerExceptions e)
//					{
//						Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "ScannerException"));
//					}
//				}
//			}
//		}
	}
	
	public class ScannerTask extends Thread implements Runnable
	{
		private ScannerUser currentUser;
		private ArrayList<ScannerUser> openStack;
		//private boolean isEnable;
		private ScannerInstruction instruction;

		public ScannerTask(ScannerUser currentUser, ArrayList<ScannerUser> openStack, ScannerInstruction instruction)
		{
			this.currentUser = currentUser;
			this.openStack = openStack;
			this.instruction = instruction;
		}

		@Override
		public void run()
		{
			
		    switch (instruction.getTask())
		    {
        		    case ENABLE:
        		    {
        			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Enabling Scanner"));
        			synchronized(openStack)
				{
					try
					{									
						enable(currentUser);
						if(openStack.size() == 0)
						{
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Adding open scanner user"));
							openStack.add(currentUser);
							try
							{
								scanner.setParameters(currentUser.getParameters());
							}
							catch (ScannerExceptions e)
							{
								Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unsuccessful scanner parameter change: " + e.getMessage()));
							}
						}
						else if(!openStack.get(openStack.size() - 1).equals(currentUser.getClient()))
						{
							//If already on the stack but not currently on top, reload onto the top.
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Removing open scanner user"));
							openStack.remove(currentUser);
							Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Adding open scanner user"));
							openStack.add(currentUser);
							try
							{
								scanner.setParameters(currentUser.getParameters());
							}
							catch (ScannerExceptions e)
							{
								Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unsuccessful scanner parameter change: " + e.getMessage()));
							}
						}
					}
					catch (ScannerExceptions e1)
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Could not enable scanner: SCN" + (currentUser.getScannerId() + 1)));
					}
					catch (NullPointerException e)
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
					}
					//else already scanning
				}
				break;
        		    }
        		    case DISABLE:
        		    {
        			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Disabling Scanner"));	
				synchronized(openStack)
				{
					disable(currentUser);
					
//					openStack.get(openStack.size() -1).setScannerEnabled(false);
					
//					boolean scannerInUse = false;
//					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Searching user stack"));
//					for(ScannerUser user: userStack)
//					{
//						scannerInUse |= user.getScannerEnabled();
//					}
//					Common.elementsCore.setScannerEnabled(scannerInUse);
					
				}
        			break;
        		    }
        		    case START:
        		    {
        			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Starting Scanner"));
        			synchronized (openStack)
        			{
                			synchronized (globalLockObject)
        				{	
                        			try
                				{
                				    if(scanner != null) 
                					scanner.startScanning(EmdkScannerServiceRebranded.this);
                				}
                				catch (ScannerExceptions e)
                				{
                					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Could not start scanning"));
                				}
        				}
        			}
        			break;
        		    }
        		    case STOP:
        		    {
        			synchronized (openStack)
        			{
        			    if(scanner != null) 
        				scanner.stopScanning();
        			}
        			break;
        		    }
        		    case SETPARAMS:
        		    {
        			synchronized (openStack)
        			{
        			    setDecodeSetting(currentUser.getClient(), instruction.arg1, instruction.arg2);
        			}
        			break;
        		    }
        		    default:
        			break;
		    }
		}
	}

}
