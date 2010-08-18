/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang.util;
/*
import java.util.ArrayList;
import java.util.List;

import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.Type;

import com.xruby.compiler.codegen.Types;

class MethodFactoryHelper {
	private Type superType;
	private RunMethodHelper[] helpers;
	
	private MethodFactoryHelper(Type superType, RunMethodHelper[] helpers) {
		this.superType = superType;
		this.helpers = helpers;
	}
	
	public Type getSuperType() {
		return this.superType;
	}
	
	public void createRunMethod(ClassVisitor cv, Class hostClass, Class castClass, 
			String name, boolean staticMethod, boolean block) {
//		for (RunMethodHelper helper : this.helpers) {
    	for( int i = 0; i<this.helpers.length; i++){
    		RunMethodHelper helper = this.helpers[i];
		
			helper.createRunMethod(cv, hostClass, castClass, name, staticMethod, block);
		}
	}
	
	private static final RunMethodHelper NO_ARG_RUN_HELPER = new NoArgRunMethodHelper();
	private static final RunMethodHelper ONE_ARG_RUN_HELPER = new OneArgRunMethodHelper();
	private static final RunMethodHelper TWO_ARG_RUN_HELPER = new TwoArgRunMethodHelper();
	private static final RunMethodHelper VAR_ARG_RUN_HELPER = new VarArgRunMethodHelper();

	private static final MethodFactoryHelper NO_ARG_HELPER =
		new MethodFactoryHelper(Types.RUBY_NOARGMETHOD_TYPE, NO_ARG_RUN_HELPER);	
	private static final MethodFactoryHelper ONE_ARG_HELPER =
		new MethodFactoryHelper(Types.RUBY_ONEARGMETHOD_TYPE, ONE_ARG_RUN_HELPER);
	private static final MethodFactoryHelper NO_OR_ONE_ARG_HELPER = 
		new MethodFactoryHelper(Types.RUBY_NOORONEARGMETHOD_TYPE, 
				NO_ARG_RUN_HELPER,	ONE_ARG_RUN_HELPER);
	private static final MethodFactoryHelper TWO_ARG_HELPER = 
		new MethodFactoryHelper(Types.RUBY_TWOARGMETHOD_TYPE, TWO_ARG_RUN_HELPER);
	private static final MethodFactoryHelper ONE_OR_TWO_ARG_HELPER = 
		new MethodFactoryHelper(Types.RUBY_ONEORTWOARGMETHOD_TYPE, 
				ONE_ARG_RUN_HELPER, TWO_ARG_RUN_HELPER);
	
	public static final MethodFactoryHelper getHelper(MethodType type) {
		if (type == MethodType.NO_ARG) {
			return NO_ARG_HELPER;
		} else if (type == MethodType.ONE_ARG) {
			return ONE_ARG_HELPER;
		} else if (type == MethodType.NO_OR_ONE_ARG) {
			return NO_OR_ONE_ARG_HELPER;
		} else if (type == MethodType.TWO_ARG) {
			return TWO_ARG_HELPER;
		} else if (type == MethodType.ONE_OR_TWO_ARG) {
			return ONE_OR_TWO_ARG_HELPER;
		} else {
//			List<RunMethodHelper> helpers = new ArrayList<RunMethodHelper>();
			List helpers = new ArrayList();
			helpers.add(VAR_ARG_RUN_HELPER);
			
			if (MethodType.isNoArg(type)) {
				helpers.add(NO_ARG_RUN_HELPER);
			}
			
			if (MethodType.isOneArg(type)) {
				helpers.add(ONE_ARG_RUN_HELPER);
			}
			
			if (MethodType.isTwoArg(type)) {
				helpers.add(TWO_ARG_RUN_HELPER);
			}
			
			return new MethodFactoryHelper(Types.RUBY_VARARGMETHOD_TYPE, 
					(RunMethodHelper[])helpers.toArray(new RunMethodHelper[0]));
		}
	}
}
*/