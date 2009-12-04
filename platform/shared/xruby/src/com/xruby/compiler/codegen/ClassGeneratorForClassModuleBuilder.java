/**
 * Copyright 2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import com.xruby.runtime.lang.RubyBinding;

//Right now we just treat class/module definition as RubyProgram
class ClassGeneratorForClassModuleBuilder extends ClassGeneratorForRubyProgram {

    public ClassGeneratorForClassModuleBuilder(String name, String fileName, RubyBinding binding, boolean is_singleton) {
        super(name, fileName, binding, false, is_singleton);
    }

}
