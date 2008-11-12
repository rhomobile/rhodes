package com.rho;

import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;

public class RhoRuby {

	public static final RubyID serveID = RubyID.intern("serve");
	
	void RhoRubyStart(String szAppPath){
		String[] args = new String[0];
		
		com.xruby.runtime.lang.RubyRuntime.init(args);		
	}
	
	void RhoRubyStop(){
		com.xruby.runtime.lang.RubyRuntime.fini();	
	}
	
	String callFramework(RubyValue hashReq) {
		xruby.ServeME.main p = new xruby.ServeME.main();
		RubyValue receiver = p.invoke();
		RubyValue value = RubyAPI.callPublicOneArgMethod(receiver, hashReq, null, serveID);
		
		return value.toString();
	}
	
	RubyHash createHash() {
		return ObjectFactory.createHash();
	}

	RubyValue addTimeToHash(RubyHash hash, String key, long val) {
		return hash.add( ObjectFactory.createString(key), ObjectFactory.createTime(val) );
	}

	RubyValue addIntToHash(RubyHash hash, String key, int val) {
		return hash.add( ObjectFactory.createString(key), ObjectFactory.createInteger(val));
	}

	RubyValue addStrToHash(RubyHash hash, String key, String val) {
		return hash.add( ObjectFactory.createString(key), ObjectFactory.createString(val));
	}

	RubyValue addHashToHash(RubyHash hash, String key, RubyValue val) {
		return hash.add( ObjectFactory.createString(key), val);	
	}
	
}
