package rhomobile.datetime;

import net.rim.device.api.i18n.DateFormat;
import net.rim.device.api.i18n.SimpleDateFormat;
import net.rim.device.api.ui.UiApplication;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.RubyVarArgMethod;

class CallDateTimePickerScreen implements Runnable {
	private String _title;
	private long _init;
	private DateFormat _fmt;
	
	public Long retvalue;
	
	CallDateTimePickerScreen(String title, long init, DateFormat fmt) {
		_title = title;
		_init = init;
		_fmt = fmt;
	}
	
	public void run(){
		//Initialize the screen.
		DateTimeScreen screen = new DateTimeScreen(_title, _init, _fmt);
        UiApplication.getUiApplication().pushModalScreen(screen);
        retvalue = screen.retvalue;
	}
}

public class DateTimePicker extends RubyBasic {

	public DateTimePicker(RubyClass c) {
		super(c);
	}
	
	public static RubyValue choose(RubyValue arg1, RubyValue arg2, DateFormat fmt) {
		String title = arg1.toStr();
		long init = arg2.toRubyTime().getTime();
		
		CallDateTimePickerScreen screen = new CallDateTimePickerScreen(title, (long)init, fmt);
		UiApplication.getUiApplication().invokeAndWait(screen);
        
		//return RubyConstant.QNIL;
		if(screen.retvalue == null)
			return RubyConstant.QNIL;
		else
			return ObjectFactory.createTime(screen.retvalue.longValue());
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
				
				int fmt;
				switch(v)
				{
				case 0: fmt = DateFormat.DATE_LONG | DateFormat.TIME_FULL; break;
				case 1: fmt = DateFormat.DATE_LONG; break;
				case 2: fmt = DateFormat.TIME_FULL; break;
				default: throw new RubyException(RubyRuntime.ArgumentErrorClass,
						"in `" + this.getID() + "': wrong value of the third parameter (" +
						v + ", should be 0, 1 or 2)");
				}
				
				return DateTimePicker.choose(arg1, arg2, new SimpleDateFormat(fmt));
			}
		});		
	}

}
