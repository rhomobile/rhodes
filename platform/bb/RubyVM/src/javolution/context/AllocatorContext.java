/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2007 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.context;

import javolution.lang.Configurable;

/**
 * <p> This class represents an allocator context; it defines the 
 *     the allocation policy of {@link ObjectFactory} products.</p>
 *     
 * <p> The {@link #DEFAULT default} implementation is an instance of
 *     {@link HeapContext} context.</p>
 *     
 * <p> Specializations may allocate from local stacks ({@link StackContext}),
 *     specific memory areas (e.g. {@link ImmortalContext}), aging pools (where
 *     objects sufficiently old are recycled), switchable spaces (objects from
 *     a particular frame are recycled when buffers are swapped), etc.</p>
 *     
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.2, August 19, 2007
 */
public abstract class AllocatorContext extends Context {

    /**
     * Holds the default allocator context instance.
     */
    private static volatile AllocatorContext _Default = new HeapContext();
    
    /**
     * Holds the default implementation ({@link HeapContext} instance).
     */
    public static final Configurable/*<Class<? extends AllocatorContext>>*/
    DEFAULT = new Configurable(_Default.getClass()) {
        protected void notifyChange() {
            _Default = (AllocatorContext) ObjectFactory.getInstance(
                    (Class) get()).object();
        }
    };

    /**
     * Default constructor.
     */
    protected AllocatorContext() {
    }

    /**
     * Returns the current allocator context. If the current thread has 
     * not entered an allocator context (e.g. new thread) then 
     * {@link #getDefault()} is returned.
     *
     * @return the current allocator context.
     */
    public static/*AllocatorContext*/Context getCurrent() {
        return Context.getCurrent().getAllocatorContext();
    }
 
    /**
     * Returns the default instance ({@link #DEFAULT} implementation).
     * 
     * @return the default instance.
     */
    public static AllocatorContext getDefault() {
        return AllocatorContext._Default;
    }    
    
    /**
     * Returns the allocator for the specified factory in this context.
     * 
     * @param factory the factory for which the allocator is returned.
     * @return the allocator producing instances of the specified factory.
     */
    protected abstract Allocator getAllocator(ObjectFactory factory);

    /**
     * Deactivates the {@link Allocator allocators} belonging to this context
     * for the current thread. This method is typically called when an inner 
     * allocator context is entered by the current thread, when exiting an 
     * allocator context or when a concurrent executor has completed its task
     * within this allocator context. Deactivated allocators have no
     * {@link Allocator#user user} (<code>null</code>).
     */
    protected abstract void deactivate();
    
    /**
     * <p> This class represents a {@link javolution.lang.Reference reference}
     *     allocated from the current {@link AllocatorContext}. 
     *     The reachability level of this reference is the scope of the 
     *     {@link AllocatorContext} in which it has been 
     *     {@link #newInstance created}.</p>
     *     
     * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
     * @version 5.0, April 14, 2007
     */
    public static class Reference/*<T>*/implements javolution.lang.Reference/*<T>*/{

        /**
         * Holds the factory.
         */
        private static final ObjectFactory FACTORY = new ObjectFactory() {
            protected Object create() {
                return new Reference();
            }

            protected void cleanup(Object obj) {
                ((Reference) obj)._value = null;
            }
        };

        /**
         * Holds the reference value.
         */
        private Object/*{T}*/_value;

        /**
         * Default constructor.
         */
        public Reference() {
        }

        /**
         * Returns a new stack reference instance allocated on the current stack
         * when executing in a {@link StackContext}.
         * 
         * @return a reference object possibly recycled.
         */
        public static/*<T>*/Reference /*<T>*/newInstance() {
            return (Reference) FACTORY.object();
        }

        /**
         * Returns the string representation of the current value of 
         * this reference.
         * 
         * @return <code>String.valueOf(this.get())</code>
         */
        public String toString() {
            return String.valueOf(_value);
        }

        // Implements Reference interface.
        public final Object/*{T}*/get() {
            return _value;
        }

        // Implements Reference interface.
        public final void set(Object/*{T}*/value) {
            _value = value;
        }
    }
}