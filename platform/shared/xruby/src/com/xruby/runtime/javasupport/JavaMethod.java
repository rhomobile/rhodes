/** 
 * Copyright 2006-2007 Yu Su, Ye Zheng, Yu Zhang
 * Distributed under the BSD License
 */

package com.xruby.runtime.javasupport;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyData;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyExceptionValue;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.RubyVarArgMethod;

/**
 * Java Method Wrapper for both public method and constructor
 *
 * @author yu su (beanworms@gmail.com)
 */
public class JavaMethod extends RubyVarArgMethod {
    private Method method;
    private Field field;
    private Constructor constructor;
    private boolean isConstructor;
    private boolean isField;
    private boolean isGetter;

    /**
     * Public method
     *
     *  @param method method instance
     */
    public JavaMethod(Method method) {
        this.method = method;
    }

    /**
     *  Public constructor
     *  @param constructor Constructor instance
     */
    public JavaMethod(Constructor constructor) {
        this.constructor = constructor;
        this.isConstructor = true;
    }
    
    public JavaMethod(Method method,Field field,boolean isGetter){
        this.method = method;
        this.field = field;
        this.isField = true;
        this.isGetter = isGetter;
    }

    @SuppressWarnings("unchecked")
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
        Object[] arguments = JavaUtil.convertToJavaValues(args);

        try {
            if(!isConstructor) {                
                Object object = null;
                if(!(receiver instanceof JavaClass)){                    
                    if(receiver instanceof RubyExceptionValue){
                        object = ((RubyExceptionValue)receiver).getThrowable();
                    }else{
                        object = ((RubyData<Object>)receiver).getData();
                    }
                } 
                
                Object retValue = null;
                if(isField){
                    if(!(Modifier.isStatic(field.getModifiers())) && object == null){
                        throw new RubyException("The Instance variable "+field.getName()+"doesn't exists or can't be accessed!");
                    }
                    if(isGetter){
                        retValue = method.invoke(null, new Object[]{field,object});
                    }else{
                        method.invoke(null, new Object[]{field,object,arguments[0]});
                    }
                }else{
                    if(!(Modifier.isStatic(method.getModifiers())) && object == null){
                        throw new RubyException(RubyRuntime.NoMethodErrorClass, "undefined method '" + method.getName() + "' for " + ((RubyClass)receiver).getName());
                    }
                    //If the underlying method is static,object will be null!
                    retValue = method.invoke(object, arguments);
                }               
                
                return JavaUtil.convertToRubyValue(retValue);
            }
            else {
                Object instance = constructor.newInstance(arguments);
                RubyData<Object> value = new RubyJavaObject<Object>((JavaClass) receiver, instance);
                return value;
            }
        }catch (IllegalArgumentException e) {
            throw new RubyException(e.getMessage());
        } catch (InstantiationException e) {
            throw new RubyException(e.getMessage());
        } catch (IllegalAccessException e) {
            throw new RubyException(e.getMessage());
        } catch (InvocationTargetException e) {
            Throwable t = e.getTargetException();
            if(t != null){
                RubyExceptionValue exp;
                exp = new RubyExceptionValue((RubyClass)RubyRuntime.ObjectClass.getConstant(t.getClass().getName()), t.getLocalizedMessage(), t);
                throw new RubyException(exp);
            }else
                throw new RubyException(e.getMessage());
        }

    }
}
