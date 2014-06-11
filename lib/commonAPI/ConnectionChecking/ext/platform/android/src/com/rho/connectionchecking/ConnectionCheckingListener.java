package com.rho.connectionchecking;

import android.content.Intent;



import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.RhoExtManager;


public class ConnectionCheckingListener extends AbstractRhoListener {
	
	private static String TAG = "ConnectionCheckingListener";
	
	@Override
	public void onCreateApplication(IRhoExtManager extManager) {
		
		extManager.addRhoListener(this);
		ConnectionPageNavEvent ext=new ConnectionPageNavEvent();
		RhoExtManager.getInstance().registerExtension("ConnectionCheckingService", ext);
		
	
	}
	
	@Override
	public void onCreate(RhodesActivity activity, Intent intent) {
		
		super.onCreate(activity, intent);
		
	}

}
