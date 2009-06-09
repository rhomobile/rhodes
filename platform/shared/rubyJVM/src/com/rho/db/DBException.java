package com.rho.db;

public class DBException extends Exception {
	public DBException(Throwable exc){
		super(exc.getMessage());
	}
	public DBException(String msg){
		super(msg);
	}
	
}
