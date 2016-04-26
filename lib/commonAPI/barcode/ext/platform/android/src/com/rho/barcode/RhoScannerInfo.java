package com.rho.barcode;

import com.rhomobile.rhodes.Logger;

public abstract class RhoScannerInfo
{
	public enum RhoScannerClass
	{
		SCANNER_CLASS_IMAGER, SCANNER_CLASS_LASER, SCANNER_CLASS_CAMERA, SCANNER_CLASS_UNKNOWN
	}
	
	protected static String TAG = "RhoScannerInfo";
	protected boolean defaultScanner;
	protected String friendlyName;
	protected RhoScannerClass scannerClass;
	protected boolean isBluetooth;
	
	public boolean isDefaultScanner()
	{
		Logger.D(TAG, "RhoScannerInfo isDefaultScanner+: " + this.defaultScanner);
		return defaultScanner;
	}

	public String getFriendlyName()
	{
		Logger.D(TAG, "RhoScannerInfo getFriendlyName+: " + this.friendlyName);
		return friendlyName;
	}

	public RhoScannerClass getScannerClass()
	{
		Logger.D(TAG, "RhoScannerInfo getScannerClass+: " + this.scannerClass);
		return scannerClass;
	}	


	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + ((friendlyName == null) ? 0 : friendlyName.hashCode());
		result = prime * result + ((scannerClass == null) ? 0 : scannerClass.hashCode());
//		result = prime * result + ((decoderType == null) ? 0 : decoderType.hashCode());
//		result = prime * result + ((modelNumber == null) ? 0 : modelNumber.hashCode());
//		result = prime * result + ((scannerType == null) ? 0 : scannerType.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		RhoScannerInfo other = (RhoScannerInfo) obj;
		if (friendlyName == null)
		{
			if (other.friendlyName != null)
				return false;
		}
		else if (!friendlyName.equals(other.friendlyName))
			return false;
		if (scannerClass != other.scannerClass)
			return false;
//		if (modelNumber == null)
//		{
//			if (other.modelNumber != null)
//				return false;
//		}
//		else if (!modelNumber.equals(other.modelNumber))
//			return false;
//		if (scannerType != other.scannerType)
//			return false;
//		if (decoderType != other.decoderType)
//		return false;
		return true;
	}

	public boolean isBluetooth()
	{
		return isBluetooth;
	}

//	private DecoderType decoderType;
//	private String modelNumber;
//	private ScannerType scannerType;
	
//	public DecoderType getDecoderType()
//	{
//		return decoderType;
//	}
	
//	public String getModelNumber()
//	{
//		return modelNumber;
//	}
	
//	public ScannerType getScannerType()
//	{
//		return scannerType;
//	}
}
