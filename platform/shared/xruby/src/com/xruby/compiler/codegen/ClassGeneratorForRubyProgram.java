/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import org.objectweb.asm.*;
import org.objectweb.asm.commons.Method;
import com.xruby.runtime.lang.RubyBinding;

class ClassGeneratorForRubyProgram extends ClassGenerator {
    private final RubyBinding binding;
    private final String fileName;

    public ClassGeneratorForRubyProgram(String name, String fileName, RubyBinding binding, boolean create_main, boolean is_singleton) {
        super(name);
        this.fileName = fileName;
        this.binding = binding;
        mg_for_run_method_ = startRubyProgram(create_main, is_singleton);
    }

    public String getFileName() {
        return fileName;
    }
    
    private static final Method RUBY_PROGRAM_RUN = 
    	CgUtil.getMethod("run", Types.RUBY_VALUE_TYPE, Types.RUBY_VALUE_TYPE, Types.RUBY_ARRAY_TYPE, Types.RUBY_BLOCK_TYPE, Types.RUBY_MODULE_TYPE);

    private MethodGenerator startRubyProgram(boolean createMain, boolean is_singleton) {
        cv_.visit(CgConfig.TARGET_VERSION,
                Opcodes.ACC_PUBLIC,
                name_,
                null,										// signature
                Types.RUBY_PROGRAM_TYPE.getInternalName(),  // superName
                null										// interface
                );

        // set source file's name, for debug
        if (this.fileName != null) {
            cv_.visitSource(this.fileName, null);
        }

        CgUtil.createImplicitConstructor(this.cv_, Types.RUBY_PROGRAM_TYPE);

        if (createMain) {
            createStaticVoidMain(cv_);
        }
        
		//Implement RubyProgram
        return new MethodGenerator(Opcodes.ACC_PROTECTED,
        		RUBY_PROGRAM_RUN,
                cv_,
                this.binding,
                null,
                is_singleton);
    }

    private void createStaticVoidMain(ClassVisitor cv) {
        MethodGenerator mg = new MethodGenerator(Opcodes.ACC_PUBLIC + Opcodes.ACC_STATIC,
                Method.getMethod("void main (String[])"),
                cv,
                null,
                null,
                false);

        mg.loadArg(0);
        mg.invokeStatic(Types.RUBY_RUNTIME_TYPE, Method.getMethod("void init(String[])"));

        Type program = Type.getType("L" + name_ + ";");
        mg.newInstance(program);
        mg.dup();
        mg.invokeConstructor(program, CgUtil.CONSTRUCTOR);
        mg.invokeVirtual(program, CgUtil.getMethod("invoke", Types.RUBY_VALUE_TYPE));
        mg.pop();

        mg.invokeStatic(Types.RUBY_RUNTIME_TYPE, CgUtil.getMethod("fini", Type.VOID_TYPE));

        mg.returnValue();
        mg.endMethod();
    }

    public void storeVariable(String name) {
        updateBinding(binding, name);

        super.storeVariable(name);
    }

	public void loadArgOfMethodForBlock() {
		MethodGenerator mg = getMethodGenerator();
		mg.pushNull();
		mg.loadArg(1);
	}
}

