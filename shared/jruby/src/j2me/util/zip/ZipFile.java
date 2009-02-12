/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util.zip;

import j2me.lang.UnsupportedOperationException;

import java.util.Enumeration;

public class ZipFile {

    public ZipFile(String fileName) {
    }

    public Enumeration entries() {
        throw new UnsupportedOperationException(
                "ZipFile operations not supported for J2ME build");
    }

}
