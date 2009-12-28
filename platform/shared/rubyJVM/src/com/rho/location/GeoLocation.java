package com.rho.location;

//import rhomobile.RhoPhonebook;
//import com.rho.sync.SyncEngine;

import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyNoArgMethod;
//import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.builtin.ObjectFactory;
import com.rho.RhoClassFactory;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

public class GeoLocation extends RubyBasic {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Common GeoLocation");
	
	private static IGeoLocationImpl m_locImpl = null;
	
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
	
	private static void startSelf() {
		try {
			GeoLocation.start();
		} catch (Exception e) {
			LOG.ERROR("GeoLocation failed to start", e);
		}
	}
	
	public static String GetLocation() {
		double latitude = GetLatitude();
		double longitude = GetLongitude();
		return Math.abs(latitude) + "° " + (latitude < 0 ? "South" : "North") + " " +
			Math.abs(longitude) + "° " + (longitude < 0 ? "West" : "East") +
			";" + latitude + ";" + longitude;
	}
	
	public static double GetLatitude(){
		LOG.TRACE("GeoLocation.GetLatitude");
		
		startSelf();
		
		if (m_locImpl != null)
		  return m_locImpl.GetLatitude();
		
		return 0.0;
	}
	
	public static double GetLongitude(){
		LOG.TRACE("GeoLocation.GetLongitude");
		startSelf();
		
		if (m_locImpl != null)
		  return m_locImpl.GetLongitude();
		
		return 0.0;
	}
	
	public static boolean isStarted(){
		return m_locImpl != null && m_locImpl.isStarted();
	}

	public static boolean isKnownPosition(){
		LOG.TRACE("GeoLocation.isKnownPosition");
		startSelf();
		return m_locImpl != null && m_locImpl.isKnownPosition();
	}
	
	public static void start() throws Exception {
		LOG.TRACE("GeoLocation.start");
		if (m_locImpl == null)
			m_locImpl = RhoClassFactory.createGeoLocationImpl();
	}

	public static void wakeUp() {
		LOG.TRACE("GeoLocation.wakeUp");
		if (m_locImpl != null)
			m_locImpl.wakeUp();
	}
	
	public static void stop() {
		LOG.TRACE("GeoLocation.stop");
		if ( m_locImpl != null ){
			m_locImpl.quit();
			m_locImpl = null;
		}
	}
	
}
