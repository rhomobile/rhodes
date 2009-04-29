/*
 * J2ME Fixed-Point Math Library
 * 
 * Dan Carter, 2004
 * http://orbisstudios.com
 */

package j2me.math;

/**<pre>
 * <b>J2ME Fixed-Point Math Library</b>
 * 
 * Provided under the artistic license: <a href="http://www.opensource.org/licenses/artistic-license.html">http://www.opensource.org/licenses/artistic-license.html</a>
 * 
 * Basically it means you can use this library for free, even for commercial purposes.
 * 
 * 
 * <b>References:</b>
 * 
 * exp(), log(), atan2() converted from the floating-point implementation found here:
 * <a href="http://www.netlib.org/fdlibm/">http://www.netlib.org/fdlibm</a>
 * 
 * sin(), asin(), sqrt() borrowed from this free fixed-point library:
 * <a href="http://www.ai.mit.edu/people/hqm/imode/fplib">http://www.ai.mit.edu/people/hqm/imode/fplib</a>
 * </pre>
 * @author  Dan Carter
 * @version %I%, %G%
 */

public class Math {
	
    private static final int maxPrecision = 13;
    private static final int maxDigits = 4;
    
	/**
	 * number of fractional bits in all operations, do not modify directly
	 */
    public static int precision = 0;
    private static int frac_mask = 0;
    private static final int e = 178145;
    private static final int pi = 205887;
    private static int one_eighty_over_pi;
    private static int pi_over_one_eighty;
    private static int maxDigitsInt;
    private static int maxDigitsInt_10;
    public static int ONE, HALF, TWO, E, PI;
    /**
	 * small number used to round off errors
	 */
    public static final int EPSILON = 0x00000005;
    /**
	 * largest possible number
	 */
    public static final int INFINITY = 0x7fffffff;
    
//    private static final int sk1 = 498;
//    private static final int sk2 = 10882;
//    private static int SK1, SK2;
    
//    static final int as1 = -1228;
//    static final int as2 = 4866;
//    static final int as3 = 13901;
//    static final int as4 = 102939;
//    private static int AS1, AS2, AS3, AS4;
    
    static final int ln2 = 45426;
    static final int ln2_inv = 94548;
    private static int LN2, LN2_INV;
    
    private static final int lg[] = {
    		43690,
    		26214,
			18724,
			14563,
			11916,
			10036,
			9698
    };
    private static int LG[] = new int[lg.length];
    
    private static final int exp_p[] = {
    		10922,
			-182,
			4
    };
    private static int EXP_P[] = new int[exp_p.length];
	
    static {
    	maxDigitsInt = 1;
    	for (int i = 0; i < maxDigits; i++) {
    		maxDigitsInt *= 10;
    	}
    	maxDigitsInt_10 = maxDigitsInt*10;
    	setPrecision(maxPrecision);
    }
    
    /**
     * Converts an int to a FP.
     * @param x int to convert
     * @return FP
     */
	public static int toFP(int x) {
		return (x < 0) ? -(-x << precision) : x << precision;
    }
	
	/**
     * Converts a FP to an int.
     * @param x FP to convert
     * @return int
     */
//	public static int toInt(int x) {
//        return (x < 0) ? -(-x >> precision) : x >> precision;
//    }
	
	/**
	 * Converts a FP to the current set precision.
	 * @param x the FP to convert
	 * @param p the precision of the FP passed in
	 * @return a FP of the current precision
	 */
//	public static int convert(int x, int p) {
//		int num, xabs = abs(x);
//		if (p > maxPrecision || p < 0) return x;
//		if (p > precision)
//			num = xabs >> (p - precision);
//		else
//			num = xabs << (precision - p);
//		if (x < 0)
//			num = -num;
//		return num;
//	}
	
	/**
	 * Converts a string to a FP.
	 * <br>
	 * <br>The string should trimmed of any whitespace before-hand.
	 * <br>
	 * <br>A few examples of valid strings:<br>
	 * <pre>
	 * .01
	 * 0.01
	 * 10
	 * 130.0
	 * -30000.12345
	 * </pre>
	 * @param s the string to convert
	 * @return FP
	 */
//    public static int toFP(String s)
//    {
//    	int x, i, integer, frac;
//    	String fracstring = null;
//        boolean neg = false;
//        if (s.charAt(0) == '-') {
//        	neg = true;
//        	s = s.substring(1, s.length());
//        }
//        int index = s.indexOf('.');
//        
//        if (index < 0) {
//        	integer = Integer.parseInt(s);
//        } else if (index == 0) {
//        	integer = 0;
//        	fracstring = s.substring(1, s.length());
//        } else if (index == s.length()-1) {
//        	integer = Integer.parseInt(s.substring(0, index));
//        } else {
//        	integer = Integer.parseInt(s.substring(0, index));
//        	fracstring = s.substring(index+1, s.length());
//        }
//        
//        if (fracstring != null) {
//        	if (fracstring.length() > maxDigits) fracstring = fracstring.substring(0, maxDigits);
//        	frac = Integer.parseInt(fracstring);
//        	for (i = 0; i < maxDigits - fracstring.length(); i++)
//        		frac *= 10;
//        } else
//        	frac = 0;
//        x = (integer << precision) + ((frac * ONE) / maxDigitsInt);
//        if (neg)
//        	x = -x;
//        return x;
//    }

    /**
     * Converts a FP to a string.
     * <br>
     * <br>The FP is rounded to the maximum decimal place requested.
     * 
     * @param x the FP to convert
     * @param min_frac_digits minimum decimal digits in string (padded with zeros) 
     * @param max_frac_digits maximum decimal digits in string
     * @return a string representing the FP
     */

//    public static String toString(int x, int min_frac_digits, int max_frac_digits) {
//    	int i, len;
//        boolean neg = false, frac_nonzero = false;
//        if (x < 0) {
//        	neg = true;
//        	x = -x;
//        }
//        String fracString = Integer.toString(((x & frac_mask) * maxDigitsInt_10) / ONE);
//        len = maxDigits - fracString.length() + 1;
//        for (i = 0; i < len; i++) fracString = "0" + fracString;
//        int max = (max_frac_digits + 1 < fracString.length()) ? max_frac_digits + 1 : fracString.length();
//        int carry = 0;
//        
//        char [] chars = fracString.toCharArray();
//        if (chars[max-1] < '5') {
//        	chars[max-1] = '0';
//        } else {
//        	chars[max-1] = '0';
//        	for (i = max-2; i >= 0; i--) {
//            	if (chars[i] == '9') {
//            		chars[i] = '0';
//            	} else {
//            		chars[i]++;
//            		break;
//            	}
//            }
//        	if (i == -1)
//        		carry = 1;
//        }
//        for (i = max; i < chars.length; i++) {
//        	chars[i] = '0';
//        }
//        fracString = String.valueOf(chars);
//        for (i = fracString.length()-1; i >= 0; i--) {
//        	if (fracString.charAt(i) != '0')
//        		break;
//        }
//        fracString = fracString.substring(0, i+1);
//        for (i = fracString.length(); i < min_frac_digits; i++) {
//        	fracString += "0";
//        }
//        if (fracString.length() > max_frac_digits)
//        	fracString = fracString.substring(0, max_frac_digits);
//        int whole = (x >> precision) + carry;
//        for (i = fracString.length()-1; i >= 0; i--) {
//        	if (fracString.charAt(i) != '0') {
//        		frac_nonzero = true;
//        		break;
//        	}
//        }
//        if (fracString.length() > 0)
//        	fracString = "." + fracString;
//        return ((neg && (whole != 0 || frac_nonzero)) ? "-" : "") + Integer.toString(whole) + fracString;
//    }

    /**
     * Converts a FP to a string.
     * 
     * Same as toString(x, 0, 4)
     * 
     * @param x the FP to convert
     * @return a string representing the FP with a minimum of decimals in the string
     */
//    public static String toString(int x) {
//        return toString(x, 0, maxDigits);
//    }
    
    /**
     * Sets the precision for all fixed-point operations.
     * <br>
     * <br>The maximum precision is 13 bits by default.
     * @param p the desired precision in number of bits
     */
    public static void setPrecision(int p) {
    	if (p > maxPrecision || p < 0) return;
    	int i;
        precision = p;
        PI = pi >> (16-p);
        ONE = 1 << p;
        HALF = (p > 0) ? 1 << (p-1) : 0;
        TWO = 1 << (p+1);
        E = e >> (16-p);
//        SK1 = sk1 >> (16-p);
//        SK2 = sk2 >> (16-p);
//        AS1 = as1 >> (16-p);
//        AS2 = as2 >> (16-p);
//        AS3 = as3 >> (16-p);
//        AS4 = as4 >> (16-p);
        LN2 = ln2 >> (16-p);
        LN2_INV = ln2_inv >> (16-p);
        for (i = 0; i < lg.length; i++) LG[i] = lg[i] >> (16-p);
        for (i = 0; i < exp_p.length; i++) EXP_P[i] = exp_p[i] >> (16-p);
        frac_mask = ONE-1;
        pi_over_one_eighty = div(PI, toFP(180));
        one_eighty_over_pi = div(toFP(180), PI);
    }

    public static int abs(int x) {
    	return (x < 0) ? -x : x;
    }
    
//    public static int ceil(int x) {
//    	boolean neg = false;
//    	if (x < 0) {
//    		x = -x;
//    		neg = true;
//    	}
//    	if ((x&frac_mask) == 0)
//    		return (neg) ? -x : x;
//    	if (neg)
//    		return -(x&~frac_mask);
//    	return (x&~frac_mask) + ONE;
//    }
    
//    public static int floor(int x) {
//    	boolean neg = false;
//    	if (x < 0) {
//    		x = -x;
//    		neg = true;
//    	}
//    	if ((x&frac_mask) == 0)
//    		return (neg) ? -x : x;
//    	if (neg)
//    		return -(x&~frac_mask) - ONE;
//    	return (x&~frac_mask);
//    }
    
    /**
     * Removes the fractional part of a FP
     * @param x the FP to truncate
     * @return a truncated FP
     */
    
//    public static int trunc(int x) {
//    	return (x < 0) ? -(-x&~frac_mask) : x&~frac_mask;
//    }
    
    /**
     * Returns the fractional part of a FP
     * @param x FP to get fractional part of
     * @return positive fractional FP if input is positive, negative fractional otherwise
     */
    
//    public static int frac(int x) {
//    	return (x < 0) ? -(-x&frac_mask) : x&frac_mask;
//    }
    
    /**
     * Rounds a FP to the nearest whole number.
     * @param x the FP to round
     * @return a rounded FP
     */
    public static int round(int x) {
    	boolean neg = false;
    	if (x < 0) {
    		x = -x;
    		neg = true;
    	}
    	x += HALF;
    	x &= ~frac_mask;
		return (neg) ? -x : x;
    }
    
    /**
     * Multiplies two FPs.
     * @param x
     * @param y
     * @return
     */
    public static int mul(int x, int y) {
    	if (x == 0 || y == 0) return 0;
    	int xneg = 0, yneg = 0;
    	if (x < 0) {
    		xneg = 1;
    		x = -x;
    	}
    	if (y < 0) {
    		yneg = 1;
    		y = -y;
    	}
    	int res = ((x >> precision)*(y >> precision) << precision)
			+ (x & frac_mask)*(y >> precision)
			+ (x >> precision)*(y & frac_mask)
			+ ((x & frac_mask)*(y & frac_mask) >> precision);
    	if ((xneg^yneg) == 1)
    		res = -res;
    	return res;
    }
    
    /**
     * Divides two FPs.
     * @param x
     * @param y
     * @return
     */
    public static int div(int x, int y) {
    	if (x == 0) return 0;
    	if (y == 0) return (x < 0) ? -INFINITY : INFINITY;
    	int xneg = 0, yneg = 0;
    	if (x < 0) {
    		xneg = 1;
    		x = -x;
    	}
    	if (y < 0) {
    		yneg = 1;
    		y = -y;
    	}
    	int msb = 0, lsb = 0;
    	while ((x & (1 << 30 - msb)) == 0)
    		msb++;
    	while ((y & (1 << lsb)) == 0)
    		lsb++;
    	int shifty = precision - (msb+lsb);
    	int res = ((x << msb) / (y >> lsb));
    	if (shifty > 0)
    		res <<= shifty;
    	else
    		res >>= -shifty;
    	if ((xneg^yneg) == 1)
    		res = -res;
    	return res;
    }
    
//    public static int sqrt(int x) {
//    	int s = (x + ONE) >> 1;
//    	for (int i = 0; i < 8; i++) {
//    	    s = (s + div(x, s)) >> 1;
//    	}
//    	return s;
//    }

    /**
     * FP angle must be in radians.
     * 
     * 
     * @param f the angle in radians
     * @return sin() of the angle
     */
//    public static int sin(int f) {
//		int sign = 1;
//		int PI_OVER_2 = PI/2;
//		f %= PI*2;
//		if(f < 0)
//            f = PI*2 + f;
//		if ((f > PI_OVER_2) && (f <= PI)) {
//		    f = PI - f;
//		} else if ((f > PI) && (f <= (PI + PI_OVER_2))) {
//		    f = f - PI;
//		    sign = -1;
//		} else if (f > (PI + PI_OVER_2)) {
//		    f = (PI<<1)-f;
//		    sign = -1;
//		}
//	
//		int sqr = mul(f,f);
//		int result = SK1;
//		result = mul(result, sqr);
//		result -= SK2;
//		result = mul(result, sqr);
//		result += ONE;
//		result = mul(result, f);
//		return sign * result;
//    }
    
    /**
     * FP angle must be in radians.
     * 
     * 
     * @param f the angle in radians
     * @return cos() of the angle
     */
//    public static int cos(int f) {
//    	return sin(PI/2 - f);
//    }
    
    /**
     * FP angle must be in radians.
     * 
     * 
     * @param f the angle in radians
     * @return tan() of the angle
     */
//    public static int tan(int f) {
//    	return div(sin(f), cos(f));
//    }

    
    /**
     * input range: [-1, 1] -- output range: [-PI/2, PI/2]
     * @param f
     * @return
     */
//    public static int asin(int f) {
//    	boolean neg = false;
//    	if (f < 0) {
//    		neg = true;
//    		f = -f;
//    	}
//		int fRoot = sqrt(ONE-f);
//		int result = AS1;
//		result = mul(result, f);
//		result += AS2;
//		result = mul(result, f);
//		result -= AS3;
//		result = mul(result, f);
//		result += AS4;
//		result = PI/2 - (mul(fRoot,result));
//		if (neg)
//			result = -result;
//		return result;
//    }
    
    /**
     * input range: [-1, 1] -- output range: [0, PI]
     * @param f
     * @return
     */
//    public static int acos(int f) {
//        return PI/2 - asin(f);
//    }
    
    /**
     * input range: [-INF, INF] -- output range: [-PI/2, PI/2]
     * @param f
     * @return
     */
//    public static int atan(int f) {
//        return asin(div(f, sqrt(ONE + mul(f, f))));
//    }
    
    /**
     * input range: [-INF, INF] -- output range: [-PI, PI] -- Converts (x,y) in Cartesian to (r, <i>theta</i>) in polar, and returns <i>theta</i>.
     * @param y
     * @param x
     * @return
     */
//    public static int atan2(int y, int x) {
//        if (y == 0) {
//        	if (x >= 0) {
//        		return 0;
//        	} else if (x < 0) {
//        		return PI;
//        	}
//        } else if (x == 0) {
//        	return (y > 0) ? PI/2 : -PI/2;
//        }
//        int z = atan(abs(div(y, x)));
//        if (x > 0) {
//        	return (y > 0) ? z : -z;
//        } else {
//        	return (y > 0) ? PI - z : z - PI;
//        }
//    }
    
    /**
     * E raised to the FP x (e**x)
     * @param x
     * @return
     */
    public static int exp(int x) {
    	if (x == 0) return ONE;
    	int xabs = abs(x);
    	int k = mul(xabs, LN2_INV);
    	k += HALF;
    	k &= ~frac_mask;
    	if (x < 0)
    		k = -k;
    	x -= mul(k, LN2);
    	int z  = mul(x,x);
    	int R = TWO + mul(z,EXP_P[0]+mul(z,EXP_P[1]+mul(z,EXP_P[2])));
    	int xp = ONE + div(mul(TWO,x), R - x);
    	if (k < 0)
    		k = ONE >> (-k >> precision);
    	else
    		k = ONE << (k >> precision);
    	return mul(k, xp);
    }
    
    /**
     * Natural logarithm ln(x)
     * @param x
     * @return
     */
    public static int log(int x) {
    	if (x < 0) return 0;
    	if (x == 0) return -INFINITY;
    	int log2 = 0, xi = x;
    	while (xi >= TWO) {
    		xi >>= 1;
    		log2++;
    	}
    	int f = xi - ONE;
    	int s = div(f,TWO+f); 
    	int z = mul(s,s);
    	int w = mul(z,z);
    	int R = mul(w,LG[1]+mul(w,LG[3]+mul(w,LG[5]))) + mul(z,LG[0]+mul(w,LG[2]+mul(w,LG[4]+mul(w,LG[6]))));
    	return mul(LN2, (log2 << precision)) + f - mul(s,f - R);
    }
    
    /**
     * Logarithm with specified FP base
     * @param x
     * @return
     */
    public static int log(int x, int base) {
    	return div(log(x),log(base));
    }
    
    /**
     * FP x raised to FP y.
     * @param x
     * @param y
     * @return
     */
    public static int pow(int x, int y) {
    	if (y == 0) return ONE;
    	if (x < 0) return 0;
    	return exp(mul(log(x), y));
    }
    
//    public static int min(int x, int y) {
//        return (x < y) ? x : y;
//    }
    
//    public static int max(int x, int y) {
//        return (x > y) ? x : y;
//    }

    /**
     * Converts degrees to radians.
     * @param x FP in degrees
     * @return FP in radians
     */
//    public static int deg2rad(int x) {
//    	return mul(x, pi_over_one_eighty);
//    }
    
    /**
     * Converts radians to degrees.
     * @param x FP in radians
     * @return FP in degrees
     */
//    public static int rad2deg(int x) {
//    	return mul(x, one_eighty_over_pi);
//    }
}
