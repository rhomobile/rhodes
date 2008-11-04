/*
 * @(#)MathContext.java	1.2 03/12/19
 *
 * Copyright 2004 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/*
 * @(#)MathContext.java  1.x 01/xx/xx
 *
 * Copyright IBM Corporation, 1997, 2001. All Rights Reserved.
 *
 * This software is the proprietary information of Sun Microsystems, Inc.
 * and IBM Corporation.  Use is subject to license terms.
 *
 */

package java.math;
import java.io.*;

/**
 * Immutable objects which encapsulate the context settings which
 * describe certain rules for numerical operators, such as those
 * implemented by the {@link BigDecimal} class.
 * 
 * <p>The base-independent settings are:
 * <ol>
 * <li><tt>precision</tt>:
 * the number of digits to be used for an operation; results are
 * rounded to this precision
 * 
 * <li><tt>roundingMode</tt>:
 * a {@link RoundingMode} object which specifies the algorithm to be
 * used for rounding.
 * </ol>
 *
 * @see     BigDecimal
 * @see     RoundingMode
 * @author  Mike Cowlishaw
 * @author  Joseph D. Darcy
 */

public final class MathContext {//implements Serializable {

    /* ----- Constants ----- */

    // defaults for constructors
    private static final int DEFAULT_DIGITS = 9;
    private static final RoundingMode DEFAULT_ROUNDINGMODE = RoundingMode.HALF_UP;
    // Smallest values for digits (Maximum is Integer.MAX_VALUE)
    private static final int MIN_DIGITS = 0;

    // Serialization version
    private static final long serialVersionUID = 5579720004786848255L;

    /* ----- Public Properties ----- */
    /**
     *  A <tt>MathContext</tt> object whose settings have the values
     *  required for unlimited precision arithmetic.
     *  The values of the settings are:
     *  <code>
     *  precision=0 roundingMode=HALF_UP
     *  </code>
     */
    public static final MathContext UNLIMITED =
        new MathContext(0, RoundingMode.HALF_UP);

    /**
     *  A <tt>MathContext</tt> object with a precision setting
     *  matching the IEEE 754R Decimal32 format, 7 digits, and a
     *  rounding mode of {@link RoundingMode#HALF_EVEN HALF_EVEN}, the
     *  IEEE 754R default.
     */
    public static final MathContext DECIMAL32 =
        new MathContext(7, RoundingMode.HALF_EVEN);

    /**
     *  A <tt>MathContext</tt> object with a precision setting
     *  matching the IEEE 754R Decimal64 format, 16 digits, and a
     *  rounding mode of {@link RoundingMode#HALF_EVEN HALF_EVEN}, the
     *  IEEE 754R default.
     */
    public static final MathContext DECIMAL64 =
        new MathContext(16, RoundingMode.HALF_EVEN);

    /**
     *  A <tt>MathContext</tt> object with a precision setting
     *  matching the IEEE 754R Decimal128 format, 34 digits, and a
     *  rounding mode of {@link RoundingMode#HALF_EVEN HALF_EVEN}, the
     *  IEEE 754R default.
     */
    public static final MathContext DECIMAL128 =
        new MathContext(34, RoundingMode.HALF_EVEN);

    /* ----- Shared Properties ----- */
    /**
     * The number of digits to be used for an operation.  A value of 0
     * indicates that unlimited precision (as many digits as are
     * required) will be used.  Note that leading zeros (in the
     * coefficient of a number) are never significant.
     * 
     * <p><tt>precision</tt> will always be non-negative.
     *
     * @serial
     */
    final int precision;

    /**
     * The rounding algorithm to be used for an operation.
     *
     * @see RoundingMode
     * @serial
     */
    final RoundingMode roundingMode;

    /**
     *  Lookaside for the rounding points (the numbers which determine
     *  whether the coefficient of a number will require rounding).
     *  These will be present if precision&gt;0 and
     *  precision&lt;=MAX_LOOKASIDE.  In this case they will share the
     *  <tt>BigInteger int[]</tt> array.  Note that the transients
     *  cannot be <tt>final</tt> because they are reconstructed on
     *  deserialization.
     */
    transient BigInteger roundingMax = null;
    transient BigInteger roundingMin = null;
    private static final int MAX_LOOKASIDE = 1000;

    /* ----- Constructors ----- */

    /**
     * Constructs a new <tt>MathContext</tt> with the specified
     * precision and the {@link RoundingMode#HALF_UP HALF_UP} rounding
     * mode.
     *
     * @param setPrecision The non-negative <tt>int</tt> precision setting.
     * @throws IllegalArgumentException <tt>setPrecision</tt> parameter less 
     *         than zero.
     */
    public MathContext(int setPrecision) {
        this(setPrecision, DEFAULT_ROUNDINGMODE);
        return;
    }

    /**
     * Constructs a new <tt>MathContext</tt> with a specified
     * precision and rounding mode.
     *
     * @param setPrecision The non-negative <tt>int</tt> precision setting.
     * @param setRoundingMode The rounding mode to use.
     * @throws IllegalArgumentException <tt>setPrecision</tt> parameter less 
     *         than zero.
     */
    public MathContext(int setPrecision, 
    		RoundingMode setRoundingMode) {
    	
        if (setPrecision < MIN_DIGITS)
            throw new IllegalArgumentException("Digits < 0");
        //if (setRoundingMode == null)
        //    throw new NullPointerException("null RoundingMode");

        precision = setPrecision;
        if (precision > 0 && precision <= MAX_LOOKASIDE) {
            roundingMax = BigInteger.TEN.pow(precision);
            roundingMin = roundingMax.negate();
        }

        roundingMode = setRoundingMode;
        return;
    }

    /**
     * Constructs a new <tt>MathContext</tt> from a string.
     *
     * The string must be in the same format as that produced by the
     * {@link #toString} method.
     * 
     * <p>An <tt>IllegalArgumentException</tt> is thrown if the precision
     * section of the string is out of range (&lt; 0) or the string is
     * not in the format created by the {@link #toString} method.
     *
     * @param val The string to be parsed
     * @throws IllegalArgumentException precision parameter out of range
     * or incorrect format
     */
    public MathContext(String val) {
        boolean bad = false;
        int setPrecision;
        if (val == null)
            throw new NullPointerException("null String");
        try { // any error here is a string format problem
            if (!val.startsWith("precision=")) throw new RuntimeException();
            int fence = val.indexOf(' ');    // could be -1
            int off = 10;                     // where value starts
            setPrecision = Integer.parseInt(val.substring(10, fence));

            if (!val.startsWith("roundingMode=", fence+1)) 
		throw new RuntimeException();
            off = fence + 1 + 13;
            String str = val.substring(off, val.length());
            roundingMode = RoundingMode.valueOf(Integer.parseInt(str));
        } catch (RuntimeException re) {
            throw new IllegalArgumentException("bad string format");
        }

        if (setPrecision < MIN_DIGITS)
            throw new IllegalArgumentException("Digits < 0");
        // the other parameters cannot be invalid if we got here
        precision = setPrecision;
        if (precision > 0 && precision <= MAX_LOOKASIDE) {
            roundingMax = BigInteger.TEN.pow(precision);
            roundingMin = roundingMax.negate();
        }
    }

    /**
     * Returns the <tt>precision</tt> setting.
     * This value is always non-negative.
     *
     * @return an <tt>int</tt> which is the value of the <tt>precision</tt>
     *         setting
     */
    public int getPrecision() {
        return precision;
    }

    /**
     * Returns the roundingMode setting.
     * This will be one of
     * {@link  RoundingMode#CEILING},
     * {@link  RoundingMode#DOWN},
     * {@link  RoundingMode#FLOOR},
     * {@link  RoundingMode#HALF_DOWN},
     * {@link  RoundingMode#HALF_EVEN},
     * {@link  RoundingMode#HALF_UP},
     * {@link  RoundingMode#UNNECESSARY}, or
     * {@link  RoundingMode#UP}.
     *
     * @return a <tt>RoundingMode</tt> object which is the value of the
     *         <tt>roundingMode</tt> setting
     */

    public RoundingMode getRoundingMode() {
        return roundingMode;
    }

    /**
     * Compares this <tt>MathContext</tt> with the specified
     * <tt>Object</tt> for equality.
     *
     * @param  x <tt>Object</tt> to which this <tt>MathContext</tt> is to 
     *         be compared.
     * @return <tt>true</tt> if and only if the specified <tt>Object</tt> is
     *         a <tt>MathContext</tt> object which has exactly the same 
     *         settings as this object.
     */
    public boolean equals(Object x){
        MathContext mc;
        if (!(x instanceof MathContext))
            return false;
        mc = (MathContext) x;
        return mc.precision == this.precision
            && mc.roundingMode == this.roundingMode; // no need for .equals()
    }

    /**
     * Returns the hash code for this <tt>MathContext</tt>.
     *
     * @return hash code for this <tt>MathContext</tt>
     */
    public int hashCode() {
        return this.precision + roundingMode.hashCode() * 59;
    }

    /** 
     * Returns the string representation of this <tt>MathContext</tt>.
     * The <tt>String</tt> returned represents the settings of the
     * <tt>MathContext</tt> object as two space-delimited words
     * (separated by a single space character, <tt>'&#92;u0020'</tt>,
     * and with no leading or trailing white space), as follows:
     * <ol>
     * <li>
     * The string <tt>&quot;precision=&quot;</tt>, immediately followed
     * by the value of the precision setting as a numeric string as if
     * generated by the {@link Integer#toString(int) Integer.toString}
     * method.
     *
     * <li>
     * The string <tt>&quot;roundingMode=&quot;</tt>, immediately
     * followed by the value of the <tt>roundingMode</tt> setting as a
     * word.  This word will be the same as the name of the
     * corresponding public constant in the {@link RoundingMode}
     * enum.
     * </ol>
     * <p>
     * For example:
     * <pre>
     * precision=9 roundingMode=HALF_UP
     * </pre>
     *
     * Additional words may be appended to the result of
     * <tt>toString</tt> in the future if more properties are added to
     * this class.
     *
     * @return a <tt>String</tt> representing the context settings.
     */
    public java.lang.String toString() {
        return "precision=" +		precision + " " +
	       "roundingMode=" +	roundingMode.toString();
    }

    // Private methods

    /**
     * Reconstitute the <tt>MathContext</tt> instance from a stream (that is,
     * deserialize it).
     *
     * @param s the stream being read.
     */
    /*private synchronized void readObject(java.io.ObjectInputStream s)
        throws java.io.IOException, ClassNotFoundException {
        s.defaultReadObject();     // read in all fields
        // validate possibly bad fields
        if (precision < MIN_DIGITS) {
            String message = "MathContext: invalid digits in stream";
            throw new java.io.StreamCorruptedException(message);
        }
        if (roundingMode == null) {
            String message = "MathContext: null roundingMode in stream";
            throw new java.io.StreamCorruptedException(message);
        }
        // Set the lookaside, if applicable
        if (precision <= MAX_LOOKASIDE) {
            roundingMax = BigInteger.TEN.pow(precision);
            roundingMin = roundingMax.negate();
        }
    }*/

}
