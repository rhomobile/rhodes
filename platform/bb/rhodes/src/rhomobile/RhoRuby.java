package rhomobile;

import java.io.InputStream;

import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import java.io.IOException;
//import java.io.ByteArrayInputStream;
//import com.rho.sync.SyncEngine; 
import rhomobile.db.PerstLiteAdapter;
import rhomobile.sync.SyncEngine;
import rhomobile.location.GeoLocation;

public class RhoRuby {

	public static final RubyID serveID = RubyID.intern("serve_hash");
	public static final RubyID serveIndexID = RubyID.intern("serve_index_hash");
	public static final RubyID getStartPath = RubyID.intern("get_start_path");
	
	static RubyValue receiver;
	static xruby.ServeME.main mainObj;
	
	public static void RhoRubyStart(String szAppPath){
		String[] args = new String[0];
		
		RubyRuntime.init(args);

        PerstLiteAdapter.initMethods(RubyRuntime.DBAdapterClass);
        SyncEngine.initMethods(RubyRuntime.SyncEngineClass);
        RhoPhonebook.initMethods(RubyRuntime.PhonebookClass);
        GeoLocation.initMethods(RubyRuntime.GeoLocationClass);
        
		mainObj = new xruby.ServeME.main();
		receiver = mainObj.invoke();
	}
	
	public static void RhoRubyStop(){
		
		receiver = null;
		mainObj = null;
		
		com.xruby.runtime.lang.RubyRuntime.fini();	
	}
	
	static public InputStream loadFile(String path){
		return mainObj.getClass().getResourceAsStream(path);		
	}

	public static RubyValue processIndexRequest(String strIndex ){
		
		RubyValue value = RubyAPI.callPublicOneArgMethod(receiver, ObjectFactory.createString(strIndex), null, serveIndexID);
		
		return value;
	}

	public static String getStartPage(){
		
		RubyValue value = RubyAPI.callPublicNoArgMethod(receiver, null, getStartPath);
		
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
