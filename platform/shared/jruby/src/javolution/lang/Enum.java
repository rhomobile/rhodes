/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.lang;

import j2me.io.Serializable;
import j2me.lang.Comparable;
import j2mex.realtime.MemoryArea;

import javolution.util.FastMap;

/**
 * <p> This class is equivalent to <code>java.lang.Enum</code> 
 *     and is moved (refactored) to the <code>java.lang</code> system 
 *     package for applications targetting the J2SE 5.0+ run-time.</p>
 *    
 * <p> This is a clean-room implementation of the Enum base class.</p>
 * 
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 2.0, November 26, 2004
 */
public abstract class Enum implements Comparable, Serializable {

    /**
     * Holds the class to enum mapping.
     */
    private static final FastMap CLASS_TO_ENUMS = new FastMap();

    /**
     * Holds the enum's name.
     */
    private final String _name;

    /**
     * Holds the enum's position.
     */
    private final int _ordinal;

    /**
     * Returns the name of this enum.
     * 
     * @return the enum's name.
     */
    public final String name() {
        return _name;
    }

    /**
     * Returns the position of this enum in the enumeration (starting 
     * at <code>0</code>).
     * 
     * @return the enum's position.
     */
    public final int ordinal() {
        return _ordinal;
    }

    /**
     * Enum's base constructor. 
     * 
     * @param name the enum's name.
     * @param ordinal the enum's position.
     */
    protected Enum(String name, int ordinal) {
        _name = name;
        _ordinal = ordinal;
        synchronized (CLASS_TO_ENUMS) {
            MemoryArea.getMemoryArea(CLASS_TO_ENUMS).executeInArea(
                    new Runnable() {
                        public void run() {
                            FastMap nameToEnum = (FastMap) CLASS_TO_ENUMS
                                    .get(Enum.this.getClass());
                            if (nameToEnum == null) {
                                nameToEnum = new FastMap();
                                CLASS_TO_ENUMS.put(Enum.this.getClass(),
                                        nameToEnum);
                            }
                            Object prev = nameToEnum.put(_name, Enum.this);
                            if (prev != null) {
                                throw new IllegalArgumentException(
                                        "Duplicate enum " + _name);
                            }
                        }
                    });
        }
    }

    /**
     * Returns the <code>String</code> representation of this enum. 
     * 
     * @return the enum's name.
     */
    public String toString() {
        return _name;
    }

    /**
     * Indicates if two enums are equals. 
     * 
     * @param that the enum to be compared for equality.
     * @return <code>this == that</code>
     */
    public final boolean equals(Object that) {
        return this == that;
    }

    /**
     * Returns the enums' hashcode. 
     * 
     * @return <code>System.identityHashCode(this)</code>
     */
    public final int hashCode() {
        return System.identityHashCode(this);
    }

    /**
     * Compares the position of two enum from the same enumeration. 
     * 
     * @param  that the enum to be compared with.
     * @return a negative value, zero, or a positive value as this enum 
     *         is less than, equal to, or greater than the specified enum.
     * @throws ClassCastException if both enum do not belong to the same
     *         enumeration.
     */
    public final int compareTo(Object that) {
        Enum e = (Enum) that;
        if (this.getClass() == that.getClass()) {
            return this._ordinal - e._ordinal;
        } else {
            throw new ClassCastException();
        }
    }

    /**
     * Returns this enum's class. 
     * 
     * @return <code>this.getClass()</code>
     */
    public final Class getDeclaringClass() {
        return this.getClass();
    }

    /**
     * Returns the enum from the specified enum's class with the specified 
     * name.
     * 
     * @param enumType the enum's declaring class.
     * @param name the name of the enum to return.
     * @return the corresponding enum.
     * @throws IllegalArgumentException if the enum does not exist.
     */
    public static Enum valueOf(Class enumType, String name) {
        FastMap nameToEnum = (FastMap) CLASS_TO_ENUMS.get(enumType);
        if (nameToEnum != null) {
            Enum e = (Enum) nameToEnum.get(name);
            if (e != null) {
                return e;
            }
        }
        throw new IllegalArgumentException(enumType + "." + name + " not found");
    }

}
