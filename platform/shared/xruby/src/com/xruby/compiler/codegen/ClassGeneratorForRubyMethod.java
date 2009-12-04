/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.commons.Method;

abstract class ClassGeneratorForRubyMethod extends ClassGenerator {
    public abstract String getSuperName();
    public abstract String getRunMethodName();
    public abstract Type getSuperClassType();
    public abstract String getSuperCtorName();
    public abstract void pushBasciArgForSuperArg(MethodGenerator mg, int argc,
            boolean has_asterisk_parameter, int default_argc);
    public abstract void callSuperMethod(boolean has_no_arg, boolean has_one_arg);

    private final String method_name_;//this name is saved for 'super'
    private final boolean is_singleton_method_;

    public ClassGeneratorForRubyMethod(String method_name, String fileName, String name, int argc, boolean has_asterisk_parameter, int default_argc, boolean is_singleton_method) {
        super(name);
        method_name_ = method_name;
        is_singleton_method_ = is_singleton_method;
        mg_for_run_method_ = visitRubyMethod(argc, has_asterisk_parameter, default_argc);

        // set source file's name, for debug
        if (null != fileName) {
            cv_.visitSource(fileName, null);
        }
    }

    String getOrginalMethodName() {
        return method_name_;
    }

    boolean isSingletonMethod() {
        return is_singleton_method_;
    }

    private MethodGenerator visitRubyMethod(int argc, boolean has_asterisk_parameter, int default_argc) {
        cv_.visit(CgConfig.TARGET_VERSION,
                0,		//No modifier
                name_,
                null,								// signature
                this.getSuperName(),	// superName
                null								// interface
                );

        createCtorForRubyMethod(argc, has_asterisk_parameter, default_argc);

        return createMethodGenerator();
    }

    protected MethodGenerator createMethodGenerator() {
        return new MethodGenerator(Opcodes.ACC_PROTECTED,
                Method.getMethod(this.getRunMethodName()),
                cv_,
                null,
                null,
                false);
    }

    private void createCtorForRubyMethod(int argc, boolean has_asterisk_parameter, int default_argc) {
        MethodGenerator mg = new MethodGenerator(Opcodes.ACC_PUBLIC,
        		CgUtil.CONSTRUCTOR,
                cv_,
                null,
                null,
                false);
        mg.loadThis();

        this.pushBasciArgForSuperArg(mg, argc, has_asterisk_parameter, default_argc);

        mg.invokeConstructor(getSuperClassType(),
                        Method.getMethod(this.getSuperCtorName()));
        mg.returnValue();
        mg.endMethod();
    }
}

