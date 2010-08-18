/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;
import j2me.lang.Cloneable;

public abstract class RubyMethod extends MethodBlockBase implements Cloneable {
    private int access_;
    private RubyID id_; //its own id, saved to build better error message

    public static final int PUBLIC = 0;
    public static final int PROTECTED = 1;
    public static final int PRIVATE = 2;
    public static final int NON_PRIVATE = 3;
    public static final int ALL = 4;

    protected RubyMethod(int argc, boolean has_asterisk_parameter, int default_argc) {
        super(argc, has_asterisk_parameter, default_argc);
        access_ = PUBLIC;
    }
  
    public RubyMethod clone()
    { 
    	RubyMethod cl = null;
    	try{
    		cl = (RubyMethod)(this.getClass().newInstance());
    		cl.doClone(this);
    	}catch(Exception e){
    		throw new RubyException(RubyRuntime.ExceptionClass, e.toString());    		
    	}
    	return cl; 
    }
    
    protected void doClone(RubyMethod orig)
    {
    	access_ = orig.access_;
    	id_ = orig.id_;
    	
    	super.doClone(orig);
    }
    //RHO_COMMENT: clone
    /*
    public RubyMethod clone() {
    	RubyMethod v;
        try {
            v = (RubyMethod) super.clone();
        } catch (CloneNotSupportedException e) {
            throw new RubyException(RubyRuntime.ExceptionClass, e.toString());
        }
        return v;
    }*/

    void setID(RubyID id) {
        id_ = id;
    }

    public RubyID getID() {
        return id_;
    }

    public RubyBlock convertToRubyBolck(RubyValue self) {
        final RubyMethod m = this;
        return new RubyBlock(argc_, has_asterisk_parameter_, default_argc_, self, null, null, null, getScope(), this, false) {
            protected RubyValue run(RubyValue receiver, RubyArray args) {
                return m.invoke(receiver, args, null);
            }
        };
    }

    int getAccess() {
        return access_;
    }

    public void setAccess(int access) {
        access_ = access;
    }
    
    private void validateArgSize(int args_length) {
    	// TODO parameter checking with 'has_asterisk_parameter_' maybe incorrect
    	if ((0 == default_argc_ && !has_asterisk_parameter_ && args_length != argc_) ||
    			(0 != default_argc_ && !has_asterisk_parameter_ && args_length > argc_)) {
    		throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `" + id_.toString() + "': wrong number of arguments (" + args_length + " for " + argc_ + ")");
    	} else if (args_length < (argc_ - default_argc_)) {
    		//number of arguments falls short anyway
    		throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `" + id_.toString() + "': wrong number of arguments (" + args_length + " for " + (argc_ - default_argc_) + ")");
    	}
    }

    // multi arg invocation
    public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block) {
        if (argc_ >= 0) {
            final int args_length = (null != args) ? args.size() : 0;
            validateArgSize(args_length);
        }

        RubyValue v = run(receiver, args, block);
        if (null != block) {
            v.setReturnedInBlock(block.returned(), block.breakedOrReturned(), false);
        } else {
            v.setReturnedInBlock(false, false, false);
        }
        return v;
    }

    protected abstract RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block);

    // no arg invocation
    public RubyValue invoke(RubyValue receiver, RubyBlock block) {
    	// TODO parameter checking with 'has_asterisk_parameter_' maybe incorrect
        if (argc_ >= 0) {
        	validateArgSize(0);
        }

        RubyValue v = run(receiver, block);
        if (null != block) {
            v.setReturnedInBlock(block.returned(), block.breakedOrReturned(), false);
        } else {
            v.setReturnedInBlock(false, false, false);
        }
        return v;
    }

    protected RubyValue run(RubyValue receiver, RubyBlock block) {
        return this.run(receiver, (RubyArray)null, block);
    }

    // one arg invocation
    public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block) {
        if (argc_ >= 0) {
        	validateArgSize(1);
        }

        RubyValue v = run(receiver, arg, block);
        if (null != block) {
            v.setReturnedInBlock(block.returned(), block.breakedOrReturned(), false);
        } else {
            v.setReturnedInBlock(false, false, false);
        }
        return v;
    }

    protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block) {
        return this.run(receiver, new RubyArray(arg), block);
    }
    
    // two arg invocation
    public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
        if (argc_ >= 0) {
        	validateArgSize(2);
        }

        RubyValue v = run(receiver, arg0, arg1, block);
        if (null != block) {
            v.setReturnedInBlock(block.returned(), block.breakedOrReturned(), false);
        } else {
            v.setReturnedInBlock(false, false, false);
        }
        return v;
    }

    protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
        return this.run(receiver, new RubyArray(arg0, arg1), block);
    }
}

/**
 * Represent a method removed by 'undef'
 * When a method is 'undef'ed, it can not be simply removed, instead, a UndefMethod::getInstance()
 * should be inserted to the method table.
 */
class UndefMethod extends RubyVarArgMethod {

    private static UndefMethod instance_ = new UndefMethod();

    private UndefMethod() {
    }

    public RubyMethod clone()
    {
    	UndefMethod um = new UndefMethod(); 
    	um.doClone(this);
    	return um;
    }
    
    protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
        throw new Error("should not reach here!");
    }

    static RubyMethod getInstance() {
        return instance_;
    }

    static boolean isUndef(RubyMethod m) {
        return m == instance_;
    }

}
