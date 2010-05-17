package com.rho.rubyext;

import rhomobile.RhodesApplication;
import net.rim.blackberry.api.phone.Phone;
import net.rim.device.api.i18n.Locale;
import net.rim.device.api.system.Display;

import com.rho.BBVersionSpecific;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhodesApp;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.lang.*;
import com.rho.RhoRubyHelper;
import net.rim.device.api.system.DeviceInfo;

public class System {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("System");
	
	public static void initMethods(RubyClass klass){
		klass.getSingletonClass().defineMethod( "get_property", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
				try {
					return get_property(arg);
				} catch(Exception e) {
					LOG.ERROR("get_property failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod( "has_network", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
				try {
					return ObjectFactory.createBoolean(hasNetwork());
				} catch(Exception e) {
					LOG.ERROR("has_network failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod( "get_locale", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
				try {
					return ObjectFactory.createString(getLocale());
				} catch(Exception e) {
					LOG.ERROR("get_locale failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod( "get_screen_width", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) 
			{
				try {
					return ObjectFactory.createInteger(getScreenWidth());
				} catch(Exception e) {
					LOG.ERROR("get_screen_width failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod( "get_screen_height", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) 
			{
				try {
					return ObjectFactory.createInteger(getScreenHeight());
				} catch(Exception e) {
					LOG.ERROR("get_screen_height failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod( "set_push_notification", new RubyTwoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block )
			{
				try {
					String url = arg1 != RubyConstant.QNIL ? arg1.toStr() : "";
					String params = arg2 != RubyConstant.QNIL ? arg2.toStr() : "";
					
					RhodesApp.getInstance().setPushNotification(url, params);
					
					return RubyConstant.QNIL;
				} catch(Exception e) {
					LOG.ERROR("set_push_notification failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod( "set_screen_rotation_notification", new RubyTwoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block )
			{
				try {
					String url = arg1 != RubyConstant.QNIL ? arg1.toStr() : "";
					String params = arg2 != RubyConstant.QNIL ? arg2.toStr() : "";
					
					RhodesApp.getInstance().setScreenRotationNotification(url, params);
					
					return RubyConstant.QNIL;
				} catch(Exception e) {
					LOG.ERROR("set_screen_rotation_notification failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod("exit", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				RhodesApplication.getInstance().close();
				return RubyConstant.QNIL;
			}
		});
	}
    
    //@RubyLevelMethod(name="get_property", module=true)
    private static RubyValue get_property(RubyValue arg) {
    	String strPropName = arg.toStr();
    	if ( strPropName.equalsIgnoreCase("platform") )
    	{
    		RhoRubyHelper helper = new RhoRubyHelper();
    		return ObjectFactory.createString( helper.getPlatform() );
    	}
    	if ( strPropName.equalsIgnoreCase("has_network") )
    		return ObjectFactory.createBoolean(hasNetwork()); 
    	if ( strPropName.equalsIgnoreCase("locale") )
    		return ObjectFactory.createString(getLocale());
    	if ( strPropName.equalsIgnoreCase("country") )
    		return ObjectFactory.createString(getCountry());
    	if ( strPropName.equalsIgnoreCase("screen_width") )
    		return ObjectFactory.createInteger(getScreenWidth()); 
    	if ( strPropName.equalsIgnoreCase("screen_height") )
    		return ObjectFactory.createInteger(getScreenHeight());
    	if ( strPropName.equalsIgnoreCase("ppi_x"))
    		return ObjectFactory.createFloat(getScreenPpiX());
    	if ( strPropName.equalsIgnoreCase("ppi_y"))
    		return ObjectFactory.createFloat(getScreenPpiY());
    	if ( strPropName.equalsIgnoreCase("has_camera") )
    		return ObjectFactory.createBoolean(hasCamera()); 
    	if ( strPropName.equalsIgnoreCase("phone_number") )
    		return ObjectFactory.createString(Phone.getDevicePhoneNumber(true)); 
    	if ( strPropName.equalsIgnoreCase("device_id") )
    		return ObjectFactory.createString(new Integer( DeviceInfo.getDeviceId() ).toString()); 
    	if ( strPropName.equalsIgnoreCase("full_browser") )
    		return ObjectFactory.createBoolean(rhomobile.RhodesApplication.isFullBrowser());
    	if ( strPropName.equalsIgnoreCase("device_name") )
    		return ObjectFactory.createString(DeviceInfo.getDeviceName());
    	if ( strPropName.equalsIgnoreCase("os_version") )
    		return ObjectFactory.createString(DeviceInfo.getSoftwareVersion());
    	
    	return RubyConstant.QNIL;
    }

	public static String getLocale()
	{
    	Locale loc = Locale.getDefault();
    	
    	String lang = loc != null ? loc.getLanguage() : "en";
		return lang;
	}
	
	private static String getCountry() {
		Locale loc = Locale.getDefault();
		String country = loc != null ? loc.getCountry() : "US";
		return country;
	}
	
	public static boolean hasCamera() 
	{
		return DeviceInfo.hasCamera();
	}
	
	public static boolean hasNetwork() {
		/*if ((RadioInfo.getActiveWAFs() & RadioInfo.WAF_WLAN) != 0) {
			if (CoverageInfo.isCoverageSufficient( CoverageInfo.COVERAGE_CARRIER,RadioInfo.WAF_WLAN, false) || 
					CoverageInfo.isCoverageSufficient( CoverageInfo.COVERAGE_MDS,RadioInfo.WAF_WLAN, false) ||
					CoverageInfo.isCoverageSufficient( COVERAGE_BIS_B,RadioInfo.WAF_WLAN, false))
				return true;
		}

		if (CoverageInfo.isOutOfCoverage())
	        return false; 
		*/
		int nStatus = net.rim.device.api.system.RadioInfo.getNetworkService();
		boolean hasGPRS = ( nStatus & net.rim.device.api.system.RadioInfo.NETWORK_SERVICE_DATA) != 0;
		
		boolean hasWifi = BBVersionSpecific.isWifiActive();
		LOG.INFO("hasGPRS : " + hasGPRS + "; Wifi: " + hasWifi);
		boolean bRes = hasGPRS || hasWifi;
		return bRes;
	}

	public static int getScreenHeight() {
		return Display.getHeight();
	}

	public static int getScreenWidth() {
		return Display.getWidth();
	}
	
	public static double getScreenPpiX() {
		// Convert PPM (Pixels Per Meter) to PPI (Pixels Per Inch)
		int ppm = Display.getHorizontalResolution();
		double retval = (ppm*25.4)/1000;
		return retval;
	}
	
	public static double getScreenPpiY() {
		// Convert PPM (Pixels Per Meter) to PPI (Pixels Per Inch)
		int ppm = Display.getVerticalResolution();
		double retval = (ppm*25.4)/1000;
		return retval;
	}
	
	
}
