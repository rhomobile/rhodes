/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import java.io.OutputStream;
import java.io.PrintStream;

import com.xruby.runtime.builtin.*;
import com.xruby.GeneratedMethods.*; 
import com.rho.RhoClassFactory;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.StringScanner;

import com.rho.Extensions;

public class RubyRuntime 
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RubyRuntime");
	
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
    public static RubyClass SystemExitClass;
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
    
    //RHO
	private static RubyModule SQLite3Module;
    public static RubyClass DatabaseClass;
    public static RubyModule SyncEngineClass;
    public static RubyClass StringScannerClass;
    public static RubyClass PhonebookClass;
    public static RubyClass PBRecordClass;
    public static RubyClass GeoLocationClass;
    public static RubyModule AsyncHttpModule;
    public static RubyClass SystemClass;
    public static RubyModule JSONClass;
    public static RubyClass CameraClass;
    public static RubyClass WebViewClass;
    public static RubyClass RhoConfClass;
    public static RubyClass AlertClass;    
    public static RubyClass DateTimePickerClass;
    public static RubyClass NativeBarClass;
    public static RubyClass RingtoneManagerClass;
    public static RubyClass MediaErrorClass;
    public static RubyClass MutexClass;
    public static RubyClass TCPSocketClass;
    public static RubyClass SSLSocketClass;
    public static RubyClass TopSelfClass;
    public static RubyModule MapViewClass;
    public static RubyClass XMLParserClass;
    //RHO

    public static /*final*/ RubyValue TOP_LEVEL_SELF_VALUE;

    private static RubyMethod respondToMethod;

    public static boolean running = false;


    private static void registerExtensions() {
		int i;
		for (i = 0; i < Extensions.extensions.length; i++) {
			String ext_class = Extensions.extensions[i];
			if (ext_class.length() > 0) {
 		          Class wrapperClass = null;
		          try {
		            wrapperClass = Class.forName(ext_class);
		          } catch (ClassNotFoundException exc) {  
		        	  LOG.ERROR("Extension class [" + ext_class + "] not exist !",exc);    	
		          }
		          if (wrapperClass != null) {
		        	Runnable ext_run = null;
		        	try {
		        		ext_run = (Runnable)wrapperClass.newInstance();
		        	} catch (Exception e) {
		            	  LOG.ERROR("Extension class [" + ext_class + "] not instantiated !",e);    	
		      		
		        	}
			        if (ext_run != null ) {
			        	ext_run.run();
			        }
			        else {
		            	  LOG.ERROR("Extension class [" + ext_class + "] not implemented Runnable interface !");    	
			        }
		          }
			}
		}
    }


//    static {
    private static void initClasses()
    {
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
        //FileTestModule = RubyTypeFactory.getModule(RubyFileTestModule.class);
        //GCModule = RubyTypeFactory.getModule(RubyGC.class);

        //MarshalModule = RubyTypeFactory.getModule(RubyMarshalModule.class);//RubyAPI.defineModule("Marshal");
        //MathModule = RubyTypeFactory.getModule(RubyMathModule.class);
        //MathModule = RubyAPI.defineModule("Math");
        //ObjectSpaceModule = RubyTypeFactory.getModule(ObjectSpace.class);
        
        ProcessModule = RubyAPI.defineModule("Process");

        BindingClass = RubyAPI.defineClass("Binding", RubyRuntime.ObjectClass);

      //RHO_ADDED
        FileTestModule = RubyAPI.defineModule("FileTest");
        RubyFileTestModule_Methods.initMethods(FileTestModule);

        GCModule = RubyAPI.defineModule("GC");
        RubyGC_Methods.initMethods(GCModule);

        MarshalModule = RubyAPI.defineModule("Marshal");
        RubyMarshalModule_Methods.initMethods(MarshalModule);
        
        MathModule = RubyAPI.defineModule("Math");
        RubyMathModule_Methods.initMethods(MathModule);

//        ObjectSpaceModule = RubyAPI.defineModule("ObjectSpace");
//        RubyObjectSpace_Methods.initMethods(ObjectSpaceModule);
        
        RubyModule_Methods.initMethods(ModuleClass);
        RubyModule.initDummyMethods(ModuleClass);
        RubyKernelModule_Methods.initMethods(KernelModule);
        RubyClass_Methods.initMethods(ClassClass);
        RubyClass.initDummyMethods(ClassClass);
        RubyObject_Methods.initMethods(ObjectClass);

        ObjectClass.includeModule(KernelModule);
        
        ArrayClass = RubyAPI.defineClass("Array", RubyRuntime.ObjectClass);
        ArrayClass.includeModule(EnumerableModule);
        RubyArray_Methods.initMethods(ArrayClass);
        
        SymbolClass = RubyAPI.defineClass("Symbol", RubyRuntime.ObjectClass);
        RubySymbol_Methods.initMethods(SymbolClass);

        IOClass = RubyAPI.defineClass("IO", RubyRuntime.ObjectClass);
        RubyIO_Methods.initMethods(IOClass);
        
        StringClass = RubyAPI.defineClass("String", RubyRuntime.ObjectClass);
        RubyString_Methods.initMethods(StringClass);

        NumericClass = RubyAPI.defineClass("Numeric", RubyRuntime.ObjectClass);
        RubyNumeric_Methods.initMethods(NumericClass);
        
        IntegerClass = RubyAPI.defineClass("Integer", RubyRuntime.NumericClass);
        RubyInteger_Methods.initMethods(IntegerClass);
        
        FixnumClass = RubyAPI.defineClass("Fixnum", RubyRuntime.IntegerClass);
        RubyFixnum_Methods.initMethods(FixnumClass);
        
        BignumClass = RubyAPI.defineClass("Bignum", RubyRuntime.IntegerClass);
        RubyBignum_Methods.initMethods(BignumClass);
        
        FloatClass = RubyAPI.defineClass("Float", RubyRuntime.NumericClass);
        RubyFloat_Methods.initMethods(FloatClass);
        
        RangeClass = RubyAPI.defineClass("Range", RubyRuntime.ObjectClass);
        RubyRange_Methods.initMethods(RangeClass);

        RegexpClass = RubyAPI.defineClass("Regexp", RubyRuntime.ObjectClass);
        RubyRegexp_Methods.initMethods(RegexpClass);
        
        TimeClass = RubyAPI.defineClass("Time", RubyRuntime.ObjectClass);
        RubyTime_Methods.initMethods(TimeClass);

        HashClass = RubyAPI.defineClass("Hash", RubyRuntime.ObjectClass);
        HashClass.includeModule(EnumerableModule);
        RubyHash_Methods.initMethods(HashClass);

        ProcClass = RubyAPI.defineClass("Proc", RubyRuntime.ObjectClass);
        RubyProc_Methods.initMethods(ProcClass);

        ThreadClass = RubyAPI.defineClass("Thread", RubyRuntime.ObjectClass);
        RubyThread_Methods.initMethods(ThreadClass);
        
        IOClass = RubyAPI.defineClass("IO", RubyRuntime.ObjectClass);
        RubyIO_Methods.initMethods(IOClass);
        
        FileClass = RubyAPI.defineClass("File", RubyRuntime.IOClass);
        RubyFile_Methods.initMethods(FileClass);
        
        MethodClass = RubyAPI.defineClass("Method", RubyRuntime.ObjectClass);
        RubyMethodValue_Methods.initMethods(MethodClass);

        TimeClass = RubyAPI.defineClass("Time", RubyRuntime.ObjectClass);
        RubyTime_Methods.initMethods(TimeClass);

        MatchDataClass = RubyAPI.defineClass("MatchData", RubyRuntime.ObjectClass);
        RubyMatchData_Methods.initMethods(MatchDataClass);

        DirClass = RubyAPI.defineClass("Dir", RubyRuntime.ObjectClass);
        RubyDir_Methods.initMethods(DirClass);
        DirClass.includeModule(EnumerableModule);

        StructClass = RubyAPI.defineClass("StructData", RubyRuntime.ObjectClass);
        RubyStruct_Methods.initMethods(StructClass);
        
        ThreadGroupClass = RubyAPI.defineClass("ThreadGroup", RubyRuntime.ObjectClass );
        RubyThreadGroup_Methods.initMethods(ThreadGroupClass);
        
        ExceptionClass = RubyAPI.defineClass("Exception", RubyRuntime.ObjectClass);
        RubyExceptionValue_Methods.initMethods(ExceptionClass);

		SQLite3Module = RubyAPI.defineModule("SQLite3");
		DatabaseClass = SQLite3Module.defineClass("Database", RubyRuntime.ObjectClass);
//        DBAdapterClass = RubyAPI.defineClass("DbAdapter", RubyRuntime.ObjectClass);
//        PerstLiteAdapter.initMethods(DBAdapterClass);
        
        SyncEngineClass = RubyAPI.defineModule("SyncEngine" );//, RubyRuntime.ObjectClass);
//        SyncEngine.initMethods(SyncEngineClass);

        StringScannerClass = RubyAPI.defineClass("StringScanner", RubyRuntime.ObjectClass);
        StringScanner.initMethods(StringScannerClass);

        PhonebookClass = RubyAPI.defineClass("Phonebook", RubyRuntime.ObjectClass);
        PBRecordClass = RubyAPI.defineClass("PBRecord", RubyRuntime.ObjectClass);
        GeoLocationClass = RubyAPI.defineClass("GeoLocation", RubyRuntime.ObjectClass);
        RubyModule rhoModule = RubyAPI.defineModule("Rho"); 
        AsyncHttpModule = rhoModule.defineModule("AsyncHttp");
		SystemClass = RubyAPI.defineClass("System", RubyRuntime.ObjectClass);
        JSONClass = rhoModule.defineModule("JSON" );//, RubyRuntime.ObjectClass);
        CameraClass = RubyAPI.defineClass("Camera", RubyRuntime.ObjectClass);
        WebViewClass = RubyAPI.defineClass("WebView", RubyRuntime.ObjectClass);
        RhoConfClass = RubyAPI.defineClass("RhoConf", RubyRuntime.ObjectClass);
        AlertClass = RubyAPI.defineClass("Alert", RubyRuntime.ObjectClass);        
        DateTimePickerClass = RubyAPI.defineClass("DateTimePicker", RubyRuntime.ObjectClass);
        NativeBarClass = RubyAPI.defineClass("NativeBar", RubyRuntime.NativeBarClass);
        RingtoneManagerClass = RubyAPI.defineClass("RingtoneManager", RubyRuntime.ObjectClass);
        MediaErrorClass = RubyAPI.defineClass("MediaError", RubyRuntime.ObjectClass);
        MutexClass = RubyAPI.defineClass("Mutex", RubyRuntime.ObjectClass);
        TCPSocketClass = RubyAPI.defineClass("TCPSocket", RubyRuntime.ObjectClass);
        SSLSocketClass = RubyAPI.defineClass("SSLSocket", RubyRuntime.ObjectClass);
        MapViewClass = RubyAPI.defineModule("MapView" );//, RubyRuntime.ObjectClass);
        RubyMutex.initMethods(MutexClass);

        RubyModule rexmlModule = RubyAPI.defineModule("REXML"); 
        RubyModule parsersModule = rexmlModule.defineModule("Parsers");
        XMLParserClass = parsersModule.defineClass("BaseParser", RubyRuntime.ObjectClass);
                
//      RhoPhonebook.initMethods(PhonebookClass);
        
//
        /*RubyTypeFactory.getClass(RubyObject.class);
        RubyTypeFactory.getClass(RubyModule.class);
        RubyTypeFactory.getClass(RubyClass.class);

        RubyModule m = RubyTypeFactory.getModule(RubyKernelModule.class);*/
        respondToMethod = KernelModule.findMethod(RubyID.RESPOND_TO_P);
/*        RubyTypeFactory.getModule(RubyRandom.class);

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
        ThreadClass = RubyTypeFactory.getClass(RubyThread.class);*/
        UnboundMethodClass = RubyAPI.defineClass("UnboundMethod", MethodClass);
//        ExceptionClass = RubyTypeFactory.getClass(RubyExceptionValue.class);
        SystemExitClass = RubyAPI.defineClass("SystemExit", ExceptionClass);
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

        //TOP_LEVEL_SELF_VALUE = RubyTypeFactory.getObject(RubyTopSelf.class);
      //RHO_ADDED
        TopSelfClass = RubyAPI.defineClass("__TopSelf", ObjectClass);
        RubyTopSelf_Methods.initMethods(TopSelfClass);        
        TOP_LEVEL_SELF_VALUE = TopSelfClass;//new RubyObject( TopSelfClass );//RubyTopSelf();
        
        StringIOClass = RubyAPI.defineClass("StringIO", RubyRuntime.ObjectClass);
        RubyStringIO_Methods.initMethods(StringIOClass);
        StringIOClass.includeModule(EnumerableModule);
        
        RhoSupport.init();
        
      //  
        //StringIOClass = RubyTypeFactory.getClass(RubyStringIO.class);

        RubyThread.init();

    }

    private static void initARGV(String[] args) {
        RubyArray argv = new RubyArray();
        if (args != null) {
//            for (String arg : args) {
        	for( int i = 0; i<args.length; i++){
        		String arg = args[i];        	
                argv.add(ObjectFactory.createString(arg));
            }
        }

        RubyAPI.setTopLevelConstant(argv, "ARGV");
    }

    private static void loadBuildinDotRb() {
        try {
            Class c = Class.forName(RhoSupport.getAppName() + ".builtinME.main");
            Object o = c.newInstance();
            RubyProgram p = (RubyProgram) o;
            p.invoke();
        } catch (ClassNotFoundException e) {
        } catch (InstantiationException e) {
        } catch (IllegalAccessException e) {
            throw new Error(e.toString());
        }
    }

    public static void init(String[] args) {
        if (RubyRuntime.running) {
            return;
        }

        initClasses();
        
        initARGV(args);

	registerExtensions();

        RubyAPI.setTopLevelConstant(RubyAPI.isWindows() ? ObjectFactory.createString("mswin32") : ObjectFactory.createString("java"), "RUBY_PLATFORM");

        if (RubyAPI.isWindows()) {
            RubyAPI.setConstant(ObjectFactory.createString("\\"), RubyRuntime.FileClass, "ALT_SEPARATOR");
            RubyAPI.setConstant(ObjectFactory.createString(";"), RubyRuntime.FileClass, "PATH_SEPARATOR");
        } else {
        	RubyAPI.setConstant(ObjectFactory.createString(":"), RubyRuntime.FileClass, "PATH_SEPARATOR");
        }

    	RubyAPI.setConstant(ObjectFactory.createFixnum(RubyRegexp.RE_OPTION_IGNORECASE), RubyRuntime.RegexpClass, "IGNORECASE");
    	RubyAPI.setConstant(ObjectFactory.createFixnum(RubyRegexp.RE_OPTION_EXTENDED), RubyRuntime.RegexpClass, "EXTENDED");
    	RubyAPI.setConstant(ObjectFactory.createFixnum(RubyRegexp.RE_OPTION_MULTILINE), RubyRuntime.RegexpClass, "MULTILINE");
        
        //RubyTypeFactory.getObject(RubyENV.class);
        //RHO_ADDED
        RubyClass EnvClass = RubyAPI.defineClass("ENV", RubyRuntime.ObjectClass);
        RubyENV_Methods.initMethods(EnvClass);
        //
        GlobalVariables.initialize();

        //RHO_ADED
        RubyAPI.setTopLevelConstant(RubyConstant.QTRUE, "RHO_ME");
        
        try{
	        if( RhoClassFactory.createRhoRubyHelper().getPlatform().equals("Blackberry" ) )
	        	RubyAPI.setTopLevelConstant(RubyConstant.QTRUE, "RHO_DBME");
        }catch(Exception e)
        {
        }
        
        RubyAPI.setTopLevelConstant(ObjectFactory.createString("1.9.1"), "RUBY_VERSION");
        RubyAPI.setTopLevelConstant(ObjectFactory.createString("2009-05-12"), "RUBY_RELEASE_DATE");
        RubyAPI.setTopLevelConstant(ObjectFactory.createString("java"), "RUBY_PLATFORM");
        RubyAPI.setTopLevelConstant(ObjectFactory.createInteger(129), "RUBY_PATCHLEVEL");
        RubyAPI.setTopLevelConstant(ObjectFactory.createInteger(23412), "RUBY_REVISION");
        RubyAPI.setTopLevelConstant(ObjectFactory.createString("xruby"), "RUBY_DESCRIPTION");
        RubyAPI.setTopLevelConstant(ObjectFactory.createString("ruby - Copyright (C)"), "RUBY_COPYRIGHT");
        RubyAPI.setTopLevelConstant(ObjectFactory.createString("ruby"), "RUBY_ENGINE");
        
        //
        
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
    	/*PrintStream ps = getPrintStream(os);
        System.setOut(ps);
        updateStdout();*/
    	//RHO_COMMENT: setStdout
    }

    private static void updateStdout() {
        RubyIO io = new RubyFile(new OutputStreamExecutor(false), RubyRuntime.IOClass);
        RubyAPI.setTopLevelConstant(io, "STDOUT");
        GlobalVariables.set(io, "$stdout");
    }

    private static void updateStderr() {
        RubyIO io = new RubyFile(new OutputStreamExecutor(true), RubyRuntime.IOClass);
        RubyAPI.setTopLevelConstant(io, "STDERR");
        GlobalVariables.set(io, "$stderr");
    }

    private static PrintStream getPrintStream(OutputStream os) {
        return (os instanceof PrintStream) ? (PrintStream)os : new PrintStream(os);
    }
}
