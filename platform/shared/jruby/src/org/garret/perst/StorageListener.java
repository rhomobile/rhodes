package org.garret.perst;

/**
 * Listener of database events. Programmer should derive his own subclass and register
 * it using Storage.setListener method.
 */
public abstract class StorageListener {
    /**
     * This metod is called during database open when database was not
     * close normally and has to be recovered
     */
    public void databaseCorrupted() {}

    /**
     * This method is called after completion of recovery
     */
    public void recoveryCompleted() {}
}
     
