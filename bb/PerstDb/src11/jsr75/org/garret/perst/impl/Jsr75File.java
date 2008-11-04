package org.garret.perst.impl;
import org.garret.perst.*;

import java.io.IOException;
import java.io.EOFException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import javax.microedition.io.*;
import javax.microedition.io.file.*;

public class Jsr75File implements SimpleFile 
{
    private FileConnection fconn;
    private InputStream    in;
    private OutputStream   out;
    private long           inPos;
    private long           outPos;
    private long           currPos;
    private long           fileSize;
    private boolean        noFlush;

    private static final int ZERO_BUF_SIZE = 4096;

    public void open(String path, boolean readOnly, boolean noFlush) 
    {
        String url = path;
        this.noFlush = noFlush;
        if (!url.startsWith("file:")) { 
            if (url.startsWith("/")) { 
                url = "file:///" + path;
            } else { 
                Enumeration e = FileSystemRegistry.listRoots();
                while (e.hasMoreElements()) {
                    // choose arbitrary root directory
                    url = "file:///" + (String)e.nextElement() + path;
                    try { 
                        fconn = (FileConnection)Connector.open(url);
                        // If no exception is thrown, then the URI is valid, but the file may or may not exist.
                        if (!fconn.exists()) { 
                            fconn.create();  // create the file if it doesn't exist
                        }
                        break;
                    } catch (Exception x) {
                        fconn = null;
                        url = "file:///" + path;
                        // try next root
                    }
                }
            }
        }
        try { 
            if (fconn == null) { 
                fconn = (FileConnection)Connector.open(url);
                // If no exception is thrown, then the URI is valid, but the file may or may not exist.
                if (!fconn.exists()) { 
                    fconn.create();  // create the file if it doesn't exist
                }
            }
            fileSize = fconn.fileSize();
            if (!readOnly) { 
                out = fconn.openOutputStream();
                outPos = 0;
            }
        } catch (IOException x) { 
            throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
        }        
        in = null;
        inPos = 0;
    }

    public int read(long pos, byte[] b)
    {
        int len = b.length;

        if (pos >= fileSize) { 
            return 0;
        }

        try {
            if (in == null || inPos > pos) { 
                sync(); 
                if (in != null) { 
                    in.close();
                }
                in = fconn.openInputStream();
                inPos = in.skip(pos);
            } else if (inPos < pos) { 
                inPos += in.skip(pos - inPos);
            }
            if (inPos != pos) { 
                return 0;
            }        
            int off = 0;
            while (len > 0) { 
                int rc = in.read(b, off, len);
                if (rc > 0) { 
                    inPos += rc;
                    off += rc;
                    len -= rc;
                } else { 
                    break;
                }
            }
            return off;
        } catch(IOException x) { 
            throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
        } 
    }

    public void write(long pos, byte[] b)
    {
        int len = b.length;
        if (out == null) { 
            throw new StorageError(StorageError.FILE_ACCESS_ERROR, "Illegal mode");
        }
        try {
            if (outPos != pos) {                         
                out.close();
                out = fconn.openOutputStream(pos);
                if (pos > fileSize) { 
                    byte[] zeroBuf = new byte[ZERO_BUF_SIZE];
                    do { 
                        int size = pos - fileSize > ZERO_BUF_SIZE ? ZERO_BUF_SIZE : (int)(pos - fileSize);
                        out.write(zeroBuf, 0, size);
                        fileSize += size;
                    } while (pos != fileSize);
                }
                outPos = pos;
            }
            out.write(b, 0, len);
            outPos += len;
            if (outPos > fileSize) { 
                fileSize = outPos;
            }
            if (in != null) { 
                in.close();
                in = null;
            }
        } catch(IOException x) { 
            throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
        }
    }

    public long length() { 
        return fileSize;
    }

    public void close() { 
        try {
            if (in != null) { 
                in.close();
                in = null;
            }
            if (out != null) { 
                out.close();
                out = null;
            }
            fconn.close();
        } catch(IOException x) { 
            throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
        }
    }
    
    public void sync() {
        if (!noFlush && out != null) { 
            try { 
                out.flush();
            } catch(IOException x) { 
                throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
            }
        }
    }
}
