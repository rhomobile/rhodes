package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.*;
import com.rho.Mutex;

public class RubyMutex extends RubyBasic {

	Mutex m_nativeMutex;
	
	public RubyMutex(RubyClass c) {
		super(c);
		// TODO Auto-generated constructor stub
	}
    public RubyMutex() {
        super(RubyRuntime.MutexClass);
    }

    //@RubyAllocMethod
    public static RubyValue alloc(RubyValue receiver) {
    	return new RubyMutex();
    }
	
    //@RubyLevelMethod(name="initialize")
    public RubyValue initialize() {
    	m_nativeMutex = new Mutex();
    	return this;
    }
    
	public static void initMethods( RubyClass klass){
		klass.defineMethod( "initialize", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return ((RubyMutex)receiver).initialize();}
		});
		klass.defineAllocMethod(new RubyNoArgMethod(){
			protected RubyValue run(RubyValue receiver, RubyBlock block )	{
				return RubyMutex.alloc(receiver);	}
		});
		klass.defineMethod( "locked?", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				boolean bRes = ((RubyMutex)receiver).m_nativeMutex.isLocked();
				return ObjectFactory.createBoolean(bRes);
			}
		});
		klass.defineMethod( "try_lock", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				boolean bRes = ((RubyMutex)receiver).m_nativeMutex.tryLock();
				return ObjectFactory.createBoolean(bRes);
			}
		});
		klass.defineMethod( "lock", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				((RubyMutex)receiver).m_nativeMutex.Lock();
				return receiver; 
			}
		});
		klass.defineMethod( "unlock", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				((RubyMutex)receiver).m_nativeMutex.Unlock();
				return receiver; 
			}
		});
		klass.defineMethod( "sleep", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block ){
				int nSec = arg0 == RubyConstant.QNIL ? -1 : arg0.toInt();
				int nSleep = ((RubyMutex)receiver).m_nativeMutex.Sleep(nSec);
				return ObjectFactory.createInteger(nSleep);
			}
		});
	}
}
