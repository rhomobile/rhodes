package com.rho.printer;

import java.util.Hashtable;
import java.util.Map;

import com.rhomobile.printing.api.IPrinterDevice.IPrinterResponce;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.util.Utils;

public class RhoPrinterResponceBase implements IPrinterResponce {

	private IMethodResult mMethodResult = null;
	
	
	public RhoPrinterResponceBase(IMethodResult result) {
		mMethodResult = result;
	}
	
	public IMethodResult getMethodResult() {
		return mMethodResult;
	}
	
	public void returnHash(String name1, Object value1) {
		returnHash(name1, value1, null, null);
	}

	public void returnHash(String name1, Object value1, String name2, Object value2) {
		returnHash(name1, value1, name2, value2, null, null);
	}
	
	public void returnHash(String name1, Object value1, String name2, Object value2, String name3, Object value3) {
		Map<String,Object> resHash = new Hashtable<String,Object>();

		if ((name1 != null) && (value1 != null)) {
			resHash.put(name1, value1);
		}
		if ((name2 != null) && (value2 != null)) {
			resHash.put(name2, value2);
		}
		if ((name3 != null) && (value3 != null)) {
			resHash.put(name3, value3);
		}
		getMethodResult().set(resHash);
	}
	
	public void onError(String message) {
		Thread.dumpStack();
		Utils.platformLog("ZebraPrinterResponce", "ERROR : "+message);
		getMethodResult().set(IPrinterSingleton.PRINTER_STATUS_ERROR);
	}
	
	public void onErrorHash(String message) {
		Thread.dumpStack();
		Map<String,Object> resHash = new Hashtable<String,Object>();
		resHash.put(IPrinterSingleton.HK_STATUS, IPrinterSingleton.PRINTER_STATUS_ERROR);
		if (message != null) {
			resHash.put(IPrinterSingleton.HK_MESSAGE, message);
			Utils.platformLog("ZebraPrinterResponce HASH", "ERROR : "+message);
		}
		getMethodResult().set(resHash);
	}

	public void onSuccess() {
		getMethodResult().set(IPrinterSingleton.PRINTER_STATUS_SUCCESS);
	}

	public void onReturnHash(Map<String, Object> result) {
		getMethodResult().set(result);
	}

	public void onReturnStatus(String status) {
		getMethodResult().set(status);		
	}	
}
