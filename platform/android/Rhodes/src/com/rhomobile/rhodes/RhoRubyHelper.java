package com.rhomobile.rhodes;

import android.content.Context;
import android.telephony.*; 

import com.rho.IRhoRubyHelper;
import com.rhomobile.rhodes.camera.Camera;
import com.rhomobile.rhodes.datetime.DateTimePicker;
import com.rhomobile.rhodes.phonebook.RhoPhonebook;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
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
		DateTimePicker.initMethods(RubyRuntime.DateTimePickerClass);
	}

	public String getPlatform() {
		return "Android";
	}

	public void loadBackTrace(RubyArray backtrace) {
		try {
			if ( backtrace != null )
				backtrace.clear();
			else
				return;
			
			StackTraceElement[] trace = Thread.currentThread().getStackTrace();
			for (int m = 0; m < trace.length; m++) {
				StackTraceElement e = trace[m];
				String s = e.getClassName();

				if (s.startsWith("xruby.")) {
					String fileName = e.getFileName();
					String funcName = null;

					String[] classParts = s.split("\\$");
					for (int i = 0; i < classParts.length && funcName == null; i++) {
						String[] subPaths = classParts[i].split("\\.");

						for (int j = 0; j < subPaths.length && funcName == null; j++) {
							if (fileName.indexOf(subPaths[j]) != -1) {
								if (j + 1 < subPaths.length)
									funcName = subPaths[j + 1];
								else
									funcName = subPaths[j];
							}
						}
					}

					fileName = fileName.replaceAll("\\\\", "/");
					fileName = fileName.replace("RhoBundle", "");

					String traceStr = "at " + fileName + ":"
							+ e.getLineNumber() + ": in `" + funcName + "`\r\n";
					backtrace.add(ObjectFactory.createString(traceStr));
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public String getDeviceId() {

        TelephonyManager mTelephonyMgr = 
            (TelephonyManager) RhodesInstance.getInstance().getSystemService(Context.TELEPHONY_SERVICE);
        
		return mTelephonyMgr.getDeviceId();
	}

	public boolean hasNetwork() {
		return RhodesInstance.getInstance().isNetworkAvailable();
	}

}
