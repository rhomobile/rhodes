package com.rho;

import rhomobile.RhoPhonebook;
import rhomobile.WebView;
import rhomobile.camera.Camera;
import rhomobile.datetime.DateTimePicker;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyProgram;
import com.xruby.runtime.lang.RubyRuntime;

//import net.rim.device.api.system.CoverageInfo;
//import net.rim.device.api.system.RadioInfo;
import net.rim.device.api.system.DeviceInfo;

public class RhoRubyHelper implements IRhoRubyHelper {

	public static final int COVERAGE_BIS_B = 4;
	  
	public void initRubyExtensions(){
        RhoPhonebook.initMethods(RubyRuntime.PhonebookClass);
        Camera.initMethods(RubyRuntime.CameraClass);
        WebView.initMethods(RubyRuntime.WebViewClass);
        DateTimePicker.initMethods(RubyRuntime.DateTimePickerClass);
	}
	
	public RubyProgram createMainObject() {
		return new xruby.ServeME.main();
	}

	public String getPlatform() {
		return "Blackberry";
	}

	public void loadBackTrace(RubyArray backtrace) {
		//TODO:
	}

	public boolean hasNetwork() {
		/*if ((RadioInfo.getActiveWAFs() & RadioInfo.WAF_WLAN) != 0) {
			if (CoverageInfo.isCoverageSufficient( CoverageInfo.COVERAGE_CARRIER,RadioInfo.WAF_WLAN, false) || 
					CoverageInfo.isCoverageSufficient( CoverageInfo.COVERAGE_MDS,RadioInfo.WAF_WLAN, false) ||
					CoverageInfo.isCoverageSufficient( COVERAGE_BIS_B,RadioInfo.WAF_WLAN, false))
				return true;
		}

		if (CoverageInfo.isOutOfCoverage())
	        return false; 
		*/
		return true;
	}

	public String getDeviceId(){
		return new Integer( DeviceInfo.getDeviceId() ).toString();
      }
}
