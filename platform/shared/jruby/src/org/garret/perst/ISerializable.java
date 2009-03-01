package org.garret.perst;

/**
 * Interface which should be implemented by any object which can be serialized.
 * Implementation of <code>writeObject</code> <code>readObject</code> methods should 
 * stpre/fetch content to of the object to the provided stream.
 * Order of storing/fetching fields should be the same in both methods.
 */
public interface ISerializable { 
    /**
     * Store object fields in the stream.
     * @param out stream to which object is written
     */
    void writeObject(IOutputStream out);    
    /**
     * Fetch object fields from the stream
     * @param in stream from which object data should be fetched
     */
    void readObject(IInputStream in);

    /**
     * Check if objects can be pinned in cache in survive transaction commit.
     * Commit performs cleanup of strong object cache, leaving only limited number
     * of most frequently used objects (pin objects). 
     * it is not possible to pin object which contains references to other persistent objects,
     * since such reference can point to the object which thrown away from the cache.
     * @return if object can be pinned in memory (object contains no referenes)
     */
    boolean isPinnable();
}