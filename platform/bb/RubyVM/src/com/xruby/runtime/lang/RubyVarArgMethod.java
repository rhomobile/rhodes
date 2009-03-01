/** 
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;

public abstract class RubyVarArgMethod extends RubyMethod {
	public RubyVarArgMethod() {
		super(-1, false, 0);
	}
	
	public RubyVarArgMethod(int argc, boolean has_asterisk_parameter, int default_argc) {
		super(argc, has_asterisk_parameter, default_argc);
	}
	
	protected abstract RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block);
}
