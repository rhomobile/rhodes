package com.rho;

import java.util.Hashtable;

import net.rim.device.api.system.ApplicationDescriptor;
import net.rim.device.api.system.CodeModuleGroup;
import net.rim.device.api.system.DeviceInfo;
import rhomobile.Alert;
import rhomobile.NativeBar;
import rhomobile.RhoPhonebook;
import rhomobile.RhodesApplication;
import rhomobile.RingtoneManager;
import rhomobile.WebView;
import rhomobile.camera.Camera;
import rhomobile.datetime.DateTimePicker;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyProgram;
import com.xruby.runtime.lang.RubyRuntime;

public class RhoRubyHelper implements IRhoRubyHelper {

	public static final int COVERAGE_BIS_B = 4;
	  
	public void initRubyExtensions(){
        RhoPhonebook.initMethods(RubyRuntime.PhonebookClass);
        Camera.initMethods(RubyRuntime.CameraClass);
        WebView.initMethods(RubyRuntime.WebViewClass);
        RhoConf.initMethods(RubyRuntime.RhoConfClass);
        Alert.initMethods(RubyRuntime.AlertClass);        
        DateTimePicker.initMethods(RubyRuntime.DateTimePickerClass);
        RingtoneManager.initMethods(RubyRuntime.RingtoneManagerClass);
        NativeBar.initMethods(RubyRuntime.NativeBarClass);
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

	public boolean isSimulator(){
		return DeviceInfo.isSimulator();
    }

	public String getModuleName()
	{
		return ApplicationDescriptor.currentApplicationDescriptor().getModuleName();
	}
	
	public void showLog()
	{
		synchronized ( RhodesApplication.getEventLock() ) {		
			RhodesApplication.getInstance().showLogScreen();
		}
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
				CodeModuleGroup[] codeModule = CodeModuleGroup.loadAll();

				if ( codeModule != null )
				{
					String moduleName = ApplicationDescriptor
					   .currentApplicationDescriptor().getModuleName();
					
					for(int i = 0; i < codeModule.length; i++) 
					{
						String module = codeModule[i].getName();
						if( module.indexOf( moduleName ) != -1)
						{
							CodeModuleGroup group = codeModule[i];
							if ( group != null )
								strRes = group.getProperty(name);
							break;
						}
					}
				}
/*				
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
				} */
				
				if ( strRes == null )
					strRes = "";
				
				m_appProperties.put(name,strRes);
			}
		}
		
		return strRes;
	}
}
