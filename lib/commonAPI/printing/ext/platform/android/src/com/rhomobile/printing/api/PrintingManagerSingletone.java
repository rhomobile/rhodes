package com.rhomobile.printing.api;

import com.rhomobile.printing.impl.PrintingManagerImpl;

public class PrintingManagerSingletone  {

private static IPrintingManager ourPrintingManager = null;	
	
	
public static IPrintingManager getInstance() {
	if (ourPrintingManager == null) {
		ourPrintingManager = new PrintingManagerImpl();
	}
	return ourPrintingManager;
}




}
