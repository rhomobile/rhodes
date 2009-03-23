package org.garret.perst;

import java.util.*;
import org.garret.perst.impl.ThreadTransactionContext;

/**
 * Object storage
 */
public interface Storage { 
    /**
     * Constant specifying that page pool should be dynamically extended 
     * to conatins all database file pages
     */
    public static final int INFINITE_PAGE_POOL = 0;
    /**
     * Constant specifying default pool size
     */
    public static final int DEFAULT_PAGE_POOL_SIZE = 64*1024;

    /**
     * Open the storage
     * @param filePath path to the database file
     * @param pagePoolSize size of page pool (in bytes). Page pool should contain
     * at least ten 4kb pages, so minimal page pool size should be at least 40Kb.
     * But larger page pool usually leads to better performance (unless it could not fit
     * in memory and cause swapping). Value 0 of this paremeter corresponds to infinite
     * page pool (all pages are cashed in memory). It is especially useful for in-memory
     * database, when storage is created with NullFile.
     * 
     */
    public void open(String filePath, int pagePoolSize);

    /**
     * Open the storage
     * @param file user specific implementation of IFile interface
     * @param pagePoolSize size of page pool (in bytes). Page pool should contain
     * at least ten 4kb pages, so minimal page pool size should be at least 40Kb.
     * But larger page pool ussually leads to better performance (unless it could not fit
     * in memory and cause swapping).
     */
    public void open(IFile file, int pagePoolSize);

    /**
     * Open the storage with default page pool size
     * @param file user specific implementation of IFile interface
     */ 
    public void open(IFile file);

    /**
     * Open the storage with default page pool size
     * @param filePath path to the database file
     */ 
    public void open(String filePath);

    /**
     * Open the encrypted storage
     * @param filePath path to the database file
     * @param pagePoolSize size of page pool (in bytes). Page pool should contain
     * at least then 4kb pages, so minimal page pool size should be at least 40Kb.
     * But larger page pool usually leads to better performance (unless it could not fit
     * in memory and cause swapping).
     * @param cipherKey cipher key
     */
    public void open(String filePath, int pagePoolSize, String cipherKey);

    /**
     * Check if database is opened
     * @return <code>true</code> if database was opened by <code>open</code> method, 
     * <code>false</code> otherwise
     */
    public boolean isOpened();
    
    /**
     * Get storage root. Storage can have exactly one root object. 
     * If you need to have several root object and access them by name (as is is possible 
     * in many other OODBMSes), you should create index and use it as root object.
     * @return root object or <code>null</code> if root is not specified (storage is not yet initialized)
     */
    public IPersistent getRoot();
    
    /**
     * Set new storage root object.
     * Previous reference to the root object is rewritten but old root is not automatically deallocated.
     * @param root object to become new storage root. If it is not persistent yet, it is made
     * persistent and stored in the storage
     */
    public void setRoot(IPersistent root);

    

    /**
     * Commit changes done by the last transaction. Transaction is started implcitlely with forst update
     * opertation.
     */
    public void commit();

    /**
     * Rollback changes made by the last transaction
     */
    public void rollback();


    /**
     * Backup current state of database
     * @param out output stream to which backup is done
     */
    public void backup(java.io.OutputStream out) throws java.io.IOException;

    /**
     * Exclusive per-thread transaction: each thread access database in exclusive mode
     */
    public static final int EXCLUSIVE_TRANSACTION = 0;
    /**
     * Alias for EXCLUSIVE_TRANSACTION. In case of multiclient access, 
     * and transaction modifying database should be exclusive.
     */
    public static final int READ_WRITE_TRANSACTION = EXCLUSIVE_TRANSACTION;
    /**
     * Cooperative mode; all threads share the same transaction. Commit will commit changes made
     * by all threads. To make this schema work correctly, it is necessary to ensure (using locking)
     * that no thread is performing update of the database while another one tries to perform commit.
     * Also please notice that rollback will undo the work of all threads. 
     */
    public static final int COOPERATIVE_TRANSACTION = 1;
    /**
     * Alias for COOPERATIVE_TRANSACTION. In case of multiclient access, 
     * only read-only transactions can be executed in parallel.
     */
    public static final int READ_ONLY_TRANSACTION = COOPERATIVE_TRANSACTION;
    /**
     * Serializable per-thread transaction. Unlike exclusive mode, threads can concurrently access database, 
     * but effect will be the same as them work exclusively.
     * To provide such behavior, programmer should lock all access objects (or use hierarchical locking).
     * When object is updated, exclusive lock should be set, otherwise shared lock is enough.
     * Lock should be preserved until the end of transaction.
     */
    public static final int SERIALIZABLE_TRANSACTION = 2;

    /** 
     * Begin per-thread transaction. Three types of per-thread transactions are supported: 
     * exclusive, cooperative and serializable. In case of exclusive transaction, only one 
     * thread can update the database. In cooperative mode, multiple transaction can work 
     * concurrently and commit() method will be invoked only when transactions of all threads
     * are terminated. Serializable transactions can also work concurrently. But unlike
     * cooperative transaction, the threads are isolated from each other. Each thread
     * has its own associated set of modified objects and committing the transaction will cause
     * saving only of these objects to the database. To synchronize access to the objects
     * in case of serializable transaction programmer should use lock methods
     * of IResource interface. Shared lock should be set before read access to any object, 
     * and exclusive lock - before write access. Locks will be automatically released when
     * transaction is committed (so programmer should not explicitly invoke unlock method)
     * In this case it is guaranteed that transactions are serializable.<br>
     * It is not possible to use <code>IPersistent.store()</code> method in
     * serializable transactions. That is why it is also not possible to use Index and FieldIndex
     * containers (since them are based on B-Tree and B-Tree directly access database pages
     * and use <code>store()</code> method to assign OID to inserted object. 
     * You should use <code>SortedCollection</code> based on T-Tree instead or alternative
     * B-Tree implemenataion (set "perst.alternative.btree" property).
     * @param mode <code>EXCLUSIVE_TRANSACTION</code>, <code>COOPERATIVE_TRANSACTION</code>, 
     * or <code>SERIALIZABLE_TRANSACTION</code> 
     */
    public void beginThreadTransaction(int mode);
    
    /**
     * End per-thread transaction started by beginThreadTransaction method.<br>
     * If transaction is <i>exclusive</i>, this method commits the transaction and
     * allows other thread to proceed.<br>
     * If transaction is <i>serializable</i>, this method commits sll changes done by this thread
     * and release all locks set by this thread.<br>     
     * If transaction is <i>cooperative</i>, this method decrement counter of cooperative
     * transactions and if it becomes zero - commit the work
     */
    public void endThreadTransaction();

    /**
     * End per-thread cooperative transaction with specified maximal delay of transaction
     * commit. When cooperative transaction is ended, data is not immediately committed to the
     * disk (because other cooperative transaction can be active at this moment of time).
     * Instead of it cooperative transaction counter is decremented. Commit is performed
     * only when this counter reaches zero value. But in case of heavy load there can be a lot of
     * requests and so a lot of active cooperative transactions. So transaction counter never reaches zero value.
     * If system crash happens a large amount of work will be lost in this case. 
     * To prevent such scenario, it is possible to specify maximal delay of pending transaction commit.
     * In this case when such timeout is expired, new cooperative transaction will be blocked until
     * transaction is committed.
     * @param maxDelay maximal delay in milliseconds of committing transaction.  Please notice, that Perst could 
     * not force other threads to commit their cooperative transactions when this timeout is expired. It will only
     * block new cooperative transactions to make it possible to current transaction to complete their work.
     * If <code>maxDelay</code> is 0, current thread will be blocked until all other cooperative trasnaction are also finished
     * and changhes will be committed to the database.
     */
    public void endThreadTransaction(int maxDelay);
   
    /**
     * Rollback per-thread transaction. It is safe to use this method only for exclusive transactions.
     * In case of cooperative transactions, this method rollback results of all transactions.
     */
    public void rollbackThreadTransaction();

    /**
     * Create new peristent set
     * @return persistent object implementing set
     */
    public IPersistentSet createSet();

    /**
     * Create new peristent list. Implementation of this list is based on B-Tree so it can efficiently
     * handle large number of objects but in case of very small list memory overhead is too high.
     * @return persistent object implementing list
     */
    public IPersistentList createList();

    /**
     * Create new scalable set references to persistent objects.
     * This container can effciently store small number of references as well as very large
     * number references. When number of memers is small, Link class is used to store 
     * set members. When number of members exceed some threshold, PersistentSet (based on B-Tree)
     * is used instead.
     * @return scalable set implementation
     */
    public IPersistentSet createScalableSet();

    /**
     * Create new scalable set references to persistent objects.
     * This container can effciently store small number of references as well as very large
     * number references. When number of memers is small, Link class is used to store 
     * set members. When number of members exceed some threshold, PersistentSet (based on B-Tree)
     * is used instead.
     * @param initialSize initial size of the set
     * @return scalable set implementation
     */
    public IPersistentSet createScalableSet(int initialSize);

    /**
     * Create new index
     * @param type type of the index key (org.garret.perst.Types enumeration)
     * @param unique whether index is unique (duplicate value of keys are not allowed)
     * @return persistent object implementing index
     * @exception StorageError(StorageError.UNSUPPORTED_INDEX_TYPE) exception if 
     * specified key type is not supported by implementation.
     */
    public Index createIndex(int type, boolean unique);

    /**
     * Create new compound index
     * @param types types of the index compound key components  (org.garret.perst.Types enumeration)
     * @param unique whether index is unique (duplicate value of keys are not allowed)
     * @return persistent object implementing compound index
     * @exception StorageError(StorageError.UNSUPPORTED_INDEX_TYPE) exception if 
     * specified key type is not supported by implementation.
     */
    public Index createIndex(int[] types, boolean unique);

    /**
     * Create new multidimensional index
     * @param comparator multidimensinal comparator
     * @return multidimensional index
     */
    public MultidimensionalIndex createMultidimensionalIndex(MultidimensionalComparator comparator);

    /**
     * Create new thick index (index with large number of duplicated keys)
     * @param type type of the index key (org.garret.perst.Types enumeration)
     * @return persistent object implementing index
     * @exception StorageError(StorageError.UNSUPPORTED_INDEX_TYPE) exception if 
     * specified key type is not supported by implementation.
     */
    public Index createThickIndex(int type);

    /**
     * Create new bit index. Bit index is used to select object 
     * with specified set of (boolean) properties.
     * @return persistent object implementing bit index
     */
    public BitIndex createBitIndex();

    /**
     * Create new index optimized for access by element position.
     * @param type type of the index key (org.garret.perst.Types enumeration)
     * @param unique whether index is unique (duplicate value of keys are not allowed)
     * @return persistent object implementing index
     * @exception StorageError(StorageError.UNSUPPORTED_INDEX_TYPE) exception if 
     * specified key type is not supported by implementation.
     */
    public Index createRandomAccessIndex(int type, boolean unique);

    /**
     * Create new spatial index with integer coordinates
     * @return persistent object implementing spatial index
     */
    public SpatialIndex createSpatialIndex();

    /**
     * Create new R2 spatial index 
     * @return persistent object implementing spatial index
     */
    public SpatialIndexR2 createSpatialIndexR2();

    /**
     * Create new sorted collection with specified comparator
     * @param comparator comparator class specifying order in the collection
     * @param unique whether index is collection (members with the same key value are not allowed)
     * @return persistent object implementing sorted collection
     */
    public SortedCollection createSortedCollection(PersistentComparator comparator, boolean unique);

    /**
     * Create new sorted collection. Members of this collections should implement 
     * <code>java.lang.Comparable</code> interface and make it possible to compare collection members
     * with each other as well as with serch key.
     * @param unique whether index is collection (members with the same key value are not allowed)
     * @return persistent object implementing sorted collection
     */
    public SortedCollection createSortedCollection(boolean unique);

    /**
     * Create one-to-many link.
     * @return new empty link, new members can be added to the link later.
     */
    public Link createLink();
    
    /**
     * Create one-to-many link with specified initially allocated size.
     * @param initialSize initial size of array
     * @return new empty link, new members can be added to the link later.
     */
    public Link createLink(int initialSize);
    
    /**
     * Create relation object. Unlike link which represent embedded relation and stored
     * inside owner object, this Relation object is standalone persisitent object
     * containing references to owner and members of the relation
     * @param owner owner of the relation
     * @return object representing empty relation (relation with specified owner and no members), 
     * new members can be added to the link later.
     */
    public Relation createRelation(IPersistent owner);


    /**
     * Create new BLOB. Create object for storing large binary data.
     * @return empty BLOB
     */
    public Blob createBlob();

    /**
     * Create new time series object. 
     * @param blockClass class derived from TimeSeries.Block
     * @param maxBlockTimeInterval maximal difference in milliseconds between timestamps 
     * of the first and the last elements in a block. 
     * If value of this parameter is too small, then most blocks will contains less elements 
     * than preallocated. 
     * If it is too large, then searching of block will be inefficient, because index search 
     * will select a lot of extra blocks which do not contain any element from the 
     * specified range.
     * Usually the value of this parameter should be set as
     * (number of elements in block)*(tick interval)*2. 
     * Coefficient 2 here is used to compencate possible holes in time series.
     * For example, if we collect stocks data, we will have data only for working hours.
     * If number of element in block is 100, time series period is 1 day, then
     * value of maxBlockTimeInterval can be set as 100*(24*60*60*1000)*2
     * @return new empty time series
     */
    public TimeSeries createTimeSeries(String blockClass, long maxBlockTimeInterval);


    /**
     * Create PATRICIA trie (Practical Algorithm To Retrieve Information Coded In Alphanumeric)
     * Tries are a kind of tree where each node holds a common part of one or more keys. 
     * PATRICIA trie is one of the many existing variants of the trie, which adds path compression 
     * by grouping common sequences of nodes together.<BR>
     * This structure provides a very efficient way of storing values while maintaining the lookup time 
     * for a key in O(N) in the worst case, where N is the length of the longest key. 
     * This structure has it's main use in IP routing software, but can provide an interesting alternative 
     * to other structures such as hashtables when memory space is of concern.
     * @return created PATRICIA trie
     */
    public PatriciaTrie createPatriciaTrie();

    /**
     * Commit transaction (if needed) and close the storage
     */
    public void close();

    /**
     * Retrieve object by OID. This method should be used with care because
     * if object is deallocated, its OID can be reused. In this case
     * getObjectByOID will return reference to the new object with may be
     * different type.
     * @param oid object oid
     * @return reference to the object with specified OID
     */
    public IPersistent getObjectByOID(int oid);

    /**
     * Explicitely make object peristent. Usually objects are made persistent
     * implicitlely using "persistency on reachability apporach", but this
     * method allows to do it explicitly. If object is already persistent, execution of
     * this method has no effect.
     * @param obj object to be made persistent
     * @return OID assigned to the object  
     */
    public int makePersistent(IPersistent obj);

 
    /**
     * Set database property. This method should be invoked before opening database. 
     * Currently the following boolean properties are supported:
     * <TABLE><TR><TH>Property name</TH><TH>Parameter type</TH><TH>Default value</TH><TH>Description</TH></TR>
     * <TR><TD><code>perst.object.cache.init.size</code></TD><TD>Integer</TD><TD>1319</TD>
     * <TD>Initial size of object cache
     * </TD></TR>
     * <TR><TD><code>perst.object.cache.pin.count</code></TD><TD>Integer</TD><TD>0</TD>
     * <TD>Number of objects pinned in object cache after cache clear
     * </TD></TR>
     * <TR><TD><code>perst.object.index.init.size</code></TD><TD>Integer</TD><TD>1024</TD>
     * <TD>Initial size of object index (specifying large value increase initial size of database, but reduce
     * number of index reallocations)
     * </TD></TR>
     * <TR><TD><code>perst.extension.quantum</code></TD><TD>Long</TD><TD>1048576</TD>
     * <TD>Object allocation bitmap extension quantum. Memory is allocate by scanning bitmap. If there is no
     * large enough hole, then database is extended by the value of dbDefaultExtensionQuantum. 
     * This parameter should not be smaller than 64Kb.
     * </TD></TR>
     * <TR><TD><code>perst.file.readonly</code></TD><TD>Boolean</TD><TD>false</TD>
     * <TD>Database file should be opened in read-only mode.
     * </TD></TR>
     * <TR><TD><code>perst.file.noflush</code></TD><TD>Boolean</TD><TD>false</TD>
     * <TD>Do not flush file during transaction commit. It will greatly increase performance because
     * eliminate synchronous write to the disk (when program has to wait until all changed
     * are actually written to the disk). But it can cause database corruption in case of 
     * OS or power failure (but abnormal termination of application itself should not cause
     * the problem, because all data which were written to the file, but is not yet saved to the disk is 
     * stored in OS file buffers and sooner or later them will be written to the disk)
     * </TD></TR>
     * <TR><TD><code>perst.alternative.btree</code></TD><TD>Boolean</TD><TD>false</TD>
     * <TD>Use aternative implementation of B-Tree (not using direct access to database
     * file pages). This implementation should be used in case of serialized per thread transctions.
     * New implementation of B-Tree will be used instead of old implementation
     * if "perst.alternative.btree" property is set. New B-Tree has incompatible format with 
     * old B-Tree, so you could not use old database or XML export file with new indices. 
     * Alternative B-Tree is needed to provide serializable transaction (old one could not be used).
     * Also it provides better performance (about 3 times comaring with old implementation) because
     * of object caching. And B-Tree supports keys of user defined types. 
     * </TD></TR>
     * <TR><TD><code>perst.string.encoding</code></TD><TD>String</TD><TD>null</TD>
     * <TD>Specifies encoding of storing strings in the database. By default Perst stores 
     * strings as sequence of chars (two bytes per char). If all strings in application are in 
     * the same language, then using encoding  can signifficantly reduce space needed
     * to store string (about two times). But please notice, that this option has influence
     * on all strings  stored in database. So if you already have some data in the storage
     * and then change encoding, then it can cause incorrect fetching of strings and even database crash.
     * </TD></TR>
     * <TR><TD><code>perst.concurrent.iterator</code></TD><TD>Boolean</TD><TD>false</TD>
     * <TD>By default iterator will throw ConcurrentModificationException if iterated collection
     * was changed outside iterator, when the value of this property is true then iterator will 
     * try to restore current position and continue iteration
     * </TD></TR>
     * <TR><TD><code>perst.force.store</code></TD><TD>Boolean</TD><TD>true</TD>
     * <TD>When the value of this parameter is true Storage.makePersistent method
     * cause immediate storing of object in the storage, otherwise object is assigned OID and is marked 
     * as modified. Storage.makePersistent method is mostly used when object is inserted in B-Tree.
     * If application put in index object referencing a large number of other objects which also has to 
     * be made persistent, then marking object as modified instead of immediate storing may cause
     * memory overflow because garbage collector and finalization threads will store objects
     * with less speed than application creates new ones.
     * But if object will be updated after been placed in B-Tree, then immediate store will just cause
     * cause extra overhead, because object has to be stored twice. 
     * </TD></TR>
     * </TABLE>
     * @param name name of the property
     * @param value value of the property (for boolean properties pass <code>java.lang.Boolean.TRUE</code>
     * and <code>java.lang.Boolean.FALSE</code>
     */
    public void setProperty(String name, Object value);

   /**
     * Set database properties. This method should be invoked before opening database. 
     * For list of supported properties please see <code>setProperty</code> command. 
     * All not recognized properties are ignored.
     */
    public void setProperties(Hashtable props);

    /**
     * Get property value.
     * @param name property name
     * @return value of the property previously assigned by setProperty or setProperties method
     * or <code>null</code> if property was not set
     */
    public Object getProperty(String name);

    /**
     * Get all set properties
     * @return all properties set by setProperty or setProperties method
     */
    public Hashtable getProperties();
    
    /**
     * Merge results of several index searches. This method efficiently merge selections without loading objects themselve
     * @param selections selections to be merged
     * @return Iterator through merged result
     */
    public Iterator merge(Iterator[] selections);

    
    /**
     * Join results of several index searches. This method efficiently join selections without loading objects themselve
     * @param selections selections to be merged
     * @return Iterator through joineded result
     */
    public Iterator join(Iterator[] selections);
 
    /**
     * Set storage listener.
     * @param listener new storage listener (may be null)
     * @return previous storage listener
     */
    public StorageListener setListener(StorageListener listener);

    /**
     * Get total size of all allocated objects in the database
     */
    public long getUsedSize();

    /**
     * Get size of the database
     */
    public long getDatabaseSize();

    /**
     * This method is used internally by Perst to get transaction context associated with current thread.
     * But it can be also used by application to get transaction context, store it in some variable and
     * use in another thread. I will make it possible to share one transaction between multiple threads.
     * @return transaction context associated with current thread
     */     
    public ThreadTransactionContext getTransactionContext();

    /**
     * Associate transaction context with the thread
     * This method can be used by application to share the same transaction between multiple threads
     * @param ctx new transaction context 
     * @return transaction context previously associated with this thread
     */     
    public ThreadTransactionContext setTransactionContext(ThreadTransactionContext ctx);

    /**
     * Get storage version
     * @return storage version ID previosly assigned by setVersion. If it was not assigned, then
     * 0 is returned.
     */
    public int getVersion();

    /**
     * Set storage version. It case be used to handle database schema evolution.
     * @param version new storage version ID. 
     */
    public void setVersion(int version);

    /**
     * Get version of database format for this database. When new database is created it is
     * always assigned the current database format version
     * @return databasse format version
     */
    public int getDatabaseFormatVersion();

    // Internal methods (I have to made them public to be used by AspectJ API)

    public void deallocateObject(IPersistent obj);

    public void storeObject(IPersistent obj);

    public void modifyObject(IPersistent obj);

    public void loadObject(IPersistent obj);

    public boolean lockObject(IPersistent obj);

    public void throwObject(IPersistent obj);
}


