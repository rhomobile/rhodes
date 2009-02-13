/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;

public abstract class RubyProgram {

    public RubyValue invoke() {
        RubyRuntime.ObjectClass.setAccessPrivate();//so that top level methods are private methods of Object by default
        return invoke(RubyRuntime.TOP_LEVEL_SELF_VALUE, null, null, RubyRuntime.ObjectClass);
    }

    public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block, RubyModule scope) {
        return run(receiver, args, block, scope);
    }

    protected abstract RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block, RubyModule scope);
}
