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
		String callback = arg1.toStr();
		String title = arg2.toStr();
		long init = arg3.toRubyTime().getTime();
		
		Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(), DateTimePickerScreen.class);
		String prefix = "com.rhomobile.rhodes.datetime.DateTimePicker";
		intent.putExtra("callback", callback);
		intent.putExtra("title", title);
		intent.putExtra("init", init);
		intent.putExtra("fmt", v);
		
		RhodesInstance.getInstance().startActivityForResult(intent, 5);
		return arg1;
		//throw new RubyException(RubyRuntime.NotImplementedErrorClass, "in `DateTimePicker': not implemented yet");
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
