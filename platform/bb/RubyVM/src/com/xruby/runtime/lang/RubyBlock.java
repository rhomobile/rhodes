/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;

public abstract class RubyBlock extends MethodBlockBase {

    // if run finished, and __break__ is not true, it indicated break happend
    //e.g.
    // loop do
    //      break 'xxx'
    // end
    protected boolean __break__ = false;
    protected boolean __return__ = false;
    protected boolean __retry__ = false;

    // Normally RubyBlock was created in one place, invoked(yield) later in another
    // place. The block needs to keep the context of its creator(self, arg, block arg,
    // scope etc).
    protected RubyValue selfOfCurrentMethod_;//need this for {self} TODO why do we need 'receiver' for run method?
    protected RubyArray argsOfCurrentMethod_;//need this for {super}
    protected final RubyValue argOfCurrentMethod_;//need this for {super}
    protected final RubyBlock blockOfCurrentMethod_;//need this for {yield}

    private final boolean definedInAnotherBlock_;//not null if defined in another block

    private boolean createdByLambda_ = false;

    private RubyMethod currentMethod_;

    public RubyBlock(int argc,
                        boolean has_asterisk_parameter,
                        int default_argc,
                        RubyValue self,
                        RubyValue arg,//not null for one arg method
                        RubyArray args,//not null for var arg method
                        RubyBlock block,
                        RubyModule scope,
                        RubyMethod currentMethod,
                        boolean definedInAnotherBlock) {
        super(argc, has_asterisk_parameter, default_argc);
        selfOfCurrentMethod_ = self;
        argOfCurrentMethod_ = arg;
        argsOfCurrentMethod_ = args;
        blockOfCurrentMethod_ = block;
        setScope(scope);
        currentMethod_ = currentMethod;
        definedInAnotherBlock_ = definedInAnotherBlock;
    }

    public void setArgsOfCurrentMethod(RubyArray args) {
        argsOfCurrentMethod_ = args;
    }

    public void setCurrentMethod(RubyMethod m) {
        currentMethod_ = m;
    }

    public RubyMethod getCurrentMethod() {
        return currentMethod_;
    }

    public RubyID getID() {
        return currentMethod_.getID();
    }

    public void setSelf(RubyValue v) {
        selfOfCurrentMethod_ = v;
    }

    public RubyValue getSelf() {
        return selfOfCurrentMethod_;
    }

    public boolean isDefinedInAnotherBlock() {
        return definedInAnotherBlock_;
    }

    public boolean breakedOrReturned() {
        return __break__ || __return__;
    }

    public boolean returned() {
        return createdByLambda_ ? false : __return__;
    }

    public boolean shouldRetry() {
        return __retry__;
    }

    public void setCreatedByLambda() {
        createdByLambda_ = true;
    }

    public boolean createdByLambda() {
        return createdByLambda_;
    }

    private void validateParameterForProcCall(int actual_args_length) {
        if (argc_ >= 0 && !has_asterisk_parameter_) {
            int required_args_length = argc_ - default_argc_;
            if (actual_args_length != required_args_length) {
                throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong number of arguments (" + actual_args_length + " for " + required_args_length + ")");
            }
        }
    }

    public RubyValue invoke(RubyValue receiver, RubyArray args) {
        if (createdByLambda_) {
            validateParameterForProcCall((null == args) ? 0 : args.size());
        }

        __break__ = false;
        __return__ = false;
        __retry__ = false;

        RubyValue v = run(receiver, null != args ? args : new RubyArray(0));
        //TODO Maybe we can just use the fields in BlockCallStatus, remove the
        //__break__, __return__, __retry__ here
        if (v.returnedInBlock()) {
            __return__ = true;
        }
        return v;
    }

    protected abstract RubyValue run(RubyValue receiver, RubyArray args);

    // no arg invocation
    public RubyValue invoke(RubyValue receiver) {
        if (createdByLambda_) {
            validateParameterForProcCall(0);
        }

        __break__ = false;
        __return__ = false;
        __retry__ = false;

        RubyValue v = run(receiver);
        //TODO Maybe we can just use the fields in BlockCallStatus, remove the
        //__break__, __return__, __retry__ here
        if (v.returnedInBlock()) {
            __return__ = true;
        }
        return v;
    }

    // no arg run
    protected RubyValue run(RubyValue receiver) {
        return this.run(receiver, new RubyArray(0));
    }

    // one arg invocation
    public RubyValue invoke(RubyValue receiver, RubyValue arg) {
        if (createdByLambda_) {
            validateParameterForProcCall(1);
        }

        __break__ = false;
        __return__ = false;
        __retry__ = false;

        RubyValue v = run(receiver, arg);
        //TODO Maybe we can just use the fields in BlockCallStatus, remove the
        //__break__, __return__, __retry__ here
        if (v.returnedInBlock()) {
            __return__ = true;
        }
        return v;
    }

    // one arg run
    protected RubyValue run(RubyValue receiver, RubyValue arg) {
        return this.run(receiver, new RubyArray(arg));
    }

    // two args invocation
    public RubyValue invoke(RubyValue receiver, RubyValue arg1, RubyValue arg2) {
        if (createdByLambda_) {
            validateParameterForProcCall(2);
        }

        __break__ = false;
        __return__ = false;
        __retry__ = false;

        RubyValue v = run(receiver, arg1, arg2);
        //TODO Maybe we can just use the fields in BlockCallStatus, remove the
        //__break__, __return__, __retry__ here
        if (v.returnedInBlock()) {
            __return__ = true;
        }
        return v;
    }

    // two args run
    protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1) {
        return this.run(receiver, new RubyArray(arg0, arg1));
    }
}

