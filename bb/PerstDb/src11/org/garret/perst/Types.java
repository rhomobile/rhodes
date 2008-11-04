package org.garret.perst;

/**
 * Enumeration of Java types supported by Perst.Lite
 * This enumeration is used to describe type code in Perst container constructors.
 */
public class Types 
{
    public static final int Boolean          = 0;
    public static final int Byte             = 1;
    public static final int Char             = 2;
    public static final int Short            = 3;
    public static final int Int              = 4;
    public static final int Long             = 5;
    public static final int Float            = 6;
    public static final int Double           = 7;
    public static final int String           = 8;
    public static final int Date             = 9;
    public static final int Object           = 10;
    public static final int Link             = 11;
    public static final int ArrayOfBoolean   = 20;
    public static final int ArrayOfByte      = 21;
    public static final int ArrayOfChar      = 22;
    public static final int ArrayOfShort     = 23;
    public static final int ArrayOfInt       = 24;
    public static final int ArrayOfLong      = 25;
    public static final int ArrayOfFloat     = 26;
    public static final int ArrayOfDouble    = 27;
    public static final int ArrayOfString    = 28;
    public static final int ArrayOfDate      = 29;
    public static final int ArrayOfObject    = 30;

    public static String getSignature(int type) { 
        return signature[type];
    }

    private static final String signature[] = {
        "boolean", 
        "byte",
        "char",
        "short",
        "int",
        "long",
        "float",
        "double",
        "String",
        "Date",
        "Object",
        "Link",
        "",
        "",
        "", 
        "", 
        "", 
        "", 
        "", 
        "", 
        "ArrayOfBoolean",
        "ArrayOfByte",
        "ArrayOfChar",
        "ArrayOfShort",
        "ArrayOfInt",
        "ArrayOfLong",
        "ArrayOfFloat",
        "ArrayOfDouble",
        "ArrayOfString",
        "ArrayOfDate",
        "ArrayOfObject"
    };
}