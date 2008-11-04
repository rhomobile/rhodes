/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang.util;
/*
import java.lang.annotation.Annotation;

import org.objectweb.asm.Type;
import org.objectweb.asm.commons.GeneratorAdapter;
import org.objectweb.asm.commons.Method;

import com.xruby.compiler.codegen.CgUtil;
import com.xruby.compiler.codegen.Types;
import com.xruby.runtime.lang.annotation.RubyLevelClass;
import com.xruby.runtime.lang.annotation.UndefMethod;

class RubyClassFactory extends RubyTypeFactory {
	private static final Method RubyClassBuilderCreateRubyClassMethod = 
		CgUtil.getMethod("createRubyClass", Types.RUBY_CLASS_TYPE);
	private static final Method RubyAPIDefineClassMethod = 
		CgUtil.getMethod("defineClass", Types.RUBY_CLASS_TYPE, Type.getType(String.class), Types.RUBY_VALUE_TYPE);
	private static final Type BuilderInterface = Type.getType(RubyClassBuilder.class);
	
	RubyClassFactory(Class klass) {
		super(klass);
	}
	
	protected boolean isModule() {
		return false;
	}
	
	protected Class getTypeAnnotationClass() {
		return RubyLevelClass.class;
	}

	protected String getBuilderName(Class klass) {
		return CgUtil.getInternalName(klass.getName() + "$ClassBuilder");
	}

	protected Type getInterface() {
		return BuilderInterface;
	}

	protected Method getBuilderMethod() {
		return RubyClassBuilderCreateRubyClassMethod; 
	}

	protected int createRubyType(GeneratorAdapter mg, Annotation annotation) {
		RubyLevelClass klassAnnotation = (RubyLevelClass)annotation;
		mg.push(klassAnnotation.name());
		loadRubyClass(mg, klassAnnotation.superclass());
		mg.invokeStatic(Types.RUBY_API_TYPE, RubyAPIDefineClassMethod);

		int rubyTypeIdx = mg.newLocal(Types.RUBY_CLASS_TYPE);
		mg.storeLocal(rubyTypeIdx);

//		for (String moduleName : klassAnnotation.modules()) {
    	for( int i = 0; i<klassAnnotation.modules().length; i++){
    		String moduleName = klassAnnotation.modules()[i];
		
			includeModule(mg, rubyTypeIdx, moduleName);
		}
		
//		for (UndefMethod method : klassAnnotation.undef()) {
    	for( int i = 0; i<klassAnnotation.undef().length; i++){
    		UndefMethod method = klassAnnotation.undef()[i];
    	
			undefMethod(mg, rubyTypeIdx, method);
		}
		
//		for (com.xruby.runtime.lang.annotation.DummyMethod dummy : klassAnnotation.dummy()) {
    	for( int i = 0; i<klassAnnotation.dummy().length; i++){
    		com.xruby.runtime.lang.annotation.DummyMethod dummy = klassAnnotation.dummy()[i];
    	
			mg.loadLocal(rubyTypeIdx);
			mg.push(dummy.name());
			Type type = Type.getType(DummyMethod.class);
			mg.getStatic(type, "INSTANCE", type);
			if (dummy.privateMethod()) {
				mg.invokeVirtual(Types.RUBY_MODULE_TYPE, 
						CgUtil.getMethod("definePrivateMethod", Types.RUBY_VALUE_TYPE, Type.getType(String.class), Types.RUBY_METHOD_TYPE));
			} else {
				mg.invokeVirtual(Types.RUBY_MODULE_TYPE,
						CgUtil.getMethod("defineMethod", Types.RUBY_VALUE_TYPE, Type.getType(String.class), Types.RUBY_VALUE_TYPE));
			}
		}

		return rubyTypeIdx;
	}

	private void includeModule(GeneratorAdapter mg, int rubyTypeIdx, String moduleName) {
		mg.loadLocal(rubyTypeIdx);
		loadModule(mg, moduleName, rubyTypeIdx);
		mg.invokeVirtual(Types.RUBY_MODULE_TYPE,
				CgUtil.getMethod("includeModule", Type.VOID_TYPE, Types.RUBY_MODULE_TYPE));
	}
	
	private void undefMethod(GeneratorAdapter mg, int rubyTypeIdx, UndefMethod method) {
		mg.loadLocal(rubyTypeIdx);
		if (method.classMethod()) {
			mg.invokeVirtual(Types.RUBY_VALUE_TYPE, 
					CgUtil.getMethod("getRubyClass", Types.RUBY_CLASS_TYPE));
		}
		
		mg.push(method.name());
		mg.invokeVirtual(Types.RUBY_MODULE_TYPE,
				CgUtil.getMethod("undefMethod", Type.VOID_TYPE, Type.getType(String.class)));
	}

	private void loadModule(GeneratorAdapter mg, String module, int rubyTypeIdx) {
		if (Types.isBuiltinModule(module)) {
			mg.getStatic(Types.RUBY_RUNTIME_TYPE, 
					module + "Module", Types.RUBY_MODULE_TYPE);
		} else {
			mg.loadLocal(rubyTypeIdx);
			mg.push(module);
			mg.invokeStatic(Types.RUBY_API_TYPE,
					CgUtil.getMethod("getCurrentNamespaceConstant", Types.RUBY_VALUE_TYPE, Types.RUBY_MODULE_TYPE, Type.getType(String.class)));
			mg.checkCast(Types.RUBY_MODULE_TYPE);
		}
	}
}*/
