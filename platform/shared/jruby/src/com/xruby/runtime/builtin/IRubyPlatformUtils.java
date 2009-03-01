package com.xruby.runtime.builtin;

import java.io.InputStream;

public interface IRubyPlatformUtils {

	public abstract InputStream loadFile(String path, String mode);
	
	public abstract String getPlatform();
}
