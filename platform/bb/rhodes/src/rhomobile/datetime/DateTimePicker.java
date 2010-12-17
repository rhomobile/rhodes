package rhomobile.datetime;

import net.rim.device.api.i18n.DateFormat;
import net.rim.device.api.i18n.SimpleDateFormat;
import net.rim.device.api.ui.UiApplication;
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
	private String _callback;
	private String _title;
	private long _init;
	private DateFormat _fmt;
	private String _opaque;
	
	private static DateTimeScreen screen = null;
	
	CallDateTimePickerScreen(String callback, String title, long init, DateFormat fmt, String opaque) {
		_callback = callback;
		_title = title;
		_init = init;
		_fmt = fmt;
		_opaque = opaque;
	}
	
	public void run(){
		if (screen != null)
			return;
		//Initialize the screen.
		screen = new DateTimeScreen(_callback, _title, _init, _fmt, _opaque);
        UiApplication.getUiApplication().pushModalScreen(screen);
        screen = null;
	}
}

public class DateTimePicker extends RubyBasic {

	public DateTimePicker(RubyClass c) {
		super(c);
	}
	
	public static RubyValue choose(RubyValue arg1, RubyValue arg2, RubyValue arg3, DateFormat fmt, String opaque) {
		String callback = arg1.toStr();
		String title = arg2.toStr();
		long init = arg3.toRubyTime().getTime();
		
		CallDateTimePickerScreen screen = new CallDateTimePickerScreen(callback, title, (long)init, fmt, opaque);
		UiApplication.getUiApplication().invokeLater(screen);
        
		return RubyConstant.QNIL;
	}

	public static RubyValue choose_with_range(RubyValue arg1, RubyValue arg2, RubyValue arg3, DateFormat fmt, String opaque, RubyValue arg6, RubyValue arg7) {
		String callback = arg1.toStr();
		String title = arg2.toStr();
		long init = arg3.toRubyTime().getTime();
		
		CallDateTimePickerScreen screen = new CallDateTimePickerScreen(callback, title, (long)init, fmt, opaque);
		UiApplication.getUiApplication().invokeLater(screen);
        
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
				RubyValue arg4 = args.get(3);
				
				String opaque = null;
				if(args.size() == 5)
					opaque = args.get(4).toStr();
				
				int fmt;
				switch(arg4.toInt())
				{
				case 0: fmt = DateFormat.DATE_LONG | DateFormat.TIME_FULL; break;
				case 1: fmt = DateFormat.DATE_LONG; break;
				case 2: fmt = DateFormat.TIME_FULL; break;
				default: throw new RubyException(RubyRuntime.ArgumentErrorClass,
						"in `" + this.getID() + "': wrong value of the third parameter (" +
						arg4.toInt() + ", should be 0, 1 or 2)");
				}
				
				return DateTimePicker.choose(arg1, arg2, arg3, new SimpleDateFormat(fmt), opaque);
			}
		});		
		klass.getSingletonClass().defineMethod("choose_with_range", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
				if(args.size() != 7)
					throw new RubyException(RubyRuntime.ArgumentErrorClass,
							"in `" + this.getID() + "': wrong number of arguments (" + args.size() + " for 4 or 5)");
				
				RubyValue arg1 = args.get(0);
				RubyValue arg2 = args.get(1);
				RubyValue arg3 = args.get(2);
				RubyValue arg4 = args.get(3);
				
				String opaque = null;
				opaque = args.get(4).toStr();

				RubyValue arg6 = args.get(5);
				RubyValue arg7 = args.get(6);

				
				int fmt;
				switch(arg4.toInt())
				{
				case 0: fmt = DateFormat.DATE_LONG | DateFormat.TIME_FULL; break;
				case 1: fmt = DateFormat.DATE_LONG; break;
				case 2: fmt = DateFormat.TIME_FULL; break;
				default: throw new RubyException(RubyRuntime.ArgumentErrorClass,
						"in `" + this.getID() + "': wrong value of the third parameter (" +
						arg4.toInt() + ", should be 0, 1 or 2)");
				}
				
				return DateTimePicker.choose_with_range(arg1, arg2, arg3, new SimpleDateFormat(fmt), opaque, arg6, arg7);
			}
		});		
	}

}
