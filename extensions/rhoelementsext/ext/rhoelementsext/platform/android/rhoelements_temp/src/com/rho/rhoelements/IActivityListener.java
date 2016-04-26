package com.rho.rhoelements;

import com.rhomobile.rhodes.extmanager.IRhoWebView;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public interface IActivityListener
{
	public void onCreate(Activity activity, View mainView, Bundle savedInstanceState);
	public void onStart(Activity activity);
	public void onResume(Activity activity);
	public void onPause(Activity activity);
	public void onStop(Activity activity);
	public void onDestroy(Activity activity);
	
	/**
	 * @param intent
	 * @param updatedIntentId Instruct whether updating the internal reference to the current intent or not
	 */
	public void onNewIntent(Intent intent, boolean updateIntentId);
}
