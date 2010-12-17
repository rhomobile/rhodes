package com.rho.db;

public class DBException extends Exception {
	public DBException(Throwable exc){
		super("Class name:" + exc.getClass().getName() + ";Message: " + exc.getMessage() != null ? exc.getMessage() : "");
	}
	public DBException(String msg){
		super(msg);
	}
	
}
