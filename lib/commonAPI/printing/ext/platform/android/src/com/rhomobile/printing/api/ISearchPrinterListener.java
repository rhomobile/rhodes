package com.rhomobile.printing.api;

public interface ISearchPrinterListener {
	public void onFound(String printer_id);
	
	public void onError(String message);
	
	public void onFinished();
	
	public void onNotConnected(String message);
}
