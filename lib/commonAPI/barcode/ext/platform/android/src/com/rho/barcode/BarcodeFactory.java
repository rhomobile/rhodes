package com.rho.barcode;

import java.io.File;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Iterator;
import java.util.Map;

import android.annotation.SuppressLint;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.Camera;
import android.os.BatteryManager;
import android.view.KeyEvent;
import com.google.zxing.Result;
import com.rho.barcode.emdk3.EMDK3ExtensionListener;
import com.rho.barcode.emdk3.EMDK3Scanner;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.RhoApiFactory;
import com.rhomobile.rhodes.util.ContextFactory;
import com.symbol.emdk.barcode.BarcodeManager;
import com.rhomobile.rhodes.RhoConf;

/**
 * Barcode Factory class
 * @author Ben Kennedy (NCVT73)
 */
public class BarcodeFactory extends RhoApiFactory<Barcode, BarcodeSingleton> implements IBarcodeFactory
{
	private static String TAG = "BarcodeFactory";
	private static boolean isZxingOnMotoAllowed = false;	
	
	private static BarcodeFactory factoryInstance;
	public static float batPercent;
	private HashMap<String, Barcode> barcodeObjects;
	private String[] emdkIds;
	private String[] zxingIds;
	private static String enabledScanner;
	private static int lastDecodeVolume = -1;
	private static int lastDecodeFrequency = -1;
	private static int lastDecodeDuration = -1;
	private static File lastWavFile = null;
	private int defaultEMDKScanner = -1;
	protected final int KEYCODE_TRIGGER_1 = 102;
	protected final int KEYCODE_TRIGGER_2 = 103;
	private int lastKeyAction;
	private StartScannerTask startTask;
	private StopScannerTask stopTask;
	private boolean isPaused;
	private int devicesEmdkVersion;
	private EMDK3ExtensionListener emdk3Listener;
	private boolean isEMDKScannerSetupCalled = false;
	private ArrayList<EMDK3Scanner> emdk3ScannerSetupQueue;
	private boolean isEmdk3ScannerSettingUp = false;

	private boolean releaseEMDKOnPause = false;
	private static final String CFG_RELEASE_EMDK = "Barcode.ReleaseEMDKOnPause";
	
	private List<com.symbol.emdk.barcode.ScannerInfo> scannerInfos = null;
	public BarcodeFactory(boolean useDW)
	{
		super();
		Logger.D(TAG, "BarcodeFactory constructor");
		emdk3ScannerSetupQueue = new ArrayList<EMDK3Scanner>();
		devicesEmdkVersion = getEMDKDeviceVersion();
		Logger.D(TAG, "EMDK Device Version " + devicesEmdkVersion);
		
		BarcodeFactory.factoryInstance = this;
		barcodeObjects = new HashMap<String, Barcode>();
		
		setupZxingScannerIds();
		
		releaseEMDKOnPause = RhoConf.isExist(CFG_RELEASE_EMDK) && RhoConf.getBool(CFG_RELEASE_EMDK);
			
		if(devicesEmdkVersion == 3)//TODO change to a constant.
		{
				if(useDW)//
				{
				Logger.I(TAG, "BarcodeFactory cons-do not setup EMDK");
				}
				else
				{
					Logger.I(TAG, "BarcodeFactory cons-setup EMDK");
					setupEMDK3();
					Logger.I(TAG, "BarcodeFactory cons-EMDK is setup");
				}
		}
		
		
	

		startTask = new StartScannerTask();
		stopTask = new StopScannerTask();
		lastKeyAction = -1;
		setPaused(false);
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
	
	private void setupEMDK3()
	{
		emdk3Listener = new EMDK3ExtensionListener(this);
	}

	/**
	 * Sets up the EMDK (Zebra) barcode objects
	 * @author Ben Kennedy (NCVT73)
	 */
	public void setupEmdkScannerIds()
	{
		Logger.D(TAG, "setupEmdkScannerIds+");
		switch(devicesEmdkVersion)
		{
			case 3:
				emdkIds = getNumberOfEMDK3Scanners();
				for(int i = 0; i < emdkIds.length; i++)
				{
					emdkIds[i] = "EMDK" + i;
					getApiObject("EMDK" + i); //Initialise EMDK3 ScannerConfig so that we can get parameters when not enabled
				}
				Logger.D(TAG, "setupEmdkScannerIds EMDK3 scanners: " + emdkIds.length);
				break;
			case 1:
				
				break;
			case 0:
				emdkIds = null;
				break;
		}
		Logger.D(TAG, "setupEmdkScannerIds-");
	}
	
//	/**
//	 * Sets up the ZXing barcode objects
//	 * @author Ben Kennedy (NCVT73)
//	 */
//	private void setupZxingScannerIds()
//	{
//		Logger.D(LOGTAG, "setupZxingScannerIds");
//	
//		zxingIds = new String[Camera.getNumberOfCameras()];
//		for(int i = 0; i < zxingIds.length; i++)
//		{
//			
//			zxingIds[i] = "ZXING" + i;
//		}
//		Logger.D(LOGTAG, "setupZxingScannerIds cameras: " + zxingIds.length);
//	}
	
	private void setupZxingScannerIds()
	{
		Logger.D(TAG, "setupZxingScannerIds");
		
		//If it is a Zebra Device and ZXing is not allowed on these devices, then stop.
		if(!isZxingOnMotoAllowed && devicesEmdkVersion > 0) return;
		
		int noOfCameras=Camera.getNumberOfCameras();
		Camera.CameraInfo info =new Camera.CameraInfo();
		for(int i=0;i<noOfCameras;i++)
		{
			Camera.getCameraInfo(i, info);
			if (info.facing == Camera.CameraInfo.CAMERA_FACING_BACK) 
			{
				if (zxingIds==null)
					zxingIds=new String[1];
				else
					zxingIds=Arrays.copyOf(zxingIds,zxingIds.length+1);
				zxingIds[zxingIds.length-1] = "ZXING" + i;
				System.out.println("new one= "+zxingIds[zxingIds.length-1]);
			}
		}
		
		
		
		/*if(noOfCameras>0)
		{
			//Allow only back camera for scanning and not front-facing camera for consumer device
			zxingIds = new String[1];
			zxingIds[0] = "ZXING" + 0;
		}*/
		
		Logger.D(TAG, "setupZxingScannerIds cameras: " + ((zxingIds==null)?0:zxingIds.length));
	}
	
	
	
	@Override
	protected BarcodeSingleton createSingleton()
	{
		Logger.D(TAG, "createSingleton");
		return new BarcodeSingleton(this);
	}

	@Override
	protected Barcode createApiObject(String id)
	{
		Logger.D(TAG, "createApiObject+ " + id);
		
		Barcode requestedBarcode = barcodeObjects.get(id);
		if(requestedBarcode == null)
		{
			if(id.startsWith("EMDK") && devicesEmdkVersion == 3)
			{
				//XXX Shall we call the EMDK devices EMDK? How else will users detect the difference between the EMDK3 & Zxing scanners?
				EMDK3Scanner emdk3RequestedBarcode = new EMDK3Scanner(id, emdk3Listener.getBarcodeManager().getDevice(scannerInfos.get(Integer.parseInt(id.substring(4)))), this);
				//TODO emdk3ScannerSettingUp Sync
				if(isEmdk3ScannerSettingUp)
				{
					addScannerToEMDK3ScannerSetupQueue(emdk3RequestedBarcode);
				}
				else
				{
					isEmdk3ScannerSettingUp = emdk3RequestedBarcode.setupScanner();
				}
				requestedBarcode = emdk3RequestedBarcode;
			}

			else if(id.startsWith("ZXING"))
			{
				Logger.D(TAG, "createApiObject creating ZXingScanner");
				requestedBarcode = new ZXingScanner(id);
				Logger.D(TAG, "createApiObject created ZXingScanner");
			}

			barcodeObjects.put(id, requestedBarcode);
		}

		Logger.D(TAG, "createApiObject-");

		return requestedBarcode;
	}
	
	public synchronized void emdk3BarcodeHasSetup()
	{
		Logger.D(TAG, "emdk3BarcodeHasSetup+");
		isEmdk3ScannerSettingUp = false;
		while(emdk3ScannerSetupQueue.size() > 0 && !isEmdk3ScannerSettingUp)
		{
			Logger.D(TAG, "emdk3BarcodeHasSetup loop");
			isEmdk3ScannerSettingUp = emdk3ScannerSetupQueue.remove(0).setupScanner();
		}
		Logger.D(TAG, "emdk3BarcodeHasSetup-");
	}
	
	public void addScannerToEMDK3ScannerSetupQueue(EMDK3Scanner emdk3Scanner)
	{
		Logger.D(TAG, "addScannerToEMDK3ScannerSetupQueue");
		emdk3ScannerSetupQueue.add(emdk3Scanner);
	}
	
	public static int getEMDKDeviceVersion()
	{
		Logger.D(TAG, "getEMDKDeviceVersion");
		try
		{
			Class.forName("com.symbol.emdk.barcode.Scanner", false, ContextFactory.getAppContext().getClassLoader());
			Logger.D(TAG, "is EMDK 3 device");
			return 3;
		}
		catch (ClassNotFoundException e)
		{
			Logger.D(TAG, "not EMDK 2 Device");
		}
		
		return 0;
	}

	public List<String> getIds()
	{
		Logger.D(TAG, "getIds");
		return enumerate();
	}
	
	/**
	 * Returns the scanner object identified by the scannerId. Returns null if not found
	 * @param scannerId The ID of the barcode object to fetch
	 * @return The barcode object requested
	 * @author Ben Kennedy (NCVT73)
	 */
	public Barcode getScanner(String scannerId)
	{
		Logger.D(TAG, "getScanner");
		return barcodeObjects.get(scannerId);
	}

	/**
	 * Enumerates all of the available EMDK and ZXing scanners
	 * @return a String List of the Scanner IDs
	 * @author Ben Kennedy (NCVT73)
	 */
	public List<String> enumerate()
	{
		Logger.D(TAG, "enumerate");
		
		List<String> barcodeIdStrings = new ArrayList<String>();
		if(emdkIds != null && emdkIds.length > 0) barcodeIdStrings.addAll(Arrays.asList(emdkIds));
		if(zxingIds != null && zxingIds.length > 0) barcodeIdStrings.addAll(Arrays.asList(zxingIds));
		//TODO add real time enumeration of scanners, if more get added at runtime
		
		return barcodeIdStrings;
	}
	
	/**
	 * Called by the ZXing Activity when the OK button is pressed.
	 * @param barcodeData an object containing the decoded barcode string and meta data
	 * @param barcodeObjectId the ID of the ZXingScanner to which to send the barcode
	 * @author Ben Kennedy (NCVT73)
	 */
	public static void callOKCallback(Result barcodeData, String barcodeObjectId)
	{
		Logger.D(TAG, "callOKCallback");
		if(barcodeObjectId.equals(factoryInstance.getEnabledScanner()))
		{
			Logger.D(TAG, "callOKCallback: passed");
			ZXingScanner zxingScanner = ((ZXingScanner)factoryInstance.getScanner(barcodeObjectId));
			if(zxingScanner != null) zxingScanner.decodeEvent(barcodeData);
			else Logger.E(TAG, "Zebra Crossing Object cuold not be found");
		}
		else
		{
			Logger.D(TAG, "callOKCallback: Failed: " + barcodeObjectId + " vs " + factoryInstance.getEnabledScanner());
		}
	}
	
	/**
	 * Called by the ZXing Activity when the Cancel button is pressed.
	 * @param barcodeObjectId the ID of the ZXingScanner to which to send the cancel button press
	 * @author Ben Kennedy (NCVT73)
	 */
	public static void callCancelCallback(String barcodeObjectId)
	{
		Logger.D(TAG, "callCancelCallback");
		ZXingScanner zxingScanner = ((ZXingScanner)factoryInstance.getScanner(barcodeObjectId));
		if(zxingScanner != null) zxingScanner.cancel();
		else Logger.E(TAG, "Zebra Crossing Object cuold not be found");
	}

	/**
	 * Called by the ZXing Activity when the ZXing window encounters an error is pressed.
	 * @param errorMessage The error message created by the ZXing window.
	 * @param barcodeObjectId the ID of the ZXingScanner to which to send the cancel button press
	 * @author Ben Kennedy (NCVT73)
	 */
	public static void callErrorCallback(String errorMessage, String barcodeObjectId)
	{
		Logger.D(TAG, "callErrorCallback");
		ZXingScanner zxingScanner = ((ZXingScanner)factoryInstance.getScanner(barcodeObjectId));
		if(zxingScanner != null) zxingScanner.error(errorMessage);
		else Logger.E(TAG, "Zebra Crossing Object cuold not be found");
	}
	
	/**
	 * As we have the limitation that only one scanner can be active at any one time, this method registers the
	 * last enabled call with the singleton. If a different object calls enabled then the previously enabled
	 * scanner will be disabled.
	 * @param scanner the scanner that is being enabled;
	 * @author Ben Kennedy (NCVT73)
	 */
	public static void setEnabledState(String scannerId)
	{
		Logger.D(TAG, "setEnabledState");
		//If there is a different scanner enabled, disable it.
		if(enabledScanner != null && !enabledScanner.equals(scannerId))
		{
			Barcode scanner = factoryInstance.getScanner(enabledScanner);
			if(scanner != null) scanner.disable(null);
			else Logger.E(TAG, "Scanner could not be disabled as the object could not be found");
		}
		enabledScanner = scannerId;
	}

	/**
	 * @return the enabled Scanner ID
	 * @author Ben Kennedy (NCVT73)
	 */
	private String getEnabledScanner()
	{
		return enabledScanner;
	}
	
	/**
	 * As we have the limitation that only one scanner can be active at any one time, this method deregisters the
	 * enabled scanner, as long as it is the enabled scanner calling the method. If this is the case then no scanner
	 * will be enabled.
	 * @param scanner the scanner that is being disabled;
	 * @author Ben Kennedy (NCVT73)
	 */
	public static void setDisabledState(String scannerId)
	{
		Logger.D(TAG, "setDisabledState");
		if(scannerId.equals(enabledScanner))
		{
			//factoryInstance.getScanner(scannerId).disable(null);
			enabledScanner = null;
			factoryInstance.setLastKeyAction(KeyEvent.ACTION_UP);
		}
		else
		{
			//Ignore, nothing needs doing as it was disabled before
		}
	}
	
	/**
	 * A Mutator method for the last key action. This saves whether the last trigger was a trigger down or a trigger
	 * up message
	 * @param action whether it was a KeyEvent.ACTION_UP or KeyEvent.ACTION_DOWN event.
	 */
	private void setLastKeyAction(int action)
	{
		lastKeyAction = action;
	}

	/**
	 * Returns the number of EMDK (EMDK  Add-on, aka EMDK 3) scanners present on the device.
	 * Only call this if getEMDKDeviceVersion returns 3
	 * @author Ben Kennedy (NCVT73)
	 */
	
	public String[] getNumberOfEMDK3Scanners()
	{
		Logger.D(TAG, "getNumberOfEMDK3Scanners+");
		/* Will be called once the EMDKListener gets the emdkManager */ 
		ArrayList<String> scannerList = new ArrayList<String>();
		String[] returnArray;
		
		BarcodeManager barcodeManager = emdk3Listener.getBarcodeManager();

		scannerInfos = barcodeManager.getSupportedDevicesInfo();
		Logger.D(TAG, "Number of EMDK3 Scanners found: " + scannerInfos.size());
		for(int i = 0; i < scannerInfos.size(); i++)
		{
			if(scannerInfos.get(i).isDefaultScanner())
			{
				defaultEMDKScanner = i;
			}
			scannerList.add("" + i);
		}
		
		returnArray = new String[scannerList.size()];
		return scannerList.toArray(returnArray);
	}

	

	

	/**
	 * Returns the default emdk scanner number
	 * @return The default EMDK scanner number
	 */
	public int getDefaultEMDKScanner()
	{
		return defaultEMDKScanner;
	}
	
	public boolean onKey(int keyCode, KeyEvent event)
	{
		//Logger.D(TAG, " onKey+ enabledScanner = " + enabledScanner);
		//TODO enabledScanner is not being populated by EMDK3 Scanner
		if(enabledScanner != null)
		{
			if (((keyCode == KEYCODE_TRIGGER_1) || (keyCode == KEYCODE_TRIGGER_2)) && event.getRepeatCount() == 0)
			{
				boolean isDown = (event.getAction() == KeyEvent.ACTION_DOWN);
				Logger.D(TAG, "Caught Trigger for the Scanner: " + isDown);
				if (lastKeyAction != event.getAction())
				{
				    if(isDown)
				    {
				    	Thread startThread = new Thread(startTask);
				    	startThread.start();
				    }
				    else
				    {
				    	Thread stopThread = new Thread(stopTask);
				    	stopThread.start();
				    }
				}
				if (isDown)	lastKeyAction = KeyEvent.ACTION_DOWN;
				else		lastKeyAction = KeyEvent.ACTION_UP;
			}
		}
		return false;
	}
	
	/**
	 * A Runnable class that sends a start EMDK scanner command (created from trigger presses)
	 * @author Ben Kennedy (NCVT73)
	 */
	private class StartScannerTask implements Runnable
	{
		@Override
		public void run()
		{
			Barcode scanner = getScanner(enabledScanner);
			if(scanner != null) scanner.start(null);
			else Logger.E(TAG, "Scanner could not be started as the object could not be found");
		}	
	}
	
	/**
	 * A Runnable class that sends a stop EMDK scanner command (created from trigger presses)
	 * @author Ben Kennedy (NCVT73)
	 */
	private class StopScannerTask implements Runnable
	{
		@Override
		public void run()
		{
			Barcode scanner = getScanner(enabledScanner);
			if(scanner != null) scanner.stop(null);
			else Logger.E(TAG, "Scanner could not be stopped as the object could not be found");
		}	
	}
	
	/**
	 * Generates a tone wave file described by the parameters
	 * @param decodeVolume the volume of the tone, ranged 0-5 (5 loudest)
	 * @param decodeFrequency the frequency of the tone, range 0-65535
	 * @param decodeDuration the duration of the tone, in milliseconds
	 * @return the generated wave file or null if failed
	 * @author Ben Kennedy (NCVT73)
	 */
	public static File generateTone(int decodeVolume, int decodeFrequency, int decodeDuration)
	{	
		if(decodeVolume != lastDecodeVolume || decodeFrequency != lastDecodeFrequency || decodeDuration != lastDecodeDuration )
		{
			ToneFileFactory toneWavGenerator = new ToneFileFactory(decodeFrequency, decodeVolume, decodeDuration);
			File wavFile = toneWavGenerator.generateWavFile();
			if(wavFile != null && wavFile.exists())
			{
				lastWavFile = wavFile; //Not really needed (as it is a reference to the overwritten file), but just in case 
				return wavFile;
			}
		}
		else
		{
			return lastWavFile;//Cached version
		}
		return null;
	}
	
	/**
	 * Returns whether the application is paused (i.e. minimized)
	 * @return whether the application is paused
	 * @author Ben Kennedy (NCVT73)
	 */
	public static boolean isPaused()
	{
		return factoryInstance.isPaused;
	}

	public static void setPaused(boolean isPaused)
	{
		factoryInstance.isPaused = isPaused;
	}
	
	public void onResume()
	{
		Logger.D(TAG, "onResume+");

		setPaused(false);
		RhodesActivity.getContext().registerReceiver(this.batteryInfoReceiver,	new IntentFilter(Intent.ACTION_BATTERY_CHANGED));

		if (releaseEMDKOnPause) {		
			Logger.D( TAG, "Will restore EMDK resources" );
			emdk3Listener.resume();			
		}

		if(enabledScanner != null)
		{
			getScanner(enabledScanner).onResume();
		}

		Logger.D(TAG, "onResume-");
	}

	private void removeAllEMDKScannerIDs() 
	{
		for(Iterator<Map.Entry<String, Barcode>> it = barcodeObjects.entrySet().iterator(); it.hasNext(); )
		{
      		Map.Entry<String, Barcode> entry = it.next();
      		final String scannerID = entry.getKey();

      		if(scannerID.startsWith("EMDK"))
      		{
			    try
			    {
      				EMDK3Scanner scanner = (EMDK3Scanner) entry.getValue();
      				it.remove();
      				removeApiObject(scannerID);
      			}
      			catch ( ClassCastException e )
      			{
      				Logger.E(TAG, "While cleaning up scanner ID " + scannerID + ": not an actual EMDK scanner!");
      			}        		
      		}
    	}
	}

	public void onPause()
	{
		Logger.D(TAG, "onPause+");

		setPaused(true);
		if(enabledScanner != null)
		{
			getScanner(enabledScanner).onPause();			
		}

		if (releaseEMDKOnPause) {		
			Logger.D( TAG, "Will release EMDK resources" );
			enabledScanner = null;
			removeAllEMDKScannerIDs();
			emdk3Listener.pause();
			setIsEMDKScannerSetupCalled(false);
		}

		Logger.D(TAG, "onPause-");
	}

	public void onStop()
	{
		Logger.D(TAG, "onStop+");

		if(enabledScanner != null)
		{
			getScanner(enabledScanner).onStop();
		}

		Logger.D(TAG, "onStop-");
	}

	public void onDestroy()
	{
		Logger.D(TAG, "onDestroy+");

		if(enabledScanner != null)
		{
			getScanner(enabledScanner).onDestroy();
		}

		RhodesActivity.getContext().unregisterReceiver(batteryInfoReceiver);

		Logger.D(TAG, "onDestroy-");
	}

	public boolean isEMDKScannerSetupCalled()
	{
		return isEMDKScannerSetupCalled;
	}

	public void setIsEMDKScannerSetupCalled(boolean b)
	{
		isEMDKScannerSetupCalled = b;
	}
	public void disableScannerOnNavigate()
	{
		if(enabledScanner != null)
		{
			getScanner(enabledScanner).disable(null);
		}
	}
	
}
