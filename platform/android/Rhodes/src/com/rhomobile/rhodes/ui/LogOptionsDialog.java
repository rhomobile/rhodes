/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/
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

		includeClasses = (EditText) this.findViewById(AndroidR.id.includeClasses);

		includeClasses.setText( RhoLogConf.getEnabledCategories() );

		excludeClasses = (EditText) this.findViewById(AndroidR.id.excludeClasses);

		excludeClasses.setText( RhoLogConf.getDisabledCategories() );

		logLevel = (Spinner) this.findViewById(AndroidR.id.loglevel);

		ArrayAdapter<String> spinnerArrayAdapter = new ArrayAdapter<String>(this.getContext(),
				android.R.layout.simple_spinner_item,
	            new String[] { "Trace","Info","Warning","Error" });

		logLevel.setAdapter(spinnerArrayAdapter);

		logLevel.setSelection( RhoLogConf.getMinSeverity() );
	}

	public void onClick(View view) {
		switch (view.getId()) {
		case AndroidR.id.logoptsSaveButton:
			RhoLogConf.setMinSeverity(logLevel.getSelectedItemPosition());
			RhoLogConf.setEnabledCategories(includeClasses.getText().toString());
			RhoLogConf.setDisabledCategories(excludeClasses.getText().toString());

			RhoLogConf.saveToFile();

			dismiss();
			break;
		case AndroidR.id.logoptsCloseButton:
			cancel();
			break;
		}
	}
}
