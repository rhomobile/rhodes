/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

public class RubyExceptionValueForThrow extends RubyExceptionValue {

    private RubyValue symbol_;
    private RubyValue returnValue_;

    public RubyExceptionValueForThrow(RubyValue symbol, RubyValue returnValue) {
        super(RubyRuntime.NameErrorClass, "uncaught throw `" + symbol.toString() + "'");
        symbol_= symbol;
        returnValue_ = returnValue;
    }

    public boolean isSameSymbol(RubyValue another) {
        return symbol_.toString() == another.toString();
    }

    public RubyValue getReturnValue() {
        return returnValue_;
    }
}
