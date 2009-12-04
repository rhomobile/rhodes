/**
 * Copyright 2006-2007 Jie Li
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import java.math.BigDecimal;
import java.math.BigInteger;

import com.xruby.runtime.lang.*;
import com.xruby.runtime.lang.annotation.RubyLevelClass;
import com.xruby.runtime.lang.annotation.RubyLevelMethod;

@RubyLevelClass(name="Bignum", superclass="Integer")
public class RubyBignum extends RubyInteger {
    private static final BigInteger FIXNUM_MAX = BigInteger.valueOf(Integer.MAX_VALUE);
    private static final BigInteger FIXNUM_MIN = BigInteger.valueOf(Integer.MIN_VALUE);
    private BigInteger value_;
    private RubyClass klass;

    RubyBignum(String value, int radix) {
        this(new BigInteger(value, radix));
    }

    RubyBignum(String value) {
        this(new BigInteger(value));
    }

    RubyBignum(BigInteger value) {
        value_ = value;
        this.klass = RubyRuntime.BignumClass;
    }
    
	public RubyClass getRubyClass() {
		return this.klass;
	}

	public void setRubyClass(RubyClass klass) {
		this.klass = klass;
	}
    
    public int toInt() {
		return this.value_.intValue();
	}
    
	public double toFloat() {
		return this.value_.doubleValue();
	}
	
	public RubyFloat toRubyFloat() {
		return ObjectFactory.createFloat(this.value_.doubleValue());
	}

	@RubyLevelMethod(name="to_s")
	public RubyString to_s() {
        return ObjectFactory.createString(value_.toString(10));
    }

	@RubyLevelMethod(name="to_s")
    public RubyString to_s(RubyValue radix) {
        return ObjectFactory.createString(value_.toString(radix.toInt()));
    }
	
	@RubyLevelMethod(name="to_f")
	public RubyFloat to_f() {
		return ObjectFactory.createFloat(this.value_.doubleValue());
	}

    public BigInteger getInternal() {
        return value_;
    }

    public boolean sign() {
    	return value_.signum() >= 0;
    }
	
	public long longValue() {
		return value_.longValue();
	}

    public void setValue(BigInteger value) {
        value_ = value;
    }
    
    @RubyLevelMethod(name="size")
    public RubyFixnum rubySize() {
    	return ObjectFactory.createFixnum(size());
    }

    public int size() {
        int alignedBytesCount = (((value_.bitLength() - 1) & -32) >> 3) + 4;
        return alignedBytesCount;
    }
    
    @RubyLevelMethod(name="coerce")    
	public RubyArray coerce(RubyValue v) {
		if (v instanceof RubyFixnum) {
			return new RubyArray(new RubyBignum(BigInteger.valueOf(v.toInt())), this);
		} else if (v instanceof RubyBignum) {
			return new RubyArray(v, this);
		} 
		
		throw new RubyException(RubyRuntime.TypeErrorClass, 
				"can't coerce " + v.getRubyClass().getName() + " to Bignum");
	}
    
    @RubyLevelMethod(name="-@")
    public RubyBignum uminus() {
    	return ObjectFactory.createBignum(this.value_.negate());
    }
    
    @RubyLevelMethod(name="~")
    public RubyValue op_neg() {
    	return bignorm(this.value_.not());
    }
    
    @RubyLevelMethod(name="+")
    public RubyValue op_add(RubyValue value) {
        if (value instanceof RubyBignum) {
        	RubyBignum bigValue = (RubyBignum) value;
        	return bignorm(value_.add(bigValue.value_));
        } else if (value instanceof RubyFixnum) {
            BigInteger bigValue = BigInteger.valueOf(value.toInt());
            return bignorm(value_.add(bigValue));
        } else if (value instanceof RubyFloat) {
            double ret = value_.doubleValue() + value.toFloat();
            return ObjectFactory.createFloat(ret);
        } else {
        	return coerceBin(RubyID.plusID, value);
        }
    }

	@RubyLevelMethod(name="-")
    public RubyValue op_sub(RubyValue value) {
        if (value instanceof RubyBignum) {
            RubyBignum bigValue = (RubyBignum) value;
            return bignorm(value_.subtract(bigValue.value_));
        } else if (value instanceof RubyFixnum) {
            BigInteger bigValue = BigInteger.valueOf(value.toInt());
            return bignorm(value_.subtract(bigValue));
        } else if (value instanceof RubyFloat) {
            double ret = value_.doubleValue() - value.toFloat();
            return ObjectFactory.createFloat(ret);
        } else {
        	return coerceBin(RubyID.subID, value);
        }
    }

	@RubyLevelMethod(name="*")
	public RubyValue op_mul(RubyValue value) {
        if (value instanceof RubyBignum) {
            RubyBignum bigValue = (RubyBignum) value;
            return bignorm(value_.multiply(bigValue.value_));
        } else if (value instanceof RubyFixnum) {
            BigInteger bigValue = BigInteger.valueOf(value.toInt());
            return bignorm(value_.multiply(bigValue));
        } else if (value instanceof RubyFloat) {
            double ret = value_.doubleValue() * value.toFloat();
            return ObjectFactory.createFloat(ret);
        } else {
        	return coerceBin(RubyID.mulID, value);
        }
    }
	
	@RubyLevelMethod(name="/")
    public RubyValue op_div(RubyValue value) {
        if (value instanceof RubyBignum) {
            RubyBignum bigValue = (RubyBignum) value;
            if (bigValue.equals(BigInteger.ZERO)) {
            	zeroDiv();
            }
            return bignorm(value_.divide(bigValue.value_));
        } else if (value instanceof RubyFixnum) {
            int intValue = value.toInt();
            if (intValue == 0) {
            	zeroDiv();
            }
			BigInteger bigValue = BigInteger.valueOf(intValue);
            return bignorm(value_.divide(bigValue));
        } else if (value instanceof RubyFloat) {
            double ret = value_.doubleValue() / value.toFloat();
            return ObjectFactory.createFloat(ret);
        } else {
        	return coerceBin(RubyID.divID, value);
        }
    }
	
	@RubyLevelMethod(name="quo")
	public RubyValue quo(RubyValue v) {
		double dx = this.value_.doubleValue();
		double dy;
		if (v instanceof RubyFixnum) {
			dy = v.toInt();
		} else if (v instanceof RubyBignum) {
			dy = v.toFloat();
		} else if (v instanceof RubyFloat) {
			dy = v.toFloat();
		} else {
			return coerceBin(RubyID.quoID, v);
		}
		
		return ObjectFactory.createFloat(dx / dy);
	}
	
	@RubyLevelMethod(name="**")
	public RubyValue pow(RubyValue v) {
		if (v instanceof RubyFixnum) {
			if (v.toInt() == 0) {
				return ObjectFactory.FIXNUM1;
			}
		}
		
		double d;
		
		if (v instanceof RubyFloat) {
			d = v.toFloat();
		} else if (v instanceof RubyBignum) {
			// TODO: rb_warn("in a**b, b may be too big");
			d = v.toFloat();
		} else if (v instanceof RubyFixnum) {
			int yy = v.toInt();
			if (yy > 0) {
                if (this.value_.bitLength() * yy > 1024 * 1024) {
                    // TODO: output wraning: "in a**b, b may be too big"
                } else {
                    return bignorm(this.value_.pow(yy));
                }
            }
			
			d = yy;
		} else {
			return coerceBin(RubyID.powID, v);
		}
		
		return ObjectFactory.createFloat(Math.pow(this.value_.doubleValue(), d));
	}

	@RubyLevelMethod(name="%")
    public RubyValue op_mod(RubyValue value) {
        if (value instanceof RubyBignum) {
            RubyBignum bigValue = (RubyBignum) value;
            return bignorm(ruby_mod(value_, bigValue.value_));
        } else if (value instanceof RubyFixnum) {
            BigInteger bigValue = BigInteger.valueOf(value.toInt());
            return bignorm(ruby_mod(value_, bigValue));
        } else {
        	return coerceBin(RubyID.modID, value);
        }
    }
    
    private static BigInteger ruby_mod(BigInteger a, BigInteger b) {
        BigInteger modValue = a.abs().mod(b.abs());
        if (modValue.signum() == 0) {
            return modValue;
        }
        
        BigInteger result = modValue;
        if (a.signum() > 0) {
            if (b.signum() < 0) {
                result = b.add(modValue);
            }
        }
        
        if (a.signum() < 0) {
            if (b.signum() > 0) {
                result = b.add(modValue.negate());
            }
            
            if (b.signum() < 0) {
                result = modValue.negate();
            }
        }
        return result;
    }

    @RubyLevelMethod(name="&")
    public RubyValue op_band(RubyValue value) {
        BigInteger result;
        if (value instanceof RubyBignum) {
            RubyBignum bigValue = (RubyBignum) value;
            result = value_.and(bigValue.value_);
        } else if (value instanceof RubyFixnum) {
            BigInteger bigValue = BigInteger.valueOf(value.toInt());
            result = value_.and(bigValue);
        } else if (value instanceof RubyFloat) {
            double floatValue = ((RubyFloat) value).doubleValue();
            BigInteger bigValue = BigDecimal.valueOf(floatValue).toBigInteger();
            result = value_.and(bigValue);
        } else {
            throw new RubyException(RubyRuntime.TypeErrorClass, value
                    .getRubyClass().toString()
                    + " not expected");
        }
        return bignorm(result);
    }

    @RubyLevelMethod(name="|")
    public RubyValue op_bor(RubyValue value) {
        BigInteger result;
        if (value instanceof RubyBignum) {
            RubyBignum bigValue = (RubyBignum) value;
            result = value_.or(bigValue.value_);
        } else if (value instanceof RubyFixnum) {
            BigInteger bigValue = BigInteger.valueOf(value.toInt());
            result = value_.or(bigValue);
        } else if (value instanceof RubyFloat) {
            double floatValue = ((RubyFloat) value).doubleValue();
            BigInteger bigValue = BigDecimal.valueOf(floatValue).toBigInteger();
            result = value_.or(bigValue);
        } else {
            throw new RubyException(RubyRuntime.TypeErrorClass, value
                    .getRubyClass().toString()
                    + " not expected");
        }
        return bignorm(result);
    }

    @RubyLevelMethod(name="^")
    public RubyValue op_bxor(RubyValue value) {
        BigInteger result;
        if (value instanceof RubyBignum) {
            RubyBignum bigValue = (RubyBignum) value;
            result = value_.xor(bigValue.value_);
        } else if (value instanceof RubyFixnum) {
            BigInteger bigValue = BigInteger.valueOf(value.toInt());
            result = value_.xor(bigValue);
        } else if (value instanceof RubyFloat) {
            double floatValue = ((RubyFloat) value).doubleValue();
            BigInteger bigValue = BigDecimal.valueOf(floatValue).toBigInteger();
            result = value_.xor(bigValue);
        } else {
            throw new RubyException(RubyRuntime.TypeErrorClass, value
                    .getRubyClass().toString()
                    + " not expected");
        }
        return bignorm(result);
    }
    
    @RubyLevelMethod(name="<<")
    public RubyValue lshift(RubyValue v) {
    	int shift = v.toInt();
    	if (shift < 0) {
    		return rshift(bignorm(-shift));
    	}
    	
    	return bignorm(this.value_.shiftLeft(shift));
    }
    
    @RubyLevelMethod(name=">>")
    public RubyValue rshift(RubyValue v) {
    	int shift = v.toInt();
    	if (shift < 0) {
    		return lshift(ObjectFactory.createFixnum(-shift));
    	}
    	
    	return bignorm(this.value_.shiftRight(shift));
    }
    
    @RubyLevelMethod(name="[]")
    public RubyFixnum aref(RubyValue v) {
        int index = v.toInt();
        return this.value_.testBit(index) ? ObjectFactory.FIXNUM1 : ObjectFactory.FIXNUM0;
    }
    
    @RubyLevelMethod(name="<=>")
    public RubyValue cmp(RubyValue v) {
    	BigInteger b;
    	if (v instanceof RubyFixnum) {
    		b = BigInteger.valueOf(v.toInt());
    	} else if (v instanceof RubyBignum) {
    		b = ((RubyBignum)v).value_;
    	} else if (v instanceof RubyFloat) {
    		return this.toRubyFloat().cmp(v);
    	} else {
    		return coerceCmp(RubyID.unequalID, v);
    	}
    	
    	int result = this.value_.compareTo(b);
    	if (result == 0) {
    		return ObjectFactory.FIXNUM0;
    	} else if (result > 0) {
    		return ObjectFactory.FIXNUM1;
    	} else {
    		return ObjectFactory.FIXNUM_NEGATIVE_ONE;
    	}
    }
    
    public static RubyValue bignorm(RubyValue v) {
    	if (v instanceof RubyFixnum) {
    		return v;
    	}
    	
    	if (v instanceof RubyBignum) {
    		return bignorm(((RubyBignum)v).value_);
    	}
    	
    	return v;
    }

    public static RubyInteger bignorm(BigInteger value) {
        if (value.compareTo(FIXNUM_MAX) > 0 || value.compareTo(FIXNUM_MIN) < 0)
            return ObjectFactory.createBignum(value);
        return ObjectFactory.createFixnum(value.intValue());
    }

    public static RubyInteger bignorm(long value) {
        if (value > Integer.MAX_VALUE || value < Integer.MIN_VALUE)
            return ObjectFactory.createBignum(BigInteger.valueOf(value));
        return ObjectFactory.createFixnum((int) value);
    }
}
