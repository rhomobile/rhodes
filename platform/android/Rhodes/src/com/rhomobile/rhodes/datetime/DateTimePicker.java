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
	
	public static RubyValue choose(RubyValue arg1, RubyValue arg2, RubyValue arg3, int v, String opaque) {
		String callback = arg1.toStr();
		String title = arg2.toStr();
		long init = arg3.toRubyTime().getTime();
		
		Intent intent = new Intent(RhodesInstance.getInstance().getApplicationContext(),
				DateTimePickerScreen.class);
		intent.putExtra("callback", callback);
		intent.putExtra("title", title);
		intent.putExtra("init", init);
		intent.putExtra("fmt", v);
		intent.putExtra("opaque", opaque);
		
		RhodesInstance.getInstance().startActivityForResult(intent, 5);
		return RubyConstant.QNIL;
	}
	
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("choose", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
				if(args.size() != 4 && args.size() != 5)
					throw new RubyException(RubyRuntime.ArgumentErrorClass,
							"in `" + this.getID() + "': wrong number of arguments (" + args.size() + " for 4 or 5)");
				
				RubyValue arg1 = args.get(0);
				RubyValue arg2 = args.get(1);
				RubyValue arg3 = args.get(2);
				int v = args.get(3).toInt();
				
				String opaque = null;
				if(args.size() == 5)
					opaque = args.get(4).toStr();
				
				return DateTimePicker.choose(arg1, arg2, arg3, v, opaque);
			}
		});
	}

}
