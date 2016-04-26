package com.rhomobile.printing.impl;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;


import com.rhomobile.printing.api.IPrinterDevice;
import com.rhomobile.printing.api.IPrintingManager;
import com.rhomobile.printing.api.IPrintingProvider;
import com.rhomobile.printing.api.ISearchPrinterListener;
import com.rhomobile.rhodes.util.Utils;

public class PrintingManagerImpl  implements IPrintingManager {

private ArrayList<IPrintingProvider> mProviders = null;	
	
	
	
	
public PrintingManagerImpl() {
	mProviders = new ArrayList<IPrintingProvider>();
}

public void registerPrintingProvider(IPrintingProvider provider) {
	if (!mProviders.contains(provider)) {
		mProviders.add(provider);
	}
}

public boolean searchPrinters(Map<String,Object> options, ISearchPrinterListener listener) {
	Iterator<IPrintingProvider> iterator = mProviders.iterator();
	boolean found_printer_type = false;
	while (iterator.hasNext()) {
		IPrintingProvider provider = iterator.next(); 
		String printerType = IPrinterDevice.PRINTER_TYPE_ANY;
		if (options != null) {
			Object valueObj = options.get("printerType");
			if ((valueObj != null) && (valueObj instanceof String)) {
				printerType = (String)valueObj;
			}
		}
		if ( (printerType.equals(IPrinterDevice.PRINTER_TYPE_ANY)) || (printerType.equals(provider.getType())) ) {
			found_printer_type = provider.searchPrinters(options, listener);
		}
	}
	return found_printer_type;
}

public void stopSearching() {
	Iterator<IPrintingProvider> iterator = mProviders.iterator();
	while (iterator.hasNext()) {
		iterator.next().stopSearching();
	}	
}

public IPrinterDevice getPrinterByID(String printerID) {
	
	Utils.platformLog("PrintingManagerImpl", "@@@@@ getPrinterByID("+printerID+")");
	if (printerID == null) {
		return null;
	}
	String fixed_id = printerID;
	if (fixed_id.startsWith("RHOPRINTER_")) {
		fixed_id = fixed_id.substring("RHOPRINTER_".length());
	}	
	Iterator<IPrintingProvider> iterator = mProviders.iterator();
	while (iterator.hasNext()) {
		IPrinterDevice p = iterator.next().getPrinterByID(fixed_id);
		if (p != null) {
			return p;
		}
	}	
	return null;
}

public List<Object> getRegisteredProvidersNames() {
    List<Object> ids = new LinkedList<Object>();
	Iterator<IPrintingProvider> iterator = mProviders.iterator();
	while (iterator.hasNext()) {
		IPrintingProvider p = iterator.next();
		ids.add(p.getType());
	}	
    return ids;	
}


public List<Object> getPrintersIDs() {
    List<Object> ids = new LinkedList<Object>();
	Iterator<IPrintingProvider> iterator = mProviders.iterator();
	while (iterator.hasNext()) {
		IPrintingProvider p = iterator.next();
		ids.addAll(p.getPrintersIDs());
	}	
    return ids;	    	
}


}
