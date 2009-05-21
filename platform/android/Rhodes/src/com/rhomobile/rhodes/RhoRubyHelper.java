package com.rhomobile.rhodes;

import com.rho.IRhoRubyHelper;
import com.rhomobile.rhodes.camera.Camera;
import com.rhomobile.rhodes.phonebook.RhoPhonebook;
import com.xruby.runtime.lang.RubyProgram;
import com.xruby.runtime.lang.RubyRuntime;

public class RhoRubyHelper implements IRhoRubyHelper {

	public RubyProgram createMainObject() {
		return new xruby.ServeME.main();
	}

	public void initRubyExtensions() {
		RhoPhonebook.initMethods(RubyRuntime.PhonebookClass);
		Camera.initMethods(RubyRuntime.CameraClass);
        WebView.initMethods(RubyRuntime.WebViewClass);
	}

	public String getPlatform() {
		return "Android";
	}


}
