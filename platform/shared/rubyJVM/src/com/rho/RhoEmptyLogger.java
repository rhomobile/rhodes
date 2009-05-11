package com.rho;

public class RhoEmptyLogger extends RhoLogger {
	
	public void ASSERT(boolean exp, String message) {
	}

	public void ERROR(String message, Throwable e) {
	}

	public void ERROR(String message) {
	}

	public void ERROR(Throwable e) {
	}

	public void FATAL(String message, Throwable e) {
		processFatalError();
	}

	public void FATAL(String message) {
		processFatalError();
	}

	public void FATAL(Throwable e) {
		processFatalError();
	}

	public void INFO(String message) {
	}

	public void TRACE(String message) {
	}

	public void WARNING(String message) {
	}

	public RhoEmptyLogger(){
		super("");
	}

}
