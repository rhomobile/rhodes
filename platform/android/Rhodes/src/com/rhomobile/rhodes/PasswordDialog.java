package com.rhomobile.rhodes;


import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.app.Activity;
import android.R;
import android.app.Dialog;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;


public class PasswordDialog  {
	
	private static EditText        mEditText;
	private static TextView        text;
	private static Button          dialogOKButton;
	private static Button          dialogCancelButton;
	
	public static void createpopup() {
		
		// custom dialog
		final Dialog dialog = new Dialog(RhodesActivity.getContext());
		
		dialog.setContentView(RhoExtManager.getResourceId("layout","password_prompt"));
		dialog.setTitle("Please enter password");
		dialog.setCanceledOnTouchOutside(false);
		
		// set the custom dialog components - text, view and button
		  mEditText = (EditText) dialog.findViewById(RhoExtManager.getResourceId("id","edit_text"));
		  text = (TextView) dialog.findViewById(RhoExtManager.getResourceId("id","text"));
		 dialogOKButton = (Button) dialog.findViewById(RhoExtManager.getResourceId("id","dialogButtonOK"));
		 dialogCancelButton = (Button) dialog.findViewById(RhoExtManager.getResourceId("id","dialogButtoncancel"));
		
		dialogOKButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				checkpassword();
			}
		});
		
		dialogCancelButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				dialog.dismiss();
			}
		});

		dialog.show();
	  }
	
	private static void checkpassword(){
		if(mEditText.getText().toString().equals(RhodesService.getExitPasswordValue()))
			RhodesService.PerformRealExit();
		
		else
			{
			text.setText("Invalid Password");
			mEditText.setText("");
			}
	}	

	 
}    
