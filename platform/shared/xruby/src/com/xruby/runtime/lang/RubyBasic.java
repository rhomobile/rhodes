/** 
 * Copyright 2006-2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

//TODO some stuffs in RubyValue should be moved into here.
public abstract class RubyBasic extends RubyValue {
    protected RubyClass class_;
    
    public RubyBasic(RubyClass c) {
		this.class_ = c;
	}
    public RubyBasic() {
	}

    public void setRubyClass(RubyClass c) {
        class_ = c;
    }

    public RubyClass getRubyClass() {
        return class_;
    }
    
    protected void doClone(RubyBasic orig)
    {
    	class_ = orig.class_;
    	super.doClone(orig);
    }
    
}
