/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.lang;

/**
 * <p> This interfaces identifies mutable objects capable of being used again
 *     or repeatedly; once {@link #reset reset}, reusable objects behave as if
 *     they were brand-new.</p>
 *      
 * <p> Reusable instances should not allocate new internal objects after  
 *     creation except for the purpose of increasing their internal capacities. 
 *     In such case the new allocations have to be performed in the same memory
 *     areas as the reusable objects themselves (necessary to avoid memory leaks
 *     or memory clashes when running on 
 *     <a href="https://rtsj.dev.java.net/">RTSJ</a> VMs). For example:[code]
 *     import javax.realtime.MemoryArea;
 *     public class Targets implements Reusable {
 *         private Target[] _targets = new Target[32];
 *         private int _count;
 *         public void add(Target target) {
 *             if (_count >= _targets.length) capacityOverflow(); 
 *             _targets[_count++] = target;
 *         }
 *         private void capacityOverflow() {
 *              MemoryArea.getMemoryArea(this).executeInArea(new Runnable() {
 *                  public void run() {
 *                       Target[] tmp = new Target[_targets.length * 2];
 *                       System.arraycopy(_targets, 0, tmp, 0, _count);
 *                       _targets = tmp;
 *                  }
 *              });
 *         }
 *         ...
 *     }[/code]</p>
 *     
 * <p> Instances of this class can safely reside in permanent memory 
 *     (e.g. <code>static</code> members) or be an integral part of a
 *     higher level component. For example:[code]
 *     public class XMLFormat {
 *          // RTSJ Unsafe! Memory leaks (when entries removed) or IllegalAssignmentError (when new entries while in ScopedArea).   
 *          static HashMap<Class, XMLFormat> ClassToFormat = HashMap<Class, XMLFormat>();
 *             
 *          // RTSJ safe! FastMap is Reusable. Removed entries are internally recycled, new entries are in ImmortalMemory.
 *          static FastMap<Class, XMLFormat> ClassToFormat = FastMap<Class, XMLFormat>();
 *     }[/code]</p>
 *     
 * <p> Reusable objects can also be allocated on the stack providing that
 *     their {@link javolution.context.ObjectFactory factory} cleanup 
 *     method calls the {@link #reset reset} method. For example:[code]
 *     public class Foo implements Reusable {
 *         private static final ObjectFactory<Foo> FACTORY = new ObjectFactory<Foo>() {
 *             protected Foo create() {
 *                 return new Foo();
 *             }
 *             protected void cleanup(Foo obj) {
 *                 obj.reset();
 *             }
 *         };
 *         public static Foo newInstance() {
 *             return FACTORY.object(); // On the "stack" when executing in a StackContext.
 *         } 
 *         ...
 *     }[/code]</p>
 *        
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 3.7, January 1, 2006
 */
public interface Reusable {

    /**
     * Resets the internal state of this object to its default values.
     */
    void reset();

}