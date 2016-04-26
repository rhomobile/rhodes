package com.rho.rhoelements.npapi;

public abstract class Huddle
{

	public static final int TYPE_SIZE = 2;
	public static final int LENGTH_SIZE = 6;

	public static final int SOT = 0;
	public static final int HUDDLE_TYPE = 1;
	public static final int JS_OBJECT_NAME = 2;
	public static final int EMML = 3;
	public static final int ACTIVEX_TYPE = 4;
	public static final int ACTIVEX_ID = 5;
	public static final int METHOD = 6;
	public static final int ARG = 7;
	public static final int EOA = 8;
	public static final int EOT = 9;

	public abstract String[] execute();
}
