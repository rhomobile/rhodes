package org.garret.perst.impl;

import org.garret.perst.*;
import java.util.*;
import java.io.*;

public class StorageImpl implements Storage { 
    /**
     * Initialial database index size - increasing it reduce number of inde reallocation but increase
     * initial database size. Should be set before openning connection.
     */
    static final int  dbDefaultInitIndexSize = 1024;

    /**
     * Initial capacity of object hash
     */
    static final int  dbDefaultObjectCacheInitSize = 1319;

    /**
     * Default number of pinned objects in object cache
     */
    static final int  dbDefaultObjectCachePinCount = 100;

    /**
     * Database extension quantum. Memory is allocate by scanning bitmap. If there is no
     * large enough hole, then database is extended by the value of dbDefaultExtensionQuantum 
     * This parameter should not be smaller than dbFirstUserId
     */
    static final long dbDefaultExtensionQuantum = 1024*1024;

    static final int  dbDatabaseOidBits = 20;          // up to 1 million of objects
    static final int  dbDatabaseOffsetBits = 30;       // up to 1 gigabyte
    static final int  dbLargeDatabaseOffsetBits = 40;  // up to 1 terabyte

    static final int  dbMaxObjectOid = (1 << dbDatabaseOidBits) - 1;

    static final int  dbAllocationQuantumBits = 5;
    static final int  dbAllocationQuantum = 1 << dbAllocationQuantumBits;
    static final int  dbBitmapSegmentBits = Page.pageSizeLog + 3 + dbAllocationQuantumBits;
    static final int  dbBitmapSegmentSize = 1 << dbBitmapSegmentBits;
    static final int  dbBitmapPages = 1 << (dbDatabaseOffsetBits-dbBitmapSegmentBits);
    static final int  dbLargeBitmapPages = 1 << (dbLargeDatabaseOffsetBits-dbBitmapSegmentBits);
    static final int  dbHandlesPerPageBits = Page.pageSizeLog - 3;
    static final int  dbHandlesPerPage = 1 << dbHandlesPerPageBits;
    static final int  dbDirtyPageBitmapSize = 1 << (dbDatabaseOidBits-dbHandlesPerPageBits-3);

    static final int  dbInvalidId   = 0;
    static final int  dbBitmapId    = 1;
    static final int  dbFirstUserId = dbBitmapId + dbBitmapPages;
    
    static final int  dbPageObjectFlag = 1;
    static final int  dbModifiedFlag   = 2;
    static final int  dbFreeHandleFlag = 4;
    static final int  dbFlagsMask      = 7;
    static final int  dbFlagsBits      = 3;

    /**
     * Current version of database format. 0 means that database is not initilized.
     * Used to provide backward compatibility of Perst releases.
     */
    static final byte dbDatabaseFormatVersion = (byte)2;

    final int getBitmapPageId(int i) { 
        return i < dbBitmapPages ? dbBitmapId + i : header.root[1-currIndex].bitmapExtent + i - bitmapExtentBase;
    }

    final long getPos(int oid) { 
        synchronized (objectCache) {
            if (oid == 0 || oid >= currIndexSize) { 
                throw new StorageError(StorageError.INVALID_OID);
            }
            Page pg = pool.getPage(header.root[1-currIndex].index 
                                   + ((long)(oid >>> dbHandlesPerPageBits) << Page.pageSizeLog));
            long pos = Bytes.unpack8(pg.data, (oid & (dbHandlesPerPage-1)) << 3);
            pool.unfix(pg);
            return pos;
        }
    }
    
    final void setPos(int oid, long pos) { 
        synchronized (objectCache) {
            dirtyPagesMap[oid >>> (dbHandlesPerPageBits+5)] 
                |= 1 << ((oid >>> dbHandlesPerPageBits) & 31);
            Page pg = pool.putPage(header.root[1-currIndex].index 
                                   + ((long)(oid >>> dbHandlesPerPageBits) << Page.pageSizeLog));
            Bytes.pack8(pg.data, (oid & (dbHandlesPerPage-1)) << 3, pos);
            pool.unfix(pg);
        }
    }

    final byte[] get(int oid) { 
        long pos = getPos(oid);
        if ((pos & (dbFreeHandleFlag|dbPageObjectFlag)) != 0) { 
            throw new StorageError(StorageError.INVALID_OID);
        }
        return pool.get(pos & ~dbFlagsMask);
    }
    
    final Page getPage(int oid) {  
        long pos = getPos(oid);
        if ((pos & (dbFreeHandleFlag|dbPageObjectFlag)) != dbPageObjectFlag) { 
            throw new StorageError(StorageError.DELETED_OBJECT);
        }
        return pool.getPage(pos & ~dbFlagsMask);
    }

    final Page putPage(int oid) {  
        synchronized (objectCache) {
            long pos = getPos(oid);
            if ((pos & (dbFreeHandleFlag|dbPageObjectFlag)) != dbPageObjectFlag) { 
                throw new StorageError(StorageError.DELETED_OBJECT);
            }
            if ((pos & dbModifiedFlag) == 0) { 
                dirtyPagesMap[oid >>> (dbHandlesPerPageBits+5)] 
                    |= 1 << ((oid >>> dbHandlesPerPageBits) & 31);
                allocate(Page.pageSize, oid);
                cloneBitmap(pos & ~dbFlagsMask, Page.pageSize);
                pos = getPos(oid);
            }
            modified = true;
            return pool.putPage(pos & ~dbFlagsMask);
        }
    }


    int allocatePage() { 
        int oid = allocateId();
        setPos(oid, allocate(Page.pageSize, 0) | dbPageObjectFlag | dbModifiedFlag);
        return oid;
    }

    public synchronized void deallocateObject(IPersistent obj) 
    {
        synchronized (objectCache) {
            if (obj.getOid() == 0) { 
                return;
            }
            if (useSerializableTransactions) { 
                ThreadTransactionContext ctx = getTransactionContext();
                if (ctx.nested != 0) { // serializable transaction
                    ctx.deleted.add(obj);  
                    return;
                }
            }
            deallocateObject0(obj);
        }
    }

    private void deallocateObject0(IPersistent obj)
    {
        int oid = obj.getOid();
        long pos = getPos(oid);
        objectCache.remove(oid);
        int offs = (int)pos & (Page.pageSize-1);
        if ((offs & (dbFreeHandleFlag|dbPageObjectFlag)) != 0) { 
            throw new StorageError(StorageError.DELETED_OBJECT);
        }
        Page pg = pool.getPage(pos - offs);
        offs &= ~dbFlagsMask;
        int size = ObjectHeader.getSize(pg.data, offs);
        pool.unfix(pg);
        freeId(oid);
        if ((pos & dbModifiedFlag) != 0) { 
            free(pos & ~dbFlagsMask, size);
        } else { 
            cloneBitmap(pos, size);
        }
        obj.assignOid(null, 0, false);
    }    

    final void freePage(int oid) {
        long pos = getPos(oid);
        Assert.that((pos & (dbFreeHandleFlag|dbPageObjectFlag)) == dbPageObjectFlag);
        if ((pos & dbModifiedFlag) != 0) { 
            free(pos & ~dbFlagsMask, Page.pageSize);
        } else { 
            cloneBitmap(pos & ~dbFlagsMask, Page.pageSize);
        } 
        freeId(oid);
    }

    int allocateId() {
        synchronized (objectCache) { 
            int oid;
            int curr = 1-currIndex;
            setDirty();
            if ((oid = header.root[curr].freeList) != 0) { 
                header.root[curr].freeList = (int)(getPos(oid) >> dbFlagsBits);
                Assert.that(header.root[curr].freeList >= 0);
                dirtyPagesMap[oid >>> (dbHandlesPerPageBits+5)] 
                    |= 1 << ((oid >>> dbHandlesPerPageBits) & 31);
                return oid;
            }

            if (currIndexSize > dbMaxObjectOid) { 
                throw new StorageError(StorageError.TOO_MUCH_OBJECTS);
            }
            if (currIndexSize >= header.root[curr].indexSize) {
                int oldIndexSize = header.root[curr].indexSize;
                int newIndexSize = oldIndexSize << 1;
                if (newIndexSize < oldIndexSize) { 
                    newIndexSize = Integer.MAX_VALUE & ~(dbHandlesPerPage-1);
                    if (newIndexSize <= oldIndexSize) { 
                        throw new StorageError(StorageError.NOT_ENOUGH_SPACE);
                    }
                }
                long newIndex = allocate(newIndexSize*8L, 0);
                if (currIndexSize >= header.root[curr].indexSize) {
                    long oldIndex = header.root[curr].index;
                    pool.copy(newIndex, oldIndex, currIndexSize*8L);
                    header.root[curr].index = newIndex;
                    header.root[curr].indexSize = newIndexSize;
                    free(oldIndex, oldIndexSize*8L);
                } else { 
                    // index was already reallocated
                    free(newIndex, newIndexSize*8L);
                }
            }
            oid = currIndexSize;
            header.root[curr].indexUsed = ++currIndexSize;
            return oid;
        }
    }
    
    void freeId(int oid)
    {
        synchronized (objectCache) { 
            setPos(oid, ((long)(header.root[1-currIndex].freeList) << dbFlagsBits)
                   | dbFreeHandleFlag);
            header.root[1-currIndex].freeList = oid;
        }
    }
    
    final static byte firstHoleSize [] = {
        8,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
        5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
        6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
        5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
        7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
        5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
        6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
        5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
    };
    final static byte lastHoleSize [] = {
        8,7,6,6,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
    final static byte maxHoleSize [] = {
        8,7,6,6,5,5,5,5,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
        5,4,3,3,2,2,2,2,3,2,2,2,2,2,2,2,4,3,2,2,2,2,2,2,3,2,2,2,2,2,2,2,
        6,5,4,4,3,3,3,3,3,2,2,2,2,2,2,2,4,3,2,2,2,1,1,1,3,2,1,1,2,1,1,1,
        5,4,3,3,2,2,2,2,3,2,1,1,2,1,1,1,4,3,2,2,2,1,1,1,3,2,1,1,2,1,1,1,
        7,6,5,5,4,4,4,4,3,3,3,3,3,3,3,3,4,3,2,2,2,2,2,2,3,2,2,2,2,2,2,2,
        5,4,3,3,2,2,2,2,3,2,1,1,2,1,1,1,4,3,2,2,2,1,1,1,3,2,1,1,2,1,1,1,
        6,5,4,4,3,3,3,3,3,2,2,2,2,2,2,2,4,3,2,2,2,1,1,1,3,2,1,1,2,1,1,1,
        5,4,3,3,2,2,2,2,3,2,1,1,2,1,1,1,4,3,2,2,2,1,1,1,3,2,1,1,2,1,1,0
    };
    final static byte maxHoleOffset [] = {
        0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,0,1,5,5,5,5,5,5,0,5,5,5,5,5,5,5,
        0,1,2,2,0,3,3,3,0,1,6,6,0,6,6,6,0,1,2,2,0,6,6,6,0,1,6,6,0,6,6,6,
        0,1,2,2,3,3,3,3,0,1,4,4,0,4,4,4,0,1,2,2,0,1,0,3,0,1,0,2,0,1,0,5,
        0,1,2,2,0,3,3,3,0,1,0,2,0,1,0,4,0,1,2,2,0,1,0,3,0,1,0,2,0,1,0,7,
        0,1,2,2,3,3,3,3,0,4,4,4,4,4,4,4,0,1,2,2,0,5,5,5,0,1,5,5,0,5,5,5,
        0,1,2,2,0,3,3,3,0,1,0,2,0,1,0,4,0,1,2,2,0,1,0,3,0,1,0,2,0,1,0,6,
        0,1,2,2,3,3,3,3,0,1,4,4,0,4,4,4,0,1,2,2,0,1,0,3,0,1,0,2,0,1,0,5,
        0,1,2,2,0,3,3,3,0,1,0,2,0,1,0,4,0,1,2,2,0,1,0,3,0,1,0,2,0,1,0,0
    };

    static final int pageBits = Page.pageSize*8;
    static final int inc = Page.pageSize/dbAllocationQuantum/8;

    static final void memset(Page pg, int offs, int pattern, int len) { 
        byte[] arr = pg.data;
        byte pat = (byte)pattern;
        while (--len >= 0) { 
            arr[offs++] = pat;
        }
    }

    final void extend(long size)
    {
        if (size > header.root[1-currIndex].size) { 
            header.root[1-currIndex].size = size;
        }
    }

    public long getUsedSize() { 
        return usedSize;
    }

    public long getDatabaseSize() { 
        return header.root[1-currIndex].size;
    }

    static class Location { 
        long     pos;
        long     size;
        Location next;
    }

    final boolean wasReserved(long pos, long size) 
    {
        for (Location location = reservedChain; location != null; location = location.next) { 
            if ((pos >= location.pos && pos - location.pos < location.size) 
                || (pos <= location.pos && location.pos - pos < size)) 
            {
                return true;
            }
        }
        return false;
    }

    final void reserveLocation(long pos, long size)
    {
        Location location = new Location();
        location.pos = pos;
        location.size = size;
        location.next = reservedChain;
        reservedChain = location;
    }

    final void commitLocation()
    {
        reservedChain = reservedChain.next;
    }


    final void setDirty() 
    {
        modified = true;
        if (!header.dirty) { 
            header.dirty = true;
            Page pg = pool.putPage(0);
            header.pack(pg.data);
            pool.flush();
            pool.unfix(pg);
        }
    }

    final Page putBitmapPage(int i) { 
        return putPage(getBitmapPageId(i));
    }

    final Page getBitmapPage(int i) { 
        return getPage(getBitmapPageId(i));
    }


    final long allocate(long size, int oid)
    {
        synchronized (objectCache) {
            setDirty();
            size = (size + dbAllocationQuantum-1) & ~(dbAllocationQuantum-1);
            Assert.that(size != 0);

            int  objBitSize = (int)(size >> dbAllocationQuantumBits);
            Assert.that(objBitSize == (size >> dbAllocationQuantumBits));
            long pos;    
            int  holeBitSize = 0;
            int  alignment = (int)size & (Page.pageSize-1);
            int  offs, firstPage, lastPage, i, j;
            int  holeBeforeFreePage  = 0;
            int  freeBitmapPage = 0;
            int  curr = 1 - currIndex;
            Page pg;


            lastPage = header.root[curr].bitmapEnd - dbBitmapId;
            usedSize += size;

            if (alignment == 0) { 
                firstPage = currPBitmapPage;
                offs = (currPBitmapOffs+inc-1) & ~(inc-1);
            } else { 
                firstPage = currRBitmapPage;
                offs = currRBitmapOffs;
            }
        
            while (true) { 
                if (alignment == 0) { 
                    // allocate page object 
                    for (i = firstPage; i < lastPage; i++){
                        int spaceNeeded = objBitSize - holeBitSize < pageBits 
                            ? objBitSize - holeBitSize : pageBits;
                        if (bitmapPageAvailableSpace[i] <= spaceNeeded) {
                            holeBitSize = 0;
                            offs = 0;
                            continue;
                        }
                        pg = getBitmapPage(i);
                        int startOffs = offs;   
                        while (offs < Page.pageSize) { 
                            if (pg.data[offs++] != 0) { 
                                offs = (offs + inc - 1) & ~(inc-1);
                                holeBitSize = 0;
                            } else if ((holeBitSize += 8) == objBitSize) { 
                                pos = (((long)i*Page.pageSize + offs)*8 - holeBitSize) 
                                    << dbAllocationQuantumBits;
                                if (wasReserved(pos, size)) { 
                                    startOffs = offs = (offs + inc - 1) & ~(inc-1);
                                    holeBitSize = 0;
                                    continue;
                                }       
                                reserveLocation(pos, size);
                                currPBitmapPage = i;
                                currPBitmapOffs = offs;
                                extend(pos + size);
                                if (oid != 0) { 
                                    long prev = getPos(oid);
                                    int marker = (int)prev & dbFlagsMask;
                                    pool.copy(pos, prev - marker, size);
                                    setPos(oid, pos | marker | dbModifiedFlag);
                                }
                                pool.unfix(pg);
                                pg = putBitmapPage(i);
                                int holeBytes = holeBitSize >> 3;
                                if (holeBytes > offs) { 
                                    memset(pg, 0, 0xFF, offs);
                                    holeBytes -= offs;
                                    pool.unfix(pg);
                                    pg = putBitmapPage(--i);
                                    offs = Page.pageSize;
                                }
                                while (holeBytes > Page.pageSize) { 
                                    memset(pg, 0, 0xFF, Page.pageSize);
                                    holeBytes -= Page.pageSize;
                                    bitmapPageAvailableSpace[i] = 0;
                                    pool.unfix(pg);
                                    pg = putBitmapPage(--i);
                                }
                                memset(pg, offs-holeBytes, 0xFF, holeBytes);
                                commitLocation();
                                pool.unfix(pg);
                                return pos;
                            }
                        }
                        if (startOffs == 0 && holeBitSize == 0
                            && spaceNeeded < bitmapPageAvailableSpace[i]) 
                        { 
                            bitmapPageAvailableSpace[i] = spaceNeeded;
                        }
                        offs = 0;
                        pool.unfix(pg);
                    }
                } else { 
                    for (i = firstPage; i < lastPage; i++){
                        int spaceNeeded = objBitSize - holeBitSize < pageBits 
                            ? objBitSize - holeBitSize : pageBits;
                        if (bitmapPageAvailableSpace[i] <= spaceNeeded) {
                            holeBitSize = 0;
                            offs = 0;
                            continue;
                        }
                        pg = getBitmapPage(i);
                        int startOffs = offs;
                        while (offs < Page.pageSize) { 
                            int mask = pg.data[offs] & 0xFF; 
                            if (holeBitSize + firstHoleSize[mask] >= objBitSize) { 
                                pos = (((long)i*Page.pageSize + offs)*8 
                                       - holeBitSize) << dbAllocationQuantumBits;
                                if (wasReserved(pos, size)) {                       
                                    startOffs = offs += 1;
                                    holeBitSize = 0;
                                    continue;
                                }       
                                reserveLocation(pos, size);
                                currRBitmapPage = i;
                                currRBitmapOffs = offs;
                                extend(pos + size);
                                if (oid != 0) { 
                                    long prev = getPos(oid);
                                    int marker = (int)prev & dbFlagsMask;
                                    pool.copy(pos, prev - marker, size);
                                    setPos(oid, pos | marker | dbModifiedFlag);
                                }
                                pool.unfix(pg);
                                pg = putBitmapPage(i);
                                pg.data[offs] |= (byte)((1 << (objBitSize - holeBitSize)) - 1); 
                                if (holeBitSize != 0) { 
                                    if (holeBitSize > offs*8) { 
                                        memset(pg, 0, 0xFF, offs);
                                        holeBitSize -= offs*8;
                                        pool.unfix(pg);
                                        pg = putBitmapPage(--i);
                                        offs = Page.pageSize;
                                    }
                                    while (holeBitSize > pageBits) { 
                                        memset(pg, 0, 0xFF, Page.pageSize);
                                        holeBitSize -= pageBits;
                                        bitmapPageAvailableSpace[i] = 0;
                                        pool.unfix(pg);
                                        pg = putBitmapPage(--i);
                                    }
                                    while ((holeBitSize -= 8) > 0) { 
                                        pg.data[--offs] = (byte)0xFF; 
                                    }
                                    pg.data[offs-1] |= (byte)~((1 << -holeBitSize) - 1);
                                }
                                pool.unfix(pg);
                                commitLocation();
                                return pos;
                            } else if (maxHoleSize[mask] >= objBitSize) { 
                                int holeBitOffset = maxHoleOffset[mask];
                                pos = (((long)i*Page.pageSize + offs)*8 + holeBitOffset) << dbAllocationQuantumBits;
                                if (wasReserved(pos, size)) { 
                                    startOffs = offs += 1;
                                    holeBitSize = 0;
                                    continue;
                                }       
                                reserveLocation(pos, size);
                                currRBitmapPage = i;
                                currRBitmapOffs = offs;
                                extend(pos + size);
                                if (oid != 0) { 
                                    long prev = getPos(oid);
                                    int marker = (int)prev & dbFlagsMask;
                                    pool.copy(pos, prev - marker, size);
                                    setPos(oid, pos | marker | dbModifiedFlag);
                                }
                                pool.unfix(pg);
                                pg = putBitmapPage(i);
                                pg.data[offs] |= (byte)(((1<<objBitSize) - 1) << holeBitOffset);
                                pool.unfix(pg);
                                commitLocation();
                                return pos;
                            }
                            offs += 1;
                            if (lastHoleSize[mask] == 8) { 
                                holeBitSize += 8;
                            } else { 
                                holeBitSize = lastHoleSize[mask];
                            }
                        }
                        if (startOffs == 0 && holeBitSize == 0
                            && spaceNeeded < bitmapPageAvailableSpace[i]) 
                        {
                            bitmapPageAvailableSpace[i] = spaceNeeded;
                        }
                        offs = 0;
                        pool.unfix(pg);
                    }
                }
                if (firstPage == 0) { 
                    if (freeBitmapPage > i) { 
                        i = freeBitmapPage;
                        holeBitSize = holeBeforeFreePage;
                    }
                    objBitSize -= holeBitSize;
                    // number of bits reserved for the object and aligned on page boundary
                    int skip = (objBitSize + Page.pageSize/dbAllocationQuantum - 1) 
                        & ~(Page.pageSize/dbAllocationQuantum - 1);
                    // page aligned position after allocated object
                    pos = ((long)i << dbBitmapSegmentBits) + ((long)skip << dbAllocationQuantumBits);

                    long extension = (size > extensionQuantum) ? size : extensionQuantum;
                    int oldIndexSize = 0;
                    long oldIndex = 0;
                    int morePages = (int)((extension + Page.pageSize*(dbAllocationQuantum*8-1) - 1)
                                          / (Page.pageSize*(dbAllocationQuantum*8-1)));
                    if (i + morePages > dbLargeBitmapPages) { 
                        throw new StorageError(StorageError.NOT_ENOUGH_SPACE);
                    }
                    if (i <= dbBitmapPages && i + morePages > dbBitmapPages) {   
                        // We are out of space mapped by memory default allocation bitmap
                        oldIndexSize = header.root[curr].indexSize;
                        if (oldIndexSize <= currIndexSize + dbLargeBitmapPages - dbBitmapPages) {
                            int newIndexSize = oldIndexSize;
                            oldIndex = header.root[curr].index;
                            do { 
                                newIndexSize <<= 1;                    
                                if (newIndexSize < 0) { 
                                    newIndexSize = Integer.MAX_VALUE & ~(dbHandlesPerPage-1);
                                    if (newIndexSize < currIndexSize + dbLargeBitmapPages - dbBitmapPages) {
                                        throw new StorageError(StorageError.NOT_ENOUGH_SPACE);
                                    }
                                    break;
                                }
                            } while (newIndexSize <= currIndexSize + dbLargeBitmapPages - dbBitmapPages);

                            if (size + newIndexSize*8L > extensionQuantum) { 
                                extension = size + newIndexSize*8L;
                                morePages = (int)((extension + Page.pageSize*(dbAllocationQuantum*8-1) - 1)
                                                  / (Page.pageSize*(dbAllocationQuantum*8-1)));
                            }
                            extend(pos + (long)morePages*Page.pageSize + newIndexSize*8L);
                            long newIndex = pos + (long)morePages*Page.pageSize;                        
                            fillBitmap(pos + (skip>>3) + (long)morePages * (Page.pageSize/dbAllocationQuantum/8),
                                       newIndexSize >>> dbAllocationQuantumBits);
                            pool.copy(newIndex, oldIndex, oldIndexSize*8L);
                            header.root[curr].index = newIndex;
                            header.root[curr].indexSize = newIndexSize;
                        }
                        int[] newBitmapPageAvailableSpace = new int[dbLargeBitmapPages];
                        System.arraycopy(bitmapPageAvailableSpace, 0, newBitmapPageAvailableSpace, 0, dbBitmapPages);
                        for (j = dbBitmapPages; j < dbLargeBitmapPages; j++) { 
                            newBitmapPageAvailableSpace[j] = Integer.MAX_VALUE;
                        }
                        bitmapPageAvailableSpace = newBitmapPageAvailableSpace;
                        
                        for (j = 0; j < dbLargeBitmapPages - dbBitmapPages; j++) { 
                            setPos(currIndexSize + j, dbFreeHandleFlag);
                        }

                        header.root[curr].bitmapExtent = currIndexSize;
                        header.root[curr].indexUsed = currIndexSize += dbLargeBitmapPages - dbBitmapPages;
                    }
                    extend(pos + (long)morePages*Page.pageSize);
                    long adr = pos;
                    int len = objBitSize >> 3;
                    // fill bitmap pages used for allocation of object space with 0xFF 
                    while (len >= Page.pageSize) { 
                        pg = pool.putPage(adr);
                        memset(pg, 0, 0xFF, Page.pageSize);
                        pool.unfix(pg);
                        adr += Page.pageSize;
                        len -= Page.pageSize;
                    }
                    // fill part of last page responsible for allocation of object space
                    pg = pool.putPage(adr);
                    memset(pg, 0, 0xFF, len);
                    pg.data[len] = (byte)((1 << (objBitSize&7))-1);
                    pool.unfix(pg);

                    // mark in bitmap newly allocated object
                    fillBitmap(pos + (skip>>3), morePages * (Page.pageSize/dbAllocationQuantum/8));
                    
                    j = i;
                    while (--morePages >= 0) { 
                        setPos(getBitmapPageId(j++), pos | dbPageObjectFlag | dbModifiedFlag);
                        pos += Page.pageSize;
                    }
                    header.root[curr].bitmapEnd = j + dbBitmapId;
                    j = i + objBitSize / pageBits; 
                    if (alignment != 0) { 
                        currRBitmapPage = j;
                        currRBitmapOffs = 0;
                    } else { 
                        currPBitmapPage = j;
                        currPBitmapOffs = 0;
                    }
                    while (j > i) { 
                        bitmapPageAvailableSpace[--j] = 0;
                    }
                
                    pos = ((long)i*Page.pageSize*8 - holeBitSize)  << dbAllocationQuantumBits;
                    if (oid != 0) { 
                        long prev = getPos(oid);
                        int marker = (int)prev & dbFlagsMask;
                        pool.copy(pos, prev - marker, size);
                        setPos(oid, pos | marker | dbModifiedFlag);
                    }
                
                    if (holeBitSize != 0) { 
                        reserveLocation(pos, size);
                        while (holeBitSize > pageBits) { 
                            holeBitSize -= pageBits;
                            pg = putBitmapPage(--i);
                            memset(pg, 0, 0xFF, Page.pageSize);
                            bitmapPageAvailableSpace[i] = 0;
                            pool.unfix(pg);
                        }
                        pg = putBitmapPage(--i);
                        offs = Page.pageSize;
                        while ((holeBitSize -= 8) > 0) { 
                            pg.data[--offs] = (byte)0xFF; 
                        }
                        pg.data[offs-1] |= (byte)~((1 << -holeBitSize) - 1);
                        pool.unfix(pg);
                        commitLocation();
                    }
                    if (oldIndex != 0) { 
                        free(oldIndex, oldIndexSize*8L);
                    }
                    return pos;
                } 
                freeBitmapPage = i;
                holeBeforeFreePage = holeBitSize;
                holeBitSize = 0;
                lastPage = firstPage + 1;
                firstPage = 0;
                offs = 0;
            }
        }
    } 


    final void fillBitmap(long adr, int len) { 
        while (true) { 
            int off = (int)adr & (Page.pageSize-1);
            Page pg = pool.putPage(adr - off);
            if (Page.pageSize - off >= len) { 
                memset(pg, off, 0xFF, len);
                pool.unfix(pg);
                break;
            } else { 
                memset(pg, off, 0xFF, Page.pageSize - off);
                pool.unfix(pg);
                adr += Page.pageSize - off;
                len -= Page.pageSize - off;
            }
        }
    }

    final void free(long pos, long size)
    {
        synchronized (objectCache) {
            Assert.that(pos != 0 && (pos & (dbAllocationQuantum-1)) == 0);
            long quantNo = pos >>> dbAllocationQuantumBits;
            int  objBitSize = (int)((size+dbAllocationQuantum-1) >>> dbAllocationQuantumBits);
            int  pageId = (int)(quantNo >>> (Page.pageSizeLog+3));
            int  offs = (int)(quantNo & (Page.pageSize*8-1)) >> 3;
            Page pg = putBitmapPage(pageId);
            int  bitOffs = (int)quantNo & 7;

            usedSize -= (long)objBitSize << dbAllocationQuantumBits;

            if ((pos & (Page.pageSize-1)) == 0 && size >= Page.pageSize) { 
                if (pageId == currPBitmapPage && offs < currPBitmapOffs) { 
                    currPBitmapOffs = offs;
                }
            }
            if (pageId == currRBitmapPage && offs < currRBitmapOffs) { 
                currRBitmapOffs = offs;
            }
            bitmapPageAvailableSpace[pageId] = Integer.MAX_VALUE;
        
            if (objBitSize > 8 - bitOffs) { 
                objBitSize -= 8 - bitOffs;
                pg.data[offs++] &= (1 << bitOffs) - 1;
                while (objBitSize + offs*8 > Page.pageSize*8) { 
                    memset(pg, offs, 0, Page.pageSize - offs);
                    pool.unfix(pg);
                    pg = putBitmapPage(++pageId);
                    bitmapPageAvailableSpace[pageId] = Integer.MAX_VALUE;
                    objBitSize -= (Page.pageSize - offs)*8;
                    offs = 0;
                }
                while ((objBitSize -= 8) > 0) { 
                    pg.data[offs++] = (byte)0;
                }
                pg.data[offs] &= (byte)~((1 << (objBitSize + 8)) - 1);
            } else { 
                pg.data[offs] &= (byte)~(((1 << objBitSize) - 1) << bitOffs); 
            }
            pool.unfix(pg);
        }
    }

    static class CloneNode {
        long      pos;
        CloneNode next;

        CloneNode(long pos, CloneNode list) { 
            this.pos = pos;
            this.next = list;
        }
    }

    final void cloneBitmap(long pos, long size)
    {
        synchronized (objectCache) {
            if (insideCloneBitmap) { 
                Assert.that(size == Page.pageSize);
                cloneList = new CloneNode(pos, cloneList);
            } else { 
                insideCloneBitmap = true;
                while (true) { 
                    long quantNo = pos >>> dbAllocationQuantumBits;
                    int  objBitSize = (int)((size+dbAllocationQuantum-1) >>> dbAllocationQuantumBits);
                    int  pageId = (int)(quantNo >>> (Page.pageSizeLog + 3));
                    int  offs = (int)(quantNo & (Page.pageSize*8-1)) >> 3;
                    int  bitOffs = (int)quantNo & 7;
                    int  oid = getBitmapPageId(pageId);
                    pos = getPos(oid);
                    if ((pos & dbModifiedFlag) == 0) { 
                        dirtyPagesMap[oid >>> (dbHandlesPerPageBits+5)] 
                            |= 1 << ((oid >>> dbHandlesPerPageBits) & 31);
                        allocate(Page.pageSize, oid);
                        cloneBitmap(pos & ~dbFlagsMask, Page.pageSize);
                    }
                    
                    if (objBitSize > 8 - bitOffs) { 
                        objBitSize -= 8 - bitOffs;
                        offs += 1;
                        while (objBitSize + offs*8 > Page.pageSize*8) { 
                            oid = getBitmapPageId(++pageId);
                            pos = getPos(oid);
                            if ((pos & dbModifiedFlag) == 0) { 
                                dirtyPagesMap[oid >>> (dbHandlesPerPageBits+5)] 
                                    |= 1 << ((oid >>> dbHandlesPerPageBits) & 31);
                                allocate(Page.pageSize, oid);
                                cloneBitmap(pos & ~dbFlagsMask, Page.pageSize);
                            }
                            objBitSize -= (Page.pageSize - offs)*8;
                            offs = 0;
                        }
                    }
                    if (cloneList == null) { 
                        break;
                    }
                    pos = cloneList.pos;
                    size = Page.pageSize;
                    cloneList = cloneList.next;
                }
                insideCloneBitmap = false;
            }
        }
    }
        

    public void open(String filePath) {
        open(filePath, DEFAULT_PAGE_POOL_SIZE);
    }

    public void open(IFile file) {
        open(file, DEFAULT_PAGE_POOL_SIZE);
    }

    public synchronized void open(String filePath, int pagePoolSize) {
        SimpleFile file = FileFactory.createFile();
        file.open(filePath, readOnly, noFlush);      
        try {
            open(file, pagePoolSize);
        } catch (StorageError ex) {
            file.close();            
            throw ex;
        }
    }

    public synchronized void open(String filePath, int pagePoolSize, String cryptKey) {
        Rc4File file = new Rc4File(filePath, readOnly, noFlush, cryptKey);      
        try {
            open(file, pagePoolSize);
        } catch (StorageError ex) {
            file.close();            
            throw ex;
        }
    }
    

    protected OidHashTable createObjectCache() 
    { 
        if ("weak".equals(cacheKind)) { 
            try { 
                Class cls = Class.forName("org.garret.perst.impl.WeakHashTable");
                return (OidHashTable)cls.newInstance();
            } catch (Exception x) {}
        } else if ("lru".equals(cacheKind)) { 
            try { 
                Class cls = Class.forName("org.garret.perst.impl.LruObjectCache");
                return (OidHashTable)cls.newInstance();
            } catch (Exception x) {}
        }        
        return new StrongHashTable();
    }

    protected boolean isDirty() { 
        return header.dirty;
    }

    protected void initialize(IFile file, int pagePoolSize) 
    { 
        this.file = file;
        try { 
            classDescriptorClass = Class.forName("org.garret.perst.impl.ClassDescriptor");
        } catch (ClassNotFoundException x) {
            System.err.println(x);
        }
        reservedChain = null;
        cloneList = null;
        insideCloneBitmap = false;

        dirtyPagesMap = new int[dbDirtyPageBitmapSize/4+1];
        nNestedTransactions = 0;
        nBlockedTransactions = 0;
        nCommittedTransactions = 0;
        scheduledCommitTime = Long.MAX_VALUE;
        transactionMonitor = new Object();
        transactionLock = new PersistentResource();

        modified = false; 

        objectCache = createObjectCache();
        objectCache.init(objectCacheInitSize, objectCachePinCount);

        classDescMap = new Hashtable();
        descList = null;
        
        header = new Header();
        pool = PagePool.create(file, pagePoolSize); 
    }        

    public synchronized void open(IFile file, int pagePoolSize) {
        Page pg;
        int i;

        if (opened) {
            throw new StorageError(StorageError.STORAGE_ALREADY_OPENED);
        }
        initialize(file, pagePoolSize);


        byte[] buf = new byte[Page.pageSize];
        int rc = file.read(0, buf);
        if (rc > 0 && rc < Page.pageSize) { 
            throw new StorageError(StorageError.DATABASE_CORRUPTED);
        }
        header.unpack(buf);
        if (header.curr < 0 || header.curr > 1) { 
            throw new StorageError(StorageError.DATABASE_CORRUPTED);
        }
        transactionId = header.transactionId;
        if (header.databaseFormatVersion == 0) { // database not initialized
            int indexSize = initIndexSize;
            if (indexSize < dbFirstUserId) { 
                indexSize = dbFirstUserId;
            }
            indexSize = (indexSize + dbHandlesPerPage - 1) & ~(dbHandlesPerPage-1);

            bitmapExtentBase = dbBitmapPages;

            header.curr = currIndex = 0;
            long used = Page.pageSize;
            header.root[0].index = used;
            header.root[0].indexSize = indexSize;
            header.root[0].indexUsed = dbFirstUserId;
            header.root[0].freeList = 0;
            used += indexSize*8L;
            header.root[1].index = used;
            header.root[1].indexSize = indexSize;
            header.root[1].indexUsed = dbFirstUserId;
            header.root[1].freeList = 0;
            used += indexSize*8L;
        
            header.root[0].shadowIndex = header.root[1].index;
            header.root[1].shadowIndex = header.root[0].index;
            header.root[0].shadowIndexSize = indexSize;
            header.root[1].shadowIndexSize = indexSize;
            
            int bitmapPages = 
                (int)((used + Page.pageSize*(dbAllocationQuantum*8-1) - 1)
                      / (Page.pageSize*(dbAllocationQuantum*8-1)));
            long bitmapSize = (long)bitmapPages*Page.pageSize;
            int usedBitmapSize = (int)((used + bitmapSize) >>> (dbAllocationQuantumBits + 3));

            for (i = 0; i < bitmapPages; i++) { 
                pg = pool.putPage(used + (long)i*Page.pageSize);
                byte[] bitmap = pg.data;
                int n = usedBitmapSize > Page.pageSize ? Page.pageSize : usedBitmapSize;
                for (int j = 0; j < n; j++) { 
                    bitmap[j] = (byte)0xFF;
                }
                usedBitmapSize -= Page.pageSize;
                pool.unfix(pg);
            }
            int bitmapIndexSize = 
                ((dbBitmapId + dbBitmapPages)*8 + Page.pageSize - 1)
                & ~(Page.pageSize - 1);
            byte[] index = new byte[bitmapIndexSize];
            Bytes.pack8(index, dbInvalidId*8, dbFreeHandleFlag);
            for (i = 0; i < bitmapPages; i++) { 
                Bytes.pack8(index, (dbBitmapId+i)*8, used | dbPageObjectFlag);
                used += Page.pageSize;
            }
            header.root[0].bitmapEnd = dbBitmapId + i;
            header.root[1].bitmapEnd = dbBitmapId + i;
            while (i < dbBitmapPages) { 
                Bytes.pack8(index, (dbBitmapId+i)*8, dbFreeHandleFlag);
                i += 1;
            }
            header.root[0].size = used;
            header.root[1].size = used;
            usedSize = used;
            committedIndexSize = currIndexSize = dbFirstUserId;

            pool.write(header.root[1].index, index);
            pool.write(header.root[0].index, index);

            header.dirty = true;
            header.root[0].size = header.root[1].size;
            pg = pool.putPage(0);
            header.pack(pg.data);
            pool.flush();
            pool.modify(pg);
            header.databaseFormatVersion = dbDatabaseFormatVersion;
            header.pack(pg.data);
            pool.unfix(pg);
            pool.flush();
        } else {
            int curr = header.curr;
            currIndex = curr;
            if (header.root[curr].indexSize != header.root[curr].shadowIndexSize) {
                throw new StorageError(StorageError.DATABASE_CORRUPTED);
            }           
            bitmapExtentBase = (header.databaseFormatVersion < 2) ? 0 : dbBitmapPages;

           if (isDirty()) { 
                if (listener != null) {
                    listener.databaseCorrupted();
                }
                System.err.println("Database was not normally closed: start recovery");
                header.root[1-curr].size = header.root[curr].size;
                header.root[1-curr].indexUsed = header.root[curr].indexUsed; 
                header.root[1-curr].freeList = header.root[curr].freeList; 
                header.root[1-curr].index = header.root[curr].shadowIndex;
                header.root[1-curr].indexSize = header.root[curr].shadowIndexSize;
                header.root[1-curr].shadowIndex = header.root[curr].index;
                header.root[1-curr].shadowIndexSize = header.root[curr].indexSize;
                header.root[1-curr].bitmapEnd = header.root[curr].bitmapEnd;
                header.root[1-curr].rootObject = header.root[curr].rootObject;
                header.root[1-curr].classDescList = header.root[curr].classDescList;
                header.root[1-curr].bitmapExtent = header.root[curr].bitmapExtent;

                pg = pool.putPage(0);
                header.pack(pg.data);
                pool.unfix(pg);

                pool.copy(header.root[1-curr].index, header.root[curr].index, 
                          (header.root[curr].indexUsed*8L + Page.pageSize - 1) & ~(Page.pageSize-1));
                if (listener != null) {
                    listener.recoveryCompleted();
                }
                System.err.println("Recovery completed");
            } 
            currIndexSize = header.root[1-curr].indexUsed;
            committedIndexSize = currIndexSize;
            usedSize = header.root[curr].size;
        }
        int bitmapSize = header.root[1-currIndex].bitmapExtent == 0 ? dbBitmapPages : dbLargeBitmapPages;
        bitmapPageAvailableSpace = new int[bitmapSize];
        for (i = 0; i < bitmapPageAvailableSpace.length; i++) { 
            bitmapPageAvailableSpace[i] = Integer.MAX_VALUE;
        }        
        currRBitmapPage = currPBitmapPage = 0;
        currRBitmapOffs = currPBitmapOffs = 0;

        opened = true;
        reloadScheme();
    }

    public boolean isOpened() { 
        return opened;
    }

    void reloadScheme() {
        classDescMap.clear();
        int descListOid = header.root[1-currIndex].classDescList;
        classDescMap.put(classDescriptorClass.getName(), 
                         new ClassDescriptor(this, classDescriptorClass));
        if (descListOid != 0) {             
            ClassDescriptor desc;
            descList = findClassDescriptor(descListOid);
            for (desc = descList; desc != null; desc = desc.next) { 
                desc.load();
            }
            for (desc = descList; desc != null; desc = desc.next) { 
                if (classDescMap.get(desc.name) == desc) { 
                    desc.resolve();
                }
            }
        } else { 
            descList = null;
        }
    }

    final void assignOid(IPersistent obj, int oid) { 
        obj.assignOid(this, oid, false);
    }

    final void registerClassDescriptor(ClassDescriptor desc) { 
        classDescMap.put(desc.name, desc);
        desc.next = descList;
        descList = desc;
        storeObject0(desc);
        header.root[1-currIndex].classDescList = desc.getOid();
        modified = true;
    }        

    final ClassDescriptor getClassDescriptor(Class cls) { 
        ClassDescriptor desc = (ClassDescriptor)classDescMap.get(cls.getName());
        if (desc == null) { 
            desc = new ClassDescriptor(this, cls);
            registerClassDescriptor(desc);
        }
        return desc;
    }


    public synchronized IPersistent getRoot() {
        if (!opened) {
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        int rootOid = header.root[1-currIndex].rootObject;
        return (rootOid == 0) ? null : lookupObject(rootOid);
    }
    
    public synchronized void setRoot(IPersistent root) {
        if (!opened) {
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        if (root == null) { 
            header.root[1-currIndex].rootObject = 0;
        } else { 
            if (!root.isPersistent()) { 
                storeObject0(root);
            }
            header.root[1-currIndex].rootObject = root.getOid();
        }
        modified = true;
    }
 
    public void commit() {
        synchronized (this) { 
            if (!opened) {
                throw new StorageError(StorageError.STORAGE_NOT_OPENED);
            }
            objectCache.flush();
            
            if (modified) { 
                commit0();
                modified = false;
            }
            objectCache.clear();
        }
    }

    private final void commit0() 
    {
        int i, j, n;
        int curr = currIndex;
        int[] map = dirtyPagesMap;
        int oldIndexSize = header.root[curr].indexSize;
        int newIndexSize = header.root[1-curr].indexSize;
        int nPages = committedIndexSize >>> dbHandlesPerPageBits;
        Page pg;

        if (newIndexSize > oldIndexSize) { 
            cloneBitmap(header.root[curr].index, oldIndexSize*8L);
            long newIndex;
            while (true) { 
                newIndex = allocate(newIndexSize*8L, 0);
                if (newIndexSize == header.root[1-curr].indexSize) { 
                    break;
                }
                free(newIndex, newIndexSize*8L);
                newIndexSize = header.root[1-curr].indexSize;
            }
            header.root[1-curr].shadowIndex = newIndex;
            header.root[1-curr].shadowIndexSize = newIndexSize;
            free(header.root[curr].index, oldIndexSize*8L);
        }
        long currSize = header.root[1-curr].size;
        for (i = 0; i < nPages; i++) { 
            if ((map[i >> 5] & (1 << (i & 31))) != 0) { 
                Page srcIndex = pool.getPage(header.root[1-curr].index + (long)i*Page.pageSize);
                Page dstIndex = pool.getPage(header.root[curr].index + (long)i*Page.pageSize);
                for (j = 0; j < Page.pageSize; j += 8) {
                    long pos = Bytes.unpack8(dstIndex.data, j);
                    if (Bytes.unpack8(srcIndex.data, j) != pos && pos < currSize) { 
                        if ((pos & dbFreeHandleFlag) == 0) {
                            if ((pos & dbPageObjectFlag) != 0) {  
                                free(pos & ~dbFlagsMask, Page.pageSize);
                            } else if (pos != 0) { 
                                int offs = (int)pos & (Page.pageSize-1);
                                pg = pool.getPage(pos-offs);
                                free(pos, ObjectHeader.getSize(pg.data, offs));
                                pool.unfix(pg);
                            }
                        }
                    }
                }
                pool.unfix(srcIndex);
                pool.unfix(dstIndex);
            }
        }
        n = committedIndexSize & (dbHandlesPerPage-1);
        if (n != 0 && (map[i >> 5] & (1 << (i & 31))) != 0) { 
            Page srcIndex = pool.getPage(header.root[1-curr].index + (long)i*Page.pageSize);
            Page dstIndex = pool.getPage(header.root[curr].index + (long)i*Page.pageSize);
            j = 0;
            do { 
                long pos = Bytes.unpack8(dstIndex.data, j);
                if (Bytes.unpack8(srcIndex.data, j) != pos && pos < currSize) { 
                    if ((pos & dbFreeHandleFlag) == 0) {
                        if ((pos & dbPageObjectFlag) != 0) { 
                            free(pos & ~dbFlagsMask, Page.pageSize);
                        } else if (pos != 0) { 
                            int offs = (int)pos & (Page.pageSize-1);
                            pg = pool.getPage(pos - offs);
                            free(pos, ObjectHeader.getSize(pg.data, offs));
                            pool.unfix(pg);
                        }
                    }
                }
                j += 8;
            } while (--n != 0);
            pool.unfix(srcIndex);
            pool.unfix(dstIndex);
        }
        for (i = 0; i <= nPages; i++) { 
            if ((map[i >> 5] & (1 << (i & 31))) != 0) { 
                pg = pool.putPage(header.root[1-curr].index + (long)i*Page.pageSize);
                for (j = 0; j < Page.pageSize; j += 8) {
                    Bytes.pack8(pg.data, j, Bytes.unpack8(pg.data, j) & ~dbModifiedFlag);
                }
                pool.unfix(pg);
            }
        }
        if (currIndexSize > committedIndexSize) { 
            long page = (header.root[1-curr].index 
                         + committedIndexSize*8L) & ~(Page.pageSize-1);
            long end = (header.root[1-curr].index + Page.pageSize - 1
                        + currIndexSize*8L) & ~(Page.pageSize-1);
            while (page < end) { 
                pg = pool.putPage(page);
                for (j = 0; j < Page.pageSize; j += 8) {
                    Bytes.pack8(pg.data, j, Bytes.unpack8(pg.data, j) & ~dbModifiedFlag);
                }
                pool.unfix(pg);
                page += Page.pageSize;
            }
        }
        header.root[1-curr].usedSize = usedSize;
        pg = pool.putPage(0);
        header.pack(pg.data);
        pool.flush();
        pool.modify(pg);
        Assert.that(header.transactionId == transactionId);
        header.transactionId = ++transactionId;
        header.curr = curr ^= 1;
        header.dirty = true;
        header.pack(pg.data);
        pool.unfix(pg);
        pool.flush();
        header.root[1-curr].size = header.root[curr].size;
        header.root[1-curr].indexUsed = currIndexSize; 
        header.root[1-curr].freeList  = header.root[curr].freeList; 
        header.root[1-curr].bitmapEnd = header.root[curr].bitmapEnd; 
        header.root[1-curr].rootObject = header.root[curr].rootObject; 
        header.root[1-curr].classDescList = header.root[curr].classDescList; 
        header.root[1-curr].bitmapExtent = header.root[curr].bitmapExtent; 
        if (currIndexSize == 0 || newIndexSize != oldIndexSize) {
            header.root[1-curr].index = header.root[curr].shadowIndex;
            header.root[1-curr].indexSize = header.root[curr].shadowIndexSize;
            header.root[1-curr].shadowIndex = header.root[curr].index;
            header.root[1-curr].shadowIndexSize = header.root[curr].indexSize;
            pool.copy(header.root[1-curr].index, header.root[curr].index,
                      currIndexSize*8L);
            i = (currIndexSize+dbHandlesPerPage*32-1) >>> (dbHandlesPerPageBits+5);
            while (--i >= 0) { 
                map[i] = 0;
            }
        } else { 
            for (i = 0; i < nPages; i++) { 
                if ((map[i >> 5] & (1 << (i & 31))) != 0) { 
                    map[i >> 5] -= (1 << (i & 31));
                    pool.copy(header.root[1-curr].index + (long)i*Page.pageSize,
                              header.root[curr].index + (long)i*Page.pageSize,
                              Page.pageSize);
                }
            }
            if (currIndexSize > i*dbHandlesPerPage &&
                ((map[i >> 5] & (1 << (i & 31))) != 0
                 || currIndexSize != committedIndexSize))
            {
                pool.copy(header.root[1-curr].index + (long)i*Page.pageSize,
                          header.root[curr].index + (long)i*Page.pageSize,
                          8L*currIndexSize - (long)i*Page.pageSize);
                j = i>>>5;
                n = (currIndexSize + dbHandlesPerPage*32 - 1) >>> (dbHandlesPerPageBits+5); 
                while (j < n) { 
                    map[j++] = 0;
                }
            }
        }
        currIndex = curr;
        committedIndexSize = currIndexSize;
    }

    public synchronized void rollback() {
        if (!opened) {
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        objectCache.invalidate();
        if (!modified) { 
            return;
        }
        rollback0();
        modified = false;
    }

    private final void rollback0() {
        int curr = currIndex;
        int[] map = dirtyPagesMap;
        if (header.root[1-curr].index != header.root[curr].shadowIndex) { 
            pool.copy(header.root[curr].shadowIndex, header.root[curr].index, 8L*committedIndexSize);
        } else { 
            int nPages = (committedIndexSize + dbHandlesPerPage - 1) >>> dbHandlesPerPageBits;
            for (int i = 0; i < nPages; i++) { 
                if ((map[i >> 5] & (1 << (i & 31))) != 0) { 
                    pool.copy(header.root[curr].shadowIndex + (long)i*Page.pageSize,
                              header.root[curr].index + (long)i*Page.pageSize,
                              Page.pageSize);
                }
            }
        }
        for (int j = (currIndexSize+dbHandlesPerPage*32-1) >>> (dbHandlesPerPageBits+5);
             --j >= 0;
             map[j] = 0);
        header.root[1-curr].index = header.root[curr].shadowIndex;
        header.root[1-curr].indexSize = header.root[curr].shadowIndexSize;
        header.root[1-curr].indexUsed = committedIndexSize;
        header.root[1-curr].freeList  = header.root[curr].freeList; 
        header.root[1-curr].bitmapEnd = header.root[curr].bitmapEnd; 
        header.root[1-curr].size = header.root[curr].size;
        header.root[1-curr].rootObject = header.root[curr].rootObject;
        header.root[1-curr].classDescList = header.root[curr].classDescList;
        header.root[1-curr].bitmapExtent = header.root[curr].bitmapExtent;
        usedSize = header.root[curr].size;
        currIndexSize = committedIndexSize;
        currRBitmapPage = currPBitmapPage = 0;
        currRBitmapOffs = currPBitmapOffs = 0;
        reloadScheme();
    }

    public synchronized void backup(OutputStream out) throws java.io.IOException
    {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        objectCache.flush();

        int   curr = 1-currIndex;
        final int nObjects = header.root[curr].indexUsed;
        long  indexOffs = header.root[curr].index;
        int   i, j, k;
        int   nUsedIndexPages = (nObjects + dbHandlesPerPage - 1) / dbHandlesPerPage;
        int   nIndexPages = (int)((header.root[curr].indexSize + dbHandlesPerPage - 1) / dbHandlesPerPage);
        long  totalRecordsSize = 0;
        long  nPagedObjects = 0;
        int   bitmapExtent = header.root[curr].bitmapExtent;
        final long[] index = new long[nObjects];
        final int[]  oids = new int[nObjects];
            
        if (bitmapExtent == 0) { 
            bitmapExtent = Integer.MAX_VALUE;
        }
        for (i = 0, j = 0; i < nUsedIndexPages; i++) {
            Page pg = pool.getPage(indexOffs + (long)i*Page.pageSize);
            for (k = 0; k < dbHandlesPerPage && j < nObjects; k++, j++) { 
                long pos = Bytes.unpack8(pg.data, k*8);
                index[j] = pos;
                oids[j] = j;
                if ((pos & dbFreeHandleFlag) == 0) { 
                    if ((pos & dbPageObjectFlag) != 0) {
                        nPagedObjects += 1;
                    } else if (pos != 0) { 
                        int offs = (int)pos & (Page.pageSize-1);
                        Page op = pool.getPage(pos - offs);
                        int size = ObjectHeader.getSize(op.data, offs & ~dbFlagsMask);
                        size = (size + dbAllocationQuantum-1) & ~(dbAllocationQuantum-1);
                        totalRecordsSize += size; 
                        pool.unfix(op);
                    }
                }
            }
            pool.unfix(pg);
        
        } 
        Header newHeader = new Header();
        newHeader.curr = 0;
        newHeader.dirty = false;
        newHeader.databaseFormatVersion = header.databaseFormatVersion;
        long newFileSize = (long)(nPagedObjects + nIndexPages*2 + 1)*Page.pageSize + totalRecordsSize;
        newFileSize = (newFileSize + Page.pageSize-1) & ~(Page.pageSize-1);     
        newHeader.root = new RootPage[2];
        newHeader.root[0] = new RootPage();
        newHeader.root[1] = new RootPage();
        newHeader.root[0].size = newHeader.root[1].size = newFileSize;
        newHeader.root[0].index = newHeader.root[1].shadowIndex = Page.pageSize;
        newHeader.root[0].shadowIndex = newHeader.root[1].index = Page.pageSize + (long)nIndexPages*Page.pageSize;
        newHeader.root[0].shadowIndexSize = newHeader.root[0].indexSize = 
            newHeader.root[1].shadowIndexSize = newHeader.root[1].indexSize = nIndexPages*dbHandlesPerPage;
        newHeader.root[0].indexUsed = newHeader.root[1].indexUsed = nObjects;
        newHeader.root[0].freeList = newHeader.root[1].freeList = header.root[curr].freeList;
        newHeader.root[0].bitmapEnd = newHeader.root[1].bitmapEnd = header.root[curr].bitmapEnd;

        newHeader.root[0].rootObject = newHeader.root[1].rootObject = header.root[curr].rootObject;
        newHeader.root[0].classDescList = newHeader.root[1].classDescList = header.root[curr].classDescList;
        newHeader.root[0].bitmapExtent = newHeader.root[1].bitmapExtent = header.root[curr].bitmapExtent;
        byte[] page = new byte[Page.pageSize];
        newHeader.pack(page);
        out.write(page);
        
        long pageOffs = (long)(nIndexPages*2 + 1)*Page.pageSize;
        long recOffs = (long)(nPagedObjects + nIndexPages*2 + 1)*Page.pageSize;
        GenericSort.sort(new GenericSortArray() { 
                public int size() { 
                    return nObjects;
                }
                public int compare(int i, int j) { 
                    return index[i] < index[j] ? -1 : index[i] == index[j] ? 0 : 1;
                }
                public void swap(int i, int j) { 
                    long t1 = index[i];
                    index[i] = index[j];
                    index[j] = t1;
                    int t2 = oids[i];
                    oids[i] = oids[j];
                    oids[j] = t2;
                }
            }
        );
        byte[] newIndex = new byte[nIndexPages*dbHandlesPerPage*8];
        for (i = 0; i < nObjects; i++) {
            long pos = index[i];
            int oid = oids[i];
            if ((pos & dbFreeHandleFlag) == 0) { 
                if ((pos & dbPageObjectFlag) != 0) {
                    Bytes.pack8(newIndex, oid*8, pageOffs | dbPageObjectFlag);
                    pageOffs += Page.pageSize;
                } else if (pos != 0) { 
                    Bytes.pack8(newIndex, oid*8, recOffs);
                    int offs = (int)pos & (Page.pageSize-1);
                    Page op = pool.getPage(pos - offs);
                    int size = ObjectHeader.getSize(op.data, offs & ~dbFlagsMask);
                    size = (size + dbAllocationQuantum-1) & ~(dbAllocationQuantum-1);
                    recOffs += size; 
                    pool.unfix(op);
                }
            } else { 
                Bytes.pack8(newIndex, oid*8, pos);
            }
        }
        out.write(newIndex);
        out.write(newIndex);

        for (i = 0; i < nObjects; i++) {
            long pos = index[i];
            if (((int)pos & (dbFreeHandleFlag|dbPageObjectFlag)) == dbPageObjectFlag) { 
                if (oids[i] < dbBitmapId + dbBitmapPages 
                    || (oids[i] >= bitmapExtent && oids[i] < bitmapExtent + dbLargeBitmapPages - dbBitmapPages))
                { 
                    int pageId = oids[i] < dbBitmapId + dbBitmapPages 
                        ? oids[i] - dbBitmapId : oids[i] - bitmapExtent + bitmapExtentBase;
                    long mappedSpace = (long)pageId*Page.pageSize*8*dbAllocationQuantum;
                    if (mappedSpace >= newFileSize) { 
                        Arrays.fill(page, (byte)0);
                    } else if (mappedSpace + Page.pageSize*8*dbAllocationQuantum <= newFileSize) { 
                        Arrays.fill(page, (byte)-1);
                    } else { 
                        int nBits = (int)((newFileSize - mappedSpace) >> dbAllocationQuantumBits);
                        Arrays.fill(page, 0, nBits >> 3, (byte)-1);
                        page[nBits >> 3] = (byte)((1 << (nBits & 7)) - 1);
                        Arrays.fill(page, (nBits >> 3) + 1, Page.pageSize, (byte)0);
                    }
                    out.write(page);
                } else {                        
                    Page pg = pool.getPage(pos & ~dbFlagsMask);
                    out.write(pg.data);
                    pool.unfix(pg);
                }
            }
        }
        for (i = 0; i < nObjects; i++) {
            long pos = index[i];
            if (pos != 0 && ((int)pos & (dbFreeHandleFlag|dbPageObjectFlag)) == 0) { 
                pos &= ~dbFlagsMask;
                int offs = (int)pos & (Page.pageSize-1);
                Page pg = pool.getPage(pos - offs);
                int size = ObjectHeader.getSize(pg.data, offs);
                size = (size + dbAllocationQuantum-1) & ~(dbAllocationQuantum-1);

                while (true) { 
                    if (Page.pageSize - offs >= size) { 
                        out.write(pg.data, offs, size);
                        break;
                    }
                    out.write(pg.data, offs, Page.pageSize - offs);
                    size -= Page.pageSize - offs;
                    pos += Page.pageSize - offs;
                    offs = 0;
                    pool.unfix(pg); 
                    pg = pool.getPage(pos);
                }
                pool.unfix(pg);
            }
        }
        if (recOffs != newFileSize) {       
            Assert.that(newFileSize - recOffs < Page.pageSize);
            int align = (int)(newFileSize - recOffs);
            Arrays.fill(page, 0, align, (byte)0);
            out.write(page, 0, align);
        }        
    }

    public IPersistentList createList() {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        return new PersistentListImpl(this);
    }

    public synchronized IPersistentSet createSet() {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        IPersistentSet set = alternativeBtree 
            ? (IPersistentSet)new AltPersistentSet()
            : (IPersistentSet)new PersistentSet();
        set.assignOid(this, 0, false);
        return set;
    }

    public synchronized IPersistentSet createScalableSet() {
        return createScalableSet(8);
    }

    public synchronized IPersistentSet createScalableSet(int initialSize) {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        return new ScalableSet(this, initialSize);
    }

    public synchronized Index createIndex(int keyType, boolean unique) {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        Index index = alternativeBtree 
            ? (Index)new AltBtree(keyType, unique)
            : (Index)new Btree(keyType, unique);
        index.assignOid(this, 0, false);
        return index;
    }

    public synchronized Index createIndex(int[] keyTypes, boolean unique) {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        if (alternativeBtree) { 
            throw new StorageError(StorageError.UNSUPPORTED_INDEX_TYPE);
        }            
        Index index = new BtreeCompoundIndex(keyTypes, unique);
        index.assignOid(this, 0, false);
        return index;
    }

    public synchronized MultidimensionalIndex createMultidimensionalIndex(MultidimensionalComparator comparator)
    {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        return new KDTree(this, comparator);
    }      

    public synchronized Index createThickIndex(int keyType) {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        return new ThickIndex(keyType, this);
    }      

    public synchronized BitIndex createBitIndex() {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        BitIndex index = new BitIndexImpl();
        index.assignOid(this, 0, false);
        return index;
    }

    public synchronized SpatialIndex createSpatialIndex() {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        return new Rtree();
    }

    public synchronized SpatialIndexR2 createSpatialIndexR2() {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        SpatialIndexR2 index = new RtreeR2();
        index.assignOid(this, 0, false);
        return index;
    }

    public synchronized Index createRandomAccessIndex(int keyType, boolean unique) {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        Index index = new RndBtree(keyType, unique);
        index.assignOid(this, 0, false);
        return index;
    }

    public SortedCollection createSortedCollection(PersistentComparator comparator, boolean unique) {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }        
        return new Ttree(comparator, unique);
    }
        
    public SortedCollection createSortedCollection(boolean unique) {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }        
        return new Ttree(new DefaultPersistentComparator(), unique);
    }
        
    public Link createLink() {
        return createLink(8);
    }

    public Link createLink(int initialSize) {
        return new LinkImpl(initialSize);
    }

    public Relation createRelation(IPersistent owner) {
        return new RelationImpl(owner);
    }

    public Blob createBlob() { 
        return new BlobImpl(this, Page.pageSize - BlobImpl.headerSize);
    }

    public TimeSeries createTimeSeries(String blockClass, long maxBlockTimeInterval) {
        return new TimeSeriesImpl(this, blockClass, maxBlockTimeInterval);
    }

    public PatriciaTrie createPatriciaTrie() { 
        return new PTrie();
    }

    /**
     * This method is used internally by Perst to get transaction context associated with current thread.
     * But it can be also used by application to get transaction context, store it in some variable and
     * use in another thread. I will make it possible to share one transaction between multiple threads.
     * @return transaction context associated with current thread
     */     
    public ThreadTransactionContext getTransactionContext() { 
        Thread self = Thread.currentThread();
        ThreadTransactionContext ctx = (ThreadTransactionContext)transactionContext.get(self);
        if (ctx == null) { 
            ctx = new ThreadTransactionContext();
            transactionContext.put(self, ctx);
        }
        return ctx;
    }

    /**
     * Associate transaction context with the thread
     * This method can be used by application to share the same transaction between multiple threads
     * @param ctx new transaction context 
     * @return transaction context previously associated with this thread
     */     
    public ThreadTransactionContext setTransactionContext(ThreadTransactionContext ctx) { 
        Thread self = Thread.currentThread();
        ThreadTransactionContext oldCtx = (ThreadTransactionContext)transactionContext.get(self);
        transactionContext.put(self, ctx);
        return oldCtx;
    }

    public void beginThreadTransaction(int mode)
    {
        switch (mode) {
        case SERIALIZABLE_TRANSACTION:
            useSerializableTransactions = true;
            getTransactionContext().nested += 1;
            break;
        case EXCLUSIVE_TRANSACTION:
        case COOPERATIVE_TRANSACTION:
            synchronized (transactionMonitor) {
                if (scheduledCommitTime != Long.MAX_VALUE) { 
                    nBlockedTransactions += 1;
                    while (System.currentTimeMillis() >= scheduledCommitTime) { 
                        try { 
                            transactionMonitor.wait();
                        } catch (InterruptedException x) {}
                    }
                    nBlockedTransactions -= 1;
                }
                nNestedTransactions += 1;
            }           
            if (mode == EXCLUSIVE_TRANSACTION) { 
                transactionLock.exclusiveLock();
            } else { 
                transactionLock.sharedLock();
            }
            break;
        default:
            throw new IllegalArgumentException("Illegal transaction mode");
        }
    }

    public void endThreadTransaction() { 
        endThreadTransaction(Integer.MAX_VALUE);
    }

    public void endThreadTransaction(int maxDelay)
    {
        ThreadTransactionContext ctx = getTransactionContext();
        if (ctx.nested != 0) { // serializable transaction
            if (--ctx.nested == 0) { 
                ArrayList modified = ctx.modified;
                ArrayList deleted = ctx.deleted;
                Hashtable locked = ctx.locked;
                synchronized (this) { 
                    synchronized (objectCache) { 
                        for (int i = modified.size(); --i >= 0;) { 
                            ((IPersistent)modified.get(i)).store();
                        } 
                        for (int i = deleted.size(); --i >= 0;) { 
                            deallocateObject0((IPersistent)deleted.get(i));
                        } 
                        if (modified.size() + deleted.size() > 0) { 
                            commit0();
                        }
                    }
                }
                Enumeration iterator = locked.elements();
                while (iterator.hasMoreElements()) { 
                    ((IResource)iterator.nextElement()).reset();
                }
                modified.clear();
                deleted.clear();
                locked.clear();
            } 
        } else { // exclusive or cooperative transaction        
            synchronized (transactionMonitor) { 
                transactionLock.unlock();
                
                if (nNestedTransactions != 0) { // may be everything is already aborted
                    if (--nNestedTransactions == 0) { 
                        nCommittedTransactions += 1;
                        commit();
                        scheduledCommitTime = Long.MAX_VALUE;
                        if (nBlockedTransactions != 0) { 
                            transactionMonitor.notifyAll();
                        }
                    } else {
                        if (maxDelay != Integer.MAX_VALUE) { 
                            long nextCommit = System.currentTimeMillis() + maxDelay;
                            if (nextCommit < scheduledCommitTime) { 
                                scheduledCommitTime = nextCommit;
                            }
                            if (maxDelay == 0) { 
                                int n = nCommittedTransactions;
                                nBlockedTransactions += 1;
                                do { 
                                    try { 
                                        transactionMonitor.wait();
                                    } catch (InterruptedException x) {}
                                } while (nCommittedTransactions == n);
                                nBlockedTransactions -= 1;
                            }                                   
                        }
                    }
                }
            }
        }
    }


    public void rollbackThreadTransaction()
    {
        ThreadTransactionContext ctx = getTransactionContext();
        if (ctx.nested != 0) { // serializable transaction
            ArrayList modified = ctx.modified;
            Hashtable locked = ctx.locked;
            synchronized (this) { 
                synchronized (objectCache) {
                    int i = modified.size();
                    while (--i >= 0) { 
                        IPersistent obj = (IPersistent)modified.get(i);
                        int oid = obj.getOid();
                        if (getPos(oid) == 0) {
                            freeId(oid);
                        }
                        obj.invalidate();
                    }
                }
            }
            Enumeration iterator = locked.elements();
            while (iterator.hasMoreElements()) { 
                ((IResource)iterator.nextElement()).reset();
            }
            ctx.nested = 0; 
            modified.clear();
            ctx.deleted.clear();
            locked.clear();
        } else { 
            synchronized (transactionMonitor) { 
                transactionLock.reset();
                nNestedTransactions = 0;
                if (nBlockedTransactions != 0) { 
                    transactionMonitor.notifyAll();
                }
                rollback();
            }
        }
    }
         
    public boolean lockObject(IPersistent obj) { 
        if (useSerializableTransactions) { 
            ThreadTransactionContext ctx = getTransactionContext();
            if (ctx.nested != 0) { // serializable transaction
                return ctx.locked.put(new ThreadTransactionContext.Proxy(obj), obj) == null;
            }
        }
        return true;
    }
         
    public void close() 
    {
        commit();
        opened = false;
        if (isDirty()) { 
            Page pg = pool.putPage(0);
            header.pack(pg.data);
            pool.flush();
            pool.modify(pg);
            header.dirty = false;
            header.pack(pg.data);
            pool.unfix(pg);
            pool.flush();
        }
        pool.close();
        // make GC easier
        pool = null;
        objectCache = null;
        classDescMap = null;
        bitmapPageAvailableSpace = null;
        dirtyPagesMap  = null;
        descList = null;
    }

    private boolean getBooleanValue(Object value) { 
        if (value instanceof Boolean) { 
            return ((Boolean)value).booleanValue();
        } else if (value instanceof String) {
            String s = (String)value;
            if ("true".equalsIgnoreCase(s) || "t".equalsIgnoreCase(s) || "1".equals(s)) { 
                return true;
            } else if ("false".equalsIgnoreCase(s) || "f".equalsIgnoreCase(s) || "0".equals(s)) { 
                return false;
            }
        }
        throw new StorageError(StorageError.BAD_PROPERTY_VALUE);
    }

    private long getIntegerValue(Object value) { 
        if (value instanceof String) {
            try { 
                return Long.parseLong((String)value, 10);
            } catch (NumberFormatException x) {}
        } else if (value instanceof Integer) { 
            return ((Integer)value).intValue();
        } else if (value instanceof Long) { 
            return ((Integer)value).longValue();
        } 
        throw new StorageError(StorageError.BAD_PROPERTY_VALUE);
    }

     
    public void setProperties(Hashtable props) 
    {
        String value;
        Enumeration e = props.keys();
        while (e.hasMoreElements()) { 
            Object key = e.nextElement();
            properties.put(key, props.get(key));
        }
        if ((value = (String)props.get("perst.object.cache.kind")) != null) { 
            cacheKind = value;
        }        
        if ((value = (String)props.get("perst.object.cache.init.size")) != null) { 
            objectCacheInitSize = (int)getIntegerValue(value);
        }
        if ((value = (String)props.get("perst.object.cache.pin.count")) != null) { 
            objectCachePinCount = (int)getIntegerValue(value);
        }
        if ((value = (String)props.get("perst.object.index.init.size")) != null) { 
            initIndexSize = (int)getIntegerValue(value);
        }
        if ((value = (String)props.get("perst.extension.quantum")) != null) { 
            extensionQuantum = getIntegerValue(value);
        } 
        if ((value = (String)props.get("perst.file.readonly")) != null) { 
            readOnly = getBooleanValue(value);
        }
        if ((value = (String)props.get("perst.file.noflush")) != null) { 
            noFlush = getBooleanValue(value);
        }
        if ((value = (String)props.get("perst.alternative.btree")) != null) { 
            alternativeBtree = getBooleanValue(value);
        }
        if ((value = (String)props.get("perst.string.encoding")) != null) { 
            encoding = value;
        }
        if ((value = (String)props.get("perst.concurrent.iterator")) != null) { 
            concurrentIterator = getBooleanValue(value);
        }
        if ((value = (String)props.get("perst.force.store")) != null) { 
            forceStore = getBooleanValue(value);
        }
    }

    public void setProperty(String name, Object value)
    {
        properties.put(name, value);
        if (name.equals("perst.object.cache.init.size")) { 
            objectCacheInitSize = (int)getIntegerValue(value);
        } else if (name.equals("perst.object.cache.kind")) { 
            cacheKind = (String)value;
        } else if (name.equals("perst.object.cache.pin.count")) { 
            objectCachePinCount = (int)getIntegerValue(value);
        } else if (name.equals("perst.object.index.init.size")) { 
            initIndexSize = (int)getIntegerValue(value);
        } else if (name.equals("perst.extension.quantum")) { 
            extensionQuantum = getIntegerValue(value);
        } else if (name.equals("perst.file.readonly")) { 
            readOnly = getBooleanValue(value);
        } else if (name.equals("perst.file.noflush")) { 
            noFlush = getBooleanValue(value);
        } else if (name.equals("perst.alternative.btree")) { 
            alternativeBtree = getBooleanValue(value);
        } else if (name.equals("perst.string.encoding")) { 
            encoding = (value == null) ? null : value.toString();
        } else if (name.equals("perst.concurrent.iterator")) { 
            concurrentIterator = getBooleanValue(value);
        } else if (name.equals("perst.force.store")) { 
            forceStore = getBooleanValue(value);
        } else { 
            throw new StorageError(StorageError.NO_SUCH_PROPERTY);
        }
    }

    public Object getProperty(String name)
    {
        return properties.get(name);
    }

    public Hashtable getProperties()
    {
        return properties;
    }
    
    public StorageListener setListener(StorageListener listener)
    {
        StorageListener prevListener = this.listener;
        this.listener = listener;
        return prevListener;
    }

    public synchronized IPersistent getObjectByOID(int oid)
    {
        return oid == 0 ? null : lookupObject(oid);
    }

    public synchronized void modifyObject(IPersistent obj) {
        synchronized(objectCache) { 
            if (!obj.isModified()) { 
                if (useSerializableTransactions) { 
                    ThreadTransactionContext ctx = getTransactionContext();
                    if (ctx.nested != 0) { // serializable transaction
                        ctx.modified.add(obj);
                        return;
                    }
                }
                objectCache.setDirty(obj);
            }
        }
    }
    

    public void throwObject(IPersistent obj) 
    {
        if (obj != null) { 
            objectCache.remove(obj.getOid());
        }
    }

    public synchronized void storeObject(IPersistent obj) 
    {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        synchronized (objectCache) { 
            storeObject0(obj);
        }
    }



    public synchronized int makePersistent(IPersistent obj) 
    {
        if (!opened) { 
            throw new StorageError(StorageError.STORAGE_NOT_OPENED);
        }
        if (obj == null) {
            return 0;
        }
        int oid = obj.getOid();
        if (oid != 0) {
            return oid;
        }
        if (forceStore && (!useSerializableTransactions || getTransactionContext().nested == 0)) {
            synchronized (objectCache) { 
                storeObject0(obj);
            }
            return obj.getOid();
        } else { 
            synchronized (objectCache) {
                oid = allocateId();
                obj.assignOid(this, oid, false);
                setPos(oid, 0);
                objectCache.put(oid, obj);
                obj.modify();
                return oid;
            }
        }
    }

    private final void storeObject0(IPersistent obj) 
    {
        obj.onStore();
        int oid = obj.getOid();
        boolean newObject = false;
        if (oid == 0) { 
            oid = allocateId();
            objectCache.put(oid, obj);
            obj.assignOid(this, oid, false);
            newObject = true;
        } else if (obj.isModified()) {
            objectCache.clearDirty(obj);
        }
        byte[] data = packObject(obj);
        long pos;
        int newSize = ObjectHeader.getSize(data, 0);
        if (newObject || (pos = getPos(oid)) == 0) { 
            pos = allocate(newSize, 0);
            setPos(oid, pos | dbModifiedFlag);
        } else {
            int offs = (int)pos & (Page.pageSize-1);
            if ((offs & (dbFreeHandleFlag|dbPageObjectFlag)) != 0) { 
                throw new StorageError(StorageError.DELETED_OBJECT);
            }
            Page pg = pool.getPage(pos - offs);
            int size = ObjectHeader.getSize(pg.data, offs & ~dbFlagsMask);
            pool.unfix(pg);
            if ((pos & dbModifiedFlag) == 0) { 
                cloneBitmap(pos & ~dbFlagsMask, size);
                pos = allocate(newSize, 0);
                setPos(oid, pos | dbModifiedFlag);
            } else {
                pos &= ~dbFlagsMask;
                if (newSize != size) { 
                    if (((newSize + dbAllocationQuantum - 1) & ~(dbAllocationQuantum-1))
                        > ((size + dbAllocationQuantum - 1) & ~(dbAllocationQuantum-1)))
                    { 
                        long newPos = allocate(newSize, 0);
                        cloneBitmap(pos, size);
                        free(pos, size);
                        pos = newPos;
                        setPos(oid, pos | dbModifiedFlag);
                    } else if (newSize < size) { 
                        ObjectHeader.setSize(data, 0, size);
                    }
                }
            }
        }        
        modified = true;
        pool.put(pos, data, newSize);
    }

    public synchronized void loadObject(IPersistent obj) {
        if (obj.isRaw()) { 
            loadStub(obj.getOid(), obj);
        }
    }

    final synchronized IPersistent lookupObject(int oid) {
        IPersistent obj = objectCache.get(oid);
        if (obj == null || obj.isRaw()) { 
            obj = loadStub(oid, obj);
        }
        return obj;
    }
 
    protected int swizzle(IPersistent obj) { 
        int oid = 0;
        if (obj != null) { 
            if (!obj.isPersistent()) { 
                storeObject0(obj);
            }
            oid = obj.getOid();
        }
        return oid;
    }
        
    final ClassDescriptor findClassDescriptor(int oid) { 
        return (ClassDescriptor)lookupObject(oid);
    }

    protected IPersistent unswizzle(int oid, boolean recursiveLoading) { 
        if (oid == 0) { 
            return null;
        } 
        if (recursiveLoading) {
            return lookupObject(oid);
        }
        IPersistent stub = objectCache.get(oid);
        if (stub != null) { 
            return stub;
        }
        long pos = getPos(oid);
        int offs = (int)pos & (Page.pageSize-1);
        if ((offs & (dbFreeHandleFlag|dbPageObjectFlag)) != 0) { 
            throw new StorageError(StorageError.DELETED_OBJECT);
        }
        Page pg = pool.getPage(pos - offs);
        int typeOid = ObjectHeader.getType(pg.data, offs & ~dbFlagsMask);
        pool.unfix(pg);
        if (typeOid == 0) { 
            stub = new ClassDescriptor();
        } else { 
            ClassDescriptor desc = findClassDescriptor(typeOid);
            stub = (IPersistent)desc.newInstance();
        } 
        stub.assignOid(this, oid, true);
        objectCache.put(oid, stub);
        return stub;
    }

    class PerstObjectInputStream implements IInputStream { 
        byte[]  arr;        
        int     pos;
        IPersistent obj;

        PerstObjectInputStream(IPersistent po, byte[] rec, int offs) { 
            obj = po;
            arr = rec;
            pos = offs;
        }
        
        
        protected void check(int type) { 
            if (arr[pos++] != type) { 
                throw new StorageError(StorageError.UNEXPECTED_TYPE, "expected field type " + Types.getSignature(type) + ", actual type " + Types.getSignature(arr[pos-1]));
            } 
        }

        public boolean readBoolean() { 
            check(Types.Boolean);
            return arr[pos++] != 0;
        }

        public byte readByte() { 
            check(Types.Byte);
            return arr[pos++];
        }

        public char readChar() {
            check(Types.Char);
            char v = (char)Bytes.unpack2(arr, pos);
            pos += 2;
            return v;
        }

        public short readShort() { 
            check(Types.Short);
            short v = Bytes.unpack2(arr, pos);
            pos += 2;
            return v;
        }

        public int readInt() { 
            check(Types.Int);
            int v = Bytes.unpack4(arr, pos);
            pos += 4;
            return v;
        }
        
        public long readLong() { 
            check(Types.Long);
            long v = Bytes.unpack8(arr, pos);
            pos += 8;
            return v;
        }

        public float readFloat() { 
            check(Types.Float);
            float v = Bytes.unpackF4(arr, pos);
            pos += 4;
            return v;
        }
        
        public double readDouble() { 
            check(Types.Double);
            double v = Bytes.unpackF8(arr, pos);
            pos += 8;
            return v;
        }

        public String readString() {            
            check(Types.String);
            return readStringBody();
        }

        private String readStringBody() {
            int len = Bytes.unpack2(arr, pos);  
            String s = null;
            pos += 2;
            if (len >= 0) { 
                char[] chars = new char[len];
                for (int i = 0; i < len; i++) { 
                    chars[i] = (char)Bytes.unpack2(arr, pos);
                    pos += 2;
                }
                s = new String(chars);
            } else if (len < -1) { 
                if (encoding != null) { 
                    try { 
                        s = new String(arr, pos, -len-2, encoding);
                    } catch (UnsupportedEncodingException x) { 
                        throw new StorageError(StorageError.UNSUPPORTED_ENCODING);
                    }
                } else { 
                    s = new String(arr, pos, -len-2);
                }
                pos += -len-2;                
            }
            return s;
        }

        public Date readDate() { 
            check(Types.Date);
            Date v = new Date(Bytes.unpack8(arr, pos));
            pos += 8;
            return v;
        }

        public IPersistent readObject() { 
            check(Types.Object);
            int oid = Bytes.unpack4(arr, pos);
            pos += 4;
            return unswizzle(oid, obj.recursiveLoading());
        }
        
        public Link readLink() {
            check(Types.Link);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return null;
            }
            IPersistent[] v = new IPersistent[size];
            for (int j = 0; j < size; j++) { 
                int elemOid = Bytes.unpack4(arr, pos);
                pos += 4;
                if (elemOid != 0) { 
                    v[j] = new PersistentStub(StorageImpl.this, elemOid);
                }
            }
            return new LinkImpl(v, obj);
        }

        public  boolean[] readArrayOfBoolean() { 
            check(Types.ArrayOfBoolean);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return null;
            }
            boolean[] v = new boolean[size];
            int j = pos;
            for (int i = 0; i < size; i++) { 
                v[i] = arr[j++] != 0;
            }
            pos = j;
            return v;
        }
        
        public byte[] readArrayOfByte() {
            check(Types.ArrayOfByte);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return null;
            }
            byte[] v = new byte[size];
            System.arraycopy(arr, pos, v, 0, size);
            pos += size;
            return v;
        }
        

        public char[] readArrayOfChar() {
            check(Types.ArrayOfChar);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return null;
            }
            char[] v = new char[size];
            int j = pos;
            for (int i = 0; i < size; i++) { 
                v[i] = (char)Bytes.unpack2(arr, j);
                j += 2;
            }
            pos = j;
            return v;
        }
        
        public short[] readArrayOfShort() { 
            check(Types.ArrayOfShort);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return null;
            }
            short[] v = new short[size];
            int j = pos;
            for (int i = 0; i < size; i++) { 
                v[i] = Bytes.unpack2(arr, j);
                j += 2;
            }
            pos = j;
            return v;
        }
        
        public int[] readArrayOfInt() {
            check(Types.ArrayOfInt);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return null;
            }
            int[] v = new int[size];
            int j = pos;
            for (int i = 0; i < size; i++) { 
                v[i] = Bytes.unpack4(arr, j);
                j += 4;
            }
            pos = j;
            return v;
        }
        
        public long[] readArrayOfLong() { 
            check(Types.ArrayOfLong);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return null;
            }
            long[] v = new long[size];
            int j = pos;
            for (int i = 0; i < size; i++) { 
                v[i] = Bytes.unpack8(arr, j);
                j += 8;
            }
            pos = j;
            return v;
        }
        
        public float[] readArrayOfFloat() {
            check(Types.ArrayOfFloat);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return null;
            }
            float[] v = new float[size];
            int j = pos;
            for (int i = 0; i < size; i++) { 
                v[i] = Bytes.unpackF4(arr, j);
                j += 4;
            }
            pos = j;
            return v;
        }
        
        public double[] readArrayOfDouble() { 
            check(Types.ArrayOfDouble);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return null;
            }
            double[] v = new double[size];
            int j = pos;
            for (int i = 0; i < size; i++) { 
                v[i] = Bytes.unpackF8(arr, j);
                j += 8;
            }
            pos = j;
            return v;
        }
        
        public String[] readArrayOfString() { 
            check(Types.ArrayOfString);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return null;
            }
            String[] v = new String[size];
            for (int i = 0; i < size; i++) { 
                v[i] = readStringBody();
            }
            return v;
        }
 
        public Date[] readArrayOfDate() { 
            check(Types.ArrayOfDate);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return null;
            }
            Date[] v = new Date[size];
            int j = pos;
            for (int i = 0; i < size; i++) { 
                v[i] = new Date(Bytes.unpack8(arr, j));
                j += 8;
            }
            pos = j;
            return v;
        }
 
        public int readArrayOfObject(IPersistent[] v) { 
            check(Types.ArrayOfObject);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return -1;
            }
            if (size > v.length) {
                throw new IllegalArgumentException();
            }
            for (int i = 0; i < size; i++) { 
                v[i] = readObject();
            }
            return size;
        }
            
        public IPersistent[] readArrayOfObject() { 
            check(Types.ArrayOfObject);
            int size = Bytes.unpack4(arr, pos);
            pos += 4;
            if (size < 0) { 
                return null;
            }
            IPersistent[] v = new IPersistent[size];
            for (int i = 0; i < size; i++) { 
                v[i] = readObject();
            }
            return v;
        }
    }
        

    final IPersistent loadStub(int oid, IPersistent obj)
    {
        long pos = getPos(oid);
        if ((pos & (dbFreeHandleFlag|dbPageObjectFlag)) != 0) { 
            throw new StorageError(StorageError.DELETED_OBJECT);
        }
        byte[] body = pool.get(pos & ~dbFlagsMask);
        ClassDescriptor desc;
        int typeOid = ObjectHeader.getType(body, 0);
        if (typeOid == 0) { 
            desc = (ClassDescriptor)classDescMap.get(classDescriptorClass.getName());
        } else { 
            desc = findClassDescriptor(typeOid);
        }
        if (obj == null) { 
            obj = (IPersistent)desc.newInstance();
            objectCache.put(oid, obj);
        }
        obj.assignOid(this, oid, false);
        obj.readObject(new PerstObjectInputStream(obj, body, ObjectHeader.sizeof));
        obj.onLoad();
        return obj;
    }

    class PerstObjectOutputStream extends ByteBuffer implements IOutputStream { 
        IPersistent po;

        PerstObjectOutputStream(IPersistent obj) { 
            super.extend(ObjectHeader.sizeof);
            po = obj;
        }
        
        public void writeByte(byte v) { 
            super.append(2);
            arr[used++] = Types.Byte;
            arr[used++] = v;
        }

        public void writeBoolean(boolean v) { 
            super.append(2);
            arr[used++] = Types.Boolean;
            arr[used++] = (byte)(v ? 1 : 0);
        }

        public void writeChar(char v) { 
            super.append(3);
            arr[used++] = Types.Char;
            Bytes.pack2(arr, used, (short)v);
            used += 2;
        }

        public void writeShort(short v) { 
            super.append(3);
            arr[used++] = Types.Short;
            Bytes.pack2(arr, used, v);
            used += 2;
        }

        public void writeInt(int v) { 
            super.append(5);
            arr[used++] = Types.Int;
            Bytes.pack4(arr, used, v);
            used += 4;
        }

        public void writeLong(long v) { 
            super.append(9);
            arr[used++] = Types.Long;
            Bytes.pack8(arr, used, v);
            used += 8;
        }

        public void writeFloat(float v) { 
            super.append(5);
            arr[used++] = Types.Float;
            Bytes.packF4(arr, used, v);
            used += 4;
        }

        public void writeDouble(double v) { 
            super.append(9);
            arr[used++] = Types.Double;
            Bytes.packF8(arr, used, v);
            used += 8;
        }

        public void writeString(String s) { 
            int offs = used;
            if (s == null) { 
                super.append(3);
                byte[] a = arr;
                a[offs++] = Types.String;
                Bytes.pack2(a, offs, (short)-1);
                offs += 2;
            } else if (encoding == null) { 
                int n = s.length(); 
                super.append(3 + n*2);
                byte[] a = arr;
                a[offs++] = Types.String;
                Bytes.pack2(a, offs, (short)n);
                offs += 2;
                for (int i = 0; i < n; i++) {
                    Bytes.pack2(a, offs, (short)s.charAt(i));
                    offs += 2;
                }
            } else {
                try { 
                    byte[] bytes = s.getBytes(encoding);
                    super.append(3 + bytes.length);
                    byte[] a = arr;
                    a[offs++] = Types.String;
                    Bytes.pack2(a, offs, (short)(-2-bytes.length));
                    System.arraycopy(bytes, 0, a, offs+2, bytes.length);
                    offs += 2 + bytes.length;
                } catch (UnsupportedEncodingException x) { 
                    throw new StorageError(StorageError.UNSUPPORTED_ENCODING);
                }
            }
            used = offs;
        }

        public void writeDate(Date v) { 
            super.append(9);
            arr[used++] = Types.Date;
            Bytes.pack8(arr, used, v == null ? -1 : v.getTime());
            used += 8;
        }
        
        public void writeObject(IPersistent obj) { 
            super.append(5);
            arr[used++] = Types.Object;
            Bytes.pack4(arr, used, swizzle(obj));
            used += 4;
        }

        public void writeLink(Link v) { 
            if (v == null) { 
                super.append(5);            
                arr[used++] = Types.Link;
                Bytes.pack4(arr, used, -1);
                used += 4;
            } else { 
                int len = v.size();
                if (v instanceof EmbeddedLink) { 
                    ((EmbeddedLink)v).setOwner(po);
                }
                super.append(5 + len*4);            
                arr[used++] = Types.Link;
                Bytes.pack4(arr, used, len);
                used += 4;
                for (int i = 0; i < len; i++) {
                    Bytes.pack4(arr, used, swizzle(v.getRaw(i)));
                    used += 4;
                }
                v.unpin();
            }
        }

        public void writeArrayOfBoolean(boolean[] v) { 
            if (v == null) { 
                super.append(5);
                arr[used++] = Types.ArrayOfBoolean;
                Bytes.pack4(arr, used, -1);
                used += 4;
            } else { 
                super.append(5 + v.length);
                arr[used++] = Types.ArrayOfBoolean;
                Bytes.pack4(arr, used, v.length);
                used += 4;
                for (int i = 0; i < v.length; i++) { 
                    arr[used++] = (byte)(v[i] ? 1 : 0);
                }
            }
        }        
        
        
        public void writeArrayOfByte(byte[] v) { 
            if (v == null) { 
                super.append(5);
                arr[used++] = Types.ArrayOfByte;
                Bytes.pack4(arr, used, -1);
                used += 4;
            } else { 
                super.append(5 + v.length);
                arr[used++] = Types.ArrayOfByte;
                Bytes.pack4(arr, used, v.length);
                used += 4;
                System.arraycopy(v, 0, arr, used, v.length);
                used += v.length;
            }
        }        
        
        public void writeArrayOfShort(short[] v) { 
            if (v == null) { 
                super.append(5);
                arr[used++] = Types.ArrayOfShort;
                Bytes.pack4(arr, used, -1);
                used += 4;
            } else { 
                super.append(5 + v.length*2);
                arr[used++] = Types.ArrayOfShort;
                Bytes.pack4(arr, used, v.length);
                used += 4;
                for (int i = 0; i < v.length; i++) { 
                    Bytes.pack2(arr, used, v[i]);
                    used += 2;
                }
            }
        }        
        
        public void writeArrayOfChar(char[] v) { 
            if (v == null) { 
                super.append(5);
                arr[used++] = Types.ArrayOfChar;
                Bytes.pack4(arr, used, -1);
                used += 4;
            } else { 
                super.append(5 + v.length*2);
                arr[used++] = Types.ArrayOfChar;
                Bytes.pack4(arr, used, v.length);
                used += 4;
                for (int i = 0; i < v.length; i++) { 
                    Bytes.pack2(arr, used, (short)v[i]);
                    used += 2;
                }
            }
        }        
        
        
        public void writeArrayOfInt(int[] v) { 
            if (v == null) { 
                super.append(5);
                arr[used++] = Types.ArrayOfInt;
                Bytes.pack4(arr, used, -1);
                used += 4;
            } else { 
                super.append(5 + v.length*4);
                arr[used++] = Types.ArrayOfInt;
                Bytes.pack4(arr, used, v.length);
                used += 4;
                for (int i = 0; i < v.length; i++) { 
                    Bytes.pack4(arr, used, v[i]);
                    used += 4;
                }
            }
        }        
    
        
        public void writeArrayOfLong(long[] v) { 
            if (v == null) { 
                super.append(5);
                arr[used++] = Types.ArrayOfLong;
                Bytes.pack4(arr, used, -1);
                used += 4;
            } else { 
                super.append(5 + v.length*8);
                arr[used++] = Types.ArrayOfLong;
                Bytes.pack4(arr, used, v.length);
                used += 4;
                for (int i = 0; i < v.length; i++) { 
                    Bytes.pack8(arr, used, v[i]);
                    used += 8;
                }
            }
        }        
        
        public void writeArrayOfFloat(float[] v) { 
            if (v == null) { 
                super.append(5);
                arr[used++] = Types.ArrayOfFloat;
                Bytes.pack4(arr, used, -1);
                used += 4;
            } else { 
                super.append(5 + v.length*4);
                arr[used++] = Types.ArrayOfFloat;
                Bytes.pack4(arr, used, v.length);
                used += 4;
                for (int i = 0; i < v.length; i++) { 
                    Bytes.packF4(arr, used, v[i]);
                    used += 4;
                }
            }
        }        
    
        
        public void writeArrayOfDouble(double[] v) { 
            if (v == null) { 
                super.append(5);
                arr[used++] = Types.ArrayOfDouble;
                Bytes.pack4(arr, used, -1);
                used += 4;
            } else { 
                super.append(5 + v.length*8);
                arr[used++] = Types.ArrayOfDouble;
                Bytes.pack4(arr, used, v.length);
                used += 4;
                for (int i = 0; i < v.length; i++) { 
                    Bytes.packF8(arr, used, v[i]);
                    used += 8;
                }
            }
        }        
        
        public void writeArrayOfString(String[] v) { 
            super.append(5);
            int offs = used;            
            arr[offs++] = Types.ArrayOfString;
            if (v == null) { 
                Bytes.pack4(arr, offs, -1);
                offs += 4;
            } else { 
                Bytes.pack4(arr, offs, v.length);
                offs += 4;
                for (int i = 0; i < v.length; i++) {
                    String s = v[i];
                    used = offs;
                    if (s == null) { 
                        super.append(2);
                        Bytes.pack2(arr, offs, (short)-1);
                        offs += 2;
                    } else if (encoding == null) { 
                        int n = s.length(); 
                        super.append(2 + n*2);
                        byte[] a = arr;
                        Bytes.pack2(a, offs, (short)n);
                        offs += 2;
                        for (int j = 0; j < n; j++) {
                            Bytes.pack2(a, offs, (short)s.charAt(j));
                            offs += 2;
                        }
                    } else {
                        try { 
                            byte[] bytes = s.getBytes(encoding);
                            super.append(2 + bytes.length);
                            Bytes.pack2(arr, offs, (short)(-2-bytes.length));
                            System.arraycopy(bytes, 0, arr, offs+2, bytes.length);
                            offs += 2 + bytes.length;
                        } catch (UnsupportedEncodingException x) { 
                            throw new StorageError(StorageError.UNSUPPORTED_ENCODING);
                        }
                    }
                }
            }
            used = offs;
        }        
    
        public void writeArrayOfDate(Date[] v) { 
            if (v == null) { 
                super.append(5);
                arr[used++] = Types.ArrayOfDate;
                Bytes.pack4(arr, used, -1);
                used += 4;
            } else { 
                super.append(5 + v.length*8);
                arr[used++] = Types.ArrayOfDate;
                Bytes.pack4(arr, used, v.length);
                used += 4;
                for (int i = 0; i < v.length; i++) { 
                    Bytes.pack8(arr, used, v[i] == null ? -1 : v[i].getTime());
                    used += 8;
                }
            }
        }        

        public void writeArrayOfObject(IPersistent[] v) { 
            super.append(5);
            arr[used++] = Types.ArrayOfObject;
            if (v == null) { 
                Bytes.pack4(arr, used, -1);
                used += 4;
            } else { 
                Bytes.pack4(arr, used, v.length);
                used += 4;
                for (int i = 0; i < v.length; i++) {
                    writeObject(v[i]);
                }
            }
        }
    }

    final byte[] packObject(IPersistent obj) { 
        PerstObjectOutputStream out = new PerstObjectOutputStream(obj);
        ClassDescriptor desc = getClassDescriptor(obj.getClass());
        obj.writeObject(out);
        ObjectHeader.setSize(out.arr, 0, out.size());
        ObjectHeader.setType(out.arr, 0, desc.getOid());
        return out.arr;        
    }

                
    class HashIterator extends Iterator 
    {
        Enumeration oids;

        HashIterator(Hashtable result)
        {
            oids = result.keys();
        }
             
        public Object next() { 
            return lookupObject(nextOid());
        }

        public int nextOid() { 
            return ((Integer)oids.nextElement()).intValue();
        }

        public boolean hasNext() { 
            return oids.hasMoreElements();
        }

        public void remove() { 
            throw new org.garret.perst.UnsupportedOperationException();
        }
    }

    public Iterator merge(Iterator[] selections) {             
        Hashtable result = null;
        for (int i = 0; i < selections.length; i++) { 
            Iterator iterator = selections[i];
            Hashtable newResult = new Hashtable();
            while (iterator.hasNext()) {
                Integer oid = new Integer(iterator.nextOid());                
                if (result == null || result.get(oid) != null) {
                    newResult.put(oid, oid);
                }  
            }
            result = newResult;
            if (result.size() == 0) {
                break;
            }
        }
        if (result == null) {
            result = new Hashtable();
        }
        return new HashIterator(result);     
    }

    public Iterator join(Iterator[] selections) {             
        Hashtable result = new Hashtable();
        for (int i = 0; i < selections.length; i++) { 
            Iterator iterator = selections[i];
            while (iterator.hasNext()) {
                Integer oid = new Integer(iterator.nextOid());
                result.put(oid, oid);
            }
        }
        return new HashIterator(result);     
    }
    
    public int getVersion() { 
        return header.version;
    }

    public void setVersion(int version) { 
        header.version = version;
        setDirty();
    }

    public int getDatabaseFormatVersion() { 
        return header.databaseFormatVersion;
    }
   
    private int     initIndexSize = dbDefaultInitIndexSize;
    private int     objectCacheInitSize = dbDefaultObjectCacheInitSize;
    private int     objectCachePinCount = dbDefaultObjectCachePinCount;
    private long    extensionQuantum = dbDefaultExtensionQuantum;
    private String  cacheKind = "lru";
    private boolean readOnly = false;
    private boolean noFlush = false;
    private boolean alternativeBtree = false;
    private boolean forceStore = true;
    
    boolean concurrentIterator = false;

    Hashtable properties = new Hashtable();

    String    encoding = null; 

    PagePool  pool;
    Header    header;           // base address of database file mapping
    int       dirtyPagesMap[];  // bitmap of changed pages in current index
    boolean   modified;

    int       currRBitmapPage;//current bitmap page for allocating records
    int       currRBitmapOffs;//offset in current bitmap page for allocating 
                              //unaligned records
    int       currPBitmapPage;//current bitmap page for allocating page objects
    int       currPBitmapOffs;//offset in current bitmap page for allocating 
                              //page objects
    Location  reservedChain;
    CloneNode cloneList;
    boolean   insideCloneBitmap;

    int       committedIndexSize;
    int       currIndexSize;

    int       currIndex;  // copy of header.root, used to allow read access to the database 
                          // during transaction commit
    long      usedSize;   // total size of allocated objects since the beginning of the session
    int[]     bitmapPageAvailableSpace;
    boolean   opened;

    StorageListener listener;

    int       bitmapExtentBase;

    long      transactionId;
    IFile     file;

    int       nNestedTransactions;
    int       nBlockedTransactions;
    int       nCommittedTransactions;
    long      scheduledCommitTime;
    Object    transactionMonitor;
    PersistentResource transactionLock;

    final Hashtable transactionContext = new Hashtable();
    boolean useSerializableTransactions;

    Class classDescriptorClass;

    OidHashTable     objectCache;
    Hashtable        classDescMap;
    ClassDescriptor  descList;
}

class RootPage { 
    long size;            // database file size
    long index;           // offset to object index
    long shadowIndex;     // offset to shadow index
    long usedSize;        // size used by objects
    int  indexSize;       // size of object index
    int  shadowIndexSize; // size of object index
    int  indexUsed;       // userd part of the index   
    int  freeList;        // L1 list of free descriptors
    int  bitmapEnd;       // index of last allocated bitmap page
    int  rootObject;      // OID of root object
    int  classDescList;   // List of class descriptors
    int  bitmapExtent;    // Offset of extended bitmap pages in object index

    final static int sizeof = 64;
} 

class Header { 
    int      curr;  // current root
    boolean  dirty; // database was not closed normally
    byte     databaseFormatVersion;
    int      version; 
    long     transactionId;

    RootPage root[];

    final static int sizeof = 15 + RootPage.sizeof*2;
    
    final void pack(byte[] rec) { 
        int offs = 0;
        rec[offs++] = (byte)curr;
        rec[offs++] = (byte)(dirty ? 1 : 0);
        rec[offs++] = databaseFormatVersion;
        Bytes.pack4(rec, offs, version);
        offs += 4;
        Bytes.pack8(rec, offs, transactionId);
        offs += 8;
        for (int i = 0; i < 2; i++) { 
            Bytes.pack8(rec, offs, root[i].size);
            offs += 8;
            Bytes.pack8(rec, offs, root[i].index);
            offs += 8;
            Bytes.pack8(rec, offs, root[i].shadowIndex);
            offs += 8;
            Bytes.pack8(rec, offs, root[i].usedSize);
            offs += 8;
            Bytes.pack4(rec, offs, root[i].indexSize);
            offs += 4;
            Bytes.pack4(rec, offs, root[i].shadowIndexSize);
            offs += 4;
            Bytes.pack4(rec, offs, root[i].indexUsed);
            offs += 4;
            Bytes.pack4(rec, offs, root[i].freeList);
            offs += 4;
            Bytes.pack4(rec, offs, root[i].bitmapEnd);
            offs += 4;
            Bytes.pack4(rec, offs, root[i].rootObject);
            offs += 4;
            Bytes.pack4(rec, offs, root[i].classDescList);
            offs += 4;
            Bytes.pack4(rec, offs, root[i].bitmapExtent);
            offs += 4;
        }
        Assert.that(offs == sizeof);
    }
    
    final void unpack(byte[] rec) { 
        int offs = 0;
        curr = rec[offs++];
        dirty = rec[offs++] != 0;
        databaseFormatVersion = rec[offs++];
        version = Bytes.unpack4(rec, offs);
        offs += 4;
        transactionId = Bytes.unpack8(rec, offs);
        offs += 8;
        root = new RootPage[2];
        for (int i = 0; i < 2; i++) { 
            root[i] = new RootPage();
            root[i].size = Bytes.unpack8(rec, offs);
            offs += 8;
            root[i].index = Bytes.unpack8(rec, offs);
            offs += 8;
            root[i].shadowIndex = Bytes.unpack8(rec, offs);
            offs += 8;
            root[i].usedSize = Bytes.unpack8(rec, offs);
            offs += 8;
            root[i].indexSize = Bytes.unpack4(rec, offs);
            offs += 4;
            root[i].shadowIndexSize = Bytes.unpack4(rec, offs);
            offs += 4;
            root[i].indexUsed = Bytes.unpack4(rec, offs);
            offs += 4;
            root[i].freeList = Bytes.unpack4(rec, offs);
            offs += 4;
            root[i].bitmapEnd = Bytes.unpack4(rec, offs);
            offs += 4;
            root[i].rootObject = Bytes.unpack4(rec, offs);
            offs += 4;
            root[i].classDescList = Bytes.unpack4(rec, offs);
            offs += 4;
            root[i].bitmapExtent = Bytes.unpack4(rec, offs);
            offs += 4;
        }
        Assert.that(offs == sizeof);
    }   
}

