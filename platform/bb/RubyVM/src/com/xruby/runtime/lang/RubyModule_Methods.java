package com.xruby.runtime.lang;
//import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
//import com.xruby.runtime.lang.RubyModule;
public class RubyModule_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "const_get", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).constGet(arg);}
});
klass.defineMethod( "const_defined?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).isConstDefined(arg);}
});
klass.defineMethod( "constants", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyModule)receiver).getConstants();}
});
klass.defineMethod( "method_defined?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).isMethodDefined(arg);}
});

klass.defineMethod( "public_method_defined?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).isPublicMethodDefined(arg);}
});
		
klass.defineMethod( "===", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).caseEqual(arg);}
});
klass.defineMethod( "private_class_method", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).private_class_method(args);}
});
klass.defineMethod( "protected_instance_methods", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).protected_instance_methods(args);}
});
klass.defineMethod( "const_set", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyModule)receiver).constSet(arg0, arg1);}
});
klass.defineMethod( ">=", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).opGe(arg);}
});
klass.defineMethod( "module_eval", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).module_eval(args, block);}
});
klass.aliasMethod("class_eval","module_eval");
klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyModule)receiver).modName();}
});
klass.aliasMethod("name","to_s");
klass.definePrivateMethod( "private", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).modPrivate(args);}
});
klass.defineMethod( "<=>", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).opSpaceship(arg);}
});
klass.defineMethod( "module_function", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).module_function(args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return ((RubyModule)receiver).module_function();}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(1, 0, block != null? block.createdByLambda() : false );
		args.add(arg);
		return ((RubyModule)receiver).module_function(args);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return ((RubyModule)receiver).module_function(args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.definePrivateMethod( "protected", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).modProtected(args);}
});
klass.defineMethod( "public_class_method", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).public_class_method(args);}
});
klass.defineMethod( "ancestors", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyModule)receiver).ancestors();}
});
klass.defineMethod( "instance_methods", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).instance_methods(args);}
});
klass.defineMethod( "initialize", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyModule)receiver).initializeModule(block);}
});
klass.defineMethod( "extend_object", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).extendObject(arg);}
});
klass.defineMethod( "remove_method", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).remove_method(args);}
});
klass.definePrivateMethod( "attr", new RubyOneOrTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).attr(arg);}
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyModule)receiver).attr(arg0, arg1);}
});
klass.defineMethod( "append_features", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).append_features(arg);}
});
klass.defineMethod( "undef_method", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).undef_method(arg);}
});
klass.definePrivateMethod( "attr_accessor", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).attrAccessor(args);}
});
klass.definePrivateMethod( "include", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).include(args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return ((RubyModule)receiver).include();}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).include(arg);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return ((RubyModule)receiver).include(args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "define_method", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).define_method(args, block);}
});
klass.definePrivateMethod( "attr_writer", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).attrWriter(args);}
});
klass.defineMethod( "alias_method", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyModule)receiver).alias_method(arg0, arg1);}
});
klass.definePrivateMethod( "attr_reader", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).attrReader(args);}
});
klass.defineMethod( "public_instance_methods", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).public_instance_methods(args);}
});
klass.defineMethod( "private_instance_methods", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).private_instance_methods(args);}
});
klass.defineMethod( "instance_method", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).instance_method(arg);}
});

klass.definePrivateMethod( "public", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyModule)receiver).modPublic(args);}
});
klass.defineMethod( ">", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).opGt(arg);}
});
klass.defineMethod( "inspect", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyModule)receiver).rubyInspect();}
});
klass.defineMethod( "<", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyModule)receiver).opLt(arg);}
});
klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyModule.allocModule(receiver);	}} );
}}
