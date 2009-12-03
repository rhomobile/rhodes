/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyFixnum;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyProc;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.*;

import org.objectweb.asm.Type;

/*
 * This class is created to control the dependency between "compiler" and "runtime" package
 * This is still an ongoing process in very early stage. The goal is to make
 * other classes in 'codegen' package decoupled from the runtime, so that the
 * changes in 'runtime' package can have minimal impact on the 'codegen'.
 *
 * for example, in codegen, you can either use Types.RubyValueClass, or use RubyValue.class directly.
 * If you use the last one, than your code depends on runtime directly. The first one add a thin layer.
 * It is really nothing for know, but later we are going to more more to there, like method signatures
 * */
public class Types {
	public final static Type[] NULL_TYPE_ARRAY = new Type[0];
	
    static final Class RUBY_ARRAY_CLASS = RubyArray.class;
    static final Class RUBY_VALUE_CLASS = RubyValue.class;
    
    public static final Type RUBY_API_TYPE = Type.getType(RubyAPI.class);

    public static final Type RUBY_NOARGMETHOD_TYPE = Type.getType(RubyNoArgMethod.class);
    public static final Type RUBY_ONEARGMETHOD_TYPE = Type.getType(RubyOneArgMethod.class);
    public static final Type RUBY_TWOARGMETHOD_TYPE = Type.getType(RubyTwoArgMethod.class);
    public static final Type RUBY_VARARGMETHOD_TYPE = Type.getType(RubyVarArgMethod.class);
    public static final Type RUBY_NOORONEARGMETHOD_TYPE = Type.getType(RubyNoOrOneArgMethod.class);
    public static final Type RUBY_ONEORTWOARGMETHOD_TYPE = Type.getType(RubyOneOrTwoArgMethod.class);

    static final Type RUBY_NOARGBLOCK_TYPE = Type.getType(RubyNoArgBlock.class);
    static final Type RUBY_ONEARGBLOCK_TYPE = Type.getType(RubyOneArgBlock.class);
    static final Type RUBY_VARARGBLOCK_TYPE = Type.getType(RubyVarArgBlock.class);

    public static final Type RUBY_RUNTIME_TYPE = Type.getType(RubyRuntime.class);
    static final Type RUBY_PROGRAM_TYPE = Type.getType(RubyProgram.class);
    public static final Type RUBY_BLOCK_TYPE = Type.getType(RubyBlock.class);
    public static final Type RUBY_CLASS_TYPE = Type.getType(RubyClass.class);
    public static final Type RUBY_MODULE_TYPE = Type.getType(RubyModule.class);
    public static final Type RUBY_METHOD_TYPE = Type.getType(RubyMethod.class);
    public static final Type RUBY_OBJECT_TYPE = Type.getType(RubyObject.class);
	static final Type METHOD_BLOCK_BASE_TYPE = Type.getType(MethodBlockBase.class);
    public static final Type RUBY_ARRAY_TYPE = Type.getType(RubyArray.class);
    public static final Type RUBY_ID_TYPE = Type.getType(RubyID.class);
    public static final Type RUBY_VALUE_TYPE = Type.getType(RubyValue.class);
    public static final Type RUBY_STRING_TYPE = Type.getType(RubyString.class);
    public static final Type RUBY_CONSTANT_TYPE = Type.getType(RubyConstant.class);
    static final Type RUBY_PROC_TYPE = Type.getType(RubyProc.class);
    static final Type RUBY_BINDING_TYPE = Type.getType(RubyBinding.class);
    static final Type RUBY_HASH_TYPE = Type.getType(RubyHash.class);
    public static final Type RUBY_FIXNUM_TYPE = Type.getType(RubyFixnum.class);
    public static final Type RUBY_EXCEPTION_TYPE = Type.getType(RubyException.class);
    public static final Type OBJECTFACTORY_TYPE = Type.getType(ObjectFactory.class);
    public static final Type GLOBAL_VARIABLES_TYPE = Type.getType(GlobalVariables.class);

    public static boolean isBuiltinClass(String name) {
        final String[] builtin_classes = {
                "Array", "Bignum", "Binding", "Class",
                //"Continuation",
                "Dir", "Exception",
                //"File::Stat",
                "FalseClass", "File", "Fixnum", "Float", "Hash", "Integer", "IO", "MatchData", "Method",
                "Module", "NilClass", "Numeric", "Object", "Proc", "Range", "Regexp", "String", "Struct",
                //"Struct::Tms"
                "Symbol", "ThreadGroup", "Thread", "ArgumentError",
                "Time", "TrueClass", "UnboundMethod",

                "Exception", "StandardError", "TypeError", "IndexError", "RangeError", "NameError", "NoMethodError", "IOError", "RuntimeError", "LocalJumpError","SystemCallError",
                "ScriptError", "SyntaxError", "LoadError", "NotImplementedError","ThreadError",
        };

        for (String s : builtin_classes) {
            if (s.equals(name)) {
                return true;
            }
        }

        return false;
    }

    public static boolean isBuiltinModule(String name) {
        final String[] builtin_classes = {
                "Comparable",
                "Enumerable",
                "Errno",
                "FileTest",
                "GC",
                "Kernel",
                "Marshal",
                "Math",
                "ObjectSpace",
                "Process ",
        };

        for (String s : builtin_classes) {
            if (s.equals(name)) {
                return true;
            }
        }

        return false;
    }
    
    private Types() {}
}
