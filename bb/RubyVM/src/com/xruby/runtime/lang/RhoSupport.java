package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.ObjectFactory;
//import com.xruby.runtime.lang.*;

public class RhoSupport {

	public static RubyModule SystemModule;
	
	public static void init(){
		
		SystemModule = RubyAPI.defineModule("System");
		
		SystemModule.defineModuleMethod( "get_property", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return get_property(receiver, arg);}
		});
		
	}
	
    //@RubyLevelMethod(name="get_property", module=true)
    public static RubyValue get_property(RubyValue receiver, RubyValue arg) {
    	String strPropName = arg.toStr();
    	if ( strPropName.equalsIgnoreCase("platform") ){
    		return ObjectFactory.createString("Blackberry");
    	}
    	
    	return RubyConstant.QNIL;
    }
    
}
