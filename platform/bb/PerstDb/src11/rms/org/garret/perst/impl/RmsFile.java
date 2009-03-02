package org.garret.perst.impl;

import java.io.*;
import javax.microedition.rms.*;
import org.garret.perst.*;

public class RmsFile implements SimpleFile 
{
    private RecordStore[]  store;
    private String         name;
    private boolean        readWrite;
    private int            maxPagesInStore;
    private int            nPages;
    private int            size;
    private int            nOpenedStorages;
    private boolean        singleStorage;

    static final int PAGE_SIZE_LOG2 = 12;
    static final int PAGE_SIZE = 1 << PAGE_SIZE_LOG2;

    /**
     * Store authorization mode: 
     * AUTHMODE_ANY
     *     Authorization to allow access to any MIDlet suites.
     * AUTHMODE_PRIVATE
     *     Authorization to allow access only to the current MIDlet suite.
     */
    public static int authorizationMode = RecordStore.AUTHMODE_ANY;

    /**
     * Import data from specified input stream to record store
     * @param storeName storage name prefix. If there is limitation for storage size, 
     * then several storages with this prefix and suffixes 0, 1, 2,... will be created.
     * It should be the same as storage name used in Storage.open method.
     * @param in input stream. Data can be imported from resource file (jar file), from network connection or from other
     * data source
     * @return true if data was successfully imported, false if storage is not empty or read operation from input stream
     * is failed
     */
    public static boolean importData(String storeName, InputStream in) throws IOException, RecordStoreException 
    { 
        byte[] buf = new byte[PAGE_SIZE];
        int storeNo = 0;
        RecordStore store = RecordStore.openRecordStore(storeName + storeNo, true, authorizationMode, true);
        try { 
            if (store.getNumRecords() != 0) { 
                return false;
            }
            while (true) { 
                int rc, offs = 0;
                while (offs < PAGE_SIZE && (rc = in.read(buf, offs, PAGE_SIZE - offs)) > 0) { 
                    offs += rc;
                }
                if (offs <= 0) { 
                    return true;
                } else if (offs != PAGE_SIZE) {
                    return false;
                }
                try {
                    store.addRecord(buf, 0, PAGE_SIZE);
                } catch (RecordStoreFullException x) { 
                    store.closeRecordStore();
                    storeNo += 1;
                    store = RecordStore.openRecordStore(storeName + storeNo, true, authorizationMode, true);
                }
            }
        } finally { 
            store.closeRecordStore();
        }
    }

    /**
     * Export data from record store to the specified output stream
     * @param storeName storage name prefix. If there is limitation for storage size, then several storages with 
     * this prefix and suffixes 0, 1, 2,... will be openned.
     * It should be the same as storage name used in Storage.open method.
     * @param out output stream. Data can be exported to the file, network connection or to some other destination
     * @return true if data was successfully exported, false if read operation from record store is failed
     */
    public static boolean exportData(String storeName, OutputStream out) throws IOException, RecordStoreException 
    { 
        byte[] buf = new byte[PAGE_SIZE];
        int storeNo = 0;
        RecordStore store = RecordStore.openRecordStore(storeName + storeNo, false);
        if (store == null) { 
            return false;
        }
        try { 
            while (true) {             
                int n = store.getNextRecordID();
                for (int i = 1; i < n; i++) { 
                    int rc = store.getRecord(i, buf, 0);
                    if (rc != PAGE_SIZE) { 
                        return false;
                    }
                    out.write(buf, 0, PAGE_SIZE);
                }
                try {
                    store.closeRecordStore();
                    storeNo += 1;
                    store = null;
                    store = RecordStore.openRecordStore(storeName + storeNo, false);
                } catch (RecordStoreNotFoundException x) { 
                    return true;
                }
            }
        } finally { 
            if (store != null) { 
                store.closeRecordStore();
            }
        }
    }

    /**
     * Delete all record storages with specified prefix
     * @param prefix record storage name prefix
     */
    public static void deleteRecordStores(String prefix) throws RecordStoreException { 
        String[] stores = RecordStore.listRecordStores();
        if (stores != null) { 
            for (int i = 0; i < stores.length; i++) { 
                if (stores[i].startsWith(prefix)) {
                    RecordStore.deleteRecordStore(stores[i]);
                }
            }
        }
    } 

    public void open(String name, boolean readOnly, boolean noFlush)
        {
        try { 
            this.name = name;
            readWrite = !readOnly;
            String[] stores = RecordStore.listRecordStores();
            int maxStoreId = 0;
            int prefixLength = name.length();
            if (stores != null) { 
                for (int i = 0; i < stores.length; i++) { 
                    if (stores[i].startsWith(name)) { 
                        try {
                            int sid = Integer.parseInt(stores[i].substring(prefixLength));
                            if (sid > maxStoreId) { 
                                maxStoreId = sid;
                            }
                        } catch (NumberFormatException x) {}
                    }
                }
            }
            store = new RecordStore[maxStoreId+1];
            store[0] = RecordStore.openRecordStore(name + '0', readWrite, authorizationMode, readWrite);
            nOpenedStorages += 1;
            nPages = store[0].getNumRecords();
            if (maxStoreId > 0) { 
                maxPagesInStore = nPages;
                store[maxStoreId] = RecordStore.openRecordStore(name + Integer.toString(maxStoreId), readWrite, authorizationMode, readWrite);
                nOpenedStorages += 1;
                size = (maxStoreId*maxPagesInStore + store[maxStoreId].getNumRecords())*PAGE_SIZE;
            } else { 
                size = nPages*PAGE_SIZE;
            }                
        } catch (Exception x) { 
            throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
        }
    }

    private boolean extend(int sid) throws RecordStoreException { 
        if (singleStorage && (nOpenedStorages > 1 || (sid >= store.length || store[sid] == null))) { 
            closeAllStorages();
        }
        if (sid >= store.length) { 
            RecordStore s;
            try { 
                s = RecordStore.openRecordStore(name + Integer.toString(sid), readWrite, authorizationMode, readWrite);
                nOpenedStorages += 1;
            } catch (RecordStoreNotFoundException x) { 
                return false;
            }
            RecordStore[] newStore = new RecordStore[sid+1];
            System.arraycopy(store, 0, newStore, 0, store.length);
            store = newStore;
            newStore[sid] = s;
        } else if (store[sid] == null) { 
            store[sid] = RecordStore.openRecordStore(name + Integer.toString(sid), readWrite, authorizationMode, readWrite);
            nOpenedStorages += 1;
        }
        nPages = store[sid].getNumRecords();
        return true;
    }

    public int read(long pos, byte[] b)
    {
        int currPos = (int)pos;
        int off = 0;
        int len = b.length;
        int rc = len;

        while (len > 0) { 
            try { 
                int currPage = currPos >>> PAGE_SIZE_LOG2;
                int sid = 0;
                if (maxPagesInStore != 0) {
                    sid = currPage / maxPagesInStore;
                    currPage %= maxPagesInStore;
                    if (!extend(sid)) { 
                        break;
                    }
                }
                if (currPage >= nPages) { 
                    break;
                }
                byte[] pageData = store[sid].getRecord(currPage+1);
                int pageOffs = currPos & (PAGE_SIZE - 1);
                int n = PAGE_SIZE - pageOffs < len ? PAGE_SIZE - pageOffs : len;
                System.arraycopy(pageData, pageOffs, b, off, n);
                len -= n;
                currPos += n;
                off += n;
            } catch (RecordStoreException x) { 
                if (!singleStorage && store.length > 1) { 
                    singleStorage = true;
                } else { 
                    throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
                }
            } 
        }
        return rc - len;
    }

    public void write(long pos, byte[] b)
    {
        int currPos = (int)pos;
        int off = 0;
        int len = b.length;
        while (len > 0) { 
            try { 
                int currPage = currPos >>> PAGE_SIZE_LOG2;            
                int sid = 0;
                if (maxPagesInStore != 0) {
                    sid = currPage / maxPagesInStore;
                    currPage %= maxPagesInStore;
                    if (!extend(sid)) { 
                        throw new StorageError(StorageError.FILE_ACCESS_ERROR, "Failed to create new storage");
                    }
                }
                byte[] pageData;
                if (currPage >= nPages) { 
                    pageData = new byte[PAGE_SIZE];
                } else if (off != 0 || len != PAGE_SIZE || (currPos & (PAGE_SIZE-1)) != 0) { 
                    pageData = store[sid].getRecord(currPage+1);
                } else { 
                    pageData = b;
                }
                int pageOffs = currPos & (PAGE_SIZE - 1);            
                int n = PAGE_SIZE - pageOffs < len ? PAGE_SIZE - pageOffs : len;
                if (currPage >= nPages) {
                    try { 
                        while (currPage > nPages) { 
                            store[sid].addRecord(pageData, 0, PAGE_SIZE);
                            nPages += 1;
                        }
                        System.arraycopy(b, off, pageData, pageOffs, n);
                        store[sid].addRecord(pageData, 0, PAGE_SIZE);
                        nPages += 1;
                    } catch (RecordStoreFullException x) { 
                        if (maxPagesInStore != 0) { 
                            throw new StorageError(StorageError.FILE_ACCESS_ERROR, "Space is exhausted");
                        }
                        maxPagesInStore = nPages;
                        continue;
                    }
                } else { 
                    if (pageData != b) { 
                        System.arraycopy(b, off, pageData, pageOffs, n);
                    }
                    store[sid].setRecord(currPage+1, pageData, 0, PAGE_SIZE);
                }
                len -= n;
                currPos += n;
                off += n;
            } catch (RecordStoreException x) { 
                if (!singleStorage && store.length > 1) { 
                    singleStorage = true;
                } else { 
                    throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
                }
            } 
        }
        if (currPos > size) { 
            size = (currPos + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
        }
    }

    public long length() { 
        return size;
    }
    
    public void close() { 
        try {
            closeAllStorages();
        } catch (RecordStoreException x) { 
            throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
        } 
    }

    private void closeAllStorages() throws RecordStoreException { 
        for (int i = 0; i < store.length; i++) { 
            if (store[i] != null) { 
                store[i].closeRecordStore();
                store[i] = null;
                nOpenedStorages -= 1;
            }
        }
    }        
    
    public void sync() {
    }
}
