/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.commons.GeneratorAdapter;
import org.objectweb.asm.commons.Method;

public class CgUtil {
	public static Method CONSTRUCTOR = new Method("<init>", Type.VOID_TYPE, Types.NULL_TYPE_ARRAY);
	
	public static String getInternalName(String name) {
		return name.replace(".", "/");
	}
	
	public static Method getMethod(String method, Type returnType) {
		return new Method(method, returnType, Types.NULL_TYPE_ARRAY);
	}
	
	public static Method getMethod(String method, Type returnType, Type... params) {
		return new Method(method, returnType, params);
	}
	
	public static Method getMethod(String method, Class returnType, Class... params) {
		Type[] types = new Type[params.length];
		for (int i = 0; i < params.length; i++) {
			types[i] = Type.getType(params[i]);
		}
		return new Method(method, Type.getType(returnType), types);
	}
	
	public static void createImplicitConstructor(ClassVisitor cv, Type superType) {
		GeneratorAdapter mg = new GeneratorAdapter(Opcodes.ACC_PUBLIC,
                CONSTRUCTOR, null, null, cv);
		mg.visitCode();
        mg.loadThis();
        mg.invokeConstructor(superType, CONSTRUCTOR);
        mg.returnValue();
        mg.endMethod();
    }
	
	private CgUtil() {}
}
