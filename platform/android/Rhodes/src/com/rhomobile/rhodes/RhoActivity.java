package com.rhomobile.rhodes;

import android.app.Activity;

public class RhoActivity extends Activity {

	@Override
	protected void onStart() {
		super.onStart();
		if (RhodesService.getInstance() != null) {
			RhodesService.getInstance().activityStarted();
		}
	}
	
	@Override
	protected void onStop() {
		RhodesService.getInstance().activityStopped();
		super.onStop();
	}
}
