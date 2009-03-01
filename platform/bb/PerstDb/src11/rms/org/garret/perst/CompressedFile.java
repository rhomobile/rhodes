package org.garret.perst;

import java.io.*;
import javax.microedition.rms.*;

public class CompressedFile implements IFile {
    private RecordStore[]  store;
    private String         name;
    private boolean        readWrite;
    private int            maxPagesInStore;
    private int            nPages;
    private long           size;
    private int            nOpenedStorages;
    private boolean        singleStorage;
    private ICompressor    compressor;
    private int            authorizationMode;
    private byte[]         page;
    private byte[]         zeroPage;
    private int            zeroPageSize;

    static final int PAGE_SIZE_LOG2 = 12;
    static final int PAGE_SIZE = 1 << PAGE_SIZE_LOG2;

    static final String SYSINFO_STORE_NAME = "SYSINFO";

    /**
     * Import data from specified input stream to record store
     * @param stream input stream. Data can be imported from resource file (jar file), 
     * from network connection or from other data source
     * @return true if data was successfully imported, false if storage is not empty or read operation from input stream
     * is failed
     */
    public boolean importData(InputStream stream) 
    { 
        try { 
            if (store.length != 1 && store[0].getNumRecords() != 0) { 
                return false;
            } 
            int pid = 0;
            int sid = 0;
            byte[] buf = page;
            DataInputStream in = new DataInputStream(stream);
            while (true) { 
                int rc, offs = 0;
                int size = in.readUnsignedShort();
                while (offs < size && (rc = in.read(buf, offs, size - offs)) > 0) { 
                    offs += rc;
                }
                if (offs <= 0) {
                    nPages = pid;
                    return true;
                } else if (offs != size) {
                    return false;
                }
                if (maxPagesInStore != 0 && pid >= maxPagesInStore) { 
                    store[sid].closeRecordStore();
                    store[sid] = null;
                    nOpenedStorages -= 1;
                    pid = 0;
                    if (!extend(++sid)) { 
                        return false;
                    }
                }
                while (true) { 
                    try {
                        store[sid].addRecord(buf, 0, size);
                        pid += 1;
                    } catch (RecordStoreFullException x) { 
                        if (maxPagesInStore == 0 && pid != 0) { 
                            store[sid].closeRecordStore();
                            store[sid] = null;
                            nOpenedStorages -= 1;
                            maxPagesInStore = pid;
                            pid = 0;
                            if (!extend(++sid)) { 
                                return false;
                            }                        
                        } else { 
                            return false;
                        }
                    }
                }
            }
        } catch (Exception x) {
            throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
        }
    }

    /**
     * Export data from record store to the specified output stream
     * @param stream output stream. Data can be exported to the file, network connection or to some other destination
     * @return true if data was successfully exported, false if read operation from record store is failed
     */
    public boolean exportData(OutputStream stream) 
    { 
        byte[] buf = page;
        try { 
            DataOutputStream out = new DataOutputStream(stream);
            closeAllStorages();
            for (int sid = 0; sid < store.length; sid++) { 
                if (!extend(sid)) { 
                    return false;
                }
                int n = store[sid].getNextRecordID();
                for (int i = 1; i < n; i++) { 
                    int rc = store[sid].getRecord(i, buf, 0);
                    if (rc <= 0) { 
                        return false;
                    }
                    out.writeShort(rc);
                    out.write(buf, 0, rc);
                }
                store[sid].closeRecordStore();
                store[sid] = null;
                nOpenedStorages -= 1;
            }
            return true;
        } catch (Exception x) {
            throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
        }
    }

    private String getStoreName(int sid) { 
        return name + Integer.toString(sid);
    }

    /**
     * Delete all record storages for this database
     */
    public void delete() { 
        try { 
            for (int sid = 0; sid < store.length; sid++) { 
                RecordStore.deleteRecordStore(getStoreName(sid));
            }
        } catch (Exception x) {
            throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
        }
     } 

    public static final int GUESS_STORE_LIMIT = -1;

    /**
     * Create compressed file
     * @param name store name (if database doesn't fit in one store, then several stores
     * with subsequent suffixes will be created
     * @param compressor implementation of data comrpession and/or encryption
     */
    public CompressedFile(String name, ICompressor compressor) {
        this(name, compressor, GUESS_STORE_LIMIT, true, RecordStore.AUTHMODE_ANY);
    }

    /**
     * Create compressed file
     * @param name store name (if database doesn't fit in one store, then several stores
     * with subsequent suffixes will be created) 
     * @param compressor implementation of data comrpession and/or encryption
     * @param storeSizeLimit maximal number of pages to be placed in storage.
     * As far as size of compressed page may varry depending on its content, 
     * it is not correct to calculate this value by catching RecordStoreFullException.
     * So it is necessary to specify some positive value or use GUESS_STORE_LIMIT which
     * makes Perst to detect this limit itself, but inserting dummy data in the storage.
     * @param readWrite open store in read-write mode (if false, then store will be opened in read-only mode)
     * @param authorizationMode authorization mode for store:
     * RecordStore.AUTHMODE_ANY
     *     Authorization to allow access to any MIDlet suites.
     * RecordStore.AUTHMODE_PRIVATE
     *     Authorization to allow access only to the current MIDlet suite.
     */
    public CompressedFile(String name, ICompressor compressor, 
                          int storeSizeLimit, boolean readWrite, int authorizationMode)
    {
        try { 
            this.name = name;
            this.compressor = compressor;
            this.authorizationMode = authorizationMode;
            this.readWrite = readWrite;
            page = new byte[PAGE_SIZE];
            zeroPage = new byte[PAGE_SIZE];
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
            store[0] = RecordStore.openRecordStore(getStoreName(0), readWrite, authorizationMode, readWrite);
            nOpenedStorages += 1;
            nPages = store[0].getNumRecords();
            if (maxStoreId > 0) { 
                maxPagesInStore = nPages;
                store[maxStoreId] = RecordStore.openRecordStore(getStoreName(maxStoreId), readWrite, authorizationMode, readWrite);
                nOpenedStorages += 1;
                size = (maxStoreId*maxPagesInStore + store[maxStoreId].getNumRecords())*PAGE_SIZE;
            } else { 
                if (storeSizeLimit == GUESS_STORE_LIMIT) { 
                    RecordStore sysinfo = RecordStore.openRecordStore(SYSINFO_STORE_NAME, true, RecordStore.AUTHMODE_PRIVATE, false);
                    if (sysinfo.getNumRecords() != 0) { 
                        DataInputStream in = new DataInputStream(new ByteArrayInputStream(sysinfo.getRecord(1)));
                        maxPagesInStore = in.readInt();
                    } else { 
                        int maxRecords = 0;
                        while (true) { 
                            try { 
                                sysinfo.addRecord(zeroPage, 0, PAGE_SIZE);
                                maxRecords += 1;
                            } catch (RecordStoreFullException x) { 
                                break;
                            }
                        } 
                        maxPagesInStore = maxRecords;
                        sysinfo.closeRecordStore();
                        RecordStore.deleteRecordStore(SYSINFO_STORE_NAME);
                        sysinfo = RecordStore.openRecordStore(SYSINFO_STORE_NAME, true, RecordStore.AUTHMODE_PRIVATE, false);
                        ByteArrayOutputStream out = new ByteArrayOutputStream();
                        DataOutputStream dout = new DataOutputStream(out);
                        dout.writeInt(maxRecords);
                        dout.close();
                        byte[] info = out.toByteArray();
                        sysinfo.addRecord(info, 0, info.length);
                    }
                    sysinfo.closeRecordStore();
                } else {
                    maxPagesInStore = storeSizeLimit / PAGE_SIZE;
                }
                size = nPages*PAGE_SIZE;
            }                
            zeroPageSize = compressor.compress(zeroPage, page, PAGE_SIZE);
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
                s = RecordStore.openRecordStore(getStoreName(sid), readWrite, authorizationMode, readWrite);
                nOpenedStorages += 1;
            } catch (RecordStoreNotFoundException x) { 
                return false;
            }
            RecordStore[] newStore = new RecordStore[sid+1];
            System.arraycopy(store, 0, newStore, 0, store.length);
            store = newStore;
            newStore[sid] = s;
        } else if (store[sid] == null) { 
            store[sid] = RecordStore.openRecordStore(getStoreName(sid), readWrite, authorizationMode, readWrite);
            nOpenedStorages += 1;
        }
        nPages = store[sid].getNumRecords();
        return true;
    }

    public void sync()
    {
    }

    public int read(long pos, byte[] buf) 
    {
        while (true) { 
            try { 
                int currPage = (int)(pos >>> PAGE_SIZE_LOG2);
                int sid = 0;
                if (maxPagesInStore != 0) {
                    sid = currPage / maxPagesInStore;
                    currPage %= maxPagesInStore;
                    if (sid >= store.length) {
                        return 0;
                    }
                    extend(sid);
                }
                if (currPage >= nPages) { 
                    return 0;
                }
                int compressedSize = store[sid].getRecord(currPage+1, page, 0);
                int size = compressor.decompress(buf, page, compressedSize);
                if (size != PAGE_SIZE) { 
                    throw new StorageError(StorageError.FILE_ACCESS_ERROR, "Decompression error");
                }     
                return size;
            } catch (RecordStoreException x) { 
                if (!singleStorage && store.length > 1) { 
                    singleStorage = true;
                } else { 
                    throw new StorageError(StorageError.FILE_ACCESS_ERROR, x.getMessage());
                }
            } catch (IOException x) { 
                throw new StorageError(StorageError.FILE_ACCESS_ERROR, x.getMessage());
            } 
        }
    }

    public void write(long pos, byte[] buf) 
    {
        while (true) { 
            try { 
                int currPage = (int)(pos >>> PAGE_SIZE_LOG2);            
                int sid = 0;
                if (maxPagesInStore != 0) {
                    sid = currPage / maxPagesInStore;
                    currPage %= maxPagesInStore;
                    if (!extend(sid)) { 
                        throw new StorageError(StorageError.FILE_ACCESS_ERROR, "Failed to create new storage");
                    }
                }
                if (currPage >= nPages) {
                    try { 
                        while (currPage > nPages) { 
                            store[sid].addRecord(zeroPage, 0, zeroPageSize);
                            nPages += 1;
                        }
                        int compressedSize = compressor.compress(page, buf, PAGE_SIZE);
                        store[sid].addRecord(page, 0, compressedSize);
                        nPages += 1;
                    } catch (RecordStoreFullException x) { 
                        if (maxPagesInStore != 0) { 
                            throw new StorageError(StorageError.FILE_ACCESS_ERROR, "Space is exhausted");
                        }
                        maxPagesInStore = nPages;
                        continue;
                    }
                } else { 
                    int compressedSize = compressor.compress(page, buf, PAGE_SIZE);
                    store[sid].setRecord(currPage+1, page, 0, compressedSize);
                }
                break;
            } catch (RecordStoreException x) { 
                if (!singleStorage && store.length > 1) { 
                    singleStorage = true;
                } else { 
                    throw new StorageError(StorageError.FILE_ACCESS_ERROR, x.getMessage());
                }
            } catch (IOException x) { 
                throw new StorageError(StorageError.FILE_ACCESS_ERROR, x.getMessage());
            } 
        }
        if (pos >= size) { 
            size = pos + PAGE_SIZE;
        }
    }

    public long length()  { 
        return size;
    }
    
    public void close() { 
        try {
            closeAllStorages();
        } catch (RecordStoreException x) { 
            throw new StorageError(StorageError.FILE_ACCESS_ERROR, x.getMessage());
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
}
