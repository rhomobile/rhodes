package rhomobile.mapview;

import j2me.lang.MathEx;

public class MapTools {
	
	private static final double LN2 = 0.693147180559945;

	//================================================================
	// Mathematical functions
	
	// Sine of the a
	public static double math_sin(double a) {
		return Math.sin(a);
	}
	
	// Arc sine of the a
	public static double math_asin(double a) {
		//return MathUtilities.asin(a);
		return MathEx.asin(a);
	}
	
	// Exponential (base E ~ 2.718281828) of a
	public static double math_exp(double a) {
		//return MathUtilities.exp(a);
		return MathEx.exp(a);
	}
	
	// Natural logarithm (base E ~ 2.718281828) of a
	public static double math_ln(double a) {
		//return MathUtilities.log(a);
		return MathEx.log(a);
	}
	
	// Binary logarithm (base 2) of a
	public static double math_log2(double a) {
		return math_ln(a)/LN2;
	}
	
	// 2 raised to the power of n
	public static long math_pow2(int n) {
		long ret = 1;
		if (n < 0) {
			for (; n < 0; ++n) {
				ret /= 2;
			}
		}
		if (n > 0) {
			for (; n > 0; --n) {
				ret *= 2;
			}
		}
		return ret;
	}
	
	public static double math_sqrt(double a) {
		return MathEx.pow(a, 0.5);
	}
	
	// Hyperbolic tangent of a
	public static double math_tanh(double a) {
		double epx = math_exp(a);
		double emx = math_exp(-a);
		return (epx - emx)/(epx + emx);
	}
	
	// Hyperbolic arc tangent of a
	public static double math_atanh(double a) {
		return 0.5*math_ln((1 + a)/(1 - a));
	}
	
}
