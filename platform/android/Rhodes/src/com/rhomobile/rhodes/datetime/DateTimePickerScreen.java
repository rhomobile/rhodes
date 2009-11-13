package com.rhomobile.rhodes.datetime;

import java.util.Date;

import com.rhomobile.rhodes.AndroidR;

import android.app.Activity;
import android.app.DatePickerDialog;
import android.app.Dialog;
import android.app.TimePickerDialog;
import android.app.DatePickerDialog.OnDateSetListener;
import android.app.TimePickerDialog.OnTimeSetListener;
import android.content.DialogInterface;
import android.content.DialogInterface.OnDismissListener;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.LinearLayout;
import android.widget.TimePicker;

public class DateTimePickerScreen extends Activity {
	
	private static final int DIALOG_DATE_PICKER_ID = 0;
	private static final int DIALOG_TIME_PICKER_ID = 1;
	
	private DateTimePickerScreen _dateTimePicker = this;
	
	private String _callback;
	private Date _init;
	private int _fmt;
	private String _opaque;
	
	private DatePicker _datePicker;
	private TimePicker _timePicker;
	private Button _okButton;
	private Button _cancelButton;
	
	private static final int FORMAT_DATE_TIME = 0;
	private static final int FORMAT_DATE = 1;
	private static final int FORMAT_TIME = 2;
	
	private OnClickListener mOkListener = new OnClickListener() {
		public void onClick(View arg0) {
			_init.setYear(_datePicker.getYear() - 1900);
			_init.setMonth(_datePicker.getMonth());
			_init.setDate(_datePicker.getDayOfMonth());
			_init.setHours(_timePicker.getCurrentHour());
			_init.setMinutes(_timePicker.getCurrentMinute());
			sendResult(_callback, _init);
		}
	};
	
	private OnClickListener mCancelListener = new OnClickListener() {
		public void onClick(View v) {
			sendResult(_callback, null);
		}
	};
	
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		Log.d(getClass().getSimpleName(), "onCreate");
		setContentView(AndroidR.layout.datetime);
		
		Bundle extras = this.getIntent().getExtras();
		
		_callback = extras.getString("callback");
		_init = new Date(extras.getLong("init"));
		_fmt = extras.getInt("fmt");
		_opaque = extras.getString("opaque");
		
		this.setTitle(extras.getString("title"));
		
		_datePicker = (DatePicker)findViewById(AndroidR.id.datePicker);
		_timePicker = (TimePicker)findViewById(AndroidR.id.timePicker);
		_okButton = (Button)findViewById(AndroidR.id.okButton);
		_cancelButton = (Button)findViewById(AndroidR.id.cancelButton);
		
		_okButton.setOnClickListener(mOkListener);
		_cancelButton.setOnClickListener(mCancelListener);
		
		_datePicker.init(_init.getYear() + 1900, _init.getMonth(), _init.getDay(), null);
		_timePicker.setCurrentHour(_init.getHours());
		_timePicker.setCurrentMinute(_init.getMinutes());
		
		switch (_fmt) {
		case FORMAT_DATE:
			_timePicker.setVisibility(View.INVISIBLE);
			break;
		case FORMAT_TIME:
			_datePicker.setVisibility(View.INVISIBLE);
			break;
		}
	}
	
	private void setFieldsEnabled(boolean v) {
		_datePicker.setEnabled(v);
		_timePicker.setEnabled(v);
		_okButton.setEnabled(v);
		_cancelButton.setEnabled(v);
	}
	
	private class ResultSender implements Runnable {
		
		private String _callback;
		private Date _result;
		
		public ResultSender(String callback, Date result) {
			_callback = callback;
			_result = result;
		}
		
		public void run() {
			/*
			IHttpConnection connection = null;
			
			String fullUrl = RhodesInstance.getInstance().
				getCurrentUrl().replaceAll("\\\\", "/");

			String[] paths = _callback.replaceAll("\\\\", "/").split("/");

			for (int i = paths.length - 1; i >= 0; i--) {
				System.out.println(fullUrl);

				if (!paths[i].equals("")) {
					int pos = fullUrl.lastIndexOf("/" + paths[i]);

					if (pos != -1) {
						fullUrl = fullUrl.substring(0, pos);
					}
				}
			}

			if (fullUrl.endsWith("/"))
				fullUrl = fullUrl.substring(0, fullUrl.length() - 2);

			if (_callback.startsWith("/"))
				fullUrl += _callback;
			else
				fullUrl += "/" + _callback;

			System.out.println(fullUrl);

			HttpHeader headers = new HttpHeader();
			headers.setHeader("Content-Type", "application/x-www-form-urlencoded");
			
			String body;
			if (_result == null)
				body = "status=cancel";
			else {
				body = "status=ok&result=" + _result.getTime()/1000;
				if (_opaque != null)
					body += "&opaque=" + _opaque;
			}
			
			try {
				connection = AndroidHttpConnection.makeConnection(fullUrl, headers, body.getBytes());
				int code = connection.getResponseCode();
				if (code != IHttpConnection.HTTP_OK) {
					System.out.println("Error posting data: " + code);
				}
			} catch (Exception e) {
				System.out.println("Error posting data: " + e.getMessage());
			} finally {
				if (connection != null)
					try {
						connection.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				finish();
			}
			*/
		}
	};
	
	private void sendResult(String callback, Date result) {
		this.setFieldsEnabled(false);
		new Thread(new ResultSender(callback, result)).start();
	}
}
