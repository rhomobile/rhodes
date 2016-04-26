package com.rho.barcode;

import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.webview.GoogleWebView;
import com.rhomobile.rhodes.webview.WebViewConfig;

public class BarcodeRhoExtension extends AbstractRhoExtension{
	
	@Override
	public boolean onNavigateStarted(IRhoExtManager extManager, String url,
			IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
		int disable_scanner_val = 0;
		IRhoConfig rhoelementsGetConfig= null;
        IRhoWebView vi=RhodesActivity.safeGetInstance().getMainView().getWebView(0);
        GoogleWebView web=(GoogleWebView)vi;
        rhoelementsGetConfig=web.getConfig();
        if(rhoelementsGetConfig.getString(WebViewConfig.DISABLE_SCANNER_ON_NAVIGATION) != null){
        	disable_scanner_val = Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.DISABLE_SCANNER_ON_NAVIGATION));
        }

		if(disable_scanner_val == 1){
			BarcodeFactory factory=(BarcodeFactory) BarcodeFactorySingleton.getInstance();
			if(factory!=null)
				factory.disableScannerOnNavigate();
		}
		return super.onNavigateStarted(extManager, url, ext, res);
	}


	
}
