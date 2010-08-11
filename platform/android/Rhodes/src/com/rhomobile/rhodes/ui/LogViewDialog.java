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

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.RhodesService;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class LogViewDialog extends Dialog implements OnClickListener {

	public LogViewDialog(Context context) {
		super(context);
	}

	private Button refreshButton;
	private Button clearButton;
	private Button sendButton;
	private Button closeButton;
	private TextView logContent;
	private int curLine = 0;
	
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);

		getWindow().requestFeature(Window.FEATURE_NO_TITLE);

		setContentView(AndroidR.layout.logview);
		refreshButton = (Button) findViewById(AndroidR.id.logviewRefreshButton);
		clearButton = (Button) findViewById(AndroidR.id.logviewClearButton);
		sendButton = (Button) findViewById(AndroidR.id.logviewSendButton);
		closeButton = (Button) findViewById(AndroidR.id.logviewCloseButton);

		refreshButton.setOnClickListener(this);
		clearButton.setOnClickListener(this);
		sendButton.setOnClickListener(this);
		closeButton.setOnClickListener(this);

		logContent = (TextView) this.findViewById(AndroidR.id.logcontent);
		
		loadLogText();
	}

	public void onClick(View view) {
		switch (view.getId()) {
		case AndroidR.id.logviewRefreshButton:
			loadLogText();
			break;
		case AndroidR.id.logviewClearButton:
			RhodesService.getInstance().getLogConf().clearLog();
			loadLogText();
			break;
		case AndroidR.id.logviewSendButton:
			RhodesService.getInstance().getLogConf().sendLog();
			break;
		case AndroidR.id.logviewCloseButton:
			cancel();
			break;
		}
	}

	private void loadLogText(){
        String strLog = RhodesService.getInstance().getLogConf().getLogText();
        findCurLine(strLog);
        
        logContent.setText(strLog);
	}
	
	void findCurLine(String strLog){
        int nPos = RhodesService.getInstance().getLogConf().getLogTextPos();
        curLine = 0;
        
        for ( int nEndLine = strLog.indexOf('\n'); nEndLine >= 0 && nEndLine<nPos; 
        		nEndLine = strLog.indexOf('\n', nEndLine+1) ) 
        	curLine++;
        
        if (curLine>0)
        	curLine--;
	}

}
