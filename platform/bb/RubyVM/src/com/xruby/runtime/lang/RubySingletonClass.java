/**
 * Copyright 2005-2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

public class RubySingletonClass extends RubyClass {

    private RubyValue owner_;

    public RubySingletonClass(RubyValue owner, RubyClass superclass, RubyModule scope) {
        super(null, superclass, scope);
        owner_ = owner;
        owner.setRubyClass(this);
        this.setInstanceVariable(owner, RubyID.attachedID);

        if (owner instanceof RubySingletonClass) {
            RubySingletonClass klass = (RubySingletonClass)owner;
            this.setRubyClass(this);
            RubyClass singletonsuper = klass.getSuperClass().getRealClass().getRubyClass();
            klass.setSuperClass(singletonsuper);
        } else {
            RubyClass metasuper = superclass.getRealClass().getRubyClass();
            if (metasuper != null) {
                this.setRubyClass(metasuper);
            }
        }
    }

    public RubyValue getConstant(String name) {
        RubyValue v = getOwnConstant(name);
        if (null != v) {
            return v;
        }

        if (owner_ instanceof RubyModule) {
            v = ((RubyModule)owner_).getConstant(name);
            if (null != v) {
                return v;
            }
        }

        //TODO this will call 'getOwnConstant' again, which is a waste of time
        return super.getConstant(name);
    }

    public RubyValue getClassVariable(String name) {
        if (owner_ instanceof RubyModule) {
            return ((RubyModule)owner_).getClassVariable(name);
        } else {
            return super.getClassVariable(name);
        }
    }

    //RHO_COMMENT : setClassVariable added
    public RubyValue setClassVariable(RubyValue value, String name) {
        if (owner_ instanceof RubyModule) {
            return ((RubyModule)owner_).setClassVariable(value, name);
        } else {
            return super.setClassVariable(value, name);
        }
    }
    
    public boolean isSingleton() {
        return true;
    }

    public boolean isRealClass() {
        return false;
    }

    public RubyValue allocObject(RubyBlock block) {
        throw new RubyException(RubyRuntime.TypeErrorClass, "can't create instance of virtual class");
    }
}
