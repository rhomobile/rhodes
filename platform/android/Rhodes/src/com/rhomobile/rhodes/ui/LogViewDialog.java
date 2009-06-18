package com.rhomobile.rhodes.ui;

import com.rho.RhoLogger;
import com.rhomobile.rhodes.R;

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
		closeButton = (Button) findViewById(R.id.logviewCloseButton);

		refreshButton.setOnClickListener(this);
		clearButton.setOnClickListener(this);
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
			RhoLogger.clearLog();
			loadLogText();
			break;
		case R.id.logviewCloseButton:
			cancel();
			break;
		}
	}

	private void loadLogText(){
        String strLog = RhoLogger.getLogText();
        findCurLine(strLog);
        
        logContent.setText(strLog);
	}
	
	void findCurLine(String strLog){
        int nPos = RhoLogger.getLogTextPos();
        curLine = 0;
        
        for ( int nEndLine = strLog.indexOf('\n'); nEndLine >= 0 && nEndLine<nPos; 
        		nEndLine = strLog.indexOf('\n', nEndLine+1) ) 
        	curLine++;
        
        if (curLine>0)
        	curLine--;
	}

}
