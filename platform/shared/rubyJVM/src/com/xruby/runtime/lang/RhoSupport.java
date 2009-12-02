package com.xruby.runtime.lang;

import com.rho.IRhoRubyHelper;
import com.rho.RhoClassFactory;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;

public class RhoSupport {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RhoSupport");
	
	public static RubyModule SystemModule;
//	private static String    m_strCurAppPath;
	
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
		RubyRuntime.KernelModule.defineModuleMethod( "rhoInfo", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return rhoInfo(receiver, arg, block);}
		});
		RubyRuntime.KernelModule.defineModuleMethod( "rhoError", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
				return rhoError(receiver, args, block);}
		});
		
		SystemModule = RubyAPI.defineModule("System");
		SystemModule.defineModuleMethod( "get_property", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return get_property(receiver, arg);}
		});
		SystemModule.defineModuleMethod( "has_network", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return has_network(receiver);}
		});
		SystemModule.defineModuleMethod( "get_locale", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
				String strLocale = "";
		    	try {
		    		strLocale = RhoClassFactory.createRhoRubyHelper().getLocale();
				} catch (Exception e) {
					e.printStackTrace();
				} 
				return ObjectFactory.createString(strLocale);
			}
		});

		RubyRuntime.KernelModule.defineModuleMethod( "rho_get_app_property", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return rb_rho_get_app_property(receiver, arg, block);}
		});
	}

	private static final RhoLogger RHOLOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RHO");
	
    public static RubyValue rhoInfo(RubyValue receiver, RubyValue arg, RubyBlock block) {
        String msg = arg.toStr();
        RHOLOG.INFO(msg);
        return RubyConstant.QNIL;
    }
    public static RubyValue rhoError(RubyValue receiver, RubyArray args, RubyBlock block) {
        //String msg = arg.toStr();
        //RHOLOG.ERROR(msg);
        return RubyConstant.QNIL;
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
        required_file += ".main";
        return "xruby." + required_file;
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
		}catch(ClassNotFoundException exc){}
		
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
    
    //@RubyLevelMethod(name="get_property", module=true)
    public static RubyValue get_property(RubyValue receiver, RubyValue arg) {
    	String strPropName = arg.toStr();
    	if ( strPropName.equalsIgnoreCase("platform") ){
    		try {
    			return ObjectFactory.createString( RhoClassFactory.createRhoRubyHelper().getPlatform() );
    		}catch ( Exception e ) {
    			return ObjectFactory.createString("Unknown");
    		}
    	}
    	
    	return RubyConstant.QNIL;
    }

    //@RubyLevelMethod(name="has_network", module=true)
    public static RubyValue has_network(RubyValue receiver) {
    	try {
			return RhoClassFactory.createRhoRubyHelper().hasNetwork() ? RubyConstant.QTRUE : RubyConstant.QFALSE;
		} catch (Exception e) {
			e.printStackTrace();
		} 
		return RubyConstant.QFALSE;
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
        	return RubyConstant.QNIL;
        
        return ObjectFactory.createString(strValue);
    }
    
    //@RubyLevelMethod(name="__load_with_reflection__", module=true)
    public static RubyValue loadWithReflection(RubyValue receiver, RubyValue arg, RubyBlock block) {
        String required_file = arg.toStr();
        if ( required_file.endsWith("/") || required_file.endsWith("\\") )
        	return RubyConstant.QTRUE;

        if ( required_file.equals("socket") || required_file.equals("strscan") || required_file.equals("enumerator") )
        	return RubyConstant.QTRUE;
        
        String name = RhoSupport.createMainClassName(required_file);
        try {
        	
        	LOG.INFO("require_compiled: " + required_file);
        	
        	Class c = null;
            try {
            	c = Class.forName(name);
            } catch (ClassNotFoundException e) {
            }
            if ( c == null ){
            	String altPath = "/apps/app/";
            	name = RhoSupport.createMainClassName(altPath+required_file);
            	c = Class.forName(name);
            	
            	arg = ObjectFactory.createString(altPath+required_file);
            }
            
            Object o = c.newInstance();
            RubyProgram p = (RubyProgram) o;

            //$".push(file_name) unless $".include?(file_name)
            RubyValue var = GlobalVariables.get("$\"");
            if ( var != RubyConstant.QNIL ){
	            RubyArray a = (RubyArray)var;
	            if (a.include(arg) == RubyConstant.QFALSE) {
	                a.push(arg);
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
