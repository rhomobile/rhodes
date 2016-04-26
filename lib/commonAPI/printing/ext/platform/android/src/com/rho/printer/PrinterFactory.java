package com.rho.printer;


import com.rhomobile.printing.api.PrintingManagerSingletone;
import com.rhomobile.rhodes.api.RhoApiFactory;

public class PrinterFactory
        extends RhoApiFactory< Printer, PrinterSingleton>
        implements IPrinterFactory {

    @Override
    protected PrinterSingleton createSingleton() {
        return new PrinterSingleton(this);
    }

    @Override
    protected Printer createApiObject(String id) {
    	if (PrintingManagerSingletone.getInstance().getPrinterByID(id) != null) {
    		return new Printer(id);
    	}
    	return null;    
    }
}
