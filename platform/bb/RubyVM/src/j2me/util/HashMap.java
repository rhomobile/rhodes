/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.util;
import javolution.util.FastMap;
public class HashMap extends FastMap {
    
    public HashMap() {
        super();
    }
    public HashMap(int capacity) {
        super(capacity);
    }

    public HashMap(Map/*<? extends K, ? extends V>*/map) {
        super(map);
    }
    
    public HashMap clone()
    {
        return new HashMap(this); 
    }
}
