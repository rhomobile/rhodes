/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang.util;
/*
import org.objectweb.asm.Type;
import org.objectweb.asm.commons.GeneratorAdapter;

import com.xruby.compiler.codegen.CgUtil;

public class MethodType {
	public static MethodType UNKNOWN = new FixedMethodType(-1, "UNKNOWN");
	public static MethodType NO_ARG = new FixedMethodType(1, "NO_ARG");
	public static MethodType ONE_ARG = new FixedMethodType(2, "ONE_ARG");
	public static MethodType NO_OR_ONE_ARG = new FixedMethodType(3, "NO_OR_ONE_ARG");
	public static MethodType TWO_ARG = new FixedMethodType(4, "TWO_ARG");
	public static MethodType ONE_OR_TWO_ARG = new FixedMethodType(6, "ONE_OR_TWO_ARG");
	public static MethodType VAR_ARG = new FixedMethodType(8, "VAR_ARG");
	
	private int value;
	
	private MethodType(int value) {
		this.value = value;
	}
	
	public int value() {
		return this.value;
	}
	
	private static final Type methodTypeType = Type.getType(MethodType.class);

	public void generateMethodType(GeneratorAdapter mg) {
		mg.push(this.value);
		mg.invokeStatic(methodTypeType,
				CgUtil.getMethod("valueOf", methodTypeType, Type.INT_TYPE));
	}
	
	static boolean isNoArg(MethodType type) {
		return (type.value & NO_ARG.value) != 0;
	}
	
	static boolean isOneArg(MethodType type) {
		return (type.value & ONE_ARG.value) != 0;
	}
	
	static boolean isTwoArg(MethodType type) {
		return (type.value & TWO_ARG.value) != 0;
	}
	
	public static MethodType valueOf(int value) {
		switch(value) {
		case -1:
			return UNKNOWN;
		case 1:
			return NO_ARG;
		case 2:
			return ONE_ARG;
		case 3:
			return NO_OR_ONE_ARG;
		case 4:
			return TWO_ARG;
		case 6:
			return ONE_OR_TWO_ARG;
		case 8:
			return VAR_ARG;
		default:
			return new MethodType(value);
		}
	}
	
	static class FixedMethodType extends MethodType {
		private String str;
		
		private FixedMethodType(int value, String text) {
			super(value);
			this.str = text;
		}

		public void generateMethodType(GeneratorAdapter mg) {
			mg.getStatic(methodTypeType, this.str, methodTypeType);
		}
	}
}
*/