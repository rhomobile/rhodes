package com.rhomobile.rhodes.datetime;

import android.content.Intent;

import com.rhomobile.rhodes.RhodesInstance;
import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
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
	
	public static RubyValue choose(RubyValue arg1, RubyValue arg2, RubyValue arg3, int v) {
		// TODO:
		//Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(), DateTimePickerScreen.class);
		//RhodesInstance.getInstance().startActivityForResult(intent, 5);
		throw new RubyException(RubyRuntime.NotImplementedErrorClass, "in `DateTimePicker': not implemented yet");
	}
	
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("choose", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
				if(args.size() != 3 && args.size() != 4)
					throw new RubyException(RubyRuntime.ArgumentErrorClass,
							"in `" + this.getID() + "': wrong number of arguments (" + args.size() + " for 3 or 4)");
				
				RubyValue arg1 = args.get(0);
				RubyValue arg2 = args.get(1);
				RubyValue arg3 = args.get(2);
				
				int v = 0;
				if(args.size() == 4)
					v = args.get(3).toInt();
				
				return DateTimePicker.choose(arg1, arg2, arg3, v);
			}
		});
	}

}
