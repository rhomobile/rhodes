package org.garret.perst;

import java.util.Date;

/**
 * Interface for storing component of the objects.
 * Programmer should use methods of this interface while
 * implementation of </code>ISerializable.writeObject(IOutputStream)</code> method
 */
public interface IOutputStream { 
    /**
     * Write boolean value
     * @param v value of the field
     */
    void writeBoolean(boolean v);    
    /**
     * Write byte value
     * @param v value of the field
     */
    void writeByte(byte v);    
    /**
     * Write char value
     * @param v value of the field
     */
    void writeChar(char v);
    /**
     * Write short value
     * @param v value of the field
     */
    void writeShort(short v);    
    /**
     * Write int value
     * @param v value of the field
     */
    void writeInt(int v);    
    /**
     * Write long value
     * @param v value of the field
     */
    void writeLong(long v);    
    /**
     * Write float value
     * @param v value of the field
     */
    void writeFloat(float v);    
    /**
     * Write double value
     * @param v value of the field
     */
    void writeDouble(double v);    
    /**
     * Write String value
     * @param v value of the field (may be null)
     */
    void writeString(String v);    
    /**
     * Write date value
     * @param v value of the field (may be null)
     */
    void writeDate(Date v);    
    /**
     * Write persistent object reference
     * @param v value of the field (may be null)
     */
    void writeObject(IPersistent v);
    /**
     * Write persistent object link
     * @param v value of the field (may be null)
     */
    void writeLink(Link v);
    /**
     * Write array of boolean value
     * @param v value of the field (may be null)
     */
    void writeArrayOfBoolean(boolean[] v);    
    /**
     * Write array of byte value
     * @param v value of the field (may be null)
     */
    void writeArrayOfByte(byte[] v);    
    /**
     * Write array of char value
     * @param v value of the field (may be null)
     */
    void writeArrayOfChar(char[] v);    
    /**
     * Write array of short value
     * @param v value of the field (may be null)
     */
    void writeArrayOfShort(short[] v);    
    /**
     * Write array of int value
     * @param v value of the field (may be null)
     */
    void writeArrayOfInt(int[] v);    
    /**
     * Write array of long value
     * @param v value of the field (may be null)
     */
    void writeArrayOfLong(long[] v);    
    /**
     * Write array of float value
     * @param v value of the field (may be null)
     */
    void writeArrayOfFloat(float[] v);    
    /**
     * Write array of double value
     * @param v value of the field (may be null)
     */
    void writeArrayOfDouble(double[] v);    
    /**
     * Write array of string value
     * @param v value of the field (may be null)
     */
    void writeArrayOfString(String[] v);    
    /**
     * Write array of date value
     * @param v value of the field (may be null)
     */
    void writeArrayOfDate(Date[] v);    
    /**
     * Write array of object value. Restrictions of array elements type are the same as for 
     * <code>writeObject()</code> method.
     * @param v value of the field (may be null)
     */
    void writeArrayOfObject(IPersistent[] v);  
}