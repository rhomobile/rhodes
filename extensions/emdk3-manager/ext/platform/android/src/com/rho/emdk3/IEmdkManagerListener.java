package com.rho.emdk3;

import com.symbol.emdk.EMDKManager;

public interface IEmdkManagerListener
{
	public void onOpened(EMDKManager emdkManager);
	public void onClosed();
}
