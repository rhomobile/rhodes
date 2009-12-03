package com.xruby.runtime.lang.annotation;

public @interface DummyMethod {
	String name();
	boolean privateMethod() default false;
}
