package com.rho.sync;

import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyNoArgMethod;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;

public class SyncEngine extends RubyBasic{
	
	private static SyncThread sThread = null;

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
    
    //@RubyLevelMethod(name="start")
    public static RubyValue start(RubyValue receiver) {
    	if(sThread == null) {
    		sThread = new SyncThread();
    	}
    	return RubyConstant.QFALSE;
    }
    
    //@RubyLevelMethod(name="stop")
    public static RubyValue stop(RubyValue receiver) {
    	return RubyConstant.QFALSE;
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
		klass.getSingletonClass().defineMethod( "start", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver,RubyBlock block ){
				return SyncEngine.start(receiver);}
		});
		klass.getSingletonClass().defineMethod( "stop", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver,RubyBlock block ){
				return SyncEngine.stop(receiver);}
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
