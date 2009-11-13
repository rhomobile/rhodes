package com.rhomobile.rhodes.ui;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.RhodesInstance;

public class LogOptionsDialog extends Dialog implements OnClickListener {

	private Button saveButton;
	private Button closeButton;
	private Spinner logLevel;
	private EditText includeClasses;
	private EditText excludeClasses;
	
	public LogOptionsDialog(Context context) {
		super(context);
	}
	
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);

		getWindow().requestFeature(Window.FEATURE_NO_TITLE);

		setContentView(AndroidR.layout.logoptions);

		saveButton = (Button) findViewById(AndroidR.id.logoptsSaveButton);
		closeButton = (Button) findViewById(AndroidR.id.logoptsCloseButton);

		saveButton.setOnClickListener(this);
		closeButton.setOnClickListener(this);

		includeClasses = (EditText) this.findViewById(AndroidR.id.includeClasses);
		
		includeClasses.setText( RhodesInstance.getLogConf().getEnabledCategories() );
		
		excludeClasses = (EditText) this.findViewById(AndroidR.id.excludeClasses);
		
		excludeClasses.setText( RhodesInstance.getLogConf().getDisabledCategories() );
		
		logLevel = (Spinner) this.findViewById(AndroidR.id.loglevel);

		ArrayAdapter<String> spinnerArrayAdapter = new ArrayAdapter<String>(this.getContext(),
				android.R.layout.simple_spinner_item,
	            new String[] { "Trace","Info","Warning","Error" });
		
		logLevel.setAdapter(spinnerArrayAdapter);
		
		logLevel.setSelection( RhodesInstance.getLogConf().getMinSeverity() );
	}

	public void onClick(View view) {
		switch (view.getId()) {
		case AndroidR.id.logoptsSaveButton:
			RhodesInstance.getLogConf().setMinSeverity(logLevel.getSelectedItemPosition());
			RhodesInstance.getLogConf().setEnabledCategories(includeClasses.getText().toString());
			RhodesInstance.getLogConf().setDisabledCategories(excludeClasses.getText().toString());
			
			RhodesInstance.getLogConf().saveToFile();
			
			dismiss();
			break;
		case AndroidR.id.logoptsCloseButton:
			cancel();
			break;
		}
	}
}
