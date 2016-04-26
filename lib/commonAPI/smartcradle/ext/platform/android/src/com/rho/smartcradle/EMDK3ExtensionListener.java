package com.rho.smartcradle;

import com.rho.emdk3.EMDK3Extension;
import com.rho.emdk3.IEmdkManagerListener;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.IRhoExtension;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.symbol.emdk.EMDKManager;

public class EMDK3ExtensionListener implements IEmdkManagerListener{
	
	private static final String TAG = "EMDK3ExtensionListener";
	
	private EMDK3Extension emdkExtension = null; 
	private EMDKManager emdkManager;
	
	
	public EMDK3ExtensionListener()
	{
		Logger.D(TAG, "+");
		//this.mobilePaymentFactory = mobilePaymentFactory;
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
	public void onOpened(EMDKManager emdkManager) {
		Logger.D(TAG, "onOpened");
		this.emdkManager = emdkManager;
		
	}

	@Override
	public void onClosed() {
		Logger.D(TAG, "onClosed+");
		if(emdkManager != null) emdkManager.release();
		this.emdkManager = null;
		
	}
	public EMDKManager getEMDKManager()
	{
		Logger.D(TAG, "getEMDKManager");
		
		return emdkManager;
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

}
