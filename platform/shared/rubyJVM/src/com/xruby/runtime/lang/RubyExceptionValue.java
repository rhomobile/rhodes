/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

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
        loadBackTrace();
        GlobalVariables.set(this, "$!");
    }

    public RubyExceptionValue(RubyClass c, String message) {
        super(c);
        message_ = message;
        loadBackTrace();
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
    	return ObjectFactory.createString(this.message_);
    }
    
    public String toString() {
        return message_;
    }

    //@RubyLevelMethod(name="backtrace")
    public RubyArray backtrace() {
        return backTrace;
    }
        
    private void loadBackTrace(){
    	try
    	{
	        StackTraceElement[] trace = Thread.currentThread().getStackTrace();
	        for (StackTraceElement e : trace) {
            String s = e.getClassName();
	            
            	if ( s.startsWith("xruby.")) {
            		String fileName = e.getFileName();
            		String funcName = null;
            		
            		String[] classParts = s.split("\\$");
            		for ( int i = 0; i < classParts.length && funcName == null ; i++ ){
            			String[] subPaths = classParts[i].split("\\.");
            			
            			for ( int j = 0; j < subPaths.length && funcName == null; j++ ){
            				if ( fileName.indexOf(subPaths[j]) != -1 ) {
            					if ( j + 1 < subPaths.length )
            						funcName = subPaths[j+1];
            					else
            						funcName = subPaths[j];
            }
    }
            		}
            		
            		fileName = fileName.replaceAll("\\\\", "/");
            		fileName = fileName.replace("RhoBundle", "");

            		String traceStr = "at " + fileName + ":" + e.getLineNumber() + ": in `" + funcName + "`\r\n";
            		backTrace.add(ObjectFactory.createString(traceStr));
                }
	        }
    	}
    	catch(Exception e){
    		e.printStackTrace();
    	}
    }
    public Throwable getThrowable(){
        return throwable_;
    }
    
}
