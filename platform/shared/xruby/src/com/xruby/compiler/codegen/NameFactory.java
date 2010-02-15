/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import java.io.File;
import java.util.concurrent.atomic.AtomicInteger;

public class NameFactory {

    private static AtomicInteger count_ = new AtomicInteger(0);
    private static final String DefaultName = "main";
	public static String name_classprefix_ = "xruby";

    public static String DEFAULT_RUBY_ID_CLASS_NAME = "RubyIDContainer";

    public static void reset() {
        count_.set(0);
    }

    /**
     * About the @extra parameter:
     * If you compile a dir, for example:
     * dir
     *  |- test.rb
     *  |- subdir
     *        |-test2.rb
     * If the command line is 'ruby -c dir', it will generate the following class files:
     * test/main.class
     * subdir/test2/main.class
     * ...
     * In this case createClassName("dir", "dir/subdir/test.rb") will be called
     */
    public static String createClassName(String extra, String script_name, String method_name) {
        if (null == script_name) {
            script_name = "STDIN";
        }

        if (null == method_name) {
            return name_classprefix_ + "/" + getNameWithoutPrefixAndSufix(extra, script_name) + "/" + DefaultName;
        } else {
            return name_classprefix_ + "/" + getNameWithoutPrefixAndSufix(extra, script_name) + "/" + removeInvalidIdentifierPart(method_name, false) + "$" + count_.getAndIncrement();
        }
    }

    public static String createClassnameForClassModuleBuilder(String extra, String script_name, String class_name) {
        return createClassName(extra, script_name, class_name);
    }

    public static String createClassNameForIDContainer(String extra, String script_name) {
        if (null == script_name) {
            script_name = "STDIN";
        }

        return name_classprefix_ + "/" + getNameWithoutPrefixAndSufix(extra, script_name) + "/" + DEFAULT_RUBY_ID_CLASS_NAME;
    }

    private static String removeInvalidIdentifierPart(String method_name, boolean ignore_back_slash_and_dot) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < method_name.length(); ++i) {
            char current_char = method_name.charAt(i);
            if (Character.isJavaIdentifierPart(current_char)) {
                sb.append(current_char);
            } else {
                sb.append(translateCommonChar(current_char, ignore_back_slash_and_dot));
            }
        }
        return sb.toString();
    }

    private static String translateCommonChar(char v, boolean ignore_back_slash_and_dot) {
        switch (v) {
            case '+':
                return "plus";
            case '-':
                return "minus";
            case '*':
                return "star";
            case '/':
                if (ignore_back_slash_and_dot) {
                    return "/";
                } else {
                    return "div";
                }
            case '\\':
                if (ignore_back_slash_and_dot)
                {
                    return "/";
                }
                else
                {
                    return "div";
                }
            case '.':
                if (ignore_back_slash_and_dot) {
                    return ".";
                } else {
                    return "dot";
                }
            case '=':
                return "equal";
            case '?':
                return "question";
            case '!':
                return "danger";
            default:
                return "$";
        }
    }

    public static String createClassNameForBlock(String extra, String script_name, String method_name) {
        if (null == script_name) {
            script_name = "STDIN";
        }

        return name_classprefix_ + "/" + getNameWithoutPrefixAndSufix(extra, script_name) +
            "/BLOCK" +
            ((null == method_name) ? "" : "_" + removeInvalidIdentifierPart(method_name, false)) +
            "$" +
            count_.getAndIncrement();
    }

    public static String createClassNameForSmap(String script_name) {
        return name_classprefix_ + "/" + getNameWithoutSufix(script_name) + "/" + script_name + ".smap";
    }

    public static String createMainClass(String script_name) {
        return name_classprefix_ + "." + getNameWithoutSufix(script_name) + "." + DefaultName;
    }

    public static String createClassFileName(String method_name) {
            return method_name + ".class";
    }

    public static String createJarFileName(String script_name) {
        return (null == script_name) ? "STDIN.jar" : getNameWithoutSufix(script_name) + ".jar";
    }

    public static String createMainClassName(String required_file) {
        //remove ".rb" if has one
        if (required_file.endsWith(".rb")) {
            required_file = required_file.substring(0, required_file.length() - 3);
        }

        required_file = required_file.replace('/', '.');
        required_file = required_file.replace('\\', '.');
        required_file += ".main";
        return name_classprefix_ + "." + required_file;
    }

    private static String getNameWithoutSufix(String script_name) {
        File file = new File(script_name);
        String name = file.getName();
        if (name.endsWith(".rb")) {
            int position_of_last_dot = name.lastIndexOf('.');
            if (position_of_last_dot >= 0) {
                name = name.substring(0, position_of_last_dot);
            }
        }

        return removeInvalidIdentifierPart(name, true);
    }

    private static String getNameWithoutPrefixAndSufix(String omit, String script_name) {
        if (null == omit) {
            return getNameWithoutSufix(script_name);
        }

        assert(script_name.startsWith(omit));
        String extra = script_name.substring(omit.length() + 1);
        int position_of_first_dot = extra.indexOf('.');
        if (position_of_first_dot >= 0) {
            extra = extra.substring(0, position_of_first_dot);
        }

        return removeInvalidIdentifierPart(extra, true);
    }

    /// @param name e.g. test/main.class
    public static String filename2classname(String name) {
        return convertSlashToDot(name.replace(".class", ""));
    }

    public static String convertSlashToDot(String name) {
        return name.replace('/', '.');
    }
}
