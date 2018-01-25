package com.rho.emdk3;

import android.content.Intent;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.util.ContextFactory;

/**
 * @author Ben Kennedy (NCVT73)
 */
public class EMDK3RhoListener extends AbstractRhoListener
{
	private static final String TAG = "EMDK3RhoListener";
	private static EMDK3Extension emdk3Extension = null;
	
	@Override
	public void onCreateApplication(IRhoExtManager extManager)
	{
		Logger.D(TAG, "onCreateApplication+");
		extManager.addRhoListener(this);

		if(isEMDK3Device())
		{
			emdk3Extension = new EMDK3Extension();
			extManager.registerExtension("EMDK3Manager", emdk3Extension);
		}
		
		Logger.D(TAG, "onCreateApplication-");
	}


	@Override
	public void onCreate(RhodesActivity activity, Intent intent)
	{		
		Logger.D(TAG, "onCreate+");
		if(isEMDK3Device())
		{
			emdk3Extension.initialize();
		}
		Logger.D(TAG, "onCreate-");		
	}

	@Override
	public void onDestroy(RhodesActivity activity)
	{		
		Logger.D(TAG, "onDestroy+");
		if(emdk3Extension != null)
		{
			emdk3Extension.setEnding();
			emdk3Extension = null;
		}
		Logger.D(TAG, "onDestroy-");
	}
	
	/**
	 * Initialises the EMDK3Listener if this device is EMDK3 enabled.
	 * @return true if device is EMDK3 enabled, false otherwise.
	 */
	private boolean isEMDK3Device()
	{
		Logger.D(TAG, "initEMDKListener+");
		try
		{
			Class.forName("com.symbol.emdk.barcode.Scanner", false, ContextFactory.getAppContext().getClassLoader());
			Logger.D(TAG, "is EMDK3 Device");
			return true;
		}
		catch (ClassNotFoundException e)
		{
			Logger.D(TAG, "is not EMDK3 Device");
			return false;
		}
	}
}