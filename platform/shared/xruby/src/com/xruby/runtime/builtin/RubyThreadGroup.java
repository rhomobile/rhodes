/**
 * Copyright 2005-2007 Yu Zhang, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import java.util.List;
import java.util.Vector;

import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.annotation.RubyLevelClass;
import com.xruby.runtime.lang.annotation.RubyLevelConstant;
import com.xruby.runtime.lang.annotation.RubyLevelMethod;

@RubyLevelClass(name="ThreadGroup")
public class RubyThreadGroup extends RubyBasic{
    
    private boolean enclosed = false;
    private List<RubyThread> threads_ = new Vector<RubyThread>();
    
    @RubyLevelConstant(name="Default")
    public static RubyThreadGroup defaultThreadGroup = new RubyThreadGroup();   
    
    public RubyThreadGroup(){
        super(RubyRuntime.ThreadGroupClass);
    }
    
    @RubyLevelMethod(name="new")
    public static RubyValue newThreadGroup(RubyValue receiver) {
    	return new RubyThreadGroup();
    }
    
    @RubyLevelMethod(name="add")
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
    
    @RubyLevelMethod(name="list")
    public RubyArray listAll() {
    	RubyArray result = new RubyArray();
    	for (RubyThread thread : this.threads_) {
    		result.add(thread);
    	}
    	
    	return result;
    }
    
    @RubyLevelMethod(name="enclose")
    public RubyValue enclose(){
        enclosed = true;
        return this;
    }
    
    public boolean isEnclosed() {
    	return this.enclosed;
    }
    
    @RubyLevelMethod(name="enclosed?")
    public RubyValue enclosed_p() {
        return ObjectFactory.createBoolean(enclosed);
    }
}
