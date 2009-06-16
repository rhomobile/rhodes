package com.rhomobile.rhodes.datetime;

import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.RubyVarArgMethod;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.builtin.RubyArray;

public class DateTimePicker extends RubyBasic {

	public DateTimePicker(RubyClass c) {
		super(c);
	}
	
	public static RubyValue choose(RubyValue arg1, RubyValue arg2, int v) {
		// TODO:
		throw new RubyException(RubyRuntime.NotImplementedErrorClass, "in `DateTimePicker': not implemented yet");
		//return RubyConstant.QNIL;
	}
	
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("choose", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
				if(args.size() != 2 && args.size() != 3)
					throw new RubyException(RubyRuntime.ArgumentErrorClass,
							"in `" + this.getID() + "': wrong number of arguments (" + args.size() + " for 2 or 3)");
				
				RubyValue arg1 = args.get(0);
				RubyValue arg2 = args.get(1);
				
				int v = 0;
				if(args.size() == 3)
					v = args.get(2).toInt();
				
				return DateTimePicker.choose(arg1, arg2, v);
			}
		});
	}

}
