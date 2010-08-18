/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.lang;

/**
 * <p> This interface identifies classes whose instances are not subject or 
 *     susceptible to change or variation after creation. Once a class is 
 *     declared immutable, any subclass must ensure immutability as well.</p>
 *     
 * <p> {@link Immutable} objects can safely be used in a multi-threaded 
 *     environment and <b>do not require defensive copying</b>.
 *     For example:[code]
 *     class Polygon implements Immutable {
 *          private List<Point2D> _vertices;
 *          public Polygon(List<Point2D> vertices) {
 *              _vertices = (vertices instanceof Immutable) ?
 *                   vertices : // Safe, the vertices cannot be modified by the client.
 *                   new FastTable<Point2D>(vertices); // Defensive copying required.
 *          }
 *     }[/code]</p>
 * @see <a href="http://en.wikipedia.org/wiki/Immutable_object">
 *      Wikipedia: Immutable Object<a>    
 * 
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 3.7, February 6, 2006
 */
public interface Immutable {

    // No method (tagging interface).
    
}