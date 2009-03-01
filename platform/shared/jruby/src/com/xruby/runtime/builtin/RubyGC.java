/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.*;

////@RubyLevelModule(name="GC")
public class RubyGC {
    //@RubyLevelMethod(name="start", singleton=true)
    public static RubyValue start(RubyValue receiver) {
        System.gc();
        return RubyConstant.QNIL;
    }
}
