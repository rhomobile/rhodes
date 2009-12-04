/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import org.objectweb.asm.Type;

class ClassGeneratorForOneArgRubyMethod extends ClassGeneratorForRubyMethod {
    public ClassGeneratorForOneArgRubyMethod(String method_name, String fileName, String name, int argc, boolean has_asterisk_parameter, int default_argc, boolean is_singleton_method) {
        super(method_name, fileName, name, argc, has_asterisk_parameter, default_argc,
                is_singleton_method);
    }

    public String getSuperName() {
        return "com/xruby/runtime/lang/RubyOneArgMethod";
    }

    public String getRunMethodName() {
        return "com.xruby.runtime.lang.RubyValue run(com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyBlock)";
    }

    public Type getSuperClassType() {
        return Types.RUBY_ONEARGMETHOD_TYPE;
    }

    public String getSuperCtorName() {
        return "void <init> ()";
    }

    public void pushBasciArgForSuperArg(MethodGenerator mg, int argc, boolean has_asterisk_parameter, int default_argc) {
    }

    public void loadMethodPrameter(int index) {
        assert(0 == index);
        getMethodGenerator().loadArg(1);
    }

    public void storeMethodParameter(int index) {
        assert(0 == index);
        getMethodGenerator().storeArg(1);
    }

    public void callSuperMethod(boolean has_no_arg, boolean has_one_arg) {
        this.getMethodGenerator().RubyAPI_callSuperOneArgMethod();
    }

	public void loadArgOfMethodForBlock() {
		MethodGenerator mg = getMethodGenerator();
		mg.loadArg(1);
		mg.pushNull();
	}
}
