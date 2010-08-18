/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;

public abstract class RubyTwoArgBlock extends RubyBlock {
    public RubyTwoArgBlock(RubyValue self, RubyValue arg, RubyArray args, RubyBlock block, RubyModule scope, RubyMethod owner, boolean definedInAnotherBlock) {
        super(2, false, 0, self, arg, args, block, scope, owner, definedInAnotherBlock);
    }

    protected RubyValue run(RubyValue receiver, RubyArray args) {
        RubyValue arg0 = RubyConstant.QNIL;
        RubyValue arg1 = RubyConstant.QNIL;

        if (args != null) {
            int size = args.size();
            if (size >= 1) {
                arg0 = args.get(0);
            }

            if (size >= 2) {
                arg1 = args.get(1);
            }
        }

        return this.run(receiver, arg0, arg1);
    }

    protected RubyValue run(RubyValue receiver, RubyValue arg) {
        return this.run(receiver, arg, RubyConstant.QNIL);
    }

    protected RubyValue run(RubyValue receiver) {
        return this.run(receiver, RubyConstant.QNIL, RubyConstant.QNIL);
    }

    protected abstract RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1);
}
