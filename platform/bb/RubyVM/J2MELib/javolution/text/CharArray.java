/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.text;

import j2me.lang.CharSequence;
import j2me.lang.Comparable;
import javolution.util.FastComparator;

/**
 * <p> This class represents a character sequence backed up by a 
 *     <code>char</code> array. Instances of this class are mutable and 
 *     are typically used/reused to hold temporary text (unlike 
 *     <code>String</code> they do not forces object creation).</p>
 *     
 * <p> Instances of this classes have the following properties:<ul>
 * 
 *     <li> They support equality or lexical comparison with any
 *          <code>CharSequence</code> (e.g. <code>String</code>).</li>
 *          
 *     <li> They have the same hashcode than <code>String</code> and can be
 *          used to retrieve data from maps for which the keys are 
 *          <code>String</code> instances.</li>
 *          
 *     <li> They support fast conversions to primitive types 
 *          (e.g. {@link #toBoolean() Boolean}, {@link #toInt int}).</li>
 *          
 *     </ul></p>
 * 
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 4.0, June 16, 2006
 */
public final class CharArray implements CharSequence, Comparable {

    /**
     * Holds the character array.
     */
    private char[] _array;

    /**
     * Holds the index of the first character.
     */
    private int _offset;

    /**
     * Holds the length of char sequence.
     */
    private int _length;

    /**
     * Holds the string representation of this CharArray (if known).
     */
    private String _asString;

    /**
     * Default constructor.
     */
    public CharArray() {
    }

    /**
     * Creates a character array from the specified String.
     * 
     * @param string the string source.
     */
    public CharArray(String string) {
        _array = string.toCharArray();
        _length = string.length();
        _asString = string;
    }

    /**
     * Creates a character array from the specified character sequence.
     * 
     * @param csq the character sequence source.
     */
    public CharArray(CharSequence csq) {
        _length = csq.length();
        _array = new char[_length];
        for (int i=0; i < _length;) {
            _array[i] = csq.charAt(i++);
        }
    }

    /**
     * Returns the underlying array (read-only).
     * The array returned should not be modified (unfortunately there is 
     * no way to make an array immutable in Java).
     * 
     * @return the underlying array.
     */
    public char[] array() {
        return _array;
    }

    /**
     * Returns the length of this character sequence.
     *
     * @return the number of characters (16-bits Unicode) composing this
     *         character sequence.
     */
    public int length() {
        return _length;
    }

    /**
     * Returns the offset of the first character in the underlying array.
     *
     * @return the offset of the first character.
     */
    public int offset() {
        return _offset;
    }

    /**
     * Sets the underlying array of this CharArray.
     *
     * @param offset the new offset.
     * @param array the new underlying array.
     * @param length the new length.
     * @return <code>this</code>
     */
    public CharArray setArray(char[] array, int offset, int length) {
        _array = array;
        _offset = offset;
        _length = length;
        _asString = null;
        return this;
    }

    /**
     * Returns the character at the specified index.
     *
     * @param  index the index of the character starting at <code>0</code>.
     * @return the character at the specified index of this character sequence.
     * @throws IndexOutOfBoundsException  if <code>((index < 0) || 
     *         (index >= length))</code>
     */
    public char charAt(int index) {
        if ((index < 0) || (index >= _length))
            throw new IndexOutOfBoundsException("index: " + index);
        return _array[_offset + index];
    }

    /**
     * Returns a new character sequence that is a subsequence of this sequence.
     *
     * @param  start the index of the first character inclusive.
     * @param  end the index of the last character exclusive.
     * @return the character sequence starting at the specified
     *         <code>start</code> position and ending just before the specified
     *         <code>end</code> position.
     * @throws IndexOutOfBoundsException if <code>(start < 0) || (end < 0) ||
     *         (start > end) || (end > this.length())</code>
     */
    public CharSequence subSequence(int start, int end) {
        if ((start < 0) || (end < 0) || (start > end) || (end > this.length()))
            throw new IndexOutOfBoundsException();
        CharArray chars = new CharArray();
        chars._array = _array;
        chars._offset = _offset + start;
        chars._length = end - start;
        return chars;
    }

    /**
     * Returns the offset within this character array of the first occurrence
     * of the specified characters sequence searching forward from this 
     * character array {@link #offset()} to <code>offset() + length()</code>.
     *
     * @param  csq a character sequence searched for.
     * @return the offset of the specified character sequence in the range
     *         <code>[offset(), offset() + length()[</code> 
     *         or <code>-1</code> if the character sequence is not found.
     */
    public final int offsetOf(CharSequence csq) {
        final char c = csq.charAt(0);
        final int csqLength = csq.length();
        for (int i=_offset, end = _offset + _length - csqLength + 1; i < end; i++) {
            if (_array[i] == c) { // Potential match.
                boolean match = true;
                for (int j = 1; j < csqLength; j++) {
                    if (_array[i + j] != csq.charAt(j)) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    return i;
                }
            }
        }
        return -1; 
    }

    /**
     * Returns the offset within this character array of the first occurrence
     * of the specified character searching forward from this 
     * character array {@link #offset()} to <code>offset() + length()</code>.
     *
     * @param  c the character to search for.
     * @return the offset of the specified character in the range
     *         <code>[offset(), offset() + length()[</code> 
     *         or <code>-1</code> if the character is not found.
     */
    public final int offsetOf(char c) {
        for (int i=_offset, end = _offset + _length; i < end; i++) {
            if (_array[i] == c) return i;   
        }
        return -1; 
    }
    
    /**
     * Returns the <code>String<code> corresponding to this character
     * sequence. The <code>String</code> returned is always allocated on the
     * heap and can safely be referenced elsewhere.
     *
     * @return the <code>java.lang.String</code> for this character sequence.
     */
    public String toString() {
        if (_asString == null) {
            _asString = new String(_array, _offset, _length);
        }
        return _asString;
    }

    /**
     * Returns the hash code for this {@link CharArray}.
     *
     * <p> Note: Returns the same hashCode as <code>java.lang.String</code>
     *           (consistent with {@link #equals})</p>
     * @return the hash code value.
     */
    public int hashCode() {
        if (_asString != null) return _asString.hashCode();
        int h = 0;
        for (int i = 0, j = _offset; i < _length; i++) {
            h = 31 * h + _array[j++];
        }
        return h;
    }

    /**
     * Compares this character sequence against the specified object
     * (<code>String</code> or <code>CharSequence</code>).
     *
     * @param  that the object to compare with.
     * @return <code>true</code> if both objects represent the same sequence;
     *         <code>false</code> otherwise.
     */
    public boolean equals(Object that) {
        if (that instanceof String) { 
            return equals((String) that);
        } else if (that instanceof CharArray) {
            return equals((CharArray) that);
        } else if (that instanceof CharSequence) {
            return equals((CharSequence) that);
        } else {
            return false;
        }
    }

    // Do not make public or String instances may not use equals(String)
    private boolean equals(CharSequence chars) {
        if (chars == null)
            return false;
        if (this._length != chars.length())
            return false;
        for (int i = _length, j = _offset + _length; --i >= 0;) {
            if (_array[--j] != chars.charAt(i))
                return false;
        }
        return true;
    }

    /**
     * Compares this character array against the specified {@link CharArray}.
     *
     * @param  that the character array to compare with.
     * @return <code>true</code> if both objects represent the same sequence;
     *         <code>false</code> otherwise.
     */
    public boolean equals(CharArray that) {
        if (this == that)
            return true;
        if (that == null)
            return false;
        if (this._length != that._length)
            return false;
        final char[] thatData = that._array;
        for (int i = that._offset + _length, j = _offset + _length; --j >= _offset;) {
            if (_array[j] != thatData[--i])
                return false;
        }
        return true;
    }

    /**
     * Compares this character array against the specified String.
     * In case of equality, the CharArray keeps a reference to the 
     * String for future comparisons.
     * 
     * @param  str the string  to compare with.
     * @return <code>true</code> if both objects represent the same sequence;
     *         <code>false</code> otherwise.
     */
    public boolean equals(String str) {
        if (_asString != null) 
            return (_asString == str) ? true : _asString.equals(str) ? 
                    (_asString = str) == str : false;
        if (str == null)
            return false;
        if (_length != str.length())
            return false;
        for (int i = _length, j = _offset + _length; --i >= 0;) {
            if (_array[--j] != str.charAt(i))
                return false;
        }
        _asString = str;
        return true;
    }


    /**
     * Compares this character array with the specified character
     * sequence lexicographically.
     *
     * @param   seq the character sequence to be compared.
     * @return  <code>{@link FastComparator#LEXICAL}.compare(this, seq)</code>
     * @throws  ClassCastException if the specifed object is not a
     *          <code>CharSequence</code>.
     */
    public int compareTo(Object seq) {
        return ((FastComparator)FastComparator.LEXICAL).compare(this, seq);
    }

    /**
     * Returns the <code>boolean</code> represented by this character array.
     *
     * @return the corresponding <code>boolean</code> value.
     * @throws NumberFormatException if this character sequence
     *         does not contain a parsable <code>boolean</code>.
     */
    public boolean toBoolean() {
        int i = _offset;
        if ((_length == 4) && (_array[i] == 't' || _array[i] == 'T')
                && (_array[++i] == 'r' || _array[i] == 'R')
                && (_array[++i] == 'u' || _array[i] == 'U')
                && (_array[++i] == 'e' || _array[i] == 'E'))
            return true;
        if ((_length == 5) && (_array[i] == 'f' || _array[i] == 'F')
                && (_array[++i] == 'a' || _array[i] == 'A')
                && (_array[++i] == 'l' || _array[i] == 'L')
                && (_array[++i] == 's' || _array[i] == 'S')
                && (_array[++i] == 'e' || _array[i] == 'E'))
            return false;
        throw new IllegalArgumentException("Cannot parse " + this
                + " as boolean");
    }

    /**
     * Returns the decimal <code>int</code> represented by this character array.
     *
     * @return <code>toInt(10)</code>
     * @throws NumberFormatException if this character sequence
     *         does not contain a parsable <code>int</code>.
     */
    public int toInt() { 
        return TypeFormat.parseInt(this);
    }

    /**
     * Returns the <code>int</code> represented by this character array
     * in the specified radix.
     * 
     * @param  radix the radix (e.g. <code>16</code> for hexadecimal).
     * @return the corresponding <code>int</code> value.
     * @throws NumberFormatException if this character sequence
     *         does not contain a parsable <code>int</code>.
     */
    public int toInt(int radix) {  
        return TypeFormat.parseInt(this, radix);
    }

    /**
     * Returns the decimal <code>long</code> represented by this character 
     * array.
     *
     * @return the corresponding <code>long</code> value.
     * @throws NumberFormatException if this character sequence
     *         does not contain a parsable <code>long</code>.
     */
    public long toLong() {
        return TypeFormat.parseLong(this);
    }

    /**
     * Returns the decimal <code>long</code> represented by this character 
     * array in the specified radix.
     * 
     * @param  radix the radix (e.g. <code>16</code> for hexadecimal).
     * @return the corresponding <code>long</code> value.
     * @throws NumberFormatException if this character sequence
     *         does not contain a parsable <code>long</code>.
     */
    public long toLong(int radix) {
        return TypeFormat.parseLong(this, radix);
    }

    /**
     * Returns the <code>float</code> represented by this character array.
     *
     * @return the corresponding <code>float</code> value.
     * @return <code>TypeFormat.parseFloat(this)</code>
     * @throws NumberFormatException if this character sequence
     *         does not contain a parsable <code>float</code>.
     /*@JVM-1.1+@
     public float toFloat() {
     return TypeFormat.parseFloat(this);
     }
     /**/

    /**
     * Returns the <code>double</code> represented by this character array.
     *
     * @return the corresponding <code>double</code> value.
     * @throws NumberFormatException if this character sequence
     *         does not contain a parsable <code>double</code>.
     /*@JVM-1.1+@
     public double toDouble() {
     return TypeFormat.parseDouble(this);
     }
     /**/

}