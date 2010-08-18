/**
 * Copyright 2006-2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

class ClassFactory {
    public static RubyClass defineClass(String name, RubyClass superclass) {
        if (superclass == null) {
            superclass = RubyRuntime.ObjectClass;
        }

        RubyValue value = RubyRuntime.ObjectClass.getConstant(name);
        if (value != null) {
            if (!(value instanceof RubyClass)) {
                throw new RubyException(RubyRuntime.TypeErrorClass, name + " is not a class");
            }

            RubyClass klass = (RubyClass)value;

            if (klass.getSuperClass().getRealClass() != superclass) {
                throw new RubyException(RubyRuntime.NameErrorClass, name + " is already defined");
            }

            // FIXME: remove this method
            klass.setAccessPublic();

            ObjectSpace.add(klass);
            return klass;
        }

        RubyClass klass = makeRubyClass(superclass);

        klass.setName(name);
        RubyRuntime.ObjectClass.setConstant(name, klass);
        inheritedClass(superclass,klass);

        ObjectSpace.add(klass);
        return klass;
    }

    public static RubyModule defineModule(String name) {
        RubyValue value = RubyRuntime.ObjectClass.getConstant(name);
        if (value != null) {
            if (value instanceof RubyModule) {
                RubyModule module = ((RubyModule)value);
                if (module.isRealModule()) {
                    return module;
                }
            }

            throw new RubyException(RubyRuntime.TypeErrorClass, name + " is not a module");
        }

        RubyModule module = createRubyModule(name);
        RubyRuntime.ObjectClass.setConstant(name, module);

        ObjectSpace.add(module);
        return module;
    }

    /**
     * Define a boot class.
     *
     * name, the name of the boot class to be defined.
     * superclass, the super class of the boot class to be defined.
     */
    static RubyClass defineBootClass(String name, RubyClass superclass) {
        RubyClass klass = createBootClass(superclass);
        klass.setName(name);
        RubyClass constObj = (RubyRuntime.ObjectClass != null) ? RubyRuntime.ObjectClass : klass;
        constObj.setConstant(name, klass);
        return klass;
    }

    private static RubyClass createRubyClass(RubyClass superclass) {
        if (superclass == RubyRuntime.ClassClass) {
            throw new RubyException(RubyRuntime.TypeErrorClass, "can't make subclass of Class");
        }

        if (!superclass.isRealClass()) {
            throw new RubyException(RubyRuntime.TypeErrorClass, "can't make subclass of virtual class");
        }

        return createBootClass(superclass);
    }

    static RubyClass makeRubyClass(RubyClass superclass) {
        if (superclass == null) {
            superclass = RubyRuntime.ObjectClass;
        }

        RubyClass klass = createRubyClass(superclass);
        new RubySingletonClass(klass, superclass.getRubyClass(), null);
        return klass;
    }

    public static RubyClass createBootClass(RubyClass superclass) {
        RubyClass klass = new RubyClass(null, superclass, null);
        klass.setRubyClass(RubyRuntime.ClassClass);

        return klass;
    }

    private static RubyModule createRubyModule(String name) {
        RubyModule module = new RubyModule(name, null);
        module.setName(name);
        module.setRubyClass(RubyRuntime.ModuleClass);
        return module;
    }

    static void inheritedClass(RubyClass superclass, RubyClass klass){
        if(RubyRuntime.running) {
            if (superclass == null) {
                superclass = RubyRuntime.ObjectClass;
            }
            superclass.setInherited(klass);
        }
    }

    private ClassFactory() {
    }
}
