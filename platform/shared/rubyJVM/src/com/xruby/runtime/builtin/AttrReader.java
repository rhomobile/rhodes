/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyID;
import com.xruby.runtime.lang.RubyNoArgMethod;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.RubyMethod;

public class AttrReader extends RubyNoArgMethod {
    private RubyID attrName;

    public AttrReader(String methodName) {
        attrName = RubyID.intern("@" + methodName);
    }
    
    public AttrReader(RubyID id) {
        attrName = RubyID.intern("@" + id.toString());
    }

    protected RubyValue run(RubyValue receiver, RubyBlock block) {
        return receiver.getInstanceVariable(attrName);
    }
    
    public RubyMethod clone(){
    	AttrReader cl = new AttrReader("");
    	cl.doClone(this);
    	return cl;
    }
    
    protected void doClone(RubyMethod orig){
    	attrName = ((AttrReader)orig).attrName;
    	
    	super.doClone(orig);
    }
}
