package com.rho.printerzebra;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import com.rho.printer.IPrinterSingleton;
import com.rho.printer.RhoPrinterResponceBase;
import com.rhomobile.printing.api.IPrintingProvider;
import com.rhomobile.printing.api.ISearchPrinterListener;
import com.rhomobile.printing.api.PrintingManagerSingletone;
import com.rhomobile.printing.zebra.api.ZebraPrintingProviderSingletone;
import com.rhomobile.rhodes.api.IMethodResult;

class PrinterZebraSingleton extends PrinterZebraSingletonBase implements IPrinterZebraSingleton {
    public PrinterZebraSingleton(PrinterZebraFactory factory) {
        super(factory);
    }

    List<Object> getIDs() {
        List<Object> ids = ZebraPrintingProviderSingletone.getInstance().getPrintersIDs();
        return ids;
    }
    
    @Override
    protected String getInitialDefaultID() {
    	//TODO:
    	List<Object> prn_list = getIDs();
    	if (prn_list.size() > 0) {
    		return (String)prn_list.get(0);
    	}
        return "ZEBRA_PRINTER_1";//(String)(getIDs().get(0));
    }

    @Override
    public void enumerate(IMethodResult res) {
        res.set(getIDs());
    }
    
    public void getPrinterByID(String id, IMethodResult result) {
    	String fixed_id = id;
    	if (id == null) {
    		result.setError("Invalid printer id == null !");
    		return;
    	}
    	if (fixed_id.startsWith("RHOPRINTER_")) {
    		fixed_id = fixed_id.substring("RHOPRINTER_".length());
    	}    	
    	if (ZebraPrintingProviderSingletone.getInstance().getPrinterByID(fixed_id) != null) {
        	result.set(fixed_id);
    	}
    }
    
    public void enumerateSupportedTypes(IMethodResult result) {
        List<Object> ids = new LinkedList<Object>();
   		ids.add(IPrinterZebraSingleton.PRINTER_TYPE_ZEBRA);
        result.set(ids);	    	
    }
    
    private class SearchZebraPrinterListenerImpl extends RhoPrinterResponceBase implements ISearchPrinterListener {
    	
    	public SearchZebraPrinterListenerImpl(IMethodResult result) {
    		super(result);
    	}

		public void onFound(String printer_id) {
			returnHash(IPrinterSingleton.HK_STATUS, IPrinterSingleton.PRINTER_STATUS_SUCCESS, IPrinterSingleton.HK_PRINTER_ID, printer_id);
		}

		public void onFinished() {
			returnHash(IPrinterSingleton.HK_STATUS, IPrinterSingleton.PRINTER_STATUS_SUCCESS);
		}
    	
		public void onError(String message) {
			Thread.dumpStack();
			returnHash(IPrinterSingleton.HK_STATUS, IPrinterSingleton.PRINTER_STATUS_ERROR, IPrinterSingleton.HK_MESSAGE, message);
		}    	
		
		public void onNotConnected(String message) {
			returnHash(IPrinterSingleton.HK_STATUS, IPrinterSingleton.PRINTER_STATUS_ERR_NOT_CONNECTED, IPrinterSingleton.HK_MESSAGE, message);
		}    	
    }    
    
    public void searchPrinters(Map<String, Object> options, IMethodResult result) {
    	SearchZebraPrinterListenerImpl bridge = new SearchZebraPrinterListenerImpl(result);
    	if(!ZebraPrintingProviderSingletone.getInstance().searchPrinters(options, bridge)) {
    		bridge.returnHash(IPrinterSingleton.HK_STATUS, IPrinterSingleton.PRINTER_STATUS_ERR_UNSUPPORTED);
    	}
    }
    
    public void stopSearch(IMethodResult result) {
    	ZebraPrintingProviderSingletone.getInstance().stopSearching();
    }
    
     
}
