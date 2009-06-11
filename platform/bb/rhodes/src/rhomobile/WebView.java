package rhomobile;

import j2me.util.ArrayList;

import java.util.Hashtable;

import javolution.util.FastTable;

import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyOneArgMethod;
import com.xruby.runtime.lang.RubyNoArgMethod;
import com.xruby.runtime.lang.RubyTwoArgMethod;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyString;

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
		RhodesApplication.getInstance().navigateUrl(url);
		return RubyConstant.QNIL;
	}

	public static RubyValue current_location() {
		String url = RhodesApplication.getInstance().getCurrentPageUrl();
		return ObjectFactory.createString(url);
	}
	
	public static RubyValue add_menu_items(RubyValue arg0, RubyValue arg1) {
		RubyArray items = (RubyArray) (new RubyArray(arg0)).get(0);
		for(int i = 0; i < items.size(); i++) {
			RubyHash currentItem = (RubyHash)items.get(i);
			RubyString title = (RubyString)currentItem.keys().get(0);
			RubyString path = (RubyString)currentItem.get(title);
			RhodesApplication.getInstance().addMenuItem(title.toString(), path.toString());
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
		klass.getSingletonClass().defineMethod("add_menu_items", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
				return WebView.add_menu_items(arg0, arg1);
			}
		});
	}
	
}
