/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang.util;
/*
import java.lang.annotation.Annotation;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.commons.GeneratorAdapter;
import org.objectweb.asm.commons.Method;
import org.objectweb.asm.util.CheckClassAdapter;

import com.xruby.compiler.XRubyClassLoader;
import com.xruby.compiler.codegen.CgConfig;
import com.xruby.compiler.codegen.CgUtil;
import com.xruby.compiler.codegen.ClassDumper;
import com.xruby.compiler.codegen.Types;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyMethod;
import com.xruby.runtime.lang.RubyModule;
import com.xruby.runtime.lang.RubyObject;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.annotation.RubyAllocMethod;
import com.xruby.runtime.lang.annotation.RubyLevelConstant;
import com.xruby.runtime.lang.annotation.RubyLevelMethod;

public abstract class RubyTypeFactory {
	public static RubyClass getClass(Class klass) {	
		Class loadClass = new RubyClassFactory(klass).loadClass();
		if (loadClass == null) { 
			return null; 
		}
		
		return loadRubyClassBuilder(loadClass).createRubyClass();
	}

	private static RubyClassBuilder loadRubyClassBuilder(Class loadClass) {
		try {
			return (RubyClassBuilder)loadClass.newInstance();
		} catch (InstantiationException ie) {
			throw new RuntimeException("fail to create Ruby class", ie);
		} catch (IllegalAccessException iae) {
			throw new RuntimeException("fail to create Ruby class", iae);
		}
	}
	
	public static RubyModule getModule(Class klass) {
		Class loadClass = new RubyModuleFactory(klass).loadClass();
		if (loadClass == null) { 
			return null; 
		}
		
		return loadRubyModuleBuilder(loadClass).createRubyModule();
	}
	
	
	
	private static RubyModuleBuilder loadRubyModuleBuilder(Class loadClass) {
		try {
			return (RubyModuleBuilder)loadClass.newInstance();
		} catch (InstantiationException ie) {
			throw new RuntimeException("fail to create Ruby module", ie);
		} catch (IllegalAccessException iae) {
			throw new RuntimeException("fail to create Ruby module", iae);
		}
	}
	
	public static RubyObject getObject(Class klass) {	
		Class loadClass = new RubyObjectFactory(klass).loadClass();
		if (loadClass == null) { 
			return null; 
		}
		
		return loadRubyObjectBuilder(loadClass).createRubyObject();
	}

	private static RubyObjectBuilder loadRubyObjectBuilder(Class loadClass) {
		try {
			return (RubyObjectBuilder)loadClass.newInstance();
		} catch (InstantiationException ie) {
			throw new RuntimeException("fail to create Ruby object", ie);
		} catch (IllegalAccessException iae) {
			throw new RuntimeException("fail to create Ruby object", iae);
		}
	}
	
	protected static void loadRubyClass(GeneratorAdapter mg, String superclass) {
		if (superclass == null || superclass.length() ==  0) {
			mg.push((String)null);
		}
		
		if (Types.isBuiltinClass(superclass)) {
			mg.getStatic(Types.RUBY_RUNTIME_TYPE, 
					superclass + "Class", Types.RUBY_CLASS_TYPE);
		} else {
			// FIXME: 
		}
	}

	Class loadClass() {
		String name = getBuilderName(klass);
		Class klass = tryClass(name);
		if (klass == null) {
			return createClass(name);
		}
		
		return klass;
	}

	private Class createClass(String name) {
		Annotation annotation = klass.getAnnotation(this.getTypeAnnotationClass());
		if (annotation == null) {
			throw new RuntimeException("no annotation class:" + klass);
		}
		
		startBuilder(name);
		createBuilderMethod(annotation);
		endClassBuilder();
		
		return loadBuidlerClass(name);
	}
	
	private Class loadBuidlerClass(String name) {
		byte[] content = cw.toByteArray();
        dupmer.dump(name, content);
		return cl.load(name, content);
	}
	
	private Class tryClass(String name) {
		try {
			return cl.load(name);
		} catch (Exception e) {
			return null;
		}
	}
	
	private static final Type methodFactoryType = Type.getType(MethodFactory.class);
	private static final XRubyClassLoader cl = new XRubyClassLoader();
	private static ClassDumper dupmer = new ClassDumper();
	private static final Type OBJECT_TYPE = Type.getType(Object.class);
	
	private Class klass;
	private ClassWriter cw;
	private ClassVisitor cv;
	
	RubyTypeFactory(Class klass) {
		this.klass = klass;
		this.cw = new ClassWriter(ClassWriter.COMPUTE_MAXS);
		this.cv = new CheckClassAdapter(cw);
	}
	
	protected abstract Class getTypeAnnotationClass();
	protected abstract String getBuilderName(Class klass);
	protected abstract Type getInterface();
	protected abstract Method getBuilderMethod();
	protected abstract int createRubyType(GeneratorAdapter mg, Annotation annotation);
	protected abstract boolean isModule();
	
	private void startBuilder(String name) {
		this.cv.visit(CgConfig.TARGET_VERSION, Opcodes.ACC_PUBLIC, name, 
				null, OBJECT_TYPE.getInternalName(), new String[] { getInterface().getInternalName()});
		
		CgUtil.createImplicitConstructor(this.cv, OBJECT_TYPE);
	}
	
	private GeneratorAdapter startBuilderMethod() {
		GeneratorAdapter mg = new GeneratorAdapter(Opcodes.ACC_PUBLIC,
                getBuilderMethod(), null, null, this.cv);
		mg.visitCode();
		return mg;
	}
	
	private void endClassBuilder() {
		cv.visitEnd();
	}
	
	private void createBuilderMethod(Annotation annotation) {	
		GeneratorAdapter mg = startBuilderMethod();
		int rubyTypeIdx = createRubyType(mg, annotation);
		generateMethod(mg, rubyTypeIdx);
		generateConstant(mg, rubyTypeIdx);
		
		mg.loadLocal(rubyTypeIdx);		
        mg.returnValue();
        mg.endMethod();
	}
	
	private void generateConstant(GeneratorAdapter mg, int rubyTypeIdx) {
//		for (Field field : klass.getFields()) {
        for (Iterator iter = klass.getFields().iterator(); iter.hasNext();) {
        	Field field = (Field)iter.next();
		
			int modifier = field.getModifiers();
			if (!(Modifier.isStatic(modifier) || Modifier.isPublic(modifier))) {
				continue;
			}
			
			Annotation rawFieldAnnotation = field.getAnnotation(RubyLevelConstant.class);
			if (rawFieldAnnotation == null) {
				continue;
			}
			
			defineRubyConstant(mg, field, (RubyLevelConstant)rawFieldAnnotation, rubyTypeIdx);
		}
	}

	private void defineRubyConstant(GeneratorAdapter mg, Field field, 
			RubyLevelConstant constant, int rubyTypeIdx) {
		mg.loadLocal(rubyTypeIdx);
		mg.push(constant.name());
		mg.getStatic(Type.getType(this.klass), field.getName(), Type.getType(field.getType()));
		mg.invokeVirtual(Types.RUBY_MODULE_TYPE, 
				CgUtil.getMethod("setConstant", Types.RUBY_VALUE_TYPE, Type.getType(String.class), Types.RUBY_VALUE_TYPE));
	}

	private void generateMethod(GeneratorAdapter mg, int rubyTypeIdx) {
		int factoryIdx = createLocalMethodFactory(mg);
		
		//Map<String, CgMethodItem> methodMap = new HashMap<String, CgMethodItem> ();
		Map methodMap = new HashMap ();		
		CgMethodItem allocItem = null;
		
//		for (java.lang.reflect.Method method : klass.getMethods()) {
        for (Iterator iter = klass.getMethods().iterator(); iter.hasNext();) {
        	java.lang.reflect.Method method = (java.lang.reflect.Method)iter.next();
		
			if (method.getDeclaringClass() != klass) {
				continue;
			}
			
			Annotation rawMethodAnnotation = method.getAnnotation(RubyLevelMethod.class);
			if (rawMethodAnnotation != null) {
				CgMethodItem newItem = createMethodItem((RubyLevelMethod)rawMethodAnnotation, method);
				CgMethodItem item = methodMap.get(getItemName(newItem));
				if (item != null) {
					item.type = MethodType.valueOf((item.type.value() | newItem.type.value()));
				} else {
					methodMap.put(getItemName(newItem), newItem);
				}
				
				continue;
			}
			
			Annotation allocMethodAnnotation = method.getAnnotation(RubyAllocMethod.class);
			if (allocMethodAnnotation != null) {
				CgMethodItem item = createAllocItem((RubyAllocMethod)allocMethodAnnotation, method);
				if (allocItem != null) {
					allocItem.type = MethodType.valueOf(allocItem.type.value() | (item.type.value()));
				} else {
					allocItem = createAllocItem((RubyAllocMethod)allocMethodAnnotation, method);
				}
				continue;
			}
		}
		
//		for (CgMethodItem item : methodMap.values()) {
        for (Iterator iter = methodMap.values().iterator(); iter.hasNext();) {
        	CgMethodItem item = (CgMethodItem)iter.next();
        
			defineRubyMethod(mg, rubyTypeIdx, factoryIdx, item);
		}
		
		if (allocItem != null) {
			defineAllocMethod(mg, rubyTypeIdx, factoryIdx, allocItem);
		}
		
//		for (Class innerClass : klass.getClasses()) {
        for (Iterator iter = klass.getClasses().iterator(); iter.hasNext();) {
        	Class innerClass = (Class)iter.next();
		
			Annotation rawMethodAnnotation = innerClass.getAnnotation(RubyLevelMethod.class);
			if (rawMethodAnnotation != null) {
				if (!RubyMethod.class.isAssignableFrom(innerClass)) {
					throw new RuntimeException(innerClass.getName() + " should be subclasss of RubyMethod");
				}
				
				CgMethodItem item = createMethodItem((RubyLevelMethod)rawMethodAnnotation, innerClass);
				defineRubyMethodWithClass(mg, rubyTypeIdx, factoryIdx, innerClass, item);
			}
		}
	}

	private void defineRubyMethodWithClass(GeneratorAdapter mg, int rubyTypeIdx, int factoryIdx, Class innerClass, CgMethodItem item) {
		loadRubyType(mg, rubyTypeIdx, item);
		mg.push(item.name);
		Type type = Type.getType(innerClass);
		mg.newInstance(type);
		mg.dup();
		mg.invokeConstructor(type, CgUtil.CONSTRUCTOR);
		defineMethod(mg, item);
		defineAlias(mg, factoryIdx, rubyTypeIdx, item);
	}
	
	private String getItemName(CgMethodItem item) {
		return (item.singleton) ? item.name + "Single" : item.name;
	}
	
	private CgMethodItem createMethodItem(RubyLevelMethod annotation, Class klass) {
		CgMethodItem item = new CgMethodItem();
		item.javaName = klass.getName();
		makeGenneralItem(annotation, item);
		return item;
	}
	
	private CgMethodItem createMethodItem(RubyLevelMethod annotation, java.lang.reflect.Method method) {
		CgMethodItem item = makeMethodItemPros(method);
		item.javaName = method.getName();
		makeGenneralItem(annotation, item);
		return item;
	}

private void makeGenneralItem(RubyLevelMethod annotation, CgMethodItem item) {
	item.name = annotation.name();
	item.alias = annotation.alias();
	
	if (annotation.singleton()) {
		item.singleton = true;
	}
	
	if (annotation.module()) {
		item.moduleMethod = true;
	}
	
	if (annotation.privateMethod()) {
		item.privateMethod = true;
	}
}
	
	private CgMethodItem createAllocItem(RubyAllocMethod annotation, java.lang.reflect.Method method) {
		CgMethodItem item = makeMethodItemPros(method);
		item.name = null;
		item.javaName = method.getName();
		item.alias = null;
		
		return item;
	}

	private CgMethodItem makeMethodItemPros(java.lang.reflect.Method method) {
		CgMethodItem item = new CgMethodItem();
		item.singleton = false;
		Class[] paramTypes = method.getParameterTypes();
		int start = 0;
		int end = paramTypes.length - 1;
		if (Modifier.isStatic(method.getModifiers())) {
			if (paramTypes[0] != RubyValue.class) {
				throw new IllegalArgumentException("unknown Ruby method specification:" + method);
			}
			
			if (!isModule()) {
				item.singleton = true;
			}
			
			start++;
		}
		
		item.block = false;
		if (paramTypes.length > 0 && paramTypes[paramTypes.length - 1] == RubyBlock.class) {
			item.block = true;
			end--;
		}
		
		item.type = getMethodType(paramTypes, start, end);
		if (item.type == MethodType.UNKNOWN) {
			throw new IllegalArgumentException("unknown Ruby method specification:" + method);
		}
		
		return item;
	}

	private MethodType getMethodType(Class[] paramTypes, int start, int end) {
		int argSize = end - start;
		if (argSize < 0) {
			return MethodType.NO_ARG;
		} else if (argSize == 0) {
			if (paramTypes[start] == RubyValue.class) {
				return MethodType.ONE_ARG;
			} else if (paramTypes[start] == RubyArray.class) {
				return MethodType.VAR_ARG;
			}
		} else if (argSize == 1) {
			if (paramTypes[start] == RubyValue.class && paramTypes[end] == RubyValue.class) {
				return MethodType.TWO_ARG;
			}
		}

		return MethodType.UNKNOWN;
	}
	
	private void defineAllocMethod(GeneratorAdapter mg, int rubyTypeIdx, int factoryIdx, CgMethodItem item) {
		mg.loadLocal(rubyTypeIdx);
		mg.loadLocal(factoryIdx);
		
		getMethod(mg, item.javaName, item.type, true, item.block);
		mg.invokeVirtual(Types.RUBY_CLASS_TYPE, 
				CgUtil.getMethod("defineAllocMethod", Type.VOID_TYPE, Types.RUBY_METHOD_TYPE));
	}
	
	private int createLocalMethodFactory(GeneratorAdapter mg) {
		createMehtodFactory(mg, klass);
		int factoryIdx = mg.newLocal(methodFactoryType);
		mg.storeLocal(factoryIdx);
		return factoryIdx;
	}
	
	private void loadRubyType(GeneratorAdapter mg, int rubyTypeIdx, CgMethodItem item) {
		mg.loadLocal(rubyTypeIdx);
		if (item.singleton) {
			mg.invokeVirtual(Types.RUBY_VALUE_TYPE, 
					CgUtil.getMethod("getSingletonClass", Types.RUBY_CLASS_TYPE));
		}
	}
	
	private void defineRubyMethod(GeneratorAdapter mg, int rubyTypeIdx, 
			int factoryIdx, CgMethodItem item) {
		loadRubyType(mg, rubyTypeIdx, item);
		
		String rubyName = item.name;
		defineMethod(mg, factoryIdx, rubyName, item);
		defineAlias(mg, factoryIdx, rubyTypeIdx, item);
	}

	private void defineMethod(GeneratorAdapter mg, int factoryIdx, String rubyName, CgMethodItem item) {
		mg.push(rubyName);
		mg.loadLocal(factoryIdx);
		getMethod(mg, item);
		
		defineMethod(mg, item);
	}

	private static final Method RubyModuleDefinePrivateMethodMethod = 
		CgUtil.getMethod("definePrivateMethod", Types.RUBY_VALUE_TYPE, Type.getType(String.class), Types.RUBY_METHOD_TYPE);
	private static final Method RubyModuleDefineMethodMethod = 
		CgUtil.getMethod("defineMethod", Types.RUBY_VALUE_TYPE, Type.getType(String.class), Types.RUBY_METHOD_TYPE);
	private static final Method RubyModuleDefineModuleMethodMethod = 
		CgUtil.getMethod("defineModuleMethod", Type.VOID_TYPE, Type.getType(String.class), Types.RUBY_METHOD_TYPE);
	private static final Method RubyValueGetSingletonClass = 
		CgUtil.getMethod("getSingletonClass", Types.RUBY_CLASS_TYPE);
	private static final Method RubyModuleAliasMethod = 
		CgUtil.getMethod("aliasMethod", Type.VOID_TYPE, Type.getType(String.class), Type.getType(String.class));
	private static final Method MethodFactoryGetMethodMethod = 
		CgUtil.getMethod("getMethod", Types.RUBY_METHOD_TYPE, Type.getType(String.class), Type.getType(MethodType.class), Type.BOOLEAN_TYPE, Type.BOOLEAN_TYPE);
	private static final Method MethodFactoryCreateMethodFactory = 
		CgUtil.getMethod("createMethodFactory", methodFactoryType, Type.getType(Class.class), Type.BOOLEAN_TYPE);
	
	private void defineMethod(GeneratorAdapter mg, CgMethodItem item) {
		if (item.moduleMethod) {
			mg.invokeVirtual(Types.RUBY_MODULE_TYPE, 
					RubyModuleDefineModuleMethodMethod);
		} else if (item.privateMethod) {
			mg.invokeVirtual(Types.RUBY_MODULE_TYPE,
					RubyModuleDefinePrivateMethodMethod);
		} else {
			mg.invokeVirtual(Types.RUBY_MODULE_TYPE, 
					RubyModuleDefineMethodMethod);
		}
	}

	private void defineAlias(GeneratorAdapter mg, int factoryIdx, int rubyTypeIdx, CgMethodItem item) {
		String oldName = item.name;
		String[] aliases = item.alias;
		boolean singleton = item.singleton;
		
//		for (String alias : aliases) {
		for(int i = 0; i<aliases.length;i++){
			String alias = aliases[i];
			
			mg.loadLocal(rubyTypeIdx);
			if (singleton) {
				mg.invokeVirtual(Types.RUBY_MODULE_TYPE, 
						RubyValueGetSingletonClass);
			}
			
			if (item.moduleMethod) {
				defineMethod(mg, factoryIdx, alias, item);
			} else {
				mg.push(alias);
				mg.push(oldName);
				mg.invokeVirtual(Types.RUBY_MODULE_TYPE, 
						RubyModuleAliasMethod);
			}
		}
	}
	
	private void getMethod(GeneratorAdapter mg, CgMethodItem item) {
		getMethod(mg, item.javaName, item.type, item.singleton, item.block);
	}
	
	private void getMethod(GeneratorAdapter mg, String methodName, MethodType type, boolean singleton, boolean block) {
		mg.push(methodName);
		type.generateMethodType(mg);
		mg.push(singleton);
		mg.push(block);
		mg.invokeVirtual(methodFactoryType, MethodFactoryGetMethodMethod);
	}

	private void createMehtodFactory(GeneratorAdapter mg, Class klass) {
		mg.push(Type.getType(klass));
		mg.push(this.isModule());
		mg.invokeStatic(methodFactoryType, MethodFactoryCreateMethodFactory);
	}
}*/