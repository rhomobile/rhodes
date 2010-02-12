/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import java.io.OutputStream;
import java.io.PrintStream;

import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.util.RubyTypeFactory;
import com.xruby.runtime.stdlib.*;

public class RubyRuntime {
    public static RubyClass ObjectClass;
    public static RubyClass ModuleClass;
    public static RubyClass ClassClass;
    public static RubyModule KernelModule;
    public static RubyModule ComparableModule;
    public static RubyModule EnumerableModule;
    public static RubyModule ErrnoModule;
    public static RubyModule FileTestModule;
    public static RubyModule GCModule;
    public static RubyModule MarshalModule;
    public static RubyModule MathModule;
    public static RubyModule ObjectSpaceModule;
    public static RubyModule ProcessModule;
    public static RubyClass BindingClass;
    public static RubyClass NilClassClass;
    public static RubyClass TrueClassClass;
    public static RubyClass FalseClassClass;
    public static RubyClass NumericClass;
    public static RubyClass IntegerClass;
    public static RubyClass FixnumClass;
    public static RubyClass BignumClass;
    public static RubyClass StringClass;
    public static RubyClass FloatClass;
    public static RubyClass ArrayClass;
    public static RubyClass HashClass;
    public static RubyClass SymbolClass;
    public static RubyClass IOClass;
    public static RubyClass ProcClass;
    public static RubyClass RangeClass;
    public static RubyClass RegexpClass;
    public static RubyClass FileClass;
    public static RubyClass MethodClass;
    public static RubyClass TimeClass;
    public static RubyClass MatchDataClass;
    public static RubyClass DirClass;
    public static RubyClass StructClass;
    public static RubyClass ThreadGroupClass;
    public static RubyClass ThreadClass;
    public static RubyClass UnboundMethodClass;

    public static RubyClass ExceptionClass;
    public static RubyClass StandardErrorClass;
    public static RubyClass TypeErrorClass;
    public static RubyClass ArgumentErrorClass;
    public static RubyClass IndexErrorClass;
    public static RubyClass RangeErrorClass;
    public static RubyClass NameErrorClass;
    public static RubyClass ThreadErrorClass;
    public static RubyClass NoMethodErrorClass;
    public static RubyClass IOErrorClass;
    public static RubyClass EOFErrorClass;
    public static RubyClass RuntimeErrorClass;
    public static RubyClass LocalJumpErrorClass;
    public static RubyClass SystemCallErrorClass;
    public static RubyClass ZeroDivErrorClass;

    public static RubyClass ScriptErrorClass;
    public static RubyClass SyntaxErrorClass;
    public static RubyClass LoadErrorClass;
    public static RubyClass NotImplementedErrorClass;

    public static RubyClass StringIOClass;

    public static final RubyValue TOP_LEVEL_SELF_VALUE;

    private static RubyMethod respondToMethod;

    public static boolean running = false;

    static {
        //Note: order is important: should creare parent classes first!

        ObjectClass = ClassFactory.defineBootClass("Object", null);
        ModuleClass = ClassFactory.defineBootClass("Module", RubyRuntime.ObjectClass);
        ClassClass = ClassFactory.defineBootClass("Class", RubyRuntime.ModuleClass);

        ClassClass.setRubyClass(ClassClass);

        RubySingletonClass metaClass = new RubySingletonClass(ObjectClass, ClassClass, null);
        metaClass = new RubySingletonClass(ModuleClass, metaClass, null);
        metaClass = new RubySingletonClass(ClassClass, metaClass, null);

        KernelModule = RubyAPI.defineModule("Kernel");

        NilClassClass = RubyAPI.defineClass("NilClass", RubyRuntime.ObjectClass);
        TrueClassClass = RubyAPI.defineClass("TrueClass", RubyRuntime.ObjectClass);
        FalseClassClass = RubyAPI.defineClass("FalseClass", RubyRuntime.ObjectClass);

        ComparableModule = RubyAPI.defineModule("Comparable");
        EnumerableModule = RubyAPI.defineModule("Enumerable");
        ErrnoModule = RubyAPI.defineModule("Errno");
        FileTestModule = RubyTypeFactory.getModule(RubyFileTestModule.class);
        GCModule = RubyTypeFactory.getModule(RubyGC.class);

        MarshalModule = RubyTypeFactory.getModule(RubyMarshalModule.class);//RubyAPI.defineModule("Marshal");
        MathModule = RubyTypeFactory.getModule(RubyMathModule.class);
        ObjectSpaceModule = RubyTypeFactory.getModule(ObjectSpace.class);
        ProcessModule = RubyAPI.defineModule("Process");

        BindingClass = RubyAPI.defineClass("Binding", RubyRuntime.ObjectClass);

        RubyTypeFactory.getClass(RubyObject.class);
        RubyTypeFactory.getClass(RubyModule.class);
        RubyTypeFactory.getClass(RubyClass.class);

        RubyModule m = RubyTypeFactory.getModule(RubyKernelModule.class);
        respondToMethod = m.findMethod(RubyID.RESPOND_TO_P);
        RubyTypeFactory.getModule(RubyRandom.class);

        NumericClass = RubyTypeFactory.getClass(RubyNumeric.class);
        IntegerClass = RubyTypeFactory.getClass(RubyInteger.class);
        FixnumClass = RubyTypeFactory.getClass(RubyFixnum.class);
        BignumClass = RubyTypeFactory.getClass(RubyBignum.class);
        StringClass = RubyTypeFactory.getClass(RubyString.class);
        FloatClass = RubyTypeFactory.getClass(RubyFloat.class);
        ArrayClass = RubyTypeFactory.getClass(RubyArray.class);
        HashClass = RubyTypeFactory.getClass(RubyHash.class);
        SymbolClass = RubyTypeFactory.getClass(RubySymbol.class);
        IOClass = RubyTypeFactory.getClass(RubyIO.class);
        ProcClass = RubyTypeFactory.getClass(RubyProc.class);
        RangeClass = RubyTypeFactory.getClass(RubyRange.class);
        RegexpClass = RubyTypeFactory.getClass(RubyRegexp.class);
        FileClass = RubyTypeFactory.getClass(RubyFile.class);
        MethodClass = RubyTypeFactory.getClass(RubyMethodValue.class);
        TimeClass = RubyTypeFactory.getClass(RubyTime.class);
        MatchDataClass = RubyTypeFactory.getClass(RubyMatchData.class);
        DirClass = RubyTypeFactory.getClass(RubyDir.class);
        StructClass = RubyTypeFactory.getClass(RubyStruct.class);
        ThreadGroupClass = RubyTypeFactory.getClass(RubyThreadGroup.class);
        ThreadClass = RubyTypeFactory.getClass(RubyThread.class);
        UnboundMethodClass = RubyAPI.defineClass("UnboundMethod", MethodClass);

        ExceptionClass = RubyTypeFactory.getClass(RubyExceptionValue.class);
        StandardErrorClass = RubyAPI.defineClass("StandardError", ExceptionClass);
        TypeErrorClass = RubyAPI.defineClass("TypeError", StandardErrorClass);
        ArgumentErrorClass = RubyAPI.defineClass("ArgumentError", StandardErrorClass);
        IndexErrorClass = RubyAPI.defineClass("IndexError", StandardErrorClass);
        RangeErrorClass = RubyAPI.defineClass("RangeError", StandardErrorClass);
        NameErrorClass = RubyAPI.defineClass("NameError", StandardErrorClass);
        ThreadErrorClass = RubyAPI.defineClass("ThreadError", StandardErrorClass);
        NoMethodErrorClass = RubyAPI.defineClass("NoMethodError", NameErrorClass);
        IOErrorClass = RubyAPI.defineClass("IOError", StandardErrorClass);
        EOFErrorClass = RubyAPI.defineClass("EOFError", IOErrorClass);
        RuntimeErrorClass = RubyAPI.defineClass("RuntimeError", StandardErrorClass);
        LocalJumpErrorClass = RubyAPI.defineClass("LocalJumpError", StandardErrorClass);
        SystemCallErrorClass = RubyAPI.defineClass("SystemCallError", StandardErrorClass);
        ZeroDivErrorClass = RubyAPI.defineClass("ZeroDivisionError", StandardErrorClass);

        ScriptErrorClass = RubyAPI.defineClass("ScriptError", ExceptionClass);
        SyntaxErrorClass = RubyAPI.defineClass("SyntaxError", ScriptErrorClass);
        LoadErrorClass = RubyAPI.defineClass("LoadError", ScriptErrorClass);
        NotImplementedErrorClass = RubyAPI.defineClass("NotImplementedError", ScriptErrorClass);

        TOP_LEVEL_SELF_VALUE = RubyTypeFactory.getObject(RubyTopSelf.class);

        StringIOClass = RubyTypeFactory.getClass(RubyStringIO.class);

        RubyThread.init();
    }

    private static void initARGV(String[] args) {
        RubyArray argv = new RubyArray();
        if (args != null) {
            for (String arg : args) {
                argv.add(ObjectFactory.createString(arg));
            }
        }

        RubyAPI.setTopLevelConstant(argv, "ARGV");
    }

    private static void loadBuildinDotRb() {
        try {
            Class c = Class.forName(com.xruby.compiler.codegen.NameFactory.name_classprefix_ + ".builtin.main");
            Object o = c.newInstance();
            RubyProgram p = (RubyProgram) o;
            p.invoke();
        } catch (ClassNotFoundException e) {
        } catch (InstantiationException e) {
        } catch (IllegalAccessException e) {
            throw new Error(e);
        }
    }

    public static void init(String[] args) {
        if (RubyRuntime.running) {
            return;
        }

        initARGV(args);

        RubyAPI.setTopLevelConstant(RubyAPI.isWindows() ? ObjectFactory.createString("mswin32") : ObjectFactory.createString("java"), "RUBY_PLATFORM");

        if (RubyAPI.isWindows()) {
            RubyAPI.setConstant(ObjectFactory.createString("\\"), RubyRuntime.FileClass, "ALT_SEPARATOR");
            RubyAPI.setConstant(ObjectFactory.createString(";"), RubyRuntime.FileClass, "PATH_SEPARATOR");
        } else {
        	RubyAPI.setConstant(ObjectFactory.createString(":"), RubyRuntime.FileClass, "PATH_SEPARATOR");
        }

        RubyTypeFactory.getObject(RubyENV.class);
        GlobalVariables.initialize();

        updateStdout();
        updateStderr();

        RubyAPI.setTopLevelConstant(RubyConstant.QTRUE, "TRUE");
        loadBuildinDotRb();
        RubyRuntime.running = true;
    }

    public static void fini() {
        AtExitBlocks.invokeAll();
    }

    private static boolean supported;
    public static boolean javaIsSupported(){
        return supported;
    }

    public static void setJavaSupported(boolean val){
        supported = val;
    }

    public static RubyMethod getRespondMethod() {
        return respondToMethod;
    }

    public static void setStdout(OutputStream os) {
        PrintStream ps = getPrintStream(os);
        System.setOut(ps);
        updateStdout();
    }

    private static void updateStdout() {
        RubyIO io = new RubyFile(new OutputStreamExecutor(System.out), RubyRuntime.IOClass);
        RubyAPI.setTopLevelConstant(io, "STDOUT");
        GlobalVariables.set(io, "$stdout");
    }

    private static void updateStderr() {
        RubyIO io = new RubyFile(new OutputStreamExecutor(System.err), RubyRuntime.IOClass);
        RubyAPI.setTopLevelConstant(io, "STDERR");
        GlobalVariables.set(io, "$stderr");
    }

    private static PrintStream getPrintStream(OutputStream os) {
        return (os instanceof PrintStream) ? (PrintStream)os : new PrintStream(os);
    }
}
