package com.rhomobile.rhodes.datetime;

import java.util.Date;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.Rhodes;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.TimePicker;

public class DateTimePickerScreen extends Activity {
	
	private static final String TAG = "DateTimePicker";
	
	private String _callback;
	private Date _init;
	private int _fmt;
	private byte[] _opaque;
	
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
			sendResult(_callback, _init, _opaque);
		}
	};
	
	private OnClickListener mCancelListener = new OnClickListener() {
		public void onClick(View v) {
			sendResult(_callback, null, _opaque);
		}
	};
	
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		Logger.D(TAG, "onCreate");
		
		getWindow().setFlags(Rhodes.WINDOW_FLAGS, Rhodes.WINDOW_MASK);
		setContentView(AndroidR.layout.datetime);
		
		Bundle extras = this.getIntent().getExtras();
		
		_callback = extras.getString("callback");
		_init = new Date(extras.getLong("init")*1000);
		_fmt = extras.getInt("fmt");
		_opaque = extras.getByteArray("opaque");
		
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
		case FORMAT_DATE_TIME:
			break;
		}
	}
	
	private void setFieldsEnabled(boolean v) {
		_datePicker.setEnabled(v);
		_timePicker.setEnabled(v);
		_okButton.setEnabled(v);
		_cancelButton.setEnabled(v);
	}
	
	private void sendResult(String callback, Date result, byte[] opaque) {
		this.setFieldsEnabled(false);
		long res = result == null ? 0 : result.getTime()/1000;
		Logger.D(TAG, "Return result: " + res);
		DateTimePicker.callback(callback, res, opaque, result == null);
		finish();
	}
}
