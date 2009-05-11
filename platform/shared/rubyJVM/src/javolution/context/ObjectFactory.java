/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.context;

import javolution.util.FastMap;
import j2me.lang.ThreadLocal;

/**
 * <p> This class represents an object factory; it allows for object 
 *     recycling, pre-allocation and stack allocations.
 *     
 * <p> Object factories are recommended over class constructors (ref. "new" 
 *     keyword) to allows for custom allocation policy (see 
 *     {@link AllocatorContext}). For example:[code]
 *     static ObjectFactory<int[][]> BOARD_FACTORY = new ObjectFactory<int[][]>() { 
 *         protected int[][] create() {
 *             return new int[8][8];
 *         }
 *     };
 *     ...
 *     int[][] board = BOARD_FACTORY.object(); 
 *         // The board object might have been preallocated at start-up,
 *         // it might also be on the thread "stack/pool" for threads 
 *         // executing in a StackContext. 
 *     ...
 *     BOARD_FACTORY.recycle(board); // Immediate recycling of the board object (optional).                      
 *     [/code]</p>
 *     
 * <p> For arrays of variable length {@link ArrayFactory} is recommended.</p>
 * 
 * <p> For convenience, this class provides a static {@link #getInstance} method 
 *     to retrieve a factory implementation for any given class.
 *     For example:[code]
 *        ObjectFactory<ArrayList> listFactory = ObjectFactory.getInstance(ArrayList.class);
 *        ArrayList list = listFactory.object();
 *        ... // Do something.
 *        listFactory.recycle(list); // Optional.
 *    [/code]</p> 
 *          
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.2, August 14, 2007
 */
public abstract class ObjectFactory/*<T>*/{

    /**
     * Indicates if the objects products of this factory require
     * {@link #cleanup(Object) cleanup} when recycled.
     */
    private boolean _doCleanup = true;

    /**
     * Default constructor.
     */
    protected ObjectFactory() {
    }

    /**
     * Returns a factory implementation producing instances of the specified
     * class. By default this method returns a factory creating new objects 
     * using the class public no-arg constructor (through reflection). 
     * If that constructor is not accessible, the factory instance can be 
     * {@link #setInstance set explicitly}:[code]
     * class LogContext {
     *     public static final Class<LogContext> NULL = Null.class;
     *     ...
     *     private static class Null extends LogContext ... // Private.
     *     static {
     *          // Allows Null instances to be factory produced (even so the class is not accessible).
     *          ObjectFactory.setInstance(new ObjectFactory<Null> {
     *              protected Null create() { return new Null() }},
     *              Null.class);
     *     }
     *  }[/code]
     *  
     * @param forClass the class for which an object factory is returned.
     * @return an object factory producing instances of the specified class.
     */
    public static/*<T>*/ObjectFactory/*<T>*/getInstance(Class/*<T>*/forClass) {
        ObjectFactory factory = (ObjectFactory) Generic.CLASS_TO_FACTORY
                .get(forClass);
        return factory != null ? factory : Generic.newInstance(forClass);
    }

    /**
     * Sets explicitely the factory to be used for the specified class 
     * (see {@link #getInstance}).
     * 
     * @param factory the factory to use.
     * @param forClass the associated class.
     * @see #getInstance(Class)
     */
    public static/*<T>*/void setInstance(ObjectFactory/*<T>*/factory,
            Class/*<T>*/forClass) {
        Generic.CLASS_TO_FACTORY.put(forClass, factory);
    }

    /**
     * Returns a factory object possibly recycled or preallocated.
     * This method is equivalent to <code>currentAllocator().nextInQueue()</code>.
     * 
     * @return a recycled, pre-allocated or new factory object.
     */
    public final Object/*{T}*/object() {
        final Allocator/*<T>*/ allocator = _allocator;
        return allocator.user == Thread.currentThread() ? 
             allocator.next() : currentAllocator().next();
    }

    private Allocator/*<T>*/ _allocator = NULL_ALLOCATOR; // Hopefully in the cache.   

    private static final Allocator NULL_ALLOCATOR = new Allocator() {
		protected Object allocate() {
			return null;
		}
		protected void recycle(Object object) {
		}};
    
    /**
     * Recycles the specified object.
     * This method is equivalent to <code>getAllocator().recycle(obj)</code>.
     * 
     * @param obj the object to be recycled.
     */
    public final void recycle(Object/*{T}*/obj) {
        currentAllocator().recycle(obj);
    }

    /**
     * Returns the factory allocator for the current thread (equivalent 
     * to <code>AllocatorContext.current().getAllocator(this)</code>).
     * 
     * @return the current object queue for this factory. 
     */
    public final Allocator/*<T>*/currentAllocator() {

        // Search thread-local value first.
        Allocator allocator = (Allocator) _localAllocator.get();
        if (allocator.user != null) // Active.
            return _allocator = allocator;

        // Retrieves allocator from current allocator context.
        allocator = ((AllocatorContext) AllocatorContext.getCurrent()).getAllocator(this);

        // Sets diverse shortcuts.
        _localAllocator.set(allocator);
        _allocator = allocator;

        // Returns the queue.
        return allocator;
    }

    private ThreadLocal _localAllocator = new ThreadLocal() {
        protected Object initialValue() {
            return NULL_ALLOCATOR;
        }
    };

    /**
     * Constructs a new object for this factory (using the <code>new</code> 
     * keyword).
     *
     * @return a new factory object.
     */
    protected abstract Object/*{T}*/create();

    /**
     * Cleans-up this factory's objects for future reuse. 
     * When overriden, this method is called on objects being recycled to 
     * dispose of system resources or to clear references to external
     * objects potentially on the heap (it allows these external objects to
     * be garbage collected immediately and therefore reduces the memory 
     * footprint). For example:[code]
     *     static ObjectFactory<ArrayList> ARRAY_LIST_FACTORY = new ObjectFactory<ArrayList>() { 
     *         protected ArrayList create() {
     *             return new ArrayList();
     *         }
     *         protected void cleanup(ArrayList obj) {
     *             obj.clear(); // Clears external references.
     *         }
     *     };[/code]
     *
     * @param  obj the factory object being recycled.
     */
    protected void cleanup(Object/*{T}*/obj) {
        _doCleanup = false;
    }

    /**
     * Indicates if this factory requires cleanup. 
     *
     * @return <code>true</code> if {@link #cleanup} is overriden and 
     *         {@link #cleanup} has been called at least once; 
     *         <code>false</code> otherwise.
     */
    protected final boolean doCleanup() {
        return _doCleanup;
    }

    // Generic implementation using public no-arg constructor (reflection).
    private static class Generic extends ObjectFactory {
        private static final FastMap CLASS_TO_FACTORY = new FastMap()
                .setShared(true);

        private final Class _class;

        private Generic(Class cls) {
            _class = cls;
        }

        private static Generic newInstance(Class cls) {
            Generic generic = new Generic(cls);
            CLASS_TO_FACTORY.put(cls, generic);
            return generic;
        }

        protected Object create() {
            try {
                return _class.newInstance();
            } catch (InstantiationException e) {
                throw new Error(
                        "Cannot instantiate no-arg constructor for "
                                + _class.getName()
                                + ", the factory should be set explicitly using ObjectFactory.setInstance");
            } catch (IllegalAccessException e) {
                throw new Error(
                        "Cannot access no-arg constructor for "
                                + _class.getName()
                                + ", the factory should be set explicitly using ObjectFactory.setInstance");
            }
        }
    }
     
}