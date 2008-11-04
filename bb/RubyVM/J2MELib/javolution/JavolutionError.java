/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution;

/**
 *  Signals that a serious problem (bug ?!) has been detected 
 *  within the library.
 *  
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 1.0, September 14, 2004
 */
public final class JavolutionError extends Error {

    /**
     * Creates an error message with the specified message 
     * and cause.
     * 
     * @param  message the detail message.
     */
    public JavolutionError(String message) {
        super(message);
    }

    /**
     * Creates an error message with the specified message 
     * and cause. The cause stack trace is printed to the
     * current error stream (System.err).
     * 
     * @param  message the detailed message.
     * @param  cause the cause of this error.
     */
    public JavolutionError(String message, Throwable cause) {
        super(message);
        cause.printStackTrace();
    }

    /**
     * Creates an error message with the specified cause 
     * The cause stack trace is printed to the current error
     * stream (System.err).
     * 
     * @param  cause the cause of this error.
     */
    public JavolutionError(Throwable cause) {
        cause.printStackTrace();
    }

    private static final long serialVersionUID = 1L;
}