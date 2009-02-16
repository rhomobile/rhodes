/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang.util;
/*
import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.util.CheckClassAdapter;

import com.xruby.compiler.XRubyClassLoader;
import com.xruby.compiler.codegen.CgConfig;
import com.xruby.compiler.codegen.CgUtil;
import com.xruby.compiler.codegen.ClassDumper;
import com.xruby.runtime.lang.RubyMethod;

public class MethodFactory {
	private static final XRubyClassLoader cl = new XRubyClassLoader();
	private static final ClassDumper dumper = new ClassDumper();
	
	private Class hostClass;
	private Class castClass;
	private boolean module;
	
	public static MethodFactory createMethodFactory(Class klass) {
		return new MethodFactory(klass, false);
	}
	
	public static MethodFactory createMethodFactory(Class klass, boolean module) {
		return new MethodFactory(klass, module);
	}
	
	public static MethodFactory createModuleMethodFactory(Class klass) {
		return new MethodFactory(klass, true);
	}
	
	private MethodFactory(Class klass, boolean module) {
		this(klass, klass, module);
	}
	
	private MethodFactory(Class hostClass, Class castClass, boolean module) {
		this.hostClass = hostClass;
		this.castClass = castClass;
		this.module = module;
	}
	
	public RubyMethod getMethod(String name, MethodType type, boolean singleton, boolean block) {
		return loadMethod(name, type, singleton, block);
	}
	
	public RubyMethod getMethod(String name, MethodType type) {
		return loadMethod(name, type, false, false);
	}
	
	public RubyMethod getMethodWithBlock(String name, MethodType type) {
		return loadMethod(name, type, false, true);
	}
	
	public RubyMethod getSingletonMethod(String name, MethodType type) {
		return loadMethod(name, type, true, false);
	}
	
	public RubyMethod getSingletonMethodWithBlock(String name, MethodType type) {
		return loadMethod(name, type, true, true);
	} 

	private RubyMethod loadMethod(String name, MethodType type, boolean singleton, boolean block) {
		String invokerName = getInvokerName(name, block);
		Class klass = tryClass(invokerName);
		if (klass == null) {
			klass = createMethodClass(invokerName, name, type, singleton, block);
		}
		
		try {	
			return (RubyMethod)klass.newInstance();
		} catch (InstantiationException ie) {
			throw new RuntimeException("fail to create Ruby method", ie);
		} catch (IllegalAccessException iae) {
			throw new RuntimeException("fail to create Ruby method", iae);
		}
	}
	
	private Class tryClass(String name) {
		try {
			return cl.load(name);
		} catch (ClassNotFoundException e) {
			return null;
		}
	}

	private Class createMethodClass(String invokerName, String name, MethodType type, 
			boolean singleton, boolean block) {
		MethodFactoryHelper helper = MethodFactoryHelper.getHelper(type);		
		ClassWriter cw = new ClassWriter(ClassWriter.COMPUTE_MAXS);
		ClassVisitor cv = new CheckClassAdapter(cw);
		
		startInvoker(cv, helper, invokerName);
		helper.createRunMethod(cv, this.hostClass, this.castClass, name, (singleton | this.module), block);
		endInvoker(cv);
		
		return loadClass(invokerName, cw);
	}

	private String getInvokerName(String name, boolean block) {
		StringBuilder sb = new StringBuilder();
		sb.append(hostClass.getName());
		sb.append("$");
		sb.append(name);
		if (block) {
			sb.append("WithBlock");
		}
		sb.append("$Invoker");
		return CgUtil.getInternalName(sb.toString());
	}

	private void startInvoker(ClassVisitor cv, MethodFactoryHelper helper, String invokerName) {
		Type superType = helper.getSuperType();
		cv.visit(CgConfig.TARGET_VERSION, Opcodes.ACC_PUBLIC, invokerName, 
				null, superType.getInternalName(), null);		
		CgUtil.createImplicitConstructor(cv, superType);
	}
	
	private void endInvoker(ClassVisitor cv) {
		cv.visitEnd();
	}
	
	private Class loadClass(String invokerName, ClassWriter cw) {
		dumper.dump(invokerName, cw.toByteArray());
		return cl.load(invokerName, cw.toByteArray());
	}
}
*/