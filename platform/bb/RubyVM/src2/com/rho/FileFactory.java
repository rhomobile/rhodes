package com.rho;

public class FileFactory 
{ 
    public static SimpleFile createFile() 
    {
        Class systemClass;
        Class wrapperClass;
        //RHO COMMENT
//        try { 
//            systemClass = Class.forName("java.io.RandomAccessFile");
//            wrapperClass = Class.forName("org.garret.perst.impl.CdcFile");
//        } catch (ClassNotFoundException x) {
            try {
                systemClass = Class.forName("javax.microedition.io.file.FileConnection");
                wrapperClass = Class.forName("com.rho.Jsr75File");
            } catch (ClassNotFoundException x2) {  
                try { 
                    systemClass = Class.forName("javax.microedition.rms.RecordStore");
                    wrapperClass = Class.forName("org.garret.perst.impl.RmsFile");
                } catch (ClassNotFoundException x3) {  
                    throw new StorageError(StorageError.FILE_ACCESS_ERROR, x3);
                }
            }
//        }
        try { 
            return (SimpleFile)wrapperClass.newInstance();
        } catch (Exception x) { 
            throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
        }
    }
}
