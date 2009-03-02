package com.xruby.runtime.builtin;

import java.io.InputStream;

public class RubyPlatformUtils {

	private static IRubyPlatformUtils rubyPlatformUtils;

	/**
	 * Call this method to specify platform specific RubyPlatformUtils
	 * implementation
	 * 
	 * @param rubyPlatformUtilsImpl
	 */
	static public void setRubyPlatformUtilsImpl(IRubyPlatformUtils rubyPlatformUtilsImpl) {
		if (rubyPlatformUtils == null)
			rubyPlatformUtils = rubyPlatformUtilsImpl;
	}

	static public InputStream loadFile(String path, String mode) {
		if (rubyPlatformUtils != null)
			return rubyPlatformUtils.loadFile(path, mode);

		return null;
	}
	
	static public String getPlatform()
	{
		if (rubyPlatformUtils != null)
			return rubyPlatformUtils.getPlatform();

		return null;
	}

}
