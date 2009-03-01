package com.rho;

import java.io.IOException;
import java.io.InputStream;

import com.rho.util.Properties;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyPlatformUtils;
import com.xruby.runtime.lang.RubyValue;

//Rho ruby wrapper
public class RhoRuby {

	private static IRhoRuby rhoRuby;

	/**
	 * Call this method to specify platform specific RhoRuby implementation
	 * 
	 * @param rhoRubyImpl
	 */
	static public void setRhoRubyImpl(IRhoRuby rhoRubyImpl) {
		if (rhoRuby == null)
			rhoRuby = rhoRubyImpl;
	}

	static public void PlatformRhoRubyStart(String szAppPath) {

		if (rhoRuby != null)
			rhoRuby.PlatformRhoRubyStart(szAppPath);
	}

	static public void RhoRubyStart(String szAppPath) {
		if (rhoRuby != null)
			rhoRuby.RhoRubyStart(szAppPath);
	}

	static public void RhoRubyStop() {
		if (rhoRuby != null)
			rhoRuby.RhoRubyStop();
	}

	static public RubyValue addHashToHash(RubyHash hash, String key, RubyValue val) {
		if (rhoRuby != null)
			return rhoRuby.addHashToHash(hash, key, val);

		return null;
	}

	static public RubyValue addIntToHash(RubyHash hash, String key, int val) {
		if (rhoRuby != null)
			return rhoRuby.addIntToHash(hash, key, val);

		return null;

	}

	static public RubyValue addStrToHash(RubyHash hash, String key, String val) {
		if (rhoRuby != null)
			return rhoRuby.addStrToHash(hash, key, val);

		return null;
	}

	static public RubyValue addTimeToHash(RubyHash hash, String key, long val) {
		if (rhoRuby != null)
			return rhoRuby.addTimeToHash(hash, key, val);

		return null;
	}

	static public RubyHash createHash() {

		if (rhoRuby != null)
			return rhoRuby.createHash();

		return null;
	}

	static public String getStartPage() {

		if (rhoRuby != null)
			return rhoRuby.getStartPage();

		return null;
	}

	static public InputStream loadFile(String path) {

		InputStream is = null;
		if (rhoRuby != null)
			is = rhoRuby.loadFile(path);

		if ( is == null )
			is = RubyPlatformUtils.loadFile(path, null);
		
		return is;
	}

	static public RubyValue processIndexRequest(String strIndex) {

		if (rhoRuby != null)
			return rhoRuby.processIndexRequest(strIndex);

		return null;
	}

	static public RubyValue processRequest(Properties reqHash, Properties reqHeaders,
			Properties resHeaders) throws IOException {

		if (rhoRuby != null)
			return rhoRuby.processRequest(reqHash, reqHeaders, resHeaders);

		return null;
	}

}
