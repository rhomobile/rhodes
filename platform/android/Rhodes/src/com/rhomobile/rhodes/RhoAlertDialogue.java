package com.rhomobile.rhodes;

import android.app.AlertDialog;
import android.content.Context;

public class RhoAlertDialogue {
	static AlertDialog.Builder builder1 = null;
	static AlertDialog alert = null;
	
	public static void createBuilder(Context ctx)
	{
		 builder1=new AlertDialog.Builder(ctx);
		 builder1.setMessage(RhoConf.getString("Message"));
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


