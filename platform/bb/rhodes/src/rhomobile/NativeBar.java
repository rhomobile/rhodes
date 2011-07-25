/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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