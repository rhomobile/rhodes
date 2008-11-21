/*
 * @(#)RoundingMode.java        1.3 04/06/18
 *
 * Copyright 2004 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/*
 * @(#)RoundingMode.java  1.x 01/xx/xx
 *
 * Copyright 2001 Sun Microsystems, Inc. All Rights Reserved.
 * Portions Copyright IBM Corporation, 2001. All Rights Reserved.
 *
 * This software is the proprietary information of Sun Microsystems, Inc.
 * Use is subject to license terms.
 *
 */
package j2me.math;

/**
 * Specifies a <i>rounding behavior</i> for numerical operations
 * capable of discarding precision. Each rounding mode indicates how
 * the least significant returned digit of a rounded result is to be
 * calculated.  If fewer digits are returned than the digits needed to
 * represent the exact numerical result, the discarded digits will be
 * referred to as the <i>discarded fraction</i> regardless the digits'
 * contribution to the value of the number.  In other words,
 * considered as a numerical value, the discarded fraction could have
 * an absolute value greater than one.
 *
 * <p>Each rounding mode description includes a table listing how
 * different two-digit decimal values would round to a one digit
 * decimal value under the rounding mode in question.  The result
 * column in the tables could be gotten by creating a
 * <tt>BigDecimal</tt> number with the specified value, forming a
 * {@link MathContext} object with the proper settings
 * (<tt>precision</tt> set to <tt>1</tt>, and the
 * <tt>roundingMode</tt> set to the rounding mode in question), and
 * calling {@link BigDecimal#round round} on this number with the
 * proper <tt>MathContext</tt>.  A summary table showing the results
 * of these rounding operations for all rounding modes appears below.
 *
 *<p>
 *<table border>
 * <caption top><h3>Summary of Rounding Operations Under Different Rounding Modes</h3></caption>
 * <tr><th></th><th colspan=8>Result of rounding input to one digit with the given 
 *                           rounding mode</th>
 * <tr valign=top>
 * <th>Input Number</th>         <th><tt>UP</tt></th> 
 *                                           <th><tt>DOWN</tt></th> 
 *                                                        <th><tt>CEILING</tt></th>
 *                                                                       <th><tt>FLOOR</tt></th>
 *                                                                                    <th><tt>HALF_UP</tt></th>
 *                                                                                                   <th><tt>HALF_DOWN</tt></th>
 *                                                                                                                    <th><tt>HALF_EVEN</tt></th>
 *                                                                                                                                     <th><tt>UNNECESSARY</tt></th>
 *    
 * <tr align=right><td>5.5</td>  <td>6</td>  <td>5</td>    <td>6</td>    <td>5</td>  <td>6</td>      <td>5</td>       <td>6</td>       <td>throw <tt>ArithmeticException</tt></td>
 * <tr align=right><td>2.5</td>  <td>3</td>  <td>2</td>    <td>3</td>    <td>2</td>  <td>3</td>      <td>2</td>       <td>2</td>       <td>throw <tt>ArithmeticException</tt></td>
 * <tr align=right><td>1.6</td>  <td>2</td>  <td>1</td>    <td>2</td>    <td>1</td>  <td>2</td>      <td>2</td>       <td>2</td>       <td>throw <tt>ArithmeticException</tt></td>
 * <tr align=right><td>1.1</td>  <td>2</td>  <td>1</td>    <td>2</td>    <td>1</td>  <td>1</td>      <td>1</td>       <td>1</td>       <td>throw <tt>ArithmeticException</tt></td>
 * <tr align=right><td>1.0</td>  <td>1</td>  <td>1</td>    <td>1</td>    <td>1</td>  <td>1</td>      <td>1</td>       <td>1</td>       <td>1</td>                                
 * <tr align=right><td>-1.0</td> <td>-1</td> <td>-1</td>   <td>-1</td>   <td>-1</td> <td>-1</td>     <td>-1</td>      <td>-1</td>      <td>-1</td>                               
 * <tr align=right><td>-1.1</td> <td>-2</td> <td>-1</td>   <td>-1</td>   <td>-2</td> <td>-1</td>     <td>-1</td>      <td>-1</td>      <td>throw <tt>ArithmeticException</tt></td>
 * <tr align=right><td>-1.6</td> <td>-2</td> <td>-1</td>   <td>-1</td>   <td>-2</td> <td>-2</td>     <td>-2</td>      <td>-2</td>      <td>throw <tt>ArithmeticException</tt></td>
 * <tr align=right><td>-2.5</td> <td>-3</td> <td>-2</td>   <td>-2</td>   <td>-3</td> <td>-3</td>     <td>-2</td>      <td>-2</td>      <td>throw <tt>ArithmeticException</tt></td>
 * <tr align=right><td>-5.5</td> <td>-6</td> <td>-5</td>   <td>-5</td>   <td>-6</td> <td>-6</td>     <td>-5</td>      <td>-6</td>      <td>throw <tt>ArithmeticException</tt></td>
 *</table>
 *
 * 
 * <p>This <tt>enum</tt> is intended to replace the integer-based
 * enumeration of rounding mode constants in {@link BigDecimal}
 * ({@link BigDecimal#ROUND_UP}, {@link BigDecimal#ROUND_DOWN},
 * etc. ).
 *
 * @see     BigDecimal
 * @see     MathContext
 * @version 1.x 01/xx/xx
 * @author  Josh Bloch
 * @author  Mike Cowlishaw
 * @author  Joseph D. Darcy
 */
public class RoundingMode {
        /**
         * Rounding mode to round away from zero.  Always increments the
         * digit prior to a non-zero discarded fraction.  Note that this
         * rounding mode never decreases the magnitude of the calculated
         * value.
         *
         *<p>Example:
         *<table border>
         *<tr valign=top><th>Input Number</th>
         *    <th>Input rounded to one digit<br> with <tt>UP</tt> rounding
         *<tr align=right><td>5.5</td>  <td>6</td>
         *<tr align=right><td>2.5</td>  <td>3</td>
         *<tr align=right><td>1.6</td>  <td>2</td>
         *<tr align=right><td>1.1</td>  <td>2</td>
         *<tr align=right><td>1.0</td>  <td>1</td>
         *<tr align=right><td>-1.0</td> <td>-1</td>
         *<tr align=right><td>-1.1</td> <td>-2</td>
         *<tr align=right><td>-1.6</td> <td>-2</td>
         *<tr align=right><td>-2.5</td> <td>-3</td>
         *<tr align=right><td>-5.5</td> <td>-6</td>
         *</table>
         */
        public final static RoundingMode UP = new RoundingMode(BigDecimal.ROUND_UP);
            
        /**
         * Rounding mode to round towards zero.  Never increments the digit
         * prior to a discarded fraction (i.e., truncates).  Note that this
         * rounding mode never increases the magnitude of the calculated value.
         *
         *<p>Example:
         *<table border>
         *<tr valign=top><th>Input Number</th>
         *    <th>Input rounded to one digit<br> with <tt>DOWN</tt> rounding
         *<tr align=right><td>5.5</td>  <td>5</td>
         *<tr align=right><td>2.5</td>  <td>2</td>
         *<tr align=right><td>1.6</td>  <td>1</td>
         *<tr align=right><td>1.1</td>  <td>1</td>
         *<tr align=right><td>1.0</td>  <td>1</td>
         *<tr align=right><td>-1.0</td> <td>-1</td>
         *<tr align=right><td>-1.1</td> <td>-1</td>
         *<tr align=right><td>-1.6</td> <td>-1</td>
         *<tr align=right><td>-2.5</td> <td>-2</td>
         *<tr align=right><td>-5.5</td> <td>-5</td>
         *</table>
         */
        public final static RoundingMode DOWN = new RoundingMode(BigDecimal.ROUND_DOWN);
            
        /**
         * Rounding mode to round towards positive infinity.  If the
         * result is positive, behaves as for <tt>RoundingMode.UP</tt>;
         * if negative, behaves as for <tt>RoundingMode.DOWN</tt>.  Note
         * that this rounding mode never decreases the calculated value.
         *
         *<p>Example:
         *<table border>
         *<tr valign=top><th>Input Number</th>
         *    <th>Input rounded to one digit<br> with <tt>CEILING</tt> rounding
         *<tr align=right><td>5.5</td>  <td>6</td>
         *<tr align=right><td>2.5</td>  <td>3</td>
         *<tr align=right><td>1.6</td>  <td>2</td>
         *<tr align=right><td>1.1</td>  <td>2</td>
         *<tr align=right><td>1.0</td>  <td>1</td>
         *<tr align=right><td>-1.0</td> <td>-1</td>
         *<tr align=right><td>-1.1</td> <td>-1</td>
         *<tr align=right><td>-1.6</td> <td>-1</td>
         *<tr align=right><td>-2.5</td> <td>-2</td>
         *<tr align=right><td>-5.5</td> <td>-5</td>
         *</table>
         */
        public final static RoundingMode CEILING = new RoundingMode(BigDecimal.ROUND_CEILING);

        /**
         * Rounding mode to round towards negative infinity.  If the
         * result is positive, behave as for <tt>RoundingMode.DOWN</tt>;
         * if negative, behave as for <tt>RoundingMode.UP</tt>.  Note that
         * this rounding mode never increases the calculated value.
         *
         *<p>Example:
         *<table border>
         *<tr valign=top><th>Input Number</th>
         *    <th>Input rounded to one digit<br> with <tt>FLOOR</tt> rounding
         *<tr align=right><td>5.5</td>  <td>5</td>
         *<tr align=right><td>2.5</td>  <td>2</td>
         *<tr align=right><td>1.6</td>  <td>1</td>
         *<tr align=right><td>1.1</td>  <td>1</td>
         *<tr align=right><td>1.0</td>  <td>1</td>
         *<tr align=right><td>-1.0</td> <td>-1</td>
         *<tr align=right><td>-1.1</td> <td>-2</td>
         *<tr align=right><td>-1.6</td> <td>-2</td>
         *<tr align=right><td>-2.5</td> <td>-3</td>
         *<tr align=right><td>-5.5</td> <td>-6</td>
         *</table>
         */
        public final static RoundingMode FLOOR = new RoundingMode(BigDecimal.ROUND_FLOOR);

        /**
         * Rounding mode to round towards &quot;nearest neighbor&quot;
         * unless both neighbors are equidistant, in which case round up.
         * Behaves as for <tt>RoundingMode.UP</tt> if the discarded
         * fraction is &gt;= 0.5; otherwise, behaves as for
         * <tt>RoundingMode.DOWN</tt>.  Note that this is the rounding
         * mode commonly taught at school.
         *
         *<p>Example:
         *<table border>
         *<tr valign=top><th>Input Number</th>
         *    <th>Input rounded to one digit<br> with <tt>HALF_UP</tt> rounding
         *<tr align=right><td>5.5</td>  <td>6</td>
         *<tr align=right><td>2.5</td>  <td>3</td>
         *<tr align=right><td>1.6</td>  <td>2</td>
         *<tr align=right><td>1.1</td>  <td>1</td>
         *<tr align=right><td>1.0</td>  <td>1</td>
         *<tr align=right><td>-1.0</td> <td>-1</td>
         *<tr align=right><td>-1.1</td> <td>-1</td>
         *<tr align=right><td>-1.6</td> <td>-2</td>
         *<tr align=right><td>-2.5</td> <td>-3</td>
         *<tr align=right><td>-5.5</td> <td>-6</td>
         *</table>
         */
        public final static RoundingMode HALF_UP = new RoundingMode(BigDecimal.ROUND_HALF_UP);

        /**
         * Rounding mode to round towards &quot;nearest neighbor&quot;
         * unless both neighbors are equidistant, in which case round
         * down.  Behaves as for <tt>RoundingMode.UP</tt> if the discarded
         * fraction is &gt; 0.5; otherwise, behaves as for
         * <tt>RoundingMode.DOWN</tt>.
         *
         *<p>Example:
         *<table border>
         *<tr valign=top><th>Input Number</th>
         *    <th>Input rounded to one digit<br> with <tt>HALF_DOWN</tt> rounding
         *<tr align=right><td>5.5</td>  <td>5</td>
         *<tr align=right><td>2.5</td>  <td>2</td>
         *<tr align=right><td>1.6</td>  <td>2</td>
         *<tr align=right><td>1.1</td>  <td>1</td>
         *<tr align=right><td>1.0</td>  <td>1</td>
         *<tr align=right><td>-1.0</td> <td>-1</td>
         *<tr align=right><td>-1.1</td> <td>-1</td>
         *<tr align=right><td>-1.6</td> <td>-2</td>
         *<tr align=right><td>-2.5</td> <td>-2</td>
         *<tr align=right><td>-5.5</td> <td>-5</td>
         *</table>
         */
        public final static RoundingMode HALF_DOWN = new RoundingMode(BigDecimal.ROUND_HALF_DOWN);

        /**
         * Rounding mode to round towards the &quot;nearest neighbor&quot;
         * unless both neighbors are equidistant, in which case, round
         * towards the even neighbor.  Behaves as for
         * <tt>RoundingMode.HALF_UP</tt> if the digit to the left of the
         * discarded fraction is odd; behaves as for
         * <tt>RoundingMode.HALF_DOWN</tt> if it's even.  Note that this
         * is the rounding mode that statistically minimizes cumulative
         * error when applied repeatedly over a sequence of calculations.
         * It is sometimes known as &quot;Banker's rounding,&quot; and is
         * chiefly used in the USA.  This rounding mode is analogous to
         * the rounding policy used for <tt>float</tt> and <tt>double</tt>
         * arithmetic in Java.
         *
         *<p>Example:
         *<table border>
         *<tr valign=top><th>Input Number</th>
         *    <th>Input rounded to one digit<br> with <tt>HALF_EVEN</tt> rounding
         *<tr align=right><td>5.5</td>  <td>6</td>
         *<tr align=right><td>2.5</td>  <td>2</td>
         *<tr align=right><td>1.6</td>  <td>2</td>
         *<tr align=right><td>1.1</td>  <td>1</td>
         *<tr align=right><td>1.0</td>  <td>1</td>
         *<tr align=right><td>-1.0</td> <td>-1</td>
         *<tr align=right><td>-1.1</td> <td>-1</td>
         *<tr align=right><td>-1.6</td> <td>-2</td>
         *<tr align=right><td>-2.5</td> <td>-2</td>
         *<tr align=right><td>-5.5</td> <td>-6</td>
         *</table>
         */
        public final static RoundingMode HALF_EVEN = new RoundingMode(BigDecimal.ROUND_HALF_EVEN);

        /**
         * Rounding mode to assert that the requested operation has an exact
         * result, hence no rounding is necessary.  If this rounding mode is
         * specified on an operation that yields an inexact result, an
         * <tt>ArithmeticException</tt> is thrown.
         *<p>Example:
         *<table border>
         *<tr valign=top><th>Input Number</th>
         *    <th>Input rounded to one digit<br> with <tt>UNNECESSARY</tt> rounding
         *<tr align=right><td>5.5</td>  <td>throw <tt>ArithmeticException</tt></td>
         *<tr align=right><td>2.5</td>  <td>throw <tt>ArithmeticException</tt></td>
         *<tr align=right><td>1.6</td>  <td>throw <tt>ArithmeticException</tt></td>
         *<tr align=right><td>1.1</td>  <td>throw <tt>ArithmeticException</tt></td>
         *<tr align=right><td>1.0</td>  <td>1</td>
         *<tr align=right><td>-1.0</td> <td>-1</td>
         *<tr align=right><td>-1.1</td> <td>throw <tt>ArithmeticException</tt></td>
         *<tr align=right><td>-1.6</td> <td>throw <tt>ArithmeticException</tt></td>
         *<tr align=right><td>-2.5</td> <td>throw <tt>ArithmeticException</tt></td>
         *<tr align=right><td>-5.5</td> <td>throw <tt>ArithmeticException</tt></td>     
         *</table>
         */
        public final static RoundingMode UNNECESSARY = new RoundingMode(BigDecimal.ROUND_UNNECESSARY);

    // Corresponding BigDecimal rounding constant
    final int oldMode;

    /**
     * Constructor
     *
     * @param oldMode The <tt>BigDecimal</tt> constant corresponding to 
     *        this mode
     */
    public RoundingMode(int oldMode) {
        this.oldMode = oldMode;
    }

    public int getIntValue(){ return oldMode; }
    /**
     * Returns the <tt>RoundingMode</tt> object corresponding to a
     * legacy integer rounding mode constant in {@link BigDecimal}.
     *
     * @param  rm legacy integer rounding mode to convert
     * @return <tt>RoundingMode</tt> corresponding to the given integer.
     * @throws IllegalArgumentException integer is out of range
     */
    public static RoundingMode valueOf(int rm) {
        switch(rm) {

        case BigDecimal.ROUND_UP:
            return UP;

        case BigDecimal.ROUND_DOWN:
            return DOWN;

        case BigDecimal.ROUND_CEILING:
            return CEILING;
            
        case BigDecimal.ROUND_FLOOR:
            return FLOOR;

        case BigDecimal.ROUND_HALF_UP:
            return HALF_UP;
            
        case BigDecimal.ROUND_HALF_DOWN:
            return HALF_DOWN;
            
        case BigDecimal.ROUND_HALF_EVEN:
            return HALF_EVEN;
            
        case BigDecimal.ROUND_UNNECESSARY:
            return UNNECESSARY;
            
        default:
            throw new IllegalArgumentException("argument out of range");                
        }
    }
}
