/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util.logging;

public class Level {

    public static final Level OFF = new Level(Integer.MAX_VALUE);

    public static final Level SEVERE = new Level(1000);

    public static final Level WARNING = new Level(900);
    
    public static final Level INFO = new Level(800);

    public static final Level CONFIG = new Level(700);

    public static final Level FINE = new Level(500);

    public static final Level FINER = new Level(400);

    public static final Level FINEST = new Level(300);

    public static final Level ALL = new Level(Integer.MIN_VALUE);

    int _value;
    private Level(int value) {
        _value = value;
    }
    public final int intValue() {
        return _value;
    }

    public void setLevel(Level newLevel) throws SecurityException {
        
    }
}
