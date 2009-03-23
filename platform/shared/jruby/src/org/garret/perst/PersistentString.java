package org.garret.perst;

/**
 * Class encapsulating native Java string. java.lang.String is not persistent object
 * so it can not be stored in Perst as independent persistent object. 
 * But sometimes it is needed. This class sole this problem providing implcit conversion
 * operator from java.lang.String to PerisstentString.
 * Also PersistentString class is mutable, allowing to change it's values.
 */
public class PersistentString extends PersistentResource
{
    /** 
     * Consutrctor of perisstent string
     * @param str Java string
     */
    public PersistentString(String str)
    {
        this.str = str;
    }
    
    public PersistentString() {}

    /**
     * Get Java string
     * @return Java string
     */
    public String toString() 
    {
        return str;
    }
    
    /**
     * Append string to the current string value of PersistentString
     * @param tail appended string
     */
    public void append(String tail) 
    {
        modify();
        str = str + tail;
    }
    
    /**
     * Assign new string value to the PersistentString
     * @param str new string value
     */
    public void set(String str)
    {
        modify();
        this.str = str;
    }
    
    /**
     * Get current string value
     * @return Java string
     */
    public String get() 
    {
        return str;
    }
    
    private String str;
}
