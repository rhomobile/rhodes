/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
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
import com.rhomobile.rhodes.RhoLogConf;
import com.rhomobile.rhodes.RhodesService;

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

		RhoLogConf logConf = RhodesService.getInstance().getLogConf();
		
		includeClasses = (EditText) this.findViewById(AndroidR.id.includeClasses);
		
		includeClasses.setText( logConf.getEnabledCategories() );
		
		excludeClasses = (EditText) this.findViewById(AndroidR.id.excludeClasses);
		
		excludeClasses.setText( logConf.getDisabledCategories() );
		
		logLevel = (Spinner) this.findViewById(AndroidR.id.loglevel);

		ArrayAdapter<String> spinnerArrayAdapter = new ArrayAdapter<String>(this.getContext(),
				android.R.layout.simple_spinner_item,
	            new String[] { "Trace","Info","Warning","Error" });
		
		logLevel.setAdapter(spinnerArrayAdapter);
		
		logLevel.setSelection( logConf.getMinSeverity() );
	}

	public void onClick(View view) {
		switch (view.getId()) {
		case AndroidR.id.logoptsSaveButton:
			RhoLogConf logConf = RhodesService.getInstance().getLogConf();
			logConf.setMinSeverity(logLevel.getSelectedItemPosition());
			logConf.setEnabledCategories(includeClasses.getText().toString());
			logConf.setDisabledCategories(excludeClasses.getText().toString());
			
			logConf.saveToFile();
			
			dismiss();
			break;
		case AndroidR.id.logoptsCloseButton:
			cancel();
			break;
		}
	}
}
