package com.rho.rhoelements.activex;

import android.content.Context;
import android.view.inputmethod.InputMethod;
import android.view.inputmethod.InputMethodManager;

import com.rho.rhoelements.Common;

public class NoSIP extends ActiveX
{
	private NoSIP()
	{
	};
	
	static public NoSIP getInstance()
	{
		// decide here if this class should be a singleton
		NoSIP temp = new NoSIP();
		return temp;
	}
	
	public String[] showsip(String[] args)
	{
		int length;
		if (args == null)
			return null;
		if (args.length != 1)
			return null;
		InputMethodManager imm = (InputMethodManager)Common.mainActivity.getSystemService(Context.INPUT_METHOD_SERVICE);
		if (args[0].equalsIgnoreCase("true"))
		{
 			imm.showSoftInput(Common.elementsCore.getWebViewInstance().getView(), 0);
		}
		else if (args[0].equalsIgnoreCase("false"))
		{
			imm.hideSoftInputFromWindow(Common.elementsCore.getWebViewInstance().getView().getWindowToken(), 0);
		}
		return null;
	}
}
