package org.garret.perst;

/**
 * Interface of object supporting locking
 */
public interface IResource {
   /**
    * Lock persistent object in shared mode. Other threads will be able to set their
    * shared locks on this objects, but not exclusive lock can be set until this lock is released.<BR>
    * Upgrading of the lock is not possible (thread having read lock can not upgrade it to exclusive lock).
    * It is done to prevent possible deadlocks caused by lock updates. 
    * But locks are reentrant - so thread can request the same lock many times (and correspondent 
    * number of unlocks is needed to release the lock).<BR> 
    * Locking the object doesn't prevent other threads from accessing the object - 
    * it only has influence on <code>sharedLock</code> and <code>exclusiveLock</code> methods.
    * So programmer should set proper lock before accessing the object in multithreaded application.<BR> 
    * If object is concurrently accessed by several threads in read-only mode, then explicit locking
    * of this object is not needed, because language API provides consistent retrieving of objects itself.<BR> 
    * Only persistent object (object which were assigned to the the storage either implicitly by
    * saving some other persistent object referencing this object, either explicitly by 
    * <code>Storage.makeObjectPersistent</code> method.<BR>
    */
    public void    sharedLock();
  
   /**
    * Lock persistent object in shared mode. Other threads will be able to set their
    * shared locks on this objects, but not exclusive lock can be set until this lock is released.<BR>
    * Upgrading of the lock is not possible (thread having read lock can not upgrade it to exclusive lock).
    * It is done to prevent possible deadlocks caused by lock updates. 
    * But locks are reentrant - so thread can request the same lock many times (and correspondent 
    * number of unlocks is needed to release the lock).<BR> 
    * Locking the object doesn't prevent other threads from accessing the object - 
    * it only has influence on <code>sharedLock</code> and <code>exclusiveLock</code> methods.
    * So programmer should set proper lock before accessing the object in multithreaded application.<BR> 
    * If object is concurrently accessed by several threads in read-only mode, then explicit locking
    * of this object is not needed, because language API provides consistent retrieving of objects itself.<BR> 
    * Only persistent object (object which were assigned to the the storage either implicitly by
    * saving some other persistent object referencing this object, either explicitly by 
    * <code>Storage.makeObjectPersistent</code> method.<BR>
    * @param timeout timeout of operation in milliseconds. If timeout is 0 and
    * lock can not be granted, the request will fail immediately. Otherwise the system will try
    * to grant lock within specified amount of time.
    * @return 
    * <DL>
    * <DT><code>true</code> if lock is successfully granted<br>
    * <DT><code>false</code> if lock can not be granted within specified time 
    * </DL>
    */
    public boolean sharedLock(long timeout);

   /**
    * Lock persistent object in exclusive mode. Only one thread can lock object in exclusive mode at each
    * moment of time. Shared or exclusive lock requests of other threads will be blocked until
    * this lock is released.
    * shared locks on this objects, but not exclusive lock can be set until this lock is released.<BR>
    * This lock is reentrant, so thread owning the lock can successfully retrieve the lock many times
    * (and correspondent number of unlocks is needed to release the lock).<BR> 
    * Locking the object doesn't prevent other threads from accessing the object - 
    * it only has influence on <code>sharedLock</code> and <code>exclusiveLock</code> methods.
    *So programmer should set proper lock before accessing the object in multithreaded application.<BR> 
    * Only persistent object (object which were assigned to the the storage either implicitly by
    * saving some other persistent object referencing this object, either explicitly by 
    * <code>Storage.makeObjectPersistent</code> method.   
    */
    public void    exclusiveLock();

   /**
    * Lock persistent object in exclusive mode. Only one thread can lock object in exclusive mode at each
    * moment of time. Shared or exclusive lock requests of other threads will be blocked until
    * this lock is released.
    * shared locks on this objects, but not exclusive lock can be set until this lock is released.<BR>
    * This lock is reentrant, so thread owning the lock can successfully retrieve the lock many times
    * (and correspondent number of unlocks is needed to release the lock).<BR> 
    * Locking the object doesn't prevent other threads from accessing the object - 
    * it only has influence on <code>sharedLock</code> and <code>exclusiveLock</code> methods.
    *So programmer should set proper lock before accessing the object in multithreaded application.<BR> 
    * Only persistent object (object which were assigned to the the storage either implicitly by
    * saving some other persistent object referencing this object, either explicitly by 
    * <code>Storage.makeObjectPersistent</code> method.   
    * @param timeout timeout of operation in milliseconds. If timeout is 0 and
    * lock can not be granted, the request will fail immediately. Otherwise the system will try
    * to grant lock within specified amount of time.
    * @return 
    * <DL>
    * <DT><code>true</code> if lock is successfully granted<br>
    * <DT><code>false</code> if lock can not be granted within specified time 
    * </DL>
    */
    public boolean exclusiveLock(long timeout);

   /**
    * Remove granted lock. If lock was requested several times by one thread, then correspondent number
    * of unlocks is needed to release the lock.
    */
    public void    unlock();

    /**
     * Reset resource to original state.
     * Wakeup all threads waiting for this resource.
     */
    public void    reset();
}
