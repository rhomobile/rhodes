package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.builtin.RubyIO;
public class RubyIO_Methods{
public static void initMethods( RubyClass klass){

klass.getSingletonClass().defineMethod( "read", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyIO.read(receiver, args, block);}
});
klass.getSingletonClass().defineMethod( "binread", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyIO.binread(receiver, args, block);}
});

klass.defineMethod( "gets", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyIO)receiver).gets(args);}
});
klass.defineMethod( "readline", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyIO)receiver).readline(args);}
});

klass.defineMethod( "print", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyIO)receiver).print(args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return ((RubyIO)receiver).print();}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyIO)receiver).print(arg);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return ((RubyIO)receiver).print(args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
/*klass.getSingletonClass().defineMethod( "pipe", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyIO.pipeSingleton(receiver, block);}
});*/
klass.defineMethod( "close", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyIO)receiver).close();}
});
klass.defineMethod( "seek", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyIO)receiver).seek(arg);}
});
klass.defineMethod( "flush", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyIO)receiver).flush();}
});
klass.defineMethod( "puts", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyIO)receiver).puts(args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return ((RubyIO)receiver).puts();}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(1, 0, block != null? block.createdByLambda() : false );
		args.add(arg);
		return ((RubyIO)receiver).puts(args);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return ((RubyIO)receiver).puts(args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "closed?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyIO)receiver).closedP(arg);}
});
klass.defineMethod( "readpartial", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyIO)receiver).readpartial(args);}
});
klass.defineMethod( "write", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyIO)receiver).write(arg);}
});
klass.defineMethod( "read", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyIO)receiver).read(args);}
});
klass.defineMethod( "eof", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyIO)receiver).eof();}
});
klass.aliasMethod("eof?","eof");
}}
