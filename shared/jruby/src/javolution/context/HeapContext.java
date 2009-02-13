/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.context;

import j2me.lang.ThreadLocal;
import javolution.util.FastMap;
import javolution.util.FastTable;

/**
 * <p> This class represents the default allocator context. Allocations are 
 *     performed using the <code>new</code> keyword and explicit object 
 *     {@link ObjectFactory#recycle(Object) recycling} is supported:[code]
 *         char[] buffer = ArrayFactory.CHARS_FACTORY.array(4098); // Possibly recycled.
 *         while (reader.read(buffer) > 0) { ... }
 *         ArrayFactory.CHARS_FACTORY.recycle(buffer); // Explicit recycling.
 *     [/code]</p>
 *       
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.2, August 19, 2007
 */
public class HeapContext extends AllocatorContext {

    /**
     * Holds the factory to allocator mapping (per thread).
     */
    private static final ThreadLocal FACTORY_TO_ALLOCATOR = new ThreadLocal() {
        protected Object initialValue() {
            return new FastMap();
        }
    };

    /**
     * Holds the allocators which have been activated (per thread).
     */
    private static final ThreadLocal ACTIVE_ALLOCATORS = new ThreadLocal() {
        protected Object initialValue() {
            return new FastTable();
        }
    };

    /**
     * Enters a heap context.
     * 
     * @return the heap context entered.
     */
    public static HeapContext enter() {
        return (HeapContext) Context.enter(CLASS);
    }
    private static final Class CLASS = new HeapContext().getClass();

    /**
     * Exits the current heap context.
     * 
     * @return the heap context being exited.
     * @throws ClassCastException if the context is not a heap context.
     */
    public static/*HeapContext*/Context exit() {
        return (HeapContext) Context.exit();
    }

    /**
     * Default constructor.
     */
    public HeapContext() {
    }

    // Overrides.
    protected void deactivate() {
        FastTable allocators = (FastTable) ACTIVE_ALLOCATORS.get();
        for (int i = 0, n = allocators.size(); i < n;) {
            ((Allocator) allocators.get(i++)).user = null;
        }
        allocators.clear();
    }

    // Overrides.
    protected Allocator getAllocator(ObjectFactory factory) {
        final FastMap factoryToAllocator = (FastMap) FACTORY_TO_ALLOCATOR.get();
        HeapAllocator allocator = (HeapAllocator) factoryToAllocator
                .get(factory);
        if (allocator == null) {
            allocator = new HeapAllocator(factory);
            factoryToAllocator.put(factory, allocator);
        }
        if (allocator.user == null) { // Activate.
            allocator.user = Thread.currentThread();
            FastTable activeAllocators = (FastTable) ACTIVE_ALLOCATORS.get();
            activeAllocators.add(allocator);
        }
        return allocator;
    }

    // Overrides.
    protected void enterAction() {
        getOuter().getAllocatorContext().deactivate();
    }

    // Overrides.
    protected void exitAction() {
        this.deactivate();
    }

    // Holds heap allocator implementation.
    private static final class HeapAllocator extends Allocator {

        private final ObjectFactory _factory;

        public HeapAllocator(ObjectFactory factory) {
            _factory = factory;
        }

        protected Object allocate() {
            return _factory.create();
        }

        protected void recycle(Object object) {
            if (_factory.doCleanup()) {
                _factory.cleanup(object);
            }
            if (queueSize >= queue.length) {
                resize();
            }
            queue[queueSize++] = object;
        }

        public String toString() {
            return "Heap allocator for " + _factory.getClass();
        }
    }

}