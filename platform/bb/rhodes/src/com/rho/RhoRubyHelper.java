package com.rho;

import rhomobile.RhoPhonebook;
import rhomobile.WebView;
import rhomobile.Alert;
import rhomobile.camera.Camera;
import rhomobile.datetime.DateTimePicker;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyProgram;
import com.xruby.runtime.lang.RubyRuntime;

//import net.rim.device.api.system.CoverageInfo;
//import net.rim.device.api.system.RadioInfo;
import net.rim.device.api.system.DeviceInfo;
import java.util.Hashtable;
import net.rim.device.api.system.CodeModuleGroup;
import net.rim.device.api.system.CodeModuleGroupManager;
import net.rim.device.api.system.ApplicationDescriptor;

public class RhoRubyHelper implements IRhoRubyHelper {

	public static final int COVERAGE_BIS_B = 4;
	  
	public void initRubyExtensions(){
        RhoPhonebook.initMethods(RubyRuntime.PhonebookClass);
        Camera.initMethods(RubyRuntime.CameraClass);
        WebView.initMethods(RubyRuntime.WebViewClass);
        Alert.initMethods(RubyRuntime.AlertClass);        
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
	
	static Hashtable m_appProperties = new Hashtable(); 
	public String getAppProperty(String name)
	{
		String strRes = null;
		synchronized (m_appProperties)
		{
			if ( m_appProperties.containsKey(name) )
				strRes = (String)m_appProperties.get(name);
			else	
			{
				CodeModuleGroup[] allGroups = CodeModuleGroupManager.loadAll();
				if ( allGroups != null )
				{
					String moduleName = ApplicationDescriptor
					   .currentApplicationDescriptor().getModuleName();
	
					CodeModuleGroup myGroup = null;
					for (int i = 0; i < allGroups.length; i++) {
					   if (allGroups[i].containsModule(moduleName)) {
					      myGroup = allGroups[i];
					      break;
					   	 }
					}
	
					if ( myGroup != null )
						strRes = myGroup.getProperty(name);
				}
				
				if ( strRes == null )
					strRes = "";
				
				m_appProperties.put(name,strRes);
			}
		}
		
		return strRes;
	}
}
