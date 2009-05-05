/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;

//@RubyLevelClass(name="Class", superclass="Module", dummy={
        //@DummyMethod(name="inherited", privateMethod=true)
        //}
//)
public class RubyClass extends RubyModule {
    private static MethodCache cache = new MethodCache();

    public static void resetCache() {
        cache.reset();
    }

    private int objectAddress;

    public RubyClass(String name, RubyClass superclass, RubyModule owner) {
        super(name, owner);
        superclass_ = superclass;
        this.objectAddress = super.hashCode();
    }

    public boolean isRealModule() {
        return false;
    }

    public boolean isRealClass() {
        return true;
    }

    public boolean isSingleton() {
        return false;
    }

    public String getName() {
        RubyClass realClass = this.getRealClass();
        return (realClass != null) ? realClass.name_ : null;
    }

    public RubyClass getRealClass() {
        RubyClass klass = this;
        while (klass != null && !klass.isRealClass()) {
            klass = klass.superclass_;
        }

        return klass;
    }
    
    //@RubyLevelMethod(name="new")
    public RubyValue newInstance(RubyBlock block) {
        RubyValue v = this.allocObject();
        callInitializeMethod(v, block);
        return v;
    }
    
    //@RubyLevelMethod(name="new")
    public RubyValue newInstance(RubyValue arg, RubyBlock block) {
        RubyValue v = this.allocObject();
        callInitializeMethod(v, arg, block);
        return v;
    }

    //@RubyLevelMethod(name="new")
    public RubyValue newInstance(RubyArray args, RubyBlock block) {
        RubyValue v = this.allocObject();
        callInitializeMethod(v, args, block);
        return v;
    }
    
    private void callInitializeMethod(RubyValue v, RubyBlock block) {
    	RubyAPI.callNoArgMethod(v, block, RubyID.initializeId);
    }
    
    private void callInitializeMethod(RubyValue v, RubyValue arg, RubyBlock block) {
    	RubyAPI.callOneArgMethod(v, arg, block, RubyID.initializeId);
    }

    private void callInitializeMethod(RubyValue v, RubyArray args, RubyBlock block) {
    	RubyAPI.callMethod(v, args, block, RubyID.initializeId);
    }
    
    //@RubyLevelMethod(name="initialize")
    public RubyValue initialize(RubyBlock block) {
    	return this.initialize(RubyRuntime.ObjectClass, block);
    }
    
    //@RubyLevelMethod(name="initialize")
    public RubyValue initialize(RubyValue arg, RubyBlock block) {
    	if (this.superclass_ != null) {
    		throw new RubyException(RubyRuntime.TypeErrorClass, "already initialized class");
    	}
    	
    	this.superclass_ = checkInheritable(arg);
    	new RubySingletonClass(this, this.superclass_.getRubyClass(), null);
    	ClassFactory.inheritedClass(this.superclass_, this);
    	this.initializeModule(block);
    	
    	return this;
    }
    
    //@RubyAllocMethod
    public static RubyClass allocClass(RubyValue receiver) {
    	return ClassFactory.createBootClass(null);
    }
    
    private RubyClass checkInheritable(RubyValue superclass) {
    	if (!(superclass instanceof RubyClass)) {
    		throw new RubyException(RubyRuntime.TypeErrorClass, "superclass must be a Class (" + superclass.getClass().getName() + " given)");
    	}
    	
    	RubyClass klass = (RubyClass)superclass;
    	if (klass.isSingleton()) {
    		throw new RubyException(RubyRuntime.TypeErrorClass, "can't make subclass of virtual class");
    	}
    	
    	return klass;
    }

    public int objectAddress() {
        return this.objectAddress;
    }

    public RubyClass getSuperClass() {
        return superclass_;
    }

    public void setSuperClass(RubyClass superclass) {
        this.superclass_ = superclass;
    }

    //@RubyLevelMethod(name="superclass")
    public RubyValue superclass() {
        RubyClass c = this.superclass_;

        if (null != c) {
            c = c.getRealClass();
        }

        if ( null == c )
        	return RubyConstant.QNIL;
        return c;
        //return (null == c) ? RubyConstant.QNIL : c;
    }

    //@RubyLevelMethod(name="allocate")
    public RubyValue allocate() {
        //TODO this implmentation does not look right...
        return new RubyObject(this);
    }

    public void defineAllocMethod(RubyMethod m) {
        m.setAccess(RubyMethod.PRIVATE);
        this.getRubyClass().addMethod(RubyID.ID_ALLOCATOR, m, RubyMethod.PRIVATE);
    }

    public void defineModuleMethod(String name, RubyMethod m) {
        throw new Error("should not reach here!");
    }

    public void setInherited(RubyClass klass) {
        RubyAPI.callOneArgMethod(this, klass, null, RubyID.inheritedID);
    }

    private RubyValue allocObject() {
        RubyValue value = RubyAPI.callNoArgMethod(this, null, RubyID.ID_ALLOCATOR);
        if (value.getRubyClass().getRealClass() != this.getRealClass()) {
            throw new RubyException(RubyRuntime.TypeErrorClass, "wrong instance allocation");
        }

        return value;
    }

    boolean isMyParent(final RubyClass superclass) {
        return superclass_ == superclass;
    }

    protected RubyMethod findSuperMethod(RubyID mid) {
        return null != this.superclass_
                ? this.superclass_.findOwnMethod(mid)
                : null;
    }

    public RubyMethod findOwnMethod(RubyID mid) {
        MethodCache.CacheEntry entry = cache.getMethod(this, mid);
        if (entry.klass == this && entry.mid == mid) {
            if (entry.method == null) {
                return null;
            } else {
                return entry.method;
            }
        }

        RubyMethod m = super.findOwnMethod(mid);
        if (null != m) {
            cache.putMethod(this, mid, m);
        }
        return m;
    }

    public RubyMethod findOwnPublicMethod(RubyID mid) {
        MethodCache.CacheEntry entry = cache.getMethod(this, mid);
        if (entry.klass == this && entry.mid == mid) {
            RubyMethod em = entry.method;
            if (em != null && RubyMethod.PUBLIC == em.getAccess()) {
                return em;
            } else {
                return null;
            }
        }

        RubyMethod m = super.findOwnPublicMethod(mid);
        if (null != m) {
            cache.putMethod(this, mid, m);
        }
        return m;
    }

    public void collectClassMethodNames(RubyArray a, int mode) {
        RubyClass klass = this;

        while (klass != null) {
            klass.collectOwnMethodNames(a, mode);
            klass = klass.superclass_;
        }
    }

    protected RubyValue addMethod(RubyID id, RubyMethod method, int attribute) {
        cache.removeMethod(id);
        return super.addMethod(id, method, attribute);
    }
    
    public static void initDummyMethods(RubyClass klass){
    	klass.definePrivateMethod("inherited", new RubyOneArgMethod(){
	    	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block)
	    	{
	    		return RubyConstant.QNIL;
	    	}
	    } );
    	
    }
    
}
