/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.lang;

public interface CharSequence {
    int length();

    char charAt(int index);

    CharSequence subSequence(int start, int end);

    public String toString();
}