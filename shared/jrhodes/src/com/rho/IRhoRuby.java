package com.rho;

import java.io.IOException;
import java.io.InputStream;

import com.rho.util.Properties;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.lang.RubyValue;

public interface IRhoRuby {

	public abstract void RhoRubyStart(String szAppPath);

	public abstract void RhoRubyStop();

	public abstract InputStream loadFile(String path);

	public abstract RubyValue processIndexRequest(String strIndex);

	public abstract String getStartPage();

	public abstract RubyValue processRequest(Properties reqHash,
			Properties reqHeaders, Properties resHeaders) throws IOException;

	public abstract RubyHash createHash();

	public abstract RubyValue addTimeToHash(RubyHash hash, String key, long val);

	public abstract RubyValue addIntToHash(RubyHash hash, String key, int val);

	public abstract RubyValue addStrToHash(RubyHash hash, String key, String val);

	public abstract RubyValue addHashToHash(RubyHash hash, String key,
			RubyValue val);

	public abstract void PlatformRhoRubyStart(String szAppPath);
}