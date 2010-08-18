/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util;
public
class MissingResourceException extends RuntimeException {
    private String _className;
    private String _key;

    public MissingResourceException(String s, String className, String key) {
        super(s);
        _className = className;
        _key = key;
    }

    public String getClassName() {
        return _className;
    }

    public String getKey() {
        return _key;
    }
}
