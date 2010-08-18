/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.text;

import java.io.IOException;
import java.io.Writer;

import j2me.io.Serializable;
import j2me.lang.CharSequence;
import j2mex.realtime.MemoryArea;

import javolution.JavolutionError;
import javolution.context.ObjectFactory;
import javolution.io.UTF8StreamWriter;
import javolution.lang.MathLib;
import javolution.lang.Realtime;
import javolution.lang.Reusable;

/**
 * <p> This class represents an {@link Appendable} text whose capacity expands 
 *     gently without incurring expensive resize/copy operations ever.</p>
 *     
 * <p> This class is not intended for large documents manipulations which 
 *     should be performed with the {@link Text} class directly 
 *     (<code>O(Log(n))</code> {@link Text#insert insertion} and 
 *     {@link Text#delete deletion} capabilities).</p>
 *     
 * <p> This implementation is not synchronized.</p>
 *     
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 3.7, January 1, 2006
 */
public class TextBuilder implements Appendable,
        CharSequence, Reusable, Realtime, Serializable {

    /**
     * Holds the factory for this text builder.
     */
    private static final ObjectFactory FACTORY = new ObjectFactory() {
        public Object create() {
            return new TextBuilder();
        }
    };

    //
    // Holds the character buffers. The array sizes are adjusted to ensures that
    // no more than 4 time the required space is ever allocated.
    //
    // char[1<<D3][1<<D2][1<<D1][1<<D0]
    // with charAt(i) = char[(i>>R3)&M3][(i>>R2)&M2][(i>>R1)&M1][(i>>R0)&M0]
    // 

    private static final int D0 = 5;

    private static final int M0 = (1 << D0) - 1;

    private static final int C0 = 1 << D0; // capacity chars0

    private static final int D1 = D0 + 2;

    private static final int R1 = D0;

    private static final int M1 = (1 << D1) - 1;

    private static final int C1 = 1 << (D0 + D1); // capacity chars1

    private static final int D2 = D1 + 2;

    private static final int R2 = D0 + D1;

    private static final int M2 = (1 << D2) - 1;

    private static final int C2 = 1 << (D0 + D1 + D2); // capacity chars2

    private static final int D3 = D2 + 2;

    private static final int R3 = D0 + D1 + D2;

    private final char[] _chars0 = new char[1 << D0]; // 5 bits (32).

    private char[][] _chars1; // new char[1<<7][1<<5]; // 12 bits (4096)

    private char[][][] _chars2; // new char[1<<9][1<<7][1<<5]; // 21 bits (2097152)

    private char[][][][] _chars3; // new char[1<<11][1<<9][1<<7][1<<5]; 

    /**
     * Holds the current capacity. 
     */
    private int _capacity = C0;

    /**
     * Holds the current length.
     */
    private int _length;

    /**
     * Creates a text builder of small initial capacity.
     */
    public TextBuilder() {
    }

    /**
     * Creates a text builder holding the specified character sequence.
     * 
     * @param csq the initial character sequence of this text builder.
     */
    public TextBuilder(CharSequence csq) {
        append(csq);
    }

    /**
     * Creates a text builder holding the specified <code>String</code>.
     * 
     * @param str the initial string content of this text builder.
     */
    public TextBuilder(String str) {
        append(str);
    }

    /**
     * Creates a text builder of specified initial capacity.
     * Unless the text length exceeds the specified capacity, operations 
     * on this text builder will not allocate memory.
     * 
     * @param capacity the initial capacity.
     */
    public TextBuilder(int capacity) {
        while (capacity > _capacity) {
            increaseCapacity();
        }
    }

    /**
     * Returns a new, preallocated or {@link #recycle recycled} text builder
     * (on the stack when executing in a {@link javolution.context.StackContext
     * StackContext}).
     *
     * @return a new, preallocated or recycled text builder instance.
     */
    public static TextBuilder newInstance() {
        TextBuilder textBuilder = (TextBuilder) FACTORY.object();
        textBuilder._length = 0;
        return textBuilder;
    }

    /**
     * Recycles a text builder {@link #newInstance() instance} immediately
     * (on the stack when executing in a {@link javolution.context.StackContext
     * StackContext}). 
     */
    public static void recycle(TextBuilder instance) {
        FACTORY.recycle(instance);
    }

    /**
     * Returns the length (character count) of this text builder.
     *
     * @return the number of characters (16-bits Unicode).
     */
    public final int length() {
        return _length;
    }

    /**
     * Returns the character at the specified index.
     *
     * @param  i the index of the character.
     * @return the character at the specified index.
     * @throws IndexOutOfBoundsException if <code>(i < 0) || (i >= this.length())</code>.
     */
    public final char charAt(int i) {
        if ((i < 0) || (i >= _length))
            throw new IndexOutOfBoundsException();
        return charsAt(i)[i & M0];
    }
    
    // Returns character block.
    final char[] charsAt(int i) { 
        return (i < C0) ? _chars0 : (i < C1) ? _chars1[(i >> R1)]
                : (i < C2) ? _chars2[(i >> R2)][(i >> R1) & M1]
                        : _chars3[(i >> R3)][(i >> R2) & M2][(i >> R1) & M1];
    }

    /**
     * Copies the character from this text builder into the destination
     * character array. 
     *
     * @param srcBegin this text start index.
     * @param srcEnd this text end index (not included).
     * @param dst the destination array to copy the data into.
     * @param dstBegin the offset into the destination array. 
     * @throws IndexOutOfBoundsException if <code>(srcBegin < 0) ||
     *  (dstBegin < 0) || (srcBegin > srcEnd) || (srcEnd > this.length())
     *  || ((dstBegin + srcEnd - srcBegin) >  dst.length)</code>
     */
    public final void getChars(int srcBegin, int srcEnd, char[] dst,
            int dstBegin) {
        if ((srcBegin < 0) || (srcBegin > srcEnd) || (srcEnd > this._length))
            throw new IndexOutOfBoundsException();
        for (int i = srcBegin, j = dstBegin; i < srcEnd;) {
            char[] chars0 = charsAt(i); // Gets character block.
            int i0 = i & M0;
            int length = MathLib.min(C0 - i0, srcEnd - i);
            System.arraycopy(chars0, i0, dst, j, length);
            i += length;
            j += length; 
        }
    }

    /**
     * Sets the character at the specified position.
     *
     * @param index the index of the character to modify.
     * @param c the new character. 
     * @throws IndexOutOfBoundsException if <code>(index < 0) || 
     *          (index >= this.length())</code>
     */
    public final void setCharAt(int index, char c) {
        if ((index < 0) || (index >= _length))
            throw new IndexOutOfBoundsException("index: " + index);
        charsAt(index)[index & M0] = c;
    }

    /**
     * Sets the length of this character builder.
     * If the length is greater than the current length; the 
     * null character <code>'&#92;u0000'</code> is inserted. 
     *
     * @param newLength the new length of this builder.
     * @throws IndexOutOfBoundsException if <code>(newLength < 0)</code>
     */
    public final void setLength(int newLength) {
        if (newLength < 0)
            throw new IndexOutOfBoundsException();
        if (newLength <= _length) {
            _length = newLength;
        } else {
            for (int i = _length; i++ < newLength;) {
                append('\u0000');
            }
        }
    }

    /**
     * Returns an instance of {@link Text} (immutable) corresponding 
     * to the character sequence between the specified indexes.
     *
     * @param  start the index of the first character inclusive.
     * @param  end the index of the last character exclusive.
     * @return an immutable character sequence.
     * @throws IndexOutOfBoundsException if <code>(start < 0) || (end < 0) ||
     *         (start > end) || (end > this.length())</code>
     */
    public final CharSequence subSequence(int start, int end) {
        if ((start < 0) || (end < 0) || (start > end) || (end > _length))
            throw new IndexOutOfBoundsException();
        return Text.valueOf(this, start, end);
    }

    /**
     * Appends the specified character.
     *
     * @param  c the character to append.
     * @return <code>this</code>
     */
    public final Appendable/*TextBuilder*/append(char c) { // Short to be inlined.
        if (_length >= C0)
            return append2(c);
        _chars0[_length++] = c;
        return this;
    }

    private TextBuilder append2(char c) {
        if (_length >= _capacity)
            increaseCapacity();
        final int i = _length++;
        if (i < C1) {
            _chars1[(i >> R1)][i & M0] = c;
        } else if (i < C2) {
            _chars2[(i >> R2)][(i >> R1) & M1][i & M0] = c;
        } else {
            _chars3[(i >> R3)][(i >> R2) & M2][(i >> R1) & M1][i & M0] = c;
        }
        return this;
    }

    /**
     * Appends the textual representation of the specified object. 
     * If the specified object is <code>null</code> this method 
     * is equivalent to <code>append("null")</code>. 
     *
     * @param obj the object to represent or <code>null</code>.
     * @return <code>this</code>
     */
    public final TextBuilder append(Object obj) {
        if (obj instanceof String) 
            return append((String) obj);
        return append(Text.valueOf(obj));
    }
        
    /**
     * Appends the specified character sequence. If the specified character
     * sequence is <code>null</code> this method is equivalent to
     * <code>append("null")</code>.
     *
     * @param  csq the character sequence to append or <code>null</code>.
     * @return <code>this</code>
     */
    public final Appendable/*TextBuilder*/append(CharSequence csq) {
        return (csq == null) ? append("null") : append(csq, 0, csq.length());
    }

    /**
     * Appends a subsequence of the specified character sequence.
     * If the specified character sequence is <code>null</code> this method 
     * is equivalent to <code>append("null")</code>. 
     *
     * @param  csq the character sequence to append or <code>null</code>.
     * @param  start the index of the first character to append.
     * @param  end the index after the last character to append.
     * @return <code>this</code>
     * @throws IndexOutOfBoundsException if <code>(start < 0) || (end < 0) 
     *         || (start > end) || (end > csq.length())</code>
     */
    public final Appendable/*TextBuilder*/append(CharSequence csq, int start,
            int end) {
        if (csq == null)
            return append("null");
        if ((start < 0) || (end < 0) || (start > end) || (end > csq.length()))
            throw new IndexOutOfBoundsException();
        for (int i = start; i < end;) {
            append(csq.charAt(i++));
        }
        return this;
    }


    /**
     * Appends the specified string to this text builder. 
     * If the specified string is <code>null</code> this method 
     * is equivalent to <code>append("null")</code>. 
     *
     * @param str the string to append or <code>null</code>.
     * @return <code>this</code>
     */
    public final TextBuilder append(String str) {
        return (str == null) ? append("null") : 
            appendNoCheck(str, 0, str.length());
    }

    /**
     * Appends a subsequence of the specified string.
     * If the specified character sequence is <code>null</code> this method 
     * is equivalent to <code>append("null")</code>. 
     *
     * @param  str the string to append or <code>null</code>.
     * @param  start the index of the first character to append.
     * @param  end the index after the last character to append.
     * @return <code>this</code>
     * @throws IndexOutOfBoundsException if <code>(start < 0) || (end < 0) 
     *         || (start > end) || (end > str.length())</code>
     */
    public final TextBuilder append(String str, int start, int end) {
        if (str == null)
            return append("null");
        if ((start < 0) || (end < 0) || (start > end) || (end > str.length()))
            throw new IndexOutOfBoundsException();
        return appendNoCheck(str, start, end);
    } 
    
    private final TextBuilder appendNoCheck(String str, int start, int end) {
        int newLength = _length + end - start; 
        while (_capacity < newLength) {
            increaseCapacity();
        }
        for (int i = start, j = _length; i < end;) {
            char[] chars = charsAt(j);
            int inc = MathLib.min(C0 - (j & M0), end - i);
            str.getChars(i, (i += inc), chars, j & M0);
            j += inc;
        }
        _length = newLength;
        return this;
    }

    /**
     * Appends the specified text to this text builder. 
     * If the specified text is <code>null</code> this method 
     * is equivalent to <code>append("null")</code>. 
     *
     * @param txt the text to append or <code>null</code>.
     * @return <code>this</code>
     */
    public final TextBuilder append(Text txt) {
        return (txt == null) ? append("null") : 
            appendNoCheck(txt, 0, txt.length());
    }

    /**
     * Appends a subsequence of the specified text.
     * If the specified character sequence is <code>null</code> this method 
     * is equivalent to <code>append("null")</code>. 
     *
     * @param  txt the text to append or <code>null</code>.
     * @param  start the index of the first character to append.
     * @param  end the index after the last character to append.
     * @return <code>this</code>
     * @throws IndexOutOfBoundsException if <code>(start < 0) || (end < 0) 
     *         || (start > end) || (end > txt.length())</code>
     */
    public final TextBuilder append(Text txt, int start, int end) {
        if (txt == null)
            return append("null");
        if ((start < 0) || (end < 0) || (start > end) || (end > txt.length()))
            throw new IndexOutOfBoundsException();
        return appendNoCheck(txt, start, end);
    }

    private final TextBuilder appendNoCheck(Text txt, int start, int end) {
        int newLength = _length + end - start; 
        while (_capacity < newLength) {
            increaseCapacity();
        }
        for (int i = start, j = _length; i < end;) {
            char[] chars = charsAt(j);
            int inc = MathLib.min(C0 - (j & M0), end - i);
            txt.getChars(i, (i += inc), chars, j & M0);
            j += inc;
        }
        _length = newLength;
        return this;
    }

    /**
     * Appends the characters from the char array argument.
     *
     * @param  chars the character array source.
     * @return <code>this</code>
     */
    public final TextBuilder append(char chars[]) {
        return append(chars, 0, chars.length);
    }

    /**
     * Appends the characters from a subarray of the char array argument.
     *
     * @param  chars the character array source.
     * @param  offset the index of the first character to append.
     * @param  length the number of character to append.
     * @return <code>this</code>
     * @throws IndexOutOfBoundsException if <code>(offset < 0) || 
     *         (length < 0) || ((offset + length) > chars.length)</code>
     */
    public final TextBuilder append(char chars[], int offset, int length) {
        if ((offset < 0) || (length < 0) || ((offset + length) > chars.length))
            throw new IndexOutOfBoundsException();
        final int end = offset + length;
        for (int i = offset; i < end;) {
            append(chars[i++]);
        }
        return this;
    }

    /**
     * Appends the textual representation of the specified <code>boolean</code>
     * argument.
     *
     * @param  b the <code>boolean</code> to format.
     * @return <code>this</code>
     * @see    TypeFormat
     */
    public final TextBuilder append(boolean b) {
        return b ? append("true") : append("false");
    }

    /**
     * Appends the decimal representation of the specified <code>int</code>
     * argument.
     *
     * @param  i the <code>int</code> to format.
     * @return <code>this</code>
     */
    public final TextBuilder append(int i) {
        if (i < 0) {
            if (i == Integer.MIN_VALUE) // Negation would overflow.
                return append("-2147483648"); // 11 char max.
            i = -i;
            append('-');
        }
        if (i >= 100000) {
            int high = i / 100000;
            writeDigits(high, false);
            i -= high * 100000;
            writeDigits(i, true);
        } else {
            writeDigits(i, false);
        }
        return this;
    }

    /**
     * Appends the radix representation of the specified <code>int</code>
     * argument.
     *
     * @param  i the <code>int</code> to format.
     * @param  radix the radix (e.g. <code>16</code> for hexadecimal).
     * @return <code>this</code>
     */
    public final TextBuilder append(int i, int radix) {
        if (radix == 10)
            return append(i); // Faster version.
        if (radix < 2 || radix > 36)
            throw new IllegalArgumentException("radix: " + radix);
        if (i < 0) {
            append('-');
        } else {
            i = -i;
        }
        append2(i, radix);
        return this;
    }

    private void append2(int i, int radix) {
        if (i <= -radix) {
            append2(i / radix, radix);
            append(DIGIT_TO_CHAR[-(i % radix)]);
        } else {
            append(DIGIT_TO_CHAR[-i]);
        }
    }

    /**
     * Appends the decimal representation of the specified <code>long</code>
     * argument.
     *
     * @param  l the <code>long</code> to format.
     * @return <code>this</code>
     */
    public final TextBuilder append(long l) {
        if (l < 0) {
            if (l == Long.MIN_VALUE) // Negation would overflow.
                return append("-9223372036854775808"); // 20 characters max.
            l = -l;
            append('-');
        }
        boolean writeZero = false;
        if (l >= 1000000000000000L) {
            int high = (int) (l / 1000000000000000L);
            writeDigits(high, writeZero);
            l -= high * 1000000000000000L;
            writeZero = true;
        }
        if (writeZero || (l >= 10000000000L)) {
            int high = (int) (l / 10000000000L);
            writeDigits(high, writeZero);
            l -= high * 10000000000L;
            writeZero = true;
        }
        if (writeZero || (l >= 100000)) {
            int high = (int) (l / 100000);
            writeDigits(high, writeZero);
            l -= high * 100000L;
            writeZero = true;
        }
        writeDigits((int) l, writeZero);
        return this;
    }

    /**
     * Appends the radix representation of the specified <code>long</code>
     * argument.
     *
     * @param  l the <code>long</code> to format.
     * @param  radix the radix (e.g. <code>16</code> for hexadecimal).
     * @return <code>this</code>
     */
    public final TextBuilder append(long l, int radix) {
        if (radix == 10)
            return append(l); // Faster version.
        if (radix < 2 || radix > 36)
            throw new IllegalArgumentException("radix: " + radix);
        if (l < 0) {
            append('-');
        } else {
            l = -l;
        }
        append2(l, radix);
        return this;
    }

    private void append2(long l, int radix) {
        if (l <= -radix) {
            append2(l / radix, radix);
            append(DIGIT_TO_CHAR[(int) -(l % radix)]);
        } else {
            append(DIGIT_TO_CHAR[(int) -l]);
        }
    }

    /**
     * Appends the textual representation of the specified <code>float</code>
     * (equivalent to 
     * <code>append(f, 10, (abs(f) &gt;= 1E7) || (abs(f) &lt; 0.001), false)</code>).
     *
     * @param  f the <code>float</code> to format.
     * @return <code>this</code>
     * @JVM-1.1+@
     public final TextBuilder append(float f) {
     return append(f, 10, (MathLib.abs(f) >= 1E7) || (MathLib.abs(f) < 0.001), false);
     }
     /**/

    /**
     * Appends the textual representation of the specified <code>double</code>;
     * the number of digits is 17 or 16 when the 16 digits representation 
     * can be parsed back to the same <code>double</code> (mimic the standard
     * library formatting).
     * 
     * @param  d the <code>double</code> to format.
     * @return <code>append(d, -1, (MathLib.abs(d) >= 1E7) ||
     *        (MathLib.abs(d) < 0.001), false)</code>
     * @JVM-1.1+@
     public final TextBuilder append(double d) {
     return append(d, -1, (MathLib.abs(d) >= 1E7) || (MathLib.abs(d) < 0.001), false);
     }
     /**/

    /**
     * Appends the textual representation of the specified <code>double</code>
     * according to the specified formatting arguments.
     *
     * @param  d the <code>double</code> value.
     * @param  digits the number of significative digits (excludes exponent) or
     *         <code>-1</code> to mimic the standard library (16 or 17 digits).
     * @param  scientific <code>true</code> to forces the use of the scientific 
     *         notation (e.g. <code>1.23E3</code>); <code>false</code> 
     *         otherwise. 
     * @param  showZero <code>true</code> if trailing fractional zeros are 
     *         represented; <code>false</code> otherwise.
     * @return <code>this</code>
     * @throws IllegalArgumentException if <code>(digits &gt; 19)</code>)
     * @JVM-1.1+@
     public final TextBuilder append(double d, int digits,
     boolean scientific, boolean showZero)  {
     if (digits > 19)
     throw new IllegalArgumentException("digits: " + digits);
     if (d != d)  // NaN
     return append("NaN");
     if (d < 0) { // Work with positive number.
     d = -d;
     append('-');
     }
     if (d == Double.POSITIVE_INFINITY) // Infinity.
     return append("Infinity");
     if (d == 0.0) { // Zero.
     if (digits == 1)
     return append("0.");
     if (!showZero)
     return append("0.0");
     append("0.0");
     for (int i = 2; i < digits; i++) {
     append('0');
     }
     return this;
     }
     
     // Find the exponent e such as: value == x.xxx * 10^e
     int e = MathLib.floorLog10(d);
     
     long m;
     if (digits < 0) { // Use 16 or 17 digits.
     // Try 17 digits.
     long m17 = MathLib.toLongPow10(d, (17 - 1) - e);
     // Check if we can use 16 digits.
     long m16 = m17 / 10;
     double dd = MathLib.toDoublePow10(m16, e - 16 + 1);
     if (dd == d) { // 16 digits is enough.
     digits = 16;
     m = m16;
     } else { // We cannot remove the last digit.
     digits = 17;
     m = m17;
     }
     } else { // Use the specified number of digits.
     m = MathLib.toLongPow10(d, (digits - 1) - e);
     }
     
     // Formats.
     if (scientific || (e >= digits)) {
     // Scientific notation has to be used ("x.xxxEyy").
     long pow10 = POW10_LONG[digits - 1];
     int i = (int) (m / pow10); // Single digit.
     append(DIGIT_TO_CHAR[i]);
     m = m - pow10 * i;
     appendFraction(m, digits - 1, showZero);
     append('E');
     append(e);
     } else { // Dot within the string ("xxxx.xxxxx").
     if (e < 0) {
     append('0');
     } else {
     long pow10 = POW10_LONG[digits - e - 1];
     long l = m / pow10;
     append(l);
     m = m - pow10 * l;
     }
     appendFraction(m, digits - e - 1, showZero);
     }
     return this;
     }
     private static final long[] POW10_LONG = new long[] { 1L, 10L, 100L, 1000L,
     10000L, 100000L, 1000000L, 10000000L, 100000000L, 1000000000L,
     10000000000L, 100000000000L, 1000000000000L, 10000000000000L,
     100000000000000L, 1000000000000000L, 10000000000000000L,
     100000000000000000L, 1000000000000000000L };
     /**/

    final void appendFraction(long l, int digits, boolean showZero) {
        append('.');
        int length = MathLib.digitLength(l);
        if ((length == 0) && !showZero) { // Only one zero shown xxx.0
            append('0');
            return;
        }
        for (int i = length; i < digits; i++) {
            append('0');
        }
        if (l != 0) {
            append(l);
        }
        if (!showZero) { // Remove trailing zeros.
            int trailingZeros = 0;
            while (true) {
                char c = charAt(_length - trailingZeros - 1);
                if (c != '0')
                    break;
                trailingZeros++;
            }
            this.setLength(_length - trailingZeros);
        }
    }

    /**
     * Inserts the specified character sequence at the specified location.
     *
     * @param index the insertion position.
     * @param csq the character sequence being inserted.
     * @return <code>this</code>
     * @throws IndexOutOfBoundsException if <code>(index < 0) || 
     *         (index > this.length())</code>
     */
    public final TextBuilder insert(int index, CharSequence csq) {
        if ((index < 0) || (index > _length))
            throw new IndexOutOfBoundsException("index: " + index);
        final int shift = csq.length();
        _length += shift;
        while (_length >= _capacity) {
            increaseCapacity();
        }
        for (int i = _length - shift; --i >= index;) {
            this.setCharAt(i + shift, this.charAt(i));
        }
        for (int i = csq.length(); --i >= 0;) {
            this.setCharAt(index + i, csq.charAt(i));
        }
        return this;
    }

    /**
     * Removes all the characters of this text builder 
     * (equivalent to <code>this.delete(start, this.length())</code>).
     * 
     * @return <code>this.delete(0, this.length())</code>
     */
    public final TextBuilder clear() {
        _length = 0;
        return this;
    }

    /**
     * Removes the characters between the specified indices.
     * 
     * @param start the beginning index, inclusive.
     * @param end the ending index, exclusive.
     * @return <code>this</code>
     * @throws IndexOutOfBoundsException if <code>(start < 0) || (end < 0) 
     *         || (start > end) || (end > this.length())</code>
     */
    public final TextBuilder delete(int start, int end) {
        if ((start < 0) || (end < 0) || (start > end) || (end > this.length()))
            throw new IndexOutOfBoundsException();
        for (int i = end, j = start; i < _length;) {
            this.setCharAt(j++, this.charAt(i++));
        }
        _length -= end - start;
        return this;
    }

    /**
     * Reverses this character sequence.
     *
     * @return <code>this</code>
     */
    public final TextBuilder reverse() {
        final int n = _length - 1;
        for (int j = (n - 1) >> 1; j >= 0;) {
            char c = charAt(j);
            setCharAt(j, charAt(n - j));
            setCharAt(n - j--, c);
        }
        return this;
    }

    /**
     * Returns the {@link Text} corresponding to this {@link TextBuilder}
     * (allocated on the "stack" when executing in a 
     * {@link javolution.context.StackContext StackContext}).
     *
     * @return the corresponding {@link Text} instance.
     */
    public final Text toText() {
        // TODO Avoids copying the arrays and makes it immutable ??
        return Text.valueOf(this, 0, _length);
    }
    
    /**
     * Returns the <code>String</code> representation of this 
     * {@link TextBuilder}.
     *
     * @return the <code>java.lang.String</code> for this text builder.
     */
    public final String toString() {
        char[] data = new char[_length];
        this.getChars(0, _length, data, 0);
        return new String(data, 0, _length);
    }

    /**
     * Resets this text builder for reuse (equivalent to {@link #clear}).
     */
    public final void reset() {
        _length = 0;
    }

    /**
     * Returns the hash code for this text builder.
     *
     * @return the hash code value.
     */
    public final int hashCode() {
        int h = 0;
        for (int i = 0; i < _length;) {
            h = 31 * h + charAt(i++);
        }
        return h;
    }

    /**
     * Compares this text builder against the specified object for equality.
     * Returns <code>true</code> if the specified object is a text builder 
     * having the same character content.
     * 
     * @param  obj the object to compare with or <code>null</code>.
     * @return <code>true</code> if that is a text builder with the same 
     *         character content as this text; <code>false</code> otherwise.
     */
    public final boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (!(obj instanceof TextBuilder))
            return false;
        TextBuilder that = (TextBuilder) obj;
        if (this._length != that._length)
            return false;
        for (int i = 0; i < _length;) {
            if (this.charAt(i) != that.charAt(i++))
                return false;
        }
        return true;
    }

    /**
     * Prints out this text builder to <code>System.out</code> (UTF-8 encoding).
     * This method is equivalent to:[code]
     *     synchronized(OUT) {
     *         print(OUT);
     *         OUT.flush();
     *     }
     *     ...
     *     static final OUT = new UTF8StreamWriter().setOutput(System.out);
     * [/code]
     */
    public void print() {
        try {
            synchronized (SYSTEM_OUT_WRITER) {
                print(SYSTEM_OUT_WRITER);
                SYSTEM_OUT_WRITER.flush();
            }
        } catch (IOException e) { // Should never happen.
            throw new JavolutionError(e);
        }
    }

    private static final UTF8StreamWriter SYSTEM_OUT_WRITER = new UTF8StreamWriter()
            .setOutput(System.out);

    /**
     * Prints out this text builder to <code>System.out</code> and then 
     * terminates the line. This method is equivalent to:[code]
     *     synchronized(OUT) {
     *         println(OUT);
     *         OUT.flush();
     *     }
     *     ...
     *     static final OUT = new UTF8StreamWriter().setOutput(System.out);
     * [/code]
     */
    public void println() {
        try {
            synchronized (SYSTEM_OUT_WRITER) {
                println(SYSTEM_OUT_WRITER);
                SYSTEM_OUT_WRITER.flush();
            }
        } catch (IOException e) { // Should never happen.
            throw new JavolutionError(e);
        }
    }

    /**
     * Prints out this text builder to the specified writer.
     * 
     * @param writer the destination writer.
     */
    public void print(Writer writer) throws IOException {
        for (int i=0; i < _length; i += C0) {
                char[] chars = charsAt(i);
                writer.write(chars, 0, MathLib.min(C0, _length - i));
        }
    }

    /**
     * Prints out this text builder to the specified writer and then terminates 
     * the line.
     * 
     * @param writer the destination writer.
     */
    public void println(Writer writer) throws IOException {
        print(writer);
        writer.write('\n');
    }

    /**
     * Appends the content of this text builder to the specified 
     * string buffer (only for text builder with length less than 32).
     *
     * @param sb the string buffer.
     */
    final void appendTo(StringBuffer sb) {
        sb.append(_chars0, 0, _length);
    }

    /**
     * Appends the content of this text builder to the specified 
     * string buffer (only for text builder with length less than 32).
     *
     * @param sb the string builder.
     * @JVM-1.5+@
     final void appendTo(StringBuilder sb) {
     sb.append(_chars0, 0, _length);
     }
     /**/

    /**
     * Indicates if this text builder has the same character content as the 
     * specified character sequence.
     *
     * @param csq the character sequence to compare with.
     * @return <code>true</code> if the specified character sequence has the 
     *        same character content as this text; <code>false</code> otherwise.
     */
    public final boolean contentEquals(CharSequence csq) {
        if (csq.length() != _length)
            return false;
        char[] chars = charsAt(0); // Gets character block.
        for (int i = 0; i < _length;) {
            if (chars[i & M0] != csq.charAt(i++))
                return false;
            if ((i & M0) == 0) { // Changes character block.
                chars = charsAt(i);
            }
        }
        return true;
    }

    /**
     * Equivalent to {@link #contentEquals(CharSequence)} 
     * (for J2ME compability).
     *
     * @param csq the string character sequence to compare with.
     * @return <code>true</code> if the specified string has the 
     *        same character content as this text; <code>false</code> otherwise.
     */
    public final boolean contentEquals(String csq) {
        if (csq.length() != _length)
            return false;
        char[] chars = charsAt(0); // Gets character block.
        for (int i = 0; i < _length;) {
            if (chars[i & M0]  != csq.charAt(i++))
                return false;
            if ((i & M0) == 0) { // Changes character block.
                chars = charsAt(i);
            }
        }
        return true;
    }

    /**
     * Increases this text builder capacity.
     */
    private void increaseCapacity() {
        MemoryArea.getMemoryArea(this).executeInArea(new Runnable() {
            public void run() {
                final int c = _capacity;
                _capacity += 1 << D0;
                if (c < C1) {
                    if (_chars1 == null) {
                        _chars1 = new char[1 << D1][];
                    }
                    _chars1[(c >> R1)] = new char[1 << D0];

                } else if (c < C2) {
                    if (_chars2 == null) {
                        _chars2 = new char[1 << D2][][];
                    }
                    if (_chars2[(c >> R2)] == null) {
                        _chars2[(c >> R2)] = new char[1 << D1][];
                    }
                    _chars2[(c >> R2)][(c >> R1) & M1] = new char[1 << D0];

                } else {
                    if (_chars3 == null) {
                        _chars3 = new char[1 << D3][][][];
                    }
                    if (_chars3[(c >> R3)] == null) {
                        _chars3[(c >> R3)] = new char[1 << D2][][];
                    }
                    if (_chars3[(c >> R3)][(c >> R2) & M2] == null) {
                        _chars3[(c >> R3)][(c >> R2) & M2] = new char[1 << D1][];
                    }
                    _chars3[(c >> R3)][(c >> R2) & M2][(c >> R1) & M1] = new char[1 << D0];
                }
            }
        });
    }

    /**
     * Appends the specified positive integer [0 .. 99999] in decimal
     * representation to this text builder.
     *
     * @param i the integer to write.
     * @param writeZero <code>true</code> if leading zero are included;
     *       <code>false</code> otherwise.
     */
    private void writeDigits(int i, boolean writeZero) {
        {
            final int e = 10000;
            if (i >= e) {
                writeZero = true;
                if (i >= e * 5) {
                    if (i >= e * 8) {
                        if (i >= e * 9) {
                            append('9');
                            i -= e * 9;
                        } else {
                            append('8');
                            i -= e * 8;
                        }
                    } else { // [5 .. 8[
                        if (i >= e * 7) {
                            append('7');
                            i -= e * 7;
                        } else if (i >= e * 6) {
                            append('6');
                            i -= e * 6;
                        } else {
                            append('5');
                            i -= e * 5;
                        }
                    }
                } else { // [1 ..5[
                    if (i >= e * 3) {
                        if (i >= e * 4) {
                            append('4');
                            i -= e * 4;
                        } else {
                            append('3');
                            i -= e * 3;
                        }
                    } else { // [1 .. 2[
                        if (i >= e * 2) {
                            append('2');
                            i -= e * 2;
                        } else {
                            append('1');
                            i -= e;
                        }
                    }
                }
            } else if (writeZero) {
                append('0');
            }
        }
        {
            final int e = 1000;
            if (i >= e) {
                writeZero = true;
                if (i >= e * 5) {
                    if (i >= e * 8) {
                        if (i >= e * 9) {
                            append('9');
                            i -= e * 9;
                        } else {
                            append('8');
                            i -= e * 8;
                        }
                    } else { // [5 .. 8[
                        if (i >= e * 7) {
                            append('7');
                            i -= e * 7;
                        } else if (i >= e * 6) {
                            append('6');
                            i -= e * 6;
                        } else {
                            append('5');
                            i -= e * 5;
                        }
                    }
                } else { // [1 ..5[
                    if (i >= e * 3) {
                        if (i >= e * 4) {
                            append('4');
                            i -= e * 4;
                        } else {
                            append('3');
                            i -= e * 3;
                        }
                    } else { // [1 .. 2[
                        if (i >= e * 2) {
                            append('2');
                            i -= e * 2;
                        } else {
                            append('1');
                            i -= e;
                        }
                    }
                }
            } else if (writeZero) {
                append('0');
            }
        }
        {
            final int e = 100;
            if (i >= e) {
                writeZero = true;
                if (i >= e * 5) {
                    if (i >= e * 8) {
                        if (i >= e * 9) {
                            append('9');
                            i -= e * 9;
                        } else {
                            append('8');
                            i -= e * 8;
                        }
                    } else { // [5 .. 8[
                        if (i >= e * 7) {
                            append('7');
                            i -= e * 7;
                        } else if (i >= e * 6) {
                            append('6');
                            i -= e * 6;
                        } else {
                            append('5');
                            i -= e * 5;
                        }
                    }
                } else { // [1 ..5[
                    if (i >= e * 3) {
                        if (i >= e * 4) {
                            append('4');
                            i -= e * 4;
                        } else {
                            append('3');
                            i -= e * 3;
                        }
                    } else { // [1 .. 2[
                        if (i >= e * 2) {
                            append('2');
                            i -= e * 2;
                        } else {
                            append('1');
                            i -= e;
                        }
                    }
                }
            } else if (writeZero) {
                append('0');
            }
        }
        {
            final int e = 10;
            if (i >= e) {
                writeZero = true;
                if (i >= e * 5) {
                    if (i >= e * 8) {
                        if (i >= e * 9) {
                            append('9');
                            i -= e * 9;
                        } else {
                            append('8');
                            i -= e * 8;
                        }
                    } else { // [5 .. 8[
                        if (i >= e * 7) {
                            append('7');
                            i -= e * 7;
                        } else if (i >= e * 6) {
                            append('6');
                            i -= e * 6;
                        } else {
                            append('5');
                            i -= e * 5;
                        }
                    }
                } else { // [1 ..5[
                    if (i >= e * 3) {
                        if (i >= e * 4) {
                            append('4');
                            i -= e * 4;
                        } else {
                            append('3');
                            i -= e * 3;
                        }
                    } else { // [1 .. 2[
                        if (i >= e * 2) {
                            append('2');
                            i -= e * 2;
                        } else {
                            append('1');
                            i -= e;
                        }
                    }
                }
            } else if (writeZero) {
                append('0');
            }
        }
        append(DIGIT_TO_CHAR[i]);
    }

    /**
     * Holds the digits to character mapping. 
     */
    private final static char[] DIGIT_TO_CHAR = { '0', '1', '2', '3', '4', '5',
            '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
            'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z' };

}
