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
import com.xruby.runtime.lang.annotation.RubyLevelModule;

class RubyModuleFactory extends RubyTypeFactory {
	private static final Method RubyModuleBuilderCreateRubyModuleMethod = 
		CgUtil.getMethod("createRubyModule", Types.RUBY_MODULE_TYPE);
	private static final Method RubyAPIDefineModuleMethod = 
		CgUtil.getMethod("defineModule", Types.RUBY_MODULE_TYPE, Type.getType(String.class));
	private static final Type BuilderInterface = Type.getType(RubyModuleBuilder.class);
	
	RubyModuleFactory(Class klass) {
		super(klass);
	}
	
	protected boolean isModule() {
		return true;
	}
	
	protected Class getTypeAnnotationClass() {
		return RubyLevelModule.class;
	}
	
	protected String getBuilderName(Class klass) {
		return CgUtil.getInternalName(klass.getName() + "$ModuleBuilder");
	}

	protected Type getInterface() {
		return BuilderInterface;
	}
	
	protected Method getBuilderMethod() {
		return RubyModuleBuilderCreateRubyModuleMethod;
	}
	
	protected int createRubyType(GeneratorAdapter mg, Annotation annotation) {
		RubyLevelModule moduleAnnotation = (RubyLevelModule)annotation;
		mg.push(moduleAnnotation.name());
		mg.invokeStatic(Types.RUBY_API_TYPE, RubyAPIDefineModuleMethod);
		int rubyTypeIdx = mg.newLocal(Types.RUBY_MODULE_TYPE);
		mg.storeLocal(rubyTypeIdx);
		
		return rubyTypeIdx;
	}
}*/
