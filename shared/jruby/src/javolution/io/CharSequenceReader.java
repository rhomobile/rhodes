/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.io;

import j2me.lang.CharSequence;
import j2me.lang.IllegalStateException;
import java.io.IOException;
import java.io.Reader;
import javolution.lang.MathLib;
import javolution.lang.Reusable;
import javolution.text.Appendable;
import javolution.text.CharArray;
import javolution.text.Text;
import javolution.text.TextBuilder;

/**
 * <p> This class allows any <code>CharSequence</code> to be used as 
 *     a reader.</p>
 *
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 3.8, May 8, 2004
 */
public final class CharSequenceReader extends Reader implements Reusable {

    /**
     * Holds the character sequence input.
     */
    private CharSequence _input;

    /**
     * Holds the current index into the character sequence.
     */
    private int _index;

    /**
     * Creates a new character sequence reader for which the character 
     * sequence input is not set.
     * 
     * @see #setInput
     */
    public CharSequenceReader() {
    }

    /**
     * Sets the character sequence to use for reading.
     *
     * @param  charSequence the character sequence to be read.
     * @return this reader.
     * @throws IllegalStateException if this reader is being reused and 
     *         it has not been {@link #close closed} or {@link #reset reset}.
     */
    public CharSequenceReader setInput(CharSequence charSequence) {
        if (_input != null)
            throw new IllegalStateException("Reader not closed or reset");
        _input = charSequence;
        return this;
    }

    /**
     * Indicates if this stream is ready to be read.
     *
     * @return <code>true</code> if this reader has remaining characters to 
     *         read; <code>false</code> otherwise.
     * @throws  IOException if an I/O error occurs.
     */
    public boolean ready() throws IOException {
        if (_input == null)
            throw new IOException("Reader closed");
        return true;
    }

    /**
     * Closes and {@link #reset resets} this reader for reuse.
     */
    public void close() {
        if (_input != null) {
            reset();
        }
    }

    /**
     * Reads a single character.  This method does not block, <code>-1</code>
     * is returned if the end of the character sequence input has been reached.
     *
     * @return the 31-bits Unicode of the character read, or -1 if there is 
     *         no more remaining bytes to be read.
     * @throws IOException if an I/O error occurs (e.g. incomplete 
     *         character sequence being read).
     */
    public int read() throws IOException {
        if (_input == null)
            throw new IOException("Reader closed");
        return (_index < _input.length()) ? _input.charAt(_index++) : -1;
    }

    /**
     * Reads characters into a portion of an array.  This method does not 
     * block.
     *
     * @param  cbuf the destination buffer.
     * @param  off the offset at which to start storing characters.
     * @param  len the maximum number of characters to read
     * @return the number of characters read,  or -1 if there is no more
     *         character to be read.
     * @throws IOException if an I/O error occurs.
     */
    public int read(char cbuf[], int off, int len) throws IOException {
        if (_input == null)
            throw new IOException("Reader closed");
        final int inputLength = _input.length();
        if (_index >= inputLength)
            return -1;
        final int count = MathLib.min(inputLength - _index, len);
        final Object csq = _input;
        if (csq instanceof String) {
            String str = (String) csq;
            str.getChars(_index, _index + count, cbuf, off);
        } else if (csq instanceof Text) {
            Text txt = (Text) csq;
            txt.getChars(_index, _index + count, cbuf, off);
        } else if (csq instanceof TextBuilder) {
            TextBuilder tb = (TextBuilder) csq;
            tb.getChars(_index, _index + count, cbuf, off);
        } else if (csq instanceof CharArray) {
            CharArray ca = (CharArray) csq;
            System
                    .arraycopy(ca.array(), _index + ca.offset(), cbuf, off,
                            count);
        } else { // Generic CharSequence.
            for (int i = off, n = off + count, j = _index; i < n;) {
                cbuf[i++] = _input.charAt(j++);
            }
        }
        _index += count;
        return count;
    }

    /**
     * Reads characters into the specified appendable. This method does not 
     * block.
     *
     * @param  dest the destination buffer.
     * @throws IOException if an I/O error occurs.
     */
    public void read(Appendable dest) throws IOException {
        if (_input == null)
            throw new IOException("Reader closed");
        dest.append(_input);
    }

    // Implements Reusable.
    public void reset() {
        _index = 0;
        _input = null;
    }

}