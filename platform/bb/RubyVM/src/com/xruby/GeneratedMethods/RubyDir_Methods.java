package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyDir;
import com.xruby.runtime.builtin.RubyFile;
public class RubyDir_Methods{
public static void initMethods( RubyClass klass){

klass.getSingletonClass().defineMethod( "rmdir", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyDir.rmdir(receiver, arg);}
});
klass.getSingletonClass().aliasMethod("delete","rmdir");
klass.getSingletonClass().aliasMethod("unlink","rmdir");
klass.getSingletonClass().defineMethod( "entries", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyDir.entries(receiver, arg, block);}
});
klass.getSingletonClass().defineMethod( "foreach", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyDir.foreach(receiver, arg, block);}
});
klass.defineMethod( "pos=", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyDir)receiver).setPos(arg);}
});
klass.defineMethod( "rewind", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyDir)receiver).rewind();}
});
klass.getSingletonClass().defineMethod( "[]", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyDir.array_access(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "glob", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyDir.glob(receiver, arg, block);}
});
klass.defineMethod( "close", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyDir)receiver).close();}
});
klass.defineMethod( "pos", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyDir)receiver).pos();}
});
klass.aliasMethod("tell","pos");
klass.defineMethod( "seek", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyDir)receiver).seek(arg);}
});
klass.getSingletonClass().defineMethod( "open", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyDir.open(receiver, arg, block);}
});
klass.getSingletonClass().defineMethod( "mkdir", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyDir.mkdir(receiver, arg);}
});
klass.defineMethod( "path", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyDir)receiver).getPath();}
});
klass.getSingletonClass().defineMethod( "chdir", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyDir.chdir(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "new", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyDir.dirNew(receiver, arg);}
});
klass.defineMethod( "read", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyDir)receiver).read();}
});
klass.getSingletonClass().defineMethod( "getwd", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyDir.getwd(receiver);}
});
klass.getSingletonClass().aliasMethod("pwd","getwd");
klass.defineMethod( "each", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyDir)receiver).each(block);}
});

klass.getSingletonClass().defineMethod( "exist?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyDir.exist_question(receiver, arg);}
});
klass.getSingletonClass().aliasMethod("exists?","exist?");

}}
