package com.rhomobile.rhodes;

import android.content.Intent;


public interface RhodesActivityListener {
	
	void onCreate(RhodesActivity activity, Intent intent);
	void onPause(RhodesActivity activity);
	void onResume(RhodesActivity activity);
	void onNewIntent(RhodesActivity activity, Intent intent);
	
	void onRhodesActivityStartup(RhodesActivity activity);
	
}