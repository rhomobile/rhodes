package com.rhomobile.rhodes.geolocation;

import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyNoArgMethod;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.builtin.ObjectFactory;

public class GeoLocation extends RubyBasic {

	private static GeoLocationImpl locImpl = new GeoLocationImpl();
	
	GeoLocation(RubyClass c) {
		super(c);
	}
	
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
		if (locImpl != null)
		  return locImpl.GetLatitude();
		
		return 0.0;
	}
	
	public static double GetLongitude(){
		if (locImpl != null)
		  return locImpl.GetLongitude();
		
		return 0.0;
	}
	
	public static boolean isKnownPosition(){
		return locImpl != null && locImpl.isKnownPosition();
	}
}

