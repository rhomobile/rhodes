package com.xruby.compiler.codegen;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.commons.Method;

public class ClassGeneratorForTwoArgRubyMethod extends ClassGeneratorForRubyMethod {
	public ClassGeneratorForTwoArgRubyMethod(String method_name, String fileName, String name, int argc, boolean has_asterisk_parameter, int default_argc, boolean is_singleton_method) {
		super(method_name, fileName, name, argc, has_asterisk_parameter, default_argc,
				is_singleton_method);
	}
	
	public String getSuperName() {
		return "com/xruby/runtime/lang/RubyTwoArgMethod";
	}
	
	public String getRunMethodName() {
		return "com.xruby.runtime.lang.RubyValue run(com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyBlock)";
	}
	
	public Type getSuperClassType() {
		return Types.RUBY_TWOARGMETHOD_TYPE;
	}
	
	public String getSuperCtorName() {
		return "void <init> ()";
	}
	
	public void pushBasciArgForSuperArg(MethodGenerator mg, int argc, boolean has_asterisk_parameter, int default_argc) {
	}
	
	public void loadMethodPrameter(int index) {
		assert(0 == index || 1 == index);
        getMethodGenerator().loadArg(index + 1);
	}
	
	public void storeMethodParameter(int index) {
		assert(0 == index || 1 == index);
		getMethodGenerator().storeArg(index + 1);
	}
	
	protected MethodGenerator createMethodGenerator() {
		return new MethodGenerator(Opcodes.ACC_PROTECTED,
				Method.getMethod(this.getRunMethodName()),
				cv_,
				null,
				null,
				false) {
			public void loadCurrentBlock() {
				this.loadArg(3);
			}

			public void loadMethodArg() {
				MethodGenerator mg = getMethodGenerator();
				mg.loadArg(1);
				mg.loadArg(2);
			}
		};
	}
	
	public void callSuperMethod(boolean has_no_arg, boolean has_one_arg) {
		this.getMethodGenerator().RubyAPI_callSuperTwoArgMethod();
	}

	public void loadArgOfMethodForBlock() {
		MethodGenerator mg = getMethodGenerator();
		mg.pushNull();
		mg.newInstance(Types.RUBY_ARRAY_TYPE);
		mg.dup();
		mg.loadArg(1);
		mg.loadArg(2);
		mg.invokeConstructor(Types.RUBY_ARRAY_TYPE, 
				CgUtil.getMethod("<init>", Type.VOID_TYPE, Types.RUBY_VALUE_TYPE, Types.RUBY_VALUE_TYPE));
	}

}
