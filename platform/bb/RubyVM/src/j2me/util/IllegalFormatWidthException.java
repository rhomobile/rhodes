/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util;

public class IllegalFormatWidthException extends IllegalFormatException {
    int _width;

    public IllegalFormatWidthException(int w) {
        _width = w;
    }

    public int getWidth() {
        return _width;
    }
}