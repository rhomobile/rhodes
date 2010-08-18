package com.xruby.runtime.lang;

public abstract class RubyConstant extends RubyBasic {
    public static RubyConstant QFALSE = new RubyConstant(RubyRuntime.FalseClassClass) {
        public boolean isTrue() {
            return false;
        }
    };

    public static RubyConstant QTRUE = new RubyConstant(RubyRuntime.TrueClassClass) {
        public boolean isTrue() {
            return true;
        }
    };

    public static RubyConstant QNIL = new RubyConstant(RubyRuntime.NilClassClass) {
        public boolean isTrue() {
            return false;
        }
        
        public String toStr() {
            throw new RubyException(RubyRuntime.TypeErrorClass, "Cannot convert nil into String");
        }
        
    };

    private RubyConstant(RubyClass c) {
    	super(c);
    }
}

