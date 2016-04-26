package com.rho.printerzebra;

import com.rhomobile.printing.api.IPrintingManager;
import com.rhomobile.printing.api.PrintingManagerSingletone;
import com.rhomobile.printing.zebra.api.IZebraPrinter;
import com.rhomobile.printing.zebra.api.ZebraPrintingProviderSingletone;
import com.rhomobile.rhodes.api.RhoApiFactory;
import com.rhomobile.rhodes.util.Utils;

public class PrinterZebraFactory
        extends RhoApiFactory< PrinterZebra, PrinterZebraSingleton>
        implements IPrinterZebraFactory {

	public PrinterZebraFactory() {
		// register Zebra provider in main printing manager (see printing extension)
    	Utils.platformLog("Zebra Printing extension", "init()");
	   	IPrintingManager manager = PrintingManagerSingletone.getInstance();
    	manager.registerPrintingProvider(ZebraPrintingProviderSingletone.getInstance());
	}
	
	
    @Override
    protected PrinterZebraSingleton createSingleton() {
        return new PrinterZebraSingleton(this);
    }

    @Override
    protected PrinterZebra createApiObject(String id) {
    	if ((IZebraPrinter)ZebraPrintingProviderSingletone.getInstance().getPrinterByID(id) != null) {
    		return new PrinterZebra(id);
    	}
    	return null;
    }
}
