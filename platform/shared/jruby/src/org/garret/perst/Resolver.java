//-< Resolver.java >-------------------------------------------------*--------*
// JSQL                       Version 1.04       (c) 1999  GARRET    *     ?  *
// (Java SQL)                                                        *   /\|  *
//                                                                   *  /  \  *
//                          Created:     10-Dec-2002  K.A. Knizhnik  * / [] \ *
//                          Last update: 10-Dec-2002  K.A. Knizhnik  * GARRET *
//-------------------------------------------------------------------*--------*
// Abstraction of class resolver which can be used in JSQL
// Resolver can be used to replaced SQL JOINs: given object ID,
// it will provide reference to the resolved object
//-------------------------------------------------------------------*--------*

package org.garret.perst;


/**
 * Abstraction of class resolver.
 * Resolver can be used to replaced SQL JOINs: given object ID, 
 * it will provide reference to the resolved object
 */
public interface Resolver {
    /**
     * Resolve object
     * @param obj original object to be resolved
     * @return resolved object
     */
    public Object resolve(Object obj);
}

