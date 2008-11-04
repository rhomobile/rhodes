package com.rho;

import com.xruby.runtime.lang.*;

public class SyncEngine extends RubyBasic{

	SyncEngine(RubyClass c) {
		super(c);
	}

    //@RubyAllocMethod
    public static SyncEngine alloc(RubyValue receiver) {
        return new SyncEngine(RubyRuntime.SyncEngineClass);
    }
	
    //@RubyLevelMethod(name="initialize")
    public SyncEngine initialize() {
        return this;
    }

    //@RubyLevelMethod(name="dosync")
    public static RubyValue dosync(RubyValue receiver) {
        return RubyConstant.QFALSE;
    }
    //@RubyLevelMethod(name="lock_sync_mutex")
    public static RubyValue lock_sync_mutex(RubyValue receiver) {
        return RubyConstant.QFALSE;
    }
    //@RubyLevelMethod(name="unlock_sync_mutex")
    public static RubyValue unlock_sync_mutex(RubyValue receiver) {
        return RubyConstant.QFALSE;
    }
    
	public static void initMethods( RubyClass klass){
		klass.defineMethod( "initialize", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver,RubyBlock block ){
				return ((SyncEngine)receiver).initialize();}
		});
		klass.defineAllocMethod(new RubyNoArgMethod(){
			protected RubyValue run(RubyValue receiver, RubyBlock block )	{
				return SyncEngine.alloc(receiver);}
		});
		klass.getSingletonClass().defineMethod( "dosync", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver,RubyBlock block ){
				return SyncEngine.dosync(receiver);}
		});
		klass.getSingletonClass().defineMethod( "lock_sync_mutex", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver,RubyBlock block ){
				return SyncEngine.lock_sync_mutex(receiver);}
		});
		klass.getSingletonClass().defineMethod( "unlock_sync_mutex", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver,RubyBlock block ){
				return SyncEngine.unlock_sync_mutex(receiver);}
		});
		
	}    
}
