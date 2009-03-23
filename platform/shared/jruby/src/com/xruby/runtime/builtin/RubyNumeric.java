/**
 * Copyright 2007 Xue Yong Zhi, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.RubyAPI;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyID;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;

//@RubyLevelClass(name="Numeric")
public abstract class RubyNumeric extends RubyValue {
	//@RubyLevelMethod(name="+@")
	public RubyNumeric uplus() {
		return this;
	}
	
	//@RubyLevelMethod(name="-@")
	public RubyValue uminus() {
		RubyArray array = ObjectFactory.FIXNUM0.doDoerce(this, true);
		return RubyAPI.callOneArgMethod(array.get(0), array.get(1), null, RubyID.subID);
	}
	
	//@RubyLevelMethod(name="step")
	public final RubyValue step(RubyValue v, RubyBlock block) {
		return doStep(v, ObjectFactory.FIXNUM1, block);
	}
	
	//@RubyLevelMethod(name="step")
	public final RubyValue step(RubyValue toArg, RubyValue stepArg, RubyBlock block) {
		if (ObjectFactory.FIXNUM0.equals(stepArg)) {
			throw new RubyException(RubyRuntime.ArgumentErrorClass, "step can't be 0");
		}
		
		return doStep(toArg, stepArg, block);
	}
	
	protected RubyValue doStep(RubyValue toArg, RubyValue stepArg, RubyBlock block) {
		throw new RubyException("not implemented!");
	}
	
	//@RubyLevelMethod(name="coerce")
	public RubyArray coerce(RubyValue v) {
		if (this.getRubyClass() == v.getRubyClass()) {
			return new RubyArray(v, this);
		}
		
		return new RubyArray(v.toRubyFloat(), this.toRubyFloat());
	}
	
	protected final RubyValue coerceBin(RubyID id, RubyValue v) {
		RubyArray array = doDoerce(v, true);
		return RubyAPI.callOneArgMethod(array.get(0), array.get(1), null, id);
	}
	
	protected final RubyValue coerceRelop(RubyID id, RubyValue v) {
		RubyArray array = this.doDoerce(v, false);
		if (array != null) {
			RubyValue result = RubyAPI.callOneArgMethod(array.get(0), array.get(1), null, id);
			if (result != RubyConstant.QNIL) {
				return result;
			}
		}
		
		throw new RubyException(RubyRuntime.ArgumentErrorClass, 
				"comparison of " + this.getRubyClass().getName() + " with " + v.getRubyClass().getName() + " failed");
	}
	
	protected final RubyValue coerceCmp(RubyID id, RubyValue v) {
		RubyArray array = doDoerce(v, false);
		if (array != null) {
			return RubyAPI.callOneArgMethod(array.get(0), array.get(1), null, id);
		}
		
		return RubyConstant.QNIL;
	}
	
	protected final RubyArray doDoerce(RubyValue v, boolean err) {
		RubyValue result;
        try {
            result = coerceBody(v);
        } catch (RubyException e) {
        	if (err) {
        		throw new RubyException(RubyRuntime.TypeErrorClass,	
        				v.getRubyClass().getName() + " can't be coerced into " + this.getRubyClass().getName() + ":" + e);
        	}
        	return null;
        }
    
        if (!(result instanceof RubyArray) || ((RubyArray) result).size() != 2) {
        	throw new RubyException(RubyRuntime.TypeErrorClass, "coerce must return [x, y]");
        }
        
        return (RubyArray) result;
	}
	
	private RubyValue coerceBody(RubyValue v) {
		return RubyAPI.callOneArgMethod(v, this, null, RubyID.coerceID);
	}
	
	protected void zeroDiv() {
		throw new RubyException(RubyRuntime.ZeroDivErrorClass, "divided by 0");
	}
}
