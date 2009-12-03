/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.commons.Method;

class ClassGeneratorForNoArgRubyMethod extends ClassGeneratorForRubyMethod {
	public ClassGeneratorForNoArgRubyMethod(String method_name, String fileName, String name, int argc, boolean has_asterisk_parameter, int default_argc, boolean is_singleton_method) {
		super(method_name, fileName, name, argc, has_asterisk_parameter, default_argc,
				is_singleton_method);
	}

	public String getSuperName() {
		return "com/xruby/runtime/lang/RubyNoArgMethod";
	}

	public String getRunMethodName() {
		return "com.xruby.runtime.lang.RubyValue run(com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyBlock)";
	}

	public Type getSuperClassType() {
		return Types.RUBY_NOARGMETHOD_TYPE;
	}

	public String getSuperCtorName() {
		return "void <init> ()";
	}

	public void pushBasciArgForSuperArg(MethodGenerator mg, int argc, boolean has_asterisk_parameter, int default_argc) {
	}

	public void loadMethodPrameter(int index) {
		throw new RuntimeException("should not load method paramenter in no argment method");
	}

	public void storeMethodParameter(int index) {
		throw new RuntimeException("should not store method paramenter in no argment method");
	}

	protected MethodGenerator createMethodGenerator() {
		return new MethodGenerator(Opcodes.ACC_PROTECTED,
				Method.getMethod(this.getRunMethodName()),
				cv_,
				null,
				null,
				false) {
			public void loadCurrentBlock() {
				this.loadArg(1);
			}

			public void loadMethodArg() {
			}
		};
	}

	public void callSuperMethod(boolean has_no_arg, boolean has_one_arg) {
		this.getMethodGenerator().RubyAPI_callSuperNoArgMethod();
	}

	public void loadArgOfMethodForBlock() {
		MethodGenerator mg = getMethodGenerator();
		mg.pushNull();
		mg.pushNull();
	}
}
