/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.*;

//@RubyLevelClass(name="Method")
public class RubyMethodValue extends RubyBasic {

    private RubyValue receiver_;
    private String name_;
    private RubyMethod method_;

    RubyMethodValue(RubyValue receiver, String name, RubyMethod method) {
        super(RubyRuntime.MethodClass);
        receiver_ = receiver;
        name_ = name;
        method_ = method;
    }

    public RubyValue clone(){
    	RubyMethodValue cl = new RubyMethodValue(this.receiver_, this.name_, this.method_);
    	cl.doClone(this);
    	return cl;
    }
    
    //@RubyLevelMethod(name="call")
    public RubyValue call(RubyArray args, RubyBlock block) {
    	return method_.invoke(receiver_, args, block);
    }

    //@RubyLevelMethod(name="to_proc")
    public RubyProc convertToRubyProc() {
        return ObjectFactory.createProc(method_.convertToRubyBolck(receiver_));
    }

    //@RubyLevelMethod(name="arity")
    public RubyFixnum arity() {
        return ObjectFactory.createFixnum(method_.arity());
    }

    //@RubyLevelMethod(name="bind")
    public RubyMethodValue bind(RubyValue receiver) {
		if (receiver.getRubyClass() != receiver_.getRubyClass()) {
			throw new RubyException(RubyRuntime.TypeErrorClass, "bind argument must be an instance of " + receiver_.getRubyClass().getName());
		}
		RubyMethodValue m = new RubyMethodValue(receiver, name_, method_);
		return m;
    }

    //@RubyLevelMethod(name="unbind")
    public RubyMethodValue unbind() {
		RubyMethodValue m = new RubyMethodValue(receiver_, name_, method_);
        m.setRubyClass(RubyRuntime.UnboundMethodClass);
		return m;
    }
    
    //@RubyLevelMethod(name="to_s", alias="inspect")
    public RubyString to_s() {
    	return ObjectFactory.createString(this.toString());
    }

    public String toString() {
        StringBuffer s = new StringBuffer("#<Method: ");
        s.append(receiver_.getRubyClass().getName());
        s.append("#");
        s.append(name_);
        s.append(">");
        return s.toString();
    }
    
}
