package com.rho.printer;


import java.util.List;
import java.util.Map;
import com.rhomobile.printing.api.*;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.util.Utils;

class PrinterSingleton extends PrinterSingletonBase implements IPrinterSingleton {
    public PrinterSingleton(PrinterFactory factory) {
        super(factory);
    }

    List<Object> getIDs() {
        List<Object> ids = PrintingManagerSingletone.getInstance().getPrintersIDs();
        return ids;
    }
    
    @Override
    protected String getInitialDefaultID() {
     	List<Object> prn_list = getIDs();
    	if (prn_list.size() > 0) {
    		return (String)prn_list.get(0);
    	}
        return null;//(String)(getIDs().get(0));
    }

    @Override
    public void enumerate(IMethodResult res) {
    	//TODO:
    	res.set(getIDs());
    }

    public void enumerateSupportedTypes(IMethodResult result) {
    	result.set(PrintingManagerSingletone.getInstance().getRegisteredProvidersNames());
    }
    
    
    private class SearchPrinterListenerImpl extends RhoPrinterResponceBase implements ISearchPrinterListener {
    	
    	public SearchPrinterListenerImpl(IMethodResult result) {
    		super(result);
    	}

		public void onFound(String printer_id) {
			//Utils.platformLog("@@@@ SearchPrinterListenerImpl.onFound :", printer_id);

			if (printer_id != null) {
				if (printer_id.length() > 0) {
					returnHash(IPrinterSingleton.HK_STATUS, IPrinterSingleton.PRINTER_STATUS_SUCCESS, IPrinterSingleton.HK_PRINTER_ID, /*"RHOPRINTER_"+*/printer_id);
				}
			}
		}

		public void onFinished() {
			//Utils.platformLog("@@@@ SearchPrinterListenerImpl.onFinished", "");
			returnHash(IPrinterSingleton.HK_STATUS, IPrinterSingleton.PRINTER_STATUS_SUCCESS);
		}
    	
		public void onError(String message) {
			//Utils.platformLog("@@@@ SearchPrinterListenerImpl.onError :", message);
			returnHash(IPrinterSingleton.HK_STATUS, IPrinterSingleton.PRINTER_STATUS_ERROR, IPrinterSingleton.HK_MESSAGE, message);
		}    	
		
		public void onNotConnected(String message) {
			//Utils.platformLog("@@@@ SearchPrinterListenerImpl.onNotConnected :", message);
			returnHash(IPrinterSingleton.HK_STATUS, IPrinterSingleton.PRINTER_STATUS_ERR_NOT_CONNECTED, IPrinterSingleton.HK_MESSAGE, message);
		}

    	
    }
    
    
    public void searchPrinters(Map<String, Object> options, IMethodResult result) {
    	SearchPrinterListenerImpl bridge = new SearchPrinterListenerImpl(result);
    	if (!PrintingManagerSingletone.getInstance().searchPrinters(options, bridge) ) {
    		bridge.returnHash(IPrinterSingleton.HK_STATUS, IPrinterSingleton.PRINTER_STATUS_ERR_UNSUPPORTED);
    	}
    }
    
    public void stopSearch(IMethodResult result) {
    	PrintingManagerSingletone.getInstance().stopSearching();
    }
    
    public void getPrinterByID(String id, IMethodResult result) {
    	
    	Utils.platformLog("Printer", "getPrinterByID("+id+") = "+id);
    	
    	if (id == null) {
    		result.setError("Invalid printer id == null !");
    		return;
    	}
    	
    	if (PrintingManagerSingletone.getInstance().getPrinterByID(id) != null) {
        	result.set(id);
    	}
    }
    


}
