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

import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.RhoLogConf;
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

		setContentView(R.layout.logview);
		refreshButton = (Button) findViewById(R.id.logviewRefreshButton);
		clearButton = (Button) findViewById(R.id.logviewClearButton);
		sendButton = (Button) findViewById(R.id.logviewSendButton);
		closeButton = (Button) findViewById(R.id.logviewCloseButton);

		refreshButton.setOnClickListener(this);
		clearButton.setOnClickListener(this);
		sendButton.setOnClickListener(this);
		closeButton.setOnClickListener(this);

		logContent = (TextView) this.findViewById(R.id.logcontent);
		
		loadLogText();
	}
	
	public void onClick(View view) {
		switch (view.getId()) {
		case R.id.logviewRefreshButton:
			loadLogText();
			break;
		case R.id.logviewClearButton:
			RhoLogConf.clearLog();
			loadLogText();
			break;
		case R.id.logviewSendButton:
			RhoLogConf.sendLog();
			break;
		case R.id.logviewCloseButton:
			cancel();
			break;
		}
	}

    private void loadLogText(){
        String strLog;
        try {
            strLog = RhoLogConf.getLogText();
            findCurLine(strLog);
        } catch(Throwable err) {
            strLog = err.getMessage();
            if(strLog == null) {
                strLog = err.toString();
            }
        }
        if (strLog != null) {
            logContent.setText(strLog);
        }
    }

    void findCurLine(String strLog){
        int nPos = RhoLogConf.getLogTextPos();
        curLine = 0;
        
        for ( int nEndLine = strLog.indexOf('\n'); nEndLine >= 0 && nEndLine<nPos; 
        		nEndLine = strLog.indexOf('\n', nEndLine+1) ) 
        	curLine++;
        
        if (curLine>0)
        	curLine--;
	}

}
