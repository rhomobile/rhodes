package com.rhomobile.rhodes.nativebar;

import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyTwoArgMethod;
import com.xruby.runtime.lang.RubyValue;

public class NativeBar extends RubyBasic {

	public NativeBar(RubyClass arg0) {
		super(arg0);
		// TODO Auto-generated constructor stub
	}
		
	public static void initMethods(RubyClass klass) { 
		klass.getSingletonClass().defineMethod("create", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
				return RubyConstant.QNIL;
			}
		});
	}
	
}