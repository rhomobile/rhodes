package org.garret.perst;

import java.util.Date;

/**
 * Interface for fetching component of the objects.
 * Programmer should use methods of this interface while
 * implementation of </code>ISerializable.readObject(IInputStream)</code> method
 */
public interface IInputStream { 
    /**
     * Read boolean value
     * @return boolean field value 
     */
    boolean   readBoolean();    
    /**
     * Read byte value
     * @return byte field value 
     */
    byte      readByte();    
    /**
     * Read char value
     * @return char field value 
     */
    char      readChar();
    /**
     * Read short value
     * @return short field value 
     */
    short     readShort();    
    /**
     * Read int value
     * @return int field value 
     */
    int       readInt();    
    /**
     * Read long value
     * @return long field value 
     */
    long      readLong();    
    /**
     * Read float value
     * @return float field value 
     */
    float     readFloat();    
    /**
     * Read double value
     * @return double field value 
     */
    double    readDouble();    
    /**
     * Read string value
     * @return string field value 
     */
    String    readString();    
    /**
     * Read date value
     * @return <code>java.util.date</code> field value 
     */
    Date      readDate();    
    /**
     * Read reference to persistent object.
     * @return resolved reference to persistent object
     */
    IPersistent readObject();
    /**
     * Read link
     * @return <code>Link</code> field value
     */
    Link      readLink();
    /**
     * Read array of boolean value
     * @return <code>boolean[]</code> field value 
     */
    boolean[] readArrayOfBoolean();    
    /**
     * Read array of char value
     * @return <code>char[]</code> field value 
     */
    byte[]    readArrayOfByte();    
    /**
     * Read array of byte value
     * @return <code>byte[]</code> field value 
     */
    char[]    readArrayOfChar();    
    /**
     * Read array of short value
     * @return <code>short[]</code> field value 
     */
    short[]   readArrayOfShort();    
    /**
     * Read array of int value
     * @return <code>int[]</code> field value 
     */
    int[]     readArrayOfInt();    
    /**
     * Read array of long value
     * @return <code>long[]</code> field value 
     */
    long[]    readArrayOfLong();    
    /**
     * Read array of float value
     * @return <code>float[]</code> field value 
     */
    float[]   readArrayOfFloat();    
    /**
     * Read array of double value
     * @return <code>double[]</code> field value 
     */
    double[]  readArrayOfDouble();    
    /**
     * Read array of String value
     * @return <code>String[]</code> field value 
     */
    String[]  readArrayOfString();    
    /**
     * Read array of Date value
     * @return <code>java.util.Date[]</code> field value 
     */
    Date[]    readArrayOfDate();    
    /**
     * Read array of object value. Restrictions of array elements type are the same as for 
     * <code>readObject()</code> method.
     * @return <code>IPersistent[]</code> field value 
     */
    IPersistent[] readArrayOfObject();
    /**
     * Read array of object value.
     * @param arr array in which fetched elements will be placed. Size of array should be large enough
     * to hold the whole array
     * @return number of fetched elements
     */
    int readArrayOfObject(IPersistent[] arr);
}