package com.rho.connectionchecking;

import android.content.Intent;



import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.Logger;

public class ConnectionCheckingListener extends AbstractRhoListener {
	
	private static String TAG = "ConnectionCheckingListener";
	
	@Override
	public void onCreateApplication(IRhoExtManager extManager) {
		
		extManager.addRhoListener(this);
		ConnectionPageNavEvent ext=new ConnectionPageNavEvent();
		RhoExtManager.getInstance().registerExtension("ConnectionCheckingService", ext);
		
	
	}
	
	@Override
	public void onEBLicenseDestroyed() {
		
		Logger.I(TAG, "onEBLicenseDestroyed Recieved");
		if(!ConnectionPageNavEvent.isCurLinkBadLink() && ConnectionCheckingService.getInstance()!=null)
			ConnectionCheckingService.getInstance().restartThread();
		
		super.onEBLicenseDestroyed();
	}
	@Override
	public void onEBLicenseHidden() {
		Logger.I(TAG, "onEBLicenseHidden Recieved");
		if(!ConnectionPageNavEvent.isCurLinkBadLink() && ConnectionCheckingService.getInstance()!=null)
			ConnectionCheckingService.getInstance().restartThread();
		super.onEBLicenseHidden();
	}
	@Override
	public void onEBLicenseVisible() {
		Logger.I(TAG, "onEBLicenseVisible Recieved");
		if(ConnectionCheckingService.getInstance()!=null)
			ConnectionCheckingService.getInstance().handleLicenseCreate();
		super.onEBLicenseVisible();
	}
	

}
