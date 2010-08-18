package com.xruby.runtime.lang;
//import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
//import com.xruby.runtime.builtin.RubyKernelModule;
public class RubyKernelModule_Methods{
public static void initMethods( RubyModule klass){

klass.defineMethod( "taint", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.taint(receiver);}
});
klass.defineMethod( "untaint", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.untaint(receiver);}
});

klass.defineMethod( "methods", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.objMethods(receiver);}
});
klass.defineMethod( "===", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.objEqual(receiver, arg);}
});
klass.defineMethod( "send", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.send(receiver, args, block);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.send(receiver, block);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.send(receiver, arg, block);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return RubyKernelModule.send(receiver, arg0, arg1, block);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.aliasMethod("__send__","send");
klass.defineModuleMethod( "sprintf", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.sprintf(receiver, args);}
});
klass.aliasMethod("format","sprintf");
klass.defineMethod( "open", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.open(receiver, args, block);}
});
klass.defineModuleMethod( "lambda", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.lambda(receiver, block);}
});
klass.aliasMethod("proc","lambda");
klass.defineMethod( "public_methods", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.publicMethods(receiver);}
});
klass.defineModuleMethod( "at_exit", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.atExit(receiver, block);}
});
klass.defineModuleMethod( "puts", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.puts(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.puts(receiver);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(1, 0, block != null? block.createdByLambda() : false );
		args.add(arg);
		return RubyKernelModule.puts(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return RubyKernelModule.puts(receiver, args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "frozen?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.frozen(receiver);}
});
klass.defineMethod( "instance_eval", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.instanceEval(receiver, args, block);}
});
klass.defineMethod( "instance_of?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.instanceOf(receiver, arg);}
});
klass.defineMethod( "clone", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.objClone(receiver);}
});
klass.aliasMethod("dup","clone");
klass.defineModuleMethod( "eval", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.eval(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.eval(receiver, arg);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return RubyKernelModule.eval(receiver, arg0, arg1);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `"+ this.getID() + "': wrong number of arguments");}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "class", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.objRubyClass(receiver);}
});
klass.defineModuleMethod( "untrace_var", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.untrace_var(receiver, args, block);}
});
klass.defineMethod( "singleton_methods", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.objSingletonMethods(receiver);}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.objSingletonMethods(receiver, arg);}
});
klass.defineModuleMethod( "sleep", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.sleep(receiver, arg);}
});
klass.defineModuleMethod( "require_java", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.requireJava(receiver, arg, block);}
});
klass.defineModuleMethod( "require_java", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.requireJava(receiver, arg, block);}
});
klass.defineModuleMethod( "exit", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.exit(receiver);}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.exit(receiver, arg);}
});
klass.defineModuleMethod( "method_missing", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.methodMissing(receiver, args);}
});
klass.defineModuleMethod( "binding", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.binding(receiver, args);}
});
klass.defineMethod( "inspect", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.objInsepct(receiver);}
});
klass.defineModuleMethod( "caller", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.call(receiver);}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.call(receiver, arg);}
});
klass.defineModuleMethod( "sub", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.sub(receiver, args, block);}
});
klass.defineMethod( "raise", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.raise(receiver, args);}
});
klass.aliasMethod("fail","raise");
klass.defineModuleMethod( "trace_var", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.trace_var(receiver, args, block);}
});
klass.defineModuleMethod( "gsub!", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.gsubBang(receiver, args, block);}
});
klass.defineModuleMethod( "catch", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.catchMethod(receiver, arg, block);}
});
klass.defineMethod( "kind_of?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.kindOf(receiver, arg);}
});
klass.aliasMethod("is_a?","kind_of?");
klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.anyObjToS(receiver);}
});
klass.defineModuleMethod( "throw", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.throwMethod(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.throwMethod(receiver);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(1, 0, block != null? block.createdByLambda() : false );
		args.add(arg);
		return RubyKernelModule.throwMethod(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return RubyKernelModule.throwMethod(receiver, args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "==", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.opEqual(receiver, arg);}
});
klass.aliasMethod("equal?","==");
klass.aliasMethod("eql?","==");
klass.defineModuleMethod( "loop", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.loop(receiver, args, block);}
});
klass.defineModuleMethod( "Float", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.toFloat(receiver, arg);}
});
klass.defineModuleMethod( "printf", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.printf(receiver, args);}
});
klass.defineModuleMethod( "block_given?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.blockGivenP(receiver, block);}
});
klass.defineModuleMethod( "block_given?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.blockGivenP(receiver, block);}
});
klass.defineModuleMethod( "gsub", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.gsub(receiver, args, block);}
});
klass.defineMethod( "tainted?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.tainted(receiver);}
});
klass.defineMethod( "extend", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.extend(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.extend(receiver);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.extend(receiver, arg);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return RubyKernelModule.extend(receiver, args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineModuleMethod( "gets", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.gets(receiver);}
});
klass.defineMethod( "object_id", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.objectId(receiver);}
});
klass.aliasMethod("__id__","object_id");
klass.aliasMethod("hash","object_id");
klass.defineMethod( "respond_to?", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.respond_to(receiver, args);}
});
klass.defineModuleMethod( "print", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.print(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.print(receiver);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.print(receiver, arg);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return RubyKernelModule.print(receiver, args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "freeze", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.freeze(receiver);}
});
klass.defineModuleMethod( "p", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyKernelModule.p(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return RubyKernelModule.p(receiver);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.p(receiver, arg);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return RubyKernelModule.p(receiver, args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "method", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.objMethod(receiver, arg);}
});
klass.defineModuleMethod( "Integer", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.toInteger(receiver, arg);}
});
klass.defineModuleMethod( "Array", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.toArray(receiver, arg);}
});
klass.defineModuleMethod( "String", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyKernelModule.toString(receiver, arg);}
});
 
klass.defineModuleMethod( "srand", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyRandom.srand(receiver);}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyRandom.srand(receiver, arg);}
});
klass.defineModuleMethod( "rand", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyRandom.rand(receiver);}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyRandom.rand(receiver, arg);}
});

}}
