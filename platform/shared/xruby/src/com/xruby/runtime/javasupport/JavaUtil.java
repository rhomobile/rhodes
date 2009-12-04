/**
 * Copyright 2006-2007 Yu Su, Yu Zhang
 * Distributed under the BSD License
 */

package com.xruby.runtime.javasupport;

import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.math.BigInteger;
import java.util.Iterator;
import java.util.List;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyBignum;
import com.xruby.runtime.builtin.RubyFixnum;
import com.xruby.runtime.builtin.RubyFloat;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.RubyAPI;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyID;
import com.xruby.runtime.lang.RubySymbol;
import com.xruby.runtime.lang.RubyValue;

/**
 * Helper class for Java Runtime
 *
 * @author yu su (beanworms@gmail.com), Yu Zhang (zhangyu8374@gmail.com)
 */
public class JavaUtil {

    public static RubyValue convertToRubyValue(Object value) {
        if (null == value) {
            return RubyConstant.QNIL;
        }

        if (value.getClass().equals(Integer.class) || value.getClass().equals(Byte.class) || value.getClass().equals(Short.class)) {
            int tmpVal;
            if(value instanceof Integer){
                tmpVal = ((Integer)value).intValue();
            }else if(value instanceof Byte){
                tmpVal = ((Byte)value).byteValue();
            }else {
                tmpVal = ((Short)value).shortValue();
            }
            return ObjectFactory.createFixnum(tmpVal);
        }

        if (value.getClass().equals(Double.class)) {
            return ObjectFactory.createFloat((Double) value);
        }

        if (value.getClass().equals(Float.class)) {
            return ObjectFactory.createFloat((Float) value);
        }

        if (value.getClass().equals(Boolean.class)) {
        	return ObjectFactory.createBoolean(value.equals(true));
        }

        if(value.getClass().equals(BigInteger.class)){
            return ObjectFactory.createBignum((BigInteger)value);
        }

        if(value.getClass().equals(String.class)){
            return ObjectFactory.createString((String)value);
        }

        if(value.getClass().isArray()){
            RubyArray array = new RubyArray();
            for(Object o : (Object[])value){
                array.add(convertToRubyValue(o));
            }
            return array;
        }

        // TODO: Support more data types: Hash, Array, File etc.
        //throw new IllegalArgumentException("Currently the value of Java type " + value.getClass().getName() +
        //" couldn't be changed to Ruby value");

        return new RubyJavaObject<Object>(JavaClass.createJavaClass(value.getClass()), value);
    }

    public static RubyArray convertToRubyValues(Object[] values) {
        if (null == values) {
            return new RubyArray(0);
        }

        int size = values.length;
        RubyArray retArray = new RubyArray(size);
        for (int i = 0; i < size; ++i) {
            Object obj = values[i];
            RubyValue value = convertToRubyValue(obj);
            retArray.add(value);
        }

        return retArray;
    }

    @SuppressWarnings("unchecked")
    public static Object convertToJavaValue(RubyValue value) {
        RubyClass klass;
        boolean flag = false;
        if(value instanceof JavaClass){
            klass = (JavaClass)value;
        }else{
            klass = value.getRubyClass();
            flag = true;
        }

        String className = klass.getName();

        if (className.equals("String")) {
            return ((RubyString) value).toString();
        } else if (className.equals("Fixnum")) {
            //A trick:Because don't know Fixnum actually represent
            //which Java type(byte,short,int),decide the value's type in
            //terms of the value's range.Then it can work correctly because
            //of Java's upcasting feature.

            int tmpVal = ((RubyFixnum) value).toInt();
            if(tmpVal <= Byte.MAX_VALUE && tmpVal >= Byte.MIN_VALUE)
                return (byte)tmpVal;
            if(tmpVal <= Short.MAX_VALUE && tmpVal >= Short.MIN_VALUE)
                return (short)tmpVal;
            return tmpVal;
        } else if (className.equals("Bignum")) {
            return ((RubyBignum) value).getInternal();
        } else if (className.equals("TrueClass")) {
            return true;
        } else if (className.equals("FalseClass")) {
            return false;
        } else if (className.equals("NilClass")) {
            return null;
        } else if (className.equals("Symbol")) {
            return ((RubySymbol) value).toString();
        } else if (className.equals("Exception")) {
            return new Exception(value.toString());
        } else if (className.equals("Float")) {
            return ((RubyFloat)value).doubleValue();
        } else if (className.equals("Array")){
            RubyArray array = (RubyArray)value;
            boolean first = true;
            Object[] objs = null;
            int i = 0;
            Iterator<RubyValue> iterator = array.iterator();
            while(iterator.hasNext()){
                Object obj = convertToJavaValue(iterator.next());
                if(first){
                    objs = (Object[])Array.newInstance(obj.getClass(),array.size());
                    first = false;
                }
                objs[i++] = obj;
            }
            return objs;

        } else if (className.equals("Regexp")) {
            // TODO:Convert to Java's regular expression
        }

        //Convert Ruby class that implements Java interface to Java class
        while(!klass.getName().equals("Object")){
            if(klass instanceof JavaClass){
                final RubyValue tmp = value;
                Class clazz = ((JavaClass)klass).getOriginJavaClass();

                if(clazz.isInterface()){
                    if(flag){
                        return Proxy.newProxyInstance(JavaUtil.class.getClassLoader(),new Class[]{clazz},new InvocationHandler(){
                            public Object invoke(Object proxy, Method method, Object[] nargs) throws Throwable {
                                return RubyAPI.callPublicMethod(tmp, convertToRubyValues(nargs), null, RubyID.intern(method.getName()));
                            }
                        });
                    }else{
                        return clazz;
                    }

                }else{
                    return ((JavaClass)klass).getOriginJavaClass();
                }

            }else{
                klass = klass.getSuperClass();
            }
        }

        if(value instanceof RubyJavaObject){
            return ((RubyJavaObject<Object>) value).getData();
        }else{
            throw new IllegalArgumentException("Ruby type " + className +
                       " couldn't be passed to java method");
        }
    }

    public static Object[] convertToJavaValues(RubyArray array) {
        if (null == array) {
            return new Object[0];
        }

        int size = array.size();
        Object[] retValues = new Object[size];
        for (int i = 0; i < size; ++i) {
            RubyValue value = array.get(i);
            Object obj = convertToJavaValue(value);
            retValues[i] = obj;
        }

        return retValues;
    }


    private static String[] collectTypes(RubyArray args) {
        String[] tmp = new String[50];
        int i = 0;

        for (RubyValue value : args) {
            String className;
            if(value instanceof JavaClass){
                className = "java.lang.Class";
            }else{
                RubyClass klass = value.getRubyClass();
                className = klass.getName();

                while(!klass.getName().equals("Object")){
                    if(klass instanceof JavaClass){
                        className = ((JavaClass)klass).getOriginJavaClass().getName();
                        break;
                    }else{
                        klass = klass.getSuperClass();
                    }
                }
            }
            tmp[i] = className;
            ++i;
        }

        String[] types = new String[i];
        System.arraycopy(tmp, 0, types, 0, i);

        return types;
    }

    private static boolean matchNativeTye(String type, Class clazz) {
        if (type.equals("String")) {
            if (clazz.equals(String.class)) {
                return true;
            }
        }
        if(type.equals("Fixnum")) {
            if (clazz.equals(Integer.class)) {
                return true;
            }

            //Fixnum presents int,short,byte because of the range of
            //Fixnum is same as int in Java
            if(clazz.isPrimitive()){
                if(clazz.equals(Integer.TYPE) || clazz.equals(Short.TYPE) || clazz.equals(Byte.TYPE)){
                    return true;
                }
            }
        }
        if(type.equals("Bignum")) {
            if (clazz.equals(BigInteger.class)) {
                return true;
            }
        }
        if(type.equals("TrueClass")) {
            if (clazz.equals(Boolean.class)) {
                return true;
            }
        }
        if(type.equals("FalseClass")) {
            if (clazz.equals(Boolean.class)) {
                return true;
            }
        }
        if(type.equals(clazz.getName())){
            return true;
        }
        //TODO More type
        return false;
    }

    public static Method matchMethod(List<Method> methods, RubyArray args) {
        String[] types = collectTypes(args);

        for (Method method : methods) {
            Class[] params = method.getParameterTypes();
            boolean match = true;

            for (int i = 0; i < types.length && match; i++) {
                if (!matchNativeTye(types[i], params[i])) {
                    match = false;
                }
            }

            if (match) {
                return method;
            }
        }

        throw new RubyException("Couldn't find method ");
    }

    public static Constructor matchInitMethod(List<Constructor> methods, RubyArray args) {
        String[] types = collectTypes(args);

        for (Constructor method : methods) {
            Class[] params = method.getParameterTypes();
            boolean match = true;

            for (int i = 0; i < types.length && match; i++) {
                if (!matchNativeTye(types[i], params[i])) {
                    match = false;
                }
            }

            if (match) {
                return method;
            }
        }

        throw new RubyException("Couldn't find method ");
    }
}

