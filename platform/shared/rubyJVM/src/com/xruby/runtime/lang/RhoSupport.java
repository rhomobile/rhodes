package com.xruby.runtime.lang;

import com.rho.IRhoRubyHelper;
import com.rho.RhoClassFactory;
import com.rho.RhoConf;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhodesApp;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import j2me.lang.StringMe;

public class RhoSupport {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RhoSupport");
	
	public static RubyClass  RhoLogClass;
	
	public static void init(){

		RubyRuntime.KernelModule.defineModuleMethod( "eval_compiled_file", new RubyTwoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block ){
				return eval_compiled_file(receiver, arg1, arg2);}
		});
		RubyRuntime.KernelModule.defineModuleMethod( "__rhoGetCurrentDir", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return RhoGetCurrentDir(receiver);}
		});
		RubyRuntime.KernelModule.defineModuleMethod( "__load_with_reflection__", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return loadWithReflection(receiver, arg, block);}
		});
/*		RubyRuntime.KernelModule.defineModuleMethod( "rhoInfo", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return rhoInfo(receiver, arg, block);}
		});
		RubyRuntime.KernelModule.defineModuleMethod( "rhoError", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
				return rhoError(receiver, args, block);}
		});*/
		RubyRuntime.KernelModule.defineModuleMethod( "rho_get_app_property", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return rb_rho_get_app_property(receiver, arg, block);}
		});
		RubyRuntime.KernelModule.defineModuleMethod( "__rhoGetCallbackObject", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return RhodesApp.getInstance().getCallbackObject(arg.toInt());}
		});
		
		RhoLogClass = RubyAPI.defineClass("RhoLog", RubyRuntime.ObjectClass);
		RhoLog.initMethods(RhoLogClass);
	}

	public static class RhoLog extends RubyBasic 
	{
		private static final RhoLogger APPLOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
			new RhoLogger("APP");
	
	    private RhoLog() 
	    {
	        super(RhoLogClass);
	    }
		
	    public static RhoLog alloc(RubyValue receiver) {
	        return new RhoLog();
	    }

	    public RhoLog initialize() {
	        return this;
	    }
	    
	    public RubyValue rhoLog_Write(RubyValue arg) 
	    {
	        String msg = arg.toStr();
	        APPLOG.INFO(msg);
	        return RubyConstant.QNIL;
	    }
		
	    public RubyValue rhoLog_Info(RubyValue arg, RubyValue arg2) 
	    {
	        String cat = arg.toStr();
	        String msg = arg2.toStr();
	        
			RhoLogger logger = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
				new RhoLogger(cat);
	        
			logger.INFO(msg);
	        return RubyConstant.QNIL;
	    }
	    
	    public RubyValue rhoLog_Error(RubyValue arg, RubyValue arg2) {
	        String cat = arg.toStr();
	        String msg = arg2.toStr();
	        
			RhoLogger logger = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
				new RhoLogger(cat);
	        
			logger.ERROR(msg);
	        return RubyConstant.QNIL;
	    }
	    
	    public static void initMethods( RubyClass klass)
	    {
	    	klass.defineMethod( "initialize", new RubyNoArgMethod(){ 
				protected RubyValue run(RubyValue receiver, RubyBlock block )
				{
					return ((RhoLog)receiver).initialize();
				}
			});
			klass.defineAllocMethod(new RubyNoArgMethod(){
				protected RubyValue run(RubyValue receiver, RubyBlock block )	{
					return RhoLog.alloc(receiver);
				}
			});
			
			klass.defineMethod( "write", new RubyOneArgMethod(){ 
				protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
				{
					return ((RhoLog)receiver).rhoLog_Write(arg);
				}
			});
			klass.defineMethod( "print", new RubyOneArgMethod(){ 
				protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
				{
					return ((RhoLog)receiver).rhoLog_Write(arg);
				}
			});
			klass.defineMethod( "info", new RubyTwoArgMethod(){ 
				protected RubyValue run(RubyValue receiver, RubyValue arg, RubyValue arg1, RubyBlock block )
				{
					return ((RhoLog)receiver).rhoLog_Info(arg, arg1);
				}
			});
			klass.defineMethod( "error", new RubyTwoArgMethod(){ 
				protected RubyValue run(RubyValue receiver, RubyValue arg, RubyValue arg1, RubyBlock block )
				{
					return ((RhoLog)receiver).rhoLog_Error(arg, arg1);
				}
			});
	    	
	    }
	}
	
	private static String m_strAppName = null;
	public static String getAppName()
	{
		try{
	    	/*if ( m_strAppName == null)
	    	{
	    		IRhoRubyHelper systemInfo = RhoClassFactory.createRhoRubyHelper();
	    		m_strAppName = systemInfo.getAppProperty("MIDlet-Name");
	        	if ( m_strAppName == null || m_strAppName.length() == 0 )
	        		m_strAppName = systemInfo.getModuleName();
	        	if ( m_strAppName == null || m_strAppName.length() == 0 )
	        		m_strAppName = "rhodesApp";
	        	
	        	m_strAppName += "_"; 
	    	}*/
			m_strAppName = "xruby";
		}catch(Exception exc)
		{
			LOG.ERROR("getAppName failed.", exc);
			m_strAppName = "";
		}
    	return m_strAppName;
	}
	
    public static String createMainClassName(String required_file) {

        //remove ".rb" if has one
        if (required_file.endsWith(".rb")) {
            required_file = required_file.substring(0, required_file.length() - 3);
        }else if (required_file.endsWith(".iseq")) {
            required_file = required_file.substring(0, required_file.length() - 5);
        }

        if ( required_file.charAt(0) == '/' || required_file.charAt(0) == '\\' )
        	required_file = required_file.substring(1);
        
        required_file = required_file.replace('/', '.');
        required_file = required_file.replace('\\', '.');
        required_file = StringMe.replaceAll(required_file,"-", "minus");
        
        /*int nDot = required_file.lastIndexOf('.');
        if (nDot>0)
        {
        	String fName = required_file.substring(nDot+1);
        	required_file = required_file.substring(0, nDot+1) + fName.toLowerCase();
        }*/
        
        required_file += ".main";
        return getAppName() + "." + required_file;
    }

	//@RubyLevelMethod(name="__rhoGetCurrentDir", module=true)
    public static RubyValue RhoGetCurrentDir(RubyValue receiver) {
    	return ObjectFactory.createString("");
    }
    
	public static Class findClass(String path){
		String name = createMainClassName(path);
		Class c = null;
		
		try{
			c = Class.forName(name);

			if ( c != null )
			{
	            Object o = c.newInstance();
	            if ( !(o instanceof RubyProgram) )
	            	return null;
			}
			
			//if ( !c.isAssignableFrom(RubyProgram.class) )
			//	return null;
		}catch(Exception exc){}
		
		return c;
	}
    
	//@RubyLevelMethod(name="eval_compiled_file", module=true)
    public static RubyValue eval_compiled_file(RubyValue receiver, RubyValue file, RubyValue bindingArg) {
        RubyBinding binding = null;
        if (bindingArg != null && bindingArg instanceof RubyBinding) {
            binding = (RubyBinding)bindingArg;
        }
    	
    	return evalPrecompiledFile( file.toStr(), binding );
    }
	
	private static RubyValue evalPrecompiledFile( String file_name, RubyBinding binding ){
		String name = createMainClassName(file_name);
        try {
            Class c = Class.forName(name);
            Object o = c.newInstance();
            RubyProgram p = (RubyProgram) o;

            if (null != binding) {
                return p.invoke(binding.getSelf(), binding.getVariables(), binding.getBlock(), binding.getScope());
            } else {
                return p.invoke();
            }
        }
        catch (Exception e) {
        	
			LOG.ERROR("evalPrecompiledFile failed : " + name, e);
			throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
        }
	}
    
    public static RubyValue rb_rho_get_app_property(RubyValue receiver, RubyValue arg, RubyBlock block) {
        String name = arg.toStr();
        String strValue = null;
        try{
	        IRhoRubyHelper systemInfo = RhoClassFactory.createRhoRubyHelper();
			strValue = systemInfo.getAppProperty(name);
        }catch(Exception exc)
        {
        }
        
        if ( strValue == null || strValue.length() == 0 )
        {
        	if ( RhoConf.getInstance().isExist(name) )
        		strValue = RhoConf.getInstance().getString(name);
        	else
        		return RubyConstant.QNIL;
        }
        
        return ObjectFactory.createString(strValue);
    }
    
    //@RubyLevelMethod(name="__load_with_reflection__", module=true)
    public static RubyValue loadWithReflection(RubyValue receiver, RubyValue arg, RubyBlock block) {
        String required_file = arg.toStr();
        if ( required_file.endsWith("/") || required_file.endsWith("\\") )
        	return RubyConstant.QTRUE;

        if ( required_file.equals("socket") || required_file.equals("strscan") || required_file.equals("enumerator") ||
        	 required_file.equals("stringio") )
        	return RubyConstant.QTRUE;
        
        String name = RhoSupport.createMainClassName(required_file);
        RubyValue arg1 = null;        
        try {
        	
        	LOG.INFO("require_compiled: " + required_file);
        	
        	Class c = null;
            try {
            	c = Class.forName(name);
            } catch (ClassNotFoundException e) {
            	LOG.TRACE("require_compiled first:" + e.toString());
            }
            if ( c == null ){
            	String altPath = "/apps/app/";
            	name = RhoSupport.createMainClassName(altPath+required_file);
            	
                try {
                	c = Class.forName(name);
                } catch (ClassNotFoundException e) {
                	LOG.TRACE("require_compiled second: " + e.toString());
                }
            	
            	arg1 = ObjectFactory.createString(altPath+required_file);
            }

            if ( c == null ){
            	String altPath = "/apps/";
            	name = RhoSupport.createMainClassName(altPath+required_file);
            	
               	c = Class.forName(name);
            	arg1 = ObjectFactory.createString(altPath+required_file);
            }
            
            Object o = c.newInstance();
            if ( !(o instanceof RubyProgram) )
            {
            	LOG.INFO("Wrong object: " + o.getClass().getName());
            	return RubyConstant.QFALSE;
            }
            
            RubyProgram p = (RubyProgram) o;

            //$".push(file_name) unless $".include?(file_name)
            RubyValue var = GlobalVariables.get("$\"");
            if ( var != RubyConstant.QNIL ){
	            RubyArray a = (RubyArray)var;
	            if (a.include(arg) == RubyConstant.QFALSE) {
	                a.push(arg);
	                if ( arg1 != null )
	                	a.push(arg1);
	            }
            }
            
            p.invoke();
            return RubyConstant.QTRUE;
        } catch (ClassNotFoundException e) {
            return RubyConstant.QFALSE;
        } catch (InstantiationException e) {
            return RubyConstant.QFALSE;
        } catch (IllegalAccessException e) {
            return RubyConstant.QFALSE;
        }
    }

//	public static void setCurAppPath(String curAppPath) {
//		m_strCurAppPath = curAppPath;
//	}
    
	
	public static String getRhoDBVersion(){
		String strVer = "";
		RubyValue val = RubyRuntime.ObjectClass.getConstant("Rhodes");
		if ( val != null && val != RubyConstant.QNIL && val instanceof RubyModule){
			RubyModule module = (RubyModule)val;
			RubyValue ver = module.getConstant("DBVERSION");
			if ( ver != null && ver != RubyConstant.QNIL )
				strVer = ver.toStr();
		}
		
		return strVer;
	}
}
