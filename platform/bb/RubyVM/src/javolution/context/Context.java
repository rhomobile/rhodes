/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2007 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.context;

//import javolution.xml.XMLSerializable;
import j2me.lang.IllegalStateException;
import j2me.lang.UnsupportedOperationException;
import j2me.lang.ThreadLocal;

/**
 * <p> This class represents an execution context; they can be associated to 
 *     particular threads or objects.</p>
 *     
 * <p> Context-aware applications may extend the context base class or any 
 *     predefined contexts in order to facilitate <a 
 *     href="package-summary.html#package_description">
 *     separation of concerns</a>.</p>
 *     
 * <p> The scope of a {@link Context} should be surrounded by a <code>try, 
 *     finally</code> block statement to ensure correct behavior in case 
 *     of exceptions being raised. For example:[code]
 *     LocalContext.enter(); // Current thread enter a local context.
 *     try 
 *         ModuloInteger.setModulus(m); // No impact on other threads!
 *         z = x.times(y); // Multiplication modulo m.
 *     } finally {
 *         LocalContext.exit();
 *     }[/code]</p>
 *      
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.0, April 15, 2007
 */
public abstract class Context { //implements XMLSerializable {

    /**
     * Holds the root context (top context of all threads).
     */
    public static final Context ROOT = new Root();
    
    /**
     * Holds the current context (thread-local).
     */
    private static final ThreadLocal CURRENT = new ThreadLocal() {
        protected Object initialValue() {
            return ROOT;
        }
    };
   
    /**
     * Holds the current owner of this context or <code>null</code> if global 
     * context.
     */
    private Thread _owner;

    /**
     * Holds the outer context or <code>null</code> if none (root context).
     */
    private Context _outer;

    /**
     * Holds the factory having produced this context if any (for recycling 
     * purpose upon exit).
     */
    private ObjectFactory _factory; 

    /**
     * Holds the inherited allocator context or <code>null</code>
     */
    private AllocatorContext _allocator; 

    /**
     * Default constructor. 
     */
    protected Context() {    
    }

    /**
     * Returns the current context for the current thread. 
     *
     * @return the current context.
     */
    public static Context getCurrent() {
       return (Context) Context.CURRENT.get();
    }

    /**
     * Returns the current owner of this context. The owner of a
     * context is the thread which {@link #enter(Context) entered}
     * the context and has not yet {@link #exit(Context) exited}.
     * A context can only have one owner at any given time, although
     * contexts can be shared by {@link ConcurrentContext concurrent} threads.
     *
     * @return the thread owner of this context or <code>null</code>.
     */
    public final Thread getOwner() {
        return _owner;
    }

    /**
     * Returns the outer context of this context or <code>null</code>
     * if {@link #ROOT}.
     *
     * @return the outer context or <code>null</code>.
     */
    public final Context getOuter() {
        return _outer;
    }

    /**
     * Returns the string representation of this context (default 
     * <code>"Instance of " + this.getClass().getName()</code>).
     * 
     * @return the string representation of this context.
     */
    public String toString() {
        return "Instance of " + this.getClass().getName();
    }
    
    /**
     * The action to be performed after this context becomes the current 
     * context.
     */
    protected abstract void enterAction();

    /**
     * The action to be performed before this context is no more the current 
     * context.
     */
    protected abstract void exitAction();

    /**
     * Enters the specified context.
     *
     * @param context the context being entered.
     * @return the specified context.
     * @throws IllegalStateException if this context is currently in use.
     */
    public static final /*<T extends Context>*/ /*T*/Context enter(/*T*/Context context) {
        if (context._owner != null)
            throw new IllegalStateException("Context is currently in use");
        Context current = Context.getCurrent();
        context._outer = current;
        context._owner = Thread.currentThread();
        context._allocator = context instanceof AllocatorContext ? (AllocatorContext) context : current._allocator;
        Context.CURRENT.set(context);
        context.enterAction();
        return (/*T*/Context) context;
    }

    /**
     * Enters a factory produced context of specified type to be recycled 
     * after {@link #exit exiting}.
     * This method is called by the static <code>enter()</code> method 
     * of specialized contexts. If the context class has no public 
     * no-arg constructor accessible, then the factory for the class should 
     * be {@link ObjectFactory#setInstance explicitely set} (typically 
     * in a static initializer).  
     *
     * @param contextType the type of context being entered.
     * @return the context being entered.
     * @see ObjectFactory#getInstance(Class)
     */
    public static final /*<T extends Context>*/ /*T*/Context enter(Class/*<T>*/ contextType) {
        ObjectFactory factory = ObjectFactory.getInstance(contextType);
        Context context = (Context) factory.object();
        context._factory = factory;
        Context.enter(context);
        return (/*T*/Context) context;
    }
  
    /**
     * Exits the current context (the {@link #getOuter outer} context
     * becomes the current context).
     * 
     * @return the context which has been exited.
     * @throws IllegalStateException if this context is the {@link #ROOT} 
     *         context.
     */
    public static Context exit() {
        Context context = Context.getCurrent();
        Context outer = context._outer;
        if (outer == null) 
            throw new IllegalStateException(Thread.currentThread() + " Cannot exit instance of " 
            		+ context.getClass());
        try {
            context.exitAction();
        } finally {
            Context.CURRENT.set(outer);
            context._outer = null;
            context._owner = null;
            context._allocator = null;
            if (context._factory != null) { // Factory produced.
                context._factory.recycle(context);
                context._factory = null;
            }
        }
        return context;
    }

    /**
     * Sets the current context, used by {@link ConcurrentContext}
     * exclusively.
     * 
     * @param context the concurrent context.
     */
    protected static void setCurrent(ConcurrentContext context) {
        Context.CURRENT.set(context);
    }

    /**
     * Returns the allocator context used while in this context (shortcut).
     * 
     * @return the allocator context for this context.
     */
    final AllocatorContext getAllocatorContext() {
    	return (_allocator == null) ? AllocatorContext.getDefault() : _allocator;
    }

    // Holds the root context definition.
    private static final class Root extends Context {

        protected void enterAction() {
            throw new UnsupportedOperationException(
                    "Cannot enter the root context");
        }

        protected void exitAction() {
            throw new UnsupportedOperationException(
                "Cannot enter the root context");
        } 
    
    };
    
    /**
     * @deprecated {@link #exit()} should be used.
     */
    public static final void exit(Context ctx) {
        Context.exit();
    }
    
}