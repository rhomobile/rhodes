/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.text;

import j2me.lang.CharSequence;
import javolution.text.TextFormat.Cursor;

import java.io.IOException;

/**
 * <p> This class provides utility methods to parse <code>CharSequence</code>
 *     into primitive types and to format primitive types into any 
 *     <code>Appendable</code>.</p>
 *
 * <p> Methods from this class <b>do not create temporary objects</b> and
 *     are typically faster than standard library methods (see 
 *     <a href="http://javolution.org/doc/benchmark.html">benchmark</a>).</p>
 *     
 * <p> The number of digits when formatting floating point numbers can be 
 *     specified. The default setting for <code>double</code> is 17 digits 
 *     or even 16 digits when the conversion is lossless back and forth
 *     (mimic the standard library formatting). For example:[code]
 *         TypeFormat.format(0.2, a) = "0.2" // 17 or 16 digits (as long as lossless conversion), remove trailing zeros.
 *         TypeFormat.format(0.2, 17, false, false, a) = "0.20000000000000001" // Closest 17 digits number.
 *         TypeFormat.format(0.2, 19, false, false, a) = "0.2000000000000000111" // Closest 19 digits.
 *         TypeFormat.format(0.2, 4, false, false, a) = "0.2" // Fixed-point notation, remove trailing zeros.
 *         TypeFormat.format(0.2, 4, false, true, a) = "0.2000" // Fixed-point notation, fixed number of digits.
 *         TypeFormat.format(0.2, 4, true, false, a) = "2.0E-1" // Scientific notation, remove trailing zeros.  
 *         TypeFormat.format(0.2, 4, true, true, a) = "2.000E-1" // Scientific notation, fixed number of digits.
 *         [/code]</p>        
 *
 * <p> For non-primitive objects, formatting is typically performed using 
 *     specialized {@link TextFormat} instances.</p>
 * 
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 4.1, November 30, 2006
 */
public final class TypeFormat {

    /**
     * Default constructor (forbids derivation).
     */
    private TypeFormat() {
    }

    /**
     * Parses the specified character sequence as a <code>boolean</code>.
     *
     * @param  csq the character sequence to parse.
     * @return the corresponding boolean value.
     * @throws IllegalArgumentException if the specified character sequence 
     *         is different from "true" or "false" ignoring cases.
     */
    public static boolean parseBoolean(CharSequence csq) {
        if ((csq.length() == 4)
                && (csq.charAt(0) == 't' || csq.charAt(0) == 'T')
                && (csq.charAt(1) == 'r' || csq.charAt(1) == 'R')
                && (csq.charAt(2) == 'u' || csq.charAt(2) == 'U')
                && (csq.charAt(3) == 'e' || csq.charAt(3) == 'E')) {
            return true;
        } else if ((csq.length() == 5)
                && (csq.charAt(0) == 'f' || csq.charAt(0) == 'F')
                && (csq.charAt(1) == 'a' || csq.charAt(1) == 'A')
                && (csq.charAt(2) == 'l' || csq.charAt(2) == 'L')
                && (csq.charAt(3) == 's' || csq.charAt(3) == 'S')
                && (csq.charAt(4) == 'e' || csq.charAt(4) == 'E')) {
            return false;
        }
        throw new IllegalArgumentException("Cannot parse " + csq
                + " as boolean");
    }

    /**
     * Equivalent to {@link #parseBoolean(CharSequence)} 
     * (for J2ME compatibility).
     */
    public static boolean parseBoolean(String csq) {
        if ((csq.length() == 4)
                && (csq.charAt(0) == 't' || csq.charAt(0) == 'T')
                && (csq.charAt(1) == 'r' || csq.charAt(1) == 'R')
                && (csq.charAt(2) == 'u' || csq.charAt(2) == 'U')
                && (csq.charAt(3) == 'e' || csq.charAt(3) == 'E')) {
            return true;
        } else if ((csq.length() == 5)
                && (csq.charAt(0) == 'f' || csq.charAt(0) == 'F')
                && (csq.charAt(1) == 'a' || csq.charAt(1) == 'A')
                && (csq.charAt(2) == 'l' || csq.charAt(2) == 'L')
                && (csq.charAt(3) == 's' || csq.charAt(3) == 'S')
                && (csq.charAt(4) == 'e' || csq.charAt(4) == 'E')) {
            return false;
        }
        throw new IllegalArgumentException("Cannot parse " + csq
                + " as boolean");
    }

    /**
     * Parses the specified character sequence from the specified position 
     * as a <code>boolean</code>.
     *
     * @param csq the character sequence to parse.
     * @param cursor the current cursor position (being maintained).
     * @return the next boolean value.
     * @throws IllegalArgumentException if the character sequence from the 
     *         specified position is different from "true" or "false" ignoring
     *         cases.
     */
    public static boolean parseBoolean(CharSequence csq, Cursor cursor) {
        final int i = cursor.getIndex();
        if ((cursor.getEndIndex() >= i + 4)
                && (csq.charAt(i) == 't' || csq.charAt(i) == 'T')
                && (csq.charAt(i + 1) == 'r' || csq.charAt(i + 1) == 'R')
                && (csq.charAt(i + 2) == 'u' || csq.charAt(i + 2) == 'U')
                && (csq.charAt(i + 3) == 'e' || csq.charAt(i + 3) == 'E')) {
            cursor.increment(4);
            return true;
        }
        if ((cursor.getEndIndex() >= i + 5)
                && (csq.charAt(i) == 'f' || csq.charAt(i) == 'F')
                && (csq.charAt(i + 1) == 'a' || csq.charAt(i + 1) == 'A')
                && (csq.charAt(i + 2) == 'l' || csq.charAt(i + 2) == 'L')
                && (csq.charAt(i + 3) == 's' || csq.charAt(i + 3) == 'S')
                && (csq.charAt(i + 4) == 'e' || csq.charAt(i + 4) == 'E')) {
            cursor.increment(5);
            return false;
        }
        throw new IllegalArgumentException("Cannot parse boolean " + 
                csq.subSequence(cursor.getIndex(), cursor.getEndIndex()));
    }

    /**
     * Parses the specified character sequence as a signed decimal 
     * <code>byte</code>.
     *
     * @param  csq the character sequence to parse.
     * @return <code>parseByte(csq, 10)</code>
     * @throws NumberFormatException if the specified character sequence
     *         does not contain a parsable <code>byte</code>.
     * @see    #parseByte(CharSequence, int)
     */
    public static byte parseByte(CharSequence csq) {
        return parseByte(csq, 10);
    }

    /**
     * Parses the specified character sequence as a signed <code>byte</code> 
     * in the specified radix.
     *
     * @param  csq the character sequence to parse.
     * @param  radix the radix to be used while parsing.
     * @return the corresponding <code>byte</code>.
     * @throws NumberFormatException if the specified character sequence
     *         does not contain a parsable <code>byte</code>.
     */
    public static byte parseByte(CharSequence csq, int radix) {
        int i = parseInt(csq, radix);
        if ((i < Byte.MIN_VALUE) || (i > Byte.MAX_VALUE))
            throw new NumberFormatException("Overflow");
        return (byte) i;
    }

    /**
     * Parses the specified character sequence from the specified position 
     * as a signed <code>byte</code> in the specified radix.
     *
     * @param  csq the character sequence to parse.
     * @param  radix the radix to be used while parsing.
     * @param cursor the current cursor position (being maintained).
     * @return the corresponding <code>byte</code>.
     * @throws NumberFormatException if the specified character sequence
     *         does not contain a parsable <code>byte</code>.
     */
    public static byte parseByte(CharSequence csq, int radix, Cursor cursor) {
        int i = parseInt(csq, radix, cursor);
        if ((i < Byte.MIN_VALUE) || (i > Byte.MAX_VALUE))
            throw new NumberFormatException("Overflow");
        return (byte) i;
    }

    /**
     * Parses the specified character sequence as a signed decimal 
     * <code>short</code>.
     *
     * @param  csq the character sequence to parse.
     * @return <code>parseShort(csq, 10)</code>
     * @throws NumberFormatException if the specified character sequence
     *         does not contain a parsable <code>short</code>.
     * @see    #parseShort(CharSequence, int)
     */
    public static short parseShort(CharSequence csq) {
        return parseShort(csq, 10);
    }

    /**
     * Parses the specified character sequence as a signed <code>short</code> 
     * in the specified radix.
     *
     * @param  csq the character sequence to parse.
     * @param  radix the radix to be used while parsing.
     * @return the corresponding <code>short</code>.
     * @throws NumberFormatException if the specified character sequence
     *         does not contain a parsable <code>short</code>.
     */
    public static short parseShort(CharSequence csq, int radix) {
        int i = parseInt(csq, radix);
        if ((i < Short.MIN_VALUE) || (i > Short.MAX_VALUE))
            throw new NumberFormatException("Overflow");
        return (short) i;
    }

    /**
     * Parses the specified character sequence from the specified position 
     * as a signed <code>short</code> in the specified radix.
     *
     * @param  csq the character sequence to parse.
     * @param  radix the radix to be used while parsing.
     * @param cursor the current cursor position (being maintained).
     * @return the corresponding <code>short</code>.
     * @throws NumberFormatException if the specified character sequence
     *         does not contain a parsable <code>short</code>.
     */
    public static short parseShort(CharSequence csq, int radix, Cursor cursor) {
        int i = parseInt(csq, radix, cursor);
        if ((i < Short.MIN_VALUE) || (i > Short.MAX_VALUE))
            throw new NumberFormatException("Overflow");
        return (short) i;
    }

    /**
     * Parses the specified character sequence as a signed <code>int</code>.
     *
     * @param  csq the character sequence to parse.
     * @return <code>parseInt(csq, 10)</code>
     * @throws NumberFormatException if the specified character sequence
     *         does not contain a parsable <code>int</code>.
     * @see    #parseInt(CharSequence, int)
     */
    public static int parseInt(CharSequence csq) {
        return parseInt(csq, 10);
    }

    /**
     * Equivalent to {@link #parseInt(CharSequence)} (for J2ME compatibility).
     */
    public static int parseInt(String str) {
        return parseIntString(str, 10, null);
    }

    /**
     * Parses the specified character sequence as a signed <code>int</code> 
     * in the specified radix.
     *
     * @param  csq the character sequence to parse.
     * @param  radix the radix to be used while parsing.
     * @return the corresponding <code>int</code>.
     * @throws NumberFormatException if the specified character sequence
     *         does not contain a parsable <code>int</code>.
     */
    public static int parseInt(CharSequence csq, int radix) {
        return parseInt(csq, radix, null);
    }

    /**
     * Equivalent to {@link #parseInt(CharSequence, int)} 
     * (for J2ME compatibility).
     */
    public static int parseInt(String str, int radix) {
        return parseIntString(str, radix, null);
    }

    /**
     * Parses the specified character sequence from the specified position
     * as a signed <code>int</code> in the specified radix.
     *
     * @param  csq the character sequence to parse.
     * @param  radix the radix to be used while parsing.
     * @param  cursor the current cursor position (being maintained) or
     *         <code>null</code> if the whole character sequence is parsed.
     * @return the corresponding <code>int</code>.
     * @throws NumberFormatException if the specified character sequence
     *         does not contain a parsable <code>int</code>.
     */
    public static int parseInt(CharSequence csq, int radix, Cursor cursor) {
        // Avoids dynamic cost of CharSequence.charAt
        if (csq instanceof CharArray)
            return parseIntCharArray((CharArray) csq, radix, cursor);
        if (csq instanceof TextBuilder)
            return parseIntTextBuilder((TextBuilder) csq, radix, cursor);
        if (csq instanceof Text)
            return parseIntText((Text) csq, radix, cursor);
        if (((Object) csq) instanceof String)
            return parseIntString((String) ((Object) csq), radix, cursor);
        return parseIntCharSequence(csq, radix, cursor);
    }

    private static int parseIntCharArray(CharArray csq, int radix, Cursor cursor) {
        // Parsing block identical for all CharSequences.
        final int start = (cursor != null) ? cursor.getIndex() : 0;
        final int end = (cursor != null) ? cursor.getEndIndex() : csq.length();
        boolean isNegative = false;
        int result = 0; // Accumulates negatively (avoid MIN_VALUE overflow).
        int i = start;
        for (; i < end; i++) {
            char c = csq.charAt(i);
            int digit = (c <= '9') ? c - '0'
                    : ((c <= 'Z') && (c >= 'A')) ? c - 'A' + 10
                            : ((c <= 'z') && (c >= 'a')) ? c - 'a' + 10 : -1;
            if ((digit >= 0) && (digit < radix)) {
                int newResult = result * radix - digit;
                if (newResult > result)
                    throw new NumberFormatException("Overflow");
                result = newResult;
            } else if ((c == '-') && (i == start)) {
                isNegative = true;
            } else if ((c == '+') && (i == start)) {
                // Ok.
            } else {
                if (cursor == null)
                    throw new NumberFormatException("Incomplete parsing");
                break; // Done.
            }
        }
        // Requires one valid digit character and checks for opposite overflow.
        if ((result == 0) && ((end == 0) || (csq.charAt(i - 1) != '0')))
            throw new NumberFormatException("Cannot parse " + csq + " as int");
        if ((result == Integer.MIN_VALUE) && !isNegative)
            throw new NumberFormatException("Overflow");
        if (cursor != null)
            cursor.setIndex(i);
        return isNegative ? result : -result;
    }

    private static int parseIntTextBuilder(TextBuilder csq, int radix,
            Cursor cursor) {
        // Parsing block identical for all CharSequences.
        final int start = (cursor != null) ? cursor.getIndex() : 0;
        final int end = (cursor != null) ? cursor.getEndIndex() : csq.length();
        boolean isNegative = false;
        int result = 0; // Accumulates negatively (avoid MIN_VALUE overflow).
        int i = start;
        for (; i < end; i++) {
            char c = csq.charAt(i);
            int digit = (c <= '9') ? c - '0'
                    : ((c <= 'Z') && (c >= 'A')) ? c - 'A' + 10
                            : ((c <= 'z') && (c >= 'a')) ? c - 'a' + 10 : -1;
            if ((digit >= 0) && (digit < radix)) {
                int newResult = result * radix - digit;
                if (newResult > result)
                    throw new NumberFormatException("Overflow");
                result = newResult;
            } else if ((c == '-') && (i == start)) {
                isNegative = true;
            } else if ((c == '+') && (i == start)) {
                // Ok.
            } else {
                if (cursor == null)
                    throw new NumberFormatException("Incomplete parsing");
                break; // Done.
            }
        }
        // Requires one valid digit character and checks for opposite overflow.
        if ((result == 0) && ((end == 0) || (csq.charAt(i - 1) != '0')))
            throw new NumberFormatException("Cannot parse " + csq + " as int");
        if ((result == Integer.MIN_VALUE) && !isNegative)
            throw new NumberFormatException("Overflow");
        if (cursor != null)
            cursor.setIndex(i);
        return isNegative ? result : -result;
    }

    private static int parseIntText(Text csq, int radix, Cursor cursor) {
        // Parsing block identical for all CharSequences.
        final int start = (cursor != null) ? cursor.getIndex() : 0;
        final int end = (cursor != null) ? cursor.getEndIndex() : csq.length();
        boolean isNegative = false;
        int result = 0; // Accumulates negatively (avoid MIN_VALUE overflow).
        int i = start;
        for (; i < end; i++) {
            char c = csq.charAt(i);
            int digit = (c <= '9') ? c - '0'
                    : ((c <= 'Z') && (c >= 'A')) ? c - 'A' + 10
                            : ((c <= 'z') && (c >= 'a')) ? c - 'a' + 10 : -1;
            if ((digit >= 0) && (digit < radix)) {
                int newResult = result * radix - digit;
                if (newResult > result)
                    throw new NumberFormatException("Overflow");
                result = newResult;
            } else if ((c == '-') && (i == start)) {
                isNegative = true;
            } else if ((c == '+') && (i == start)) {
                // Ok.
            } else {
                if (cursor == null)
                    throw new NumberFormatException("Incomplete parsing");
                break; // Done.
            }
        }
        // Requires one valid digit character and checks for opposite overflow.
        if ((result == 0) && ((end == 0) || (csq.charAt(i - 1) != '0')))
            throw new NumberFormatException("Cannot parse " + csq + " as int");
        if ((result == Integer.MIN_VALUE) && !isNegative)
            throw new NumberFormatException("Overflow");
        if (cursor != null)
            cursor.setIndex(i);
        return isNegative ? result : -result;
    }

    private static int parseIntString(String csq, int radix, Cursor cursor) {
        // Parsing block identical for all CharSequences.
        final int start = (cursor != null) ? cursor.getIndex() : 0;
        final int end = (cursor != null) ? cursor.getEndIndex() : csq.length();
        boolean isNegative = false;
        int result = 0; // Accumulates negatively (avoid MIN_VALUE overflow).
        int i = start;
        for (; i < end; i++) {
            char c = csq.charAt(i);
            int digit = (c <= '9') ? c - '0'
                    : ((c <= 'Z') && (c >= 'A')) ? c - 'A' + 10
                            : ((c <= 'z') && (c >= 'a')) ? c - 'a' + 10 : -1;
            if ((digit >= 0) && (digit < radix)) {
                int newResult = result * radix - digit;
                if (newResult > result)
                    throw new NumberFormatException("Overflow");
                result = newResult;
            } else if ((c == '-') && (i == start)) {
                isNegative = true;
            } else if ((c == '+') && (i == start)) {
                // Ok.
            } else {
                if (cursor == null)
                    throw new NumberFormatException("Incomplete parsing");
                break; // Done.
            }
        }
        // Requires one valid digit character and checks for opposite overflow.
        if ((result == 0) && ((end == 0) || (csq.charAt(i - 1) != '0')))
            throw new NumberFormatException("Cannot parse " + csq + " as int");
        if ((result == Integer.MIN_VALUE) && !isNegative)
            throw new NumberFormatException("Overflow");
        if (cursor != null)
            cursor.setIndex(i);
        return isNegative ? result : -result;
    }

    private static int parseIntCharSequence(CharSequence csq, int radix,
            Cursor cursor) {
        // Parsing block identical for all CharSequences.
        final int start = (cursor != null) ? cursor.getIndex() : 0;
        final int end = (cursor != null) ? cursor.getEndIndex() : csq.length();
        boolean isNegative = false;
        int result = 0; // Accumulates negatively (avoid MIN_VALUE overflow).
        int i = start;
        for (; i < end; i++) {
            char c = csq.charAt(i);
            int digit = (c <= '9') ? c - '0'
                    : ((c <= 'Z') && (c >= 'A')) ? c - 'A' + 10
                            : ((c <= 'z') && (c >= 'a')) ? c - 'a' + 10 : -1;
            if ((digit >= 0) && (digit < radix)) {
                int newResult = result * radix - digit;
                if (newResult > result)
                    throw new NumberFormatException("Overflow");
                result = newResult;
            } else if ((c == '-') && (i == start)) {
                isNegative = true;
            } else if ((c == '+') && (i == start)) {
                // Ok.
            } else {
                if (cursor == null)
                    throw new NumberFormatException("Incomplete parsing");
                break; // Done.
            }
        }
        // Requires one valid digit character and checks for opposite overflow.
        if ((result == 0) && ((end == 0) || (csq.charAt(i - 1) != '0')))
            throw new NumberFormatException("Cannot parse " + csq + " as int");
        if ((result == Integer.MIN_VALUE) && !isNegative)
            throw new NumberFormatException("Overflow");
        if (cursor != null)
            cursor.setIndex(i);
        return isNegative ? result : -result;
    }

    /**
     * Parses the specified character sequence as a decimal <code>long</code>.
     *
     * @param  csq the character sequence to parse.
     * @return <code>parseLong(csq, 10)</code>
     * @throws NumberFormatException if the specified character sequence
     *         does not contain a parsable <code>long</code>.
     * @see    #parseLong(CharSequence, int)
     */
    public static long parseLong(CharSequence csq) {
        return parseLong(csq, 10);
    }

    /**
     * Equivalent to {@link #parseLong(CharSequence)} 
     * (for J2ME compatibility).
     */
    public static long parseLong(String str) {
        return parseLongString(str, 10, null);
    }

    /**
     * Parses the specified character sequence as a signed <code>long</code>
     * in the specified radix.
     *
     * @param  csq the character sequence to parse.
     * @param  radix the radix to be used while parsing.
     * @return the corresponding <code>long</code>.
     * @throws NumberFormatException if the specified character sequence
     *         does not contain a parsable <code>long</code>.
     */
    public static long parseLong(CharSequence csq, int radix) {
        return parseLong(csq, radix, null);
    }

    /**
     * Equivalent to {@link #parseLong(CharSequence, int)} 
     * (for J2ME compatibility).
     */
    public static long parseLong(String str, int radix) {
        return parseLongString(str, radix, null);
    }

    /**
     * Parses the specified character sequence from the specified position 
     * as a signed <code>long</code> in the specified radix.
     *
     * @param  csq the character sequence to parse.
     * @param  radix the radix to be used while parsing.
     * @param  cursor the current cursor position (being maintained) or
     *         <code>null</code> if the whole character sequence is parsed.
     * @return the corresponding <code>long</code>.
     * @throws NumberFormatException if the specified character sequence
     *         does not contain a parsable <code>long</code>.
     */
    public static long parseLong(CharSequence csq, int radix, Cursor cursor) {
        // Avoids dynamic cost of CharSequence.charAt
        if (csq instanceof CharArray)
            return parseLongCharArray((CharArray) csq, radix, cursor);
        if (csq instanceof TextBuilder)
            return parseLongTextBuilder((TextBuilder) csq, radix, cursor);
        if (csq instanceof Text)
            return parseLongText((Text) csq, radix, cursor);
        if (((Object) csq) instanceof String)
            return parseLongString((String) ((Object) csq), radix, cursor);
        return parseLongCharSequence(csq, radix, cursor);
    }

    private static long parseLongCharArray(CharArray csq, int radix,
            Cursor cursor) {
        // Parsing block identical for all CharSequences.
        final int start = (cursor != null) ? cursor.getIndex() : 0;
        final int end = (cursor != null) ? cursor.getEndIndex() : csq.length();
        boolean isNegative = false;
        long result = 0; // Accumulates negatively (avoid MIN_VALUE overflow).
        int i = start;
        for (; i < end; i++) {
            char c = csq.charAt(i);
            int digit = (c <= '9') ? c - '0'
                    : ((c <= 'Z') && (c >= 'A')) ? c - 'A' + 10
                            : ((c <= 'z') && (c >= 'a')) ? c - 'a' + 10 : -1;
            if ((digit >= 0) && (digit < radix)) {
                long newResult = result * radix - digit;
                if (newResult > result)
                    throw new NumberFormatException("Overflow");
                result = newResult;
            } else if ((c == '-') && (i == start)) {
                isNegative = true;
            } else if ((c == '+') && (i == start)) {
                // Ok.
            } else {
                if (cursor == null)
                    throw new NumberFormatException("Incomplete parsing");
                break; // Done.
            }
        }
        // Requires one valid digit character and checks for opposite overflow.
        if ((result == 0) && ((end == 0) || (csq.charAt(i - 1) != '0')))
            throw new NumberFormatException("Cannot parse " + csq + " as int");
        if ((result == Long.MIN_VALUE) && !isNegative)
            throw new NumberFormatException("Overflow");
        if (cursor != null)
            cursor.setIndex(i);
        return isNegative ? result : -result;
    }

    private static long parseLongTextBuilder(TextBuilder csq, int radix,
            Cursor cursor) {
        // Parsing block identical for all CharSequences.
        final int start = (cursor != null) ? cursor.getIndex() : 0;
        final int end = (cursor != null) ? cursor.getEndIndex() : csq.length();
        boolean isNegative = false;
        long result = 0; // Accumulates negatively (avoid MIN_VALUE overflow).
        int i = start;
        for (; i < end; i++) {
            char c = csq.charAt(i);
            int digit = (c <= '9') ? c - '0'
                    : ((c <= 'Z') && (c >= 'A')) ? c - 'A' + 10
                            : ((c <= 'z') && (c >= 'a')) ? c - 'a' + 10 : -1;
            if ((digit >= 0) && (digit < radix)) {
                long newResult = result * radix - digit;
                if (newResult > result)
                    throw new NumberFormatException("Overflow");
                result = newResult;
            } else if ((c == '-') && (i == start)) {
                isNegative = true;
            } else if ((c == '+') && (i == start)) {
                // Ok.
            } else {
                if (cursor == null)
                    throw new NumberFormatException("Incomplete parsing");
                break; // Done.
            }
        }
        // Requires one valid digit character and checks for opposite overflow.
        if ((result == 0) && ((end == 0) || (csq.charAt(i - 1) != '0')))
            throw new NumberFormatException("Cannot parse " + csq + " as int");
        if ((result == Long.MIN_VALUE) && !isNegative)
            throw new NumberFormatException("Overflow");
        if (cursor != null)
            cursor.setIndex(i);
        return isNegative ? result : -result;
    }

    private static long parseLongText(Text csq, int radix, Cursor cursor) {
        // Parsing block identical for all CharSequences.
        final int start = (cursor != null) ? cursor.getIndex() : 0;
        final int end = (cursor != null) ? cursor.getEndIndex() : csq.length();
        boolean isNegative = false;
        long result = 0; // Accumulates negatively (avoid MIN_VALUE overflow).
        int i = start;
        for (; i < end; i++) {
            char c = csq.charAt(i);
            int digit = (c <= '9') ? c - '0'
                    : ((c <= 'Z') && (c >= 'A')) ? c - 'A' + 10
                            : ((c <= 'z') && (c >= 'a')) ? c - 'a' + 10 : -1;
            if ((digit >= 0) && (digit < radix)) {
                long newResult = result * radix - digit;
                if (newResult > result)
                    throw new NumberFormatException("Overflow");
                result = newResult;
            } else if ((c == '-') && (i == start)) {
                isNegative = true;
            } else if ((c == '+') && (i == start)) {
                // Ok.
            } else {
                if (cursor == null)
                    throw new NumberFormatException("Incomplete parsing");
                break; // Done.
            }
        }
        // Requires one valid digit character and checks for opposite overflow.
        if ((result == 0) && ((end == 0) || (csq.charAt(i - 1) != '0')))
            throw new NumberFormatException("Cannot parse " + csq + " as int");
        if ((result == Long.MIN_VALUE) && !isNegative)
            throw new NumberFormatException("Overflow");
        if (cursor != null)
            cursor.setIndex(i);
        return isNegative ? result : -result;
    }

    private static long parseLongString(String csq, int radix, Cursor cursor) {
        // Parsing block identical for all CharSequences.
        final int start = (cursor != null) ? cursor.getIndex() : 0;
        final int end = (cursor != null) ? cursor.getEndIndex() : csq.length();
        boolean isNegative = false;
        long result = 0; // Accumulates negatively (avoid MIN_VALUE overflow).
        int i = start;
        for (; i < end; i++) {
            char c = csq.charAt(i);
            int digit = (c <= '9') ? c - '0'
                    : ((c <= 'Z') && (c >= 'A')) ? c - 'A' + 10
                            : ((c <= 'z') && (c >= 'a')) ? c - 'a' + 10 : -1;
            if ((digit >= 0) && (digit < radix)) {
                long newResult = result * radix - digit;
                if (newResult > result)
                    throw new NumberFormatException("Overflow");
                result = newResult;
            } else if ((c == '-') && (i == start)) {
                isNegative = true;
            } else if ((c == '+') && (i == start)) {
                // Ok.
            } else {
                if (cursor == null)
                    throw new NumberFormatException("Incomplete parsing");
                break; // Done.
            }
        }
        // Requires one valid digit character and checks for opposite overflow.
        if ((result == 0) && ((end == 0) || (csq.charAt(i - 1) != '0')))
            throw new NumberFormatException("Cannot parse " + csq + " as int");
        if ((result == Long.MIN_VALUE) && !isNegative)
            throw new NumberFormatException("Overflow");
        if (cursor != null)
            cursor.setIndex(i);
        return isNegative ? result : -result;
    }

    private static long parseLongCharSequence(CharSequence csq, int radix,
            Cursor cursor) {
        // Parsing block identical for all CharSequences.
        final int start = (cursor != null) ? cursor.getIndex() : 0;
        final int end = (cursor != null) ? cursor.getEndIndex() : csq.length();
        boolean isNegative = false;
        long result = 0; // Accumulates negatively (avoid MIN_VALUE overflow).
        int i = start;
        for (; i < end; i++) {
            char c = csq.charAt(i);
            int digit = (c <= '9') ? c - '0'
                    : ((c <= 'Z') && (c >= 'A')) ? c - 'A' + 10
                            : ((c <= 'z') && (c >= 'a')) ? c - 'a' + 10 : -1;
            if ((digit >= 0) && (digit < radix)) {
                long newResult = result * radix - digit;
                if (newResult > result)
                    throw new NumberFormatException("Overflow");
                result = newResult;
            } else if ((c == '-') && (i == start)) {
                isNegative = true;
            } else if ((c == '+') && (i == start)) {
                // Ok.
            } else {
                if (cursor == null)
                    throw new NumberFormatException("Incomplete parsing");
                break; // Done.
            }
        }
        // Requires one valid digit character and checks for opposite overflow.
        if ((result == 0) && ((end == 0) || (csq.charAt(i - 1) != '0')))
            throw new NumberFormatException("Cannot parse " + csq + " as int");
        if ((result == Long.MIN_VALUE) && !isNegative)
            throw new NumberFormatException("Overflow");
        if (cursor != null)
            cursor.setIndex(i);
        return isNegative ? result : -result;
    }

    /**
     * Parses the specified character sequence as a <code>float</code>.
     *
     * @param  csq the character sequence to parse.
     * @return the float number represented by the specified character sequence.
     *@JVM-1.1+@
     public static float parseFloat(CharSequence csq) {
     return (float) parseDouble(csq);
     }
     /**/

    /**
     * Equivalent to {@link #parseFloat(CharSequence)} 
     * (for J2ME compatibility).
     *@JVM-1.1+@
     public static float parseFloat(String str) {
     return (float) parseDoubleString(str, null);
     }
     /**/

    /**
     * Parses the specified character sequence from the specified position 
     * as a <code>float</code>.
     *
     * @param  csq the character sequence to parse.
     * @param  cursor the current cursor position (being maintained).
     * @return the float number represented by the specified character sequence.
     *@JVM-1.1+@
     public static float parseFloat(CharSequence csq, Cursor cursor) {
     return (float) parseDouble(csq, cursor);
     }
     /**/

    /**
     * Parses the specified character sequence as a <code>double</code>.
     * The format must be of the form:<code>
     * &lt;decimal&gt;{'.'&lt;fraction&gt;}{'E|e'&lt;exponent&gt;}</code>.
     *
     * @param  csq the character sequence to parse.
     * @return the double number represented by this character sequence.
     * @throws NumberFormatException if the character sequence does not contain
     *         a parsable <code>double</code>.
     *@JVM-1.1+@
     public static double parseDouble(CharSequence csq)
     throws NumberFormatException {
     return parseDouble(csq, null);
     }
     /**/

    /**
     * Equivalent to {@link #parseDouble(CharSequence)} 
     * (for J2ME compatibility).
     *@JVM-1.1+@
     public static double parseDouble(String str) {
     return parseDoubleString(str, null);
     }
     /**/

    /**
     * Parses the specified character sequence from the specified position 
     * as a <code>double</code>.
     *
     * @param  csq the character sequence to parse.
     * @param  cursor the current cursor position (being maintained).
     * @return the double number represented by this character sequence.
     * @throws NumberFormatException if the character sequence does not contain
     *         a parsable <code>double</code>.
     *@JVM-1.1+@
     public static double parseDouble(CharSequence csq, Cursor cursor)
     throws NumberFormatException {
     // Avoids dynamic cost of CharSequence.charAt
     if (csq instanceof CharArray)
     return parseDoubleCharArray((CharArray) csq, cursor);
     if (csq instanceof TextBuilder)
     return parseDoubleTextBuilder((TextBuilder) csq, cursor);
     if (csq instanceof Text)
     return parseDoubleText((Text) csq, cursor);
     if (((Object) csq) instanceof String)
     return parseDoubleString((String) ((Object) csq), cursor);
     return parseDoubleCharSequence(csq,  cursor);
     }
     
     private static double parseDoubleCharArray(CharArray csq, Cursor cursor)
     throws NumberFormatException {
     // Parsing block identical for all CharSequences.
     final int start = (cursor != null) ? cursor.getIndex() : 0;
     final int length = (cursor != null) ? cursor.getEndIndex() : csq.length();
     int i = start;
     char c = csq.charAt(i);
     
     // Checks for NaN.
     if ((c == 'N') && match("NaN", csq, i, length)) {
     if (cursor != null) cursor.setIndex(i + 3);
     return Double.NaN;
     }
     
     // Reads sign.
     boolean isNegative = (c == '-');
     if ((isNegative || (c == '+')) && (++i < length)) {
     c = csq.charAt(i);
     }
     
     // Checks for Infinity.
     if ((c == 'I') && match("Infinity", csq, i, length)) {
     if (cursor != null) cursor.setIndex(i + 8);
     return isNegative ? Double.NEGATIVE_INFINITY : Double.POSITIVE_INFINITY;
     }
     
     // Reads decimal and fraction (both merged to a long).
     long decimal = 0;
     int decimalPoint = -1;
     while (true) {
     int digit =  c - '0';
     if ((digit >= 0) && (digit < 10)) {
     long tmp = decimal * 10 + digit;
     if (tmp < decimal)
     throw new NumberFormatException("Too many digits - Overflow");
     decimal = tmp;
     } else if ((c == '.') && (decimalPoint < 0)) {
     decimalPoint = i;
     } else {
     break; // Done.
     }
     if (++i >= length)
     break;
     c = csq.charAt(i);
     }
     if (isNegative) {
     decimal = - decimal;
     }
     int fractionLength = (decimalPoint >= 0) ? i - decimalPoint - 1 : 0;
     
     // Reads exponent.
     int exp = 0;
     if ((i < length) && ((c == 'E') || (c == 'e'))) {
     c = csq.charAt(++i);
     boolean isNegativeExp = (c == '-');
     if ((isNegativeExp || (c == '+')) && (++i < length)) {
     c = csq.charAt(i);
     }
     while (true) {
     int digit =  c - '0';
     if ((digit >= 0) && (digit < 10)) {
     int tmp = exp * 10 + digit;
     if (tmp < exp)
     throw new NumberFormatException("Exponent Overflow");
     exp = tmp;
     } else {
     break; // Done.
     }
     if (++i >= length)
     break;
     c = csq.charAt(i);
     }
     if (isNegativeExp) {
     exp = -exp;
     }
     }
     if (cursor != null)
     cursor.setIndex(i);
     else if (i < length)
     throw new NumberFormatException("Incomplete parsing");    
     return javolution.lang.MathLib.toDoublePow10(decimal, exp - fractionLength);
     }
     
     private static double parseDoubleTextBuilder(TextBuilder csq, Cursor cursor)
     throws NumberFormatException {
     // Parsing block identical for all CharSequences.
     final int start = (cursor != null) ? cursor.getIndex() : 0;
     final int length = (cursor != null) ? cursor.getEndIndex() : csq.length();
     int i = start;
     char c = csq.charAt(i);
     
     // Checks for NaN.
     if ((c == 'N') && match("NaN", csq, i, length)) {
     if (cursor != null) cursor.setIndex(i + 3);
     return Double.NaN;
     }
     
     // Reads sign.
     boolean isNegative = (c == '-');
     if ((isNegative || (c == '+')) && (++i < length)) {
     c = csq.charAt(i);
     }
     
     // Checks for Infinity.
     if ((c == 'I') && match("Infinity", csq, i, length)) {
     if (cursor != null) cursor.setIndex(i + 8);
     return isNegative ? Double.NEGATIVE_INFINITY : Double.POSITIVE_INFINITY;
     }
     
     // Reads decimal and fraction (both merged to a long).
     long decimal = 0;
     int decimalPoint = -1;
     while (true) {
     int digit =  c - '0';
     if ((digit >= 0) && (digit < 10)) {
     long tmp = decimal * 10 + digit;
     if (tmp < decimal)
     throw new NumberFormatException("Too many digits - Overflow");
     decimal = tmp;
     } else if ((c == '.') && (decimalPoint < 0)) {
     decimalPoint = i;
     } else {
     break; // Done.
     }
     if (++i >= length)
     break;
     c = csq.charAt(i);
     }
     if (isNegative) {
     decimal = - decimal;
     }
     int fractionLength = (decimalPoint >= 0) ? i - decimalPoint - 1 : 0;
     
     // Reads exponent.
     int exp = 0;
     if ((i < length) && ((c == 'E') || (c == 'e'))) {
     c = csq.charAt(++i);
     boolean isNegativeExp = (c == '-');
     if ((isNegativeExp || (c == '+')) && (++i < length)) {
     c = csq.charAt(i);
     }
     while (true) {
     int digit =  c - '0';
     if ((digit >= 0) && (digit < 10)) {
     int tmp = exp * 10 + digit;
     if (tmp < exp)
     throw new NumberFormatException("Exponent Overflow");
     exp = tmp;
     } else {
     break; // Done.
     }
     if (++i >= length)
     break;
     c = csq.charAt(i);
     }
     if (isNegativeExp) {
     exp = -exp;
     }
     }
     if (cursor != null)
     cursor.setIndex(i);
     else if (i < length)
     throw new NumberFormatException("Incomplete parsing");    
     return javolution.lang.MathLib.toDoublePow10(decimal, exp - fractionLength);
     }

     private static double parseDoubleText(Text csq, Cursor cursor)
     throws NumberFormatException {
     // Parsing block identical for all CharSequences.
     final int start = (cursor != null) ? cursor.getIndex() : 0;
     final int length = (cursor != null) ? cursor.getEndIndex() : csq.length();
     int i = start;
     char c = csq.charAt(i);
     
     // Checks for NaN.
     if ((c == 'N') && match("NaN", csq, i, length)) {
     if (cursor != null) cursor.setIndex(i + 3);
     return Double.NaN;
     }
     
     // Reads sign.
     boolean isNegative = (c == '-');
     if ((isNegative || (c == '+')) && (++i < length)) {
     c = csq.charAt(i);
     }
     
     // Checks for Infinity.
     if ((c == 'I') && match("Infinity", csq, i, length)) {
     if (cursor != null) cursor.setIndex(i + 8);
     return isNegative ? Double.NEGATIVE_INFINITY : Double.POSITIVE_INFINITY;
     }
     
     // Reads decimal and fraction (both merged to a long).
     long decimal = 0;
     int decimalPoint = -1;
     while (true) {
     int digit =  c - '0';
     if ((digit >= 0) && (digit < 10)) {
     long tmp = decimal * 10 + digit;
     if (tmp < decimal)
     throw new NumberFormatException("Too many digits - Overflow");
     decimal = tmp;
     } else if ((c == '.') && (decimalPoint < 0)) {
     decimalPoint = i;
     } else {
     break; // Done.
     }
     if (++i >= length)
     break;
     c = csq.charAt(i);
     }
     if (isNegative) {
     decimal = - decimal;
     }
     int fractionLength = (decimalPoint >= 0) ? i - decimalPoint - 1 : 0;
     
     // Reads exponent.
     int exp = 0;
     if ((i < length) && ((c == 'E') || (c == 'e'))) {
     c = csq.charAt(++i);
     boolean isNegativeExp = (c == '-');
     if ((isNegativeExp || (c == '+')) && (++i < length)) {
     c = csq.charAt(i);
     }
     while (true) {
     int digit =  c - '0';
     if ((digit >= 0) && (digit < 10)) {
     int tmp = exp * 10 + digit;
     if (tmp < exp)
     throw new NumberFormatException("Exponent Overflow");
     exp = tmp;
     } else {
     break; // Done.
     }
     if (++i >= length)
     break;
     c = csq.charAt(i);
     }
     if (isNegativeExp) {
     exp = -exp;
     }
     }
     if (cursor != null)
     cursor.setIndex(i);
     else if (i < length)
     throw new NumberFormatException("Incomplete parsing");    
     return javolution.lang.MathLib.toDoublePow10(decimal, exp - fractionLength);
     }
     
     private static double parseDoubleString(String csq, Cursor cursor)
     throws NumberFormatException {
     // Parsing block identical for all CharSequences.
     final int start = (cursor != null) ? cursor.getIndex() : 0;
     final int length = (cursor != null) ? cursor.getEndIndex() : csq.length();
     int i = start;
     char c = csq.charAt(i);
     
     // Checks for NaN.
     if ((c == 'N') && match("NaN", csq, i, length)) {
     if (cursor != null) cursor.setIndex(i + 3);
     return Double.NaN;
     }
     
     // Reads sign.
     boolean isNegative = (c == '-');
     if ((isNegative || (c == '+')) && (++i < length)) {
     c = csq.charAt(i);
     }
     
     // Checks for Infinity.
     if ((c == 'I') && match("Infinity", csq, i, length)) {
     if (cursor != null) cursor.setIndex(i + 8);
     return isNegative ? Double.NEGATIVE_INFINITY : Double.POSITIVE_INFINITY;
     }
     
     // Reads decimal and fraction (both merged to a long).
     long decimal = 0;
     int decimalPoint = -1;
     while (true) {
     int digit =  c - '0';
     if ((digit >= 0) && (digit < 10)) {
     long tmp = decimal * 10 + digit;
     if (tmp < decimal)
     throw new NumberFormatException("Too many digits - Overflow");
     decimal = tmp;
     } else if ((c == '.') && (decimalPoint < 0)) {
     decimalPoint = i;
     } else {
     break; // Done.
     }
     if (++i >= length)
     break;
     c = csq.charAt(i);
     }
     if (isNegative) {
     decimal = - decimal;
     }
     int fractionLength = (decimalPoint >= 0) ? i - decimalPoint - 1 : 0;
     
     // Reads exponent.
     int exp = 0;
     if ((i < length) && ((c == 'E') || (c == 'e'))) {
     c = csq.charAt(++i);
     boolean isNegativeExp = (c == '-');
     if ((isNegativeExp || (c == '+')) && (++i < length)) {
     c = csq.charAt(i);
     }
     while (true) {
     int digit =  c - '0';
     if ((digit >= 0) && (digit < 10)) {
     int tmp = exp * 10 + digit;
     if (tmp < exp)
     throw new NumberFormatException("Exponent Overflow");
     exp = tmp;
     } else {
     break; // Done.
     }
     if (++i >= length)
     break;
     c = csq.charAt(i);
     }
     if (isNegativeExp) {
     exp = -exp;
     }
     }
     if (cursor != null)
     cursor.setIndex(i);
     else if (i < length)
     throw new NumberFormatException("Incomplete parsing");    
     return javolution.lang.MathLib.toDoublePow10(decimal, exp - fractionLength);
     }
     
     private static double parseDoubleCharSequence(CharSequence csq, Cursor cursor)
     throws NumberFormatException {
     // Parsing block identical for all CharSequences.
     final int start = (cursor != null) ? cursor.getIndex() : 0;
     final int length = (cursor != null) ? cursor.getEndIndex() : csq.length();
     int i = start;
     char c = csq.charAt(i);
     
     // Checks for NaN.
     if ((c == 'N') && match("NaN", csq, i, length)) {
     if (cursor != null) cursor.setIndex(i + 3);
     return Double.NaN;
     }
     
     // Reads sign.
     boolean isNegative = (c == '-');
     if ((isNegative || (c == '+')) && (++i < length)) {
     c = csq.charAt(i);
     }
     
     // Checks for Infinity.
     if ((c == 'I') && match("Infinity", csq, i, length)) {
     if (cursor != null) cursor.setIndex(i + 8);
     return isNegative ? Double.NEGATIVE_INFINITY : Double.POSITIVE_INFINITY;
     }
     
     // Reads decimal and fraction (both merged to a long).
     long decimal = 0;
     int decimalPoint = -1;
     while (true) {
     int digit =  c - '0';
     if ((digit >= 0) && (digit < 10)) {
     long tmp = decimal * 10 + digit;
     if (tmp < decimal)
     throw new NumberFormatException("Too many digits - Overflow");
     decimal = tmp;
     } else if ((c == '.') && (decimalPoint < 0)) {
     decimalPoint = i;
     } else {
     break; // Done.
     }
     if (++i >= length)
     break;
     c = csq.charAt(i);
     }
     if (isNegative) {
     decimal = - decimal;
     }
     int fractionLength = (decimalPoint >= 0) ? i - decimalPoint - 1 : 0;
     
     // Reads exponent.
     int exp = 0;
     if ((i < length) && ((c == 'E') || (c == 'e'))) {
     c = csq.charAt(++i);
     boolean isNegativeExp = (c == '-');
     if ((isNegativeExp || (c == '+')) && (++i < length)) {
     c = csq.charAt(i);
     }
     while (true) {
     int digit =  c - '0';
     if ((digit >= 0) && (digit < 10)) {
     int tmp = exp * 10 + digit;
     if (tmp < exp)
     throw new NumberFormatException("Exponent Overflow");
     exp = tmp;
     } else {
     break; // Done.
     }
     if (++i >= length)
     break;
     c = csq.charAt(i);
     }
     if (isNegativeExp) {
     exp = -exp;
     }
     }
     if (cursor != null)
     cursor.setIndex(i);
     else if (i < length)
     throw new NumberFormatException("Incomplete parsing");    
     return javolution.lang.MathLib.toDoublePow10(decimal, exp - fractionLength);
     }
     
     static boolean match(String str, CharSequence csq, int start, int length) {
     for (int i = 0; i < str.length(); i++) {
     if ((start + i >= length)
     || csq.charAt(start + i) != str.charAt(i))
     return false;
     }
     return true;
     }
     static boolean match(String str, String csq, int start, int length) {
     for (int i = 0; i < str.length(); i++) {
     if ((start + i >= length)
     || csq.charAt(start + i) != str.charAt(i))
     return false;
     }
     return true;
     }
     /**/

    /**
     * Formats the specified <code>boolean</code> and appends the resulting
     * text to the <code>Appendable</code> argument.
     *
     * @param  b a <code>boolean</code>.
     * @param  a the <code>Appendable</code> to append.
     * @return the specified <code>StringBuffer</code> object.
     * @throws IOException if an I/O exception occurs.
     * @see    #parseBoolean
     */
    public static Appendable format(boolean b, Appendable a) throws IOException {
        return b ? a.append('t').append('r').append('u').append('e') : a
                .append('f').append('a').append('l').append('s').append('e');
    }

    /**
     * Formats the specified <code>int</code> and appends the resulting
     * text (decimal representation) to the <code>Appendable</code> argument.
     *
     * <p> Note: This method is preferred to <code>Appendable.append(int)
     *           </code> as it does not create temporary <code>String</code>
     *           objects (several times faster for small numbers).</p>
     *
     * @param  i the <code>int</code> number.
     * @param  a the <code>Appendable</code> to append.
     * @return the specified <code>Appendable</code> object.
     * @throws IOException if an I/O exception occurs.
     * @see    #parseInt
     */
    public static Appendable format(int i, Appendable a) throws IOException {
        if (a instanceof TextBuilder)
            return ((TextBuilder) a).append(i);
        TextBuilder tmp = TextBuilder.newInstance();
        tmp.append(i);
        appendTo(a, tmp);
        TextBuilder.recycle(tmp);
        return a;
    }

    /**
     * Formats the specified <code>int</code> in the specified radix and appends
     * the resulting text to the <code>Appendable</code> argument.
     *
     * @param  i the <code>int</code> number.
     * @param  radix the radix.
     * @param  a the <code>Appendable</code> to append.
     * @return the specified <code>Appendable</code> object.
     * @throws  IllegalArgumentException if radix is not in [2 .. 36] range.
     * @throws IOException if an I/O exception occurs.
     * @see    #parseInt(CharSequence, int)
     */
    public static Appendable format(int i, int radix, Appendable a)
            throws IOException {
        if (a instanceof TextBuilder)
            return ((TextBuilder) a).append(i, radix);
        TextBuilder tmp = TextBuilder.newInstance();
        tmp.append(i, radix);
        appendTo(a, tmp);
        TextBuilder.recycle(tmp);
        return a;
    }

    /**
     * Formats the specified <code>long</code> and appends the resulting
     * text (decimal representation) to the <code>Appendable</code> argument.
     *
     * <p> Note: This method is preferred to <code>Appendable.append(long)
     *           </code> as it does not create temporary <code>String</code>
     *           objects (several times faster for small numbers).</p>
     *
     * @param  l the <code>long</code> number.
     * @param  a the <code>Appendable</code> to append.
     * @return the specified <code>Appendable</code> object.
     * @throws IOException if an I/O exception occurs.
     * @see    #parseLong
     */
    public static Appendable format(long l, Appendable a) throws IOException {
        if (a instanceof TextBuilder)
            return ((TextBuilder) a).append(l);
        TextBuilder tmp = TextBuilder.newInstance();
        tmp.append(l);
        appendTo(a, tmp);
        TextBuilder.recycle(tmp);
        return a;
    }

    /**
     * Formats the specified <code>long</code> in the specified radix and
     * appends the resulting text to the <code>Appendable</code> argument.
     *
     * @param  l the <code>long</code> number.
     * @param  radix the radix.
     * @param  a the <code>Appendable</code> to append.
     * @return the specified <code>Appendable</code> object.
     * @throws  IllegalArgumentException if radix is not in [2 .. 36] range.
     * @throws IOException if an I/O exception occurs.
     * @see    #parseLong(CharSequence, int)
     */
    public static Appendable format(long l, int radix, Appendable a)
            throws IOException {
        if (a instanceof TextBuilder)
            return ((TextBuilder) a).append(l, radix);
        TextBuilder tmp = TextBuilder.newInstance();
        tmp.append(l, radix);
        appendTo(a, tmp);
        TextBuilder.recycle(tmp);
        return a;
    }

    /**
     * Formats the specified <code>float</code> value.
     *
     * @param  f the <code>float</code> value.
     * @param  a the <code>Appendable</code> to append.
     * @return the specified <code>Appendable</code> object.
     * @throws IOException if an I/O exception occurs.
     * @see    TextBuilder#append(float)
     *@JVM-1.1+@
     public static Appendable format(float f, Appendable a)
     throws IOException {
     if (a instanceof TextBuilder)
     return ((TextBuilder) a).append(f);
     TextBuilder tmp = TextBuilder.newInstance();
     tmp.append(f);
     appendTo(a, tmp);
     TextBuilder.recycle(tmp);
     return a;
     }
     /**/

    /**
     * Formats the specified <code>double</code> value (16 or 17 digits output).
     *
     * @param  d the <code>double</code> value.
     * @param  a the <code>Appendable</code> to append.
     * @return the specified <code>Appendable</code> object.
     * @throws IOException if an I/O exception occurs.
     * @see    TextBuilder#append(double)
     *@JVM-1.1+@
     public static Appendable format(double d, Appendable a)
     throws IOException {
     if (a instanceof TextBuilder)
     return ((TextBuilder) a).append(d);
     TextBuilder tmp = TextBuilder.newInstance();
     tmp.append(d);
     appendTo(a, tmp);
     TextBuilder.recycle(tmp);
     return a;
     }
     /**/

    /**
     * Formats the specified <code>double</code> value according to the 
     * specified formatting arguments.
     *
     * @param  d the <code>double</code> value.
     * @param  digits the number of significative digits (excludes exponent) or
     *         <code>-1</code> to mimic the standard library (16 or 17 digits).
     * @param  scientific <code>true</code> to forces the use of the scientific 
     *         notation (e.g. <code>1.23E3</code>); <code>false</code> 
     *         otherwise. 
     * @param  showZero <code>true</code> if trailing fractional zeros are 
     *         represented; <code>false</code> otherwise.
     * @param  a the <code>Appendable</code> to append.
     * @return the specified <code>Appendable</code> object.
     * @throws IllegalArgumentException if <code>(digits &gt; 19)</code>)
     * @throws IOException if an I/O exception occurs.
     * @see    TextBuilder#append(double, int, boolean, boolean)
     *@JVM-1.1+@
     public static Appendable format(double d, int digits,
     boolean scientific, boolean showZero, Appendable a) throws IOException {
     if (a instanceof TextBuilder)
     return ((TextBuilder) a).append(d, digits, scientific, showZero);
     TextBuilder tmp = TextBuilder.newInstance();
     tmp.append(d, digits, scientific, showZero);
     appendTo(a, tmp);
     TextBuilder.recycle(tmp);
     return a;
     }
     /**/

    /**
     * Appends to the specified appendable the text builder argument
     * (for text builder less than 32 characters).
     *
     * @param  a the appendable.
     * @param  txt the text to be append.
     * @throws IOException if an I/O exception occurs.
     */
    private static void appendTo(Object to, TextBuilder txt) throws IOException {
        if (to instanceof StringBuffer) {
            txt.appendTo((StringBuffer) to);
        /* @JVM-1.5+@            
        } else if (to instanceof StringBuilder) {
            txt.appendTo((StringBuilder) to);
        /**/
        } else {
            ((Appendable) to).append(txt);
        }
    }

}