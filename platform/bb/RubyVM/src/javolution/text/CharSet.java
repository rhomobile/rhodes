/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.text;

import javolution.lang.Immutable;
import javolution.lang.MathLib;

/**
 * <p> This class represents a set of characters.</p>
 * <p> Instances of this class are typically used for parsing purpose
 *     (faster than regular expressions for simple patterns). For example:[code]
 *     // Integration with Text.
 *     Text number;
 *     int exponentIndex = num.indexOfAny(CharSet.valueOf('e', 'E'));
 *     
 *     // Integration with TextFormat.
 *     public List<Integer> parse(CharSequence csq, TextFormat.Cursor cursor) {
 *         FastTable<Integer> numbers = FastTable.newInstance();
 *         while (cursor.skip(CharSet.WHITESPACES, csq)) {
 *             numbers.add(TypeFormat.parseInt(csq, cursor));
 *         }
 *         return numbers;
 *     }
 *     [/code]   
 * 
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 3.7, January 1, 2006
 */
public final class CharSet implements Immutable {

    /**
     * Represents an empty character set.
     */
    public static final CharSet EMPTY = new CharSet(new long[0]);

    /**
     * Represents white spaces characters according to Java 
     * (see {@link Character#isWhitespace(char)}).
     */
    public static final CharSet WHITESPACES = CharSet.valueOf(new char[] { 0x9,
            0xA, 0xB, 0xC, 0xD, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x1680, 0x180E,
            0x2000, 0x2001, 0x2002, 0x2003, 0x2004, 0x2005, 0x2006, 0x2008,
            0x2009, 0x200A, 0x200B, 0x2028, 0x2029, 0x205F, 0x3000 });

    /**
     * Represents spaces characters according to Java 
     * (see {@link Character#isSpaceChar(char)}).
     */
    public static final CharSet SPACES = CharSet.valueOf(new char[] { 0x20,
            0xA0, 0x1680, 0x180E, 0x2000, 0x2001, 0x2002, 0x2003, 0x2004,
            0x2005, 0x2006, 0x2007, 0x2008, 0x2009, 0x200A, 0x200B, 0x2028,
            0x2029, 0x202F, 0x205F, 0x3000 });

    /**
     * Represents ISO control  characters according to Java 
     * (see {@link Character#isISOControl(char)}).
     */
    public static final CharSet ISO_CONTROLS = CharSet.valueOf(new char[] {
            0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC,
            0xD, 0xE, 0xF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x7F, 0x80, 0x81,
            0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C,
            0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
            0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F });

    /**
     * Holds the containment mapping.  
     */
    private final long[] _mapping;

    /**
     * Creates a character set with the specified mapping.
     * 
     * @param mapping the character set mapping.
     */
    private CharSet(long[] mapping) {
        _mapping = mapping;
    }

    /**
     * Returns the character set holding the specified characters.
     * 
     * @param chars the characters contained by this character set.
     * @return the corresponding character set.
     */
    public static CharSet valueOf(char[]/*...*/chars) {
        int maxChar = 0;
        for (int i = chars.length; --i >= 0;) {
            if (chars[i] > maxChar) {
                maxChar = chars[i];
            }
        }
        CharSet charSet = new CharSet(new long[(maxChar >> 6) + 1]);
        for (int i = chars.length; --i >= 0;) {
            char c = chars[i];
            charSet._mapping[c >> 6] |= 1L << (c & 63);
        }
        return charSet;
    }

    /**
     * Returns the character set holding the characters in the specified 
     * range.
     * 
     * @param first the first character.
     * @param last the last character.
     * @return the corresponding character set.
     * @throws IllegalArgumentException if <code>first > last</code>
     */
    public static CharSet rangeOf(char first, char last) {
        if (first > last)
            throw new IllegalArgumentException(
                    "first should be less or equal to last");
        CharSet charSet = new CharSet(new long[(last >> 6) + 1]);
        for (char c = first; c <= last; c++) {
            charSet._mapping[c >> 6] |= 1L << (c & 63);
        }
        return charSet;

    }

    /**
     * Indicates if the specified character is contained by this character set.
     * 
     * @param c the character to test.
     * @return <code>true</code> if this character set contains the specified
     *         character; <code>false</code> otherwise.
     */
    public boolean contains(char c) {
        final int i = c >> 6;
        return i < _mapping.length ? (_mapping[i] & (1L << (c & 63))) != 0
                : false;
    }

    /**
     * Returns the character set containing the characters from this 
     * character set plus the characters from the character set specified.
     * 
     * @param that the set containing the characters to be added.
     * @return <code>this + that</code>
     */
    public CharSet plus(CharSet that) {
        if (that._mapping.length > this._mapping.length)
            return that.plus(this);
        CharSet result = this.copy();
        for (int i = that._mapping.length; --i >= 0;) {
            result._mapping[i] |= that._mapping[i];
        }
        return result;
    }

    /**
     * Returns the character set containing the characters from this 
     * character minus the characters from the character set specified.
     * 
     * @param that the set containing the character to be removed.
     * @return <code>this - that</code>
     */
    public CharSet minus(CharSet that) {
        CharSet result = this.copy();
        for (int i = MathLib.min(this._mapping.length, that._mapping.length); --i >= 0;) {
            result._mapping[i] &= ~that._mapping[i];
        }
        return result;
    }

    /**
     * Returns the textual representation of this character set.
     *  
     * @return the textual representation.
     */
    public String toString() {
        TextBuilder tb = TextBuilder.newInstance();
        tb.append('{');
        int length = _mapping.length << 6;
        for (int i = 0; i < length; i++) {
            if (this.contains((char) i)) {
                if (tb.length() > 1) {
                    tb.append(',');
                    tb.append(' ');
                }
                tb.append('\'');
                tb.append((char) i);
                tb.append('\'');
            }
        }
        tb.append('}');
        return tb.toString();
    }

    /**
     * Returns a copy of this character set.
     *  
     * @return an independant copy.
     */
    private CharSet copy() {
        CharSet charSet = new CharSet(new long[this._mapping.length]);
        for (int i = _mapping.length; --i >= 0;) {
            charSet._mapping[i] = _mapping[i];
        }
        return charSet;
    }
}