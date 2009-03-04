package com.rho;

import java.io.InputStream;

import com.rho.db.PerstLiteAdapter;
import com.rho.sync.SyncEngine;
import com.rho.util.Properties;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import java.io.IOException;

public abstract class RhoRubyBase implements IRhoRuby {

	public static final RubyID serveID = RubyID.intern("serve_hash");
	public static final RubyID serveIndexID = RubyID.intern("serve_index_hash");
	public static final RubyID getStartPath = RubyID.intern("get_start_path");
	public static final RubyID getOptionsPath = RubyID.intern("get_options_path");
	
	RubyValue receiver;
	xruby.ServeME.main mainObj;
	
	/* (non-Javadoc)
	 * @see com.rho.IRhoRuby#RhoRubyStart(java.lang.String)
	 */
	public void RhoRubyStart(String szAppPath){
		String[] args = new String[0];
		
		RubyRuntime.init(args);

		PerstLiteAdapter.initMethods(RubyRuntime.DBAdapterClass);
		SyncEngine.initMethods(RubyRuntime.SyncEngineClass);
		
		PlatformRhoRubyStart(szAppPath);
        
		mainObj = new xruby.ServeME.main();
				
		receiver = mainObj.invoke();
	}
	
	public abstract void PlatformRhoRubyStart(String szAppPath);
	
	/* (non-Javadoc)
	 * @see com.rho.IRhoRuby#RhoRubyStop()
	 */
	public void RhoRubyStop(){
		
		receiver = null;
		mainObj = null;
		
		com.xruby.runtime.lang.RubyRuntime.fini();	
	}
	
	/* (non-Javadoc)
	 * @see com.rho.IRhoRuby#loadFile(java.lang.String)
	 */
	public InputStream loadFile(String path){
		if ( RubyPlatformUtils.getPlatform().equalsIgnoreCase("android") )
			return null; //use assets instead of jar
		else
			return mainObj.getClass().getResourceAsStream(path);		
	}

	/* (non-Javadoc)
	 * @see com.rho.IRhoRuby#processIndexRequest(java.lang.String)
	 */
	public RubyValue processIndexRequest(String strIndexArg ){
		
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

	/* (non-Javadoc)
	 * @see com.rho.IRhoRuby#getStartPage()
	 */
	public String getStartPage(){
		
		RubyValue value = RubyAPI.callPublicNoArgMethod(receiver, null, getStartPath);
		
		return value.toString();
	}

        /* (non-Javadoc)
	 * @see com.rho.IRhoRuby#getOptionsPage()
	 */
        public String getOptionsPage(){
		
		RubyValue value = RubyAPI.callPublicNoArgMethod(receiver, null, getOptionsPath);
		
		return value.toString();
	}
	
	/* (non-Javadoc)
	 * @see com.rho.IRhoRuby#processRequest(com.rho.util.Properties, com.rho.util.Properties, com.rho.util.Properties)
	 */
	public RubyValue processRequest(Properties reqHash, Properties reqHeaders, Properties resHeaders )throws IOException{
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
	
	RubyValue callFramework(RubyValue hashReq) {
		RubyValue value = RubyAPI.callPublicOneArgMethod(receiver, hashReq, null, serveID);
		
		return value;
	}
	
	/* (non-Javadoc)
	 * @see com.rho.IRhoRuby#createHash()
	 */
	public RubyHash createHash() {
		return ObjectFactory.createHash();
	}

	/* (non-Javadoc)
	 * @see com.rho.IRhoRuby#addTimeToHash(com.xruby.runtime.builtin.RubyHash, java.lang.String, long)
	 */
	public RubyValue addTimeToHash(RubyHash hash, String key, long val) {
		return hash.add( ObjectFactory.createString(key), ObjectFactory.createTime(val) );
	}

	/* (non-Javadoc)
	 * @see com.rho.IRhoRuby#addIntToHash(com.xruby.runtime.builtin.RubyHash, java.lang.String, int)
	 */
	public RubyValue addIntToHash(RubyHash hash, String key, int val) {
		return hash.add( ObjectFactory.createString(key), ObjectFactory.createInteger(val));
	}

	/* (non-Javadoc)
	 * @see com.rho.IRhoRuby#addStrToHash(com.xruby.runtime.builtin.RubyHash, java.lang.String, java.lang.String)
	 */
	public RubyValue addStrToHash(RubyHash hash, String key, String val) {
		return hash.add( ObjectFactory.createString(key), ObjectFactory.createString(val));
	}

	/* (non-Javadoc)
	 * @see com.rho.IRhoRuby#addHashToHash(com.xruby.runtime.builtin.RubyHash, java.lang.String, com.xruby.runtime.lang.RubyValue)
	 */
	public RubyValue addHashToHash(RubyHash hash, String key, RubyValue val) {
		return hash.add( ObjectFactory.createString(key), val);	
	}
	
}
