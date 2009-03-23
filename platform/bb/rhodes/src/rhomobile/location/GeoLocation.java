package rhomobile.location;

import rhomobile.RhoPhonebook;
import rhomobile.sync.SyncEngine;

import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyNoArgMethod;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.builtin.ObjectFactory;

public class GeoLocation extends RubyBasic {

	private static GeoLocationImpl m_locImpl = null;
	
	GeoLocation(RubyClass c) {
		super(c);
	}
/*
	public GeoLocation initialize() {
		return this;
	}
	
	public static GeoLocation alloc(RubyValue receiver) {
		return new GeoLocation(RubyRuntime.GeoLocationClass);
	}*/
	
	public static RubyValue GetLatitudeValue() {
		return ObjectFactory.createFloat(GetLatitude());
	}
	public static RubyValue GetLongitudeValue() {
		return ObjectFactory.createFloat(GetLongitude());
	}
	public static RubyValue isKnownPositionValue() {
		return ObjectFactory.createBoolean(isKnownPosition());
	}
	
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("latitude", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return GeoLocation.GetLatitudeValue();
			}
		});
		klass.getSingletonClass().defineMethod("longitude", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return GeoLocation.GetLongitudeValue();
			}
		});
		klass.getSingletonClass().defineMethod("known_position?", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return GeoLocation.isKnownPositionValue();
			}
		});
		
	}
	
	public static double GetLatitude(){
		if (m_locImpl != null)
		  return m_locImpl.GetLatitude();
		
		return 0.0;
	}
	
	public static double GetLongitude(){
		if (m_locImpl != null)
		  return m_locImpl.GetLongitude();
		
		return 0.0;
	}
	
	public static boolean isStarted(){
		return m_locImpl != null && m_locImpl.isStarted();
	}

	public static String getLog(){
		if (m_locImpl != null)
			return m_locImpl.getLog();
		
		return "";
	}
	
	public static boolean isKnownPosition(){
		return m_locImpl != null && m_locImpl.isKnownPosition();
	}
	
	public static void start() {
		if (m_locImpl == null)
			m_locImpl = new GeoLocationImpl();
	}

	public static void wakeUp() {
		if (m_locImpl != null)
			m_locImpl.wakeUp();
	}
	
	public static void stop() {
		if ( m_locImpl != null ){
			m_locImpl.quit();
			m_locImpl = null;
		}
	}
	
}
