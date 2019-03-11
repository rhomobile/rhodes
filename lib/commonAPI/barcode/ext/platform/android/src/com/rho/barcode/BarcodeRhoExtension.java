package com.rho.barcode;

import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.webview.GoogleWebView;
import com.rhomobile.rhodes.webview.WebViewConfig;
import com.rhomobile.rhodes.Logger;

import android.net.Uri;

public class BarcodeRhoExtension extends AbstractRhoExtension{

	private final static String TAG = "BarcodeRhoExtension";

	public boolean onBeforeNavigate(IRhoExtManager extManager, String url, IRhoWebView ext, boolean res) {
		int disable_scanner_val = 0;
		IRhoConfig rhoelementsGetConfig= null;
        IRhoWebView vi=RhodesActivity.safeGetInstance().getMainView().getWebView(0);
        
        GoogleWebView web=null;

        try {
        	web=(GoogleWebView)vi;
        } catch ( ClassCastException e ) {
        	web=null;
        }

        if ( web != null ) {
	        rhoelementsGetConfig=web.getConfig();
	        if(rhoelementsGetConfig.getString(WebViewConfig.DISABLE_SCANNER_ON_NAVIGATION) != null){
	        	disable_scanner_val = Integer.parseInt(rhoelementsGetConfig.getString(WebViewConfig.DISABLE_SCANNER_ON_NAVIGATION));
	        }
	    }

		if(disable_scanner_val == 1){
			BarcodeFactory factory=(BarcodeFactory) BarcodeFactorySingleton.getInstance();
			if(factory!=null) {

				Logger.D( TAG, "Will check for url before disabling scanner on navigate: " + url );

				boolean doDisable = isControllerURL(url);

				if (doDisable) {
					Logger.D( TAG, "Will disable active scanner as navigating to other page.");
					factory.disableScannerOnNavigate();
				} else {
					Logger.D( TAG, "Won't disable active scaner as target URL didn't met required contitions.");
				}
			}
				
		}

		//return res;
		return super.onBeforeNavigate(extManager, url, ext, res);
	}
	
	//this will determine if requested URL belongs to controller part of the app.
	//we need to only apply disable scanner policy when really changing the page.
	private static boolean isControllerURL( String url ) {

		try {
			Uri uri = Uri.parse(url);
			String protocol = uri.getScheme();
			if ( null == protocol ) {
				protocol = "";
			}
			protocol = protocol.toLowerCase();

			String host = uri.getHost();
			if ( null == host ) {
				host = "";
			}
			host = host.toLowerCase();

			String path = uri.getPath();
			if ( null == path ) {
				path = "";
			}
			path = path.toLowerCase();

			boolean validProtocol = protocol.isEmpty() || protocol.equals("http") || protocol.equals( "https");
			boolean validHost = host.isEmpty() || host.equals("localhost") || host.equals("127.0.0.1");
			boolean validPath = path.startsWith("/app/");

			Logger.D( TAG, "isControllerURL: " + 
				"proto:" + protocol + ":" + String.valueOf(validProtocol) +
				" host:" + host + ":" + String.valueOf(validHost) +
				" path:" + path + ":" + String.valueOf(validPath)
			);

			return validProtocol && validHost && validPath;
		}
		catch( Throwable e ) {
			Logger.W(TAG, "Error parsing pre-navigate URL" );
			e.printStackTrace();
		}

		return false;
	}


	
}
