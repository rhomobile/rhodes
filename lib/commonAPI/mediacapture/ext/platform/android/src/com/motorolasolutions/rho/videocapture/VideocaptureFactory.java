package com.motorolasolutions.rho.videocapture;

import android.app.Dialog;
import android.content.Intent;
import android.content.res.Configuration;
import com.rho.videocapture.IVideocaptureFactory;
import com.rho.videocapture.VideocaptureFactorySingleton;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.RhoApiFactory;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import android.view.KeyEvent;

public class VideocaptureFactory extends RhoApiFactory< Videocapture, VideocaptureSingleton> implements IVideocaptureFactory, IRhoListener {

	public static Videocapture default_instance; //To invoke the onActivityResult() on the default video captur eobject
	
	@Override
	protected VideocaptureSingleton createSingleton()
	{		
		return new VideocaptureSingleton(this);
	}

	@Override
	protected Videocapture createApiObject(String id)
	{		
		default_instance = new Videocapture(id);
		return default_instance;
	}

	@Override
	public void onCreateApplication(IRhoExtManager extManager)
	{
		extManager.addRhoListener(this);
	}
	
	@Override
	public void onActivityResult(RhodesActivity activity, int requestCode, int resultCode, Intent intent)
	{
	    if (default_instance != null) {
	        default_instance.onActivityResultVideoCapture(activity, requestCode, resultCode, intent);
	    }
	}
	
	@Override
	public void onCreate(RhodesActivity activity, Intent intent)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onStart(RhodesActivity activity)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onResume(RhodesActivity activity)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onPause(RhodesActivity activity)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onStop(RhodesActivity activity)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onDestroy(RhodesActivity activity)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onNewIntent(RhodesActivity activity, Intent intent)
	{
		// TODO Auto-generated method stub
		
	}	

	@Override
	public Dialog onCreateDialog(RhodesActivity activity, int id)
	{
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void onConfigurationChanged(RhodesActivity activity, Configuration newConfig)
	{
		// TODO Auto-generated method stub
		
	}	

	@Override
	public boolean onKey(int keyCode, KeyEvent event){ return false; }
}
