package com.rho.connectionchecking;

import com.rhomobile.rhodes.RhoConf;

import android.app.AlertDialog;
import android.content.Context;

public class ConnectionCheckingDialogue {
	static AlertDialog.Builder builder1 = null;
	static AlertDialog alert = null;
	
	public static void createBuilder(Context ctx)
	{
		 builder1=new AlertDialog.Builder(ctx);
		 builder1.setMessage(ConnectionCheckingSingleton.MESSAGE);
		 builder1.setCancelable(false);
		 alert=builder1.create();
		}
	public static void show()
	{
		alert.show();
	}
	public static void hide()
	{
		alert.hide();
	}

}


