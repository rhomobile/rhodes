/* Copyright (c) 2001-2008, The HSQL Development Group
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of the HSQL Development Group nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL HSQL DEVELOPMENT GROUP, HSQLDB.ORG,
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


package org.hsqldb.lib.java;

import j2me.io.File;
//import j2me.io.FileNotFoundException;
//import j2me.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
//import java.io.PrintStream;
//import java.io.PrintWriter;
import j2me.math.BigDecimal;
import j2me.math.BigInteger;
//import java.sql.DriverManager;
//import java.util.Properties;
import java.util.Hashtable;
//import j2me.text.Collator;
import com.rho.file.RandomAccessFile;
import java.io.InputStream;

import org.hsqldb.lib.FileAccess;
import com.rho.db.FileUtilBB;

// fredt@users 20020320 - patch 1.7.0 - JDBC 2 support and error trapping
// fredt@users 20021030 - patch 1.7.2 - updates

/**
 * Handles the differences between JDK 1.1.x and 1.2.x and above
 *
 * @author fredt@users
 * @version 1.8.0
 */
public class JavaSystem {

    // variables to track rough count on object creation, to use in gc
    public static int gcFrequency;
    public static int memoryRecords;

    // Garbage Collection
    public static void gc() {

        if ((gcFrequency > 0) && (memoryRecords > gcFrequency)) {
            memoryRecords = 0;

            System.gc();
        }
    }

    /**
     * Arguments are never null.
     */
    public static int CompareIngnoreCase(String a, String b) {

//#ifdef JAVA2FULL
//        return a.compareToIgnoreCase(b);

//#else

        return a.toUpperCase().compareTo(b.toUpperCase());


//#endif JAVA2
    }

    public static double parseDouble(String s) {

//#ifdef JAVA2FULL
        return Double.parseDouble(s);

//#else
/*
        return new Double(s).doubleValue();
*/

//#endif JAVA2
    }

    public static BigInteger getUnscaledValue(BigDecimal o) {

//#ifdef JAVA2FULL
        return o.unscaledValue();

//#else
/*
        int scale = o.scale();
        return o.movePointRight(scale).toBigInteger();
*/

//#endif
    }

    /*public static void setLogToSystem(boolean value) {

//#ifdef JAVA2FULL
        try {
            PrintWriter newPrintWriter = (value) ? new PrintWriter(System.out)
                                                 : null;

            DriverManager.setLogWriter(newPrintWriter);
        } catch (Exception e) {}*/

//#else
/*
        try {
            PrintStream newOutStream = (value) ? System.out
                                               : null;
            DriverManager.setLogStream(newOutStream);
        } catch (Exception e){}
*/

//#endif
    //}

    public static void deleteOnExit(File f) {

//#ifdef JAVA2FULL
        //f.deleteOnExit();

//#endif
    }

    public static void loadPropeties(Hashtable props, InputStream fis ){

    }

    public static void saveProperties(Hashtable props, String name,
                                      OutputStream os) throws IOException {

//#ifdef JAVA2FULL
        //props.store(os, name);
//#else
/*
        props.save(os, name);
*/

//#endif
    }

    public static void runFinalizers() {

//#ifdef JAVA2FULL
//        System.runFinalizersOnExit(true);
//#endif
    }

    /*public static boolean createNewFile(File file) {

//#ifdef JAVA2FULL
        try {
            return file.createNewFile();
        } catch (IOException e) {}

        return false;

//#else

        return true;


//#endif
    }*/

    public static void setRAFileLength(RandomAccessFile raFile,
                                       long length) throws IOException {

//#ifdef JAVA2FULL
        raFile.setLength(length);

//#endif
    }

    static public FileAccess createFileAccess(){
    	// oj@openoffice.org - changed to file access api

    	/*        String fileaccess_class_name =
        (String) urlProperties.getProperty("fileaccess_class_name");

	    if (fileaccess_class_name != null) {
	        String storagekey = urlProperties.getProperty("storage_key");

	        try {
	            Class fileAccessClass = null;
	            try {
	                ClassLoader classLoader =
	                    Thread.currentThread().getContextClassLoader();
	                fileAccessClass =
	                    classLoader.loadClass(fileaccess_class_name);
	            }
	            catch (ClassNotFoundException e) {
	                fileAccessClass = Class.forName(fileaccess_class_name);
	            }
	            Constructor constructor =
	                fileAccessClass.getConstructor(new Class[]{
	                    Object.class });

	            fileaccess =
	                (FileAccess) constructor.newInstance(new Object[]{
	                    storagekey });
	            isStoredFileAccess = true;
	        } catch (Exception e) {
	            throw Trace.error(Trace.INVALID_FILE_ACCESS_CLASS,
	                              new Object[]{ e.toString() });
	        }
	    } else {*/
	        return  FileUtilBB.getDefaultInstance();
	    //}
    }

}
