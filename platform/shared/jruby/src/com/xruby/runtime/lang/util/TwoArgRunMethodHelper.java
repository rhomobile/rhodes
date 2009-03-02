/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang.util;

/*
import org.objectweb.asm.commons.GeneratorAdapter;
import org.objectweb.asm.commons.Method;

import com.xruby.compiler.codegen.CgUtil;
import com.xruby.compiler.codegen.Types;

public class TwoArgRunMethodHelper extends RunMethodHelper {
	private static final Method TwoArgRunMethod = 
		CgUtil.getMethod("run", Types.RUBY_VALUE_TYPE, Types.RUBY_VALUE_TYPE,
			Types.RUBY_VALUE_TYPE, Types.RUBY_VALUE_TYPE, Types.RUBY_BLOCK_TYPE);
	
	protected Method getRunMethod() {
		return TwoArgRunMethod;
	}
	
	protected void loadBlock(GeneratorAdapter mg) {
		mg.loadArg(3);
	}
	
	protected int rubyArgSize() {
		return 2;
	}
	
	protected void loadArgs(GeneratorAdapter mg) {
		mg.loadArg(1);
		mg.loadArg(2);
	}
}
*/