/**
 * Copyright 2005-2007 Yu Zhang, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import j2me.util.Iterator;
import j2me.util.List;
//import java.util.Vector;
import j2me.util.ArrayList;

import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
//import com.xruby.runtime.lang.annotation.RubyLevelClass;
//import com.xruby.runtime.lang.annotation.RubyLevelConstant;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;

//import com.xruby.runtime.lang.RubyNoArgMethod;
//import com.xruby.runtime.lang.RubyClass;
//import com.xruby.runtime.lang.RubyBlock;

//@RubyLevelClass(name="ThreadGroup")
public class RubyThreadGroup extends RubyBasic{
    
    private boolean enclosed = false;
    private List/*<RubyThread>*/ threads_ = new ArrayList/*<RubyThread>*/();
    
    //@RubyLevelConstant(name="Default")
    public static RubyThreadGroup defaultThreadGroup = new RubyThreadGroup();   
    
    public RubyThreadGroup(){
        super(RubyRuntime.ThreadGroupClass);
    }
    
    protected void doClone(RubyValue orig){
    	RubyThreadGroup cl = (RubyThreadGroup)orig;
    	enclosed = cl.enclosed;
    	threads_ = cl.threads_;
    	super.doClone(orig);
    }
    
    //@RubyLevelMethod(name="new")
    public static RubyValue newThreadGroup(RubyValue receiver) {
    	return new RubyThreadGroup();
    }
    
    //@RubyLevelMethod(name="add")
    public RubyValue add(RubyValue arg) {
    	this.add((RubyThread)arg);
        return this;
    }
    
    public void add(RubyThread thread){
        if(thread.getThreadGroup() != RubyConstant.QNIL){
            RubyThreadGroup group = (RubyThreadGroup)thread.getThreadGroup();
            if(group.isEnclosed())
                throw new RubyException(RubyRuntime.ThreadErrorClass,"can't move from the enclosed thread group");
            group.threads_.remove(thread);
        }
        thread.setThreadGroup(this);
        threads_.add(thread);
    }
    
    //When a thread terminates,remove it from this group.
    public void remove(RubyThread thread){
        thread.setThreadGroup(null);
        threads_.remove(thread);
    }
    
    public List list(){
        return threads_;
    }
    
    //@RubyLevelMethod(name="list")
    public RubyArray listAll() {
    	RubyArray result = new RubyArray();
//    	for (RubyThread thread : this.threads_) {
        for (Iterator iter = this.threads_.iterator(); iter.hasNext();) {
        	RubyThread thread = (RubyThread)iter.next();
    		result.add(thread);
    	}
    	
    	return result;
    }
    
    //@RubyLevelMethod(name="enclose")
    public RubyValue enclose(){
        enclosed = true;
        return this;
    }
    
    public boolean isEnclosed() {
    	return this.enclosed;
    }
    
    //@RubyLevelMethod(name="enclosed?")
    public RubyValue enclosed_p() {
        return ObjectFactory.createBoolean(enclosed);
    }
    
}
