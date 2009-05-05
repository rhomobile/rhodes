/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.lang;

import javolution.JavolutionError;
import javolution.util.FastMap;
import j2me.lang.CharSequence;
import j2me.lang.ThreadLocal;

/**
 * <p> This utility class greatly facilitates the use of reflection to invoke 
 *     constructors or methods which may or may not exist at runtime.</p>
 * 
 * <p> The constructors/methods are identified through their signatures
 *     represented as a {@link String}. When the constructor/method does
 *     not exist (e.g. class not found) or when the platform does not support
 *     reflection, the constructor/method is <code>null</code> 
 *     (no exception raised). Here is an example of timer taking advantage 
 *     of the new (JRE1.5+) high resolution time when available:[code]
 *     public static long microTime() {
 *         if (NANO_TIME_METHOD != null) { // JRE 1.5+
 *             Long time = (Long) NANO_TIME_METHOD.invoke(null); // Static method.
 *             return time.longValue() / 1000;
 *         } else { // Use the less accurate time in milliseconds.
 *             return System.currentTimeMillis() * 1000;
 *         }
 *     }
 *     private static final Reflection.Method NANO_TIME_METHOD 
 *         = Reflection.getMethod("j2me.lang.System.nanoTime()");[/code]</p>
 *   
 * <p> Arrays and primitive types are supported. For example:[code]
 *     Reflection.Constructor sbc = Reflection.getConstructor("j2me.lang.StringBuilder(int)");
 *     if (sbc != null) { // JDK 1.5+
 *        Object sb = sbc.newInstance(new Integer(32));
 *        Reflection.Method append = Reflection.getMethod("j2me.lang.StringBuilder.append(char[], int, int)");
 *        append.invoke(sb, new char[] { 'h', 'i' }, new Integer(0), new Integer(2));
 *        System.out.println(sb);
 *    }
 * 
 *    > hi[/code]</p>
 * 
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 4.0, September 1, 2006
 */
public final class Reflection {

    /**
     * Default constructor (private to forbid instantiation).
     */
    private Reflection() {
    }

    /**
     * Returns the class having the specified name. 
     * This method searches a lookup table first, then diverse class loaders 
     * (caller, context, system); the newly found class is then initialized 
     * and added to the lookup table for future reference.
     * 
     * @param name the name of the class to search for. 
     * @return the corresponding class
     * @throws ClassNotFoundException if the class is not found.
     */
    public static Class getClass(CharSequence name)
            throws ClassNotFoundException {
        Class cls = (Class) _NameToClass.get(name);
        return (cls != null) ? cls : searchClass(name.toString());
    }

    private static Class searchClass(String name) throws ClassNotFoundException {
        Class cls = null;
        try {
            cls = Class.forName(name); // Caller class loader.
        } catch (ClassNotFoundException e0) { // Try context class loader.
            /*@JVM-1.4+@
             try {
             ClassLoader cl = Thread.currentThread().getContextClassLoader();
             cls = Class.forName(name, true, cl);
             } catch (ClassNotFoundException e1) { // Try system class loader.
             ClassLoader cl = ClassLoader.getSystemClassLoader();
             cls = Class.forName(name, true, cl);
             }
             /**/
            if (cls == null)
                throw new ClassNotFoundException("Cannot found class " + name);
        }
        synchronized (_NameToClass) {
            _NameToClass.put(name, cls);
        }
        return cls;
    }

    private static final FastMap _NameToClass = new FastMap();

    /**
     * Equivalent to {@link #getClass(CharSequence)} (for J2ME compatibility).
     */
    public static Class getClass(String name) throws ClassNotFoundException {
        Class cls = (Class) _NameToClass.get(name);
        return (cls != null) ? cls : searchClass(name);
    }

    /**
     * Returns the constructor having the specified signature. 
     * 
     * @param signature the textual representation of the constructor signature. 
     * @return the corresponding constructor or <code>null</code> if none 
     *         found. 
     */
    public static Constructor getConstructor(String signature) {
        int argStart = signature.indexOf('(') + 1;
        if (argStart < 0) {
            throw new IllegalArgumentException("Parenthesis '(' not found");
        }
        int argEnd = signature.indexOf(')');
        if (argEnd < 0) {
            throw new IllegalArgumentException("Parenthesis ')' not found");
        }
        String className = signature.substring(0, argStart - 1);
        Class theClass;
        try {
            theClass = Reflection.getClass(className);
        } catch (ClassNotFoundException e) {
            return null;
        }
        String args = signature.substring(argStart, argEnd);
        if (args.length() == 0)
            return new DefaultConstructor(theClass);
        /*@JVM-1.4+@
         Class[] argsTypes;
         try {
         argsTypes = classesFor(args);
         } catch (ClassNotFoundException e) {
         return null;
         }         
         try {
         return new ReflectConstructor(theClass.getConstructor(argsTypes),
         signature);
         } catch (NoSuchMethodException e) {
         }          
         /**/
        return null;
    }

    private static class DefaultConstructor extends Constructor {
        final Class _class;

        DefaultConstructor(Class cl) {
            super(new Class[0]); // No arguments.
            _class = cl;
        }

        public Object allocate(Object[] args) {
            try {
                return _class.newInstance();
            } catch (InstantiationException e) {
                throw new JavolutionError("Instantiation error for "
                        + _class.getName() + " default constructor", e);
            } catch (IllegalAccessException e) {
                throw new JavolutionError("Illegal access error for "
                        + _class.getName() + " constructor", e);
            }
        }

        public String toString() {
            return _class + " default constructor";
        }
    }

    /*@JVM-1.4+@
     private static final class ReflectConstructor extends Constructor {
     private final java.lang.reflect.Constructor _value;

     private final String _signature;

     public ReflectConstructor(java.lang.reflect.Constructor value,
     String signature) {
     super(value.getParameterTypes());
     _value = value;
     _signature = signature;
     }

     public Object allocate(Object[] args) {
     try {
     return _value.newInstance(args);
     } catch (IllegalArgumentException e) {
     throw new JavolutionError("Illegal argument for " + _signature
     + " constructor", e);
     } catch (InstantiationException e) {
     throw new JavolutionError("Instantiation error for "
     + _signature + " constructor", e);
     } catch (IllegalAccessException e) {
     throw new JavolutionError("Illegal access error for "
     + _signature + " constructor", e);
     } catch (java.lang.reflect.InvocationTargetException e) {
     throw new JavolutionError("Invocation exception  for "
     + _signature + " constructor",
     (java.lang.reflect.InvocationTargetException) e.getTargetException());
     }
     }

     public String toString() {
     return _signature + " constructor";
     }
     }
     /**/

    /**
     * Returns the method having the specified signature.
     * 
     * @param signature the textual representation of the method signature. 
     * @return the corresponding constructor or <code>null</code> if none 
     *         found. 
     */
    public static Method getMethod(String signature) {
        /*@JVM-1.4+@
         int argStart = signature.indexOf('(') + 1;
         if (argStart < 0) {
         throw new IllegalArgumentException("Parenthesis '(' not found");
         }
         int argEnd = signature.indexOf(')');
         if (argEnd < 0) {
         throw new IllegalArgumentException("Parenthesis ')' not found");
         }
         int nameStart = signature.substring(0, argStart).lastIndexOf('.') + 1;
         try {

         String className = signature.substring(0, nameStart - 1);
         Class theClass;
         try {
         theClass = Reflection.getClass(className);
         } catch (ClassNotFoundException e) {
         return null;
         }         
         String methodName = signature.substring(nameStart, argStart - 1);
         String args = signature.substring(argStart, argEnd);
         Class[] argsTypes;
         try {
         argsTypes = classesFor(args);
         } catch (ClassNotFoundException e) {
         return null;
         }         
         return new ReflectMethod(theClass.getMethod(methodName, argsTypes),
         signature);
         } catch (Throwable t) {
         }
         /**/
        return null;
    }

    /*@JVM-1.4+@
     private static final class ReflectMethod extends Method {
     private final java.lang.reflect.Method _value;

     private final String _signature;

     public ReflectMethod(java.lang.reflect.Method value, String signature) {
     super(value.getParameterTypes());
     _value = value;
     _signature = signature;
     }

     public Object execute(Object that, Object[] args) {
     try {
     return _value.invoke(that, args);
     } catch (IllegalArgumentException e) {
     throw new JavolutionError("Illegal argument for " + _signature
     + " method", e);
     } catch (IllegalAccessException e) {
     throw new JavolutionError("Illegal access error for "
     + _signature + " method", e);
     } catch (java.lang.reflect.InvocationTargetException e) {
     throw new JavolutionError("Invocation exception for "
     + _signature + " method", (java.lang.reflect.InvocationTargetException) e
     .getTargetException());
     }
     }

     public String toString() {
     return _signature + " method";
     }
     }
     /**/

    /**
     * Returns the classes for the specified argument.
     * 
     * @param args the comma separated arguments.
     * @return the classes or <code>null</code> if one of the class is not found.
     @JVM-1.4+@
     private static Class[] classesFor(String args) throws ClassNotFoundException {
     args = args.trim();
     if (args.length() == 0) {
     return new Class[0];
     }
     // Counts commas.
     int commas = 0;
     for (int i=0;;) {
     i = args.indexOf(',', i);
     if (i++ < 0) break;
     commas++;
     }
     Class[] classes = new Class[commas + 1];

     int index = 0;
     for (int i = 0; i < commas; i++) {
     int sep = args.indexOf(',', index);
     classes[i] = classFor(args.substring(index, sep).trim());
     if (classes[i] == null) return null;
     index = sep + 1;
     }
     classes[commas] = classFor(args.substring(index).trim());
     if (classes[commas] == null) return null;
     return classes;
     }

     private static Class classFor(String className) throws ClassNotFoundException {
     int arrayIndex = className.indexOf("[]");
     if (arrayIndex >= 0) {
     if (className.indexOf("[][]") >= 0) {
     if (className.indexOf("[][][]") >= 0) {
     if (className.indexOf("[][][][]") >= 0) {
     throw new UnsupportedOperationException(
     "The maximum array dimension is 3");
     } else { // Dimension three.
     return Reflection.getClass("[[["
     + descriptorFor(className.substring(0,
     arrayIndex)));
     }
     } else { // Dimension two.
     return Reflection.getClass("[["
     + descriptorFor(className.substring(0, arrayIndex)));
     }
     } else { // Dimension one.
     return Reflection.getClass("["
     + descriptorFor(className.substring(0, arrayIndex)));
     }
     }
     if (className.equals("boolean")) {
     return boolean.class;
     } else if (className.equals("byte")) {
     return byte.class;
     } else if (className.equals("char")) {
     return char.class;
     } else if (className.equals("short")) {
     return short.class;
     } else if (className.equals("int")) {
     return int.class;
     } else if (className.equals("long")) {
     return long.class;
     } else if (className.equals("float")) {
     return float.class;
     } else if (className.equals("double")) {
     return double.class;
     } else {
     return Reflection.getClass(className);
     }
     }

     private static String descriptorFor(String className) {
     if (className.equals("boolean")) {
     return "Z";
     } else if (className.equals("byte")) {
     return "B";
     } else if (className.equals("char")) {
     return "C";
     } else if (className.equals("short")) {
     return "S";
     } else if (className.equals("int")) {
     return "I";
     } else if (className.equals("long")) {
     return "J";
     } else if (className.equals("float")) {
     return "F";
     } else if (className.equals("double")) {
     return "D";
     } else {
     return "L" + className + ";";
     }
     }
     /**/

    /**
     * This class represents a run-time constructor obtained through reflection.
     * 
     * Here are few examples of utilization:[code]
     * // Default constructor (fastList = new FastList())
     * Reflection.Constructor fastListConstructor 
     *     = Reflection.getConstructor("javolution.util.FastList()");
     * Object fastList = fastListConstructor.newInstance();
     * 
     * // Constructor with arguments (fastMap = new FastMap(64))
     * Reflection.Constructor fastMapConstructor 
     *     = Reflection.getConstructor("javolution.util.FastMap(int)");
     * Object fastMap = fastMapConstructor.newInstance(new Integer(64));
     * [/code]
     */
    public static abstract class Constructor {

        /**
         * Holds the parameter types.
         */
        private final Class[] _parameterTypes;

        /**
         * Creates a new constructor having the specified parameter types.
         * 
         * @param parameterTypes the parameters types.
         */
        protected Constructor(Class[] parameterTypes) {
            _parameterTypes = parameterTypes;
        }

        /**
         * Returns an array of <code>Class</code> objects that represents 
         * the formal parameter types, in declaration order of this constructor.
         * 
         * @return the parameter types for this constructor.
         */
        public Class[] getParameterTypes() {
            return _parameterTypes;
        }

        /**
         * Allocates a new object using this constructor with the specified
         * arguments.
         * 
         * @param args the constructor arguments. 
         * @return the object being instantiated.
         */
        protected abstract Object allocate(Object[] args);

        /**
         * Invokes this constructor with no argument (convenience method).
         * 
         * @return the object being instantiated. 
         */
        public final Object newInstance() {
            if (_parameterTypes.length != 0) 
                throw new IllegalArgumentException(
                        "Expected number of parameters is " + _parameterTypes.length);
            return allocate(ARRAY_0);
        }

        /**
         * Invokes this constructor with the specified single argument.
         * 
         * @param arg0 the first argument. 
         * @return the object being instantiated. 
         */
        public final Object newInstance(Object arg0) {
            if (_parameterTypes.length != 1) 
                throw new IllegalArgumentException(
                        "Expected number of parameters is " + _parameterTypes.length);
            Object[] args = (Object[])ARRAY_1.get();
            args[0] = arg0;
            Object result = allocate(args);
            args[0] = null;
            return result;            
        }

        /**
         * Invokes this constructor with the specified two arguments.
         * 
         * @param arg0 the first argument. 
         * @param arg1 the second argument. 
         * @return the object being instantiated. 
         */
        public final Object newInstance(Object arg0, Object arg1) {
            if (_parameterTypes.length != 2) 
                throw new IllegalArgumentException(
                        "Expected number of parameters is " + _parameterTypes.length);
            Object[] args = (Object[])ARRAY_2.get();
            args[0] = arg0;
            args[1] = arg1;
            Object result = allocate(args);
            args[0] = null;
            args[1] = null;
            return result;            
        }

        /**
         * Invokes this constructor with the specified three arguments.
         * 
         * @param arg0 the first argument. 
         * @param arg1 the second argument. 
         * @param arg2 the third argument. 
         * @return the object being instantiated. 
         */
        public final Object newInstance(Object arg0, Object arg1, Object arg2) {
            if (_parameterTypes.length != 3) 
                throw new IllegalArgumentException(
                        "Expected number of parameters is " + _parameterTypes.length);
            Object[] args = (Object[])ARRAY_3.get();
            args[0] = arg0;
            args[1] = arg1;
            args[2] = arg2;
            Object result = allocate(args);
            args[0] = null;
            args[1] = null;
            args[2] = null;
            return result;            
        }

        /**
         * Invokes this constructor with the specified four arguments.
         * 
         * @param arg0 the first argument. 
         * @param arg1 the second argument. 
         * @param arg2 the third argument. 
         * @param arg3 the fourth argument. 
         * @return the object being instantiated. 
         */
        public final Object newInstance(Object arg0, Object arg1, Object arg2, Object arg3) {
            if (_parameterTypes.length != 4) 
                throw new IllegalArgumentException(
                        "Expected number of parameters is " + _parameterTypes.length);
            Object[] args = (Object[])ARRAY_4.get();
            args[0] = arg0;
            args[1] = arg1;
            args[2] = arg2;
            args[3] = arg3;
            Object result = allocate(args);
            args[0] = null;
            args[1] = null;
            args[2] = null;
            args[3] = null;
            return result;            
        }

    }

    /**
     * This class represents a run-time method obtained through reflection.
     * 
     * Here are few examples of utilization:[code]
     * // Non-static method: fastMap.put(myKey, myValue)
     * Reflection.Method putKeyValue  
     *     = Reflection.getMethod(
     *         "javolution.util.FastMap.put(j2me.lang.Object, j2me.lang.Object)");
     * Object previous = putKeyValue.invoke(fastMap, myKey, myValue); 
     * 
     * // Static method: System.nanoTime()  (JRE1.5+) 
     * Reflection.Method nanoTime 
     *     = Reflection.getMethod("j2me.lang.System.nanoTime()");
     * long time = ((Long)nanoTime.invoke(null)).longValue();[/code]
     */
    public static abstract class Method {

        /**
         * Holds the parameter types.
         */
        private final Class[] _parameterTypes;

        /**
         * Creates a new constructor having the specified parameter types.
         * 
         * @param parameterTypes the parameters types.
         */
        protected Method(Class[] parameterTypes) {
            _parameterTypes = parameterTypes;
        }

        /**
         * Returns an array of <code>Class</code> objects that represents 
         * the formal parameter types, in declaration order of this constructor.
         * 
         * @return the parameter types for this constructor.
         */
        public Class[] getParameterTypes() {
            return _parameterTypes;
        }

        /**
         * Executes this method with the specified arguments.
         * 
         * @param thisObject the object upon which this method is invoked
         *        or <code>null</code> for static methods.
         * @param args the method arguments. 
         * @return the result of the execution. 
         */
        protected abstract Object execute(Object thisObject, Object[] args);

        /**
         * Invokes this method on the specified object which might be 
         * <code>null</code> if the method is static (convenience method).
         * 
         * @param thisObject the object upon which this method is invoked
         *        or <code>null</code> for static methods.
         * @return the result of the invocation.
         */
        public final Object invoke(Object thisObject) {
            return execute(thisObject, ARRAY_0);
        }

        /**
         * Invokes this method with the specified single argument
         * on the specified object which might be <code>null</code>
         * if the method is static (convenience method).
         * 
         * @param thisObject the object upon which this method is invoked
         *        or <code>null</code> for static methods.
         * @param arg0 the single argument. 
         * @return the result of the invocation.
         */
        public final Object invoke(Object thisObject, Object arg0) {
            if (_parameterTypes.length != 1) 
                throw new IllegalArgumentException(
                        "Expected number of parameters is " + _parameterTypes.length);
            Object[] args = (Object[])ARRAY_1.get();
            args[0] = arg0;
            Object result = execute(thisObject, args);
            args[0] = null;
            return result;            
        }

        /**
         * Invokes this method with the specified two arguments
         * on the specified object which might be <code>null</code>
         * if the method is static (convenience method).
         * 
         * @param thisObject the object upon which this method is invoked
         *        or <code>null</code> for static methods.
         * @param arg0 the first argument. 
         * @param arg1 the second argument. 
         * @return the result of the invocation.
         * @throws RuntimeException wrapping any exception raised during 
         *         invocation (see <code>Throwable.getCause()</code>). 
         */
        public final Object invoke(Object thisObject, Object arg0, Object arg1) {
            if (_parameterTypes.length != 2) 
                throw new IllegalArgumentException(
                        "Expected number of parameters is " + _parameterTypes.length);
            Object[] args = (Object[])ARRAY_2.get();
            args[0] = arg0;
            args[1] = arg1;
            Object result = execute(thisObject, args);
            args[0] = null;
            args[1] = null;
            return result;            
        }

        /**
         * Invokes this method with the specified three arguments
         * on the specified object which might be <code>null</code>
         * if the method is static.
         * 
         * @param thisObject the object upon which this method is invoked
         *        or <code>null</code> for static methods.
         * @param arg0 the first argument (convenience method). 
         * @param arg1 the second argument. 
         * @param arg2 the third argument. 
         * @return the result of the invocation.
         */
        public final Object invoke(Object thisObject, Object arg0, Object arg1,
                Object arg2) {
            if (_parameterTypes.length != 3) 
                throw new IllegalArgumentException(
                        "Expected number of parameters is " + _parameterTypes.length);
            Object[] args = (Object[])ARRAY_3.get();
            args[0] = arg0;
            args[1] = arg1;
            args[2] = arg2;
            Object result = execute(thisObject, args);
            args[0] = null;
            args[1] = null;
            args[2] = null;
            return result;            
        }

        /**
         * Invokes this method with the specified four arguments
         * on the specified object which might be <code>null</code>
         * if the method is static (convenience method).
         * 
         * @param thisObject the object upon which this method is invoked
         *        or <code>null</code> for static methods.
         * @param arg0 the first argument. 
         * @param arg1 the second argument. 
         * @param arg2 the third argument. 
         * @param arg3 the fourth argument. 
         * @return the result of the invocation.
         */
        public final Object invoke(Object thisObject, Object arg0, Object arg1,
                Object arg2, Object arg3) {
            if (_parameterTypes.length != 3) 
                throw new IllegalArgumentException(
                        "Expected number of parameters is " + _parameterTypes.length);
            Object[] args = (Object[])ARRAY_3.get();
            args[0] = arg0;
            args[1] = arg1;
            args[2] = arg2;
            args[3] = arg3;
            Object result = execute(thisObject, args);
            args[0] = null;
            args[1] = null;
            args[2] = null;
            args[3] = null;
            return result;            
        }

    }
    
    // Holds array containers to avoid dynamic allocations.

    private static final Object[] ARRAY_0 = new Object[0]; // Immutable.

    private static final ThreadLocal ARRAY_1 = new ThreadLocal() {
        protected Object initialValue() {
            return new Object[1];
        }
    };

    private static final ThreadLocal ARRAY_2 = new ThreadLocal() {
        protected Object initialValue() {
            return new Object[2];
        }
    };

    private static final ThreadLocal ARRAY_3 = new ThreadLocal() {
        protected Object initialValue() {
            return new Object[3];
        }
    };

    private static final ThreadLocal ARRAY_4 = new ThreadLocal() {
        protected Object initialValue() {
            return new Object[4];
        }
    };
}