/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.context;

import j2me.lang.Comparable;
import j2me.util.Map;
import javolution.util.FastMap;

/**
 * <p> This class represents a context persistent accross multiple program 
 *     executions. It is typically used to hold  
 *     {@link Reference persistent references}.</p>  
 *     
 * <p> How this context is loaded/saved is application specific. 
 *     Although, the simplest way is to use Javolution XML serialization 
 *     facility. For example:[code]
 *      import javolution.xml.XMLObjectReader;
 *      import javolution.xml.XMLObjectWriter;
 *      public void main(String[]) {
 *           // Loads persistent context (typically at start-up).
 *           XMLObjectReader reader = XMLObjectReader.newInstance(new FileInputStream("C:/persistent.xml"));
 *           PersistentContext.setCurrent(reader.read(PersistentContext.class));
 *           ...
 *           ...
 *           // Saves persistent context for future execution.
 *           XMLObjectWriter writer = XMLObjectWriter.newInstance(new FileOutputStream("C:/persistent.xml"));
 *           writer.write(PersistentContext.getCurrent(), PersistentContext.class);
 *      }[/code]</p>
 *     
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 4.2, December 31, 2006
 */
public class PersistentContext extends Context {

    /**
     * Holds the single instance.
     */
    private static PersistentContext _PersistentContext = new PersistentContext();

    /**
     * Holds current id to value mapping.
     */
    private final FastMap _idToValue = new FastMap();

    /**
     * Default constructor.
     */
    public PersistentContext() {
    }

    /**
     * Sets the persistent instance. 
     * 
     * @param ctx the persistent instance.
     */
    public static void setCurrent(PersistentContext ctx) {
        _PersistentContext = ctx;
        synchronized (Reference.INSTANCES) {
             for (FastMap.Entry e = Reference.INSTANCES.head(), end = Reference.INSTANCES.tail();
                   (e = (FastMap.Entry) e.getNext())!= end;) {
                 Reference reference = (Reference) e.getValue();
                 if (ctx._idToValue.containsKey(reference._id)) {
                     reference.set(ctx._idToValue.get(reference._id));
                 }
             }
        }
    }

    /**
     * Returns the persistent context instance (singleton).  
     *
     * @return the persistent context instance.
     */
    public static/*PersistentContext*/Context getCurrent() {
        return _PersistentContext;
    }

    /**
     * Returns the ID to value mapping for this persistent context.  
     *
     * @return the persistent value indexed by identifiers.
     */
    public Map/*<String, Object>*/ getIdToValue() {
        return _idToValue;
    }

    /**
     * Throws <code>UnsupportedOperationException</code> persistent context
     * are global to all threads (singleton).  
     */
    protected void enterAction() {
        throw new j2me.lang.UnsupportedOperationException(
                "Cannot enter persistent context (already in)");
    }

    /**
     * Throws <code>UnsupportedOperationException</code> persistent context
     * are global to all threads (singleton).  
     */
    protected void exitAction() {
        throw new j2me.lang.UnsupportedOperationException(
                "Cannot exit persistent context (always in)");
    }

    /**
     * <p> This class represents a reference over an object which can be kept 
     *     persistent accross multiple program executions. Instances of this class 
     *     are typically used to hold global data time consuming to regenerate. 
     *     For example:[code]
     *     public class FastMap<K,V> implements Map<K, V> {
     *         // Provides a constructor for persistent maps.
     *         public FastMap(String id) {
     *             new PersistentContext<Map<K, V>.Reference(id, this) {
     *                  protected void notifyChange() {
     *                      FastMap.this.clear();
     *                      FastMap.this.putAll(this.get());
     *                  }
     *             };
     *         }
     *     }
     *     ...
     *     // Persistent lookup table for units multiplications.
     *     static FastMap<Unit, FastMap<Unit, Unit>> UNITS_MULT_LOOKUP 
     *          =  new FastMap<Unit, FastMap<Unit, Unit>>("UNITS_MULT_LOOKUP").setShared(true);
     *    [/code]</p>
     *    
     * <p> Persistent references may also be used to hold optimum configuration 
     *     values set from previous executions. For example:[code]
     *     public Targets {  
     *          private static PersistentContext.Reference<Integer> CAPACITY 
     *               = new PersistentContext.Reference<Integer>("Targets#CAPACITY", 256);
     *          private Target[] _targets = new Target[CAPACITY.get()];
     *          private int _count;
     *          public void add(Target target) {
     *              if (_count == _targets.length) { // Ooops, resizes.
     *                  Target[] tmp = new Target[_count * 2];
     *                  System.arraycopy(_targets, 0, tmp, 0, _count);
     *                  _targets = tmp;
     *                  CAPACITY.setMinimum(_targets.length); // Persists. 
     *              }
     *              _targets[_count++] target;
     *         }
     *     }[/code]
     *
     * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
     * @version 4.0, September 4, 2006
     */
    public static class Reference /*<T>*/implements
            javolution.lang.Reference/*<T>*/ {

        /**
         * Holds the instances.
         */
        private final static FastMap INSTANCES = new FastMap();

        /**
         * Holds the unique identifier.
         */
        private final String _id;

        /**
         * Holds the current value.
         */
        private Object/*{T}*/ _value;

        /**
         * Creates a persistent reference identified by the specified name and 
         * having the specified default value.
         * 
         * @param id the unique identifier.
         * @param defaultValue the default value.
         * @throws IllegalArgumentException if the name is not unique.
         */
        public Reference(String id, Object/*{T}*/defaultValue) {
            _id = id;
            _value = defaultValue;
            synchronized (INSTANCES) {
                if (INSTANCES.containsKey(id))
                    throw new IllegalArgumentException("Identifier " + id
                            + " already in use");
                INSTANCES.put(id, this);
            }
            if (_PersistentContext._idToValue.containsKey(id)) {
                set((Object/*{T}*/) _PersistentContext._idToValue.get(id));
            }
        }

        // Implements Reference interface.
        public Object/*{T}*/get() {
            return _value;
        }

        // Implements Reference interface.
        public void set(Object/*{T}*/value) {
            _value = value;
            notifyChange();
        }

        /**
         * Sets this reference to the specified value only if 
         * <code>(value.compareTo(this.get()) &gt; 0)</code>.
         * 
         * @param value the minimum value for this reference.
         * @throws IllegalArgumentException if the specified value is not 
         *         {@link Comparable} or an {@link Integer} instance (J2ME).
         */
        public void setMinimum(Object/*{T}*/value) {
            synchronized (this) {
                if (value instanceof Comparable) {
                    Object prevValue = get();
                    if (((Comparable) value).compareTo(prevValue) > 0) {
                        set(value);
                    }
                } else if (value instanceof Integer) {
                    Object prevValue = get();
                    if (((Integer) value).intValue() > ((Integer) prevValue)
                            .intValue()) {
                        set(value);
                    }
                } else {
                    throw new IllegalArgumentException();
                }
            }
        }

        /**
         * Sets this reference to the specified value only if 
         * <code>(value.compareTo(this.get()) &lt; 0)</code>.
         * 
         * @param value the maximum value for this reference.
         * @throws IllegalArgumentException if the specified value is not 
         *         {@link Comparable} or an {@link Integer} instance (J2ME).
         */
        public void setMaximum(Object/*{T}*/value) {
            synchronized (this) {
                if (value instanceof Comparable) {
                    Object prevValue = get();
                    if (((Comparable) value).compareTo(prevValue) < 0) {
                        set(value);
                    }
                } else if (value instanceof Integer) {
                    Object prevValue = get();
                    if (((Integer) value).intValue() < ((Integer) prevValue)
                            .intValue()) {
                        set(value);
                    }
                } else {
                    throw new IllegalArgumentException();
                }
            }
        }

        /**
         * Returns the string representation of the current value of this 
         * reference.
         *
         * @return <code>String.valueOf(this.get())</code>
         */
        public String toString() {
            return String.valueOf(get());
        }

        /**
         * Notifies this reference that its value has changed (for example
         * a new persistent context has been loaded).
         * The default implementation does nothing.
         */
        protected void notifyChange() {
        }
    }
}
