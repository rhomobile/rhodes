package com.xruby.runtime.lang;

public abstract class RubySpecialValue extends RubyValue {
	public void setRubyClass(RubyClass klass) {
		throw new RubyException(RubyRuntime.TypeErrorClass, this.getRubyClass().getName() + " can't be set class");
	}
	
	public RubyClass getSingletonClass() {
    	throw new RubyException(RubyRuntime.TypeErrorClass, this.getRubyClass().getName() + " can't define singleton");
    }

	public RubyValue clone() {
		throw new RubyException(RubyRuntime.TypeErrorClass, "can't clone " + this.getRubyClass().getName());
	}
}
