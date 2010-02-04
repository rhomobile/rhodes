/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

//import j2me.io.BufferedReader;
//import java.io.IOException;
//import java.io.InputStreamReader;
import j2me.util.Iterator;
import j2me.lang.AssertMe;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyMethodValue;
import com.xruby.runtime.builtin.RubyProc;
import com.xruby.runtime.builtin.RubyString;
//import com.xruby.runtime.javasupport.JavaClass;
import com.xruby.runtime.builtin.RubyTypesUtil;

public class RubyAPI {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RubyAPI");
	
    public static RubyClass defineClass(String name, RubyClass superclass) {
        return ClassFactory.defineClass(name, superclass);
    }

    /* For compiler */
    public static RubyClass defineClass(String name, RubyValue superclass) {
        if (null != superclass && !(superclass instanceof RubyClass)) {
            throw new RubyException(RubyRuntime.TypeErrorClass, "superclass must be a Class (" + superclass.getRubyClass().getName() + " given)");
        }

        RubyValue value = RubyRuntime.ObjectClass.getConstant(name);
        if (value != null) {
            if (!(value instanceof RubyClass)) {
                throw new RubyException(RubyRuntime.TypeErrorClass, name + " is not a class");
            }

            RubyClass klass = (RubyClass)value;

            if (superclass != null) {
                if (superclass != klass.getSuperClass().getRealClass()) {
                    throw new RubyException(RubyRuntime.TypeErrorClass, "superclass mismatch for class " + name);
                }
            }

            klass.setAccessPublic();

            return klass;
        }

        return defineClass(name, null == superclass ? null : (RubyClass)superclass);
    }

    public static RubyModule defineModule(String name) {
        return ClassFactory.defineModule(name);
    }

    public static boolean testCaseEqual(RubyValue value1, RubyValue value2) {
        return RubyAPI.callPublicOneArgMethod(value1, value2, null, RubyID.longEqualID).isTrue();
    }

    public static boolean testCaseEqual(RubyArray values, RubyValue value2) {
//        for (RubyValue value1 : values) {
        for (Iterator iter = values.iterator(); iter.hasNext();) {
        	RubyValue value1 = (RubyValue)iter.next();

            if (RubyAPI.callPublicOneArgMethod(value1, value2, null, RubyID.longEqualID).isTrue()) {
                return true;
            }
        }

        return false;
    }

    public static boolean testEqual(RubyValue value1, RubyValue value2) {
        return RubyAPI.callPublicOneArgMethod(value1, value2, null, RubyID.equalID).isTrue();
    }

    public static boolean testCaseEqualNotNil(RubyValue value1, RubyValue value2) 
    {
        return value1.isTrue();
    }
    
    public static boolean testExceptionType(RubyArray classes_to_compare, RubyException e) {
        RubyValue value = e.getRubyValue();
//        for (RubyValue class_to_compare : classes_to_compare) {
        for (Iterator iter = classes_to_compare.iterator(); iter.hasNext();) {
        	RubyValue class_to_compare = (RubyValue)iter.next();
        
            if (RubyAPI.isKindOf(class_to_compare, value)) {
                return true;
            }
        }

        return false;
    }

    public static boolean isKindOf(RubyValue class_to_compare, RubyValue value) {
        if (class_to_compare == value) {
            return true;
        } else if (!(class_to_compare instanceof RubyModule)) {
            //TODO should test include relationship with Module
            return false;
        } else {
            return value.getRubyClass().isKindOf((RubyModule) class_to_compare);
        }
    }

    //e.g. defined? super
    public static RubyValue isDefinedSuperMethod(RubyValue receiver, String method_name, RubyMethod current_method) {
        RubyClass c = (RubyClass) current_method.getScope();
        RubyID mid = RubyID.intern(method_name);
        RubyMethod m = c.findSuperMethod(mid);
        if (null == m || UndefMethod.isUndef(m)) {
            return RubyConstant.QNIL;
        }

        return ObjectFactory.createString("super");
    }

    //e.g. defined? a.f
    //TODO bad name, should be isDefinedMethodWithReceiver
    public static RubyValue isDefinedPublicMethod(RubyValue receiver, RubyModule owner, String method_name) {
        if (receiver.getRubyClass() == owner) {
            //TODO do not have to be equal
            return isDefinedMethod(receiver, method_name);
        }

        RubyID mid = RubyID.intern(method_name);
        RubyMethod m = receiver.findPublicMethod(mid);
        if (null == m || UndefMethod.isUndef(m)) {
            return RubyConstant.QNIL;
        }

        return ObjectFactory.createString("method");
    }

    //e.g. defined? f
    public static RubyValue isDefinedMethod(RubyValue receiver, String method_name) {
        RubyID mid = RubyID.intern(method_name);
        RubyMethod m = receiver.findMethod(mid);
        if (null == m || UndefMethod.isUndef(m)) {
            return RubyConstant.QNIL;
        }

        return ObjectFactory.createString("method");
    }

    //e.g. defined? @m    
    public static RubyValue isDefinedInstanceVariable(RubyValue receiver, String method_name) {
        RubyID mid = RubyID.intern(method_name);
        RubyValue var = receiver.getInstanceVariable(mid);
        if (null == var || var == RubyConstant.QNIL) {
            return RubyConstant.QNIL;
        }

        return ObjectFactory.createString("instance-variable");
    }
    
    public static RubyValue isDefinedNonPrivateMethod(RubyValue receiver, String method_name) {
        RubyID mid = RubyID.intern(method_name);
        RubyMethod m = receiver.findMethod(mid);
        if (null == m || UndefMethod.isUndef(m) || m.getAccess() == RubyMethod.PRIVATE) {
            return RubyConstant.QNIL;
        }

        return ObjectFactory.createString("method");
    }
    
    public static RubyValue isDefinedYield(RubyBlock block) {
        if (null == block) {
            return RubyConstant.QNIL;
        } else {
            return ObjectFactory.createString("yield");
        }
    }

    private static RubyValue callMethodMissing(RubyValue receiver, RubyArray args, RubyBlock block, RubyID mid) {
        RubyMethod m = receiver.findMethod(RubyID.methodMissingId);
        if (null != m && !UndefMethod.isUndef(m)) {
            if (null == args) {
                args = new RubyArray(ObjectFactory.createSymbol(mid));
            } else {
                args.insert(0, ObjectFactory.createSymbol(mid));
            }
            return m.invoke(receiver, args, block);
        }

        RubyClass klass = receiver.getRubyClass();
        klass = (klass != null) ? klass.getRealClass() : null;
        throw new RubyException(RubyRuntime.NoMethodErrorClass, "undefined method '" + mid.toString() + "' for " + klass.getName());
    }

    private static RubyValue processException( Exception e, RubyValue receiver, RubyID mid )throws RubyException
    {
    	if ( e instanceof RubyException )
    	{
    		RubyException exc = (RubyException)e;
    		if ( exc.getRubyValue().getRubyClass() == RubyRuntime.EOFErrorClass )
    			throw exc;
    	}
    	
		String className = "Unknown::";
		
		if ( receiver instanceof RubyClass)
			className = ((RubyClass)receiver).getName() + "::";
		else
		{
			if ( receiver.getRubyClass() != null )
				className = receiver.getRubyClass().getName() + ".";
		}
		
		String strTraceMsg = className + mid.toString();
		String strErrMsg = strTraceMsg + " failed.";
		
		if ( e instanceof RubyException )
		{
			RubyException exc = (RubyException)e;
			exc.getRubyValue().addBacktrace(strTraceMsg);
			LOG.ERROR( strTraceMsg + " failed: " + e.getMessage() );
		}else
			LOG.ERROR( strErrMsg, e);
		
		throw (e instanceof RubyException ? (RubyException)e : new RubyException( strErrMsg + e.getMessage()));
    }
    
    //receiver is implicit self
    public static RubyValue callMethod(RubyValue receiver, RubyArray args, RubyBlock block, RubyID mid) 
    {
    	try{
	    	AssertMe.rho_assert(null == args || args.size() > 1);//use callOneArgMethod if has only one arg
	        RubyMethod m = receiver.findMethod(mid);
	        if (null != m && !UndefMethod.isUndef(m)) {
	            return m.invoke(receiver, args, block);
	        }
	
	        return callMethodMissing(receiver, args, block, mid);
    	}catch(Exception e)
		{
    		return processException(e,receiver,mid);
		}

    }

    public static RubyValue callNoArgMethod(RubyValue receiver, RubyBlock block, RubyID mid) 
    {
    	try{
	        RubyMethod m = receiver.findMethod(mid);
	        if (null != m && !UndefMethod.isUndef(m)) {
	            return m.invoke(receiver, block);
	        }
	
	        return callMethodMissing(receiver, null, block, mid);
    	}catch(Exception e)
		{
    		return processException(e,receiver,mid);
		}
	        
    }

    //method call with *one* argument and no block (use the other one if no arg (arg == null)!)
    //This make code (especially reverse engineered ones) more readable.
    public static RubyValue callOneArgMethod(RubyValue receiver, RubyValue arg, RubyBlock block, RubyID mid) 
    {
    	try{
	    	AssertMe.rho_assert(null != arg);
	        RubyMethod m = receiver.findMethod(mid);
	        if (null != m && !UndefMethod.isUndef(m)) {
	            return m.invoke(receiver, arg, block);
	        }
	
	        return callMethodMissing(receiver, new RubyArray(arg), block, mid);
    	}catch(Exception e)
		{
    		return processException(e,receiver,mid);
		}
	        
    }
    
    public static RubyValue callTwoArgMethod(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block, RubyID mid) 
    {
    	try{
	    	AssertMe.rho_assert(null != arg0 && null != arg1);
	        RubyMethod m = receiver.findMethod(mid);
	        if (null != m && !UndefMethod.isUndef(m)) {
	            return m.invoke(receiver, arg0, arg1, block);
	        }
	
	        return callMethodMissing(receiver, new RubyArray(arg0, arg1), block, mid);
    	}catch(Exception e)
		{
    		return processException(e,receiver,mid);
		}
	        
    }

    public static RubyValue callPublicNoArgMethod(RubyValue receiver, RubyBlock block, RubyID mid) 
    {
    	try{
    		RubyValue res = receiver.rhom_processProperty(mid);
    		if ( res != null )
    			return res;
    		
	        RubyMethod m = receiver.findPublicMethod(mid);
	        if (null != m && !UndefMethod.isUndef(m)) {
	            return m.invoke(receiver, block);
	        }
	
	        return callMethodMissing(receiver, null, block, mid);
    	}catch(Exception e)
		{
    		return processException(e,receiver,mid);
		}
	        
    }

    //method call with *one* argument and no block (use the other one if no arg (arg == null)!)
    //This make code (especially reverse engineered ones) more readable.
    public static RubyValue callPublicOneArgMethod(RubyValue receiver, RubyValue arg, RubyBlock block, RubyID mid) 
    {
    	try{
	    	AssertMe.rho_assert(null != arg);
	    	RubyMethod m = receiver.findPublicMethod(mid);
	    	if (null != m && !UndefMethod.isUndef(m)) {
	    		return m.invoke(receiver, arg, block);
	    	}
	
	    	return callMethodMissing(receiver, new RubyArray(arg), block, mid);
    	}catch(Exception e)
		{
    		return processException(e,receiver,mid);
		}
	    	
    }
    
    public static RubyValue callPublicTwoArgMethod(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block, RubyID mid) 
    {
    	try{
	    	AssertMe.rho_assert(null != arg0 && null != arg1);
	    	RubyMethod m = receiver.findPublicMethod(mid);
	    	if (null != m && !UndefMethod.isUndef(m)) {
	    		return m.invoke(receiver, arg0, arg1, block);
	    	}
	
	    	return callMethodMissing(receiver, new RubyArray(arg0, arg1), block, mid);
    	}catch(Exception e)
		{
    		return processException(e,receiver,mid);
		}
	    	
    }

    //TODO should pass owner to work with protected method
    public static RubyValue callPublicMethod(RubyValue receiver, RubyArray args, RubyBlock block, RubyID mid) {
    	//RHO
   		//AssertMe.rho_assert(null == args || args.size() > 1);//use callPublicOneArgMethod if has only one arg
    	//RHO
    	try{
	        RubyMethod m = receiver.findPublicMethod(mid);
	        if (null != m && !UndefMethod.isUndef(m)) {
	            return m.invoke(receiver, args, block);
	        }
	
	        return callMethodMissing(receiver, args, block, mid);
    	}catch(Exception e)
		{
    		return processException(e,receiver,mid);
		}
	        
    }

    public static RubyValue callSuperNoArgMethod(RubyValue receiver, RubyBlock block, MethodBlockBase mbb) 
    {
    	try{
	        RubyClass c = (RubyClass) mbb.getScope();
	        RubyMethod m = c.findSuperMethod(mbb.getID());
	        if (null == m || UndefMethod.isUndef(m)) {
	            throw new RubyException(RubyRuntime.NoMethodErrorClass, "super method '" + mbb.getID() + "' can not be found in '" + c.getName() + "'");
	        }
	
	        return m.invoke(receiver, block);
    	}catch(Exception e)
		{
    		return processException(e,receiver,mbb.getID());
		}
	        
    }

    public static RubyValue callSuperOneArgMethod(RubyValue receiver, RubyValue arg, RubyBlock block, MethodBlockBase mbb) 
    {
    	try{
	    	AssertMe.rho_assert(null != arg);
	        RubyClass c = (RubyClass) mbb.getScope();
	        RubyMethod m = c.findSuperMethod(mbb.getID());
	        if (null == m || UndefMethod.isUndef(m)) {
	            throw new RubyException(RubyRuntime.NoMethodErrorClass, "super method '" + mbb.getID() + "' can not be found in '" + c.getName() + "'");
	        }
	
	        return m.invoke(receiver, arg, block);
    	}catch(Exception e)
		{
    		return processException(e,receiver,mbb.getID());
		}
	        
    }
    
    public static RubyValue callSuperTwoArgMethod(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block, MethodBlockBase mbb) 
    {
    	try{
	    	AssertMe.rho_assert(null != arg0 && null != arg1);
	        RubyClass c = (RubyClass) mbb.getScope();
	        RubyMethod m = c.findSuperMethod(mbb.getID());
	        if (null == m || UndefMethod.isUndef(m)) {
	            throw new RubyException(RubyRuntime.NoMethodErrorClass, "super method '" + mbb.getID() + "' can not be found in '" + c.getName() + "'");
	        }
	
	        return m.invoke(receiver, arg0, arg1, block);
    	}catch(Exception e)
		{
    		return processException(e,receiver,mbb.getID());
		}
	        
    }

    public static RubyValue callSuperMethod(RubyValue receiver, RubyArray args, RubyBlock block, MethodBlockBase mbb) 
    {
    	try{
	    	AssertMe.rho_assert(null == args || args.size() > 1);//use callSuperOneArgMethod if has only one arg
	        RubyClass c = (RubyClass) mbb.getScope();
	        RubyMethod m = c.findSuperMethod(mbb.getID());
	        if (null == m || UndefMethod.isUndef(m)) {
	            throw new RubyException(RubyRuntime.NoMethodErrorClass, "super method '" + mbb.getID() + "' can not be found in '" + c.getName() + "'");
	        }
	
	        return m.invoke(receiver, args, block);
    	}catch(Exception e)
		{
    		return processException(e,receiver,mbb.getID());
		}
	        
    }
    
    public static RubyValue operatorNot(RubyValue value) {
        return value.isTrue() ? RubyConstant.QFALSE : RubyConstant.QTRUE;
    }

    public static boolean isWindows() {
    	return false;
/*    	String strOS = System.getProperty("os.name");
    	if ( strOS == null )
    		return false;
    	
        return (strOS.toUpperCase().indexOf("WINDOWS") >= 0);*/
    }

    public static RubyValue runCommandAndCaptureOutput(String value) {
    	//RHO_COMMENT: runCommandAndCaptureOutput
    	return ObjectFactory.createString(new StringBuffer());
        //some commands are builtin within the shell. e,g. echo, copy...
/*    	String[] cmdarray = new String[3];
    	cmdarray[2] = value;
        if (isWindows()) {
        	cmdarray[0] = "cmd";
        	cmdarray[1] = "/c";
        } else {
        	cmdarray[0] = "/bin/sh";
        	cmdarray[1] = "-c";
        }

        try {
            Process p = Runtime.getRuntime().exec(cmdarray);
            StringBuffer output = new StringBuffer();

            BufferedReader stderr = new BufferedReader(new InputStreamReader(p.getErrorStream()));
            int c;
            while ((c = stderr.read()) != -1) {
                if (c != '\r') {
                    output.append((char) c);
                }
            }
            stderr.close();

            BufferedReader stdout = new BufferedReader(new InputStreamReader(p.getInputStream()));
            while ((c = stdout.read()) != -1) {
                if (c != '\r') {
                    output.append((char) c);
                }
            }
            stdout.close();

            return ObjectFactory.createString(output);
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.ExceptionClass, e.toString());
        }*/
    }

    public static RubyValue expandArrayIfThereIsZeroOrOneValue(RubyArray a) {
        if (a.size() <= 1) {
            return a.get(0);
        }

        return a;
    }

    public static RubyValue expandArrayIfThereIsZeroOrOneValue(RubyValue v) {
        if (v instanceof RubyArray) {
            RubyArray a = (RubyArray) v;
            if (a.isSingleAsterisk()) {
                return expandArrayIfThereIsZeroOrOneValue(a);
            }
        }

        return v;
    }

    public static RubyArray expandArrayIfThereIsOnlyOneRubyArray(RubyArray a) {
        if (a.size() == 1 &&
                a.isSingleRhs() &&
                a.get(0) instanceof RubyArray) {
            return (RubyArray) a.get(0);
        }

        return a;
    }

    public static RubyArray convertToArrayIfNotYet(RubyValue v) {
        if (v instanceof RubyArray) {
            return (RubyArray) v;
        }

        return new RubyArray(v);
    }

    public static RubyBlock convertRubyValue2RubyBlock(RubyValue v) {
        if (RubyConstant.QNIL == v) {
            return null;
        } else if (v instanceof RubyMethodValue) {
            return ((RubyMethodValue) v).convertToRubyProc().getBlock();
        } else if (v instanceof RubyProc) {
            return ((RubyProc) v).getBlock();
        } else {
            throw new RubyException(RubyRuntime.TypeErrorClass, "wrong argument type " + v.getRubyClass().getName() + " (expected Proc) ");
        }
    }

    public static RubyValue convertRubyException2RubyValue(RubyException e) {
        return e.getRubyValue();
    }

    public static RubyValue setTopLevelConstant(RubyValue value, String name) {
        return RubyRuntime.ObjectClass.setConstant(name, value);
    }

    public static RubyValue getCurrentNamespaceConstant(RubyModule receiver, String name) {
        RubyValue v = receiver.getConstant(name);
        if (null != v) {
            return v;
        }

        v = RubyRuntime.ObjectClass.getConstant(name);
        if (null == v) {
        	//RHO_COMMENT: RubyRuntime.javaIsSupported()
/*            if(RubyRuntime.javaIsSupported()){
//                for(String pkgName :JavaClass.getPackeageList()){
            	  for(int i = 0; i < JavaClass.getPackeageList().length;i++){
            		  String pkgName = JavaClass.getPackeageList()[i];  
            	  
                    try {
                        Class clazz = Class.forName(pkgName+"."+name);
                        return JavaClass.createJavaClass(clazz);
                    } catch (ClassNotFoundException e) {
                        //do nothing
                    }
                }
            }*/
            throwUninitializedConstant(receiver, name);
        }

        return v;
    }

    public static RubyValue isDefinedCurrentNamespaceConstant(RubyValue receiver, String name) {
        RubyValue v = ((RubyModule) receiver).getConstant(name);
        if (null == v) {
            return RubyConstant.QNIL;
        }

        return ObjectFactory.createString("constant");
    }

    public static RubyValue getConstant(RubyValue receiver, String name) {
        throwTypeErrorIfNotClassModule(receiver);

        RubyModule m = (RubyModule) receiver;
        RubyValue v = m.getConstant(name);
        if (null == v) {
            throwUninitializedConstant(m, name);
        }

        return v;
    }

    //RHO_COMMENT: isConstantDefined
    public static RubyValue isConstantDefined(RubyValue receiver, String name) {
        throwTypeErrorIfNotClassModule(receiver);

        RubyModule m = (RubyModule) receiver;
        RubyValue v = m.getConstant(name);
        if (null == v)
            return RubyConstant.QFALSE;

        return RubyConstant.QTRUE;
    }
    
    private static void throwUninitializedConstant(RubyModule m, String name) {
        RubyString str = ObjectFactory.createString();
        if (m != RubyRuntime.ObjectClass) {
            m.to_s(str);
            if (str.length() > 0) {
                str.appendString("::");
            }
        }
        str.appendString(name);
        throw new RubyException(RubyRuntime.NameErrorClass, "uninitialized constant " + str.toString());
    }

    public static RubyValue setConstant(RubyValue value, RubyValue receiver, String name) {
        throwTypeErrorIfNotClassModule(receiver);

        return ((RubyModule) receiver).setConstant(name, value);
    }

    private static void throwTypeErrorIfNotClassModule(RubyValue receiver) {
        if (!(receiver instanceof RubyClass) &&
                !(receiver instanceof RubyModule)) {
            RubyValue v = RubyAPI.callPublicNoArgMethod(receiver, null, RubyID.toSID);
            String s = v.toString();
            throw new RubyException(RubyRuntime.TypeErrorClass, s + " is not a class/module");
        }
    }
    
    private static final RubyID ASERT = RubyID.intern("[]=");

    public static void callArraySet(RubyValue value, RubyValue index, RubyValue receiver) {
    	callPublicTwoArgMethod(receiver, index, value, null, ASERT);
    }

    public static RubyValue initializeAsteriskParameter(RubyArray args, int argc) {
        if (null == args) {
            return new RubyArray();
        } else {
            return args.collect(argc);
        }
    }

    public static RubyValue initializeBlockParameter(RubyBlock block) {
        if (null == block) {
            return RubyConstant.QNIL;
        } else {
            return ObjectFactory.createProc(block);
        }
    }

}
