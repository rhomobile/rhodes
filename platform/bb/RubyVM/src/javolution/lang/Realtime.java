/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2007 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.lang;

import javolution.text.Text;

/**
 * <p> This interface identifies <a href="http://www.rtsj.org/">RTSJ</a> safe 
 *     classes with predictable response time and supporting custom {@link 
 *     javolution.context.AllocatorContext allocation} policies (e.g.&nbsp; 
 *     {@link javolution.context.StackContext "stack"} allocations).</p>
 *     
 * <p> Instances of this class are typically created through an 
 *     {@link javolution.context.ObjectFactory ObjectFactory}. For example:
 *     [code]
 *         public final class Complex implements Realtime, ValueType {
 *             private double _real;
 *             private double _imaginary;
 *             private static ObjectFactory<Complex> FACTORY = new ObjectFactory() {
 *                 protected Complex create() { return new Complex(); }
 *             }
 *             private Complex() { }
 *             public static Complex valueOf(double real, double imaginary) {
 *                Complex c = FACTORY.object(); 
 *                c._real = real;
 *                c._imaginary = imaginary;
 *                return c;
 *             }
 *             public Complex copy() {
 *                 return Complex.valueOf(_real, _imaginary);
 *             }
 *             public Text toText() {
 *                 return Text.valueOf(_real).plus(" + ")
 *                     .plus(Text.valueOf(_imaginary).plus("i")); 
 *             }
 *         }[/code]<p>
 *         
 * <p> It should be noted that classes with no static reference field 
 *     or with only static final {@link Immutable immutable} fields are 
 *     always RTSJ safe. Such classes may implement this interface
 *     and be used while running in scoped memory (RTSJ) or non-heap 
 *     {@link javolution.context.AllocatorContext allocators} (Javolution).</p>
 *    
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.0, May 6, 2007
 */
public interface Realtime {

    /**
     * Returns the textual representation of this real-time object
     * (equivalent to <code>toString</code> except that the returned value
     * can be {@link javolution.context.StackContext "stack"} allocated and 
     * supports fast concatenation).
     * 
     * @return this object's textual representation.
     */
    Text toText();   
}