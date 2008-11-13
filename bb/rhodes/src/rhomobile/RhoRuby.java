package rhomobile;

import java.io.InputStream;

import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import java.io.IOException;
import java.io.ByteArrayInputStream;

public class RhoRuby {

	public static final RubyID serveID = RubyID.intern("serve_hash");
	
	public static void RhoRubyStart(String szAppPath){
		String[] args = new String[0];
		
		com.xruby.runtime.lang.RubyRuntime.init(args);		
	}
	
	public static void RhoRubyStop(){
		com.xruby.runtime.lang.RubyRuntime.fini();	
	}
	
	static public InputStream loadFile(String path){
		xruby.ServeME.main p = new xruby.ServeME.main();
		return p.getClass().getResourceAsStream("/apps"+path);		
	}
	
	public static InputStream processRequest(Properties reqHeaders, Properties resHeaders )throws IOException{
		RubyHash rh = ObjectFactory.createHash();
		for( int i = 0; i < reqHeaders.size(); i++ )
			addStrToHash(rh, reqHeaders.getKeyAt(i), reqHeaders.getValueAt(i) );
		
		RubyValue res = callFramework(rh);
		if ( res != null && res != RubyConstant.QNIL && res instanceof RubyHash ){
			RubyHash resHash = (RubyHash)res;
			RubyValue resBody = null;
			
			RubyArray arKeys = resHash.keys();
			RubyArray arValues = resHash.values();
			for( int i = 0; i < arKeys.size(); i++ ){
				String strKey = arKeys.get(i).toString();
				if ( !strKey.equals("request-body") )
					resHeaders.addProperty( strKey, arValues.get(i).toString() );
				else
					resBody = arValues.get(i);
			}
			if ( resBody != null && resBody != RubyConstant.QNIL )
				return new ByteArrayInputStream(resBody.toRubyString().toString().getBytes());
			
			return null;
		}
		
		return null;
	}
	
	static RubyValue callFramework(RubyValue hashReq) {
		xruby.ServeME.main p = new xruby.ServeME.main();
		RubyValue receiver = p.invoke();
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
