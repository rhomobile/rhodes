package org.garret.perst;

/**
 * Listener for change is database files.
 * This interface can be used in Storage.propagateChanges to collect 
 * all changes since the observation period.
 */
public interface ChangeListener { 
    /**
     * Propagate changed database page
     * @param position position of page in database file
     * @param data page data
     */
    void propagate(long position, byte[] data);
}