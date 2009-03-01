package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;

public abstract class RubyOneOrTwoArgMethod extends RubyMethod {
	public RubyOneOrTwoArgMethod() {
		super(-1, false, 0);
	}
	
	public RubyValue invoke(RubyValue receiver, RubyBlock block) {
		throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `" + this.getID() + "': wrong number of arguments (0 for 1)");
	}

	protected abstract RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block);
	
	protected abstract RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block);
	
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
		int size = args.size();
		switch (size) {
		case 0:
			// should not reach here
			throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `" + this.getID() + "': wrong number of arguments (0 for 1)");
		case 1:
			return this.run(receiver, args.get(0), block);
		case 2:
			return this.run(receiver, args.get(0), args.get(1), block);
		}
		
		// should not reach here
		throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `" + this.getID() + "': wrong number of arguments (" + size + " for 2)");
	}
}
