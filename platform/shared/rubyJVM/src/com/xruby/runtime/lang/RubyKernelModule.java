/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

/*import j2me.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;*/
import j2me.util.Iterator;
//import java.util.regex.Pattern;

//import antlr.RecognitionException;
//import antlr.TokenStreamException;

//import com.xruby.compiler.RubyCompiler;
//import com.xruby.compiler.codegen.CompilationResults;
//import com.xruby.compiler.codegen.NameFactory;
//import com.xruby.runtime.javasupport.JavaClass;
//import com.xruby.runtime.lang.*;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;
//import com.xruby.runtime.lang.annotation.RubyLevelModule;

import j2me.lang.MathEx;
import j2me.lang.PrintStreamMe;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.builtin.*;

//@RubyLevelModule(name="Kernel")
public class RubyKernelModule {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RubyKernelModule");
	
	//@RubyLevelMethod(name="==", alias={"equal?", "eql?"})
	public static RubyValue opEqual(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createBoolean(receiver == arg);
	}
	
	//@RubyLevelMethod(name="===")
	public static RubyValue objEqual(RubyValue receiver, RubyValue arg) {
		if (receiver == arg) {
            return RubyConstant.QTRUE;
        } else {
            boolean result = RubyAPI.callPublicOneArgMethod(receiver, arg, null, RubyID.equalID).isTrue();
			return ObjectFactory.createBoolean(result);
        }
	}
	
	//@RubyLevelMethod(name="class")
	public static RubyValue objRubyClass(RubyValue receiver) {
		RubyClass klass = receiver.getRubyClass();
		if (klass != null)
			return klass.getRealClass();
		return RubyConstant.QNIL;
        //return klass != null ? klass.getRealClass() : RubyConstant.QNIL;
	}
	
	// FIXME: Kernel_clone should be revised.
	//@RubyLevelMethod(name="clone", alias="dup")
	public static RubyValue objClone(RubyValue receiver) {
		return (RubyValue) receiver.clone();
	}
	
	//@RubyLevelMethod(name="to_s")
	public static RubyValue anyObjToS(RubyValue receiver) {
		String className = receiver.getRubyClass().getName();
        return ObjectFactory.createString("#<" + className + ":0x" + Integer.toHexString(receiver.hashCode()) + "x>");
	}
	
	//@RubyLevelMethod(name="inspect")
	public static RubyValue objInsepct(RubyValue receiver) {
		return ObjectFactory.createString(receiver.inspect());
	}

	//@RubyLevelMethod(name="instance_variable_set")
	public static RubyValue instance_variable_set(RubyValue receiver,RubyArray args) {
		RubyID mid = RubyID.intern(args.get(1).toStr());
		return receiver.setInstanceVariable((RubyValue)args.get(1), mid);
	}
	
	//@RubyLevelMethod(name="methods")
	public static RubyValue objMethods(RubyValue receiver) {
		RubyArray a = new RubyArray();
        RubyClass klass = receiver.getRubyClass();
        klass.collectClassMethodNames(a, RubyMethod.ALL);
        return a;
	}
	
	//@RubyLevelMethod(name="singleton_methods")
	public static RubyValue objSingletonMethods(RubyValue receiver) {
		return objSingletonMethod(receiver, true);
	}
	
	//@RubyLevelMethod(name="singleton_methods")	
	public static RubyValue objSingletonMethods(RubyValue receiver, RubyValue arg) {
        return objSingletonMethod(receiver, arg.isTrue());
	}
	
	private static RubyValue objSingletonMethod(RubyValue receiver, boolean all) {
		RubyArray a = new RubyArray();
        if(receiver.getRubyClass().isSingleton()) {
            RubyClass rubyClass = receiver.getRubyClass();
            rubyClass.collectOwnMethodNames(a, RubyMethod.PUBLIC);
            rubyClass = rubyClass.getSuperClass();

            if (all) {
                while(rubyClass != null && rubyClass.isSingleton()) {
                    rubyClass.collectOwnMethodNames(a, RubyMethod.PUBLIC);
                    rubyClass = rubyClass.getSuperClass();
                }
            }
        }
        return a;
	}
	
	// FIXME:This method should be module.
	//@RubyLevelMethod(name="raise", alias="fail")
	public static RubyValue raise(RubyValue value, RubyArray args) {
		RubyExceptionValue e;
        if (null == args) {
            //With no arguments, raises the exception in $! or raises a RuntimeError if $! is nil.
            RubyValue v = GlobalVariables.get("$!");
            if (RubyConstant.QNIL == v) {
                e = new RubyExceptionValue(RubyRuntime.RuntimeErrorClass, "");
            } else {
                e = (RubyExceptionValue)v;
            }
        } else if (1 == args.size() && (args.get(0) instanceof RubyString)) {
            //With a single String argument, raises a RuntimeError with the string as a message.
            e = new RubyExceptionValue(RubyRuntime.RuntimeErrorClass, ((RubyString) args.get(0)).toString());
        } else if (args.get(0) instanceof RubyExceptionValue) {
            //Otherwise, the first parameter should be the name of an Exception class
            //(or an object that returns an Exception when sent exception). The optional second
            //parameter sets the message associated with the exception, and the third parameter
            //is an array of callback information.
            e = (RubyExceptionValue) args.get(0);
            if (args.size() > 1) {
                e.setMessage(((RubyString) args.get(1)).toString());
            }
        } else {
            RubyClass v = (RubyClass) args.get(0);
            e = new RubyExceptionValue(v, 1 == args.size() ? "" : ((RubyString) args.get(1)).toString());
        }

        throw new RubyException(e);
	}
	
	//@RubyLevelMethod(name="exit", module=true)
	public static RubyValue exit(RubyValue receiver) {
		// TODO should raise SystemExit exception and call at_exit blocks
		System.exit(0);
        return RubyConstant.QNIL;
	}
	
	//@RubyLevelMethod(name="exit", module=true)
	public static RubyValue exit(RubyValue receiver, RubyValue arg) {
		// TODO should raise SystemExit exception and call at_exit blocks
		int status;
		if (arg == RubyConstant.QTRUE) {
			status = 0;
		} else if (arg == RubyConstant.QFALSE) {
			status = 1;
		} else {
			status = arg.toInt();
		}
		System.exit(status);
        return RubyConstant.QNIL;
	}
	
	//@RubyLevelMethod(name="loop", module=true)
	public static RubyValue loop(RubyValue receiver, RubyArray args, RubyBlock block) {
		if (null == block) {
            throw new RubyException(RubyRuntime.LocalJumpErrorClass, "in `loop': no block given");
        }

        for (; ;) {
            RubyValue v = block.invoke(receiver, args);
            if (block.breakedOrReturned()) {
                return v;
            }
        }
	}
	
	//@RubyLevelMethod(name="kind_of?", alias="is_a?")
	public static RubyValue kindOf(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createBoolean(RubyAPI.isKindOf(arg, receiver));
	}
	
	//@RubyLevelMethod(name="instance_of?")
	public static RubyValue instanceOf(RubyValue receiver, RubyValue arg) {
		return ObjectFactory.createBoolean(receiver.getRubyClass().getRealClass() == arg);
	}
	
	//@RubyLevelMethod(name="respond_to?")
	public static RubyValue respond_to(RubyValue receiver, RubyArray args) {
		if (null == args || args.size() < 1) {
            int actual_argc = (null == args ) ? 0 : args.size();
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `respond_to': wrong number of arguments (" + actual_argc + " for 1)");
        }
		
        boolean include_private = (RubyConstant.QTRUE == args.get(1));
        RubyID mid = RubyID.intern(args.get(0).toStr());
        return ObjectFactory.createBoolean(hasMethod(receiver, mid, include_private));
	}
	
	private static boolean hasMethod(RubyValue receiver, RubyID mid, boolean include_private) {
        if (include_private) {
            return (null != receiver.findMethod(mid));
        } else {
            return (null != receiver.findPublicMethod(mid));
        }
    }
	
	//@RubyLevelMethod(name="send", alias="__send__")
	public static RubyValue send(RubyValue receiver, RubyBlock block) {
		throw new RubyException(RubyRuntime.ArgumentErrorClass, "no method name given");
	}
	
	//@RubyLevelMethod(name="send", alias="__send__")
	public static RubyValue send(RubyValue receiver, RubyValue value, RubyBlock block) {
		RubyID mid = RubyID.intern(value.toStr());
		return RubyAPI.callNoArgMethod(receiver, block, mid);
	}
	
	//@RubyLevelMethod(name="send", alias="__send__")
	public static RubyValue send(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) {
        RubyID mid = RubyID.intern(arg0.toStr());
        return RubyAPI.callOneArgMethod(receiver, arg1, block, mid);
	}
	
	//@RubyLevelMethod(name="send", alias="__send__")
	public static RubyValue send(RubyValue receiver, RubyArray args, RubyBlock block) {
        RubyValue method_name = args.delete_at(0);
        RubyID mid = RubyID.intern(method_name.toStr());
        return RubyAPI.callMethod(receiver, args, block, mid);
	}
	
	//@RubyLevelMethod(name="instance_eval")
	public static RubyValue instanceEval(RubyValue receiver, RubyArray args, RubyBlock block) {
		if (null == args && null == block) {
			throw new RubyException(RubyRuntime.ArgumentErrorClass,
					"block not supplied");
		}

		if (null != args) {
			RubyBinding binding = new RubyBinding();
			LOG.INFO("instanceEval reciever:" + receiver.toString());
			binding.setScope((RubyModule) receiver);
			binding.setSelf(receiver);
			return eval(args.get(0).toStr(), binding);
		} else {
			block.setSelf(receiver);
			return block.invoke(receiver);
		}
	}
	
	private static RubyValue eval(String evalText) {
		//RHO_COMMENT: eval
		throw new RubyException("Not implemented: eval(evalText)");
		
/*		RubyCompiler compiler = new RubyCompiler();
        try {
			CompilationResults codes = compiler.compileString(evalText);
            RubyProgram p = codes.getRubyProgram();
            return p.invoke();
        } catch (RecognitionException e) {
            throw new RubyException(RubyRuntime.SyntaxErrorClass, e.toString());
        } catch (TokenStreamException e) {
            throw new RubyException(RubyRuntime.SyntaxErrorClass, e.toString());
        } catch (InstantiationException e) {
            throw new RubyException(e.toString());
        } catch (IllegalAccessException e) {
            throw new RubyException(e.toString());
        }*/
	}
	
	public static RubyValue eval(String evalText, RubyBinding binding) {
		//RHO_COMMENT: eval
		throw new RubyException("Not implemented: eval(evalText,binding)");
		
		/*RubyCompiler compiler = new RubyCompiler();
        try {
			CompilationResults codes = compiler.compileString(evalText);
            RubyProgram p = codes.getRubyProgram();
            return p.invoke(binding.getSelf(), binding.getVariables(), binding.getBlock(), binding.getScope());
        } catch (RecognitionException e) {
            throw new RubyException(RubyRuntime.SyntaxErrorClass, e.toString());
        } catch (TokenStreamException e) {
            throw new RubyException(RubyRuntime.SyntaxErrorClass, e.toString());
        } catch (InstantiationException e) {
            throw new RubyException(e.toString());
        } catch (IllegalAccessException e) {
            throw new RubyException(e.toString());
        }*/
	}

	private static RubyValue eval(String evalText, RubyBinding binding, String file_name) {
		//RHO_COMMENT: eval
		throw new RubyException("Not implemented: eval(evalText,binding)");
		
		/*RubyCompiler compiler = new RubyCompiler(binding, false);
        try {
			CompilationResults codes = compiler.compileString(file_name, evalText);
            RubyProgram p = codes.getRubyProgram();
            if (null != binding) {
                return p.invoke(binding.getSelf(), binding.getVariables(), binding.getBlock(), binding.getScope());
            } else {
                return p.invoke();
            }
        } catch (RecognitionException e) {
            throw new RubyException(RubyRuntime.SyntaxErrorClass, e.toString());
        } catch (TokenStreamException e) {
            throw new RubyException(RubyRuntime.SyntaxErrorClass, e.toString());
        } catch (InstantiationException e) {
            throw new RubyException(e.toString());
        } catch (IllegalAccessException e) {
            throw new RubyException(e.toString());
        }*/
	}
	
	//@RubyLevelMethod(name="eval", module=true)
    public static RubyValue eval(RubyValue receiver, RubyValue arg) {
		return eval(arg.toStr());
    }
	
	//@RubyLevelMethod(name="eval", module=true)
    public static RubyValue eval(RubyValue receiver, RubyValue arg0, RubyValue arg1) {
		if (arg1 instanceof RubyBinding) {
			return eval(arg0.toStr(), (RubyBinding)arg1);
		} else {
			return eval(arg0.toStr());
		}
	}

	//@RubyLevelMethod(name="eval", module=true)
    public static RubyValue eval(RubyValue receiver, RubyArray args) {
        RubyBinding binding = null;
        if (args.get(1) instanceof RubyBinding) {
            binding = (RubyBinding)args.get(1);
        }

        String file_name = null;
        if (args.size() > 2) {
            file_name = args.get(2).toStr();
        }

        return eval(args.get(0).toStr(), binding, file_name);
    }
   
	//@RubyLevelMethod(name="method")
	public static RubyValue objMethod(RubyValue receiver, RubyValue arg) {
		String method_name = arg.toStr();
        RubyID mid = RubyID.intern(method_name);
        RubyMethod m = receiver.findMethod(mid);
        if (null == m) {
            throw new RubyException(RubyRuntime.NameErrorClass, "public method '" + method_name + "' can not be found in '" + receiver.getRubyClass().getName() + "'");
        }
        return ObjectFactory.createMethod(receiver, method_name, m);
	}
	
	//@RubyLevelMethod(name="public_methods")
	public static RubyValue publicMethods(RubyValue receiver) {
        RubyArray a = new RubyArray();
        RubyClass klass = receiver.getRubyClass();
        klass.collectClassMethodNames(a, RubyMethod.PUBLIC);
        return a;
    }
	
	//@RubyLevelMethod(name="caller", module=true)
	public static RubyValue call(RubyValue receiver) {
        return new RubyArray();//FIXME use a try/catch to get stacktrace
    }
	
	//@RubyLevelMethod(name="caller", module=true)
	public static RubyValue call(RubyValue receiver, RubyValue arg) {
        return new RubyArray();//FIXME use a try/catch to get stacktrace
    }
	
	//@RubyLevelMethod(name="throw", module=true)
	public static RubyValue throwMethod(RubyValue receiver) {
        throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `throw': wrong number of arguments (0 for 1)");
    }
	
	//@RubyLevelMethod(name="throw", module=true)
	public static RubyValue throwMethod(RubyValue receiver, RubyArray args) {
        RubyExceptionValue e;
        RubyValue tag = args.get(0);
		if (tag instanceof RubySymbol ||
            tag instanceof RubyString) {
            e = new RubyExceptionValueForThrow(tag, args.get(1));
        } else if (tag instanceof RubyExceptionValue) {
            e = (RubyExceptionValue)tag;
        } else if (tag instanceof RubyClass) {
            RubyClass c = (RubyClass)tag;
            e = new RubyExceptionValue(c, c.getName() + " is not a symbol");
        } else {
            e = new RubyExceptionValue(RubyRuntime.ArgumentErrorClass, tag.toString() + " is not a symbol");
        }
        throw new RubyException(e);
    }
	
	//@RubyLevelMethod(name="catch", module=true)
	public static RubyValue catchMethod(RubyValue receiver, RubyValue arg, RubyBlock block) {
        if (!(arg instanceof RubySymbol)) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, arg.toString() + " is not a symbol");
        }

        try {
            block.invoke(receiver);
        } catch (RubyException e) {
            RubyValue ev = RubyAPI.convertRubyException2RubyValue(e);
            if (ev instanceof RubyExceptionValueForThrow) {
                RubyExceptionValueForThrow v = (RubyExceptionValueForThrow) ev;
                if (v.isSameSymbol(arg)) {
                    return v.getReturnValue();
                }
            }
            throw e;
        }

        return RubyConstant.QNIL;
    }
	
	//@RubyLevelMethod(name="untrace_var", module=true)
	public static RubyValue untrace_var(RubyValue receiver, RubyArray args, RubyBlock block) {
        if (null == args || args.size() < 1) {
            int actual_argc = (null == args ) ? 0 : args.size();
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `untrace_var': wrong number of arguments (" + actual_argc + " for 1)");
        }

        if (!(args.get(0) instanceof RubySymbol)) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, args.get(0).toString() + " is not a symbol");
        }

        String name = ((RubySymbol) args.get(0)).toString();

        RubyValue v = args.get(1);
        if (v == RubyConstant.QNIL) {
            GlobalVariables.removeAllTraceProc(name);
        } else if (v instanceof RubyProc) {
            GlobalVariables.removeTraceProc(name, (RubyProc) v);
        }

        return RubyConstant.QNIL;
    }
	
	//@RubyLevelMethod(name="trace_var", module=true)
	public static RubyValue trace_var(RubyValue receiver, RubyArray args, RubyBlock block) {
        if (null == args || args.size() < 1) {
            int actual_argc = (null == args ) ? 0 : args.size();
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `trace_var': wrong number of arguments (" + actual_argc + " for 1)");
        }

        if (!(args.get(0) instanceof RubySymbol)) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, args.get(0).toString() + " is not a symbol");
        }

        String name = ((RubySymbol) args.get(0)).toString();

        RubyValue v = args.get(1);
        if (v instanceof RubyProc) {
            GlobalVariables.addTraceProc(name, (RubyProc) v);
        } else if (null != block) {
            GlobalVariables.addTraceProc(name, ObjectFactory.createProc(block));
        } else {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "tried to create Proc object without a block");
        }

        return RubyConstant.QNIL;
    }
	
	//@RubyLevelMethod(name="block_given?", alias="iterator?", module=true)
	public static RubyValue blockGivenP(RubyValue receiver, RubyBlock block) {
		return ObjectFactory.createBoolean(null != block);
    }
	
	//@RubyLevelMethod(name="Float", module=true)
	public static RubyFloat toFloat(RubyValue receiver, RubyValue arg) {
		return arg.toRubyFloat();		
	}
	
	//@RubyLevelMethod(name="Integer", module=true)
	public static RubyInteger toInteger(RubyValue receiver, RubyValue arg) {
		return arg.toRubyInteger();		
	}

	//@RubyLevelMethod(name="String", module=true)
	public static RubyString toString(RubyValue receiver, RubyValue arg) {
		return arg.toRubyString();		
	}

	//@RubyLevelMethod(name="Array", module=true)
	public static RubyArray toArray(RubyValue receiver, RubyValue arg) {
		RubyArray res = null;
		try{
			res = arg.toRubyArray();
		}catch(RubyException exc)
		{}
		if ( res == null )
			res = ObjectFactory.createArray(1, arg);
				
		return res;
	}
	
	//@RubyLevelMethod(name="puts", module=true)
	public static RubyValue puts(RubyValue receiver) {
		return RubyIO.STDOUT.puts();
    }
	
	//@RubyLevelMethod(name="puts", module=true)
	public static RubyValue puts(RubyValue receiver, RubyArray args) {
        return RubyIO.STDOUT.puts(args);
    }
    
    //@RubyLevelMethod(name="print", module=true)
    public static RubyValue print(RubyValue receiver) {
        return RubyIO.STDOUT.print();
    }
    
    //@RubyLevelMethod(name="print", module=true)
    public static RubyValue print(RubyValue receiver, RubyValue arg) {
        return RubyIO.STDOUT.print(arg);
    }
    
    //@RubyLevelMethod(name="print", module=true)
    public static RubyValue print(RubyValue receiver, RubyArray args) {
    	return RubyIO.STDOUT.print(args);
    }
    
    //@RubyLevelMethod(name="printf", module=true)
    public static RubyValue printf(RubyValue receiver, RubyArray args) {
        String fmt = args.get(0).toStr();
    	args.delete_at(0);
        PrintStreamMe.printf(System.out, fmt, args );//fmt, RubyKernelModule.buildFormatArg(args, 1));
        return RubyConstant.QNIL;
    }
    
    //@RubyLevelMethod(name="sprintf", module=true)
    public static RubyValue sprintf(RubyValue receiver, RubyArray args) {
        /*String fmt = args.get(0).toStr();
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        PrintStream ps = new PrintStream(baos);
        PrintStreamMe.printf(ps, fmt, RubyKernelModule.buildFormatArg(args, 1));
        ps.flush();
        ps.close();
        return ObjectFactory.createString(baos.toString());*/
    	String fmt = args.get(0).toStr();
    	args.delete_at(0);
    	return ObjectFactory.createString(PrintStreamMe.sprintf(fmt, args));
    }
    
    //@RubyLevelMethod(name="p", module=true)
    public static RubyValue p(RubyValue receiver) {
        return RubyConstant.QNIL;
    }
    
    //@RubyLevelMethod(name="p", module=true)
    public static RubyValue p(RubyValue receiver, RubyValue arg) {
    	RubyValue str = RubyAPI.callNoArgMethod(arg, null, RubyID.inspectID);
    	RubyString value = str.toRubyString();
    	value.appendString("\n");
    	//System.out.print(value.toString());
    	LOG.INFO(value.toString());
    	return RubyConstant.QNIL;
    }
    
    //@RubyLevelMethod(name="p", module=true)
    public static RubyValue p(RubyValue receiver, RubyArray args) {
//    	for (RubyValue arg : args) {
        for (Iterator iter = args.iterator(); iter.hasNext();) {
        	RubyValue arg = (RubyValue)iter.next();
    		RubyValue str = RubyAPI.callNoArgMethod(arg, null, RubyID.inspectID);
    		RubyString value = (RubyString) str;
    		value.appendString("\n");
    		//System.out.print(value.toString());
    		LOG.INFO(value.toString());
    	}
    	return RubyConstant.QNIL;
    }

	//private static BufferedReader in_ = new BufferedReader(new InputStreamReader(System.in));
	
    //@RubyLevelMethod(name="gets", module=true)
    public static RubyValue gets(RubyValue receiver) {
    	//RHO_COMMENT: gets
		return RubyConstant.QNIL;
        /*String s = null;
        try {
            s = in_.readLine();
        } catch (IOException e) {
        }
        GlobalVariables.set((null == s ? RubyConstant.QNIL : ObjectFactory.createString(s)), "$_");
        return GlobalVariables.get("$_");*/
    }
    
    //@RubyLevelMethod(name="object_id", alias={"__id__", "hash"})
    public static RubyValue objectId(RubyValue receiver) {
        //Object.hashCode() javadoc:
        //As much as is reasonably practical, the hashCode method defined
        //by class Object does return distinct integers for distinct objects.
        return ObjectFactory.createFixnum(receiver.hashCode());
    }
    
    //@RubyLevelMethod(name="extend")
    public static RubyValue extend(RubyValue receiver) {
    	throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong number of arguments (0 for 1)");
    }
    
    //@RubyLevelMethod(name="extend")
    public static RubyValue extend(RubyValue receiver, RubyValue arg) {
    	RubyAPI.callPublicOneArgMethod(arg, receiver, null, RubyID.extendObjectID);
    	return receiver;
    }
    
    //@RubyLevelMethod(name="extend")
    public static RubyValue extend(RubyValue receiver, RubyArray args) {
//    	for (RubyValue v : args) {
        for (Iterator iter = args.iterator(); iter.hasNext();) {
        	RubyValue v = (RubyValue)iter.next();
    		RubyAPI.callPublicOneArgMethod(v, receiver, null, RubyID.extendObjectID);
    	}

    	return receiver;
    }
    
    //@RubyLevelMethod(name="freeze")
    public static RubyValue freeze(RubyValue receiver) {
        receiver.freeze();
        return receiver;
    }

    //@RubyLevelMethod(name="frozen?")
    public static RubyValue frozen(RubyValue receiver) {
    	return ObjectFactory.createBoolean(receiver.frozen());
    }

    //@RubyLevelMethod(name="taint")
    public static RubyValue taint(RubyValue receiver) {
        receiver.taint();
        return receiver;
    }

    //@RubyLevelMethod(name="taint")
    public static RubyValue untaint(RubyValue receiver) {
        receiver.untaint();
        return receiver;
    }
    
    //@RubyLevelMethod(name="tainted?")
    public static RubyValue tainted(RubyValue receiver) {
    	return ObjectFactory.createBoolean(receiver.tainted());
    }
    
    //@RubyLevelMethod(name="method_missing", module=true)
    public static RubyValue methodMissing(RubyValue receiver, RubyArray args) {
        RubySymbol method_name = (RubySymbol)args.get(0);
        RubyClass klass = receiver.getRubyClass();
        klass = (klass != null) ? klass.getRealClass() : null;
        String msg =  "undefined method '" + method_name.toString() + "' for " + klass.getName();
        LOG.ERROR(msg);
        throw new RubyException(RubyRuntime.NoMethodErrorClass,msg);
    }
    
    //@RubyLevelMethod(name="sleep", module=true)
    public static RubyValue sleep(RubyValue receiver, RubyValue arg) {
        long milliseconds = RubyTypesUtil.convertToJavaLong(arg)*1000;
        long startTime = System.currentTimeMillis();

        RubyThread.sleep(milliseconds);

        long endTime = System.currentTimeMillis();
        return ObjectFactory.createFixnum((int)MathEx.round((endTime-startTime)/1000.0));
    }
    
    //private static Pattern packagePattern = Pattern.compile("\\.");

    //@RubyLevelMethod(name="require_java", alias="import", module=true)
    public static RubyValue requireJava(RubyValue receiver, RubyValue arg, RubyBlock block) {
    	//RHO_COMMENT: requireJava
    	return RubyConstant.QFALSE;
        /*String className = arg.toStr();
        String[] names = packagePattern.split(className);
        String name = names[names.length - 1];
        if(name.equals("*")){
            JavaClass.addPackage(className.substring(0, className.lastIndexOf('.')));
        }else{
            try {
                Class clazz = Class.forName(className);
                JavaClass.createJavaClass(clazz);
            } catch (ClassNotFoundException e) {
                throw new RubyException("Couldn't find class " + className);
            }
        }

        RubyRuntime.setJavaSupported(true);
        return RubyConstant.QTRUE;*/
    }
    
    //@RubyLevelMethod(name="binding", module=true)
    public static RubyValue binding(RubyValue receiver, RubyArray args) {
        //compiler will do the magic and insert Binding object
        return args.get(0);
    }
    
    //@RubyLevelMethod(name="lambda", alias="proc", module=true)
    public static RubyValue lambda(RubyValue receiver, RubyBlock block) {
        block.setCreatedByLambda();
        return ObjectFactory.createProc(block);
    }
    
    //@RubyLevelMethod(name="at_exit", module=true)
    public static RubyValue atExit(RubyValue receiver, RubyBlock block) {
        if (null == block) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "called without a block");
        }

        AtExitBlocks.registerBlock(block);
        return ObjectFactory.createProc(block);
    }
    
    //@RubyLevelMethod(name="gsub", module=true)
    public static RubyValue gsub(RubyValue receiver, RubyArray args, RubyBlock block) {
        if (!(GlobalVariables.get("$_") instanceof RubyString)) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "$_ value need to be String (" + GlobalVariables.get("$LAST_READ_LINE").getRubyClass().getName() + " given)");
        }

        RubyValue r = ((RubyString)GlobalVariables.get("$_")).gsub_danger(args, block);
        return GlobalVariables.set(r, "$_");
    }
    
    //@RubyLevelMethod(name="gsub!", module=true)
    public static RubyValue gsubBang(RubyValue receiver, RubyArray args, RubyBlock block) {
        if (!(GlobalVariables.get("$_") instanceof RubyString)) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "$_ value need to be String (" + GlobalVariables.get("$_").getRubyClass().getName() + " given)");
        }

        RubyValue r = ((RubyString)GlobalVariables.get("$_")).gsub_danger(args, block);
        if (r != RubyConstant.QNIL) {
            GlobalVariables.set(r, "$_");
        }
        return r;
    }
    
    //@RubyLevelMethod(name="sub", module=true)
    public static RubyValue sub(RubyValue receiver, RubyArray args, RubyBlock block) {
        if (!(GlobalVariables.get("$_") instanceof RubyString)) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "$_ value need to be String (" + GlobalVariables.get("$LAST_READ_LINE").getRubyClass().getName() + " given)");
        }

        RubyValue r = ((RubyString)GlobalVariables.get("$_")).gsub_danger(args, block);
        return GlobalVariables.set(r, "$_");
    }

    private static int RDWR = 2;
    //private static int CREAT = 256;
    //private static int EXCL = 1024;

    //@RubyLevelMethod(name="open")
    public static RubyValue open(RubyValue receiver, RubyArray args, RubyBlock block) {
        String filename = args.get(0).toStr();
        String mode = "r";

    	//RHO_COMMENT
        RubyIO io = null;
        if (args.size() <= 1) {
        } else if (args.get(1) instanceof RubyFixnum) {
            int i = args.get(1).toInt();
            if ((i & RDWR) != 0) {
                mode = mode + "w";
            }
        } else {
            RubyString val = (RubyString) args.get(1);
            mode = val.toString();
        }

        try{
        	io = ObjectFactory.createResourceFile(filename, mode);
        }catch (java.lang.Error exc)
        {
        	LOG.ERROR("Cannot open file from jar: " + filename, exc);
        }
        
        if ( io == null )
        	io = ObjectFactory.createFile(filename, mode);
        
    	//RHO_COMMENT
        
        if (null == block) {
            return io;
        } else {
            RubyValue v = block.invoke(receiver, io);
            io.close();
            return v;
        }
    }

    public static Object[] buildFormatArg(RubyArray args, int start) {
        Object[] raw_args = new Object[args.size() - start];
        for (int i = 0; i < args.size() - start; ++i) {
            Object v = args.get(i + start);
            if (v instanceof RubyFixnum) {
                raw_args[i] = new Integer(((RubyFixnum) v).toInt());
            } else if (v instanceof RubyFloat) {
                raw_args[i] = new Double(((RubyFloat) v).doubleValue());
            } else {
                raw_args[i] = v;
            }
        }
        return raw_args;
    }
    
}
