package com.xruby.runtime.lang.util;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyMethod;
import com.xruby.runtime.lang.RubyValue;

public class DummyMethod extends RubyMethod {
	public static DummyMethod INSTANCE = new DummyMethod();
	
	private DummyMethod() {
		super(-1, false, 0);
	}

	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block) {
		return RubyConstant.QNIL;
	}

	public RubyValue invoke(RubyValue receiver, RubyBlock block) {
		return RubyConstant.QNIL;
	}

	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block) {
		return RubyConstant.QNIL;
	}

	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
		return RubyConstant.QNIL;
	}

	// Can't be here
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
		return RubyConstant.QNIL;
	}
}
