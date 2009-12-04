/**
 * Copyright 2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.stdlib;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.lang.annotation.RubyAllocMethod;
import com.xruby.runtime.lang.annotation.RubyLevelClass;
import com.xruby.runtime.lang.annotation.RubyLevelMethod;

@RubyLevelClass(name="StringIO", modules="Enumerable")
public class RubyStringIO extends RubyBasic {
    private RubyString value_;

    private RubyStringIO() {
        super(RubyRuntime.StringIOClass);
    }

    @RubyAllocMethod
    public static RubyStringIO alloc(RubyValue receiver) {
        return new RubyStringIO();
    }

    @RubyLevelMethod(name="initialize")
    public RubyStringIO initialize() {
        this.value_ = ObjectFactory.createString();
        return this;
    }
    
    @RubyLevelMethod(name="initialize")
    public RubyStringIO initialize(RubyValue arg) {
        this.value_ = arg.toRubyString().clone();
        return this;
    }

    @RubyLevelMethod(name="string")
    public RubyString string() {
        return value_;
    }

    @RubyLevelMethod(name="puts")
    public RubyString puts(RubyValue arg) {
        return value_.appendString(arg);
    }
}
