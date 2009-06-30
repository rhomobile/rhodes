package rhomobile;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyOneArgMethod;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.RubyVarArgMethod;

public class Alert extends RubyBasic {

	public Alert(RubyClass arg0) {
		super(arg0);
	}

	public static RubyValue show_popup(RubyValue arg0) {
		String message = arg0.toString();
		RhodesApplication.getInstance().showPopup(message);
		return RubyConstant.QNIL;
	}
	
	public static RubyValue vibrate(RubyArray args) {
		String duration = "2500";
		if ((args != null) && (args.size() > 0))
			duration = args.get(0).toString();
		RhodesApplication.getInstance().vibrate(duration);
		return RubyConstant.QNIL;
	}

	public static RubyValue play_file(RubyArray args) {
		if ((args != null) && (args.size() > 0)) {
			String file_name = args.get(0).toString();
			String media_type = null;
			if ((args.size() > 1) && (args.get(1) != RubyConstant.QNIL))
				media_type = args.get(1).toString();
			RhodesApplication.getInstance().play_file(file_name,media_type);
		} else {
	        throw new RubyException(RubyRuntime.ArgumentErrorClass, "in WebView.play: wrong number of arguments");			
		}
		return RubyConstant.QNIL;
	}
	
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("show_popup", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				return Alert.show_popup(arg0);
			}
		});
		klass.getSingletonClass().defineMethod("vibrate", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
				return Alert.vibrate(args);
			}
		});
		klass.getSingletonClass().defineMethod("play_file", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
				return Alert.play_file(args);
			}
		});
	}
	
}