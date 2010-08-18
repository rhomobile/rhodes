/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.rho.RhoClassFactory;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyString;

/**
 * Java does not have multiple inheritance, and RubyException has to be inheritated from Exception.
 */
//@RubyLevelClass(name="Exception")
public class RubyExceptionValue extends RubyBasic {

    private RubyException exception_;
    private String message_ = "";
    private Throwable throwable_;

    RubyArray backTrace = new RubyArray();

    public RubyExceptionValue(RubyClass c) {
        super(c);
        try {
			RhoClassFactory.createRhoRubyHelper().loadBackTrace(this.backTrace);
		} catch (Exception e) {
			e.printStackTrace();
		}
        GlobalVariables.set(this, "$!");
    }

    public RubyExceptionValue(RubyClass c, String message) {
        super(c);
        message_ = message;
        try {
			RhoClassFactory.createRhoRubyHelper().loadBackTrace(this.backTrace);
		} catch (Exception e) {
			e.printStackTrace();
		}
        GlobalVariables.set(this, "$!");
    }

    public RubyExceptionValue(RubyClass c, String message, Throwable t) {
        super(c);
        message_ = message;
        throwable_ = t;
        GlobalVariables.set(this, "$!");
    }

    public RubyValue clone()
    {
    	RubyExceptionValue cl = new RubyExceptionValue(this.class_, this.message_,this.throwable_);
    	cl.setException(this.exception_);
    	cl.doClone(this);
    	return cl;
    }
    
    public void setMessage(String message) {
        message_ = message;
    }

    void setException(RubyException exception) {
        exception_ = exception;
    }
    
    //@RubyAllocMethod
    public static RubyExceptionValue alloc(RubyValue receiver) {
    	return new RubyExceptionValue((RubyClass)receiver);
    }
    
    //@RubyLevelMethod(name="initialize")
    public RubyExceptionValue initialize() {
    	this.setMessage("");
    	return this;
    }
    
    //@RubyLevelMethod(name="initialize")
    public RubyExceptionValue initialize(RubyValue v) {
    	this.setMessage(v.toStr());
    	return this;
    }
    
    //@RubyLevelMethod(name="to_s", alias={"message", "to_str"})
    public RubyString to_s() {
    	
    	String res = this.message_;

		if ( res == null || res.length() == 0 )
			res = this.getRubyClass().getName();
    	
		if ( res == null || res.length() == 0 )
			res = this.getClass().getName();
    	
    	return ObjectFactory.createString(res);
    }
    
    public String toString() {
        return message_;
    }

    //@RubyLevelMethod(name="backtrace")
    public RubyArray backtrace() {
        return backTrace;
    }

    public void addBacktrace(String strTrace)
    {
    	backTrace.add(ObjectFactory.createString(strTrace));
    }
    
    public Throwable getThrowable(){
        return throwable_;
    }
    
}
