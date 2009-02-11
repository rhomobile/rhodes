/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyID;
import com.xruby.runtime.lang.RubyMethod;
import com.xruby.runtime.lang.RubyOneArgMethod;
import com.xruby.runtime.lang.RubyValue;

public class AttrWriter extends RubyOneArgMethod {
    private RubyID attrName;

    public AttrWriter(String methodName) {
        attrName = RubyID.intern("@" + methodName);
    }
    
    public AttrWriter(RubyID id) {
        attrName = RubyID.intern("@" + id.toString());
    }

    protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block) {
        return receiver.setInstanceVariable(arg, attrName);
    }
    
    protected void doClone(RubyMethod orig){
    	attrName = ((AttrWriter)orig).attrName;
    	
    	super.doClone(orig);
    }
    
}

