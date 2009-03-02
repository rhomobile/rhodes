package org.garret.perst.impl;
import  org.garret.perst.*;
import  java.util.*;

public final class ClassDescriptor extends Persistent { 
    ClassDescriptor   next;
    String            name;

    transient Class   cls;
    transient boolean resolved;
    
    static final int sizeof[] = {
        1, // tpBoolean
        1, // tpByte
        2, // tpChar
        2, // tpShort
        4, // tpInt
        8, // tpLong
        4, // tpFloat
        8, // tpDouble
        0, // tpString
        8, // tpDate
        4  // tpObject
    };

    Object newInstance() {
        try { 
            return cls.newInstance();
        } catch (Exception x) { 
            throw new StorageError(StorageError.CONSTRUCTOR_FAILURE, cls, x);
        }
    }

    public ClassDescriptor() {}


    ClassDescriptor(StorageImpl storage, Class cls) { 
        this.cls = cls;
        name = getClassName(cls);
        resolved = true;
    }

    public static String getClassName(Class cls) { 
        return cls.getName();
    }
        
        
    public static Class loadClass(Storage storage, String name) { 
        try { 
            return Class.forName(name);
        } catch (ClassNotFoundException x) { 
            throw new StorageError(StorageError.CLASS_NOT_FOUND, name, x);
        }
    }

    public void onLoad() {         
        cls = loadClass(getStorage(), name);
        if (cls == null) { 
            return;
        }
        StorageImpl s = (StorageImpl)getStorage();
        if (s.classDescMap.get(name) == null) { 
            s.classDescMap.put(name, this);
        }
    }

       
    void resolve() {
        if (!resolved) { 
            StorageImpl classStorage = (StorageImpl)getStorage();
            ClassDescriptor desc = new ClassDescriptor(classStorage, cls);
            resolved = true;
        }
    }            

    public boolean recursiveLoading() { 
        return false;
    }

    public void writeObject(IOutputStream out) {
        out.writeObject(next);
        out.writeString(name);
    }

    public void readObject(IInputStream in) {
        next = (ClassDescriptor)in.readObject();
        name = in.readString();
    }
}
