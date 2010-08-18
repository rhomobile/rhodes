/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang.util;
/*
import java.util.ArrayList;
import java.util.List;

import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.commons.GeneratorAdapter;
import org.objectweb.asm.commons.Method;

import com.xruby.compiler.codegen.CgUtil;
import com.xruby.compiler.codegen.Types;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyValue;

public abstract class RunMethodHelper {
	protected abstract Method getRunMethod();
	protected abstract void loadBlock(GeneratorAdapter mg);
	protected abstract void loadArgs(GeneratorAdapter mg);
	protected abstract int rubyArgSize();
	
	public void createRunMethod(ClassVisitor cv, Class hostClass, Class castClass, String name, 
			boolean staticMethod, boolean block) {
		Class[] params = getParamType(staticMethod, block);
		Class returnClass;
		try {
			returnClass = hostClass.getMethod(name, params).getReturnType();
		} catch (NoSuchMethodException nsme) {
			throw new IllegalArgumentException("no such method: " + CgUtil.getMethod(name, RubyValue.class, params).getDescriptor());
		}
		
		GeneratorAdapter mg = startRun(getRunMethod(), cv);
		
		Type type = Type.getType(castClass);
		loadReceiver(mg, type, staticMethod);	
		loadArgs(mg);
		
		if (block) {	
			this.loadBlock(mg);
		}
		
		Method method = CgUtil.getMethod(name, returnClass, params);
		if (staticMethod) {
			mg.invokeStatic(type, method);
		} else {
			mg.invokeVirtual(type, method);
		}
		endRun(mg);
	}
	
	private Class[] getParamType(boolean staticMethod, boolean block) {
//		List<Class> javaArgList = new ArrayList<Class>();
 		List javaArgList = new ArrayList();
 
		if (staticMethod) {
			// receiver
			javaArgList.add(RubyValue.class);
		}
		
		int size = rubyArgSize();
		if (size >= 0) {
			for (int i = 0; i < size; i++) {
				javaArgList.add(RubyValue.class);
			}
		} else {
			javaArgList.add(RubyArray.class);
		}
		
		if (block) {
			javaArgList.add(RubyBlock.class);
		}
		
		return javaArgList.toArray(new Class[0]);
	}
	
	protected GeneratorAdapter startRun(Method runMethod, ClassVisitor cv) {
		GeneratorAdapter mg = new GeneratorAdapter(Opcodes.ACC_PROTECTED,
	            runMethod, null, null, cv);
		mg.visitCode();		
		return mg;
	}

	protected void loadReceiver(GeneratorAdapter mg, Type type, boolean module) {
		mg.loadArg(0);
		
		if (!module && !Types.RUBY_VALUE_TYPE.equals(type)) {
			mg.checkCast(type);
		}
	}

	protected void endRun(GeneratorAdapter mg) {
		mg.returnValue();
		mg.endMethod();	
	}
}*/
