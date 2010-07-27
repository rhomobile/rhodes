/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;
import java.util.HashMap;
import java.util.Map;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.commons.GeneratorAdapter;
import org.objectweb.asm.commons.Method;

import com.xruby.runtime.lang.RubyID;

public class RubyIDClassGenerator {
    private static String RubyIDClassName;
    private static Map<String, String> idMap = new HashMap<String, String>();
    private static long counter = 0;

    private static boolean isAlphabeta(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    private static boolean isNumber(char c) {
        return c >= '0' && c <= '9';
    }

    private static boolean isValidJavaID(String s) {
        char firstChar = s.charAt(0);
        if ((firstChar == '_') || isAlphabeta(firstChar)) {
            int len = s.length();
            for (int i = 1; i < len; i++) {
                char currentChar = s.charAt(i);
                if (!((currentChar == '_') || isAlphabeta(currentChar) || isNumber(currentChar))) {
                    return false;
                }
            }

            return true;
        }

        return false;
    }

    private static String nextID() {
        return "ID" + counter++;
    }

    public static void initScript(String extra, String script_name) {
        RubyIDClassName = NameFactory.createClassNameForIDContainer(extra, script_name);
    }

    public static void clear() {
        idMap.clear();
    }

    public static void getField(GeneratorAdapter mg, String s) {
        String id = idMap.get(s);
        if (id == null) {
            if (isValidJavaID(s)) {
                id = s + "ID";
            } else {
                id = nextID();
            }

            idMap.put(s, id);
        }

        mg.getStatic(Type.getType("L" + RubyIDClassName + ";"), id, Types.RUBY_ID_TYPE);
    }

    private static byte[] visitEnd() {
        ClassWriter cw = new ClassWriter(ClassWriter.COMPUTE_MAXS);
        cw.visit(Opcodes.V1_1, Opcodes.ACC_PUBLIC, RubyIDClassName, null, "java/lang/Object", null);
        Method staticBlock = Method.getMethod("void <clinit> ()V");
        GeneratorAdapter staticBlockMg = new GeneratorAdapter(Opcodes.ACC_STATIC, staticBlock, null, null, cw);

        for (Map.Entry<String, String> e : idMap.entrySet()) {
            cw.visitField(Opcodes.ACC_PUBLIC + Opcodes.ACC_STATIC, e.getValue(),
                    Types.RUBY_ID_TYPE.getDescriptor(), null, null);

            staticBlockMg.push(e.getKey());
            staticBlockMg.invokeStatic(Type.getType(RubyID.class),
                    Method.getMethod("com.xruby.runtime.lang.RubyID intern(String)"));
            staticBlockMg.putStatic(Type.getType("L" + RubyIDClassName + ";"), e.getValue(), Types.RUBY_ID_TYPE);
        }

        staticBlockMg.returnValue();
        staticBlockMg.endMethod();
        cw.visitEnd();

        clear();

        return cw.toByteArray();
    }

    public static CompilationResult getCompilationResult() {
        return new CompilationResult(RubyIDClassName, visitEnd());
    }
}
