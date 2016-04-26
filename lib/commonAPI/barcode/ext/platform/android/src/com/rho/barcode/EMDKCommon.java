package com.rho.barcode;

import java.io.File;
import java.lang.reflect.Field;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import android.app.Instrumentation;


import com.rho.barcode.RhoScannerInfo.RhoScannerClass;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.symbol.emdk.barcode.ScanDataCollection;
import com.symbol.emdk.barcode.ScanDataCollection.ScanData;

public class EMDKCommon
{
	private static final String TAG = "EMDKCommon";//"BENKEN-EC";
	
	public static final String ALL_DECODERS = "allDecoders";
	public static final String AUTO_ENTER = "autoEnter";
	public static final String AUTO_TAB = "autoTab";
	public static final String DECODE_VOLUME = "decodeVolume";
	public static final String DECODE_DURATION = "decodeDuration";
	public static final String DECODE_FREQUENCY = "decodeFrequency";
	public static final String DECODE_SOUND = "decodeSound";
	public static final String DECODE_AUDIO_FEEDBACK_URI = "decode_audio_feedback_uri";
	public static final String FRIENDLY_NAME = "friendlyName";
	public static final String ID = "id";
	public static final String SCANNER_TYPE = "scannerType";
	
	public enum AutoValue { NONE, AUTOENTER, AUTOTAB }
	
	private static class RhoScanResult
	{
		private String data;
		private Date time;
		private String label;
		
		/**
		 * EMDK3
		 * @param scanData
		 * @param scannerId
		 * @return
		 */
		public RhoScanResult(ScanData scanData)
		{
			Logger.D(TAG, "RhoScanResult+");
			Logger.D(TAG, "scanData.getTime: "+ scanData.getTimeStamp());
			Logger.D(TAG, "scanData.getData: "+ scanData.getData());
			Logger.D(TAG, "scanData.getLabelType(): "+ scanData.getLabelType().toString());
			this.data = scanData.getData();
			try
			{
				String[] cutTime = scanData.getTimeStamp().split("[.]");
				this.time = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse(cutTime[0]);
			}
			catch (ParseException e)
			{
				Logger.E(TAG, "Could not parse Scan Time");
				this.time = new Date();
			}
			this.label = scanData.getLabelType().toString();//TODO check that all of the ENUMs are the same as the EMDK3 values
		}
		
		
		

		public String getLabelType()
		{
			return label;
		}

		public String getData()
		{
			return data;
		}
		
		public String getTimeString()
		{
			return (new SimpleDateFormat("hh:mm:ss")).format(time);
		}
	}
	
	/**
	 * EMDK3
	 * @param scanData
	 * @param scannerId
	 * @return
	 */
	public static HashMap<String, Object> makeTakeResultMap(ScanData scanData, boolean isScanned)
	{
		return makeTakeResultMap(new RhoScanResult(scanData), isScanned);
	}

	/**
	 * EMDK3
	 * @param scanData
	 * @param scannerId
	 * @return
	 */
	public static HashMap<String, Object> makeEnableResultMap(ScanData scanData, String scannerId)
	{
		return makeEnableResultMap(new RhoScanResult(scanData), scannerId);
	}
	
	
	

	/**
	 * Structures the scanResult data into a MethodResult passed in by the enable method compatible Map
	 * @param scanResult The result from which to extract the data
	 * @return the HashMap containing the data of the scan
	 * @author Ben Kennedy NCVT73
	 */
	public static HashMap<String, Object> makeEnableResultMap(RhoScanResult scanResult, String scannerId)
	{
		Logger.D(TAG, "EMDKScanner makeEnableResultMap");
		Logger.D(TAG, "EMDKScanner makeEnableResultMap EMDK Label Type: " + scanResult.getLabelType());
		Logger.D(TAG, "EMDKScanner makeEnableResultMap winTypeConversionMatrix length: " + WinScannerResult.winTypeConversionMatrix.size());
		WinScannerResult scannerType = WinScannerResult.winTypeConversionMatrix.get(scanResult.getLabelType());
		if(scannerType == null)
		{
			Logger.E(TAG, "Unknown ScannerType");
			scannerType = new WinScannerResult("Unknown", "Unknown");
		}

		HashMap<String, Object> resultMap = new HashMap<String, Object>();

		resultMap.put("data", scanResult.getData());
		resultMap.put("source", scannerId +":" + scannerType.getTypeLabel());
		resultMap.put("type", scannerType.getTypeCode());
		resultMap.put("time", scanResult.getTimeString());
		resultMap.put("length", Integer.toString(scanResult.getData().length()));
		resultMap.put("direction", "unavailable");
		return resultMap;
	}

	/**
	 * Structures the scanResult data into a MethodResult passed in by the take method compatible Map
	 * @param scanResult The result from which to extract the data
	 * @param isScanned whether this is a scanEvent (true) or a stopScanning(false)
	 * @return the HashMap containing the data of the scan
	 * @author Ben Kennedy NCVT73
	 */
	public static HashMap<String, Object> makeTakeResultMap(RhoScanResult scanResult, boolean isScanned)
	{
		Logger.D(TAG, "EMDKScanner makeEnableResultMap");
		HashMap<String, Object> resultMap = new HashMap<String, Object>();
		if(isScanned)
		{
			resultMap.put("barcode", scanResult.getData());
			resultMap.put("status", "ok");
		}
		else
		{
			resultMap.put("barcode", "");
			resultMap.put("status", "cancel");
		}
		return resultMap;
	}
	
	/**
	 * This hashmap is used to map the barcode symbologies identified by hex codes to the same description
	 * as on RE WinMo/WinCE
	 * @author Ben Kennedy NCVT73
	 */
	private static class WinScannerResult
	{
		private String typeLabel;
		private String typeCode;
		public static HashMap<String, WinScannerResult> winTypeConversionMatrix = null;

		public WinScannerResult(String typeLabel, String typeCode)
		{
			this.typeLabel = typeLabel;
			this.typeCode = typeCode;
		}

		public String getTypeLabel()
		{
			return typeLabel;
		}

		public String getTypeCode()
		{
			return typeCode;
		}

				/**
		 * Sets up the EMDK 2 type conversion matrix. This converts EMDK 2 return types to their windows equivalent
		 * to be reported through decodeEvent
		 * @author Ben Kennedy NCVT73
		 */
		private static void initialiseMatrixEMDK3()
		{
			if(winTypeConversionMatrix != null)
			{
				Logger.D(TAG, "EMDKScanner WinScannerResult initialiseMatrix not required");
				return;
			}
			Class<?> scannerClass;
			try
			{
				String className = "com.symbol.emdk.barcode.Scanner";
				scannerClass = Class.forName(className);
			}
			catch (ClassNotFoundException e)
			{
				Logger.E(TAG, "Cannot load EMDK 3 Scanner class");
				return;
			}
			
			winTypeConversionMatrix = new HashMap<String, WinScannerResult>();
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.AUSPOSTAL.name(), new WinScannerResult("AUSTRALIAN POSTAL", "0x65"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.AZTEC.name(), new WinScannerResult("AZTEC", "0x74"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.BOOKLAND.name(), new WinScannerResult("BOOKLAND EAN", "0x44"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.CANPOSTAL.name(), new WinScannerResult("CANADA POSTBAR", "0x67"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.CHINESE_2OF5.name(), new WinScannerResult("CHINESE 2 Of 5", "0x70"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.CODABAR.name(), new WinScannerResult("CODABAR", "0x36"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.CODE11.name(), new WinScannerResult("CODE 11", "0x3a"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.CODE128.name(), new WinScannerResult("CODE 128", "0x3c"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.CODE32.name(), new WinScannerResult("CODE 32", "0x46"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.CODE39.name(), new WinScannerResult("CODE 3 OF 9", "0x37"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.CODE93.name(), new WinScannerResult("CODE 93", "0x3b"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.COMPOSITE_AB.name(), new WinScannerResult("COMPOSITE AB", "0x56"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.COMPOSITE_C.name(), new WinScannerResult("COMPOSITE C", "0x57"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.COUPON.name(), new WinScannerResult("COUPON CODE", "0x43"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.D2OF5.name(), new WinScannerResult("DISCRETE 2 OF 5", "0x38"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.DATABAR_COUPON.name(), new WinScannerResult("DATABAR COUPON", "0x49")); //TODO Inform Darryn
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.DATAMATRIX.name(), new WinScannerResult("DATAMATRIX", "0x49"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.DUTCHPOSTAL.name(), new WinScannerResult("DUTCH POSTAL", "0x66"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.EAN128.name(), new WinScannerResult("EAN 128", "0x3f"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.EAN13.name(), new WinScannerResult("EAN13", "0x35"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.EAN8.name(), new WinScannerResult("EAN8", "0x34"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.GS1_DATABAR.name(), new WinScannerResult("RSS-14", "0x4c"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.GS1_DATABAR_EXP.name(), new WinScannerResult("RSS EXPANDED", "0x4e"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.GS1_DATABAR_LIM.name(), new WinScannerResult("RSS LIMITED", "0x4d"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.I2OF5.name(), new WinScannerResult("INTERLEAVED 2 OF 5", "0x39"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.IATA2OF5.name(), new WinScannerResult("IATA 2 OF 5", "0x3e"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.ISBT128.name(), new WinScannerResult("ISBT 128", "0x41"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.JAPPOSTAL.name(), new WinScannerResult("JAPANESE POSTAL", "0x64"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.KOREAN_3OF5.name(), new WinScannerResult("Korean 3 Of 5", "0x76"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.MATRIX_2OF5.name(), new WinScannerResult("MATRIX 2 OF 5", "0x7a"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.MAXICODE.name(), new WinScannerResult("MAXICODE", "0x48"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.MICROPDF.name(), new WinScannerResult("MICRO PDF", "0x45"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.MICROQR.name(), new WinScannerResult("MicroQR", "0x75"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.MSI.name(), new WinScannerResult("MSI", "0x33"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.OCR.name(), new WinScannerResult("OCR", "0x7b"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.PDF417.name(), new WinScannerResult("PDF 417", "0x40"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.QRCODE.name(), new WinScannerResult("QRCODE", "0x4a"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.SIGNATURE.name(), new WinScannerResult("SIGNATURE", "0x52"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.TLC39.name(), new WinScannerResult("TCIF LINKED CODE 39", "0x58"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.TRIOPTIC39.name(), new WinScannerResult("TRIOPTIC 3 OF 9", "0x42"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.UKPOSTAL.name(), new WinScannerResult("UK POSTAL", "0x63"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.UPCA.name(), new WinScannerResult("UPCA", "0x32"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.UPCE0.name(), new WinScannerResult("UPCE0", "0x30"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.UPCE1.name(), new WinScannerResult("UPCE1", "0x31"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.US4STATE.name(), new WinScannerResult("US 4-State Postal", "0x77"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.US4STATE_FICS.name(), new WinScannerResult("US 4-State Postal FICS", "0x79"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.USPLANET.name(), new WinScannerResult("US PLANET", "0x62"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.USPOSTNET.name(), new WinScannerResult("US POSTNET", "0x61"));
			winTypeConversionMatrix.put(ScanDataCollection.LabelType.WEBCODE.name(), new WinScannerResult("SCANLET WEBCODE", "0x54"));
			winTypeConversionMatrix.put("Unknown", new WinScannerResult("Unknown", "0xff"));
		}
		
		/**
		 * This method adds an entry to the type conversion matrix via reflection. This is so that new code
		 * can run on old BSPs
		 * @param scannerClass The EMDK Scanner class
		 * @param fieldName the field to test and add
		 * @param result The conversion data
		 * @return whether the entry was added or not
		 * @author Ben Kennedy NCVT73
		 */
		private static boolean addCompatibilityEntryReflect(Class<?> scannerClass, String fieldName, WinScannerResult result)
		{
			if(scannerClass == null || fieldName == null || result == null)
			{
				Logger.D(TAG, "Null input");
				return false;
			}

			try
			{
				Field field = scannerClass.getField(fieldName);
				String labelString = "";
				labelString = (String) field.get(labelString);
				winTypeConversionMatrix.put(labelString, result);
				Logger.D(TAG, fieldName + " added successfully");
				return true;
			}
			catch(NoSuchFieldException e)
			{
				Logger.D(TAG, fieldName + " does not exist: 1");
			}
			catch (IllegalArgumentException e)
			{
				Logger.D(TAG, fieldName + " does not exist: 2");
			}
			catch (IllegalAccessException e)
			{
				Logger.D(TAG, fieldName + " does not exist: 3");
			}
			return false;
		}
	}
	
	/**
	 * Outputs the decoded barcode as keystrokes
	 * @param string the string to output as keystrokes
	 */
	public static void outputAsKeys(String string, AutoValue autoValue)
	{
		switch(autoValue)
		{
			case AUTOENTER: string += "\n"; break;
			case AUTOTAB: string += "\t"; break;
			default: break;
		}
		KeyEventGenerator keyGen = new KeyEventGenerator(string);
		keyGen.start();
	}
	
	public static class KeyEventGenerator extends Thread
	{
		private String injection;

		public KeyEventGenerator(String injection)
		{
			this.injection = injection;
		}

		@Override
		public void run()
		{
			Instrumentation ins = new Instrumentation();
			ins.sendStringSync(injection);
		}
	}
	


	public static void setupProperties(Barcode barcode, RhoScannerInfo scannerInfo)
	{
		//Set up the decode sound
		HashMap<String,String> defaultProperties = new HashMap<String, String>();
		defaultProperties.put(DECODE_VOLUME, "5");
		defaultProperties.put(DECODE_DURATION, "250");
		defaultProperties.put(DECODE_FREQUENCY, "3000");
		barcode.setProperties(defaultProperties, new MethodResult(false));
		
		MethodResult tempResult = new MethodResult(false);
		barcode.setPropertySuper(FRIENDLY_NAME, scannerInfo.getFriendlyName(), tempResult);

		tempResult = new MethodResult(false);
		barcode.setPropertySuper(ID, barcode.getId(), tempResult);
		
		tempResult = new MethodResult(false);
		String scannerType;
		RhoScannerClass scannerClass = scannerInfo.getScannerClass();
		if(scannerClass.equals(RhoScannerClass.SCANNER_CLASS_CAMERA)) {
			scannerType = "Camera";
		}
		else if(scannerClass.equals(RhoScannerClass.SCANNER_CLASS_IMAGER))	{
			scannerType = "Imager";
		}
		else if(scannerClass.equals(RhoScannerClass.SCANNER_CLASS_LASER)) {
			scannerType = "Laser";
		}
		else {
			scannerType = "Unknown";
		}
		barcode.setPropertySuper(SCANNER_TYPE, scannerType, tempResult);
	}

	public static void setProperties(Barcode barcode, Map<String, String> properties, IMethodResult result)
	{
		Logger.D(TAG, "EMDKScanner setProperties");
		if(properties != null)
		{
			if(result != null) ((MethodResult) result).keepAlive();
			for(Entry<String, String> entry: properties.entrySet())
			{
				String key = entry.getKey();
				Object value = entry.getValue();
				String strValue = "";
				if(value instanceof Boolean) strValue = ((Boolean) value).toString();
				else if(value instanceof Integer) strValue = ((Integer) value).toString();
				else if(value instanceof String) strValue = (String) value;
				else continue;
				barcode.setProperty(key, strValue, result);
			}
			if(result != null) ((MethodResult) result).release();
		}

	}
	
	public static String getDecodeAudioFilePath(String decodeSound, int decodeDuration, int decodeFrequency, int decodeVolume)
	{
		if(decodeSound != null && decodeSound.length() > 0)
		{
			Logger.D(TAG, "DecodeSound: Using Decode Sound");
			String normalisedPath = RhoFileApi.normalizePath(decodeSound);
			Logger.D(TAG, "DecodeSound: Normalising audioPath from: " + decodeSound + " to: " + normalisedPath);
			return normalisedPath;
		}
		else
		{
			Logger.D(TAG, "DecodeSound: Setting as generated tone");
			File generatedTone = BarcodeFactory.generateTone(decodeVolume, decodeFrequency, decodeDuration);
			if(generatedTone != null) return generatedTone.toURI().toString();
			else return "";
		}
	}

	static
	{
		WinScannerResult.initialiseMatrixEMDK3();
		
	}
}
