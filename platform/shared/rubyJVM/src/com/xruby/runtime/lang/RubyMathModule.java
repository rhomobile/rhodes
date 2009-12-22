package com.xruby.runtime.lang;

//import com.xruby.runtime.lang.RubyValue;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;
//import com.xruby.runtime.lang.annotation.RubyLevelModule;

import j2me.lang.MathEx;
import com.xruby.runtime.builtin.RubyArray;
//import com.xruby.runtime.lang.RubyOneArgMethod;
//import com.xruby.runtime.lang.RubyTwoArgMethod;
//import com.xruby.runtime.lang.RubyModule;
//import com.xruby.runtime.lang.RubyBlock;

import com.xruby.runtime.builtin.ObjectFactory;

//@RubyLevelModule(name="Math")
public class RubyMathModule {
	//@RubyLevelMethod(name="sqrt", module=true)
	public static RubyValue sqrt(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(Math.sqrt(arg.toFloat()));
	}
	//@RubyLevelMethod(name="cbrt", module=true)
	public static RubyValue cbrt(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(MathEx.cbrt(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="log", module=true)
	public static RubyValue log(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(MathEx.log(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="log10", module=true)
	public static RubyValue log10(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(MathEx.log10(arg.toFloat()));
	}

	//@RubyLevelMethod(name="log2", module=true)
	public static RubyValue log2(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(MathEx.log2(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="frexp", module=true)
	public static RubyValue frexp(RubyValue receiver, RubyValue arg) 
	{
		double mantissa = arg.toFloat();
        short sign = 1;
        long exponent = 0;

        if (!Double.isInfinite(mantissa) && mantissa != 0.0) {
            // Make mantissa same sign so we only have one code path.
            if (mantissa < 0) {
                mantissa = -mantissa;
                sign = -1;
            }

            // Increase value to hit lower range.
            for (; mantissa < 0.5; mantissa *= 2.0, exponent -=1) { }

            // Decrease value to hit upper range.  
            for (; mantissa >= 1.0; mantissa *= 0.5, exponent +=1) { }
        }
        RubyArray res = new RubyArray(); 
        res.add(ObjectFactory.createFloat(sign * mantissa));
        res.add(ObjectFactory.createInteger(exponent));
        
        return res;
	}
	//@RubyLevelMethod(name="ldexp", module=true)
	public static RubyValue ldexp(RubyValue receiver, RubyValue mantissa, RubyValue exponent) 
	{
        double mantissaValue = mantissa.toFloat();
        return ObjectFactory.createFloat(mantissaValue * MathEx.pow(2.0, exponent.toInt()));
	}
	
	//@RubyLevelMethod(name="exp", module=true)
	public static RubyValue exp(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(MathEx.exp(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="cos", module=true)
	public static RubyValue cos(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(Math.cos(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="sin", module=true)
	public static RubyValue sin(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(Math.sin(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="tan", module=true)
	public static RubyValue tan(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(Math.tan(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="cosh", module=true)
	public static RubyValue cosh(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(MathEx.cosh(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="sinh", module=true)
	public static RubyValue sinh(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(MathEx.sinh(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="tanh", module=true)
	public static RubyValue tanh(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(MathEx.tanh(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="acos", module=true)
	public static RubyValue acos(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(MathEx.acos(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="acosh", module=true)
	public static RubyValue acosh(RubyValue receiver, RubyValue arg) {
		double x = arg.toFloat();
		return ObjectFactory.createFloat(MathEx.log(x + Math.sqrt(x * x - 1.0)));
	}
	
	//@RubyLevelMethod(name="asin", module=true)
	public static RubyValue asin(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(MathEx.asin(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="asinh", module=true)
	public static RubyValue asinh(RubyValue receiver, RubyValue arg) {
		double x = arg.toFloat();
		return ObjectFactory.createFloat(MathEx.log(x + Math.sqrt(x * x + 1.0)));
	}
	
	//@RubyLevelMethod(name="atan", module=true)
	public static RubyValue atan(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createFloat(MathEx.atan(arg.toFloat()));
	}
	
	//@RubyLevelMethod(name="atanh", module=true)
	public static RubyValue atanh(RubyValue receiver, RubyValue arg) {
		double x = arg.toFloat();
		return ObjectFactory.createFloat(MathEx.log((1.0 + x) / (1.0 - x)) / 2.0);
	}
	
	//@RubyLevelMethod(name="atan2", module=true)
	public static RubyValue atan2(RubyValue receiver, RubyValue arg1, RubyValue arg2) {
		return ObjectFactory.createFloat(MathEx.atan2(arg1.toFloat(), arg2.toFloat()));
	}
	
	//@RubyLevelMethod(name="hypot", module=true)
	public static RubyValue hypot(RubyValue receiver, RubyValue arg1, RubyValue arg2) {
		return ObjectFactory.createFloat(MathEx.hypot(arg1.toFloat(), arg2.toFloat()));
	}

	//@RubyLevelMethod(name="erf", module=true)
	public static RubyValue erf(RubyValue receiver, RubyValue arg) {
		double x = arg.toFloat();
		return ObjectFactory.createFloat(MathEx.erf(x));
	}
	//@RubyLevelMethod(name="erfc", module=true)
	public static RubyValue erfc(RubyValue receiver, RubyValue arg) {
		double x = arg.toFloat();
		return ObjectFactory.createFloat(MathEx.erfc(x));
	}

	//@RubyLevelMethod(name="gamma", module=true)
	public static RubyValue gamma(RubyValue receiver, RubyValue arg) {
		throw new RubyException("Not implemented: gamma");
	}
	//@RubyLevelMethod(name="lgamma", module=true)
	public static RubyValue lgamma(RubyValue receiver, RubyValue arg) {
		throw new RubyException("Not implemented: lgamma");
	}
	
}
