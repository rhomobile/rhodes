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
package com.rhomobile.rhodes.datetime;

import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoActivity;
import com.rhomobile.rhodes.RhodesService;

import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.TimePicker;

public class DateTimePickerScreen extends RhoActivity {
	
	private static final String TAG = "DateTimePicker";
	
	private static final String INTENT_EXTRA_PREFIX = DateTimePicker.INTENT_EXTRA_PREFIX;
	
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
		
		getWindow().setFlags(RhodesService.WINDOW_FLAGS, RhodesService.WINDOW_MASK);
		setContentView(AndroidR.layout.datetime);
		
		Bundle extras = this.getIntent().getExtras();
		
		_callback = extras.getString(INTENT_EXTRA_PREFIX + "callback");
		
		long milliseconds = extras.getLong(INTENT_EXTRA_PREFIX + "init")*((long)1000);
		
		_init = new Date(milliseconds);

		Calendar c = new GregorianCalendar();
		c.setTimeInMillis(milliseconds);

		int year = c.get(Calendar.YEAR);
		int month = c.get(Calendar.MONTH);
		int day = c.get(Calendar.DAY_OF_MONTH);
		int hours = c.get(Calendar.HOUR_OF_DAY);
		int minutes = c.get(Calendar.MINUTE);
		int seconds = c.get(Calendar.SECOND);
		
		_init.setYear(year);
		_init.setMonth(month);
		_init.setDate(day);
		_init.setHours(hours);
		_init.setMinutes(minutes);
		
		
		
		_fmt = extras.getInt(INTENT_EXTRA_PREFIX + "fmt");
		_opaque = extras.getByteArray(INTENT_EXTRA_PREFIX + "opaque");
		
		this.setTitle(extras.getString(INTENT_EXTRA_PREFIX + "title"));
		
		_datePicker = (DatePicker)findViewById(AndroidR.id.datePicker);
		_timePicker = (TimePicker)findViewById(AndroidR.id.timePicker);
		_okButton = (Button)findViewById(AndroidR.id.okButton);
		_cancelButton = (Button)findViewById(AndroidR.id.cancelButton);
		
		_okButton.setOnClickListener(mOkListener);
		_cancelButton.setOnClickListener(mCancelListener);
		
		_datePicker.init(year, month, day, null);
		_timePicker.setCurrentHour(hours);
		_timePicker.setCurrentMinute(minutes);
		
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
		long res = result == null ? 0 : result.getTime()/((long)1000);
		Logger.D(TAG, "Return result: " + res);
		DateTimePicker.callback(callback, res, opaque, result == null);
		finish();
	}
}
