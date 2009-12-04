/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.*;

/**
 * This is a general purpuse value holder. If you want to wrap up a simple value and 
 * do not want to create a new subclass of RubyValue, this swiss army knife class is for you. 
 * 
 */
public class RubyData<T> extends RubyBasic {
	
	private T value_;
	
	protected RubyData(RubyClass c, T v) {
		super(c);
		value_ = v;
	}
	
	public T getData() {
		return value_;
	}
	
	public void setData(T v) {
		value_ = v;
	}
}
