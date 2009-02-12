package org.garret.perst;

/**
 * Thrown to indicate that the requested operation is not supported.<p>
 *
 * This class is a member of the 
 * <a href="{@docRoot}/../guide/collections/index.html">
 * Java Collections Framework</a>.
 */
public class UnsupportedOperationException extends RuntimeException {
    /**
     * Constructs an UnsupportedOperationException with no detail message.
     */
    public UnsupportedOperationException() {
    }

    /**
     * Constructs an UnsupportedOperationException with the specified
     * detail message.
     *
     * @param message the detail message
     */
    public UnsupportedOperationException(String message) {
	super(message);
    }
}
