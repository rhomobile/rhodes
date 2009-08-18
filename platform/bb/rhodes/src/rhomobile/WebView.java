package rhomobile;

import java.util.Vector;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyNoArgMethod;
import com.xruby.runtime.lang.RubyOneArgMethod;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubySymbol;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.RubyVarArgMethod;

public class WebView extends RubyBasic {

	public WebView(RubyClass arg0) {
		super(arg0);
		// TODO Auto-generated constructor stub
	}

	public static RubyValue refresh() {
		RhodesApplication.getInstance().refreshCurrentPage();
		return RubyConstant.QNIL;
	}

	public static RubyValue navigate(RubyValue arg0) {
		String url = arg0.toString();
		RhodesApplication.getInstance().addToHistory(url, null);
		RhodesApplication.getInstance().navigateUrl(url);
		return RubyConstant.QNIL;
	}

	public static RubyValue current_location() {
		String url = RhodesApplication.getInstance().getCurrentPageUrl();
		return ObjectFactory.createString(url);
	}
	
	public static RubyValue set_menu_items(RubyValue arg0) {
		RhodesApplication.getInstance().resetMenuItems();
		RubyHash items = (RubyHash)arg0;
		RubyArray keys = items.keys();
		RubyArray values = items.values();
		for( int i = 0; i < keys.size(); i++ ){
			String label = keys.get(i).toString();
			String value = values.get(i).toString();
			RhodesApplication.getInstance().addMenuItem(label, value);
		}
		return RubyConstant.QTRUE;
	}
		
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("refresh", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return WebView.refresh();
			}
		});		
		klass.getSingletonClass().defineMethod("navigate", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				return WebView.navigate(arg0);
			}
		});
		klass.getSingletonClass().defineMethod("current_location", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return WebView.current_location();
			}
		});		
		klass.getSingletonClass().defineMethod("set_menu_items", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				return WebView.set_menu_items(arg0);
			}
		});
	}
	
}
