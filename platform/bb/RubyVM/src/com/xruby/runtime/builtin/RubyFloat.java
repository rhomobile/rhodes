/** 
 * Copyright 2005-2007 Xue Yong Zhi, Jie Li, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import j2me.math.HugeDigit;
import j2me.lang.MathEx;
//import com.xruby.runtime.lang.RubyOneArgMethod;
//import com.xruby.runtime.lang.RubyBlock;

import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyID;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
//import com.xruby.runtime.lang.annotation.RubyLevelClass;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;

////@RubyLevelClass(name="Float", superclass="Numeric")
public class RubyFloat extends RubyNumeric {
	private RubyClass klass;
	private double value_;
	
	RubyFloat(double value) {
		value_ = value;
		this.klass = RubyRuntime.FloatClass;
	}
	
	public RubyValue clone()
	{
		RubyFloat cl = new RubyFloat(this.value_);
		cl.doClone(this);
		return cl;
	}
	
	public RubyClass getRubyClass() {
		return this.klass;
	}

	public void setRubyClass(RubyClass klass) {
		this.klass = klass;
	}
	
	public boolean equals(Object o) {
		if (this == o) {
			return true;
		} else if (o instanceof RubyFloat) {
			return ((RubyFloat)o).value_ == this.value_;
		} else {
			return super.equals(o);
		}
	}
	
	public String toString() {
		return Double.toString(this.value_);
	}

	public double doubleValue() {
		return value_;
	}
	
	public int toInt() {
		return (int)this.value_;
	}

	public long toLong() {
		return (long)this.value_;
	}
	
	public double toFloat() {
		return this.value_;
	}

	public RubyFloat toRubyFloat() {
		return this;
	}
	
	public RubyInteger toRubyInteger() {
		return RubyBignum.bignorm((long)this.value_);
	}

	//@RubyLevelMethod(name="to_s")
	public RubyString to_s() {
		return ObjectFactory.createString(Double.toString(this.value_));
	}
	
	//@RubyLevelMethod(name="to_i")
	public RubyFixnum to_i() {
		return ObjectFactory.createFixnum((long)this.value_);
	}
	
	//@RubyLevelMethod(name="coerce")
	public RubyArray coerce(RubyValue v) {
		return new RubyArray(v.toRubyFloat(), this);
	}
	
	//@RubyLevelMethod(name="+")
	public RubyValue opPlus(RubyValue v) {
		if (v instanceof RubyFixnum) {
			return ObjectFactory.createFloat(this.value_ + v.toFloat());
		} else if (v instanceof RubyBignum) {
			return ObjectFactory.createFloat(this.value_ + v.toFloat());
		} else if (v instanceof RubyFloat) {
			return ObjectFactory.createFloat(this.value_ + ((RubyFloat)v).value_);
		} else {
			return this.coerceBin(RubyID.plusID, v);
		}
	}
	
	//@RubyLevelMethod(name="-")
	public RubyValue opMinus(RubyValue v) {
		if (v instanceof RubyFixnum) {
			return ObjectFactory.createFloat(this.value_ - v.toFloat());
		} else if (v instanceof RubyBignum) {
			return ObjectFactory.createFloat(this.value_ - v.toFloat());
		} else if (v instanceof RubyFloat) {
			return ObjectFactory.createFloat(this.value_ - ((RubyFloat)v).value_);
		} else {
			return this.coerceBin(RubyID.subID, v);
		}
	}
	
	//@RubyLevelMethod(name="*")
	public RubyValue opMul(RubyValue v) {	
		if (v instanceof RubyFixnum) {
			return ObjectFactory.createFloat(this.value_ * v.toFloat());
		} else if (v instanceof RubyBignum) {
			return ObjectFactory.createFloat(this.value_ * v.toFloat());
		} else if (v instanceof RubyFloat) {
			return ObjectFactory.createFloat(this.value_ * ((RubyFloat)v).value_);
		} else {
			return this.coerceBin(RubyID.mulID, v);
		}
	}
	
	//@RubyLevelMethod(name="/")
	public RubyValue opDiv(RubyValue v) {	
		if (v instanceof RubyFixnum) {
			return ObjectFactory.createFloat(this.value_ / v.toFloat());
		} else if (v instanceof RubyBignum) {
			return ObjectFactory.createFloat(this.value_ / v.toFloat());
		} else if (v instanceof RubyFloat) {
			return ObjectFactory.createFloat(this.value_ / ((RubyFloat)v).value_);
		} else {
			return this.coerceBin(RubyID.divID, v);
		}
	}
	
	//@RubyLevelMethod(name="%")
	public RubyValue opMod(RubyValue v) {
		double fy;
		if (v instanceof RubyFixnum) {
			fy = v.toFloat();
		} else if (v instanceof RubyBignum) {
			fy = v.toFloat();
		} else if (v instanceof RubyFloat) {
			fy = ((RubyFloat)v).value_;
		} else {
			return coerceBin(RubyID.modID, v);
		}
		
		return ObjectFactory.createFloat(this.value_ % fy);
	}
	
	//@RubyLevelMethod(name="<=>")
	public RubyValue cmp(RubyValue v) {
		double a = this.value_;
		double b;
		if (v instanceof RubyFixnum) {
			b = v.toFloat();
		} else if (v instanceof RubyBignum) {
			b = v.toFloat();
		} else if (v instanceof RubyFloat) {
			b = ((RubyFloat)v).value_;
		} else {
			return this.coerceCmp(RubyID.unequalID, v);
		}
		
		if (Double.isNaN(a) || Double.isNaN(b)) {
            return RubyConstant.QNIL;
        } else if (a == b) {
			return ObjectFactory.FIXNUM0;
		} else if (a > b) {
			return ObjectFactory.FIXNUM1;
		} else {
			return ObjectFactory.FIXNUM_NEGATIVE_ONE;
		}
	}
	
	//@RubyLevelMethod(name="floor")
	public RubyInteger floor() {
		return createRubyInteger(Math.floor(this.value_));
	}
	
	//@RubyLevelMethod(name="ceil")
	public RubyInteger ceil() {
		return createRubyInteger(Math.ceil(this.value_));
	}
	
	//@RubyLevelMethod(name="truncate")
	public RubyInteger truncate() {
		return createRubyInteger(this.value_);
	}
	
	//@RubyLevelMethod(name="round")
	public RubyInteger round() {
		return createRubyInteger(MathEx.round(this.value_));
	}
	
	//@RubyLevelMethod(name="abs")
	public RubyFloat abs() {
		return ObjectFactory.createFloat(Math.abs(this.value_));
	}

	private RubyInteger createRubyInteger(double value) {
		if (value <= Long.MAX_VALUE && value >= Long.MIN_VALUE) {
			return ObjectFactory.createFixnum((long) value);
		}
		return ObjectFactory.createBignum(HugeDigit.valueOf(value).toBigInteger());
	}
	
	//@RubyLevelMethod(name="finite?")
	public RubyValue finite_p() {
		return ObjectFactory.createBoolean(!Double.isInfinite(this.value_));
	}
	
	//@RubyLevelMethod(name="infinite?")
	public RubyValue infinite_p() {
		if (this.value_ == Double.NEGATIVE_INFINITY) {
            return ObjectFactory.FIXNUM_NEGATIVE_ONE;
        } else if (this.value_ == Double.POSITIVE_INFINITY) {
            return ObjectFactory.FIXNUM1;
        } else {
            return RubyConstant.QNIL;
        }
	}
	
	//@RubyLevelMethod(name="nan?")
	public RubyValue nan_p() {
		return ObjectFactory.createBoolean(Double.isNaN(this.value_));
	}
	
    //@RubyLevelMethod(name="**")
    public RubyValue pow(RubyValue v) {
    	if (v instanceof RubyFixnum) {
    		long p = ((RubyFixnum)v).toLong();
    		if (p == 0) {
    			return ObjectFactory.FIXNUM1;
    		} else if (p == 1) {
    			return this;
    		}
    		//if (p > 0) {
    		//	HugeInt b = HugeInt.valueOf(this.value_);
    		//	return RubyBignum.bignorm(b.pow(p));
    		//}
    		
    		return ObjectFactory.createFloat(MathEx.pow(this.value_, p));
    	} else if (v instanceof RubyFloat) {
    		return ObjectFactory.createFloat(MathEx.pow(this.value_, v.toFloat()));
    	}
    	
    	return coerceBin(RubyID.powID, v);
    }
	
}