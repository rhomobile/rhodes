/**
 * Copyright 2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang.annotation;

public abstract class UndefMethod {
	public abstract String name();
	public boolean classMethod(){return false;}// default false;
}
