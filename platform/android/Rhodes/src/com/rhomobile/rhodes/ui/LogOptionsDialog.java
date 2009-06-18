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

import com.rho.RhoLogger;
import com.rhomobile.rhodes.R;

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

		setContentView(R.layout.logoptions);

		saveButton = (Button) findViewById(R.id.logoptsSaveButton);
		closeButton = (Button) findViewById(R.id.logoptsCloseButton);

		saveButton.setOnClickListener(this);
		closeButton.setOnClickListener(this);

		includeClasses = (EditText) this.findViewById(R.id.includeClasses);
		
		includeClasses.setText( RhoLogger.getLogConf().getEnabledCategories() );
		
		excludeClasses = (EditText) this.findViewById(R.id.excludeClasses);
		
		excludeClasses.setText( RhoLogger.getLogConf().getDisabledCategories() );
		
		logLevel = (Spinner) this.findViewById(R.id.loglevel);

		ArrayAdapter<String> spinnerArrayAdapter = new ArrayAdapter<String>(this.getContext(),
				android.R.layout.simple_spinner_item,
	            new String[] { "Trace","Info","Warning","Error" });
		
		logLevel.setAdapter(spinnerArrayAdapter);
		
		logLevel.setSelection( RhoLogger.getLogConf().getMinSeverity() );
	}

	public void onClick(View view) {
		switch (view.getId()) {
		case R.id.logoptsSaveButton:
			RhoLogger.getLogConf().setMinSeverity(logLevel.getSelectedItemPosition());
			RhoLogger.getLogConf().setEnabledCategories(includeClasses.getText().toString());
			RhoLogger.getLogConf().setDisabledCategories(excludeClasses.getText().toString());
			
			RhoLogger.getLogConf().saveToFile();
			
			dismiss();
			break;
		case R.id.logoptsCloseButton:
			cancel();
			break;
		}
	}
}
