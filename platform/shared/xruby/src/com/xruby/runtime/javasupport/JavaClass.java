/**
 * Copyright 2006-2007 Yu Su, Ye Zheng, Yu Zhang
 * Distributed under the BSD License
 */

package com.xruby.runtime.javasupport;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyAPI;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyID;
import com.xruby.runtime.lang.RubyMethod;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubySingletonClass;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.RubyVarArgMethod;

/**
 * Wrapper for Java Class
 *
 * @author Yu Su (beanworms@gmail.com), Ye Zheng(dreamhead.cn@gmail.com),
 * Yu Zhang(zhangyu8374@gmail.com)
 */
public class JavaClass extends RubyClass {

    // ----------------------
    //      Const fields
    // ----------------------
    private static final String NEW_METHOD = "new";

    // ---------------------------------------
    //   Cache of methods (and constructors)
    // ---------------------------------------
    private Map<String, List<Method>> methodMap
            = new HashMap<String, List<Method>>();

    private Map<Integer, List<Constructor>> initMap
            = new HashMap<Integer, List<Constructor>>();

    private Map<Method, JavaMethod> javaMethods
            = new HashMap<Method, JavaMethod>();

    private Map<Constructor, JavaMethod> initMethods
            = new HashMap<Constructor, JavaMethod>();

    private Map<String, Field> fieldNames
            = new HashMap<String, Field>();

    private static List<String> packageNames = new ArrayList<String>();

    private Class orginJavaClass;

    private JavaClass(Class clazz,RubyClass superclass){
        super(clazz.getName(),superclass,null);

        this.orginJavaClass = clazz;
        if(!clazz.isInterface()){
            //Initialize public constructors and methods
            initConstructors(clazz);
            initMethods(clazz);
            initFields(clazz);
        }
        setRubyClass(this);
    }


    private static JavaClass newJavaClass(Class clazz,RubyClass parent){
        JavaClass jClass = new JavaClass(clazz,parent);
        if(clazz.isInterface()){
            jClass.setRubyClass(RubyRuntime.ClassClass);
            new RubySingletonClass(jClass, parent.getRubyClass(), null);
        }

        String fullName = clazz.getName();
        //Replace new Java class name prefixed 'J' with orginal class name
        //if the class name collision occurs
        RubyAPI.setTopLevelConstant(jClass, fullName);
        String name = fullName.substring(fullName.lastIndexOf('.')+1);
        //Check that the class named <code>name</code> exists or not
        RubyValue value = RubyRuntime.ObjectClass.getConstant(name);
        if(value == null)
            RubyAPI.setTopLevelConstant(jClass, name);
        RubyAPI.setTopLevelConstant(jClass, "J"+name);
        return jClass;
    }

    //Import Java class,the parent class,etc. recursively
    public static JavaClass createJavaClass(Class clazz){
        Class superClass = clazz.getSuperclass();
        if(superClass != null){
            JavaClass parentClass = createJavaClass(superClass);
            return newJavaClass(clazz,parentClass);
        }else{
            return newJavaClass(clazz,RubyRuntime.ObjectClass);
        }
    }

    // Collect public methods of given class
    private void initMethods(Class clazz) {
        Method[] methods = clazz.getDeclaredMethods();

        for (Method method : methods) {
            int modifiers = method.getModifiers();
            if (Modifier.isPublic(modifiers)) {
                categoryByName(method);
            }
        }
    }

    // Collect public constructors of given class
    private void initConstructors(Class clazz) {
        Constructor[] constructors = clazz.getConstructors();

        for (Constructor constructor : constructors) {
            int modifiers = constructor.getModifiers();
            if (Modifier.isPublic(modifiers)) {
                categoryByParams(constructor);
            }
        }
    }

    private void initFields(Class clazz){
        Field[] fields = clazz.getDeclaredFields();

        for(Field f : fields){
            String name = f.getName();
            int modifiers = f.getModifiers();
            //Unlike the constructor and method,it's impossible
            //to have two or more fields with the same name
            fieldNames.put(name, f);
            if(!Modifier.isFinal(modifiers)){
                String setterName = name+"=";
                fieldNames.put(setterName,f);
            }

            if(Modifier.isStatic(modifiers)&&Modifier.isPublic(modifiers)){
                if(Modifier.isFinal(modifiers)){
                    try {
                        //Java class's constant fields.
                        setConstant(name,JavaUtil.convertToRubyValue(f.get(null)));
                    } catch (IllegalArgumentException e) {
                        e.printStackTrace();
                    } catch (IllegalAccessException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }


    // Helper class, catalogue the method
    private void categoryByName(Method method) {
        String name = method.getName();

        List<Method> list = methodMap.get(name);
        if (null == list) {
            list = new ArrayList<Method>();
            list.add(method);
            methodMap.put(name, list);
        } else {
            list.add(method);
        }
    }

    private void categoryByParams(Constructor constructor) {
        Class[] types = constructor.getParameterTypes();
        int size = types.length;
        List<Constructor> list = initMap.get(size);

        if (null == list) {
            list = new ArrayList<Constructor>();
            list.add(constructor);
            initMap.put(size, list);
        } else {
            list.add(constructor);
        }

    }

    /**
     * Find method according to given method, if it's "new"
     * return an "init" fake method
     *
     * @param mid method's RubyID
     * @return wrapper of the method, otherwise null
     */
    public RubyMethod findOwnPublicMethod(RubyID mid) {
        String methodName = mid.toString();
        if(methodName == null) {
            return null;
        }

        if (methodName.equals(NEW_METHOD)) {
            return new FakeInitMethod();
        }

        RubyClass klass = this;
        while(klass != null){
            if(klass instanceof JavaClass){
                if(((JavaClass)klass).methodMap.containsKey(methodName)){
                    return new FakeMethod(methodName);
                }
                if(((JavaClass)klass).fieldNames.keySet().contains(methodName)){
                    return new FakeInstanceVarMethod(methodName);
                }
            }else{
                return klass.findOwnPublicMethod(mid);
            }
            klass = klass.getSuperClass();
        }

        return null;
    }

    private JavaMethod getJavaMethod(Method method) {
        JavaMethod jMethod = javaMethods.get(method);

        if (null == jMethod) {
            jMethod = new JavaMethod(method);
            javaMethods.put(method, jMethod);
        }

        return jMethod;
    }


    // TODO: InComplete, method cache is required(indexed by params' number)
    JavaMethod findJavaMethod(String methodName, RubyArray args) {
        int size = args == null ? 0 : args.size();

        List<Method> list = null;
        RubyClass klass = this;
        while(klass != null){
            list = ((JavaClass)klass).methodMap.get(methodName);
            if (null != list) {
                break;
            }else{
                klass = klass.getSuperClass();
            }
        }

        if (null == list) {
            return null;
        }

        if (list.size() == 1) {
            Method method = list.get(0);
            Class[] params = method.getParameterTypes();
            if (size != params.length) {
                throw new RubyException("Illegal arguments");
            }

            return getJavaMethod(method);
        } else {
            List<Method> tmpList = new ArrayList<Method>();
            for (Method method : list) {
                if (method.getParameterTypes().length == size) {
                    tmpList.add(method);
                }
            }
            if (tmpList.size() == 1) {
                return getJavaMethod(tmpList.get(0));
            } else {
                //Analyzing args
                return getJavaMethod(JavaUtil.matchMethod(tmpList, args));
            }
        }
    }

    JavaMethod getJavaConstructor(Constructor constructor) {
        JavaMethod jMethod = initMethods.get(constructor);

        if (null == jMethod) {
            jMethod = new JavaMethod(constructor);
            initMethods.put(constructor, jMethod);
        }

        return jMethod;
    }


    JavaMethod findInitMethod(RubyArray args) {
        int size = args == null ? 0 : args.size();
        List<Constructor> list = initMap.get(size);

        if (null == list) {
            return null;
        }

        if (list.size() == 1) {
            Constructor constructor = list.get(0);
            return getJavaConstructor(constructor);
        } else {
            return getJavaConstructor(JavaUtil.matchInitMethod(list, args));
        }
    }

    private class FakeMethod extends RubyVarArgMethod {
        private String methodName;

        public FakeMethod(String methodName) {
            this.methodName = methodName;
        }

        protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
            JavaClass clazz = null;
            //This is a trick.Because no creating the metaclass for every Java
            //class,deal with the static methods as follows:
            if(receiver instanceof JavaClass){
                clazz = (JavaClass)receiver;
            }else{
                clazz = (JavaClass) receiver.getRubyClass();
            }
            JavaMethod method = clazz.findJavaMethod(methodName, args);

            if (null == method) {
                throw new RubyException("Signature of method " + methodName + " is illegal");
            }

            return method.run(receiver, args, block);
        }
    }

    private class FakeInitMethod extends RubyVarArgMethod {
        protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
            JavaClass clazz = (JavaClass) receiver;
            JavaMethod method = clazz.findInitMethod(args);

            if (null == method) {
                throw new RubyException("Signature is illegal");
            }

            return method.run(receiver, args, block);
        }
    }


    private class FakeInstanceVarMethod extends RubyVarArgMethod {
        private String instanceVarName;

        public FakeInstanceVarMethod(String name) {
            this.instanceVarName = name;
        }

        protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
            JavaClass clazz = null;
            //This is a trick.Because no creating the metaclass for every Java
            //class,deal with the static fields as follows:
            if(receiver instanceof JavaClass){
                clazz = (JavaClass)receiver;
            }else{
                clazz = (JavaClass) receiver.getRubyClass();
            }
            JavaMethod method = null;
            boolean flag = false;
            do{
                if(clazz.fieldNames.keySet().contains(instanceVarName)){
                    Method m = null;
                    boolean isGetter = true;
                    try {
                        if(instanceVarName.endsWith("=")){
                            isGetter = false;
                            m = JavaClass.class.getDeclaredMethod("setter", new Class[]{Field.class,Object.class,Object.class});
                        }else{
                            m = JavaClass.class.getDeclaredMethod("getter", new Class[]{Field.class,Object.class});
                        }
                    } catch (SecurityException e) {
                        e.printStackTrace();
                    } catch (NoSuchMethodException e) {
                        e.printStackTrace();
                    }
                    method = new JavaMethod(m,clazz.fieldNames.get(instanceVarName),isGetter);
                    break;
                }
                clazz = (JavaClass)clazz.getSuperClass();
                flag = clazz.getName().equals("Object");
            }while(!flag);

            if(flag)
                throw new RubyException("The Instance variable "+instanceVarName+"doesn't exists or can't be accessed!");

            return method.run(receiver, args, block);
        }

    }

    public static Object getter(Field f,Object obj){
        try {
            return f.get(obj);
        } catch (IllegalArgumentException e) {
            //ignore it
        } catch (IllegalAccessException e2) {
            //If the Java class define a getter method by itself,...
            String name = f.getName();
            StringBuffer sb = new StringBuffer();
            sb.append("get");
            sb.append(Character.toUpperCase(name.charAt(0)));
            sb.append(name.substring(1));
            String methodName = sb.toString();

            try {
                //Suppose that getter method is not overload!
                Method[] ms = obj.getClass().getMethods();
                for(Method m : ms){
                    if(m.getName().equals(methodName)){
                        return m.invoke(obj, new Object[]{});
                    }
                }
            } catch (Exception e1) {
                //ignore it
            }
        }
        throw new RubyException("The Instance variable "+f.getName()+"doesn't exists or can't be accessed!");
    }

    public static void setter(Field f,Object obj,Object val){
        try {
            f.set(obj, val);
            return;
        } catch (IllegalArgumentException e) {
            //ignore it
        } catch (IllegalAccessException e) {
            //If the Java class define a setter method by itself,...
            String name = f.getName();
            StringBuffer sb = new StringBuffer();
            sb.append("set");
            sb.append(Character.toUpperCase(name.charAt(0)));
            sb.append(name.substring(1));
            String methodName = sb.toString();

            try {
                //Suppose that setter method is not overload!
                Method[] ms = obj.getClass().getMethods();
                for(Method m : ms){
                    if(m.getName().equals(methodName)){
                        m.invoke(obj, new Object[]{val});
                        return;
                    }
                }
            } catch (Exception e1) {
                //ignore it
            }
        }
        throw new RubyException("The Instance variable "+f.getName()+"doesn't exists or can't be accessed!");
    }

    public static void addPackage(String name){
        packageNames.add(name);
    }

    public static String[] getPackeageList(){
        String[] tmp = new String[packageNames.size()];
        Iterator<String> iter = packageNames.iterator();
        int count = 0;
        while(iter.hasNext()){
            tmp[count] = iter.next();
            count++;
        }
        return tmp;
    }

    public Class getOriginJavaClass(){
        return this.orginJavaClass;
    }
}



