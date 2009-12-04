/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyFixnum;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/*
 * A simple console program that test XRuby runtime's performance 
 */
public class TestingPerformance {

    private static final int LOOP = 30000000;

    private static long test_createFixnum() {
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            ObjectFactory.createFixnum(i);
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_createString() {
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            ObjectFactory.createString("xxxxx");
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_createArray() {
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            new RubyArray(ObjectFactory.FIXNUM1);
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_arrayAccess() {
        RubyArray a = new RubyArray(ObjectFactory.FIXNUM1);
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            a.get(0);
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_javaArrayAccess() {
        RubyValue[] a = new RubyValue[]{ObjectFactory.FIXNUM1};
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            @SuppressWarnings("unused")
            RubyValue b = a[0];
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_createJavaArray() {
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            @SuppressWarnings("unused")
            RubyValue[] a = new RubyValue[]{ObjectFactory.FIXNUM1};
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_cast() {
        RubyValue v = new RubyArray(ObjectFactory.FIXNUM1);
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            @SuppressWarnings("unused")
            RubyArray a = (RubyArray) v;
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_instanceof() {
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            @SuppressWarnings("unused")
            boolean b = (ObjectFactory.FIXNUM1 instanceof RubyFixnum);
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_nothing() {
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            //Do nothing
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_hashMapLookup() {
        Map<String, RubyMethod> m = new HashMap<String, RubyMethod>();
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            m.get("+");//will return null (find nothing)
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_javaString_intern() {
        String s = "+";
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            s.intern();
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_RubyID_intern() {
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            RubyID.intern("+");
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_iteratingEmptyArrayList() {
        ArrayList<RubyModule> a = new ArrayList<RubyModule>();
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            for (//@SuppressWarnings("unused") java Bug ID: 	6294589
                    RubyModule module : a) {
                //DO nothing
            }
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_iteratorCreation() {
        ArrayList<RubyModule> a = new ArrayList<RubyModule>();
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            a.iterator();
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_StringBuilder_append1() {
        StringBuilder s1 = new StringBuilder();
        StringBuilder s2 = new StringBuilder('x');
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            s1.append(s2);
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_ArrayList_isEmpty() {
        ArrayList<RubyModule> a = new ArrayList<RubyModule>();
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            a.isEmpty();
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_StringBuilder_append2() {
        StringBuilder s1 = new StringBuilder();
        StringBuilder s2 = new StringBuilder('x');
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            s1.append(s2.toString());
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_searchEmptySingletonClass() {
        RubyObject o = new RubyObject(RubyRuntime.ObjectClass);
        RubyClass c = o.getSingletonClass();

        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            c.findOwnMethod(RubyID.plusID);//will return null (find nothing)
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_findMethodForFixnum() {
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            ObjectFactory.FIXNUM1.findMethod(RubyID.plusID);
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_findMethodForString() {
        RubyString s = ObjectFactory.createString("xxx");
        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            s.findMethod(RubyID.plusID);
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_invokeMethod() {
        RubyMethod m = ObjectFactory.FIXNUM1.findMethod(RubyID.plusID);

        long start = System.currentTimeMillis();
        for (int i = 0; i < LOOP; ++i) {
            m.invoke(ObjectFactory.FIXNUM1, ObjectFactory.FIXNUM1, null);
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_callMethodForFixnum() {
        long start = System.currentTimeMillis();
        for (int i = 0; i < 30000000; ++i) {
            RubyAPI.callPublicOneArgMethod(ObjectFactory.FIXNUM1, ObjectFactory.FIXNUM1, null, RubyID.plusID);
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    private static long test_callMethodForString() {
        RubyString s = ObjectFactory.createString("");
        long start = System.currentTimeMillis();
        for (int i = 0; i < 30000000; ++i) {
            RubyAPI.callPublicNoArgMethod(s, null, RubyID.lengthID);
        }
        long end = System.currentTimeMillis();
        return end - start;
    }

    /* Sample result:
     Fixnum Creation: 872
     String Creation: 3246
     Array Creation: 2424
     Array Access: 648
     Java Array Creation: 768
     Java Array Access: 60
     Type Casting: 115
     instanceof: 116
     Do Nothing: 25
     HashMap Lookup: 751
     Java String.intern: 6363
     StringMap.intern: 1360
     Iterating Empty ArrayList: 1591
     Iterator Creation: 903
     ArrayList.isEmpty: 39
     StringBuilder append StringBuilder: 1099
     StringBuilder append String: 2343
     Search Empty SingletonClass: 713
     Method Finding for Fixnum: 705
     Method Finding for String: 1298
     Method Invoking: 1170
     Method Calling for Fixnum: 2371
     Method Calling for String: 3566
     */
    public static void main(String[] args) {
        System.out.println("Fixnum Creation: " + test_createFixnum());
        System.out.println("String Creation: " + test_createString());
        System.out.println("Array Creation: " + test_createArray());
        System.out.println("Array Access: " + test_arrayAccess());
        System.out.println("Java Array Creation: " + test_createJavaArray());
        System.out.println("Java Array Access: " + test_javaArrayAccess());
        System.out.println("Type Casting: " + test_cast());
        System.out.println("instanceof: " + test_instanceof());
        System.out.println("Do Nothing: " + test_nothing());
        System.out.println("HashMap Lookup: " + test_hashMapLookup());
        System.out.println("Java String.intern: " + test_javaString_intern());
        System.out.println("RubyID.intern: " + test_RubyID_intern());
        System.out.println("Iterating Empty ArrayList: " + test_iteratingEmptyArrayList());
        System.out.println("Iterator Creation: " + test_iteratorCreation());
        System.out.println("ArrayList.isEmpty: " + test_ArrayList_isEmpty());
        System.out.println("StringBuilder append StringBuilder: " + test_StringBuilder_append1());
        System.out.println("StringBuilder append String: " + test_StringBuilder_append2());
        System.out.println("Search Empty SingletonClass: " + test_searchEmptySingletonClass());
        System.out.println("Method Finding for Fixnum: " + test_findMethodForFixnum());
        System.out.println("Method Finding for String: " + test_findMethodForString());
        System.out.println("Method Invoking: " + test_invokeMethod());
        System.out.println("Method Calling for Fixnum: " + test_callMethodForFixnum());
        System.out.println("Method Calling for String: " + test_callMethodForString());
    }
}
