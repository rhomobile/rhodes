package com.rho.db;

public class DBException extends Exception {
	public DBException(Throwable exc){
		super(exc.getMessage());
	}
}
