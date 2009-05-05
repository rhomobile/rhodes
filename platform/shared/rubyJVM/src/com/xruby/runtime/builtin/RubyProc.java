/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.RubyBinding;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.RubyVarArgMethod;
//import com.xruby.runtime.lang.annotation.RubyLevelClass;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;
import com.xruby.runtime.lang.RubyMethod;

//@RubyLevelClass(name="Proc")
public class RubyProc extends RubyBinding {
    private final RubyBlock value_;

    RubyProc(RubyBlock v) {
        super(RubyRuntime.ProcClass);
        setSelf(RubyRuntime.TOP_LEVEL_SELF_VALUE);//TODO should not hardcode this
        setScope(RubyRuntime.ObjectClass);//TODO should not hardcode this
        value_ = v;
    }

    //@RubyLevelMethod(name="new")
    public static RubyValue newProc(RubyValue receiver, RubyBlock block) {
        return ObjectFactory.createProc(block);
    }

    //@RubyLevelMethod(name="arity")
    public RubyFixnum arity() {
        return ObjectFactory.createFixnum(value_.arity());
    }

    //@RubyLevelMethod(name="==", alias="eql?")
    public RubyValue equal(RubyValue v) {
        return ObjectFactory.createBoolean(equals(v));
    }

    public boolean equals(Object o) {
        if (null == o) {
            return false;
        } else if (o instanceof RubyProc) {
            return value_ == ((RubyProc) o).value_;
        } else {
            return false;
        }
    }

    public RubyBlock getBlock() {
        return value_;
    }

    public boolean isDefinedInAnotherBlock() {
        return value_.isDefinedInAnotherBlock();
    }

    private void setUpCallContext() {
    	//TODO: setUpCallContext - reflection uses
    	/*
        Field[] fields = value_.getClass().getFields();
//        for (Field f : fields) {
        for(int i = 0; i<fields.length; i++){
            String name = fields[i].getName();
            
            if ('$' != name.charAt(0)) {
                continue;
            }
            name = name.substring(1);//remove '$'
            
            RubyValue v = getVariable(name);
            if (null != v) {
                try {
                	fields[i].set(value_, v);
                } catch (IllegalArgumentException e) {
                    throw new Error(e.toString());
                } catch (IllegalAccessException e) {
                    throw new Error(e.toString());
                }
            }
        }*/
    }

    public RubyValue call(RubyArray args) {
        setUpCallContext();
        return value_.invoke(value_.getSelf(), args);
    }
    
    public RubyValue call() {
        setUpCallContext();
        return value_.invoke(value_.getSelf());
    }
    
    public RubyValue call(RubyValue arg) {
        setUpCallContext();
        return value_.invoke(value_.getSelf(), arg);
    }
    
    //@RubyLevelMethod(name="call", alias="[]", alias="===", alias="yield")
    public static class Invoke extends RubyVarArgMethod {
        protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
            throw new Error("we overided invoke, so this method should never be called");
        }

        public RubyMethod clone(){
        	Invoke cl = new Invoke();
        	cl.doClone(this);
        	return cl;
        }
        
        public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block) {
            RubyValue v = ((RubyProc)receiver).call(args);
            RubyBlock anotherBlock = ((RubyProc)receiver).getBlock();
            if (null != anotherBlock) {
                v.setReturnedInBlock(anotherBlock.returned(), anotherBlock.breakedOrReturned(), !anotherBlock.createdByLambda());
            } else {
                v.setReturnedInBlock(false, false, false);
            }
            return v;
        }

        public RubyValue invoke(RubyValue receiver, RubyBlock block) {
        	RubyValue v = ((RubyProc)receiver).call();
        	RubyBlock anotherBlock = ((RubyProc)receiver).getBlock();
            if (null != anotherBlock) {
                v.setReturnedInBlock(anotherBlock.returned(), anotherBlock.breakedOrReturned(), !anotherBlock.createdByLambda());
            } else {
                v.setReturnedInBlock(false, false, false);
            }
            return v;
        }

        public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block) {
            RubyBlock anotherBlock = ((RubyProc)receiver).getBlock();
            RubyArray args = ObjectFactory.createArray(1, 0, anotherBlock.createdByLambda());
            args.add(arg);
            return this.invoke(receiver, args, anotherBlock);
        }
        
        public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
            RubyBlock anotherBlock = ((RubyProc)receiver).getBlock();
            RubyArray args = ObjectFactory.createArray(2, 0, anotherBlock.createdByLambda());
            args.add(arg0);
            args.add(arg1);
            return this.invoke(receiver, args, anotherBlock);
        }
    }
    
}
