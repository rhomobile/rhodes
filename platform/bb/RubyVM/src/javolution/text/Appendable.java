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

import java.io.IOException;

/**
 * <p> This class is equivalent to <code>java.lang.Appendable</code> 
 *     and is moved (refactored) to the <code>java.lang</code> system
 *     package for applications targetting the J2SE 5.0+ run-time.</p>
 * 
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 2.0, November 26, 2004
 */
public interface Appendable {

    /**
     * Appends the specified character. 
     *
     * @param  c the character to append.
     * @return <code>this</code>
     */
    Appendable append(char c) throws IOException;

    /**
     * Appends the specified character sequence. 
     *
     * @param  csq the character sequence to append.
     * @return <code>this</code>
     */
    Appendable append(CharSequence csq) throws IOException;

    /**
     * Appends a subsequence of the specified character sequence. 
     *
     * @param  csq the character sequence to append.
     * @param  start the index of the first character to append.
     * @param  end the index after the last character to append.
     * @return <code>this</code>
     */
    Appendable append(CharSequence csq, int start, int end) throws IOException;

}