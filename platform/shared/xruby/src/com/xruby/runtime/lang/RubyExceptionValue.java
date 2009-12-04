/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.annotation.RubyAllocMethod;
import com.xruby.runtime.lang.annotation.RubyLevelClass;
import com.xruby.runtime.lang.annotation.RubyLevelMethod;

/**
 * Java does not have multiple inheritance, and RubyException has to be inheritated from Exception.
 */
@RubyLevelClass(name="Exception")
public class RubyExceptionValue extends RubyBasic {

    private RubyException exception_;
    private String message_;
    private Throwable throwable_;

    public RubyExceptionValue(RubyClass c) {
        super(c);
        GlobalVariables.set(this, "$!");
    }

    public RubyExceptionValue(RubyClass c, String message) {
        super(c);
        message_ = message;
        GlobalVariables.set(this, "$!");
    }

    public RubyExceptionValue(RubyClass c, String message, Throwable t) {
        super(c);
        message_ = message;
        throwable_ = t;
        GlobalVariables.set(this, "$!");
    }

    public void setMessage(String message) {
        message_ = message;
    }

    void setException(RubyException exception) {
        exception_ = exception;
    }
    
    @RubyAllocMethod
    public static RubyExceptionValue alloc(RubyValue receiver) {
    	return new RubyExceptionValue((RubyClass)receiver);
    }
    
    @RubyLevelMethod(name="initialize")
    public RubyExceptionValue initialize() {
    	this.setMessage("");
    	return this;
    }
    
    @RubyLevelMethod(name="initialize")
    public RubyExceptionValue initialize(RubyValue v) {
    	this.setMessage(v.toStr());
    	return this;
    }
    
    @RubyLevelMethod(name="to_s", alias={"message", "to_str"})
    public RubyString to_s() {
    	return ObjectFactory.createString(this.message_);
    }
    
    public String toString() {
        return message_;
    }

    @RubyLevelMethod(name="backtrace")
    public RubyArray backtrace() {
        RubyArray a = new RubyArray();
        StackTraceElement[] trace = exception_.getStackTrace();
        for (StackTraceElement e : trace) {
            String s = e.getClassName();
            if ((!s.startsWith("com.xruby"))||
                s.startsWith("com.xruby.runtime.builtin")) {//filter internal calls
                a.add(ObjectFactory.createString(s));
            }
        }
        return a;
    }

    public Throwable getThrowable(){
        return throwable_;
    }
}
