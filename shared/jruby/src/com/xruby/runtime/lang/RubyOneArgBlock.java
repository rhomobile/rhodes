/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;

public abstract class RubyOneArgBlock extends RubyBlock {
    public RubyOneArgBlock(RubyValue self, RubyValue arg, RubyArray args, RubyBlock block, RubyModule scope, RubyMethod owner, boolean definedInAnotherBlock) {
        super(1, false, 0, self, arg, args, block, scope, owner, definedInAnotherBlock);
    }

    protected RubyValue run(RubyValue receiver, RubyArray args) {
        RubyValue arg;
        if (args != null && args.size() == 1) {
            arg = args.get(0);
        } else if (args == null || args.size() == 0) {
            arg = RubyConstant.QNIL;
        } else {
            // TO DO: rethinking
            // For code: yield 1, 2; f {|x|}
            arg = args;
        }

        return this.run(receiver, arg);
    }

    protected RubyValue run(RubyValue receiver) {
        return this.run(receiver, RubyConstant.QNIL);
    }

    protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1) {
        return this.run(receiver, arg0);
    }

    protected abstract RubyValue run(RubyValue receiver, RubyValue arg);
}