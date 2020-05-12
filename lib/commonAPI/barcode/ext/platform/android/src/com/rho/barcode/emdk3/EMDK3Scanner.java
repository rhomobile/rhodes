package com.rho.barcode.emdk3;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import android.annotation.SuppressLint;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.AsyncTask;
import android.os.BatteryManager;
import android.util.Pair;

import com.rho.barcode.Barcode;
import com.rho.barcode.BarcodeFactory;
import com.rho.barcode.EMDKCommon;
import com.rho.barcode.EMDKCommon.AutoValue;
import com.rho.barcode.RhoScannerInfo;
import com.rho.barcode.RhoScannerInfo.RhoScannerClass;
import com.rho.barcode.emdk3.EMDK3ScannerSettings.Decoder;
import com.rho.barcode.emdk3.EMDK3ScannerSettings.AllDecoders;
import com.rho.barcode.emdk3.EMDK3ScannerSettings.Reader;
import com.rho.barcode.emdk3.EMDK3ScannerSettings.Scan;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.webview.GoogleWebView;
import com.rhomobile.rhodes.webview.WebViewConfig;
import com.symbol.emdk.barcode.ScanDataCollection;
import com.symbol.emdk.barcode.Scanner;
import com.symbol.emdk.barcode.Scanner.DataListener;
import com.symbol.emdk.barcode.Scanner.StatusListener;
import com.symbol.emdk.barcode.Scanner.TriggerType;
import com.symbol.emdk.barcode.ScannerConfig;
import com.symbol.emdk.barcode.ScannerException;
import com.symbol.emdk.barcode.StatusData;
import com.symbol.emdk.barcode.StatusData.ScannerStates;

/**
 * If you add or change the parameter list (adding a parameter to the Rho.Barcode object, changing
 * support status) you will need to manually change it in these places:
 *  - setProperty
 *  - getProperty
 *  - getSupportedProperties
 *  - EMDK3ScannerSettings
 *  - SettingString
 *  - Defaults (if needed)
 * @author Ben Kennedy (NCVT73)
 *
 */
public class EMDK3Scanner extends Barcode implements DataListener, StatusListener
{
	public enum ENABLE_TYPE
	{
		ENABLE, TAKE
	}

	private static class Defaults
	{
		public static int DECODE_FREQUENCY = 3000;
		public static int DECODE_VOLUME = 5;
		public static int DECODE_DURATION = 250;
		public static boolean AUTOTAB = false;
		public static boolean AUTOENTER = false;
	}

	private static final String TAG = "EMDK3Scanner";
	private float batPercent;
	private boolean isMinimized = false;
	private boolean isEnabled = false;
	private boolean isScanning = false;
	private boolean didTakeEnableTheScanner = false;
	private boolean hasQueuedEnableTask = false;
	private boolean isInitialising = true;
	private boolean isBluetooth = false;
	
	private boolean emdk3_0bugOnStatusFired = false;
	private int emdk3_0onStatusAttempts = 0;
	
	private int setupRetryCount = 0;
	
	private String scannerName;
	private String scannerNameShort;
	
	private ENABLE_TYPE enableType;
	private IMethodResult enableCallback;
	private IMethodResult takeCallback;
	private AutoValue autoValue;
	
	private Scanner scanner;
	private RhoScannerInfo scannerInfo;
	private ScannerConfig scannerConfig;
	private ScannerConfig defaultScannerConfig;
	private ArrayList<String> supportedProperties;
	private ArrayList<Pair<String,String>> propertyQueue;
	
	private final ScheduledExecutorService setupRetryThread = Executors.newScheduledThreadPool(1);
	private final ScheduledExecutorService emdk3_0testerThread = Executors.newScheduledThreadPool(1);

	private BarcodeFactory factory;

	private static ArrayList<String> allProperties = new ArrayList<String>(216);

	@SuppressLint("DefaultLocale")
	/**
	 * This doesnt completely setup this scanner object, you will need to call setupScanner to initialise
	 * the scannerConfig and therefore the parameters. These two methods are seperated because setting up
	 * each scanner is asynchronous, yet only one scanner can be in the process of setting up at one time.
	 * The limitation is specifically only 1 scanner can be enabled, and the scanner must be enabled to 
	 * get the scannerConfig. 
	 * 
	 * @param id the String ID of the Scanner, for example "EMDK0"
	 * @param scanner the EMDK3 scanner object 
	 * @param factory the calling factory
	 * @author Ben Kennedy (NCVT73)
	 */
	public EMDK3Scanner(String id, Scanner scanner, BarcodeFactory factory)
	{
		super(id);	
		Logger.D(TAG, "EMDK3Scanner created: " + id);
		this.scannerId = id;
		this.scanner = scanner;
		this.factory = factory;
		scanner.addDataListener(this);
		scanner.addStatusListener(this);
		scannerInfo = new EMDK3ScannerInfo(scanner.getScannerInfo());
		
		scannerName = scannerInfo.getFriendlyName();
		scannerNameShort = scannerName.substring(0, 2).toUpperCase();
		isBluetooth = scannerInfo.isBluetooth();
		
		autoValue = AutoValue.NONE;		
		propertyQueue = new ArrayList<Pair<String,String>>();
		EMDKCommon.setupProperties(this, scannerInfo);
		Logger.D(TAG, scannerNameShort + "-");
	}
	
	/**
	 * Sets up the scanner. No other scanners should be enabled. Bluetooth scanners cannot be setup (scannerConfig obtained)
	 * as we shouldnt initiate the Bluetooth connection until enable is called. Side effect is that properties wont be 
	 * able to be getted accurately until enable is called.
	 * 
	 * @return isInitialising - whether the scanner is initialising. If this is true, other scanners should wait for
	 * this object to fire factory.emdk3BarcodeHasSetup() before the next scanner initialises.
	 * @author Ben Kennedy (NCVT73)  
	 */
	public boolean setupScanner()
	{
		Logger.D(TAG, scannerNameShort + " setupScanner+");
		boolean exitValue = false;
		if(isBluetooth)
		{
			Logger.D(TAG, scannerNameShort + " setupScanner isBluetooth");
			isInitialising = false;
			exitValue = false;
		}
		else
		{
			Logger.D(TAG, scannerNameShort + " setupScanner isNotBluetooth");
			try
			{
				scanner.enable();
				//EMDK3.0 does not fire onStatus(IDLE) when the scanner is first enabled (this is a bug)
				//This hack tries to fire IDLE when the scanner has enabled.
				emdk3_0testerThread.schedule(new Runnable(){
					@Override
					public void run()
					{
						emdk3_0onStatusAttempts++;
						if(!emdk3_0bugOnStatusFired)
						{
							if(scanner.isEnabled())
							{
								Logger.D(TAG, "EMDK3.0 fired onStatus.IDLE after " + emdk3_0onStatusAttempts + " attempts");
								onStatus(ScannerStates.IDLE);
							}
							else
							{
								Logger.D(TAG, "EMDK3.0 is not enabled after " + emdk3_0onStatusAttempts + " attempts");
								if(emdk3_0onStatusAttempts < 50)
								{
									emdk3_0testerThread.schedule(this,10,TimeUnit.MILLISECONDS);
								}
							}
						}
					}
				},10,TimeUnit.MILLISECONDS);
				scanner.isEnabled();
			}
			catch (ScannerException e)
			{
				Logger.E(TAG, "setupScanner: Scanner Exception " + e);
				e.printStackTrace();
				if(++setupRetryCount < 3)
				{
					//Try setting up the scanner in 100ms
					setupRetryThread.schedule(new Runnable(){
					    @Override
					    public void run(){
					    	setupScanner();
					    }
					}, 100, TimeUnit.MILLISECONDS);
					exitValue = true;
				}
				else
				{
					Logger.E(TAG, "Unable to setup the " + scannerName + " after multiple attempts");
					//It might still be able to work normally when the user calls enable. If this is the case then
					//let it behave as normally as possible (disable the ENABLE->GetSettings->DISABLE call)
					//The drawback from not being able to setup the scanner is that the user will not be able to 
					//get the scanner config without pre-enabling the scanner.
					isInitialising = false;
					factory.emdk3BarcodeHasSetup();
				}
			}
			exitValue = true;			
		}
		Logger.D(TAG, scannerNameShort + " setupScanner-");
		return exitValue;
	}

	@Override
	public void enable(Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.D(TAG, "enable+");
		BarcodeFactory.setEnabledState(scannerId);

		enableType = ENABLE_TYPE.ENABLE;
		
		if (result == null || !result.hasCallback()) // No callback is set
		{
			Logger.D(LOGTAG, "Callbackless enable. Will output as keystrokes");
			enableCallback = null;
		}
		else
		{
			enableCallback = result;
			result.keepAlive();
		}
		
		setProperties(propertyMap, new MethodResult(false));
		int scanOrNot = 1;
				try{
				IRhoConfig rhoelementsGetConfig= null;
				IRhoWebView vi=null;
				try {
				   vi=RhodesActivity.safeGetInstance().getMainView().getWebView(0);
				}
				catch (Exception e) {
					Logger.E(TAG, e);
					return;
				}
				GoogleWebView web=(GoogleWebView)vi;
				rhoelementsGetConfig=web.getConfig();
				batPercent = BarcodeFactory.batPercent;
				if(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_SCAN) != null){
					scanOrNot = Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.ENABLE_SCAN));
				}
				if(!isEnabled && (scanOrNot != 0 || batPercent > 5))  
					{	
						enable();
					}
				
				}
			catch(NumberFormatException e){
						
			}
		didTakeEnableTheScanner = false;
		Logger.D(TAG, "enable-");
	}

	@Override
	public void start(IMethodResult result)
	{
		Logger.D(TAG, "start+");
		startScanning();
		Logger.D(TAG, "start-");
	}

	@Override
	public void stop(IMethodResult result)
	{
		Logger.D(TAG, "stop+");
		stopScanning();
		Logger.D(TAG, "stop-");
	}

	@Override
	public void disable(IMethodResult result)
	{
		Logger.D(TAG, "disable+");
		BarcodeFactory.setDisabledState(scannerId);
		if(enableCallback != null)
		{
			enableCallback.release();
			enableCallback = null;
		}
		disable(false);
		Logger.D(TAG, "disable-");
	}

	@Override
	public void getAllProperties(IMethodResult result)
	{
		Logger.D(TAG, "getProperties+");
		getProperties(allProperties, result);
		Logger.D(TAG, "getProperties-");
	}

	@Override
	public void getProperties(List<String> arrayofNames, IMethodResult result)
	{
		Logger.D(TAG, "getProperties length: " + arrayofNames.size());
		for(String name: arrayofNames)
        {
            MethodResult propResult = new MethodResult(false);
            getProperty(name, propResult);
            propResult.collectSelf(name, result);
        }
        result.set();
        Logger.D(TAG, "getProperties-");
	}

	@SuppressLint("DefaultLocale")
	@Override
	public void getProperty(String propertyName, IMethodResult result)
	{
		Logger.D(TAG, "getProperty: " + propertyName);
		try
		{
			SettingString propertyEnumValue = SettingString.valueOf(propertyName.toUpperCase());
			RhoScannerClass scannerClass = scannerInfo.getScannerClass();			
		
			if(propertyEnumValue == null)
			{
				Logger.W(TAG, "Could not find property: " + propertyName);
				result.setError("Could not find property: " + propertyName);
				return;
			}
			
			//Pre-enable properties
			switch(propertyEnumValue)
			{
			case ALLDECODERS:
				MethodResult temp=new MethodResult(false);
				super.getProperty("allDecoders",temp);
				result.set("" + AllDecoders.AllDecodersDefault.get(temp.getString())); 				break;
			case DECODEDURATION:
			case DECODEFREQUENCY:
			case DECODESOUND:
			case DECODEVOLUME:
			case AUTOTAB:
			case AUTOENTER:
			case FRIENDLYNAME:
			case ID:
			case SCANNERTYPE:			super.getProperty(propertyName, result);	return;
			default: break;
			}

			//Post enable properties
			if(scannerConfig == null)
			{
				//result.setError("Unavailable");
				Logger.W(TAG, "The " + propertyName + " barcode property can only be queried once the scanner has finished initialising");
				result.set();
				//TODO try to resetup the scanner
				return;
			}
			
			switch(propertyEnumValue)
			{
			
			case HAPTICFEEDBACK:		result.set("" + Scan.DecodeHapticFeedback.get(scannerConfig));	break;
			case ILLUMINATIONMODE:	
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_CAMERA) {
					result.set("" + Reader.Camera.IlluminationMode.get(scannerConfig));
					break;
				}
				//result.setError("Not Supported on this scanner type");
				Logger.I(TAG, propertyName + " is not supported on this scanner type");
				result.set();
				break;
			case INVERSE1DMODE:
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_CAMERA) {
					result.set("" + Reader.Camera.Inverse1DMode.get(scannerConfig));
					break;
				}
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_IMAGER) {
					result.set("" + Reader.Imager.Inverse1DMode.get(scannerConfig));
					break;
				}
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_LASER) {
					result.set("" + Reader.Laser.Inverse1DMode.get(scannerConfig));
					break;
				}
				//result.setError("Not Supported on this scanner type");
				Logger.I(TAG, propertyName + " is not supported on this scanner type");
				result.set();
				break;
			case LCDMODE:				
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_IMAGER) {
					result.set("" + Reader.Imager.LcdMode.get(scannerConfig));
					break;
				}
				//result.setError("Not Supported on this scanner type");
				Logger.I(TAG, propertyName + " is not supported on this scanner type");
				result.set();
				break;
			case LINEARSECURITYLEVEL:
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_CAMERA) {
					result.set("" + Reader.Camera.LinearSecurityLevel.get(scannerConfig));
					break;
				}
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_IMAGER) {
					result.set("" + Reader.Imager.LinearSecurityLevel.get(scannerConfig));
					break;
				}
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_LASER) {
					result.set("" + Reader.Laser.LinearSecurityLevel.get(scannerConfig));
					break;
				}
				Logger.I(TAG, propertyName + " is not supported on this scanner type");
				result.set();
				break;
			case PICKLISTMODE:			
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_IMAGER) {
					result.set("" + Reader.Imager.PickList.get(scannerConfig));
					break;
				}
				//result.setError("PickListMode is only supported on Imager Scanners");
				Logger.I(TAG, propertyName + " is only supported on Imager Scanners");
				result.set();
				break;
			case SCANTIMEOUT:
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_CAMERA) {
					result.set("" + Reader.Camera.BeamTimer.get(scannerConfig));
					break;
				}
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_IMAGER) {
					result.set("" + Reader.Imager.BeamTimer.get(scannerConfig));
					break;
				}
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_LASER) {
					result.set("" + Reader.Laser.BeamTimer.get(scannerConfig));
					break;
				}
				//result.setError("Not Supported on this scanner type");
				Logger.I(TAG, propertyName + " is not supported on this scanner type");
				result.set();
				break;
			case VIEWFINDERMODE:
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_CAMERA) {
					result.set("" + Reader.Camera.ViewFinderMode.get(scannerConfig));
					break;
				}
				//result.setError("ViewfinderMode is only supported on Camera scanner type");
				Logger.I(TAG, propertyName + " is only supported on Camera Scanners");
				result.set();
				break;

			//TODO to implement
			case DISABLESCANNERDURINGNAVIGATE: 
				
			//Unsupported Modes
			case ADAPTIVESCANNING:
			case AIMMODE:
			case AIMTYPE:
			case BARCODEDATAFORMAT:
			case BEAMWIDTH:
			case BIDIRECTIONALREDUNDANCY:
			case CONNECTIONIDLETIMEOUT:
			case DATABUFFERSIZE:
			case DBPMODE:
			case DIFFERENTSYMBOLTIMEOUT:				
			case DISCONNECTBTONDISABLE:
			case DISPLAYBTADDRESSBARCODEONENABLE:
			case DPMMODE:
			case ENABLETIMEOUT:
			case FOCUSMODE:
			case INVALIDDECODEFREQUENCY:
			case INVALIDDECODESOUND:
			case KLASSEEINS: 
			case LOWBATTERYSCAN:
			case POORQUALITY1DMODE:
			case RASTERHEIGHT:
			case RASTERMODE:
			case RSMBATTERYCAPACITY:
			case RSMBATTERYID:
			case RSMBATTERYSTATUS:
			case RSMBLUETOOTHADDRESS:
			case RSMBLUETOOTHAUTHENTICATION:
			case RSMBLUETOOTHAUTORECONNECT:
			case RSMBLUETOOTHBEEPONRECONNECTATTEMPT:
			case RSMBLUETOOTHENCRYPTION:
			case RSMBLUETOOTHFRIENDLYNAME:
			case RSMBLUETOOTHHIDAUTORECONNECT:
			case RSMBLUETOOTHINQUIRYMODE:
			case RSMBLUETOOTHPINCODE:
			case RSMBLUETOOTHPINCODETYPE:
			case RSMBLUETOOTHRECONNECTIONATTEMPTS:
			case RSMDATEOFMANUFACTURE:
			case RSMDATEOFSERVICE:
			case RSMDECODEFEEDBACK:
			case RSMDEVICECLASS:
			case RSMFIRMWAREVERSION:
			case RSMFORCESAVEPAIRINGBARCODE:
			case RSMGOODSCANSDELAY:
			case RSMIGNORECODE128USPS:
			case RSMLOWBATTERYINDICATION:
			case RSMLOWBATTERYINDICATIONCYCLE:
			case RSMMEMS:
			case RSMMODELNUMBER:
			case RSMPAGINGBEEPSEQUENCE:
			case RSMPAGINGENABLE:
			case RSMPROXIMITYCONTINUOUS:
			case RSMPROXIMITYDISTANCE:
			case RSMPROXIMITYENABLE:
			case RSMSCANLINEWIDTH:
			case RSMSCANTRIGGERWAKEUP:
			case RSMSERIALNUMBER:
			case SAMESYMBOLTIMEOUT:
			case TIMEDAIMDURATION: 
			case TRIGGERCONNECTED:
			case VIEWFINDERFEEDBACK:
			case VIEWFINDERFEEDBACKTIME:
			case VIEWFINDERHEIGHT:
			case VIEWFINDERWIDTH:
			case VIEWFINDERX:
			case VIEWFINDERY:

			//Unsupported Decoders
			case CODE11CHECKDIGITCOUNT:
			case COMPOSITEABUSEUPCPREAMBLECHECKDIGITRULES:
			case KOREAN3OF5MAXLENGTH:
			case KOREAN3OF5MINLENGTH:
			case KOREAN3OF5REDUNDANCY:
			case MACROMICROPDF:
			case MACROMICROPDFBUFFERLABELS:
			case MACROMICROPDFCONVERTTOMICROPDF:
			case MACROMICROPDFEXCLUSIVE:
			case MACROMICROPDFREPORTAPPENDINFO:
			case MACROPDF:
			case MACROPDFBUFFERLABELS:
			case MACROPDFCONVERTTOPDF417:
			case MACROPDFEXCLUSIVE:
			case SIGNATUREIMAGEHEIGHT:
			case SIGNATUREIMAGEQUALITY:
			case SIGNATUREIMAGEWIDTH:
				//result.setError("Not supported on Android");
				Logger.I(TAG, propertyName + " is not supported on Android");
				result.set();
				return;
			
			case AUSPOSTAL:				result.set("" + Decoder.AustralianPostal.get(scannerConfig)); 				break;
			case AZTEC:					result.set("" + Decoder.Aztec.get(scannerConfig)); 							break;
			case CANPOSTAL:				result.set("" + Decoder.CanadianPostal.get(scannerConfig)); 				break;
			case CHINESE2OF5:			result.set("" + Decoder.Chinese2of5.get(scannerConfig)); 					break;
			case CODABAR:				result.set("" + Decoder.CodaBar.get(scannerConfig)); 						break;
			case CODABARCLSIEDITING:	result.set("" + Decoder.CodaBar.ClsiEditing.get(scannerConfig));			break;
			case CODABARMAXLENGTH:		result.set("" + Decoder.CodaBar.Length2.get(scannerConfig)); 				break;
			case CODABARMINLENGTH:		result.set("" + Decoder.CodaBar.Length1.get(scannerConfig)); 				break;
			case CODABARNOTISEDITING:	result.set("" + Decoder.CodaBar.NotisEditing.get(scannerConfig));			break;
			case CODABARREDUNDANCY:		result.set("" + Decoder.CodaBar.Redundancy.get(scannerConfig));				break;
			case CODE11:				result.set("" + Decoder.Code11.get(scannerConfig));							break;
			case CODE11MAXLENGTH:		result.set("" + Decoder.Code11.Length2.get(scannerConfig)); 				break;
			case CODE11MINLENGTH:		result.set("" + Decoder.Code11.Length1.get(scannerConfig)); 				break;
			case CODE11REDUNDANCY:		result.set("" + Decoder.Code11.Redundancy.get(scannerConfig));				break;
			case CODE11REPORTCHECKDIGIT:result.set("" + Decoder.Code11.ReportCheckDigit.get(scannerConfig));		break;
			case CODE128:				result.set("" + Decoder.Code128.get(scannerConfig)); 						break;
			case CODE128CHECKISBTTABLE:	result.set("" + Decoder.Code128.CheckIsbtTable.get(scannerConfig));			break;
			case CODE128EAN128:			result.set("" + Decoder.Code128.EnableEan128.get(scannerConfig));			break;
			case CODE128ISBT128:		result.set("" + Decoder.Code128.EnableIsbt128.get(scannerConfig));			break;
			case CODE128ISBT128CONCATMODE:	result.set("" + Decoder.Code128.Isbt128ConcatMode.get(scannerConfig));	break;
			case CODE128MAXLENGTH:		result.set("" + Decoder.Code128.Length2.get(scannerConfig)); 				break;
			case CODE128MINLENGTH:		result.set("" + Decoder.Code128.Length1.get(scannerConfig)); 				break;
			case CODE128OTHER128:		result.set("" + Decoder.Code128.EnablePlain.get(scannerConfig));			break; //TODO test me
			case CODE128REDUNDANCY:		result.set("" + Decoder.Code128.Redundancy.get(scannerConfig));				break;
			case CODE128SECURITYLEVEL:	result.set("" + Decoder.Code128.SecurityLevel.get(scannerConfig));			break;
			case CODE39:				result.set("" + Decoder.Code39.get(scannerConfig)); 						break;
			case CODE39CODE32PREFIX:	result.set("" + Decoder.Code39.ReportCode32Prefix.get(scannerConfig));		break;
			case CODE39CONVERTTOCODE32:	result.set("" + Decoder.Code39.ConvertToCode32.get(scannerConfig));			break;
			case CODE39FULLASCII:		result.set("" + Decoder.Code39.FullAscii.get(scannerConfig)); 				break;
			case CODE39MAXLENGTH:		result.set("" + Decoder.Code39.Length2.get(scannerConfig)); 				break;
			case CODE39MINLENGTH:		result.set("" + Decoder.Code39.Length1.get(scannerConfig)); 				break;
			case CODE39REDUNDANCY:		result.set("" + Decoder.Code39.Redundancy.get(scannerConfig));				break;
			case CODE39REPORTCHECKDIGIT:result.set("" + Decoder.Code39.ReportCheckDigit.get(scannerConfig));		break;
			case CODE39SECURITYLEVEL:	result.set("" + Decoder.Code39.SecurityLevel.get(scannerConfig)); 			break;
			case CODE39VERIFYCHECKDIGIT:result.set("" + Decoder.Code39.VerifyCheckDigit.get(scannerConfig));		break;
			case CODE93:				result.set("" + Decoder.Code93.get(scannerConfig));				 			break;
			case CODE93MAXLENGTH:		result.set("" + Decoder.Code93.Length2.get(scannerConfig));				 	break;
			case CODE93MINLENGTH:		result.set("" + Decoder.Code93.Length1.get(scannerConfig));				 	break;
			case CODE93REDUNDANCY:		result.set("" + Decoder.Code93.Redundancy.get(scannerConfig));				break;
			case COMPOSITEAB:			result.set("" + Decoder.CompositeAB.get(scannerConfig));			 		break;
			case COMPOSITEABUCCLINKMODE:result.set("" + Decoder.CompositeAB.UccLinkMode.get(scannerConfig));		break;
			case COMPOSITEC:			result.set("" + Decoder.CompositeC.get(scannerConfig)); 					break;
			case D2OF5:					result.set("" + Decoder.D2of5.get(scannerConfig)); 							break;
			case D2OF5MAXLENGTH:		result.set("" + Decoder.D2of5.Length2.get(scannerConfig)); 					break;
			case D2OF5MINLENGTH:		result.set("" + Decoder.D2of5.Length1.get(scannerConfig)); 					break;
			case D2OF5REDUNDANCY:		result.set("" + Decoder.D2of5.Redundancy.get(scannerConfig)); 				break;
			case DATAMATRIX:			result.set("" + Decoder.DataMatrix.get(scannerConfig)); 					break;
			case DUTCHPOSTAL:			result.set("" + Decoder.DutchPostal.get(scannerConfig)); 					break;
			case EAN13:					result.set("" + Decoder.Ean13.get(scannerConfig)); 							break;
			case EAN8:					result.set("" + Decoder.Ean8.get(scannerConfig)); 							break;
			case EAN8CONVERTTOEAN13:	result.set("" + Decoder.Ean8.ConvertToEan13.get(scannerConfig));			break;
			case GS1DATABAR:			result.set("" + Decoder.Gs1Databar.get(scannerConfig)); 					break;
			case GS1DATABAREXPANDED:	result.set("" + Decoder.Gs1DatabarExp.get(scannerConfig)); 					break;
			case GS1DATABARLIMITED:		result.set("" + Decoder.Gs1DatabarLim.get(scannerConfig)); 					break;
			case I2OF5:					result.set("" + Decoder.I2of5.get(scannerConfig)); 							break;
			case I2OF5CONVERTTOEAN13:	result.set("" + Decoder.I2of5.ConvertToEan13.get(scannerConfig)); 			break;
			case I2OF5MAXLENGTH:		result.set("" + Decoder.I2of5.Length2.get(scannerConfig)); 					break;
			case I2OF5MINLENGTH:		result.set("" + Decoder.I2of5.Length1.get(scannerConfig)); 					break;
			case I2OF5REDUNDANCY:		result.set("" + Decoder.I2of5.Redundancy.get(scannerConfig)); 				break;
			case I2OF5REPORTCHECKDIGIT:	result.set("" + Decoder.I2of5.ReportCheckDigit.get(scannerConfig));			break;
			case I2OF5VERIFYCHECKDIGIT:	result.set("" + Decoder.I2of5.VerifyCheckDigit.get(scannerConfig));			break;
			case JAPPOSTAL:				result.set("" + Decoder.JapanesePostal.get(scannerConfig)); 				break;
			case KOREAN3OF5:			result.set("" + Decoder.Korean3of5.get(scannerConfig)); 					break;
			case MATRIX2OF5:			result.set("" + Decoder.Matrix2of5.get(scannerConfig));						break;
			case MATRIX2OF5MAXLENGTH:	result.set("" + Decoder.Matrix2of5.Length2.get(scannerConfig));				break;
			case MATRIX2OF5MINLENGTH:	result.set("" + Decoder.Matrix2of5.Length1.get(scannerConfig));				break;
			case MATRIX2OF5REPORTCHECKDIGIT:result.set("" + Decoder.Matrix2of5.ReportCheckDigit.get(scannerConfig));break;
			case MATRIX2OF5VERIFYCHECKDIGIT:result.set("" + Decoder.Matrix2of5.VerifyCheckDigit.get(scannerConfig));break;
			case MAXICODE:				result.set("" + Decoder.MaxiCode.get(scannerConfig)); 						break;
			case MICROPDF:				result.set("" + Decoder.MicroPdf.get(scannerConfig)); 						break;
			case MICROQR:				result.set("" + Decoder.MicroQr.get(scannerConfig));						break;
			case MSI:					result.set("" + Decoder.Msi.get(scannerConfig)); 							break;
			case MSICHECKDIGITS:		result.set("" + Decoder.Msi.CheckDigits.get(scannerConfig)); 				break;
			case MSICHECKDIGITSCHEME:	result.set("" + Decoder.Msi.CheckDigitScheme.get(scannerConfig));			break;
			case MSIMAXLENGTH:			result.set("" + Decoder.Msi.Length2.get(scannerConfig));					break;
			case MSIMINLENGTH:			result.set("" + Decoder.Msi.Length1.get(scannerConfig));					break;
			case MSIREDUNDANCY:			result.set("" + Decoder.Msi.Redundancy.get(scannerConfig));					break;
			case MSIREPORTCHECKDIGIT:	result.set("" + Decoder.Msi.ReportCheckDigit.get(scannerConfig));			break;
			case PDF417:				result.set("" + Decoder.Pdf417.get(scannerConfig)); 						break;
			case QRCODE:				result.set("" + Decoder.QrCode.get(scannerConfig));							break;
			case SIGNATURE:				result.set("" + Decoder.Signature.get(scannerConfig)); 						break;
			case TLC39:					result.set("" + Decoder.Tlc39.get(scannerConfig)); 							break;
			case TRIOPTIC39:			result.set("" + Decoder.TriOptic39.get(scannerConfig)); 					break;
			case TRIOPTIC39REDUNDANCY:	result.set("" + Decoder.TriOptic39.Redundancy.get(scannerConfig));			break;
			case UKPOSTAL:				result.set("" + Decoder.UkPostal.get(scannerConfig)); 						break;
			case UKPOSTALREPORTCHECKDIGIT:result.set("" + Decoder.UkPostal.ReportCheckDigit.get(scannerConfig));	break;
			case UPCA:					result.set("" + Decoder.Upca.get(scannerConfig)); 							break;
			case UPCAPREAMBLE:			result.set("" + Decoder.Upca.Preamble.get(scannerConfig));					break;
			case UPCAREPORTCHECKDIGIT:	result.set("" + Decoder.Upca.ReportCheckDigit.get(scannerConfig));			break;
			case UPCE0:					result.set("" + Decoder.Upce0.get(scannerConfig)); 							break;
			case UPCE0CONVERTTOUPCA:	result.set("" + Decoder.Upce0.ConvertToUpca.get(scannerConfig));			break;
			case UPCE0PREAMBLE:			result.set("" + Decoder.Upce0.Preamble.get(scannerConfig)); 				break;
			case UPCE0REPORTCHECKDIGIT:	result.set("" + Decoder.Upce0.ReportCheckDigit.get(scannerConfig));			break;
			case UPCE1:					result.set("" + Decoder.Upce1.get(scannerConfig)); 							break;
			case UPCE1CONVERTTOUPCA:	result.set("" + Decoder.Upce1.ConvertToUpca.get(scannerConfig));			break;
			case UPCE1PREAMBLE:			result.set("" + Decoder.Upce1.Preamble.get(scannerConfig));					break;
			case UPCE1REPORTCHECKDIGIT:	result.set("" + Decoder.Upce1.ReportCheckDigit.get(scannerConfig));			break;
			case UPCEANBOOKLAND:		result.set("" + Decoder.UpcEanParams.BooklandCode.get(scannerConfig));		break;
			case UPCEANBOOKLANDFORMAT:	result.set("" + Decoder.UpcEanParams.BooklandFormat.get(scannerConfig));	break;
			case UPCEANCONVERTGS1DATABARTOUPCEAN:result.set("" + Decoder.UpcEanParams.ConvertDataBarToUpcEan.get(scannerConfig)); break;
			case UPCEANCOUPON:			result.set("" + Decoder.UpcEanParams.CouponCode.get(scannerConfig)); 		break;
			case UPCEANLINEARDECODE:	result.set("" + Decoder.UpcEanParams.LinearDecode.get(scannerConfig)); 		break;
			case UPCEANRANDOMWEIGHTCHECKDIGIT:	result.set("" + Decoder.UpcEanParams.RandomWeightCheckDigit.get(scannerConfig)); break;
			case UPCEANRETRYCOUNT:		result.set("" + Decoder.UpcEanParams.SupplementalRetries.get(scannerConfig));break;
			case UPCEANSECURITYLEVEL:	result.set("" + Decoder.UpcEanParams.SecurityLevel.get(scannerConfig));		break;
			case UPCEANSUPPLEMENTAL2:	result.set("" + Decoder.UpcEanParams.Supplemental2.get(scannerConfig));		break;
			case UPCEANSUPPLEMENTAL5:	result.set("" + Decoder.UpcEanParams.Supplemental5.get(scannerConfig));		break;
			case UPCEANSUPPLEMENTALMODE:result.set("" + Decoder.UpcEanParams.SupplementalMode.get(scannerConfig));	break;
			case US4STATE:				result.set("" + Decoder.Us4State.get(scannerConfig));						break;
			case US4STATEFICS:			result.set("" + Decoder.Us4StateFics.get(scannerConfig));					break;
			case USPLANET:				result.set("" + Decoder.UsPlanet.get(scannerConfig));						break;
			case USPLANETREPORTCHECKDIGIT:result.set("" + Decoder.UsPlanet.ReportCheckDigit.get(scannerConfig));	break;
			case USPOSTNET:				result.set("" + Decoder.UsPostNet.get(scannerConfig));						break;
			case USPOSTNETREPORTCHECKDIGIT:result.set("" + Decoder.UsPostNet.ReportCheckDigit.get(scannerConfig));	break;
			case WEBCODE:				result.set("" + Decoder.WebCode.get(scannerConfig));						break;
			case WEBCODEDECODEGTSUBTYPE:result.set("" + Decoder.WebCode.SubType.get(scannerConfig));				break;
			default:break;
			}
		}
		catch (NumberFormatException e) {
			Logger.W(TAG, e);
			result.setError("Incorrect parameter format");
		}
		catch (IllegalArgumentException e) {
			Logger.W(TAG, e);
			result.setError(e.getMessage());
		}
	}

	@SuppressWarnings("unchecked")
	@Override
	public void getSupportedProperties(IMethodResult result)
	{
		Logger.D(TAG, "getSupportedProperties");
		if(supportedProperties == null)
		{
			result.setError("Cannot detect supported properties if the scanner has not been enabled in this app");
			Logger.W(TAG, "Cannot detect supported properties if the scanner has not been enabled in this app");
			return;
		}
		result.set((List<Object>)(List<?>) supportedProperties);
	}

	/**
	 * This will populate the supportedProperties arraylist with all of the properties supported by the scanner.
	 * This can only be done if scannerConfig has already been obtained (i.e. the scanner has been enabled at
	 * least once since startup)
	 * 
	 * @author Ben Kennedy (NCVT73)
	 */
	private void popluateSupportedProperties()
	{
		Logger.D(TAG, "populateSupportedProperties+");
		
		RhoScannerClass scannerClass = scannerInfo.getScannerClass();
		
		supportedProperties = new ArrayList<String>();
		SettingString[] settingStrings = SettingString.values();
		Logger.D(TAG, "getSupportedProperties " + settingStrings.length);
		
		for(SettingString setting: settingStrings)
		{
			Logger.D(TAG, "getSupportedProperties. name: "+ setting.name + "  emdkName: " + setting.emdkName);
			if(setting.emdkName != null && scannerConfig.isParamSupported(setting.emdkName))
				supportedProperties.add(setting.name);
		}
		
		supportedProperties.add(SettingString.ALLDECODERS.name);
		supportedProperties.add(SettingString.AUTOENTER.name);
		supportedProperties.add(SettingString.AUTOTAB.name);
		supportedProperties.add(SettingString.DECODEDURATION.name);
		supportedProperties.add(SettingString.DECODEFREQUENCY.name);
		supportedProperties.add(SettingString.DECODESOUND.name);
		supportedProperties.add(SettingString.DECODEVOLUME.name);
		supportedProperties.add(SettingString.FRIENDLYNAME.name);
		supportedProperties.add(SettingString.ID.name);
		supportedProperties.add(SettingString.SCANNERTYPE.name);
		
		if(scannerClass == RhoScannerClass.SCANNER_CLASS_CAMERA)
		{
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.cameraSpecific.illuminationMode")) supportedProperties.add(SettingString.ILLUMINATIONMODE.name);
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.cameraSpecific.inverse1DMode")) supportedProperties.add(SettingString.INVERSE1DMODE.name);
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.cameraSpecific.linearSecurityLevel")) supportedProperties.add(SettingString.LINEARSECURITYLEVEL.name);
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.cameraSpecific.beamTimer")) supportedProperties.add(SettingString.SCANTIMEOUT.name);
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.cameraSpecific.viewFinderMode")) supportedProperties.add(SettingString.VIEWFINDERMODE.name);
		}
		else if(scannerClass == RhoScannerClass.SCANNER_CLASS_IMAGER)
		{
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.imagerSpecific.inverse1DMode")) supportedProperties.add(SettingString.INVERSE1DMODE.name);
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.imagerSpecific.lcdMode")) supportedProperties.add(SettingString.LCDMODE.name);
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.imagerSpecific.linearSecurityLevel")) supportedProperties.add(SettingString.LINEARSECURITYLEVEL.name);
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.imagerSpecific.pickList")) supportedProperties.add(SettingString.PICKLISTMODE.name);
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.imagerSpecific.beamTimer")) supportedProperties.add(SettingString.SCANTIMEOUT.name);
		}
		else if(scannerClass == RhoScannerClass.SCANNER_CLASS_LASER)
		{
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.laserSpecific.inverse1DMode")) supportedProperties.add(SettingString.INVERSE1DMODE.name);
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.laserSpecific.linearSecurityLevel")) supportedProperties.add(SettingString.LINEARSECURITYLEVEL.name);
			if(scannerConfig.isParamSupported("config.readerParams.readerSpecific.laserSpecific.beamTimer")) supportedProperties.add(SettingString.SCANTIMEOUT.name);
		}
		
		Collections.sort(supportedProperties);
		Logger.D(TAG, "populateSupportedProperties-");
	}

	@Override
	public void setProperties(Map<String, String> propertyMap, IMethodResult result)
	{
		if(propertyMap == null) return;
		Logger.D(TAG, "setProperties. Length: " + propertyMap.size());
		EMDKCommon.setProperties(this, propertyMap, result);
		Logger.D(TAG, "setProperties-");
	}

	/**
	 * result can be null if the call is coming from decodeSound. We needed some way of identifying if decode sound
	 * was coming from an internal call
	 */
	@SuppressLint("DefaultLocale")
	@Override
	public void setProperty(String propertyName, String propertyValue, IMethodResult result)
	{
		Logger.D(TAG, "setProperty: " + propertyName + ", propertyValue: " + propertyValue);
		try
		{
			SettingString propertyEnumValue = SettingString.valueOf(propertyName.toUpperCase());
			RhoScannerClass scannerClass = scannerInfo.getScannerClass();			
		
			if(propertyEnumValue == null)
			{
				Logger.W(TAG, "Could not find property: " + propertyName);
				result.setError("Could not find property: " + propertyName);
				return;
			}
			
			if(isScanning)
			{
				Logger.I(TAG, "Property will not take affect until the scanner has stopped scanning.");
				propertyQueue.add(new Pair<String, String>(propertyName, propertyValue));
				return;
			}
			
			if(isMinimized)
			{
				Logger.I(TAG, "Property will not take affect until the app is in the foreground and the scanner is enabled");
				propertyQueue.add(new Pair<String, String>(propertyName, propertyValue));
				return;
			}
			
			
			//Pre-enable properties
			switch(propertyEnumValue)
			{
			case DECODEDURATION: 	setDecodeDuration(Integer.valueOf(propertyValue), result); 	return;
			case DECODEFREQUENCY:	setDecodeFrequency(Integer.valueOf(propertyValue), result); return;
			case DECODESOUND:		
				if(result == null)
				{
					break;
				}
				else
				{
					setDecodeSound(propertyValue, result);
					return;
				}
			case DECODEVOLUME:		setDecodeVolume(Integer.valueOf(propertyValue), result);	return;
			case AUTOTAB:			setAutoTab(Boolean.valueOf(propertyValue), result);			return;
			case AUTOENTER:			setAutoEnter(Boolean.valueOf(propertyValue), result); 		return;	
			default: break;
			}
			
			//Post enable properties
			if(scannerConfig == null)
			{
				//result.setError("Unavailable");
				Logger.W(TAG, "The " + propertyName + " barcode property can only be set once the scanner has finished initialising");
				propertyQueue.add(new Pair<String, String>(propertyName, propertyValue));
				//TODO reget the scanner config here
				if(result != null) result.set();
				return;
			}

			switch(propertyEnumValue)
			{
			
			case HAPTICFEEDBACK:		result.set(Scan.DecodeHapticFeedback.set(scannerConfig, propertyValue));	break;
			case DECODESOUND:			Scan.DecodeAudioFeedbackUri.set(scannerConfig, propertyValue); 	break;
			case ILLUMINATIONMODE:	
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_CAMERA) {
					result.set(Reader.Camera.IlluminationMode.set(scannerConfig, propertyValue));
					break;
				}
				//result.setError("Not Supported on this scanner type");
				Logger.I(TAG, propertyName + " is not supported on this scanner type");
				result.set();
				break;
			case INVERSE1DMODE:
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_CAMERA) {
					result.set(Reader.Camera.Inverse1DMode.set(scannerConfig, propertyValue));
					break;
				}
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_IMAGER) {
					result.set(Reader.Imager.Inverse1DMode.set(scannerConfig, propertyValue));
					break;
				}
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_LASER) {
					result.set(Reader.Laser.Inverse1DMode.set(scannerConfig, propertyValue));
					break;
				}
				//result.setError("Not Supported on this scanner type");
				Logger.I(TAG, propertyName + " is not supported on this scanner type");
				result.set();
				break;
			case LCDMODE:				
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_IMAGER) {
					result.set(Reader.Imager.LcdMode.set(scannerConfig, propertyValue));
					break;
				}
				//result.setError("Not Supported on this scanner type");
				Logger.I(TAG, propertyName + " is not supported on this scanner type");
				result.set();
				break;
			case LINEARSECURITYLEVEL:
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_CAMERA) {
					result.set(Reader.Camera.LinearSecurityLevel.set(scannerConfig, propertyValue));
					break;
				}
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_IMAGER) {
					result.set(Reader.Imager.LinearSecurityLevel.set(scannerConfig, propertyValue));
					break;
				}
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_LASER) {
					result.set(Reader.Laser.LinearSecurityLevel.set(scannerConfig, propertyValue));
					break;
				}
				Logger.I(TAG, propertyName + " is not supported on this scanner type");
				result.set();
				break;
			case PICKLISTMODE:			
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_IMAGER) {
					result.set(Reader.Imager.PickList.set(scannerConfig, propertyValue));
					break;
				}
				//result.setError("PickListMode is only supported on Imager Scanners");
				Logger.I(TAG, propertyName + " is only supported on Imager Scanners");
				result.set();
				break;
			case SCANTIMEOUT:
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_CAMERA) {
					result.set(Reader.Camera.BeamTimer.set(scannerConfig, propertyValue));
					break;
				}
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_IMAGER) {
					result.set(Reader.Imager.BeamTimer.set(scannerConfig, propertyValue));
					break;
				}
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_LASER) {
					result.set(Reader.Laser.BeamTimer.set(scannerConfig, propertyValue));
					break;
				}
				//result.setError("Not Supported on this scanner type");
				Logger.I(TAG, propertyName + " is not supported on this scanner type");
				result.set();
				break;
			case VIEWFINDERMODE:
				if(scannerClass == RhoScannerClass.SCANNER_CLASS_CAMERA) {
					result.set(Reader.Camera.ViewFinderMode.set(scannerConfig, propertyValue));
					break;
				}
				//result.setError("ViewfinderMode is only supported on Camera scanner type");
				Logger.I(TAG, propertyName + " is only supported on Camera Scanners");
				result.set();
				break;

			//TODO to implement
			case DISABLESCANNERDURINGNAVIGATE: 
				
			//Unsupported Modes
			case ADAPTIVESCANNING:
			case AIMMODE:
			case AIMTYPE:
			case BARCODEDATAFORMAT:
			case BEAMWIDTH:
			case BIDIRECTIONALREDUNDANCY:
			case CONNECTIONIDLETIMEOUT:
			case DATABUFFERSIZE:
			case DBPMODE:
			case DIFFERENTSYMBOLTIMEOUT:				
			case DISCONNECTBTONDISABLE:
			case DISPLAYBTADDRESSBARCODEONENABLE:
			case DPMMODE:
			case ENABLETIMEOUT:
			case FOCUSMODE:
			case INVALIDDECODEFREQUENCY:
			case INVALIDDECODESOUND:
			case KLASSEEINS: 
			case LOWBATTERYSCAN:
			case POORQUALITY1DMODE:
			case RASTERHEIGHT:
			case RASTERMODE:
			case RSMBATTERYCAPACITY:
			case RSMBATTERYID:
			case RSMBATTERYSTATUS:
			case RSMBLUETOOTHADDRESS:
			case RSMBLUETOOTHAUTHENTICATION:
			case RSMBLUETOOTHAUTORECONNECT:
			case RSMBLUETOOTHBEEPONRECONNECTATTEMPT:
			case RSMBLUETOOTHENCRYPTION:
			case RSMBLUETOOTHFRIENDLYNAME:
			case RSMBLUETOOTHHIDAUTORECONNECT:
			case RSMBLUETOOTHINQUIRYMODE:
			case RSMBLUETOOTHPINCODE:
			case RSMBLUETOOTHPINCODETYPE:
			case RSMBLUETOOTHRECONNECTIONATTEMPTS:
			case RSMDATEOFMANUFACTURE:
			case RSMDATEOFSERVICE:
			case RSMDECODEFEEDBACK:
			case RSMDEVICECLASS:
			case RSMFIRMWAREVERSION:
			case RSMFORCESAVEPAIRINGBARCODE:
			case RSMGOODSCANSDELAY:
			case RSMIGNORECODE128USPS:
			case RSMLOWBATTERYINDICATION:
			case RSMLOWBATTERYINDICATIONCYCLE:
			case RSMMEMS:
			case RSMMODELNUMBER:
			case RSMPAGINGBEEPSEQUENCE:
			case RSMPAGINGENABLE:
			case RSMPROXIMITYCONTINUOUS:
			case RSMPROXIMITYDISTANCE:
			case RSMPROXIMITYENABLE:
			case RSMSCANLINEWIDTH:
			case RSMSCANTRIGGERWAKEUP:
			case RSMSERIALNUMBER:
			case SAMESYMBOLTIMEOUT:
			case TIMEDAIMDURATION: 
			case TRIGGERCONNECTED:
			case VIEWFINDERFEEDBACK:
			case VIEWFINDERFEEDBACKTIME:
			case VIEWFINDERHEIGHT:
			case VIEWFINDERWIDTH:
			case VIEWFINDERX:
			case VIEWFINDERY:

			//Unsupported Decoders
			case CODE11CHECKDIGITCOUNT:
			case COMPOSITEABUSEUPCPREAMBLECHECKDIGITRULES:
			case KOREAN3OF5MAXLENGTH:
			case KOREAN3OF5MINLENGTH:
			case KOREAN3OF5REDUNDANCY:
			case MACROMICROPDF:
			case MACROMICROPDFBUFFERLABELS:
			case MACROMICROPDFCONVERTTOMICROPDF:
			case MACROMICROPDFEXCLUSIVE:
			case MACROMICROPDFREPORTAPPENDINFO:
			case MACROPDF:
			case MACROPDFBUFFERLABELS:
			case MACROPDFCONVERTTOPDF417:
			case MACROPDFEXCLUSIVE:
			case SIGNATUREIMAGEHEIGHT:
			case SIGNATUREIMAGEQUALITY:
			case SIGNATUREIMAGEWIDTH:
				//result.setError("Not supported on Android");
				Logger.I(TAG, propertyName + " is not supported on Android");
				result.set();
				return;
			
			case AUSPOSTAL:				result.set(Decoder.AustralianPostal.set(scannerConfig, propertyValue)); 				break;
			case AZTEC:					result.set(Decoder.Aztec.set(scannerConfig, propertyValue)); 							break;
			case CANPOSTAL:				result.set(Decoder.CanadianPostal.set(scannerConfig, propertyValue)); 				break;
			case CHINESE2OF5:			result.set(Decoder.Chinese2of5.set(scannerConfig, propertyValue)); 					break;
			case CODABAR:				result.set(Decoder.CodaBar.set(scannerConfig, propertyValue)); 						break;
			case CODABARCLSIEDITING:	result.set(Decoder.CodaBar.ClsiEditing.set(scannerConfig, propertyValue));			break;
			case CODABARMAXLENGTH:		result.set(Decoder.CodaBar.Length2.set(scannerConfig, propertyValue)); 				break;
			case CODABARMINLENGTH:		result.set(Decoder.CodaBar.Length1.set(scannerConfig, propertyValue)); 				break;
			case CODABARNOTISEDITING:	result.set(Decoder.CodaBar.NotisEditing.set(scannerConfig, propertyValue));			break;
			case CODABARREDUNDANCY:		result.set(Decoder.CodaBar.Redundancy.set(scannerConfig, propertyValue));				break;
			case CODE11:				result.set(Decoder.Code11.set(scannerConfig, propertyValue));							break;
			case CODE11MAXLENGTH:		result.set(Decoder.Code11.Length2.set(scannerConfig, propertyValue)); 				break;
			case CODE11MINLENGTH:		result.set(Decoder.Code11.Length1.set(scannerConfig, propertyValue)); 				break;
			case CODE11REDUNDANCY:		result.set(Decoder.Code11.Redundancy.set(scannerConfig, propertyValue));				break;
			case CODE11REPORTCHECKDIGIT:result.set(Decoder.Code11.ReportCheckDigit.set(scannerConfig, propertyValue));		break;
			case CODE128:				result.set(Decoder.Code128.set(scannerConfig, propertyValue)); 						break;
			case CODE128CHECKISBTTABLE:	result.set(Decoder.Code128.CheckIsbtTable.set(scannerConfig, propertyValue));			break;
			case CODE128EAN128:			result.set(Decoder.Code128.EnableEan128.set(scannerConfig, propertyValue));			break;
			case CODE128ISBT128:		result.set(Decoder.Code128.EnableIsbt128.set(scannerConfig, propertyValue));			break;
			case CODE128ISBT128CONCATMODE:	result.set(Decoder.Code128.Isbt128ConcatMode.set(scannerConfig, propertyValue));	break;
			case CODE128MAXLENGTH:		result.set(Decoder.Code128.Length2.set(scannerConfig, propertyValue)); 				break;
			case CODE128MINLENGTH:		result.set(Decoder.Code128.Length1.set(scannerConfig, propertyValue)); 				break;
			case CODE128OTHER128:		result.set(Decoder.Code128.EnablePlain.set(scannerConfig, propertyValue));			break; //TODO test me
			case CODE128REDUNDANCY:		result.set(Decoder.Code128.Redundancy.set(scannerConfig, propertyValue));				break;
			case CODE128SECURITYLEVEL:	result.set(Decoder.Code128.SecurityLevel.set(scannerConfig, propertyValue));			break;
			case CODE39:				result.set(Decoder.Code39.set(scannerConfig, propertyValue)); 						break;
			case CODE39CODE32PREFIX:	result.set(Decoder.Code39.ReportCode32Prefix.set(scannerConfig, propertyValue));		break;
			case CODE39CONVERTTOCODE32:	result.set(Decoder.Code39.ConvertToCode32.set(scannerConfig, propertyValue));			break;
			case CODE39FULLASCII:		result.set(Decoder.Code39.FullAscii.set(scannerConfig, propertyValue)); 				break;
			case CODE39MAXLENGTH:		result.set(Decoder.Code39.Length2.set(scannerConfig, propertyValue)); 				break;
			case CODE39MINLENGTH:		result.set(Decoder.Code39.Length1.set(scannerConfig, propertyValue)); 				break;
			case CODE39REDUNDANCY:		result.set(Decoder.Code39.Redundancy.set(scannerConfig, propertyValue));				break;
			case CODE39REPORTCHECKDIGIT:result.set(Decoder.Code39.ReportCheckDigit.set(scannerConfig, propertyValue));		break;
			case CODE39SECURITYLEVEL:	result.set(Decoder.Code39.SecurityLevel.set(scannerConfig, propertyValue)); 			break;
			case CODE39VERIFYCHECKDIGIT:result.set(Decoder.Code39.VerifyCheckDigit.set(scannerConfig, propertyValue));		break;
			case CODE93:				result.set(Decoder.Code93.set(scannerConfig, propertyValue));				 			break;
			case CODE93MAXLENGTH:		result.set(Decoder.Code93.Length2.set(scannerConfig, propertyValue));				 	break;
			case CODE93MINLENGTH:		result.set(Decoder.Code93.Length1.set(scannerConfig, propertyValue));				 	break;
			case CODE93REDUNDANCY:		result.set(Decoder.Code93.Redundancy.set(scannerConfig, propertyValue));				break;
			case COMPOSITEAB:			result.set(Decoder.CompositeAB.set(scannerConfig, propertyValue));			 		break;
			case COMPOSITEABUCCLINKMODE:result.set(Decoder.CompositeAB.UccLinkMode.set(scannerConfig, propertyValue));		break;
			case COMPOSITEC:			result.set(Decoder.CompositeC.set(scannerConfig, propertyValue)); 					break;
			case D2OF5:					result.set(Decoder.D2of5.set(scannerConfig, propertyValue)); 							break;
			case D2OF5MAXLENGTH:		result.set(Decoder.D2of5.Length2.set(scannerConfig, propertyValue)); 					break;
			case D2OF5MINLENGTH:		result.set(Decoder.D2of5.Length1.set(scannerConfig, propertyValue)); 					break;
			case D2OF5REDUNDANCY:		result.set(Decoder.D2of5.Redundancy.set(scannerConfig, propertyValue)); 				break;
			case DATAMATRIX:			result.set(Decoder.DataMatrix.set(scannerConfig, propertyValue)); 					break;
			case DUTCHPOSTAL:			result.set(Decoder.DutchPostal.set(scannerConfig, propertyValue)); 					break;
			case EAN13:					result.set(Decoder.Ean13.set(scannerConfig, propertyValue)); 							break;
			case EAN8:					result.set(Decoder.Ean8.set(scannerConfig, propertyValue)); 							break;
			case EAN8CONVERTTOEAN13:	result.set(Decoder.Ean8.ConvertToEan13.set(scannerConfig, propertyValue));			break;
			case GS1DATABAR:			result.set(Decoder.Gs1Databar.set(scannerConfig, propertyValue)); 					break;
			case GS1DATABAREXPANDED:	result.set(Decoder.Gs1DatabarExp.set(scannerConfig, propertyValue)); 					break;
			case GS1DATABARLIMITED:		result.set(Decoder.Gs1DatabarLim.set(scannerConfig, propertyValue)); 					break;
			case I2OF5:					result.set(Decoder.I2of5.set(scannerConfig, propertyValue)); 							break;
			case I2OF5CONVERTTOEAN13:	result.set(Decoder.I2of5.ConvertToEan13.set(scannerConfig, propertyValue)); 			break;
			case I2OF5MAXLENGTH:		result.set(Decoder.I2of5.Length2.set(scannerConfig, propertyValue)); 					break;
			case I2OF5MINLENGTH:		result.set(Decoder.I2of5.Length1.set(scannerConfig, propertyValue)); 					break;
			case I2OF5REDUNDANCY:		result.set(Decoder.I2of5.Redundancy.set(scannerConfig, propertyValue)); 				break;
			case I2OF5REPORTCHECKDIGIT:	result.set(Decoder.I2of5.ReportCheckDigit.set(scannerConfig, propertyValue));			break;
			case I2OF5VERIFYCHECKDIGIT:	result.set(Decoder.I2of5.VerifyCheckDigit.set(scannerConfig, propertyValue));			break;
			case JAPPOSTAL:				result.set(Decoder.JapanesePostal.set(scannerConfig, propertyValue)); 				break;
			case KOREAN3OF5:			result.set(Decoder.Korean3of5.set(scannerConfig, propertyValue)); 					break;
			case MATRIX2OF5:			result.set(Decoder.Matrix2of5.set(scannerConfig, propertyValue));						break;
			case MATRIX2OF5MAXLENGTH:	result.set(Decoder.Matrix2of5.Length2.set(scannerConfig, propertyValue));				break;
			case MATRIX2OF5MINLENGTH:	result.set(Decoder.Matrix2of5.Length1.set(scannerConfig, propertyValue));				break;
			case MATRIX2OF5REPORTCHECKDIGIT:result.set(Decoder.Matrix2of5.ReportCheckDigit.set(scannerConfig, propertyValue));break;
			case MATRIX2OF5VERIFYCHECKDIGIT:result.set(Decoder.Matrix2of5.VerifyCheckDigit.set(scannerConfig, propertyValue));break;
			case MAXICODE:				result.set(Decoder.MaxiCode.set(scannerConfig, propertyValue)); 						break;
			case MICROPDF:				result.set(Decoder.MicroPdf.set(scannerConfig, propertyValue)); 						break;
			case MICROQR:				result.set(Decoder.MicroQr.set(scannerConfig, propertyValue));						break;
			case MSI:					result.set(Decoder.Msi.set(scannerConfig, propertyValue)); 							break;
			case MSICHECKDIGITS:		result.set(Decoder.Msi.CheckDigits.set(scannerConfig, propertyValue)); 				break;
			case MSICHECKDIGITSCHEME:	result.set(Decoder.Msi.CheckDigitScheme.set(scannerConfig, propertyValue));			break;
			case MSIMAXLENGTH:			result.set(Decoder.Msi.Length2.set(scannerConfig, propertyValue));					break;
			case MSIMINLENGTH:			result.set(Decoder.Msi.Length1.set(scannerConfig, propertyValue));					break;
			case MSIREDUNDANCY:			result.set(Decoder.Msi.Redundancy.set(scannerConfig, propertyValue));					break;
			case MSIREPORTCHECKDIGIT:	result.set(Decoder.Msi.ReportCheckDigit.set(scannerConfig, propertyValue));			break;
			case PDF417:				result.set(Decoder.Pdf417.set(scannerConfig, propertyValue)); 						break;
			case QRCODE:				result.set(Decoder.QrCode.set(scannerConfig, propertyValue));							break;
			case SIGNATURE:				result.set(Decoder.Signature.set(scannerConfig, propertyValue)); 						break;
			case TLC39:					result.set(Decoder.Tlc39.set(scannerConfig, propertyValue)); 							break;
			case TRIOPTIC39:			result.set(Decoder.TriOptic39.set(scannerConfig, propertyValue)); 					break;
			case TRIOPTIC39REDUNDANCY:	result.set(Decoder.TriOptic39.Redundancy.set(scannerConfig, propertyValue));			break;
			case UKPOSTAL:				result.set(Decoder.UkPostal.set(scannerConfig, propertyValue)); 						break;
			case UKPOSTALREPORTCHECKDIGIT:result.set(Decoder.UkPostal.ReportCheckDigit.set(scannerConfig, propertyValue));	break;
			case UPCA:					result.set(Decoder.Upca.set(scannerConfig, propertyValue)); 							break;
			case UPCAPREAMBLE:			result.set(Decoder.Upca.Preamble.set(scannerConfig, propertyValue));					break;
			case UPCAREPORTCHECKDIGIT:	result.set(Decoder.Upca.ReportCheckDigit.set(scannerConfig, propertyValue));			break;
			case UPCE0:					result.set(Decoder.Upce0.set(scannerConfig, propertyValue)); 							break;
			case UPCE0CONVERTTOUPCA:	result.set(Decoder.Upce0.ConvertToUpca.set(scannerConfig, propertyValue));			break;
			case UPCE0PREAMBLE:			result.set(Decoder.Upce0.Preamble.set(scannerConfig, propertyValue)); 				break;
			case UPCE0REPORTCHECKDIGIT:	result.set(Decoder.Upce0.ReportCheckDigit.set(scannerConfig, propertyValue));			break;
			case UPCE1:					result.set(Decoder.Upce1.set(scannerConfig, propertyValue)); 							break;
			case UPCE1CONVERTTOUPCA:	result.set(Decoder.Upce1.ConvertToUpca.set(scannerConfig, propertyValue));			break;
			case UPCE1PREAMBLE:			result.set(Decoder.Upce1.Preamble.set(scannerConfig, propertyValue));					break;
			case UPCE1REPORTCHECKDIGIT:	result.set(Decoder.Upce1.ReportCheckDigit.set(scannerConfig, propertyValue));			break;
			case UPCEANBOOKLAND:		result.set(Decoder.UpcEanParams.BooklandCode.set(scannerConfig, propertyValue));		break;
			case UPCEANBOOKLANDFORMAT:	result.set(Decoder.UpcEanParams.BooklandFormat.set(scannerConfig, propertyValue));	break;
			case UPCEANCONVERTGS1DATABARTOUPCEAN:result.set(Decoder.UpcEanParams.ConvertDataBarToUpcEan.set(scannerConfig, propertyValue)); break;
			case UPCEANCOUPON:			result.set(Decoder.UpcEanParams.CouponCode.set(scannerConfig, propertyValue)); 		break;
			case UPCEANLINEARDECODE:	result.set(Decoder.UpcEanParams.LinearDecode.set(scannerConfig, propertyValue)); 		break;
			case UPCEANRANDOMWEIGHTCHECKDIGIT:	result.set(Decoder.UpcEanParams.RandomWeightCheckDigit.set(scannerConfig, propertyValue)); break;
			case UPCEANRETRYCOUNT:		result.set(Decoder.UpcEanParams.SupplementalRetries.set(scannerConfig, propertyValue));break;
			case UPCEANSECURITYLEVEL:	result.set(Decoder.UpcEanParams.SecurityLevel.set(scannerConfig, propertyValue));		break;
			case UPCEANSUPPLEMENTAL2:	result.set(Decoder.UpcEanParams.Supplemental2.set(scannerConfig, propertyValue));		break;
			case UPCEANSUPPLEMENTAL5:	result.set(Decoder.UpcEanParams.Supplemental5.set(scannerConfig, propertyValue));		break;
			case UPCEANSUPPLEMENTALMODE:result.set(Decoder.UpcEanParams.SupplementalMode.set(scannerConfig, propertyValue));	break;
			case US4STATE:				result.set(Decoder.Us4State.set(scannerConfig, propertyValue));						break;
			case US4STATEFICS:			result.set(Decoder.Us4StateFics.set(scannerConfig, propertyValue));					break;
			case USPLANET:				result.set(Decoder.UsPlanet.set(scannerConfig, propertyValue));						break;
			case USPLANETREPORTCHECKDIGIT:result.set(Decoder.UsPlanet.ReportCheckDigit.set(scannerConfig, propertyValue));	break;
			case USPOSTNET:				result.set(Decoder.UsPostNet.set(scannerConfig, propertyValue));						break;
			case USPOSTNETREPORTCHECKDIGIT:result.set(Decoder.UsPostNet.ReportCheckDigit.set(scannerConfig, propertyValue));	break;
			case WEBCODE:				result.set(Decoder.WebCode.set(scannerConfig, propertyValue));						break;
			case WEBCODEDECODEGTSUBTYPE:result.set(Decoder.WebCode.SubType.set(scannerConfig, propertyValue));				break;
			case ALLDECODERS:			setAllDecodersState(propertyValue, result); break;
			default:break;
			}
			if(isEnabled)
			{
				scanner.setConfig(scannerConfig);
				scannerConfig = scanner.getConfig();
			}
		} catch (ScannerException e) {
			Logger.E(TAG, e);
			if(result != null) result.setError("An error occurred when trying to access the scanner");
			return;
		}
		catch (NumberFormatException e) {
			Logger.W(TAG, e);
			if(result != null) result.setError("Incorrect parameter format");
			return;
		}
		catch (IllegalArgumentException e) {
			Logger.W(TAG, e);
			if(result != null) result.setError(e.getMessage());
			return;
		}
		
		super.setProperty(propertyName, propertyValue, result);
	}

private void setAllDecodersState(String allDecodersString, IMethodResult methodResult){
		
		setProperty(SettingString.AUSPOSTAL.name, allDecodersString, methodResult);
		setProperty(SettingString.AZTEC.name, allDecodersString, methodResult);
		setProperty(SettingString.CANPOSTAL.name, allDecodersString, methodResult);
		setProperty(SettingString.CHINESE2OF5.name, allDecodersString, methodResult);
		setProperty(SettingString.CODABAR.name, allDecodersString, methodResult);
		setProperty(SettingString.CODE11.name, allDecodersString, methodResult);
		setProperty(SettingString.CODE128.name, allDecodersString, methodResult);
		setProperty(SettingString.CODE39.name, allDecodersString, methodResult);
		setProperty(SettingString.CODE93.name, allDecodersString, methodResult);
		setProperty(SettingString.COMPOSITEAB.name, allDecodersString, methodResult);
		setProperty(SettingString.COMPOSITEC.name, allDecodersString, methodResult);
		setProperty(SettingString.D2OF5.name, allDecodersString, methodResult);
		setProperty(SettingString.DATAMATRIX.name, allDecodersString, methodResult);
		setProperty(SettingString.DUTCHPOSTAL.name, allDecodersString, methodResult);
		setProperty(SettingString.EAN13.name, allDecodersString, methodResult);
		setProperty(SettingString.EAN8.name, allDecodersString, methodResult);
		setProperty(SettingString.GS1DATABAR.name, allDecodersString, methodResult);
		setProperty(SettingString.GS1DATABAREXPANDED.name, allDecodersString, methodResult);
		setProperty(SettingString.GS1DATABARLIMITED.name, allDecodersString, methodResult);
		setProperty(SettingString.I2OF5.name, allDecodersString, methodResult);
		setProperty(SettingString.JAPPOSTAL.name, allDecodersString, methodResult);
		setProperty(SettingString.KOREAN3OF5.name, allDecodersString, methodResult);
		setProperty(SettingString.MATRIX2OF5.name, allDecodersString, methodResult);
		setProperty(SettingString.MAXICODE.name, allDecodersString, methodResult);
		setProperty(SettingString.MICROPDF.name, allDecodersString, methodResult);
		setProperty(SettingString.MICROQR.name, allDecodersString, methodResult);
		setProperty(SettingString.MSI.name, allDecodersString, methodResult);
		setProperty(SettingString.PDF417.name, allDecodersString, methodResult);
		setProperty(SettingString.QRCODE.name, allDecodersString, methodResult);
		setProperty(SettingString.SIGNATURE.name, allDecodersString, methodResult);
		setProperty(SettingString.TLC39.name, allDecodersString, methodResult);
		setProperty(SettingString.TRIOPTIC39.name, allDecodersString, methodResult);
		setProperty(SettingString.UKPOSTAL.name, allDecodersString, methodResult);
		setProperty(SettingString.UPCA.name, allDecodersString, methodResult);
		setProperty(SettingString.UPCE0.name, allDecodersString, methodResult);
		setProperty(SettingString.UPCE1.name, allDecodersString, methodResult);
		setProperty(SettingString.US4STATE.name, allDecodersString, methodResult);
		setProperty(SettingString.US4STATEFICS.name, allDecodersString, methodResult);
		setProperty(SettingString.USPLANET.name, allDecodersString, methodResult);
		setProperty(SettingString.USPOSTNET.name, allDecodersString, methodResult);
		setProperty(SettingString.WEBCODE.name, allDecodersString, methodResult);
		super.setProperty("allDecoders", allDecodersString, methodResult);
	}
	@Override
	public void take(Map<String, String> propertyMap, IMethodResult result)
	{
		setProperties(propertyMap, new MethodResult(false));
		enableType = ENABLE_TYPE.TAKE;
		
		takeCallback = result;
		result.keepAlive();
		
		if(!isEnabled) didTakeEnableTheScanner = true;
		enable();
		setProperties(propertyMap, new MethodResult(false));
		startScanning();
	}

	@Override
	public void take_barcode(String rubyCallbackURL, Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.D(LOGTAG, "take_barcode");
		result.setError("take_barcode is not supported on Zebra Enterprise Scanner Engines. Use 'take' or 'enable' instead.");
	}

	@Override
	public void onResume()
	{
		Logger.D(TAG, "onResume");
		
		isMinimized = false;
		if(hasQueuedEnableTask)
		{
			hasQueuedEnableTask = false;
			enable(null, enableCallback);
		}
	}

	@Override
	public void onPause()
	{
		Logger.D(TAG, "onPause+");
		relinquishScanner();
		isMinimized = true;
		Logger.D(TAG, "onPause-");
	}


	@Override
	public void onStop()
	{
		Logger.D(TAG, "onStop+");
		relinquishScanner();
		isMinimized = true;
		Logger.D(TAG, "onStop-");
	}

	/**
	 * Call this when the Activity is minimized
	 */
	private void relinquishScanner()
	{
		Logger.D(TAG, "relinquishScanner+");
		if(!isMinimized)
		{
			stopScanning();
			disable(!isMinimized);
		}
		Logger.D(TAG, "relinquishScanner-");
	}
	
	/**
	 * Enable the scanner if app is visible, else queue the command so it isnt lost.
	 */
	private void enable()
	{
		Logger.D(TAG, "enable()+");
		
		if(scanner != null)
		{
			try
			{	
				if(isMinimized)
				{
					hasQueuedEnableTask = true;
				}
				else
				{
					scanner.enable();
					if(isBluetooth) //For some reason bluetooth scanners dont fire "DISABLED" on a disable call
					{
						onStatus(StatusData.ScannerStates.IDLE);
					}
				}
			}
			catch (ScannerException e)
			{
				Logger.E(TAG, "enable Scanner ERROR, cannot enable scanner: " + e.getMessage());
				e.printStackTrace();
			}
		}
		Logger.D(TAG, "enable()-");
	}

	/**
	 * Disable the device. Reset parameters to default if this is a Rho.Barcode.disable call (as opposed to a minimizing
	 * disable call).
	 * @param isMinimizing if set to true, will re-enable the scanner object when app restored, unless any Rho API call
	 * to disable is made.
	 * @author Ben Kennedy (NCVT73)
	 */
	private void disable(boolean isMinimizing)
	{
		Logger.D(TAG, "disable()+");
		if(scanner != null)
		{
			try
			{
				if(isMinimizing && isEnabled)
				{
					hasQueuedEnableTask = true;
				}

				if(!isMinimized)
				{
					scannerConfig = defaultScannerConfig;
					if(isEnabled) scanner.disable();
					if(isMinimizing) isMinimized = true;
					setAutoTab(Defaults.AUTOTAB, new MethodResult(false));
					setAutoEnter(Defaults.AUTOENTER, new MethodResult(false));
					setDecodeFrequency(Defaults.DECODE_FREQUENCY, new MethodResult(false));
					setDecodeDuration(Defaults.DECODE_DURATION, new MethodResult(false));
					setDecodeVolume(Defaults.DECODE_VOLUME, new MethodResult(false));
				}
			}
			catch (ScannerException e)
			{
				Logger.E(TAG, "disable Scanner ERROR, cannot disable scanner: " + e.getMessage());
				e.printStackTrace();
			}
			finally
			{
				if(!isMinimizing)
				{
					propertyQueue.clear();
					hasQueuedEnableTask = false;
				}
				if(isBluetooth) //For some reason bluetooth scanners dont fire "DISABLED" on a disable call
				{
					onStatus(StatusData.ScannerStates.DISABLED);
				}
			}
		}
		isEnabled = false;
		Logger.D(TAG, "disable()-");
	}

	/**
	 *	Stop the scanner from scanning. 
	 */
	private void stopScanning()
	{
		Logger.D(TAG, "stopScanning+");
		if(isMinimized) return;
		if(scanner != null)
		{
			try
			{
				scanner.cancelRead();
			}
			catch (ScannerException e)
			{
				Logger.E(TAG, "stopScanning Scanner ERROR, cannot stop scanner: " + e.getMessage());
				e.printStackTrace();
			}
		}
		Logger.D(TAG, "stopScanning-");
	}
	
	/**
	 * Start scanning. Doesnt work with 
	 */
	private void startScanning()
	{
		Logger.D(TAG, "startScanning+");
		if(isMinimized) return;
		if(scanner != null)
		{
			try
			{
				scanner.triggerType = TriggerType.SOFT_ONCE;
				scanner.read();
			}
			catch (ScannerException e)
			{
				Logger.E(TAG, "startScanning Scanner ERROR, cannot start scanner: " + e.getMessage());
				e.printStackTrace();
			}
		}
		Logger.D(TAG, "startScanning-");
	}

	@Override
	public void onDestroy()
	{
		Logger.D(TAG, "onDestroy");
		destroy();
	}

	@Override
	public void onStatus(StatusData statusData)
	{
		onStatus(statusData.getState());
	}
	
	/**
	 * This has had to be created as onStatus(StatusData) cannot be called from inside this class as StatusData
	 * doesnt have a public constructor.
	 * onStatus is fired by the scanning library when the scanning hardware state changes. This means that state changes
	 * are asynchronous. Consequently, a lot of scanner setup code is in the IDLE case, as we can now only get the config
	 * if the scanner is enabled. Therefore when the scanner is setup, we do a quick enable->disable cycle to fetch the
	 * config.
	 * @param status the status of the scanner
	 * @author Ben Kennedy (NCVT73)
	 */
	private void onStatus(ScannerStates status)
	{
		//TODO Probably need synchro on propertyQueue
		Logger.D(TAG, scannerNameShort + " onStatus: " + status.name());
		emdk3_0bugOnStatusFired = true;
		switch(status)
		{
		case WAITING: break;//Bluetooth Scanner's Idle (for bluetooth we need to call "read" as it uses the hard scan button)
		case IDLE:
			if(scannerConfig == null) 
			{
				try
				{
					scannerConfig = scanner.getConfig();
					defaultScannerConfig = scannerConfig;
				}
				catch (ScannerException e)
				{
					Logger.E(TAG, "A Scanner error has occurred. Cannot get scanner settings. Scanner settings can not be modified. " + e);
				}
			}
			if(supportedProperties == null) popluateSupportedProperties();
			if(isInitialising)
			{
				try
				{
					scanner.disable();
				}
				catch (ScannerException e)
				{
					// TODO	
					e.printStackTrace();
				}
				return;
			}
			if(scannerConfig != null)
			{
				try
				{
					scanner.setConfig(scannerConfig);
				}
				catch (ScannerException e)
				{
					Logger.E(TAG, "onStatus Scanner ERROR, cannot change config: " + e.getMessage());
					e.printStackTrace();
				}
			}
			isEnabled = true;
			isScanning = false;
			if(propertyQueue.size() > 0)
			{
				for(Pair<String,String> pair: propertyQueue)
				{
					setProperty(pair.first, pair.second, new MethodResult(false)); 
				}
				propertyQueue.clear();
			}
			if(isBluetooth)
			{
				scanner.triggerType = TriggerType.HARD;
				try
				{
					scanner.read();
				}
				catch (ScannerException e)
				{
					Logger.E(TAG, "onStatus Scanner ERROR, cannot initiate Bluetooth scanner read " + e.getMessage());
					e.printStackTrace();
				}
			}
			break;
		case SCANNING:
			isScanning = true;
			break;
		case DISABLED:
			isEnabled = false;
			isScanning = false;
			if(isInitialising)
			{
				AsyncTask.execute(new Runnable()
				{
					@Override
					public void run()
					{
						Logger.D(TAG, scannerNameShort + " has finished initializing");
						isInitialising = false;
						factory.emdk3BarcodeHasSetup();
					}
				});
			}
			break;
		//case ERROR: Logger.E(TAG, "Scanner is in an ERROR state");
		default:
			break;
		}
		Logger.D(TAG, scannerNameShort + " onStatus-");
	}

	@Override
	public void onData(ScanDataCollection scanDataCollection)
	{
		Logger.D(TAG, "onData +");
		//Scan has occurred
	
		switch(enableType)
		{
			case ENABLE:
				for(ScanDataCollection.ScanData scanData : scanDataCollection.getScanData())
				{
					if(enableCallback != null)
					{
						enableCallback.set(EMDKCommon.makeEnableResultMap(scanData, scannerId));
					}
					else
					{
						EMDKCommon.outputAsKeys(scanData.getData(), autoValue);
					}
				}
				break;
			case TAKE:
				for(ScanDataCollection.ScanData scanData : scanDataCollection.getScanData())
				{
					takeCallback.set(EMDKCommon.makeTakeResultMap(scanData, true));
				}
				if(didTakeEnableTheScanner)
				{
					disable(false);
					didTakeEnableTheScanner = false;
				}
				break;
			default:
				Logger.D(TAG, "Unexpected onData Event");
				break;
		}
		Logger.D(TAG, "onData -");
	}
	
	@Override
	public void setDecodeVolume(int decodeVolume, IMethodResult result)
	{		
		Logger.D(LOGTAG, "setDecodeVolume: " + decodeVolume);
		super.setProperty("decodeSound", "", new MethodResult(false));
		super.setProperty("decodeVolume", String.valueOf(decodeVolume), result);
		setProperty("decodeSound", getDecodeAudioFilePath(), result);
	}

	@Override
	public void setDecodeDuration(int decodeDuration, IMethodResult result)
	{
		Logger.D(LOGTAG, "setDecodeDuration: " + decodeDuration);
		super.setProperty("decodeSound", "", new MethodResult(false));
		super.setProperty("decodeDuration", String.valueOf(decodeDuration), result);
		setProperty("decodeSound", getDecodeAudioFilePath(), result);
	}

	@Override
	public void setDecodeFrequency(int decodeFrequency, IMethodResult result)
	{
		Logger.D(LOGTAG, "setDecodeFrequency: " + decodeFrequency);
		super.setProperty("decodeSound", "", new MethodResult(false));
		super.setProperty("decodeFrequency", String.valueOf(decodeFrequency), result);
		setProperty("decodeSound", getDecodeAudioFilePath(), result);
	}

	@Override
	public void setDecodeSound(String decodeSound, IMethodResult result)
	{
		Logger.D(LOGTAG, "setDecodeSound: " + decodeSound);
		super.setProperty("decodeSound", decodeSound, result);
		setProperty("decodeSound", getDecodeAudioFilePath(), null);
	}

	@Override
	public void setAllDecoders(boolean allDecoders, IMethodResult result)
	{
		Logger.D(TAG, "setAllDecoders: " + allDecoders);
		MethodResult methodResult = new MethodResult(false);
		String allDecodersString = Boolean.toString(allDecoders); 
		setAllDecodersState(allDecodersString, methodResult);
		Logger.D(TAG, "setAllDecoders-");
	}

	@Override
	public void setAutoEnter(boolean autoEnter, IMethodResult result)
	{
		Logger.D(TAG, "setAutoEnter: " + autoEnter);
		if(autoEnter) autoValue = AutoValue.AUTOENTER;
		else if(autoValue.equals(AutoValue.AUTOENTER)) autoValue = AutoValue.NONE;
		super.setProperty("autoEnter", Boolean.valueOf(autoEnter).toString(), result);
		Logger.D(TAG, "setAutoEnter-");
	}

	@Override
	public void setAutoTab(boolean autoTab, IMethodResult result)
	{
		Logger.D(TAG, "setAutoTab: " + autoTab);
		if(autoTab) autoValue = AutoValue.AUTOTAB;
		else if(autoValue.equals(AutoValue.AUTOTAB)) autoValue = AutoValue.NONE;
		super.setProperty("autoTab", Boolean.valueOf(autoTab).toString(), result);
		Logger.D(TAG, "setAutoTab-");
	}

	public void destroy()
	{
		if(scanner != null)
		{
			try
			{
				scanner.disable();
			}
			catch (ScannerException e)
			{
				Logger.W(TAG, "Scanner failed to disable on shutdown");
			}
			
			try
			{
				scanner.removeDataListener(this);
				scanner.removeStatusListener(this);
			}
			catch(NullPointerException e) //TODO bad EMDK. Naughty.
			{
				//continue
			}
			scanner = null;
		}
		scannerInfo = null;
		scannerConfig = null;
		propertyQueue = null;
	}

	/**
	 * Enum containing all known Scanning Settings, together with its Rho name and EMDK class name, where possible.
	 * Class name can be used to check if a setting is supported.
	 * @author Ben Kennedy (NCVT73)
	 */
	public enum SettingString
	{
		ALLDECODERS							("allDecoders", null),
		AUTOENTER							("autoEnter", null),
		AUTOTAB								("autoTab", null),
		DECODEDURATION						("decodeDuration", null),
		DECODEFREQUENCY						("decodeFrequency", null),
		DECODESOUND							("decodeSound", null),
		DECODEVOLUME						("decodeVolume", null),
		FRIENDLYNAME						("friendlyName", null),
		ID									("id", null),
		SCANNERTYPE							("scannerType", null),
		ADAPTIVESCANNING					("adaptiveScanning", null),
		AIMMODE								("aimMode", null),
		AIMTYPE								("aimType", null),
		AUSPOSTAL							("ausPostal", "config.decoderParams.australianPostal.enabled"),
		AZTEC								("aztec", "config.decoderParams.aztec.enabled"),
		BARCODEDATAFORMAT					("barcodeDataFormat", null),
		BEAMWIDTH							("beamWidth", null),
		BIDIRECTIONALREDUNDANCY				("bidirectionalRedundancy", null),
		CANPOSTAL							("canPostal", "config.decoderParams.canadianPostal.enabled"),
		CHINESE2OF5							("chinese2of5", "config.decoderParams.chinese2of5.enabled"),
		CODABAR								("codabar", "config.decoderParams.codaBar.enabled"),
		CODABARCLSIEDITING					("codabarClsiEditing", "config.decoderParams.codaBar.clsiEditing"),
		CODABARMAXLENGTH					("codabarMaxLength", "config.decoderParams.codaBar.length2"),
		CODABARMINLENGTH					("codabarMinLength", "config.decoderParams.codaBar.length1"),
		CODABARNOTISEDITING					("codabarNotisEditing", "config.decoderParams.codaBar.notisEditing"),
		CODABARREDUNDANCY					("codabarRedundancy", "config.decoderParams.codaBar.redundancy"),
		CODE11								("code11", "config.decoderParams.code11.enabled"),
		CODE11CHECKDIGITCOUNT				("code11checkDigitCount", null),
		CODE11MAXLENGTH						("code11maxLength", "config.decoderParams.code11.length2"),
		CODE11MINLENGTH						("code11minLength", "config.decoderParams.code11.length1"),
		CODE11REDUNDANCY					("code11redundancy", "config.decoderParams.code11.redundancy"),
		CODE11REPORTCHECKDIGIT				("code11reportCheckDigit", "config.decoderParams.code11.reportCheckDigit"),
		CODE128								("code128", "config.decoderParams.code128.enabled"),
		CODE128CHECKISBTTABLE				("code128checkIsBtTable", "config.decoderParams.code128.checkIsbtTable"),
		CODE128EAN128						("code128ean128", "config.decoderParams.code128.enableEan128"),
		CODE128ISBT128						("code128isbt128", "config.decoderParams.code128.enableIsbt128"),
		CODE128ISBT128CONCATMODE			("code128isbt128ConcatMode", "config.decoderParams.code128.isbt128ConcatMode"),
		CODE128MAXLENGTH					("code128maxLength", "config.decoderParams.code128.length2"),
		CODE128MINLENGTH					("code128minLength", "config.decoderParams.code128.length1"),
		CODE128OTHER128						("code128other128", "config.decoderParams.code128.enablePlain"),
		CODE128REDUNDANCY					("code128redundancy", "config.decoderParams.code128.redundancy"),
		CODE128SECURITYLEVEL				("code128securityLevel", "config.decoderParams.code128.securityLevel"),
		CODE39								("code39", "config.decoderParams.code39.enabled"),
		CODE39CODE32PREFIX					("code39code32Prefix", "config.decoderParams.code39.reportCode32Prefix"),
		CODE39CONVERTTOCODE32				("code39convertToCode32", "config.decoderParams.code39.convertToCode32"),
		CODE39FULLASCII						("code39fullAscii", "config.decoderParams.code39.fullAscii"),
		CODE39MAXLENGTH						("code39maxLength", "config.decoderParams.code39.length2"),
		CODE39MINLENGTH						("code39minLength", "config.decoderParams.code39.length1"),
		CODE39REDUNDANCY					("code39redundancy", "config.decoderParams.code39.redundancy"),
		CODE39REPORTCHECKDIGIT				("code39reportCheckDigit", "config.decoderParams.code39.reportCheckDigit"),
		CODE39SECURITYLEVEL					("code39securityLevel", "config.decoderParams.code39.securityLevel"),
		CODE39VERIFYCHECKDIGIT				("code39verifyCheckDigit", "config.decoderParams.code39.verifyCheckDigit"),
		CODE93								("code93", "config.decoderParams.code93.enabled"),
		CODE93MAXLENGTH						("code93maxLength", "config.decoderParams.code93.length2"),
		CODE93MINLENGTH						("code93minLength", "config.decoderParams.code93.length1"),
		CODE93REDUNDANCY					("code93redundancy", "config.decoderParams.code93.redundancy"),
		COMPOSITEAB							("compositeAb", "config.decoderParams.compositeAB.enabled"),
		COMPOSITEABUCCLINKMODE				("compositeAbUccLinkMode", "config.decoderParams.compositeAB.uccLinkMode"),
		COMPOSITEABUSEUPCPREAMBLECHECKDIGITRULES("compositeAbUseUpcPreambleCheckDigitRules", null),
		COMPOSITEC							("compositec", "config.decoderParams.compositeC.enabled"),
		CONNECTIONIDLETIMEOUT				("connectionIdleTimeout", null),
		D2OF5								("d2of5", "config.decoderParams.d2of5.enabled"),
		D2OF5MAXLENGTH						("d2of5maxLength", "config.decoderParams.d2of5.length2"),
		D2OF5MINLENGTH						("d2of5minLength", "config.decoderParams.d2of5.length1"),
		D2OF5REDUNDANCY						("d2of5redundancy", "config.decoderParams.d2of5.redundancy"),
		DATABUFFERSIZE						("dataBufferSize", null),
		DATAMATRIX							("dataMatrix", "config.decoderParams.dataMatrix.enabled"),
		DBPMODE								("dbpMode", null),
		DIFFERENTSYMBOLTIMEOUT				("differentSymbolTimeout", null),
		DISABLESCANNERDURINGNAVIGATE		("disableScannerDuringNavigate", null),
		DISCONNECTBTONDISABLE				("disconnectBtOnDisable", null),
		DISPLAYBTADDRESSBARCODEONENABLE		("displayBtAddressBarcodeOnEnable", null),
		DPMMODE								("dpmMode", null),
		DUTCHPOSTAL							("dutchPostal", "config.decoderParams.dutchPostal.enabled"),
		EAN13								("ean13", "config.decoderParams.ean13.enabled"),
		EAN8								("ean8", "config.decoderParams.ean8.enabled"),
		EAN8CONVERTTOEAN13					("ean8convertToEan13", "config.decoderParams.ean8.convertToEan13"),
		ENABLETIMEOUT						("enableTimeout", null),
		FOCUSMODE							("focusMode", null),
		GS1DATABAR							("gs1databar", "config.decoderParams.gs1Databar.enabled"),
		GS1DATABAREXPANDED					("gs1databarExpanded", "config.decoderParams.gs1DatabarExp.enabled"),
		GS1DATABARLIMITED					("gs1databarLimited", "config.decoderParams.gs1DatabarLim.enabled"),
		HAPTICFEEDBACK						("hapticFeedback", "config.scanParams.decodeHapticFeedback"),
		I2OF5								("i2of5", "config.decoderParams.i2of5.enabled"),
		I2OF5CONVERTTOEAN13					("i2of5convertToEan13", "config.decoderParams.i2of5.convertToEan13"),
		I2OF5MAXLENGTH						("i2of5maxLength", "config.decoderParams.i2of5.length2"),
		I2OF5MINLENGTH						("i2of5minLength", "config.decoderParams.i2of5.length1"),
		I2OF5REDUNDANCY						("i2of5redundancy", "config.decoderParams.i2of5.redundancy"),
		I2OF5REPORTCHECKDIGIT				("i2of5reportCheckDigit", "config.decoderParams.i2of5.reportCheckDigit"),
		I2OF5VERIFYCHECKDIGIT				("i2of5verifyCheckDigit", "config.decoderParams.i2of5.verifyCheckDigit"),
		ILLUMINATIONMODE					("illuminationMode", null),
		INVALIDDECODEFREQUENCY				("invaliddecodeFrequency", null),
		INVALIDDECODESOUND					("invaliddecodeSound", null),
		INVERSE1DMODE						("inverse1dMode", null),
		JAPPOSTAL							("japPostal", "config.decoderParams.japanesePostal.enabled"),
		KLASSEEINS							("klasseEins", null),
		KOREAN3OF5							("korean3of5", "config.decoderParams.korean3of5.enabled"),
		KOREAN3OF5MAXLENGTH					("korean3of5maxLength", null),
		KOREAN3OF5MINLENGTH					("korean3of5minLength", null),
		KOREAN3OF5REDUNDANCY				("korean3of5redundancy", null),
		LCDMODE								("lcdMode", null),
		LINEARSECURITYLEVEL					("linearSecurityLevel", null),
		LOWBATTERYSCAN						("lowBatteryScan", null),
		MACROMICROPDF						("macroMicroPdf", null),
		MACROMICROPDFBUFFERLABELS			("macroMicroPdfBufferLabels", null),
		MACROMICROPDFCONVERTTOMICROPDF		("macroMicroPdfConvertToMicroPdf", null),
		MACROMICROPDFEXCLUSIVE				("macroMicroPdfExclusive", null),
		MACROMICROPDFREPORTAPPENDINFO		("macroMicroPdfReportAppendInfo", null),
		MACROPDF							("macroPdf", null),
		MACROPDFBUFFERLABELS				("macroPdfBufferLabels", null),
		MACROPDFCONVERTTOPDF417				("macroPdfConvertToPdf417", null),
		MACROPDFEXCLUSIVE					("macroPdfExclusive", null),
		MATRIX2OF5							("matrix2of5", "config.decoderParams.matrix2of5.enabled"),
		MATRIX2OF5MAXLENGTH					("matrix2of5maxLength", "config.decoderParams.matrix2of5.length2"),
		MATRIX2OF5MINLENGTH					("matrix2of5minLength", "config.decoderParams.matrix2of5.length1"),
		MATRIX2OF5REPORTCHECKDIGIT			("matrix2of5reportCheckDigit", "config.decoderParams.matrix2of5.reportCheckDigit"),
		MATRIX2OF5VERIFYCHECKDIGIT			("matrix2of5verifyCheckDigit", "config.decoderParams.matrix2of5.verifyCheckDigit"),
		MAXICODE							("maxiCode", "config.decoderParams.maxiCode.enabled"),
		MICROPDF							("microPdf", "config.decoderParams.microPdf.enabled"),
		MICROQR								("microQr", "config.decoderParams.microQr.enabled"),
		MSI									("msi", "config.decoderParams.msi.enabled"),
		MSICHECKDIGITS						("msiCheckDigits", "config.decoderParams.msi.checkDigits"),
		MSICHECKDIGITSCHEME					("msiCheckDigitScheme", "config.decoderParams.msi.checkDigitScheme"),
		MSIMAXLENGTH						("msiMaxLength", "config.decoderParams.msi.length2"),
		MSIMINLENGTH						("msiMinLength", "config.decoderParams.msi.length1"),
		MSIREDUNDANCY						("msiRedundancy", "config.decoderParams.msi.redundancy"),
		MSIREPORTCHECKDIGIT					("msiReportCheckDigit", "config.decoderParams.msi.reportCheckDigit"),
		PDF417								("pdf417", "config.decoderParams.pdf417.enabled"),
		PICKLISTMODE						("picklistMode", null),
		POORQUALITY1DMODE					("poorQuality1dMode", null),
		QRCODE								("qrCode", "config.decoderParams.qrCode.enabled"),
		RASTERHEIGHT						("rasterHeight", null),
		RASTERMODE							("rasterMode", null),
		RSMBATTERYCAPACITY					("rsmBatteryCapacity", null),
		RSMBATTERYID						("rsmBatteryId", null),
		RSMBATTERYSTATUS					("rsmBatteryStatus", null),
		RSMBLUETOOTHADDRESS					("rsmBluetoothAddress", null),
		RSMBLUETOOTHAUTHENTICATION			("rsmBluetoothAuthentication", null),
		RSMBLUETOOTHAUTORECONNECT			("rsmBluetoothAutoReconnect", null),
		RSMBLUETOOTHBEEPONRECONNECTATTEMPT	("rsmBluetoothBeepOnReconnectAttempt", null),
		RSMBLUETOOTHENCRYPTION				("rsmBluetoothEncryption", null),
		RSMBLUETOOTHFRIENDLYNAME			("rsmBluetoothFriendlyName", null),
		RSMBLUETOOTHHIDAUTORECONNECT		("rsmBluetoothHidAutoReconnect", null),
		RSMBLUETOOTHINQUIRYMODE				("rsmBluetoothInquiryMode", null),
		RSMBLUETOOTHPINCODE					("rsmBluetoothPinCode", null),
		RSMBLUETOOTHPINCODETYPE				("rsmBluetoothPinCodeType", null),
		RSMBLUETOOTHRECONNECTIONATTEMPTS	("rsmBluetoothReconnectionattempts", null),
		RSMDATEOFMANUFACTURE				("rsmDateOfManufacture", null),
		RSMDATEOFSERVICE					("rsmDateOfService", null),
		RSMDECODEFEEDBACK					("rsmDecodeFeedback", null),
		RSMDEVICECLASS						("rsmDeviceClass", null),
		RSMFIRMWAREVERSION					("rsmFirmwareVersion", null),
		RSMFORCESAVEPAIRINGBARCODE			("rsmForceSavePairingBarcode", null),
		RSMGOODSCANSDELAY					("rsmGoodScansDelay", null),
		RSMIGNORECODE128USPS				("rsmIgnoreCode128Usps", null),
		RSMLOWBATTERYINDICATION				("rsmLowBatteryIndication", null),
		RSMLOWBATTERYINDICATIONCYCLE		("rsmLowBatteryIndicationCycle", null),
		RSMMEMS								("rsmMems", null),
		RSMMODELNUMBER						("rsmModelNumber", null),
		RSMPAGINGBEEPSEQUENCE				("rsmPagingBeepSequence", null),
		RSMPAGINGENABLE						("rsmPagingEnable", null),
		RSMPROXIMITYCONTINUOUS				("rsmProximityContinuous", null),
		RSMPROXIMITYDISTANCE				("rsmProximityDistance", null),
		RSMPROXIMITYENABLE					("rsmProximityEnable", null),
		RSMSCANLINEWIDTH					("rsmScanLineWidth", null),
		RSMSCANTRIGGERWAKEUP				("rsmScanTriggerWakeup", null),
		RSMSERIALNUMBER						("rsmSerialNumber", null),
		SAMESYMBOLTIMEOUT					("sameSymbolTimeout", null),
		SCANTIMEOUT							("scanTimeout", null),
		SIGNATURE							("signature", "config.decoderParams.signature.enabled"),
		SIGNATUREIMAGEHEIGHT				("signatureImageHeight", null),
		SIGNATUREIMAGEQUALITY				("signatureImageQuality", null),
		SIGNATUREIMAGEWIDTH					("signatureImageWidth", null),
		TIMEDAIMDURATION					("timedAimDuration", null),
		TLC39								("tlc39", "config.decoderParams.tlc39.enabled"),
		TRIGGERCONNECTED					("triggerConnected", null),
		TRIOPTIC39							("trioptic39", "config.decoderParams.triOptic39.enabled"),
		TRIOPTIC39REDUNDANCY				("trioptic39Redundancy", "config.decoderParams.triOptic39.redundancy"),
		UKPOSTAL							("ukPostal", "config.decoderParams.ukPostal.enabled"),
		UKPOSTALREPORTCHECKDIGIT			("ukPostalReportCheckDigit", "config.decoderParams.ukPostal.reportCheckDigit"),
		UPCA								("upca", "config.decoderParams.upca.enabled"),
		UPCAPREAMBLE						("upcaPreamble", "config.decoderParams.upca.preamble"),
		UPCAREPORTCHECKDIGIT				("upcaReportCheckDigit", "config.decoderParams.upca.reportCheckDigit"),
		UPCE0								("upce0", "config.decoderParams.upce0.enabled"),
		UPCE0CONVERTTOUPCA					("upce0convertToUpca", "config.decoderParams.upce0.convertToUpca"),
		UPCE0PREAMBLE						("upce0preamble", "config.decoderParams.upce0.preamble"),
		UPCE0REPORTCHECKDIGIT				("upce0ReportCheckDigit", "config.decoderParams.upce0.reportCheckDigit"),
		UPCE1								("upce1", "config.decoderParams.upce1.enabled"),
		UPCE1CONVERTTOUPCA					("upce1convertToUpca", "config.decoderParams.upce1.convertToUpca"),
		UPCE1PREAMBLE						("upce1preamble", "config.decoderParams.upce1.preamble"),
		UPCE1REPORTCHECKDIGIT				("upce1ReportCheckDigit", "config.decoderParams.upce1.reportCheckDigit"),
		UPCEANBOOKLAND						("upcEanBookland", "config.decoderParams.upcEanParams.booklandCode"),
		UPCEANBOOKLANDFORMAT				("upcEanBooklandFormat", "config.decoderParams.upcEanParams.booklandFormat"),
		UPCEANCONVERTGS1DATABARTOUPCEAN		("upcEanConvertGs1DatabarToUpcEan", "config.decoderParams.upcEanParams.convertDataBarToUpcEan"),
		UPCEANCOUPON						("upcEanCoupon", "config.decoderParams.upcEanParams.couponCode"),
		UPCEANLINEARDECODE					("upcEanLinearDecode", "config.decoderParams.upcEanParams.linearDecode"),
		UPCEANRANDOMWEIGHTCHECKDIGIT		("upcEanRandomWeightCheckDigit", "config.decoderParams.upcEanParams.randomWeightCheckDigit"),
		UPCEANRETRYCOUNT					("upcEanRetryCount", "config.decoderParams.upcEanParams.supplementalRetries"),
		UPCEANSECURITYLEVEL					("upcEanSecurityLevel", "config.decoderParams.upcEanParams.securityLevel"),
		UPCEANSUPPLEMENTAL2					("upcEanSupplemental2", "config.decoderParams.upcEanParams.supplemental2"),
		UPCEANSUPPLEMENTAL5					("upcEanSupplemental5", "config.decoderParams.upcEanParams.supplemental5"),
		UPCEANSUPPLEMENTALMODE				("upcEanSupplementalMode", "config.decoderParams.upcEanParams.supplementalMode"),
		US4STATE							("us4state", "config.decoderParams.us4State.enabled"),
		US4STATEFICS						("us4stateFics", "config.decoderParams.us4StateFics.enabled"),
		USPLANET							("usPlanet", "config.decoderParams.usPlanet.enabled"),
		USPLANETREPORTCHECKDIGIT			("usPlanetReportCheckDigit", "config.decoderParams.usPlanet.reportCheckDigit"),
		USPOSTNET							("usPostnet", "config.decoderParams.usPostNet.enabled"),
		USPOSTNETREPORTCHECKDIGIT			("usPostnetReportCheckDigit", "config.decoderParams.usPostNet.reportCheckDigit"),
		VIEWFINDERFEEDBACK					("viewfinderFeedback", null),
		VIEWFINDERFEEDBACKTIME				("viewfinderFeedbacktime", null),
		VIEWFINDERHEIGHT					("viewfinderHeight", null),
		VIEWFINDERMODE						("viewfinderMode", null),
		VIEWFINDERWIDTH						("viewfinderWidth", null),
		VIEWFINDERX							("viewfinderX", null),
		VIEWFINDERY							("viewfinderY", null),
		WEBCODE								("webcode", "config.decoderParams.webCode.enabled"),
		WEBCODEDECODEGTSUBTYPE				("webcodeDecodeGtSubtype", "config.decoderParams.webCode.subType");
		
		private final String name;
		private final String emdkName;
		
		SettingString (String name, String emdkName)
		{
			this.name = name;
			this.emdkName = emdkName;
		}		
	}
	
	static
	{
		SettingString[] settingStrings = SettingString.values();
		for(SettingString setting: settingStrings)
		{
			allProperties.add(setting.name);
		}
	}
}
