package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
public class RubyFile_Methods{
public static void initMethods( RubyClass klass){

klass.getSingletonClass().defineMethod( "exist?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyFile.exist_question(receiver, arg);}
});
klass.getSingletonClass().aliasMethod("exists?","exist?");

klass.getSingletonClass().defineMethod( "exist_injar?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyFile.existinjar_question(receiver, arg);}
});

klass.getSingletonClass().defineMethod( "dirname", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyFile.dirname(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "executable?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyFile.executable_question(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "rename", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return RubyFile.rename(receiver, arg0, arg1);}
});
klass.getSingletonClass().defineMethod( "writable?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyFile.writable_question(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "expand_path", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyFile.expand_path(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return RubyFile.expand_path(receiver);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(1, 0, block != null? block.createdByLambda() : false );
		args.add(arg);
		return RubyFile.expand_path(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return RubyFile.expand_path(receiver, args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.getSingletonClass().defineMethod( "mtime", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyFile.mtime(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "size", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyFile.size(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "basename", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyFile.basename(receiver, args);}
});
klass.getSingletonClass().defineMethod( "utime", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyFile.utime(receiver, args);}
});
klass.defineMethod( "truncate", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFile)receiver).truncate(arg);}
});
klass.getSingletonClass().defineMethod( "open", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyFile.open(receiver, args, block);}
});
klass.getSingletonClass().defineMethod( "separator", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyFile.separator(receiver);}
});
klass.getSingletonClass().defineMethod( "file?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyFile.file_question(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "readable?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyFile.readable_question(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "directory?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyFile.directory_question(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "delete", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyFile.delete(receiver, args);}
});
klass.getSingletonClass().aliasMethod("unlink","delete");

klass.getSingletonClass().defineMethod( "extname", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyFile.extname(receiver, arg);}
});
klass.defineMethod( "initialize", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyFile)receiver).initialize(args, block);}
});
klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyFile.alloc(receiver);	}
} );

}}
