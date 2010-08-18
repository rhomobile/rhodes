package com.rho;

public class TestProfiler {
	private static final RhoProfiler PROF = RhoProfiler.RHO_STRIP_PROFILER ? new RhoEmptyProfiler() : 
		new RhoProfiler();

	static void Sleep(int nDelay){
	    try{ Thread.sleep(nDelay); }catch(Exception exc){}
	}
	
	static void testLocalNamedCounter(){
		PROF.START("LocalCounter1");
	    Sleep(2126);
	    PROF.STOP("LocalCounter1");
	}

	static void testGlobalNamedCounter(){
		PROF.CREATE_COUNTER("GlobalCounter1");

		PROF.START("GlobalCounter1");
	    Sleep(1234);
	    PROF.STOP("GlobalCounter1");

	    PROF.START("GlobalCounter1");
	    Sleep(1234);
	    PROF.STOP("GlobalCounter1");

	    PROF.DESTROY_COUNTER("GlobalCounter1");
	}

	static void testGlobalNamedCounterFlush(){
		PROF.CREATE_COUNTER("GlobalCounter2");

		PROF.START("GlobalCounter2");
	    Sleep(1234);
	    PROF.STOP("GlobalCounter2");

	    PROF.FLUSH_COUNTER("GlobalCounter2","Step1");

	    PROF.START("GlobalCounter2");
	    Sleep(1234);
	    PROF.STOP("GlobalCounter2");

	    PROF.DESTROY_COUNTER("GlobalCounter2");
	}
	
	static public void runAllTests(){
	    testLocalNamedCounter();
	    testGlobalNamedCounter();
	    testGlobalNamedCounterFlush();
	}
	
}
