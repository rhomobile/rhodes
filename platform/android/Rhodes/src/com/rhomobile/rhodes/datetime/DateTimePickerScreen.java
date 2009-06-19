package com.rhomobile.rhodes.datetime;

import java.io.IOException;
import java.util.Date;

import com.rho.net.IHttpConnection;
import com.rhomobile.rhodes.AndroidHttpConnection;
import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.RhodesInstance;
import com.rhomobile.rhodes.http.HttpHeader;

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
import android.widget.DatePicker;
import android.widget.TimePicker;

public class DateTimePickerScreen extends Activity {
	
	private static final int DIALOG_DATE_PICKER_ID = 0;
	private static final int DIALOG_TIME_PICKER_ID = 1;
	
	private DateTimePickerScreen _dateTimePicker = this;
	
	private String _callback;
	private Date _init;
	private int _fmt;
	
	private static final int FORMAT_DATE_TIME = 0;
	private static final int FORMAT_DATE = 1;
	private static final int FORMAT_TIME = 2;
	
	private static final int EVENT_NONE = 0;
	private static final int EVENT_START = 1;
	private static final int EVENT_DATE_SET = 2;
	private static final int EVENT_DATE_CANCELLED = 3;
	private static final int EVENT_TIME_SET = 4;
	private static final int EVENT_TIME_CANCELLED = 5;
	
	private int currentEvent = EVENT_NONE;
	private void next(int event) {
		currentEvent = event;
		switch(event) {
		case EVENT_START:
			switch(_fmt) {
			case FORMAT_DATE_TIME:
			case FORMAT_DATE:
				showDialog(DIALOG_DATE_PICKER_ID);
				break;
			case FORMAT_TIME:
				showDialog(DIALOG_TIME_PICKER_ID);
				break;
			}
			break;
		case EVENT_DATE_SET:
			if(_fmt != FORMAT_DATE)
				showDialog(DIALOG_TIME_PICKER_ID);
			break;
		case EVENT_DATE_CANCELLED:
		case EVENT_TIME_CANCELLED:
			sendResult(_callback, null);
			break;
		case EVENT_TIME_SET:
			sendResult(_callback, _init);
		}
	}
	
	private OnDateSetListener mDateSetListener = new OnDateSetListener() {
		public void onDateSet(DatePicker view, int year, int monthOfYear,
				int dayOfMonth) {
			Log.d(getClass().getSimpleName(), "onDateSet");
			_init.setYear(year - 1900);
			_init.setMonth(monthOfYear);
			_init.setDate(dayOfMonth);
			_dateTimePicker.next(EVENT_DATE_SET);
		}
	};
	
	private OnDismissListener mDateDismissListener = new OnDismissListener() {
		public void onDismiss(DialogInterface arg0) {
			Log.d(getClass().getSimpleName(), "onDismiss");
			if(_dateTimePicker.currentEvent != EVENT_DATE_SET)
				_dateTimePicker.next(EVENT_DATE_CANCELLED);
		}
	};
	
	private OnTimeSetListener mTimeSetListener = new OnTimeSetListener() {
		public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
			Log.d(getClass().getSimpleName(), "onTimeSet");
			_init.setHours(hourOfDay);
			_init.setMinutes(minute);
			_dateTimePicker.next(EVENT_TIME_SET);
		}
	};
	
	private OnDismissListener mTimeDismissListener = new OnDismissListener() {
		public void onDismiss(DialogInterface dialog) {
			// TODO Auto-generated method stub
			Log.d(getClass().getSimpleName(), "onDismiss");
			if(_dateTimePicker.currentEvent != EVENT_TIME_SET)
				_dateTimePicker.next(EVENT_TIME_CANCELLED);
		}
	};
	
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		Log.d(getClass().getSimpleName(), "onCreate");
		setContentView(R.layout.dialog_activity);
		
		Bundle extras = this.getIntent().getExtras();
		
		_callback = extras.getString("callback");
		_init = new Date(extras.getLong("init"));
		_fmt = extras.getInt("fmt");
		
		this.setTitle(extras.getString("title"));
		
		next(EVENT_START);
	}
	
	@Override
	protected Dialog onCreateDialog(int id) {
		Dialog dialog;
		OnDismissListener dls; 
		switch(id) {
		case DIALOG_DATE_PICKER_ID:
			dialog = new DatePickerDialog(this, mDateSetListener,
					_init.getYear() + 1900, _init.getMonth(), _init.getDate());
			dls = mDateDismissListener;
			break;
		case DIALOG_TIME_PICKER_ID:
			dialog = new TimePickerDialog(this, mTimeSetListener,
					_init.getHours(), _init.getMinutes(), true);
			dls = mTimeDismissListener;
			break;
		default:
			return null;
		}
		
		dialog.setOnDismissListener(dls);
		return dialog;
	}
	
	private class ResultSender implements Runnable {
		
		private String _callback;
		private Date _result;
		
		public ResultSender(String callback, Date result) {
			_callback = callback;
			_result = result;
		}
		
		public void run() {
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
			
			String body = _result == null ? "status=cancel" : "status=ok&result=" + _result.getTime()/1000;
			
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
			}
		}
	};
	
	private void sendResult(String callback, Date result) {
		new Thread(new ResultSender(callback, result)).start();
		finish();
	}
}
