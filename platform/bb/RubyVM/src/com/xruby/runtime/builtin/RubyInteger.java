/**
 * Copyright 2007 Xue Yong Zhi, Jie Li, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.RubyAPI;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyID;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
//import com.xruby.runtime.lang.annotation.RubyLevelClass;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;
//import com.xruby.runtime.lang.annotation.UndefMethod;

/*@RubyLevelClass(
		name="Integer", 
		superclass="Numeric",
		undef={
				@UndefMethod(name="new", classMethod=true)
		}
)*/
public abstract class RubyInteger extends RubyNumeric {
	public RubyInteger toRubyInteger() {
		return this;
	}
	
	//@RubyLevelMethod(name="times")
	public RubyValue times(RubyBlock block) {
		RubyValue a = ObjectFactory.FIXNUM0;
        while (RubyAPI.callPublicOneArgMethod(a, this, null, RubyID.ltID).isTrue()) {
            RubyValue v = block.invoke(this, a);
            if (block.breakedOrReturned()) {
                return v;
            }
            a = RubyAPI.callPublicOneArgMethod(a, ObjectFactory.FIXNUM1, null, RubyID.plusID);
        }
        
        return this;
	}
	
	//@RubyLevelMethod(name="chr")
	public RubyValue chr() {
		int i = this.toInt();
		if (i < 0 || i > 0xff) {
			throw new RubyException(RubyRuntime.RangeErrorClass, i + " out of char range");
		}
		
		char charValue = (char)i;
        return ObjectFactory.createString("" + charValue);
	}

	//@RubyLevelMethod(name="ord")
	public RubyValue ord() {
		return this;
	}
	
}
