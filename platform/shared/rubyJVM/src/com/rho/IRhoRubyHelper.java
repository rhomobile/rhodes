package com.rho;

import com.xruby.runtime.lang.RubyProgram;

public interface IRhoRubyHelper {
	public abstract void initRubyExtensions();
	public abstract RubyProgram createMainObject();
}
