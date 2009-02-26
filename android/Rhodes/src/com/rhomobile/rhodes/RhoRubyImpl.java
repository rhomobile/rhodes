package com.rhomobile.rhodes;

import com.rho.RhoRubyBase;
import com.rhomobile.rhodes.geolocation.GeoLocation;
import com.rhomobile.rhodes.phonebook.RhoPhonebook;
import com.xruby.runtime.lang.RubyRuntime;

public class RhoRubyImpl extends RhoRubyBase{

	@Override
	public void PlatformRhoRubyStart(String szAppPath) {
		RhoPhonebook.initMethods(RubyRuntime.PhonebookClass);
		GeoLocation.initMethods(RubyRuntime.GeoLocationClass);
	}

}
