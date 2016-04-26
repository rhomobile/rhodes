package com.rho.barcode;

import com.rho.barcode.BarcodeBase;
import com.rho.barcode.IBarcode;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;

/**
 * A Interface Generator from which either an EMDK barcode object (if Zebra Device) or a Zebra Crossing (ZXing)
 * object will be created.
 * @author Ben Kennedy (NCVT73)
 */
public abstract class Barcode extends BarcodeBase implements IBarcode
{
	protected static String LOGTAG = "Barcode";
	protected String scannerId;

	public Barcode(String id)
	{
		super(id);
		this.scannerId = id;
	}

	/**
	 * Recognises a barcode from an image by using the ZXing library
	 * @param imageFilePath The path of the file to be recognised
	 * @author Ben Kennedy (NCVT73)
	 */
	@Override
	public void barcode_recognize(String imageFilePath, IMethodResult result)
	{
		Logger.D(LOGTAG, "Barcode recognize");
		BarcodeSingleton.barcode_recognize(imageFilePath, result);
	}
	
	@Override
	public void registerBluetoothStatus(IMethodResult result)
	{
		Logger.D(LOGTAG, "Barcode registerBluetoothStatus");
		result.setError("Bluetooth Scanners are unsupported on Android");
	}
	
	@Override
	public void commandRemoteScanner(String command, IMethodResult result)
	{
		Logger.D(LOGTAG, "Barcode commandRemoteScanner");
		result.setError("Bluetooth Scanners are not supported on Android");
	}

	public abstract void onResume();
	public abstract void onPause();
	public abstract void onStop();
	public abstract void onDestroy();

	/**
	 * Stores the following parameters in the propertyMap without causing any subclass side effects
	 * @param propertyName
	 * @param propertyValue
	 * @param result
	 */
	public void setPropertySuper(String propertyName, String propertyValue, IMethodResult result)
	{
		super.setProperty(propertyName, propertyValue, result);
	}
	
	/**
	 * Harvests the PropertyBag for decode Sound Parameters
	 * @return
	 */
	protected String getDecodeAudioFilePath()
	{
		//Setup the decode sound
		MethodResult paramCollector = new MethodResult(false);

		getDecodeSound(paramCollector);
		String decodeSound = paramCollector.getString();

		getDecodeDuration(paramCollector);
		int decodeDuration = paramCollector.getInteger();

		getDecodeFrequency(paramCollector);
		int decodeFrequency = paramCollector.getInteger();

		getDecodeVolume(paramCollector);
		int decodeVolume = paramCollector.getInteger();
		
		return EMDKCommon.getDecodeAudioFilePath(decodeSound, decodeDuration, decodeFrequency, decodeVolume);
	}
}