package rhomobile;

import com.xruby.runtime.lang.*;

public class NativeBar 
{
	public static void initMethods(RubyClass klass) { 
		klass.getSingletonClass().defineMethod("create", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
				return RubyConstant.QNIL;
			}
		});
		klass.getSingletonClass().defineMethod("create_native_toolbar", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
				return RubyConstant.QNIL;
			}
		});
		klass.getSingletonClass().defineMethod("create_native_tabbar", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
				return RubyConstant.QNIL;
			}
		});
		
		klass.getSingletonClass().defineMethod("remove", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return RubyConstant.QNIL;
			}
		});
		klass.getSingletonClass().defineMethod("remove_native_toolbar", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return RubyConstant.QNIL;
			}
		});
		klass.getSingletonClass().defineMethod("remove_native_tabbar", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return RubyConstant.QNIL;
			}
		});
		
		klass.getSingletonClass().defineMethod("switch_tab", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				return RubyConstant.QNIL;
			}
		});
		klass.getSingletonClass().defineMethod("native_tabbar_switch_tab", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				return RubyConstant.QNIL;
			}
		});
		klass.getSingletonClass().defineMethod("native_tabbar_set_tab_badge", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
				return RubyConstant.QNIL;
			}
		});
		
	}
	
}