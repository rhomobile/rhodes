package org.garret.perst.impl;
import  org.garret.perst.*;
import java.util.*;

public class TimeSeriesImpl extends PersistentResource implements TimeSeries { 
    public void add(TimeSeriesTick tick) { 
        long time = tick.getTime();
        Iterator iterator = index.iterator(new Key(time - maxBlockTimeInterval), new Key(time), Index.DESCENT_ORDER);
        if (iterator.hasNext()) { 
            insertInBlock((TimeSeriesBlock)iterator.next(), tick);
        } else { 
            addNewBlock(tick);
        }
    }

    class TimeSeriesIterator extends Iterator { 
        TimeSeriesIterator(long from, long till) { 
            pos = -1;
            this.till = till;
            blockIterator = index.iterator(new Key(from - maxBlockTimeInterval), new Key(till), Index.ASCENT_ORDER);
            while (blockIterator.hasNext()) { 
                TimeSeriesBlock block = (TimeSeriesBlock)blockIterator.next();
                int n = block.used;
                TimeSeriesTick[] e = block.getTicks();
                int l = 0, r = n;
                while (l < r)  {
                    int i = (l+r) >> 1;
                    if (from > e[i].getTime()) { 
                        l = i+1;
                    } else { 
                        r = i;
                    }
                }
                Assert.that(l == r && (l == n || e[l].getTime() >= from)); 
                if (l < n) {
                    if (e[l].getTime() <= till) { 
                        pos = l;
                        currBlock = block;
                    }
                    return;
                }
            } 
        }

        public int nextOid() {
            throw new org.garret.perst.UnsupportedOperationException();
        }

        public boolean hasNext() { 
            return pos >= 0;
        }

        public Object next() { 
            if (pos < 0) { 
                 throw new NoSuchElementException();
            }
            TimeSeriesTick tick = currBlock.getTicks()[pos];
            if (++pos == currBlock.used) { 
                if (blockIterator.hasNext()) { 
                    currBlock = (TimeSeriesBlock)blockIterator.next();
                    pos = 0;
                } else { 
                    pos = -1;
                    return tick;
                }
            }
            if (currBlock.getTicks()[pos].getTime() > till) {
                pos = -1;
            }
            return tick;
        }

        public void remove() { 
            throw new org.garret.perst.UnsupportedOperationException();
        }

        private Iterator blockIterator;
        private TimeSeriesBlock currBlock;
        private int      pos;
        private long     till;
    }
                
            
    class TimeSeriesReverseIterator extends Iterator { 
        TimeSeriesReverseIterator(long from, long till) { 
            pos = -1;
            this.from = from;
            blockIterator = index.iterator(new Key(from - maxBlockTimeInterval), new Key(till), Index.DESCENT_ORDER);
            while (blockIterator.hasNext()) { 
                TimeSeriesBlock block = (TimeSeriesBlock)blockIterator.next();
                int n = block.used;
                TimeSeriesTick[] e =  block.getTicks();
                int l = 0, r = n;
                while (l < r)  {
                    int i = (l+r) >> 1;
                    if (till >= e[i].getTime()) { 
                        l = i+1;
                    } else { 
                        r = i;
                    }
                }
                Assert.that(l == r && (l == n || e[l].getTime() > till)); 
                if (l > 0) {
                    if (e[l-1].getTime() >= from) { 
                        pos = l-1;
                        currBlock = block;
                    }
                    return;
                }
            } 
        }

        public boolean hasNext() { 
            return pos >= 0;
        }

        public int nextOid() {
            throw new org.garret.perst.UnsupportedOperationException();
        }

        public Object next() { 
            if (pos < 0) { 
                 throw new NoSuchElementException();
            }
            TimeSeriesTick tick = currBlock.getTicks()[pos];
            if (--pos < 0) { 
                if (blockIterator.hasNext()) { 
                    currBlock = (TimeSeriesBlock)blockIterator.next();
                    pos = currBlock.used-1;
                } else { 
                    pos = -1;
                    return tick;
                }
            }
            if (currBlock.getTicks()[pos].getTime() < from) {
                pos = -1;
            }
            return tick;
        }

        public void remove() { 
            throw new org.garret.perst.UnsupportedOperationException();
        }

        private Iterator blockIterator;
        private TimeSeriesBlock currBlock;
        private int      pos;
        private long     from;
    }
                            
    public Iterator iterator() { 
        return iterator(null, null, true);
    }

    public Iterator iterator(Date from, Date till) {
        return iterator(from, till, true);
    }

    public Iterator iterator(boolean ascent) {
        return iterator(null, null, ascent);
    }

    public Iterator iterator(Date from, Date till, boolean ascent) { 
        long low = from == null ? 0 : from.getTime();
        long high = till == null ? Long.MAX_VALUE : till.getTime();
        return ascent 
            ? (Iterator)new TimeSeriesIterator(low, high)
            : (Iterator)new TimeSeriesReverseIterator(low, high);
    }

    public Date getFirstTime() {
        Iterator blockIterator = index.iterator();
        if (blockIterator.hasNext()) { 
            TimeSeriesBlock block = (TimeSeriesBlock)blockIterator.next();            
            return new Date(block.timestamp);
        } 
        return null;
    }

    public Date getLastTime() {
        Iterator blockIterator = index.iterator(null, null, Index.DESCENT_ORDER);
        if (blockIterator.hasNext()) { 
            TimeSeriesBlock block = (TimeSeriesBlock)blockIterator.next();            
            return new Date(block.getTicks()[block.used-1].getTime());
        } 
        return null;
    }

    public int size() {
        return (int)countTicks();
    }

    public long countTicks() {
        long n = 0;
        Iterator blockIterator = index.iterator();
        while (blockIterator.hasNext()) { 
            TimeSeriesBlock block = (TimeSeriesBlock)blockIterator.next();            
            n += block.used;
        }
        return n;
    }
       
    public TimeSeriesTick getTick(Date timestamp) {
        long time = timestamp.getTime();
        Iterator blockIterator = index.iterator(new Key(time - maxBlockTimeInterval), new Key(time), Index.ASCENT_ORDER);
        while (blockIterator.hasNext()) { 
            TimeSeriesBlock block = (TimeSeriesBlock)blockIterator.next();
            int n = block.used;
            TimeSeriesTick[] e = block.getTicks();
            int l = 0, r = n;
            while (l < r)  {
                int i = (l+r) >> 1;
                if (time > e[i].getTime()) { 
                    l = i+1;
                } else { 
                    r = i;
                }
            }
            Assert.that(l == r && (l == n || e[l].getTime() >= time)); 
            if (l < n && e[l].getTime() == time) { 
                return e[l];
            }
        }
        return null;
    }

    public boolean has(Date timestamp) {
        return getTick(timestamp) != null;
    }

    public long remove(Date from, Date till) {
        long low = from == null ? 0 : from.getTime();
        long high = till == null ? Long.MAX_VALUE : till.getTime();
        long nRemoved = 0;
        Key  fromKey = new Key(low - maxBlockTimeInterval);
        Key  tillKey =  new Key(high);
        Iterator blockIterator = index.iterator(fromKey, tillKey, Index.ASCENT_ORDER);
        while (blockIterator.hasNext()) { 
            TimeSeriesBlock block = (TimeSeriesBlock)blockIterator.next();
            int n = block.used;
            TimeSeriesTick[] e = block.getTicks();
            int l = 0, r = n;
            while (l < r)  {
                int i = (l+r) >> 1;
                if (low > e[i].getTime()) { 
                    l = i+1;
                } else { 
                    r = i;
                }
            }
            Assert.that(l == r && (l == n || e[l].getTime() >= low)); 
            while (r < n && e[r].getTime() <= high) {
                r += 1;
                nRemoved += 1;
            }
            if (l == 0 && r == n) { 
                index.remove(new Key(block.timestamp), block);
                blockIterator = index.iterator(fromKey, tillKey, Index.ASCENT_ORDER);
                block.deallocate();
            } else if (l < n && l != r) { 
                if (l == 0) { 
                    index.remove(new Key(block.timestamp), block);
                    block.timestamp = e[r].getTime();
                    index.put(new Key(block.timestamp), block);
                    blockIterator = index.iterator(fromKey, tillKey, Index.ASCENT_ORDER);
                }
                while (r < n) { 
                    e[l++] = e[r++];
                }
                block.used = l;
                block.modify();
            }
        }
        return nRemoved;
    }

    private void addNewBlock(TimeSeriesTick t)
    {
        TimeSeriesBlock block;
        try { 
            block = (TimeSeriesBlock)blockClass.newInstance();             
        } catch (Exception x) { 
            throw new StorageError(StorageError.CONSTRUCTOR_FAILURE, blockClass, x);
        }
        block.timestamp = t.getTime();
        block.used = 1;
        block.getTicks()[0] = t;
        index.put(new Key(block.timestamp), block);
    }

    void insertInBlock(TimeSeriesBlock block, TimeSeriesTick tick)
    {
        long t = tick.getTime();
        int i, n = block.used;
        
        TimeSeriesTick[] e =  block.getTicks();
        int l = 0, r = n;
        while (l < r)  {
            i = (l+r) >> 1;
            if (t >= e[i].getTime()) { 
                l = i+1;
            } else { 
                r = i;
            }
        }
        Assert.that(l == r && (l == n || e[l].getTime() >= t));
        if (r == 0) { 
            if (e[n-1].getTime() - t > maxBlockTimeInterval || n == e.length) { 
                addNewBlock(tick);
                return;
            }
            if (block.timestamp != t) { 
                index.remove(new Key(block.timestamp), block);                
                block.timestamp = t;
                index.put(new Key(block.timestamp), block);
            }
        } else if (r == n) {
            if (t - e[0].getTime() > maxBlockTimeInterval || n == e.length) { 
                addNewBlock(tick);
                return;
            } 
        }
        if (n == e.length) { 
            addNewBlock(e[n-1]);
            for (i = n; --i > r; ) { 
                e[i] = e[i-1];
            }
        } else { 
            for (i = n; i > r; i--) { 
                e[i] = e[i-1];
            }
            block.used += 1;
        }
        e[r] = tick;
        block.modify();
    }

    TimeSeriesImpl(Storage storage, String blockClassName, long maxBlockTimeInterval) {
        this.blockClass = ClassDescriptor.loadClass(storage, blockClassName);
        this.maxBlockTimeInterval = maxBlockTimeInterval;
        this.blockClassName = blockClassName;
        index = storage.createIndex(Types.Long, false);
    }

    public TimeSeriesImpl() {}
   
    public void onLoad() {
        blockClass = ClassDescriptor.loadClass(getStorage(), blockClassName);
    }

    public void deallocate() {
        Iterator blockIterator = index.iterator();
        while (blockIterator.hasNext()) {
            TimeSeriesBlock block = (TimeSeriesBlock)blockIterator.next();
            block.deallocate();
        }
        index.deallocate();
        super.deallocate();
    }

    public void writeObject(IOutputStream out) {
        out.writeObject(index);
        out.writeLong(maxBlockTimeInterval);
        out.writeString(blockClassName);
    }

    public void readObject(IInputStream in) {
        index = (Index)in.readObject();
        maxBlockTimeInterval = in.readLong();
        blockClassName = in.readString();
    }

    private Index index;
    private long  maxBlockTimeInterval;
    private String blockClassName;
    private transient Class blockClass;
}

