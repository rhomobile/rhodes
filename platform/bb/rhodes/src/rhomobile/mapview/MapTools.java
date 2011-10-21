/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
		long ret;
		for (ret = 1; n > 0; --n)
			ret *= 2;
		return ret;
	}

	public static double math_pow2d(int n) {
		double ret = 1.0;
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
