/** 
 * Copyright 2007 Xue Yong Zhi, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;

public abstract class RubyTwoArgMethod extends RubyMethod {
	public RubyTwoArgMethod() {
		super(2, false, 0);
	}
	
	protected abstract RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block);
	
	public RubyValue invoke(RubyValue receiver, RubyBlock block) {
		throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `" + this.getID() + "': wrong number of arguments (0 for 2)");
	}

	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block) {
		throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `" + this.getID() + "': wrong number of arguments (1 for 2)");
	}

	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
		return this.run(receiver, args.get(0), args.get(1), block);
	}
}