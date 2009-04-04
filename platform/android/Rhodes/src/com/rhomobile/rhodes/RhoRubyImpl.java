package com.rhomobile.rhodes;

import com.rho.RhoRubyBase;
import com.rho.sync.SyncEngine;
import com.rhomobile.rhodes.camera.Camera;
import com.rhomobile.rhodes.db.DBAdapter;
import com.rhomobile.rhodes.geolocation.GeoLocation;
import com.rhomobile.rhodes.phonebook.RhoPhonebook;
import com.xruby.runtime.lang.RubyRuntime;

public class RhoRubyImpl extends RhoRubyBase{

	@Override
	public void PlatformRhoRubyStart(String szAppPath) {
		DBAdapter.initMethods(RubyRuntime.DBAdapterClass);
		SyncEngine.initMethods(RubyRuntime.SyncEngineClass);
		
		RhoPhonebook.initMethods(RubyRuntime.PhonebookClass);
		GeoLocation.initMethods(RubyRuntime.GeoLocationClass);
		Camera.initMethods(RubyRuntime.CameraClass);
        WebView.initMethods(RubyRuntime.WebViewClass);
	}

}
