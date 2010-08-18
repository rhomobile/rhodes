/**
 * Copyright 2005-2007 Yu Zhang
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyModule;
import com.xruby.runtime.lang.RubyRuntime;


public class ErrnoModuleBuilder {
    public static void initialize() {
        RubyModule m = RubyRuntime.ErrnoModule;
        RubyClass c = m.defineNewClass("ENOENT", RubyRuntime.SystemCallErrorClass);
        c.setConstant("Errno", ObjectFactory.createFixnum(IErrno.ENOENT));
        c = m.defineNewClass("EACCES", RubyRuntime.SystemCallErrorClass);
        c.setConstant("Errno", ObjectFactory.createFixnum(IErrno.EACCES));
    }
}
