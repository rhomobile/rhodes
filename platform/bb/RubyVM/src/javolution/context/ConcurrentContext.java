/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.context;

import j2mex.realtime.MemoryArea;

import javolution.lang.Configurable;
import javolution.lang.MathLib;
import javolution.lang.Reflection;

/**
 * <p> This class represents a context to take advantage of concurrent 
 *     algorithms on multi-processors systems.</p>
 *     
 * <p> When a thread enters a concurrent context, it may performs concurrent
 *     executions by calling the {@link #execute(Runnable)} static method.
 *     The logic is then executed by a concurrent thread or by the current 
 *     thread itself if there is no concurrent thread immediately available 
 *     (the number of concurrent threads is limited, see 
 *     <a href="{@docRoot}/overview-summary.html#configuration">
 *     Javolution Configuration</a> for details).</p>
 *     
 * <p> Only after all concurrent executions are completed, is the current 
 *     thread allowed to exit the scope of the concurrent context 
 *     (internal synchronization).</p>
 *     
 * <p> Concurrent logics always execute within the same {@link Context} as 
 *     the calling thread. For example, if the main thread runs in a 
 *     {@link StackContext}, concurrent executions are performed in the
 *     same {@link StackContext} as well.</p>
 *     
 * <p> Concurrent contexts are easy to use, and provide automatic 
 *     load-balancing between processors with almost no overhead. Here is
 *     an example of <b>concurrent/recursive</b> implementation of the 
 *     Karatsuba multiplication for large integers:[code]
 *     public LargeInteger multiply(LargeInteger that) {
 *         if (that._size <= 1) {
 *             return multiply(that.longValue()); // Direct multiplication.
 *         } else { // Karatsuba multiplication in O(n^log2(3))
 *             int bitLength = this.bitLength();
 *             int n = (bitLength >> 1) + (bitLength & 1);
 *                 
 *             // this = a + 2^n b,   that = c + 2^n d
 *             LargeInteger b = this.shiftRight(n);
 *             LargeInteger a = this.minus(b.shiftLeft(n));
 *             LargeInteger d = that.shiftRight(n);
 *             LargeInteger c = that.minus(d.shiftLeft(n));
 *             Multiply ac = Multiply.valueOf(a, c);
 *             Multiply bd = Multiply.valueOf(b, d);
 *             Multiply abcd = Multiply.valueOf(a.plus(b), c.plus(d));
 *             ConcurrentContext.enter();
 *             try { 
 *                 ConcurrentContext.execute(ac);
 *                 ConcurrentContext.execute(bd);
 *                 ConcurrentContext.execute(abcd);
 *             } finally {
 *                 ConcurrentContext.exit(); // Waits for all concurrent threads to complete.
 *             }
 *             // a*c + ((a+b)*(c+d)-a*c-b*d) 2^n + b*d 2^2n 
 *             return  ac.value().plus(
 *                 abcd.value().minus(ac.value().plus(bd.value())).shiftWordLeft(n)).plus(
 *                 bd.value().shiftWordLeft(n << 1));
 *         }
 *     }
 *     private static class Multiply implements Runnable {
 *         LargeInteger _left, _right, _value;
 *         static Multiply valueOf(LargeInteger left, LargeInteger right) {
 *             Multiply multiply = new Multiply(); // Or use an ObjectFactory (to allow stack allocation).
 *             multiply._left = left;
 *             multiply._right = right;
 *             return multiply;
 *         }
 *         public void run() {
 *             _value = _left.times(_right); // Recursive.
 *         }
 *         public LargeInteger value() {
 *             return _result;
 *         } 
 *     };[/code]
 *    
 *    Here is a concurrent/recursive quick/merge sort using anonymous inner 
 *    classes (the same method is used for   
 *    <a href="http://javolution.org/doc/benchmark.html">benchmark</a>):[code]
 *    private void quickSort(final FastTable<? extends Comparable> table) {
 *        final int size = table.size();
 *        if (size < 100) { 
 *            table.sort(); // Direct quick sort.
 *        } else {
 *            // Splits table in two and sort both part concurrently.
 *            final FastTable<? extends Comparable> t1 = FastTable.newInstance();
 *            final FastTable<? extends Comparable> t2 = FastTable.newInstance();
 *            ConcurrentContext.enter();
 *            try {
 *                ConcurrentContext.execute(new Runnable() {
 *                    public void run() {
 *                        t1.addAll(table.subList(0, size / 2));
 *                        quickSort(t1); // Recursive.
 *                    }
 *                });
 *                ConcurrentContext.execute(new Runnable() {
 *                    public void run() {
 *                        t2.addAll(table.subList(size / 2, size));
 *                        quickSort(t2); // Recursive.
 *                    }
 *                });
 *            } finally {
 *                ConcurrentContext.exit();
 *            }
 *            // Merges results.
 *            for (int i=0, i1=0, i2=0; i < size; i++) {
 *                if (i1 >= t1.size()) {
 *                    table.set(i, t2.get(i2++));
 *                } else if (i2 >= t2.size()) {
 *                    table.set(i, t1.get(i1++));
 *                } else {
 *                    Comparable o1 = t1.get(i1);
 *                    Comparable o2 = t2.get(i2);
 *                    if (o1.compareTo(o2) < 0) {
 *                        table.set(i, o1);
 *                        i1++;
 *                    } else {
 *                        table.set(i, o2);
 *                        i2++;
 *                    }
 *                }
 *            }
 *            FastTable.recycle(t1);  
 *            FastTable.recycle(t2);
 *        }
 *     }[/code]
 * 
 * <p> Concurrent contexts ensure the same behavior whether or not the execution
 *     is performed by the current thread or a concurrent thread. Any exception 
 *     raised during the concurrent logic executions is propagated to the 
 *     current thread.</p>
 *     
 * <p> {@link #getConcurrency() Concurrency} can be {@link LocalContext locally}
 *     adjusted. For example:[code]
 *         LocalContext.enter(); 
 *         try { // Do not use more than half of the processors during analysis.
 *             ConcurrentContext.setConcurrency((Runtime.getRuntime().availableProcessors() / 2) - 1);
 *             runAnalysis(); // Use concurrent contexts internally. 
 *         } finally {
 *             LocalContext.exit();    
 *         }[/code] </p>
 *     It should be noted that the concurrency cannot be increased above the  
 *     configurable {@link #MAXIMUM_CONCURRENCY maximum concurrency}. 
 *     In other words, if the maximum concurrency is <code>0</code>, 
 *     concurrency is disabled regardless of local concurrency settings.</p>   
 * 
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.1, July 2, 2007
 */
public abstract class ConcurrentContext extends Context {

    /**
     * Holds the default implementation. Concurrent executions are performed 
     * in the same memory area and at the same priority as the calling thread.
     * This implementation uses <code>javax.realtime.RealtimeThread</code>
     * for concurrent threads. Alternative (RTSJ) implementations could also use 
     * <code>javax.realtime.NoHeapRealtimeThread</code>. 
     */
    public static final Configurable/*<Class<? extends ConcurrentContext>>*/
        DEFAULT = new Configurable(Default.CLASS);

    /**
     * Holds the maximum number of concurrent executors
     * (see <a href="{@docRoot}/overview-summary.html#configuration">
     * Javolution Configuration</a> for details). 
     */
    public static final Configurable/*<Integer>*/MAXIMUM_CONCURRENCY = new Configurable(
            new Integer(availableProcessors() - 1)) {
        protected void notifyChange() { // The maximum concurrency is also the default concurrency. 
            CONCURRENCY.setDefault(this.get());
        }
    };

    private static int availableProcessors() {
        Reflection.Method availableProcessors = Reflection
                .getMethod("java.lang.Runtime.availableProcessors()");
        if (availableProcessors != null) {
            Integer processors = (Integer) availableProcessors.invoke(Runtime
                    .getRuntime());
            return processors.intValue();
        } else { // J2ME.
            return 1;
        }
    }

    /**
     * Holds the current concurrency. 
     */
    private static final LocalContext.Reference CONCURRENCY = new LocalContext.Reference(
            MAXIMUM_CONCURRENCY.get());

    /**
     * Default constructor.
     */
    protected ConcurrentContext() {
    }

    /**
     * Enters a concurrent context (instance of {@link #DEFAULT}). 
     * 
     * @return the concurrent context being entered.
     */
    public static ConcurrentContext enter() {
        return (ConcurrentContext) Context.enter((Class) DEFAULT.get());
    }

    /**
     * Exits the current concurrent context.
     * 
     * @return the concurrent context being exited.
     * @throws ClassCastException if the context is not a concurrent context.
     */
    public static /*ConcurrentContext*/Context exit() {
        return (ConcurrentContext) Context.exit();
    }

    /**
     * Set the {@link LocalContext local} concurrency. Concurrency is 
     * hard limited by {@link #MAXIMUM_CONCURRENCY}.
     * 
     * @param concurrency the new concurrency (<code>0</code> or negative
     *       number to disable concurrency).
     */
    public static void setConcurrency(int concurrency) {
        concurrency = MathLib.max(0, concurrency);
        concurrency = MathLib.min(((Integer) MAXIMUM_CONCURRENCY.get())
                .intValue(), concurrency);
        CONCURRENCY.set(new Integer(concurrency));
    }

    /**
     * Returns the {@link LocalContext local} concurrency.
     * 
     * @return the maximum number of concurrent thread.
     */
    public static int getConcurrency() {
        return ((Integer) CONCURRENCY.get()).intValue();
    }

    /**
     * Executes the specified logic by a concurrent thread if 
     * one available; otherwise the logic is executed by the current thread.
     * Any exception or error occuring during concurrent executions is
     * propagated to the current thread upon {@link #exit} 
     * of the concurrent context.
     * 
     * @param  logic the logic to execute concurrently if possible.
     * @throws ClassCastException if the current context is not a
     *         {@link ConcurrentContext}.
     */
    public static void execute(Runnable logic) {
        ConcurrentContext ctx = (ConcurrentContext) Context.getCurrent();
        ctx.executeAction(logic);
    }

    /**
     * Executes the specified logic concurrently if possible. 
     * 
     * @param  logic the logic to execute.
     */
    protected abstract void executeAction(Runnable logic);

    /**
     * Default implementation using {@link ConcurrentThread} executors.
     */
    static final class Default extends ConcurrentContext {

        private static final Class CLASS = new Default().getClass();
        
        /**
         * Holds the concurrent executors.
         */
        private static ConcurrentThread[] _Executors = new ConcurrentThread[0];

        /**
         * Holds the executors creation logic (to be performed in 
         * ImmortalMemory). The number of executors can only be increased
         * (typically through configuration).
         */
        private static final Runnable CREATE_EXECUTORS = new Runnable() {
            public synchronized void run() {
                int max = ((Integer) MAXIMUM_CONCURRENCY.get()).intValue();
                int count = _Executors.length;
                if (count >= max) 
                    return; // We have enough executors.
                ConcurrentThread[] executors = new ConcurrentThread[max];
                System.arraycopy(_Executors, 0, executors, 0, count);
                for (int i = count; i < max; i++) {
                    executors[i] = new ConcurrentThread();
                    executors[i].start();
                }
                _Executors = executors;
            }
        };

        /**
         * Holds the concurrency.
         */
        private int _concurrency;

        /**
         * Holds any error occuring during concurrent execution.
         */
        private Throwable _error;

        /**
         * Holds the number of concurrent execution initiated.
         */
        private int _initiated;

        /**
         * Holds the number of concurrent execution completed.
         */
        private int _completed;


        // Implements Context abstract method.
        protected void enterAction() {
            _error = null;
            _initiated = 0;
            _completed = 0;
            _concurrency = ConcurrentContext.getConcurrency();
            if (_concurrency > _Executors.length) { // We need more executors.
                MemoryArea.getMemoryArea(_Executors).executeInArea(CREATE_EXECUTORS);
            }            
        }

        // Implements ConcurrentContext abstract method.
        protected void executeAction(Runnable logic) {
            for (int i = _concurrency; --i >= 0;) {
                if (_Executors[i].execute(logic, this)) {
                    _initiated++;
                    return; // Done concurrently.
                }
            }
            // Execution by current thread.
            logic.run();
        }

        // Implements Context abstract method.
        protected void exitAction() {
            synchronized (this) {
                while (_initiated != _completed) {
                    try {
                        this.wait();
                    } catch (InterruptedException e) {
                        throw new ConcurrentException(e);
                    }
                }
            }
            // Propagates any concurrent error to current thread.
            if (_error != null) {
                if (_error instanceof RuntimeException)
                    throw ((RuntimeException) _error);
                if (_error instanceof Error)
                    throw ((Error) _error);
                throw new ConcurrentException(_error); // Wrapper.
            }

        }

        // Called when a concurrent execution starts.
        void started() {
            Context.setCurrent(this);
        }

        // Called when a concurrent execution finishes. 
        void completed() {
            synchronized (this) {
                _completed++;
                this.notify();
            }
            ((AllocatorContext) AllocatorContext.getCurrent()).deactivate();        
        }

        // Called when an error occurs.
        void error(Throwable error) {
            synchronized (this) {
                if (_error == null) { // First error.
                    _error = error;
                }
            }
        }
    }

    // Allows instances of private classes to be factory produced. 
    static {
        ObjectFactory.setInstance(new ObjectFactory() {
            protected Object create() {
                return new Default();
            } }, Default.CLASS);
    }

}