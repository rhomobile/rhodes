/**
 * Copyright 2007 Sishen
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import j2me.math.HugeInt;
import java.util.Random;
import j2me.util.RandomMe;
import j2me.lang.SystemMe;

import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;
//import com.xruby.runtime.lang.annotation.RubyLevelModule;

//@RubyLevelModule(name="Kernel")
public class RubyRandom {
	private static long randomSeed;
	private static Random random = new Random();

	private static boolean first = true;
	private static RubyInteger saved_seed = new RubyFixnum(0); 
 
	private static RubyValue rand_init(RubyValue vseed) {	
		RubyInteger seed = vseed.toRubyInteger();
		
		if (seed instanceof RubyFixnum) {
			randomSeed = seed.toLong();
		} else if (seed instanceof RubyBignum) {
			randomSeed = ((RubyBignum)seed).longValue();
		} else {
			throw new RubyException(RubyRuntime.TypeErrorClass, "failed to convert " + vseed.getRubyClass().getName() + " into Integer");
		}
		random.setSeed(randomSeed);
		
		first = false;
		RubyInteger old = saved_seed;
		saved_seed = seed;
		return old;
	}

	private static RubyValue random_seed() {
		// TODO: improve the random seed
		// seeds the generator using a combination of the time, the process id, and a sequence number.
		long seed = SystemMe.nanoTime();
		return RubyBignum.bignorm(seed);
	}

    //@RubyLevelMethod(name="srand")
    public static RubyValue srand(RubyValue receiver) {
        return srand(receiver, random_seed());
    }
    
    //@RubyLevelMethod(name="srand")
    public static RubyValue srand(RubyValue receiver, RubyValue arg) {
        return rand_init(arg);
    }
	
	//@RubyLevelMethod(name="rand")
	public static RubyValue rand(RubyValue receiver) {
		if (first) {
			rand_init(random_seed());
		}
        return new RubyFloat(random.nextDouble());
    }
	
	//@RubyLevelMethod(name="rand")
	public static RubyValue rand(RubyValue receiver, RubyValue arg) {
		long val, max = 0;
		
		if (first) {
			rand_init(random_seed());
		}
		
		if (arg instanceof RubyFloat) {
			RubyFloat vmax = (RubyFloat)arg;
			if (vmax.doubleValue() <= Long.MAX_VALUE && vmax.doubleValue() >= Long.MIN_VALUE) {
				RubyValue tmp = vmax.floor();
				if (tmp instanceof RubyFixnum) {
					max = ((RubyFixnum)tmp).toLong();
				} else if (tmp instanceof RubyBignum) {
					max = ((RubyBignum)tmp).longValue();
				}
			} else {
				if (vmax.doubleValue() < 0) {
					arg = RubyBignum.bignorm((long)-vmax.doubleValue());
				} else {
					arg = RubyBignum.bignorm((long)vmax.doubleValue());
				}
			}
		} else if (arg == RubyConstant.QNIL) {
			max = 0;
		} else if (arg instanceof RubyFixnum) {
			arg = arg.toRubyInteger();
			max = arg.toLong();
		}
		
		if (arg instanceof RubyBignum) {
			byte[] bytes = new byte[((RubyBignum)arg).getInternal().toByteArray().length-1];
			RandomMe.nextBytes(random,bytes);
			return RubyBignum.bignorm(new HugeInt(1, bytes));
		}

		if (max == 0) {
			return new RubyFloat(random.nextDouble());
		}
		
		if (max < 0) {
			max = -max;
		}
	    val = random.nextInt((int)max);
	    if (val >= Long.MIN_VALUE && val <= Long.MAX_VALUE) {
	    	return new RubyFixnum((int)val);
	    } else {
	    	return RubyBignum.bignorm(val);
	    }
    }
}