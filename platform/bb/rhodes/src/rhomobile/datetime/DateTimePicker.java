package rhomobile.datetime;

import net.rim.device.api.ui.UiApplication;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyTwoArgMethod;
import com.xruby.runtime.lang.RubyValue;

class CallDateTimePickerScreen implements Runnable {
	private String _title;
	private long _init;
	
	public Long retvalue;
	
	CallDateTimePickerScreen(String title, long init) {
		_title = title;
		_init = init;
	}
	
	public void run(){
		//Initialize the screen.
		DateTimeScreen screen = new DateTimeScreen(_title, _init);
        UiApplication.getUiApplication().pushModalScreen(screen);
        retvalue = screen.retvalue;
	}
}

public class DateTimePicker extends RubyBasic {

	public DateTimePicker(RubyClass c) {
		super(c);
	}
	
	public static RubyValue choose(RubyValue arg1, RubyValue arg2) {
		String title = arg1.toStr();
		long init = arg2.toRubyTime().getTime();
		
		CallDateTimePickerScreen screen = new CallDateTimePickerScreen(title, (long)init);
		UiApplication.getUiApplication().invokeAndWait(screen);
        
		//return RubyConstant.QNIL;
		if(screen.retvalue == null)
			return RubyConstant.QNIL;
		else
			return ObjectFactory.createTime(screen.retvalue.longValue());
	}

	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("choose", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg1,
					RubyValue arg2, RubyBlock block) {
				return DateTimePicker.choose(arg1, arg2);
			}
		});		
	}

}
