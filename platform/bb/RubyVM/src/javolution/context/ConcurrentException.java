/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.context;

/**
 * This class encapsulates errors or exceptions raised during the execution
 * of concurrent threads ({@link ConcurrentException} are raised upon exit of
 * the {@link ConcurrentContext}). 
 * {@link ConcurrentException#getCause} identifies the source of the error.
 *
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 1.0, October 4, 2004
 * @see     ConcurrentContext#exit
 */
public class ConcurrentException extends RuntimeException {

    /**
     * Holds the cause.
     */
    private Throwable _cause;

    /**
     * Constructs a {@link ConcurrentException} caused by the specified
     * error or exception.
     *
     * @param cause the cause of the error or exception.
     */
    ConcurrentException(Throwable cause) {
        _cause = cause;
    }

    /**
     * Returns the original cause of the exception or error.
     *
     * @return the exception or error as it has been raised by the
     *         <code>j2me.lang.Runnable</code>.
     */
    public Throwable getCause() {
        return _cause;
    }

    private static final long serialVersionUID = 1L;
}