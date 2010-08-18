/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng, Jie Li
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import j2me.math.HugeInt;
import j2me.lang.MathEx;

import com.xruby.runtime.lang.*;

////@RubyLevelClass(name="Fixnum", superclass="Integer")
public class RubyFixnum extends RubyInteger {
    private final long value_;

    RubyFixnum(int i) {
        value_ = i;
    }
    RubyFixnum(long i) {
        value_ = i;
    }
    
    public void setRubyClass(RubyClass klass) {
		throw new RubyException(RubyRuntime.TypeErrorClass, this.getRubyClass().getName() + " can't be set class");
	}
	
	public RubyClass getSingletonClass() {
    	throw new RubyException(RubyRuntime.TypeErrorClass, this.getRubyClass().getName() + " can't define singleton");
    }

	public RubyValue clone() {
		throw new RubyException(RubyRuntime.TypeErrorClass, "can't clone " + this.getRubyClass().getName());
	}
    
	public int toInt() {
		return (int)value_;
	}
	public long toLong() {
		return value_;
	}
	
	public double toFloat() {
		return this.value_;
	}
	
	public RubyFloat toRubyFloat() {
		return ObjectFactory.createFloat(this.value_);
	}

	public RubyFixnum convertToInteger() {
		return this;
    }

	public int hashCode() {
        return (int)value_;
    }

    public RubyClass getRubyClass() {
        return RubyRuntime.FixnumClass;
    }

    public boolean equals(Object o) {
        if (this == o) {
            return true;
        } else if (o instanceof RubyFixnum) {
            return value_ == ((RubyFixnum)o).value_;
        } else {
            return super.equals(o);
        }
    }

    public String toString() {
        return Long.toString(value_);
    }

    public String toString(int radix) {
        return Long.toString(value_, radix);
    }
    
    //@RubyLevelMethod(name="-@")
    public RubyValue uminus() {
    	return ObjectFactory.createFixnum(-this.value_);
    }
    
    //@RubyLevelMethod(name="+")
    public RubyValue opPlus(RubyValue v) {
    	if (v instanceof RubyFixnum) {
    		return RubyBignum.bignorm((long)this.value_ + (long)((RubyFixnum)v).value_);
    	}
    	
    	if (v instanceof RubyFloat) {
            return ObjectFactory.createFloat(value_ + v.toFloat());
    	}
    	
    	if (v instanceof RubyBignum) {
    		HugeInt bigValue1 = HugeInt.valueOf(value_);
    		HugeInt bigValue2 = ((RubyBignum)v).getInternal();
    		return RubyBignum.bignorm(bigValue1.add(bigValue2));
    	}
    	
    	return coerceBin(RubyID.plusID, v);
    }
    
    //@RubyLevelMethod(name="-")
    public RubyValue opMinus(RubyValue v) {
    	if (v instanceof RubyFixnum) {
    		return RubyBignum.bignorm((long)this.value_ - (long)((RubyFixnum)v).value_);
    	}
    	
    	if (v instanceof RubyFloat) {
    		return ObjectFactory.createFloat(this.value_ - v.toFloat());
    	}
    	
    	if (v instanceof RubyBignum) {
    		HugeInt bigValue1 = HugeInt.valueOf(this.value_);
            HugeInt bigValue2 = ((RubyBignum)v).getInternal();
            return RubyBignum.bignorm(bigValue1.subtract(bigValue2));
    	}
    	
    	return coerceBin(RubyID.subID, v);
    }
    
    //@RubyLevelMethod(name="*")
    public RubyValue opMul(RubyValue v) {
    	if (v instanceof RubyFixnum) {
    		return RubyBignum.bignorm((long)this.value_ * (long)((RubyFixnum)v).value_);
    	}
    	
    	if (v instanceof RubyFloat) {
    		return ObjectFactory.createFloat(this.value_ * v.toFloat());
    	}
    	
    	if (v instanceof RubyBignum) {
    		HugeInt bigValue1 = HugeInt.valueOf(this.value_);
            HugeInt bigValue2 = ((RubyBignum)v).getInternal();
            return RubyBignum.bignorm(bigValue1.multiply(bigValue2));
    	}
    	
    	return coerceBin(RubyID.mulID, v);
    }
    
    //@RubyLevelMethod(name="/")
    public RubyValue opDiv(RubyValue v) {
    	if (v instanceof RubyFixnum) {
    		long intValue1 = this.value_;
            long intValue2 = ((RubyFixnum)v).value_;
            if (intValue2 == 0) {
            	zeroDiv();
            }
    		long div = intValue1 / intValue2;
            long mod = intValue1 - div * intValue2;
            if (mod != 0 && div < 0) {
                --div;
            }
            return RubyBignum.bignorm(div);
    	} else if (v instanceof RubyFloat) {
    		return ObjectFactory.createFloat(this.value_ / v.toFloat());
    	} else if (v instanceof RubyBignum) {
    		HugeInt bigValue1 = HugeInt.valueOf(this.value_);
            HugeInt bigValue2 = ((RubyBignum)v).getInternal();
            return RubyBignum.bignorm(bigValue1.divide(bigValue2));
    	} else {
    		return coerceBin(RubyID.divID, v);
    	}
    }
    
    //@RubyLevelMethod(name="%")
    public RubyValue mod(RubyValue v) {
    	if (v instanceof RubyFixnum) {
    		return RubyBignum.bignorm(this.value_ % ((RubyFixnum)v).value_);
    	}
    	
    	return coerceBin(RubyID.modID, v);
    }
    
    //@RubyLevelMethod(name="**")
    public RubyValue pow(RubyValue v) {
    	if (v instanceof RubyFixnum) {
    		long p = ((RubyFixnum)v).value_;
    		if (p == 0) {
    			return ObjectFactory.FIXNUM1;
    		} else if (p == 1) {
    			return this;
    		}
    		if (p > 0) {
    			HugeInt b = HugeInt.valueOf(this.value_);
    			return RubyBignum.bignorm(b.pow(p));
    		}
    		
    		return ObjectFactory.createFloat(MathEx.pow(this.value_, p));
    	} else if (v instanceof RubyFloat) {
    		return ObjectFactory.createFloat(MathEx.pow(this.value_, v.toFloat()));
    	}
    	
    	return coerceBin(RubyID.powID, v);
    }
    
    
    //@RubyLevelMethod(name="~")
    public RubyValue opRev() {
    	return RubyBignum.bignorm(~this.value_);
    }
    
    //@RubyLevelMethod(name="to_f")
    public RubyFloat convertToFloat() {
    	return ObjectFactory.createFloat(this.value_);
    }
    
    //@RubyLevelMethod(name="<<")
    public RubyValue lshift(RubyValue arg) {
    	return lshift(arg.toInt());
    }
    
    //@RubyLevelMethod(name=">>")
    public RubyValue rshift(RubyValue arg) {
    	return rshift(arg.toInt());
    }
    
    private static int BIT_SIZE = 32;
    
    private RubyValue lshift(int i) {
    	if (i == 0) {
    		return this;
    	} else if (i < 0) {
    		return rshift(-i);
    	}
    	// FIXME: TO BE IMPROVED
    	HugeInt bigValue1 = HugeInt.valueOf(this.value_);
        return RubyBignum.bignorm(bigValue1.shiftLeft(i));
    }

	private RubyValue rshift(int i) {
		if (i == 0) {
    		return this;
    	} else if (i < 0) {
    		return lshift(-i);
    	}
		
		if (i >= BIT_SIZE - 1) {
    		if (this.value_ < 0) {
    			return ObjectFactory.FIXNUM_NEGATIVE_ONE;
    		}
    		
    		return ObjectFactory.FIXNUM0;
    	}
    	
    	return ObjectFactory.createFixnum(this.value_ >> i);
	}
	
	//@RubyLevelMethod(name="==")
	public RubyValue opEqual(RubyValue arg) {
		if (arg == this) {
			return RubyConstant.QTRUE;
		}
		
		if (arg instanceof RubyFixnum) {
			return ObjectFactory.createBoolean(this.value_ == ((RubyFixnum)arg).value_);
		}
		
		return RubyAPI.callOneArgMethod(arg, this, null, RubyID.equalID);
	}
	
	//@RubyLevelMethod(name="<=")
	public RubyValue opLe(RubyValue v) {
		if (v instanceof RubyFixnum) {
			return ObjectFactory.createBoolean(this.value_ <= ((RubyFixnum)v).value_);
		} else if (v instanceof RubyFloat) {
            return ObjectFactory.createBoolean(this.value_ <= v.toFloat());
		}
		
		return coerceRelop(RubyID.leID, v);
	}
	
	//@RubyLevelMethod(name="<")
	public RubyValue opLt(RubyValue v) {
		if (v instanceof RubyFixnum) {
			return ObjectFactory.createBoolean(this.value_ < ((RubyFixnum)v).value_);
		} else if (v instanceof RubyFloat) {
            return ObjectFactory.createBoolean(this.value_ < v.toFloat());
		}
		
		return coerceRelop(RubyID.ltID, v);
	}
	
	//@RubyLevelMethod(name=">=")
	public RubyValue opGe(RubyValue v) {
		if (v instanceof RubyFixnum) {
			return ObjectFactory.createBoolean(this.value_ >= ((RubyFixnum)v).value_);
		} else if (v instanceof RubyFloat) {
            return ObjectFactory.createBoolean(this.value_ >= v.toFloat());
		}
		
		return coerceRelop(RubyID.geID, v);
	}
	
	//@RubyLevelMethod(name=">")
	public RubyValue opGt(RubyValue v) {
		if (v instanceof RubyFixnum) {
			return ObjectFactory.createBoolean(this.value_ > ((RubyFixnum)v).value_);
		} else if (v instanceof RubyFloat) {
            return ObjectFactory.createBoolean(this.value_ > v.toFloat());
		}
		
		return coerceRelop(RubyID.gtID, v);
	}
	
	//@RubyLevelMethod(name="<=>")
	public RubyValue opCmp(RubyValue v) {
		if (this == v) {
			return ObjectFactory.FIXNUM0;
		}
		
		if (v instanceof RubyFixnum) {
			long a = ((RubyFixnum)v).value_;
			if (this.value_ > a) {
				return ObjectFactory.FIXNUM1;
			} else if (this.value_ == a) {
				return ObjectFactory.FIXNUM0;
			} else {
				return ObjectFactory.FIXNUM_NEGATIVE_ONE;
			}
		}
		
		return coerceCmp(RubyID.unequalID, v);
	}
	
	//@RubyLevelMethod(name="|")
	public RubyValue opOr(RubyValue v) {
		if (v instanceof RubyBignum) {
			return ((RubyBignum)v).op_bor(this);
		}
		
		return RubyBignum.bignorm(this.value_ | v.toLong());
	}
	
	//@RubyLevelMethod(name="&")
	public RubyValue opAnd(RubyValue v) {
		if (v instanceof RubyBignum) {
			return ((RubyBignum)v).op_band(this);
		}
		
		return RubyBignum.bignorm(this.value_ & v.toLong());
	}
	
	//@RubyLevelMethod(name="^")
	public RubyValue opXor(RubyValue v) {
		if (v instanceof RubyBignum) {
			return ((RubyBignum)v).op_bxor(this);
		}
		
		return RubyBignum.bignorm(this.value_ ^ v.toLong());
	}
	
	//@RubyLevelMethod(name="to_s")
	public RubyString to_s() {
		return ObjectFactory.createString(this.toString());
	}
	
	//@RubyLevelMethod(name="to_s")
	public RubyString to_s(RubyValue v) {
		 int radix = v.toInt();
         if (radix < 2 || radix > 36) {
             throw new RubyException(RubyRuntime.ArgumentErrorClass, "illegal radix " + radix);
         }
         
         return ObjectFactory.createString(this.toString(radix));
	}
	
	//@RubyLevelMethod(name="quo")
	public RubyFloat quo(RubyValue v) {
        if (v instanceof RubyFixnum) {
        	return ObjectFactory.createFloat(this.value_ / ((RubyFixnum)v).value_);
        }
        
        // FIXME: should be coerced.
        throw new RubyException(RubyRuntime.TypeErrorClass, v.getRubyClass().getName() + " can't be coersed into Fixnum");
	}
	
	//@RubyLevelMethod(name="[]")
	public RubyFixnum aref(RubyValue idx) {
		if (idx instanceof RubyBignum) {
			idx = RubyBignum.bignorm(idx);
			if (!(idx instanceof RubyFixnum)) {
				if (this.value_ > 0 || ((RubyBignum)idx).getInternal().compareTo(HugeInt.ZERO) > 0) {
					return ObjectFactory.FIXNUM0;
				} else {
					return ObjectFactory.FIXNUM1;
				}
			}
		}
		
		int i = idx.toInt();
		if (i < 0 || i > BIT_SIZE) {
			return ObjectFactory.FIXNUM0;
		}
		
		if ((this.value_ & (1l << i)) > 0) {
			return ObjectFactory.FIXNUM1;
		}
		
		return ObjectFactory.FIXNUM0;
	}

	protected RubyValue doStep(RubyValue toArg, RubyValue stepArg, RubyBlock block) {
		if ((toArg instanceof RubyFixnum) && (stepArg instanceof RubyFixnum)) {
			long i = this.value_;
			long end = ((RubyFixnum)toArg).value_;
			long diff = ((RubyFixnum)stepArg).value_;
			if (diff > 0) {
				while (i <= end) {
					RubyValue v = block.invoke(this, ObjectFactory.createFixnum(i));
                    if (block.breakedOrReturned()) {
                        return v;
                    }
                    i += diff;
				}
			} else {
				while (i >= end) {
					RubyValue v = block.invoke(this, ObjectFactory.createFixnum(i));
                    if (block.breakedOrReturned()) {
                        return v;
                    }
                    i += diff;
				}
			}
			
			return this;
		}
		
		return super.doStep(toArg, stepArg, block);
	}

	public RubyValue times(RubyBlock block) {
		long value = this.value_;
        for (int i = 0; i < value; ++i) {
            RubyValue v = block.invoke(this, ObjectFactory.createFixnum(i));
            if (block.breakedOrReturned()) {
                return v;
            }
        }
        
        return this;
	}

}