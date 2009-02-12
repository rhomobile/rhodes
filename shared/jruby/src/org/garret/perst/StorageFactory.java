package org.garret.perst;
import org.garret.perst.impl.*;

/**
 * Storage factory
 */
public class StorageFactory {
    /**
     * Create new instance of the storage
     * @return new instance of the storage (unopened, you should explicitely invoke open method)
     */
    public Storage createStorage() {
        return new StorageImpl();
    }

    /**
     * Get instance of storage factory.
     * So new storages should be create in application in the following way:
     * <code>StorageFactory.getInstance().createStorage()</code>
     * @return instance of the storage factory
     */
    public static StorageFactory getInstance() { 
        return instance;
    }

    protected static final StorageFactory instance = new StorageFactory();
};
