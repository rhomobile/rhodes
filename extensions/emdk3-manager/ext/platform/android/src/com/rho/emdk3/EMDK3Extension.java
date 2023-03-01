package com.rho.emdk3;

import java.util.ArrayList;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.extmanager.AbstractRhoExtension;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.symbol.emdk.EMDKManager;
import com.symbol.emdk.EMDKManager.EMDKListener;
import com.symbol.emdk.EMDKResults;
import com.symbol.emdk.EMDKResults.STATUS_CODE;

/**
 * Obtains and holds a reference to the EMDKManager. Auto refetches if it loses the manager through the EMDK service's
 * onClosed method.
 * @author Ben Kennedy (NCVT73)
 */
public class EMDK3Extension extends AbstractRhoExtension implements EMDKListener
{
	private static final String TAG = "EMDK3Extension";
	private STATUS_CODE statusCode; //TODO
	private EMDKManager emdkManager;
	private boolean isEnding = false;
	private boolean isAquiringEmdkManager = false;
	private ArrayList<IEmdkManagerListener> listenerList;
	
	/**
	 * This should only be called if we are sure that the device is EMDK3 enabled
	 */
	public EMDK3Extension()
	{
		Logger.D(TAG, "+");
		listenerList = new ArrayList<IEmdkManagerListener>();
		Logger.D(TAG, "-");
	}
	
	private void aquireEmdkManager()
	{
		Logger.D(TAG, "aquireEmdkManager+ Context: " + RhoExtManager.getImplementationInstance().getContext());
		try
		{
			statusCode = EMDKManager.getEMDKManager(RhoExtManager.getImplementationInstance().getContext(), this).statusCode;
			if(statusCode != EMDKResults.STATUS_CODE.SUCCESS)
			{
				Logger.E(TAG, "EMDKManager could not be aquired: " + statusCode.name());
			}
			else
			{
				isAquiringEmdkManager = true;
			}
		}
		catch( Exception e )
		{
			statusCode = EMDKResults.STATUS_CODE.FAILURE;
			Logger.E(TAG, "EMDKManager could not be aquired: " + e.getMessage());
		}

		Logger.D(TAG, "aquireEmdkManager- Status Code: " + statusCode);
	}
	
	public void registerForEmdkManagerEvents(IEmdkManagerListener listener)
	{
		Logger.D(TAG, "registerForEmdkManagerEvents");
		
		listenerList.add(listener);
		
		if(emdkManager != null) //onOpened has already been fired
		{
			Logger.D(TAG, "emdkManager is not null, firing onOpened.");
			listener.onOpened(emdkManager);
		}
	}
	
	public void unregisterForEmdkManagerEvents(IEmdkManagerListener listener)
	{
		Logger.D(TAG, "unregisterForEmdkManagerEvents");
		listenerList.remove(listener);
	}
	
	@Override
	public void onOpened(EMDKManager emdkManager)
	{
		Logger.D(TAG, "onOpened+");
		isAquiringEmdkManager = false;
		this.emdkManager = emdkManager;
		
		for(IEmdkManagerListener listener: listenerList)
		{
			listener.onOpened(this.emdkManager);
		}
		Logger.D(TAG, "onOpened-");
	}

	@Override
	public void onClosed()
	{
		Logger.D(TAG, "onClosed+");
		emdkManager = null;
		for(IEmdkManagerListener listener: listenerList)
		{
			listener.onClosed();
		}
		if(!isEnding && !isAquiringEmdkManager)
		{
			aquireEmdkManager();
		}
		Logger.D(TAG, "onClosed-");
	}
	
	public EMDKManager getEmdkManager()
	{
		Logger.D(TAG, "getEmdkManager");
		return emdkManager;
	}
	
	public void setEnding()
	{
		Logger.D(TAG, "setEnding+");
		isEnding = true;
		if(emdkManager != null) emdkManager.release();
		Logger.D(TAG, "setEnding-");
	}

	public void initialize()
	{
		aquireEmdkManager();
	}
}
