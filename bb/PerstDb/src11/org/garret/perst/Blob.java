package org.garret.perst;

import java.io.*;

/**
 * Interface to store/fetch large binary objects
 */
public interface Blob extends IPersistent, IResource { 
    /**
     * Get input stream. InputStream.availabe method can be used to get BLOB size
     * @return input stream with BLOB data
     */
    RandomAccessInputStream getInputStream();    

    /**
     * Get output stream to append data to the BLOB.
     * @return output srteam 
     */
    RandomAccessOutputStream getOutputStream();

    /**
     * Get output stream to append data to the BLOB.
     * @param multisession whether BLOB allows further appends of data or closing 
     * this output streat means that BLOB will not be changed any more. 
     * @return output srteam 
     */
    RandomAccessOutputStream getOutputStream(boolean multisession);

    /**
     * Get output stream with specified current postion in BLOB.
     * @param position current position in BLOB, if less than zero, than data will be appended to the BLOB
     * @param multisession whether BLOB allows further appends of data or closing 
     * this output streat means that BLOB will not be changed any more. 
     * @return output srteam 
     */
    RandomAccessOutputStream getOutputStream(long position, boolean multisession);

    int ENABLE_SEGMENT_CACHING = 1;
    int DOUBLE_SEGMENT_SIZE    = 2;
    int TRUNCATE_LAST_SEGMENT  = 4;
    int APPEND                 = 8;

    /**
     * Get input stream. InputStream.availabe method can be used to get BLOB size
     * @param flags bit mask of BLOB flags: ENABLE_SEGMENT_CACHING
     * @return input stream with BLOB data
     */
    RandomAccessInputStream getInputStream(int flags);    
    
    /**
     * Get output stream to append data to the BLOB.
     * @param flags bit mask of BLOB flags: ENABLE_SEGMENT_CACHING, DOUBLE_SEGMENT_SIZE, TRUNCATE_LAST_SEGMENT, APPEND
     * @return output srteam 
     */
    RandomAccessOutputStream getOutputStream(int flags);

};