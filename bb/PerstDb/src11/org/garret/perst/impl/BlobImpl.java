package org.garret.perst.impl;
import  org.garret.perst.*;

public class BlobImpl extends PersistentResource implements Blob { 
    int           size;
    BlobImpl      next;
    byte[]        body;
    transient int used;

    public void writeObject(IOutputStream out) {
        out.writeInt(size);
        out.writeObject(next);
        out.writeArrayOfByte(body);
    }
        
    public void readObject(IInputStream in) {
        size = in.readInt();
        next = (BlobImpl)in.readObject();
        body = in.readArrayOfByte();
    }

    static final int headerSize = ObjectHeader.sizeof + 3*4;

    void discard(int flags) { 
        if (--used == 0 && (flags & ENABLE_SEGMENT_CACHING) == 0) { 
            invalidate();
            next = null;
        }
    }


    static class BlobInputStream extends RandomAccessInputStream {
        protected BlobImpl curr;
        protected BlobImpl first;
        protected int      pos;
        protected int      blobOffs;
        protected int      flags;

        public int read() {
            byte[] b = new byte[1];
            return read(b, 0, 1) == 1 ? b[0] & 0xFF : -1;
        }

        public int read(byte b[], int off, int len) {
            if (pos >= first.size) { 
                return -1;
            }
            int rest = first.size - pos;
            if (len > rest) { 
                len = rest;
            }
            int rc = len;
            while (len > 0) { 
                if (blobOffs == curr.body.length) { 
                    BlobImpl prev = curr;
                    curr = prev.next;
                    curr.load();
                    curr.used += 1;
                    if (prev != first) { 
                        prev.discard(flags);
                    }
                    blobOffs = 0;
                }
                int n = len > curr.body.length - blobOffs ? curr.body.length - blobOffs : len; 
                System.arraycopy(curr.body, blobOffs, b, off, n);
                blobOffs += n;
                off += n;
                len -= n;
                pos += n;
            }
            return rc;
        }

        public long setPosition(long newPos) { 
            if (newPos < pos) { 
                if (newPos >= pos - blobOffs) { 
                    blobOffs -= pos - newPos;
                    return pos = (int)newPos;
                }
                if (first != curr) { 
                    curr.discard(flags);
                    curr = first;
                }
                pos = 0;
                blobOffs = 0;
            }
            skip(newPos - pos);
            return pos;
        }

        public long getPosition() { 
            return pos;
        }

        public long size() {
            return first.size;
        }

        public long skip(long offs) {
            int rest = first.size - pos;
            if (offs > rest) { 
                offs = rest;
            }
            int len = (int)offs;
            while (len > 0) { 
                if (blobOffs == curr.body.length) { 
                    BlobImpl prev = curr;
                    curr = prev.next;
                    curr.load();
                    curr.used += 1;
                    if (prev != first) { 
                        prev.discard(flags);
                    }
                    blobOffs = 0;
                }
                int n = len > curr.body.length - blobOffs ? curr.body.length - blobOffs : len; 
                pos += n;
                len -= n;
                blobOffs += n;
            }
            return offs;
        }


        public int available() {
            return first.size - pos;
        }

        public void close() {
            curr.discard(flags);
            if (first != curr) { 
                first.discard(flags);
            }
            curr = first = null;
        }

        protected BlobInputStream(BlobImpl first, int flags) { 
            this.flags = flags;
            this.first = first;
            first.load();
            curr = first;
            first.used += 1;
        }
    }

    static class BlobOutputStream extends RandomAccessOutputStream { 
        protected BlobImpl first;
        protected BlobImpl curr;
        protected int      pos;
        protected int      blobOffs;
        protected int      flags;
        protected boolean  modified;

        public void write(int b) { 
            byte[] buf = new byte[1];
            buf[0] = (byte)b;
            write(buf, 0, 1);
        }

        public void write(byte b[], int off, int len) { 
            while (len > 0) { 
                if (blobOffs == curr.body.length) { 
                    BlobImpl prev = curr;
                    if (prev.next == null) { 
                        int length = curr.body.length;
                        if ((flags & DOUBLE_SEGMENT_SIZE) != 0 && (length << 1) > length) { 
                            length = ((length + headerSize) << 1) - headerSize;
                        }
                        BlobImpl next = new BlobImpl(curr.getStorage(), length);
                        curr = prev.next = next;
                        modified = true;
                    } else {
                        curr = prev.next;
                        curr.load();
                    }
                    curr.used += 1;
                    if (prev != first) {
                        if (modified) { 
                            prev.store();
                        }
                        prev.discard(flags);
                    }
                    blobOffs = 0;
                }
                int n = len > curr.body.length - blobOffs ? curr.body.length - blobOffs : len;  
                System.arraycopy(b, off, curr.body, blobOffs, n);
                modified = true;
                blobOffs += n;
                off += n;
                len -= n;
                pos += n;
            }
            if (pos > first.size) { 
                first.size = pos;
            }
        }

        public void close() {
            if ((flags & TRUNCATE_LAST_SEGMENT) != 0 && blobOffs < curr.body.length && curr.next == null) { 
                byte[] tmp = new byte[blobOffs];
                System.arraycopy(curr.body, 0, tmp, 0, blobOffs);
                curr.body = tmp;
            }
            curr.store();
            curr.discard(flags);
            if (curr != first) {
                first.store();
                first.discard(flags);
            }
            first = curr = null;
        }

        public long setPosition(long newPos) { 
            if (newPos < pos) { 
                if (newPos >= pos - blobOffs) { 
                    blobOffs -= pos - newPos;
                    return pos = (int)newPos;
                }
                if (first != curr) {
                    if (modified) { 
                        curr.store();
                        modified = false;
                    }
                    curr.discard(flags);
                    curr = first;
                }
                pos = 0;
                blobOffs = 0;
            }
            skip(newPos - pos);
            return pos;
        }

        public long getPosition() { 
            return pos;
        }

        public long size() {
            return first.size;
        }

        public long skip(long offs) { 
            int rest = first.size - pos;
            if (offs > rest) { 
                offs = rest;
            }
            int len = (int)offs;
            while (len > 0) { 
                if (blobOffs == curr.body.length) { 
                    BlobImpl prev = curr;
                    curr = prev.next;
                    curr.load();
                    curr.used += 1;
                    if (prev != first) {
                        if (modified) { 
                            prev.store();
                            modified = false;
                        }
                        prev.discard(flags);
                    }
                    blobOffs = 0;
                }
                int n = len > curr.body.length - blobOffs ? curr.body.length - blobOffs : len; 
                pos += n;
                len -= n;
                blobOffs += n;
            }
            return offs;
        }
            

        BlobOutputStream(BlobImpl first, int flags) { 
            this.flags = flags;
            this.first = first;
            first.load();
            first.used += 1;
            curr = first;
            if ((flags & APPEND) != 0) { 
                skip(first.size);
            }
        }
    }

    public boolean recursiveLoading() { 
        return false;
    }

    public RandomAccessInputStream getInputStream() { 
        return getInputStream(0);
    }

    public RandomAccessInputStream getInputStream(int flags) { 
        return new BlobInputStream(this, flags);
    }

    public RandomAccessOutputStream getOutputStream() { 
        return getOutputStream(APPEND);
    }

    public RandomAccessOutputStream getOutputStream(boolean multisession) { 
        return getOutputStream(multisession ? APPEND : TRUNCATE_LAST_SEGMENT|APPEND);
    }

    public RandomAccessOutputStream getOutputStream(long position, boolean multisession) { 
        RandomAccessOutputStream stream = getOutputStream(multisession);
        stream.setPosition(position);
        return stream;
    }

    public RandomAccessOutputStream getOutputStream(int flags) { 
        return new BlobOutputStream(this, flags);
    }

    public void deallocate() { 
        load();
        if (size > 0) {
            BlobImpl curr = next;
            while (curr != null) { 
                curr.load();
                BlobImpl tail = curr.next;
                curr.deallocate();
                curr = tail;
            }
        }
        super.deallocate();
    }

    BlobImpl(Storage storage, int size) { 
        super(storage);
        body = new byte[size];
    }

    public BlobImpl() {}
}   