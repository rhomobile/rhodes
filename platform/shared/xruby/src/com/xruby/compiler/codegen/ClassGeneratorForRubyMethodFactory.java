/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

class ClassGeneratorForRubyMethodFactory {
    public static ClassGeneratorForRubyMethod createClassGeneratorForRubyMethod(
            String method_name, String fileName,
            String name, int argc,
            boolean has_asterisk_parameter, int default_argc,
            boolean is_singleton_method) {
        if (!has_asterisk_parameter && default_argc == 0) {
            switch (argc) {
            case 0:
                return new ClassGeneratorForNoArgRubyMethod(method_name, fileName,
                        name, argc, has_asterisk_parameter, default_argc, is_singleton_method);
            case 1:
                return new ClassGeneratorForOneArgRubyMethod(method_name, fileName,
                        name, argc, has_asterisk_parameter, default_argc, is_singleton_method);
            case 2:
                return new ClassGeneratorForTwoArgRubyMethod(method_name, fileName,
                        name, argc, has_asterisk_parameter, default_argc, is_singleton_method);
            }
        }
        return new ClassGeneratorForVarArgRubyMethod(method_name, fileName,
                name, argc, has_asterisk_parameter, default_argc, is_singleton_method);
    }
}