/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.lang;

import java.util.Random;

/**
 * <p> This utility class ensures cross-platform portability of the math 
 *     library. Functions not supported by the platform are emulated.
 *     Developers may replace the current implementation with native
 *     implementations for faster execution (unlike 
 *     <code>java.lang.Math</code> this class can be customized).<p> 
 * 
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 4.2, January 6, 2007
 */
public final class MathLib {

    /**
     * Default constructor.
     */
    private MathLib() {
    }

    /**
     * Returns a pseudo random <code>int</code> value in the range
     * <code>[min, max]</code> (fast and thread-safe without synchronization).
     * 
     * @param min the minimum value inclusive.
     * @param max the maximum value exclusive.
     * @return a pseudo random number in the range <code>[min, max]</code>.
     */
    public static int random(int min, int max) {
        int next = RANDOM.nextInt();
        if ((next >= min) && (next <= max))
            return next;
        next += Integer.MIN_VALUE;
        if ((next >= min) && (next <= max))
            return next;
        // We should not have interval overflow as the interval has to be less 
        // or equal to Integer.MAX_VALUE (otherwise we would have exited before).
        final int interval = 1 + max - min; // Positive.
        if (interval <= 0)
            throw new Error("Interval [" + min + ".." + max + "] error"); // In case.
        return MathLib.abs(next % interval) + min;
    }

    private static final Random RANDOM = new Random();

    /**
     * Returns a pseudo random <code>long</code> value in the range
     * <code>[min, max]</code> (fast and thread-safe without synchronization).
     * 
     * @param min the minimum value inclusive.
     * @param max the maximum value inclusive.
     * @return a pseudo random number in the range <code>[min, max]</code>.
     */
    public static long random(long min, long max) {
        long next = RANDOM.nextLong();
        if ((next >= min) && (next <= max))
            return next;
        next += Long.MIN_VALUE;
        if ((next >= min) && (next <= max))
            return next;
        // We should not have interval overflow as the interval has to be less 
        // or equal to Long.MAX_VALUE (otherwise we would have exited before).
        final long interval = 1L + max - min; // Positive.
        if (interval <= 0)
            throw new Error("Interval error"); // In case.
        return MathLib.abs(next % interval) + min;
    }

    /**
     * Returns a pseudo random <code>float</code> value in the range
     * <code>[min, max]</code> (fast and thread-safe without synchronization).
     * 
     * @param min the minimum value inclusive.
     * @param max the maximum value inclusive.
     * @return a pseudo random number in the range <code>[min, max]</code>.
     * @JVM-1.1+@
     public static float random(float min, float max) {
     return (float) random((double)min, (double) max);   
     }
     /**/

    /**
     * Returns a pseudo random <code>double</code> value in the range
     * <code>[min, max]</code> (fast and thread-safe without synchronization).
     * 
     * @param min the minimum value inclusive.
     * @param max the maximum value inclusive.
     * @return a pseudo random number in the range <code>[min, max]</code>.
     * @JVM-1.1+@
     public static double random(double min, double max) {
     double next = RANDOM.nextDouble(); // 0.0 .. 1.0
     return min + (next * max) - (next * min);
     // Due to potential numeric errors, both min and max are possible.
     }
     /**/

    /**
     * Returns the number of bits in the minimal two's-complement representation
     * of the specified <code>int</code>, excluding a sign bit.
     * For positive <code>int</code>, this is equivalent to the number of bits
     * in the ordinary binary representation. For negative <code>int</code>,
     * it is equivalent to the number of bits of the positive value 
     * <code>-(i + 1)</code>.
     * 
     * @param i the <code>int</code> value for which the bit length is returned.
     * @return the bit length of <code>i</code>.
     */
    public static int bitLength(int i) {
        if (i < 0)
            i = -++i;
        return (i < 1 << 16) ? (i < 1 << 8) ? BIT_LENGTH[i]
                : BIT_LENGTH[i >>> 8] + 8
                : (i < 1 << 24) ? BIT_LENGTH[i >>> 16] + 16
                        : BIT_LENGTH[i >>> 24] + 24;
    }

    private static final byte[] BIT_LENGTH = { 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4,
            4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6,
            6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
            6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
            7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
            7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
            7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8 };

    /**
     * Returns the number of bits in the minimal two's-complement representation
     * of the specified <code>long</code>, excluding a sign bit.
     * For positive <code>long</code>, this is equivalent to the number of bits
     * in the ordinary binary representation. For negative <code>long</code>,
     * it is equivalent to the number of bits of the positive value 
     * <code>-(l + 1)</code>.
     * 
     * @param l the <code>long</code> value for which the bit length is returned.
     * @return the bit length of <code>l</code>.
     */
    public static int bitLength(long l) {
        int i = (int) (l >> 32);
        if (i > 0)
            return (i < 1 << 16) ? (i < 1 << 8) ? BIT_LENGTH[i] + 32
                    : BIT_LENGTH[i >>> 8] + 40
                    : (i < 1 << 24) ? BIT_LENGTH[i >>> 16] + 48
                            : BIT_LENGTH[i >>> 24] + 56;
        if (i < 0)
            return bitLength(-++l);
        i = (int) l;
        return (i < 0) ? 32 : (i < 1 << 16) ? (i < 1 << 8) ? BIT_LENGTH[i]
                : BIT_LENGTH[i >>> 8] + 8
                : (i < 1 << 24) ? BIT_LENGTH[i >>> 16] + 16
                        : BIT_LENGTH[i >>> 24] + 24;
    }

    /**
     * Returns the number of digits in the minimal ten's-complement 
     * representation of the specified <code>int</code>, excluding a sign bit.
     * For positive <code>int</code>, this is equivalent to the number of digit
     * in the ordinary decimal representation. For negative <code>int</code>,
     * it is equivalent to the number of digit of the positive value 
     * <code>-(i + 1)</code>.
     * 
     * @param i the <code>int</code> value for which the digit length is returned.
     * @return the digit length of <code>i</code>.
     */
    public static int digitLength(int i) {
        if (i < 0)
            i = -++i;
        return (i >= 100000) ? (i >= 10000000) ? (i >= 1000000000) ? 10
                : (i >= 100000000) ? 9 : 8 : (i >= 1000000) ? 7 : 6
                : (i >= 100) ? (i >= 10000) ? 5 : (i >= 1000) ? 4 : 3
                        : (i >= 10) ? 2 : (i >= 1) ? 1 : 0;
    }

    /**
     * Returns the number of digits in the minimal ten's-complement 
     * representation of the specified <code>long</code>, excluding a sign bit.
     * For positive <code>long</code>, this is equivalent to the number of digit
     * in the ordinary decimal representation. For negative <code>long</code>,
     * it is equivalent to the number of digit of the positive value 
     * <code>-(value + 1)</code>.
     * 
     * @param l the <code>long</code> value for which the digit length is returned.
     * @return the digit length of <code>l</code>.
     */
    public static int digitLength(long l) {
        if (l < 0)
            l = -++l;
        if (l <= Integer.MAX_VALUE) {
            int i = (int) l;
            return (i >= 100000) ? (i >= 10000000) ? (i >= 1000000000) ? 10
                    : (i >= 100000000) ? 9 : 8 : (i >= 1000000) ? 7 : 6
                    : (i >= 100) ? (i >= 10000) ? 5 : (i >= 1000) ? 4 : 3
                            : (i >= 10) ? 2 : (i >= 1) ? 1 : 0;
        }
        // At least 10 digits or more.
        return (l >= 100000000000000L) ? (l >= 10000000000000000L) ? (l >= 1000000000000000000L) ? 19
                : (l >= 100000000000000000L) ? 18 : 17
                : (l >= 1000000000000000L) ? 16 : 15
                : (l >= 100000000000L) ? (l >= 10000000000000L) ? 14
                        : (l >= 1000000000000L) ? 13 : 12
                        : (l >= 10000000000L) ? 11 : 10;

    }

    /**
     * Returns the closest <code>double</code> representation of the
     * specified <code>long</code> number multiplied by a power of two.
     *
     * @param m the <code>long</code> multiplier.
     * @param n the power of two exponent.
     * @return <code>m * 2<sup>n</sup></code>.
     @JVM-1.1+@
     public static double toDoublePow2(long m, int n) {
     if (m == 0)
     return 0.0;
     if (m == Long.MIN_VALUE)
     return toDoublePow2(Long.MIN_VALUE >> 1, n + 1);
     if (m < 0)
     return -toDoublePow2(-m, n);
     int bitLength = MathLib.bitLength(m);
     int shift = bitLength - 53;
     long exp = 1023L + 52 + n + shift; // Use long to avoid overflow.
     if (exp >= 0x7FF)
     return Double.POSITIVE_INFINITY;
     if (exp <= 0) { // Degenerated number (subnormal, assume 0 for bit 52)
     if (exp <= -54)
     return 0.0;
     return toDoublePow2(m, n + 54) / 18014398509481984L; // 2^54 Exact.
     }
     // Normal number.
     long bits = (shift > 0) ? (m >> shift)
     + ((m >> (shift - 1)) & 1) : // Rounding.
     m << -shift;
     if (((bits >> 52) != 1) && (++exp >= 0x7FF)) // Rare case where rounding push to next power of 2.
     return Double.POSITIVE_INFINITY;
     bits &= 0x000fffffffffffffL; // Clears MSB (bit 52)
     bits |= exp << 52;
     return Double.longBitsToDouble(bits);
     }
     /**/

    /**
     * Returns the closest <code>double</code> representation of the
     * specified <code>long</code> number multiplied by a power of ten.
     *
     * @param m the <code>long</code> multiplier.
     * @param n the power of ten exponent.
     * @return <code>multiplier * 10<sup>n</sup></code>.
     * @JVM-1.1+@
     public static double toDoublePow10(long m, int n) {
     if (m == 0)
     return 0.0;
     if (m == Long.MIN_VALUE)
     return toDoublePow10(Long.MIN_VALUE / 10, n + 1);
     if (m < 0)
     return -toDoublePow10(-m, n);
     if (n >= 0) { // Positive power.
     if (n > 308)
     return Double.POSITIVE_INFINITY;
     // Works with 4 x 32 bits registers (x3:x2:x1:x0)
     long x0 = 0;  // 32 bits.
     long x1 = 0;  // 32 bits.
     long x2 = m & MASK_32; // 32 bits.
     long x3 = m >>> 32; // 32 bits.
     int pow2 = 0;
     while (n != 0) {
     int i = (n >= POW5_INT.length) ? POW5_INT.length - 1 : n;
     int coef = POW5_INT[i]; // 31 bits max.
     
     if (((int)x0) != 0) x0 *= coef; // 63 bits max.
     if (((int)x1) != 0) x1 *= coef; // 63 bits max.
     x2 *= coef; // 63 bits max.
     x3 *= coef; // 63 bits max.
     
     x1 += x0 >>> 32;
     x0 &= MASK_32;
     
     x2 += x1 >>> 32;
     x1 &= MASK_32;
     
     x3 += x2 >>> 32;
     x2 &= MASK_32;
     
     // Adjusts powers.
     pow2 += i;
     n -= i;
     
     // Normalizes (x3 should be 32 bits max).
     long carry = x3 >>> 32;
     if (carry != 0) { // Shift.
     x0 = x1;
     x1 = x2;
     x2 = x3 & MASK_32;
     x3 = carry;
     pow2 += 32;
     }
     }
     
     // Merges registers to a 63 bits mantissa.
     int shift = 31 - MathLib.bitLength(x3); // -1..30
     pow2 -= shift;
     long mantissa = (shift < 0) ?
     (x3 << 31) | (x2 >>> 1) : // x3 is 32 bits.
     (((x3 << 32) | x2) << shift) | (x1 >>> (32 - shift));
     return toDoublePow2(mantissa, pow2);
     
     } else { // n < 0
     if (n < -324 - 20) // m less than 20 digits.
     return 0.0;
     
     // Works with x1:x0 126 bits register.
     long x1 = m; // 63 bits.
     long x0 = 0; // 63 bits.
     int pow2 = 0;
     while (true) {
     
     // Normalizes x1:x0
     int shift = 63 - MathLib.bitLength(x1);
     x1 <<= shift;
     x1 |= x0 >>> (63 - shift);
     x0 = (x0 << shift) & MASK_63;
     pow2 -= shift;
     
     // Checks if division has to be performed.
     if (n == 0)
     break; // Done.
     
     // Retrieves power of 5 divisor.
     int i = (-n >= POW5_INT.length) ? POW5_INT.length - 1 : -n;
     int divisor = POW5_INT[i];
     
     // Performs the division (126 bits by 31 bits).
     long wh = (x1 >>> 32);
     long qh = wh / divisor;
     long r = wh - qh * divisor;
     long wl = (r << 32) | (x1 & MASK_32);
     long ql = wl / divisor;
     r = wl - ql * divisor;
     x1 = (qh << 32) | ql;
     
     wh = (r << 31) | (x0 >>> 32);
     qh = wh / divisor;
     r = wh - qh * divisor;
     wl = (r << 32) | (x0 & MASK_32);
     ql = wl / divisor;
     x0 = (qh << 32) | ql;
     
     // Adjusts powers.
     n += i;
     pow2 -= i;
     }
     return toDoublePow2(x1, pow2);
     }
     }
     
     private static final long MASK_63 = 0x7FFFFFFFFFFFFFFFL;
     
     private static final long MASK_32 = 0xFFFFFFFFL;
     
     private static final int[] POW5_INT = { 1, 5, 25, 125, 625, 3125, 15625,
     78125, 390625, 1953125, 9765625, 48828125, 244140625, 1220703125 };
     /**/

    /**
     * Returns the closest <code>long</code> representation of the
     * specified <code>double</code> number multiplied by a power of two.
     *
     * @param d the <code>double</code> multiplier.
     * @param n the power of two exponent.
     * @return <code>d * 2<sup>n</sup></code>
     * @throws ArithmeticException if the conversion cannot be performed
     *         (NaN, Infinity or overflow).
     * @JVM-1.1+@
     public static long toLongPow2(double d, int n) {
     long bits = Double.doubleToLongBits(d);
     boolean isNegative = (bits >> 63) != 0;
     int exp = ((int)(bits >> 52)) & 0x7FF;
     long m = bits & 0x000fffffffffffffL;
     if (exp == 0x7FF) throw new ArithmeticException(
     "Cannot convert to long (Infinity or NaN)");
     if (exp == 0) {
     if (m == 0) return 0L;
     return toLongPow2(d * 18014398509481984L, n - 54); // 2^54 Exact.
     }
     m |= 0x0010000000000000L; // Sets MSB (bit 52)
     long shift = exp - 1023L - 52 + n; // Use long to avoid overflow.
     if (shift <= -64) return 0L;
     if (shift >= 11) throw new  ArithmeticException(
     "Cannot convert to long (overflow)");
     m = (shift >= 0) ? m << shift : 
     (m >> -shift) + ((m >> -(shift + 1)) & 1) ; // Rounding.
     return isNegative ? -m : m;
     }
     /**/

    /**
     * Returns the closest <code>long</code> representation of the
     * specified <code>double</code> number multiplied by a power of ten.
     *
     * @param d the <code>double</code> multiplier.
     * @param n the power of two exponent.
     * @return <code>d * 10<sup>n</sup></code>.
     @JVM-1.1+@
     public static long toLongPow10(double d, int n) {
     long bits = Double.doubleToLongBits(d);
     boolean isNegative = (bits >> 63) != 0;
     int exp = ((int)(bits >> 52)) & 0x7FF;
     long m = bits & 0x000fffffffffffffL;
     if (exp == 0x7FF) throw new ArithmeticException(
     "Cannot convert to long (Infinity or NaN)");
     if (exp == 0) {
     if (m == 0) return 0L;
     return toLongPow10(d * 1E16, n - 16);
     }
     m |= 0x0010000000000000L; // Sets MSB (bit 52)
     int pow2 = exp - 1023 - 52;
     // Retrieves 63 bits m with n == 0.
     if (n >= 0) {
     // Works with 4 x 32 bits registers (x3:x2:x1:x0)
     long x0 = 0;  // 32 bits.
     long x1 = 0;  // 32 bits.
     long x2 = m & MASK_32; // 32 bits.
     long x3 = m >>> 32; // 32 bits.
     while (n != 0) {
     int i = (n >= POW5_INT.length) ? POW5_INT.length - 1 : n;
     int coef = POW5_INT[i]; // 31 bits max.
     
     if (((int)x0) != 0) x0 *= coef; // 63 bits max.
     if (((int)x1) != 0) x1 *= coef; // 63 bits max.
     x2 *= coef; // 63 bits max.
     x3 *= coef; // 63 bits max.
     
     x1 += x0 >>> 32;
     x0 &= MASK_32;
     
     x2 += x1 >>> 32;
     x1 &= MASK_32;
     
     x3 += x2 >>> 32;
     x2 &= MASK_32;
     
     // Adjusts powers.
     pow2 += i;
     n -= i;
     
     // Normalizes (x3 should be 32 bits max).
     long carry = x3 >>> 32;
     if (carry != 0) { // Shift.
     x0 = x1;
     x1 = x2;
     x2 = x3 & MASK_32;
     x3 = carry;
     pow2 += 32;
     }
     }
     
     // Merges registers to a 63 bits mantissa.
     int shift = 31 - MathLib.bitLength(x3); // -1..30
     pow2 -= shift;
     m = (shift < 0) ?
     (x3 << 31) | (x2 >>> 1) : // x3 is 32 bits.
     (((x3 << 32) | x2) << shift) | (x1 >>> (32 - shift));
     
     } else { // n < 0
     
     // Works with x1:x0 126 bits register.
     long x1 = m; // 63 bits.
     long x0 = 0; // 63 bits.
     while (true) {
     
     // Normalizes x1:x0
     int shift = 63 - MathLib.bitLength(x1);
     x1 <<= shift;
     x1 |= x0 >>> (63 - shift);
     x0 = (x0 << shift) & MASK_63;
     pow2 -= shift;
     
     // Checks if division has to be performed.
     if (n == 0)
     break; // Done.
     
     // Retrieves power of 5 divisor.
     int i = (-n >= POW5_INT.length) ? POW5_INT.length - 1 : -n;
     int divisor = POW5_INT[i];
     
     // Performs the division (126 bits by 31 bits).
     long wh = (x1 >>> 32);
     long qh = wh / divisor;
     long r = wh - qh * divisor;
     long wl = (r << 32) | (x1 & MASK_32);
     long ql = wl / divisor;
     r = wl - ql * divisor;
     x1 = (qh << 32) | ql;
     
     wh = (r << 31) | (x0 >>> 32);
     qh = wh / divisor;
     r = wh - qh * divisor;
     wl = (r << 32) | (x0 & MASK_32);
     ql = wl / divisor;
     x0 = (qh << 32) | ql;
     
     // Adjusts powers.
     n += i;
     pow2 -= i;
     }
     m = x1;
     }
     if (pow2 > 0) throw new ArithmeticException("Overflow");
     if (pow2 < -63) return 0;
     m = (m >> -pow2) + ((m >> -(pow2 + 1)) & 1) ; // Rounding.
     return isNegative ? -m : m;
     }
     /**/

    /**
     * Returns the largest power of 2 that is less than or equal to the
     * the specified positive value.
     *
     * @param d the <code>double</code> number.
     * @return <code>floor(Log2(abs(d)))</code>
     * @throws ArithmeticException if <code>d &lt;= 0<code> or <code>d</code>
     *         is <code>NaN</code> or <code>Infinity</code>.
     * @JVM-1.1+@
     public static int floorLog2(double d) {
     if (d <= 0) throw new ArithmeticException("Negative number or zero");
     long bits = Double.doubleToLongBits(d);
     int exp = ((int)(bits >> 52)) & 0x7FF;
     if (exp == 0x7FF) throw new ArithmeticException("Infinity or NaN");
     if (exp == 0)  // Degenerated.
     return floorLog2(d * 18014398509481984L) - 54;  // 2^54 Exact.       
     return exp - 1023;
     }
     /**/

    /**
     * Returns the largest power of 10 that is less than or equal to the
     * the specified positive value.
     *
     * @param d the <code>double</code> number.
     * @return <code>floor(Log10(abs(d)))</code>
     * @throws ArithmeticException if <code>d &lt;= 0<code> or <code>d</code>
     *         is <code>NaN</code> or <code>Infinity</code>.
     * @JVM-1.1+@
     public static int floorLog10(double d) {
     int guess = (int) (LOG2_DIV_LOG10 * MathLib.floorLog2(d));
     double pow10 = MathLib.toDoublePow10(1, guess);
     if ((pow10 <= d) && (pow10 * 10 > d)) // Good guess.
     return guess;
     if (pow10 > d)  // Too large.
     return guess - 1;
     return guess + 1;
     }    
     private static final double LOG2_DIV_LOG10 = 0.3010299956639811952137388947;
     /**/

    /**
     * The natural logarithm.
     * @JVM-1.1+@
     public static final double E = 2.71828182845904523536028747135266;
     
     /**
     * The ratio of the circumference of a circle to its diameter.
     * @JVM-1.1+@*/
     public static final double PI = 3.1415926535897932384626433832795;

     /**
     * Half the ratio of the circumference of a circle to its diameter.
     * @JVM-1.1+@*/
     public static final double HALF_PI = 1.5707963267948966192313216916398;

     /**
     * Twice the ratio of the circumference of a circle to its diameter.
     * @JVM-1.1+@*/
     public static final double TWO_PI = 6.283185307179586476925286766559;
     
     /**
     * Four time the ratio of the circumference of a circle to its diameter.
     * @JVM-1.1+@
     public static final double FOUR_PI = 12.566370614359172953850573533118;
     
     /**
     * Holds {@link #PI} * {@link #PI}.
     * @JVM-1.1+@
     public static final double PI_SQUARE = 9.8696044010893586188344909998762;
     
     /**
     * The natural logarithm of two.
     * @JVM-1.1+@
     public static final double LOG2 = 0.69314718055994530941723212145818;
     
     /**
     * The natural logarithm of ten.
     * @JVM-1.1+@
     public static final double LOG10 = 2.3025850929940456840179914546844;
     
     /**
     * The square root of two.
     * @JVM-1.1+@
     public static final double SQRT2 = 1.4142135623730950488016887242097;
     
     /**
     * Not-A-Number.
     * @JVM-1.1+@ */
     public static final double NaN = 0.0 / 0.0;

     /**
     * Infinity.
     * @JVM-1.1+@
     public static final double Infinity = 1.0 / 0.0;

     /**/

    /**
     * Converts an angle in degrees to radians.
     *
     * @param degrees the angle in degrees.
     * @return the specified angle in radians.
     * @JVM-1.1+@
     public static double toRadians(double degrees) {
     return degrees * (PI / 180.0);
     }
     /**/

    /**
     * Converts an angle in radians to degrees.
     *
     * @param radians the angle in radians.
     * @return the specified angle in degrees.
     * @JVM-1.1+@
     public static double toDegrees(double radians) {
     return radians * (180.0 / PI);
     }
     /**/

    /**
     * Returns the positive square root of the specified value.
     * 
     * @param x the value.
     * @return <code>java.lang.Math.sqrt(x)</code>
     * @JVM-1.1+@ */
     public static double sqrt(double x) {
     return Math.sqrt(x); // CLDC 1.1
     }
     /**/

    /**
     * Returns the remainder of the division of the specified two arguments.
     *
     * @param x the dividend.
     * @param y the divisor.
     * @return <code>x - round(x / y) * y</code>
     * @JVM-1.1+@
     public static double rem(double x, double y) {
     double tmp = x / y;
     if (MathLib.abs(tmp) <= Long.MAX_VALUE) { 
     return x - MathLib.round(tmp) * y;
     } else {
     return NaN;
     }
     }
     /**/

    /**
     * Returns the smallest (closest to negative infinity) 
     * <code>double</code> value that is not less than the argument and is 
     * equal to a mathematical integer.
     *
     * @param x the value.
     * @return <code>java.lang.Math.ceil(x)</code>
     * @JVM-1.1+@
     public static double ceil(double x) {
     return Math.ceil(x); // CLDC 1.1
     }
     /**/

    /**
     * Returns the largest (closest to positive infinity) 
     * <code>double</code> value that is not greater than the argument and 
     * is equal to a mathematical integer.
     *
     * @param x the value.
     * @return <code>java.lang.Math.ceil(x)</code>
     * @JVM-1.1+@
     public static double floor(double x) {
     return Math.floor(x); // CLDC 1.1
     }
     /**/

    /**
     * Returns the trigonometric sine of the specified angle in radians.
     * 
     * @param radians the angle in radians.
     * @return <code>java.lang.Math.sin(radians)</code>
     * @JVM-1.1+@
     public static double sin(double radians) {
     return Math.sin(radians); // CLDC 1.1
     }
     /**/

    /**
     * Returns the trigonometric cosine of the specified angle in radians.
     * 
     * @param radians the angle in radians.
     * @return <code>java.lang.Math.cos(radians)</code>
     * @JVM-1.1+@
     public static double cos(double radians) {
     return Math.cos(radians); // CLDC 1.1
     }
     /**/

    /**
     * Returns the trigonometric tangent of the specified angle in radians.
     * 
     * @param radians the angle in radians.
     * @return <code>java.lang.Math.tan(radians)</code>
     * @JVM-1.1+@
     public static double tan(double radians) {
     return Math.tan(radians); // CLDC 1.1
     }
     /**/

    /**
     * Returns the arc sine of the specified value, 
     * in the range of -<i>pi</i>/2 through <i>pi</i>/2. 
     *
     * @param x the value whose arc sine is to be returned.
     * @return the arc sine in radians for the specified value.
     * @JVM-1.1+@ */
     public static double asin(double x) {
	     if (x < -1.0 || x > 1.0) return MathLib.NaN;
	     if (x == -1.0) return - HALF_PI;
	     if (x == 1.0) return HALF_PI;
	     return MathLib.atan(x / MathLib.sqrt(1.0 - x * x));
     }

    /**
     * Returns the arc cosine of the specified value,
     * in the range of 0.0 through <i>pi</i>. 
     *
     * @param x the value whose arc cosine is to be returned.
     * @return the arc cosine in radians for the specified value.
     * @JVM-1.1+@*/
     public static double acos(double x) {
     return HALF_PI - MathLib.asin(x);
     }
     /**/

    /**
     * Returns the arc tangent of the specified value,
     * in the range of -<i>pi</i>/2 through <i>pi</i>/2.  
     *
     * @param x the value whose arc tangent is to be returned.
     * @return the arc tangent in radians for the specified value.
     * @see <a href="http://mathworld.wolfram.com/InverseTangent.html">
     *      Inverse Tangent -- from MathWorld</a> 
     * @JVM-1.1+@ */
     public static double atan(double x) {
     return MathLib._atan(x);
     }
     /**/

    /**
     * Returns the angle theta such that
     * <code>(x == cos(theta)) && (y == sin(theta))</code>. 
     *
     * @param y the y value.
     * @param x the x value.
     * @return the angle theta in radians.
     * @JVM-1.1+@*/
     public static double atan2(double y, double x) {
     final double epsilon = 1E-128;
     if (MathLib.abs(x) > epsilon) {
     double temp = MathLib.atan(MathLib.abs(y) / MathLib.abs(x));
     if( x < 0.0 ) temp = PI - temp;
     if( y < 0.0 ) temp = TWO_PI - temp;
     return temp;
     } else if( y >  epsilon) {
     return HALF_PI;
     } else if( y < -epsilon) {
     return 3 * HALF_PI;
     } else {
     return 0.0; 
     }
     }
     /**/

    /**
     * Returns the hyperbolic sine of x.
     * 
     * @param x the value for which the hyperbolic sine is calculated.
     * @return <code>(exp(x) - exp(-x)) / 2</code>
     * @JVM-1.1+@*/
     public static double sinh(double x) {
     return (MathLib.exp(x) - MathLib.exp(-x)) * 0.5;
     }
     /**/

    /**
     * Returns the hyperbolic cosine of x.
     * 
     * @param x the value for which the hyperbolic cosine is calculated.
     * @return <code>(exp(x) + exp(-x)) / 2</code>
     * @JVM-1.1+@*/
     public static double cosh(double x) {
     return (MathLib.exp(x) + MathLib.exp(-x)) * 0.5;
     }
     /**/

    /**
     * Returns the hyperbolic tangent of x.
     * 
     * @param x the value for which the hyperbolic tangent is calculated.
     * @return <code>(exp(2 * x) - 1) / (exp(2 * x) + 1)</code>
     * @JVM-1.1+@*/
     public static double tanh(double x) {
     return (MathLib.exp(2 * x) - 1) / (MathLib.exp(2 * x) + 1);
     }
     /**/

    /**
     * Returns <i>{@link #E e}</i> raised to the specified power.
     *
     * @param x the exponent.
     * @return <code><i>e</i><sup>x</sup></code>
     * @see <a href="http://mathworld.wolfram.com/ExponentialFunction.html">
     *      Exponential Function -- from MathWorld</a> 
     * @JVM-1.1+@*/
     public static double exp(double x) {
     return MathLib._ieee754_exp(x);
     }
     /**/

    /**
     * Returns the natural logarithm (base <i>{@link #E e}</i>) of the specified
     * value.
     *
     * @param x the value greater than <code>0.0</code>.
     * @return the value y such as <code><i>e</i><sup>y</sup> == x</code>
     * @JVM-1.1+@*/
     public static double log(double x) {
     return MathLib._ieee754_log(x);
     }
     /**/

    /**
     * Returns the decimal logarithm of the specified value.
     *
     * @param x the value greater than <code>0.0</code>.
     * @return the value y such as <code>10<sup>y</sup> == x</code>
     * @JVM-1.1+@*/
     public static double log10(double x) {
     return log(x) * INV_LOG10;
     }
     private static double INV_LOG10 = 0.43429448190325182765112891891661;    
     /**/

    /**
     * Returns the value of the first argument raised to the power of the
     * second argument. 
     *
     * @param x the base.
     * @param y the exponent.
     * @return <code>x<sup>y</sup></code>
     * @JVM-1.1+@*/
     public static double pow(double x, double y) {
     /**/
    /* @JVM-1.4+@ // Use java.lang.Math value. 
     if (true) return Math.pow(x, y);
     /**/
    /* @JVM-1.1+@ // Else (J2ME) use close approximation (+/- LSB)*/
     if ((x < 0) && (y == (int)y)) return 
     (((int)y) & 1) == 0 ? pow(-x, y) : -pow(-x, y);
     return MathLib.exp(y * MathLib.log(x));
     }
     /**/

    /**
     * Returns the closest <code>int</code> to the specified argument. 
     *
     * @param f the <code>float</code> value to be rounded to a <code>int</code>
     * @return the nearest <code>int</code> value.
     * @JVM-1.1+@
     public static int round(float f) {
     return (int) floor(f + 0.5f);
     }
     /**/

    /**
     * Returns the closest <code>long</code> to the specified argument. 
     *
     * @param d the <code>double</code> value to be rounded to a 
     *        <code>long</code>
     * @return the nearest <code>long</code> value.
     * @JVM-1.1+@
     public static long round(double d) {
     return (long) floor(d + 0.5d);
     }
     /**/

    /**
     * Returns a random number between zero and one.
     *  
     * @return  a <code>double</code> greater than or equal 
     *          to <code>0.0</code> and less than <code>1.0</code>.
     * @JVM-1.1+@
     public static double random() {
     return random(0, Integer.MAX_VALUE) * NORMALIZATION_FACTOR;
     }
     private static final double NORMALIZATION_FACTOR = -1.0 / Integer.MIN_VALUE;
     /**/

    /**
     * Returns the absolute value of the specified <code>int</code> argument.
     *
     * @param i the <code>int</code> value.
     * @return <code>i</code> or <code>-i</code>
     */
    public static int abs(int i) {
        return (i < 0) ? -i : i;
    }

    /**
     * Returns the absolute value of the specified <code>long</code> argument.
     *
     * @param l the <code>long</code> value.
     * @return <code>l</code> or <code>-l</code>
     */
    public static long abs(long l) {
        return (l < 0) ? -l : l;
    }

    /**
     * Returns the absolute value of the specified <code>float</code> argument.
     *
     * @param f the <code>float</code> value.
     * @return <code>f</code> or <code>-f</code>
     * @JVM-1.1+@
     public static float abs(float f) {
     return (f < 0) ? -f : f;
     }
     /**/

    /**
     * Returns the absolute value of the specified <code>double</code> argument.
     *
     * @param d the <code>double</code> value.
     * @return <code>d</code> or <code>-d</code>
     * @JVM-1.1+@ */
     public static double abs(double d) {
     return (d < 0) ? -d : d;
     }
     /**/

    /**
     * Returns the greater of two <code>int</code> values. 
     *
     * @param x the first value.
     * @param y the second value.
     * @return the larger of <code>x</code> and <code>y</code>.
     */
    public static int max(int x, int y) {
        return (x >= y) ? x : y;
    }

    /**
     * Returns the greater of two <code>long</code> values. 
     *
     * @param x the first value.
     * @param y the second value.
     * @return the larger of <code>x</code> and <code>y</code>.
     */
    public static long max(long x, long y) {
        return (x >= y) ? x : y;
    }

    /**
     * Returns the greater of two <code>float</code> values. 
     *
     * @param x the first value.
     * @param y the second value.
     * @return the larger of <code>x</code> and <code>y</code>.
     * @JVM-1.1+@
     public static float max(float x, float y) {
     return (x >= y) ? x : y;
     }
     /**/

    /**
     * Returns the greater of two <code>double</code> values. 
     *
     * @param x the first value.
     * @param y the second value.
     * @return the larger of <code>x</code> and <code>y</code>.
     * @JVM-1.1+@
     public static double max(double x, double y) {
     return (x >= y) ? x : y;
     }
     /**/

    /**
     * Returns the smaller of two <code>int</code> values. 
     *
     * @param x the first value.
     * @param y the second value.
     * @return the smaller of <code>x</code> and <code>y</code>.
     */
    public static int min(int x, int y) {
        return (x < y) ? x : y;
    }

    /**
     * Returns the smaller of two <code>long</code> values. 
     *
     * @param x the first value.
     * @param y the second value.
     * @return the smaller of <code>x</code> and <code>y</code>.
     */
    public static long min(long x, long y) {
        return (x < y) ? x : y;
    }

    /**
     * Returns the smaller of two <code>float</code> values. 
     *
     * @param x the first value.
     * @param y the second value.
     * @return the smaller of <code>x</code> and <code>y</code>.
     * @JVM-1.1+@
     public static float min(float x, float y) {
     return (x < y) ? x : y;
     }
     /**/

    /**
     * Returns the smaller of two <code>double</code> values. 
     *
     * @param x the first value.
     * @param y the second value.
     * @return the smaller of <code>x</code> and <code>y</code>.
     * @JVM-1.1+@
     public static double min(double x, double y) {
     return (x < y) ? x : y;
     }
     /**/

     static final double atanhi[] = {
     4.63647609000806093515e-01, // atan(0.5)hi 0x3FDDAC67, 0x0561BB4F
     7.85398163397448278999e-01, // atan(1.0)hi 0x3FE921FB, 0x54442D18 
     9.82793723247329054082e-01, // atan(1.5)hi 0x3FEF730B, 0xD281F69B 
     1.57079632679489655800e+00, // atan(inf)hi 0x3FF921FB, 0x54442D18 
     };
     static final double atanlo[] = {
     2.26987774529616870924e-17, // atan(0.5)lo 0x3C7A2B7F, 0x222F65E2 
     3.06161699786838301793e-17, // atan(1.0)lo 0x3C81A626, 0x33145C07
     1.39033110312309984516e-17, // atan(1.5)lo 0x3C700788, 0x7AF0CBBD
     6.12323399573676603587e-17, // atan(inf)lo 0x3C91A626, 0x33145C07 
     };
     static final double aT[] = {
     3.33333333333329318027e-01, // 0x3FD55555, 0x5555550D 
     -1.99999999998764832476e-01, // 0xBFC99999, 0x9998EBC4 
     1.42857142725034663711e-01, // 0x3FC24924, 0x920083FF 
     -1.11111104054623557880e-01, // 0xBFBC71C6, 0xFE231671 
     9.09088713343650656196e-02, // 0x3FB745CD, 0xC54C206E 
     -7.69187620504482999495e-02, // 0xBFB3B0F2, 0xAF749A6D 
     6.66107313738753120669e-02, // 0x3FB10D66, 0xA0D03D51 
     -5.83357013379057348645e-02, // 0xBFADDE2D, 0x52DEFD9A 
     4.97687799461593236017e-02, // 0x3FA97B4B, 0x24760DEB 
     -3.65315727442169155270e-02, // 0xBFA2B444, 0x2C6A6C2F 
     1.62858201153657823623e-02, // 0x3F90AD3A, 0xE322DA11 
     };
     static final double 
     one   = 1.0,
     huge   = 1.0e300;
     static double _atan(double x)
     {
     double w,s1,s2,z;
     int ix,hx,id;
     long xBits = Double.doubleToLongBits(x);
     int __HIx = (int) (xBits >> 32);
     int __LOx = (int) xBits;

     hx = __HIx;
     ix = hx&0x7fffffff;
     if(ix>=0x44100000) {	// if |x| >= 2^66 
     if(ix>0x7ff00000||
     (ix==0x7ff00000&&(__LOx!=0)))
     return x+x;		// NaN
     if(hx>0) return  atanhi[3]+atanlo[3];
     else     return -atanhi[3]-atanlo[3];
     } if (ix < 0x3fdc0000) {	// |x| < 0.4375
     if (ix < 0x3e200000) {	// |x| < 2^-29
     if(huge+x>one) return x;	// raise inexact
     }
     id = -1;
     } else {
     x = MathLib.abs(x);
     if (ix < 0x3ff30000) {		// |x| < 1.1875
     if (ix < 0x3fe60000) {	// 7/16 <=|x|<11/16
     id = 0; x = (2.0*x-one)/(2.0+x); 
     } else {			// 11/16<=|x|< 19/16
     id = 1; x  = (x-one)/(x+one); 
     }
     } else {
     if (ix < 0x40038000) {	// |x| < 2.4375
     id = 2; x  = (x-1.5)/(one+1.5*x);
     } else {			// 2.4375 <= |x| < 2^66
     id = 3; x  = -1.0/x;
     }
     }}
     // end of argument reduction
     z = x*x;
     w = z*z;
     // break sum from i=0 to 10 aT[i]z**(i+1) into odd and even poly
     s1 = z*(aT[0]+w*(aT[2]+w*(aT[4]+w*(aT[6]+w*(aT[8]+w*aT[10])))));
     s2 = w*(aT[1]+w*(aT[3]+w*(aT[5]+w*(aT[7]+w*aT[9]))));
     if (id<0) return x - x*(s1+s2);
     else {
     z = atanhi[id] - ((x*(s1+s2) - atanlo[id]) - x);
     return (hx<0)? -z:z;
     }
     }
     /**/

     static final double
     ln2_hi  =  6.93147180369123816490e-01,	// 3fe62e42 fee00000
     ln2_lo  =  1.90821492927058770002e-10,	// 3dea39ef 35793c76
     two54   =  1.80143985094819840000e+16,  // 43500000 00000000
     Lg1 = 6.666666666666735130e-01,  // 3FE55555 55555593
     Lg2 = 3.999999999940941908e-01,  // 3FD99999 9997FA04
     Lg3 = 2.857142874366239149e-01,  // 3FD24924 94229359
     Lg4 = 2.222219843214978396e-01,  // 3FCC71C5 1D8E78AF
     Lg5 = 1.818357216161805012e-01,  // 3FC74664 96CB03DE
     Lg6 = 1.531383769920937332e-01,  // 3FC39A09 D078C69F
     Lg7 = 1.479819860511658591e-01;  // 3FC2F112 DF3E5244
     static final double zero   =  0.0;
     static double _ieee754_log(double x)
     {
     double hfsq,f,s,z,R,w,t1,t2,dk;
     int k,hx,i,j;
     int lx; // unsigned 

     long xBits = Double.doubleToLongBits(x);
     hx = (int) (xBits >> 32);
     lx = (int) xBits;

     k=0;
     if (hx < 0x00100000) {			// x < 2**-1022 
     if (((hx&0x7fffffff)|lx)==0) 
     return -two54/zero;		// log(+-0)=-inf
     if (hx<0) return (x-x)/zero;	// log(-#) = NaN
     k -= 54; x *= two54; // subnormal number, scale up x
     xBits = Double.doubleToLongBits(x);
     hx = (int) (xBits >> 32); // high word of x
     } 
     if (hx >= 0x7ff00000) return x+x;
     k += (hx>>20)-1023;
     hx &= 0x000fffff;
     i = (hx+0x95f64)&0x100000;
     xBits = Double.doubleToLongBits(x);
     int HIx = hx|(i^0x3ff00000);	// normalize x or x/2
     xBits = ((HIx & 0xFFFFFFFFL) << 32) | (xBits & 0xFFFFFFFFL);
     x = Double.longBitsToDouble(xBits);
     k += (i>>20);
     f = x-1.0;
     if((0x000fffff&(2+hx))<3) {	// |f| < 2**-20
     if(f==zero) if(k==0) return zero;  else {dk=(double)k;
     return dk*ln2_hi+dk*ln2_lo;}
     R = f*f*(0.5-0.33333333333333333*f);
     if(k==0) return f-R; else {dk=(double)k;
     return dk*ln2_hi-((R-dk*ln2_lo)-f);}
     }
     s = f/(2.0+f); 
     dk = (double)k;
     z = s*s;
     i = hx-0x6147a;
     w = z*z;
     j = 0x6b851-hx;
     t1= w*(Lg2+w*(Lg4+w*Lg6)); 
     t2= z*(Lg1+w*(Lg3+w*(Lg5+w*Lg7))); 
     i |= j;
     R = t2+t1;
     if(i>0) {
     hfsq=0.5*f*f;
     if(k==0) return f-(hfsq-s*(hfsq+R)); else
     return dk*ln2_hi-((hfsq-(s*(hfsq+R)+dk*ln2_lo))-f);
     } else {
     if(k==0) return f-s*(f-R); else
     return dk*ln2_hi-((s*(f-R)-dk*ln2_lo)-f);
     }
     }    
     static final double
     halF[]	= {0.5,-0.5,},
     twom1000= 9.33263618503218878990e-302,     // 2**-1000=0x01700000,0
     o_threshold=  7.09782712893383973096e+02,  // 0x40862E42, 0xFEFA39EF
     u_threshold= -7.45133219101941108420e+02,  // 0xc0874910, 0xD52D3051
     ln2HI[]   ={ 6.93147180369123816490e-01,  // 0x3fe62e42, 0xfee00000
     -6.93147180369123816490e-01,},// 0xbfe62e42, 0xfee00000
     ln2LO[]   ={ 1.90821492927058770002e-10,  // 0x3dea39ef, 0x35793c76
     -1.90821492927058770002e-10,},// 0xbdea39ef, 0x35793c76
     invln2 =  1.44269504088896338700e+00, // 0x3ff71547, 0x652b82fe
     P1   =  1.66666666666666019037e-01, // 0x3FC55555, 0x5555553E
     P2   = -2.77777777770155933842e-03, // 0xBF66C16C, 0x16BEBD93
     P3   =  6.61375632143793436117e-05, // 0x3F11566A, 0xAF25DE2C
     P4   = -1.65339022054652515390e-06, // 0xBEBBBD41, 0xC5D26BF1
     P5   =  4.13813679705723846039e-08; // 0x3E663769, 0x72BEA4D0
     static double _ieee754_exp(double x)	// default IEEE double exp
     {
     double y,hi = 0,lo = 0,c,t;
     int k = 0,xsb;
     int hx; // Unsigned.
     long xBits = Double.doubleToLongBits(x);
     int __HIx = (int) (xBits >> 32);
     int __LOx = (int) xBits;

     hx  = __HIx;	// high word of x
     xsb = (hx>>31)&1;		// sign bit of x
     hx &= 0x7fffffff;		// high word of |x|

     // filter out non-finite argument
     if(hx >= 0x40862E42) {			// if |x|>=709.78...
     if(hx>=0x7ff00000) {
     if(((hx&0xfffff)|__LOx)!=0) 
     return x+x; 		// NaN
     else return (xsb==0)? x:0.0;	// exp(+-inf)={inf,0}
     }
     if(x > o_threshold) return huge*huge; // overflow
     if(x < u_threshold) return twom1000*twom1000; // underflow
     }

     // argument reduction
     if(hx > 0x3fd62e42) {		// if  |x| > 0.5 ln2 
     if(hx < 0x3FF0A2B2) {	// and |x| < 1.5 ln2
     hi = x-ln2HI[xsb]; lo=ln2LO[xsb]; k = 1-xsb-xsb;
     } else {
     k  = (int)(invln2*x+halF[xsb]);
     t  = k;
     hi = x - t*ln2HI[0];	// t*ln2HI is exact here
     lo = t*ln2LO[0];
     }
     x  = hi - lo;
     } 
     else if(hx < 0x3e300000)  {	// when |x|<2**-28
     if(huge+x>one) return one+x;// trigger inexact
     }
     else k = 0;

     // x is now in primary range
     t  = x*x;
     c  = x - t*(P1+t*(P2+t*(P3+t*(P4+t*P5))));
     if(k==0) 	return one-((x*c)/(c-2.0)-x); 
     else 		y = one-((lo-(x*c)/(2.0-c))-hi);
     long yBits = Double.doubleToLongBits(y);
     int __HIy = (int) (yBits >> 32);
     if(k >= -1021) {
     __HIy += (k<<20);	// add k to y's exponent
     yBits = ((__HIy & 0xFFFFFFFFL) << 32) | (yBits & 0xFFFFFFFFL);
     y = Double.longBitsToDouble(yBits);
     return y;
     } else {
     __HIy += ((k+1000)<<20);// add k to y's exponent 
     yBits = ((__HIy & 0xFFFFFFFFL) << 32) | (yBits & 0xFFFFFFFFL);
     y = Double.longBitsToDouble(yBits);
     return y*twom1000;
     }
     }
     /**/

}