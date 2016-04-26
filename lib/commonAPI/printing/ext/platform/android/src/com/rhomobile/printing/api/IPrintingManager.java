package com.rhomobile.printing.api;

import java.util.List;
import java.util.Map;


public interface IPrintingManager  {


public void registerPrintingProvider(IPrintingProvider provider);

public boolean searchPrinters(Map<String,Object> options, ISearchPrinterListener listener);

public void stopSearching();

public IPrinterDevice getPrinterByID(String printerID);

public List<Object> getRegisteredProvidersNames();

public List<Object> getPrintersIDs();

}
