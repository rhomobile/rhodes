package com.rho.barcode.emdk3;

import android.util.Log;

import com.rho.barcode.RhoScannerInfo;
import com.rhomobile.rhodes.Logger;
import com.symbol.emdk.barcode.ScannerInfo.ConnectionType;

public class EMDK3ScannerInfo extends RhoScannerInfo
{
	protected static final String TAG = "EMDK3ScannerInfo";
	
	/**
	 * Constructor for EMDK3 scanner
	 * @param scannerInfo
	 */
	public EMDK3ScannerInfo(com.symbol.emdk.barcode.ScannerInfo scannerInfo)
	{
		Logger.D(TAG, "EMDK3ScannerInfo+");
		this.defaultScanner = scannerInfo.isDefaultScanner();
		this.friendlyName = scannerInfo.getFriendlyName();
		this.scannerClass = getRhoScannerClass(scannerInfo.getDeviceType());
		this.isBluetooth = scannerInfo.getConnectionType() == ConnectionType.BLUETOOTH_SSI;
	}
	
	/**
	 * Converts an (EMDK3) DeviceType to a RhoScannerClass.
	 * @param deviceType the DeviceType to convert
	 * @return the equivalent RhoScannerClass
	 * @author Ben Kennedy (NCVT73)
	 */
	private static RhoScannerClass getRhoScannerClass(com.symbol.emdk.barcode.ScannerInfo.DeviceType deviceType)
	{
		Logger.D(TAG, "getRhoScannerClass(EMDK3)");
		try{
		switch(deviceType)
		{
			case IMAGER: return RhoScannerClass.SCANNER_CLASS_IMAGER;
			case LASER: return RhoScannerClass.SCANNER_CLASS_LASER;
			case CAMERA: return RhoScannerClass.SCANNER_CLASS_CAMERA;
			default: return RhoScannerClass.SCANNER_CLASS_UNKNOWN;
		}
		}
		catch(NoClassDefFoundError e)
		{
			Log.e("BENKEN", "catching here:");
			e.printStackTrace();
			throw e;
		}
	}
}
