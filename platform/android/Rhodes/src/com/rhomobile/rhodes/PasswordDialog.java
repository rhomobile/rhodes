package com.rhomobile.rhodes;


import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.text.InputType;
import android.widget.EditText;

public class PasswordDialog {
	
	static AlertDialog.Builder builder=null;
	static AlertDialog mainAlert=null;
	static AlertDialog.Builder wrongPassword=null;
	static AlertDialog wrongPwdAlert=null;
	
	private static void createDialogForWrongPaswaord()
	{
		wrongPassword= new AlertDialog.Builder(RhodesActivity.getContext());
		wrongPassword.setCancelable(true);
		wrongPassword.setMessage("Invalid Password")
				.setTitle("Alert");
		wrongPassword.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				dialog.cancel();
			}
		});
		wrongPwdAlert = wrongPassword.create();
	}
	
	private static void callExitNow()
	{
		RhodesService.PerformRealExit();
	}
	private static void createMainPopUp()
	{
		// Display Pop up for password...
				 
				final Context ctx = RhodesActivity.getContext();
				builder = new AlertDialog.Builder(ctx);
				builder.setTitle("Enter Password");

				// Set up the input
				final EditText input = new EditText(ctx);
				// Specify the type of input expected; this, for example, sets the input as a password, and will mask the text
				input.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);
				builder.setView(input);

				// Set up the buttons
				builder.setPositiveButton("OK", new DialogInterface.OnClickListener() { 
					@Override
					public void onClick(DialogInterface dialog, int which) {
						RhodesService.m_Text = input.getText().toString();
						if(RhodesService.m_Text.equals(RhodesService.getExitPasswordValue())){ 
				
							callExitNow();															
						
						}
						else
						{ 
							WrongPasswordAlertShow();
							
						}
					}
				});
				builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						dialog.cancel();
					}
				});
				
				mainAlert=builder.create();
	}
	private static void WrongPasswordAlertShow()
	{
		PerformOnUiThread.exec(new Runnable(){

			@Override
			public void run() {
				wrongPwdAlert.show();
				
			}
			
		});
		
	}
	public static void  createDialog()
	{
		createDialogForWrongPaswaord();
		createMainPopUp();
		
	}
	public static void show()
	{
		mainAlert.show();
	}
}
