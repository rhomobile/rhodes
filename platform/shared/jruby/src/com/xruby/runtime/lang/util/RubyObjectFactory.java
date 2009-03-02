package com.xruby.runtime.lang.util;

/*
import java.lang.annotation.Annotation;

import org.objectweb.asm.Type;
import org.objectweb.asm.commons.GeneratorAdapter;
import org.objectweb.asm.commons.Method;

import com.xruby.compiler.codegen.CgUtil;
import com.xruby.compiler.codegen.Types;
import com.xruby.runtime.lang.annotation.RubyLevelObject;

class RubyObjectFactory extends RubyTypeFactory {
	private static final Method RubyObjectBuilderCreateRubyObjectMethod = 
		CgUtil.getMethod("createRubyObject", Types.RUBY_OBJECT_TYPE);
	private static final Method RubyObjectConstructor = 
		CgUtil.getMethod("<init>", Type.VOID_TYPE, Types.RUBY_CLASS_TYPE);
	private static final Type BuilderInterface = Type.getType(RubyObjectBuilder.class);
	
	RubyObjectFactory(Class klass) {
		super(klass);
	}
	
	protected boolean isModule() {
		return false;
	}

	protected Class getTypeAnnotationClass() {
		return RubyLevelObject.class;
	}
	
	protected String getBuilderName(Class klass) {
		return CgUtil.getInternalName(klass.getName() + "$ObjectBuilder");
	}
	
	protected Type getInterface() {
		return BuilderInterface;
	}
	
	protected Method getBuilderMethod() {
		return RubyObjectBuilderCreateRubyObjectMethod;
	}

	protected int createRubyType(GeneratorAdapter mg, Annotation annotation) {
		RubyLevelObject objectAnnotation = (RubyLevelObject)annotation;
		
		mg.newInstance(Types.RUBY_OBJECT_TYPE);
		mg.dup();
		loadRubyClass(mg, objectAnnotation.objectClass());
		
		mg.invokeConstructor(Types.RUBY_OBJECT_TYPE, 
				RubyObjectConstructor);
		
		int rubyTypeIdx = mg.newLocal(Types.RUBY_OBJECT_TYPE);
		mg.storeLocal(rubyTypeIdx);
		
		String name = objectAnnotation.name();
		if (name != null && name.length() != 0) {
			mg.loadLocal(rubyTypeIdx);
			mg.push(name);
			mg.invokeStatic(Types.RUBY_API_TYPE, 
					CgUtil.getMethod("setTopLevelConstant", Types.RUBY_VALUE_TYPE, Types.RUBY_VALUE_TYPE, Type.getType(String.class)));
		}
		
		return rubyTypeIdx;
	}
}*/
