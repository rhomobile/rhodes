package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;

public abstract class RubyNoOrOneArgMethod extends RubyMethod {
	public RubyNoOrOneArgMethod() {
		super(-1, false, 0);
	}
	
	protected abstract RubyValue run(RubyValue receiver, RubyBlock block);
	
	protected abstract RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block);
	
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
		if (args == null || args.size() == 0) {
			return this.run(receiver, block);
		} else {
			return this.run(receiver, args.get(0), block);
		}
	}
}