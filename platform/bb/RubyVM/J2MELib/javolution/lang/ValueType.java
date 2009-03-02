/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2007 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.lang;

/**
 * <p> This interface identifies objects which can be manipulated by 
 *     value; a JVM implementation may allocate instances of this class 
 *     on the stack and pass references by copy.</p>
 *     
 * <p> {@link Realtime} instances can be "explicitly" allocated on the  
 *     "stack" by executing within a {@link javolution.context.StackContext 
 *     StackContext} and creating new instances with an  {@link 
 *     javolution.context.ObjectFactory ObjectFactory}. 
 *     It is the responsibility of the users to ensure 
 *     that "stack" objects are {@link javolution.context.StackContext#outerCopy
 *     copied out} when
 *     referenced outside of the stack context. For example:[code]
 *     public final class Complex implements Realtime, ValueType { ... }
 *     ...
 *     public Complex sumOf(Complex[] values) {
 *         StackContext.enter(); // Starts stack allocation.
 *         try {
 *             Complex sum = Complex.ZERO;
 *             for (Complex c : values) {
 *                 sum = sum.plus(c);
 *             }
 *             return StackContext.outerCopy(sum); // Copies outside the stack.
 *         } finally {
 *             StackContext.exit(); // Resets stacks.
 *         }
 *     }[/code]</p>
 *      
 * <p> <b>Note:</b> "Stack" allocation is not the only optimization that a VM 
 *     can do on {@link ValueType}. The VM might decide not to perform any 
 *     allocation at all and store values directly in registers.</p> 
 *             
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.0, May 6, 2007
 */
public interface ValueType extends Immutable {

    /**
     * Returns a deep copy of this object allocated in the memory area (RTSJ) 
     * and/or {@link javolution.context.AllocatorContext context} (Javolution)
     * of the calling thread (the one making the copy).
     * 
     * @return an object identical to this object but allocated by the calling
     *         thread (e.g. on the "stack" of the calling thread).
     */
    Object copy();
    
}