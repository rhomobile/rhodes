/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;

/**
 *
 * Ruby method with no arg.
 *
 */
public abstract class RubyNoArgMethod extends RubyMethod {
    public RubyNoArgMethod() {
        super(0, false, 0);
    }

    protected abstract RubyValue run(RubyValue receiver, RubyBlock block);
    
    public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block) {
        throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `" + this.getID() + "': wrong number of arguments (1 for 0)");
    }
    
    public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
        throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `" + this.getID() + "': wrong number of arguments (2 for 0)");
    }

    protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
        return this.run(receiver, block);
    }
}
