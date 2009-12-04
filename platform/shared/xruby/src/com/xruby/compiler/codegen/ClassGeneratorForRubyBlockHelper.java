/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import org.objectweb.asm.Type;

abstract class ClassGeneratorForRubyBlockHelper {
    public abstract String getSuperName();
    public abstract String getRunMethodName();
    public abstract Type getSuperClassType();
    public abstract String getSuperCtorName();
    public abstract void pushBasciArgForSuperArg(MethodGenerator mg, int argc, boolean has_asterisk_parameter, int default_argc);

    public static ClassGeneratorForRubyBlockHelper getHelper(int argc,
            boolean has_asterisk_parameter,
            boolean is_for_in_expression,
            boolean has_extra_comma) {
        if (!has_asterisk_parameter && !is_for_in_expression && !has_extra_comma) {
            switch (argc) {
            case 0:
                return NO_ARG_HELPER;
            case 1:
                return ONE_ARG_HELPER;
            case -1:
                return VAR_ARG_HELPER;
            }
        }

        return DEFAULT_HELPER;
    }

    private static ClassGeneratorForRubyBlockHelper DEFAULT_HELPER = new DefaultClassGeneratorForRubyBlockHelper();
    private static ClassGeneratorForRubyBlockHelper NO_ARG_HELPER = new NoArgClassGeneratorForRubyBlockHelper();
    private static ClassGeneratorForRubyBlockHelper ONE_ARG_HELPER = new OneArgClassGeneratorForRubyBlockHelper();
    private static ClassGeneratorForRubyBlockHelper VAR_ARG_HELPER = new VarArgClassGeneratorForRubyBlockHelper();

    private static class DefaultClassGeneratorForRubyBlockHelper extends ClassGeneratorForRubyBlockHelper {
        public String getSuperName() {
            return "com/xruby/runtime/lang/RubyBlock";
        }

        public String getRunMethodName() {
            return "com.xruby.runtime.lang.RubyValue run(com.xruby.runtime.lang.RubyValue, com.xruby.runtime.builtin.RubyArray)";
        }

        public Type getSuperClassType() {
            return Types.RUBY_BLOCK_TYPE;
        }

        public String getSuperCtorName() {
            return "void <init> (int, boolean, int, com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyValue, com.xruby.runtime.builtin.RubyArray, com.xruby.runtime.lang.RubyBlock, com.xruby.runtime.lang.RubyModule, com.xruby.runtime.lang.RubyMethod, boolean)";
        }

        public void pushBasciArgForSuperArg(MethodGenerator mg, int argc, boolean has_asterisk_parameter, int default_argc) {
            mg.push(argc);
            mg.push(has_asterisk_parameter);
            mg.push(default_argc);
        }
    }

    private static class NoArgClassGeneratorForRubyBlockHelper extends ClassGeneratorForRubyBlockHelper {
        public String getSuperName() {
            return "com/xruby/runtime/lang/RubyNoArgBlock";
        }

        public String getRunMethodName() {
            return "com.xruby.runtime.lang.RubyValue run(com.xruby.runtime.lang.RubyValue)";
        }

        public Type getSuperClassType() {
            return Types.RUBY_NOARGBLOCK_TYPE;
        }

        public String getSuperCtorName() {
            return "void <init> (com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyValue, com.xruby.runtime.builtin.RubyArray, com.xruby.runtime.lang.RubyBlock, com.xruby.runtime.lang.RubyModule, com.xruby.runtime.lang.RubyMethod, boolean)";
        }

        public void pushBasciArgForSuperArg(MethodGenerator mg, int argc, boolean has_asterisk_parameter, int default_argc) {
        }
    }

    private static class OneArgClassGeneratorForRubyBlockHelper extends ClassGeneratorForRubyBlockHelper {
        public String getSuperName() {
            return "com/xruby/runtime/lang/RubyOneArgBlock";
        }

        public String getRunMethodName() {
            return "com.xruby.runtime.lang.RubyValue run(com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyValue)";
        }

        public Type getSuperClassType() {
            return Types.RUBY_ONEARGBLOCK_TYPE;
        }

        public String getSuperCtorName() {
            return "void <init> (com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyValue, com.xruby.runtime.builtin.RubyArray, com.xruby.runtime.lang.RubyBlock, com.xruby.runtime.lang.RubyModule, com.xruby.runtime.lang.RubyMethod, boolean)";
        }

        public void pushBasciArgForSuperArg(MethodGenerator mg, int argc, boolean has_asterisk_parameter, int default_argc) {
        }
    }

    private static class VarArgClassGeneratorForRubyBlockHelper extends ClassGeneratorForRubyBlockHelper {
        public String getSuperName() {
            return "com/xruby/runtime/lang/RubyVarArgBlock";
        }

        public String getRunMethodName() {
            return "com.xruby.runtime.lang.RubyValue run(com.xruby.runtime.lang.RubyValue)";
        }

        public Type getSuperClassType() {
            return Types.RUBY_VARARGBLOCK_TYPE;
        }

        public String getSuperCtorName() {
            return "void <init> (com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyValue, com.xruby.runtime.builtin.RubyArray, com.xruby.runtime.lang.RubyBlock, com.xruby.runtime.lang.RubyModule, com.xruby.runtime.lang.RubyMethod, boolean)";
        }

        public void pushBasciArgForSuperArg(MethodGenerator mg, int argc, boolean has_asterisk_parameter, int default_argc) {
        }
    }
}
