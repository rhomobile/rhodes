/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.lang;

public abstract class Number {//implements java.io.Serializable {
    
    public abstract int intValue();

    public abstract long longValue();

    /*@JVM-1.1+@
    public abstract float floatValue();

    public abstract double doubleValue();
    */
    
    public byte byteValue() {
        return (byte)intValue();
    }

    public short shortValue() {
        return (short)intValue();
    }
    

}
