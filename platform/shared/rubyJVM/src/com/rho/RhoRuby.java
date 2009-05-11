package com.rho;

import java.io.InputStream;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import java.io.IOException;
import com.rho.db.DBAdapter;
import com.rho.sync.SyncEngine;
import com.rho.Properties;
//import net.rim.device.api.system.CodeModuleManager;
import com.rho.location.GeoLocation;

public class RhoRuby {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RhoRuby");
	
	public static final RubyID serveID = RubyID.intern("serve_hash");
	public static final RubyID serveIndexID = RubyID.intern("serve_index_hash");
	public static final RubyID getStartPath = RubyID.intern("get_start_path");
	public static final RubyID getOptionsPath = RubyID.intern("get_options_path");
	
	static RubyValue receiver;
	static RubyProgram mainObj;
	
	public static void RhoRubyStart(String szAppPath){
		String[] args = new String[0];

		IRhoRubyHelper helper = null;
        try{

			RubyRuntime.init(args);
	
	        DBAdapter.initMethods(RubyRuntime.DatabaseClass);
	        SyncEngine.initMethods(RubyRuntime.SyncEngineClass);
	        GeoLocation.initMethods(RubyRuntime.GeoLocationClass);
	        
	        helper = RhoClassFactory.createRhoRubyHelper();
	        helper.initRubyExtensions();
        }catch(Throwable exc){
        	LOG.ERROR("Cannot init ruby", exc);
        }
        
        try{
        	//Class mainRuby = Class.forName("xruby.ServeME.main");
        	if ( helper != null ){
	    		mainObj = helper.createMainObject();//new xruby.ServeME.main();//(RubyProgram)mainRuby.newInstance();
	    		receiver = mainObj.invoke();
        	}
        	
        /*}catch(ClassNotFoundException exc){
        	LOG.ERROR("Cannot create ServeME object", exc);*/
        }catch(Throwable exc){
        	LOG.ERROR("Cannot create ServeME object", exc);
        }
	}
	
	public static void RhoRubyStop(){
		
		receiver = null;
		mainObj = null;
		
		com.xruby.runtime.lang.RubyRuntime.fini();	
	}
	
	static public InputStream loadFile(String path){
		return mainObj.getClass().getResourceAsStream(path);		
	}

	public static RubyValue processIndexRequest(String strIndexArg ){
		
		String strIndex = strIndexArg.replace('\\', '/');
		int nAppsIndex = strIndex.indexOf("/apps/");
		if ( nAppsIndex >= 0 ){
			int endIndex = strIndex.indexOf('/', nAppsIndex+6);
			if ( endIndex >= 0 )
				RhoSupport.setCurAppPath( strIndex.substring(nAppsIndex, endIndex+1));
		}
		
		RubyValue value = RubyAPI.callPublicOneArgMethod(receiver, ObjectFactory.createString(strIndex), null, serveIndexID);
		
		return value;
	}

	public static String getStartPage(){
		
		RubyValue value = RubyAPI.callPublicNoArgMethod(receiver, null, getStartPath);
		
		return value.toString();
	}

	public static String getOptionsPage(){
		
		RubyValue value = RubyAPI.callPublicNoArgMethod(receiver, null, getOptionsPath);
		
		return value.toString();
	}
	
	public static RubyValue processRequest(Properties reqHash, Properties reqHeaders, Properties resHeaders )throws IOException{
		RubyHash rh = ObjectFactory.createHash();
		for( int i = 0; i < reqHash.size(); i++ ){
			if ( reqHash.getValueAt(i) != null)
				addStrToHash(rh, reqHash.getKeyAt(i), reqHash.getValueAt(i) );
		}

		RubyHash headers = ObjectFactory.createHash();
		for( int i = 0; i < reqHeaders.size(); i++ ){
			if ( reqHeaders.getValueAt(i) != null){
				String strKey = reqHeaders.getKeyAt(i);
				if ( strKey.equalsIgnoreCase("content-type"))
					strKey = "Content-Type";
				else if ( strKey.equalsIgnoreCase("content-length"))
					strKey = "Content-Length";
				
				addStrToHash(headers, strKey, reqHeaders.getValueAt(i) );
			}
		}
		
		addHashToHash( rh, "headers", headers );
		
		return callFramework(rh);
	}
	
	static RubyValue callFramework(RubyValue hashReq) {
		RubyValue value = RubyAPI.callPublicOneArgMethod(receiver, hashReq, null, serveID);
		
		return value;
	}
	
	public static RubyHash createHash() {
		return ObjectFactory.createHash();
	}

	public static RubyValue addTimeToHash(RubyHash hash, String key, long val) {
		return hash.add( ObjectFactory.createString(key), ObjectFactory.createTime(val) );
	}

	public static RubyValue addIntToHash(RubyHash hash, String key, int val) {
		return hash.add( ObjectFactory.createString(key), ObjectFactory.createInteger(val));
	}

	public static RubyValue addStrToHash(RubyHash hash, String key, String val) {
		return hash.add( ObjectFactory.createString(key), ObjectFactory.createString(val));
	}

	public static RubyValue addHashToHash(RubyHash hash, String key, RubyValue val) {
		return hash.add( ObjectFactory.createString(key), val);	
	}
	
}
