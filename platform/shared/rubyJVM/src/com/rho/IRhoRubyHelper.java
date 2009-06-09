package com.rho;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyProgram;

public interface IRhoRubyHelper {
	public abstract void initRubyExtensions();
	public abstract RubyProgram createMainObject();
	public abstract String getPlatform();
	
	public void loadBackTrace(RubyArray backtrace);
	public String getDeviceId();
}
