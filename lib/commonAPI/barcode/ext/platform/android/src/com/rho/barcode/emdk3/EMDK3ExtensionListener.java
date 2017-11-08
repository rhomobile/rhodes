package com.rho.barcode.emdk3;

import com.rho.barcode.BarcodeFactory;
import com.rho.emdk3.EMDK3Extension;
import com.rho.emdk3.IEmdkManagerListener;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.IRhoExtension;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.symbol.emdk.EMDKManager;
import com.symbol.emdk.EMDKManager.FEATURE_TYPE;
import com.symbol.emdk.barcode.BarcodeManager;

public class EMDK3ExtensionListener implements IEmdkManagerListener
{
	private static final String TAG = "EMDK3ExtensionListener";
	private BarcodeFactory barcodeFactory;
	private EMDK3Extension emdkExtension = null; 
	private EMDKManager emdkManager;
	private BarcodeManager barcodeManager;

	public EMDK3ExtensionListener(BarcodeFactory barcodeFactory)
	{
		Logger.D(TAG, "+");
		this.barcodeFactory = barcodeFactory;
		IRhoExtension emdkExtensionInterface = RhoExtManager.getImplementationInstance().getExtByName("EMDK3Manager");
		if(emdkExtensionInterface != null)
		{
			Logger.D(TAG, "Emdk-manager extension found. This is an EMDK 3 device");
			emdkExtension = (EMDK3Extension) emdkExtensionInterface;
			emdkExtension.registerForEmdkManagerEvents(this);
		}
		else
		{
			Logger.D(TAG, "Cannot find EMDK-manager extension");
		}
		Logger.D(TAG, "-");
	}

	@Override
	public void onOpened(EMDKManager emdkManager)
	{
		Logger.D(TAG, "onOpened");
		open( emdkManager );		
	}
	
	//TODO is onClosed called when you try to open but it fails?
	@Override
	public void onClosed()
	{
		Logger.D(TAG, "onClosed+");
		if(emdkManager != null) emdkManager.release();
		this.emdkManager = null;
		this.barcodeManager = null;
		//TODO what happens to all of the settings when the EMDKManager is closed??
	}
	
	public EMDKManager getEMDKManager()
	{
		Logger.D(TAG, "getEMDKManager");
		//TODO we need some way to wait for EMDKManager to become available;
		return emdkManager;
	}
	
	public BarcodeManager getBarcodeManager()
	{
		Logger.D(TAG, "getBarcodeManager");
		return barcodeManager;
	}
	
	public void destroy()
	{
		Logger.D(TAG, "destroy");
		if(emdkExtension != null)
		{
			emdkExtension.unregisterForEmdkManagerEvents(this);
			emdkExtension = null;
		}
		onClosed();
	}	

	public void pause()
	{
		if ( emdkManager != null )
		{
			emdkManager.release(FEATURE_TYPE.BARCODE);
		}
		barcodeManager = null;
	}

	public void resume()
	{
		if ( emdkManager != null )
		{
			open( emdkManager );			
		}
	}

	private void open( EMDKManager manager )
	{
		this.emdkManager = manager;
		this.barcodeManager = (BarcodeManager) emdkManager.getInstance(FEATURE_TYPE.BARCODE);
		
		if(!barcodeFactory.isEMDKScannerSetupCalled())
		{
			Logger.D(TAG, "EMDK3Extension.open EMDKScannerSetup has not yet been called");
			barcodeFactory.setIsEMDKScannerSetupCalled(true);
			barcodeFactory.setupEmdkScannerIds();
		}		
	}
}
