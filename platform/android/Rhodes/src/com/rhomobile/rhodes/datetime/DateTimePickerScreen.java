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

import java.io.IOException;
import java.io.Serializable;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.Vector;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoActivity;
import com.rhomobile.rhodes.RhodesService;


import android.content.res.Configuration;
import android.graphics.PointF;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.TimePicker;

public class DateTimePickerScreen extends RhoActivity {
	
	private static final String TAG = "DateTimePicker";
	
	private static final String INTENT_EXTRA_PREFIX = DateTimePicker.INTENT_EXTRA_PREFIX;

	private String _title;
	private long _initial_time;
	private long _saved_time;
	private String _callback;
	private Calendar _init;
	private int _fmt;
	private byte[] _opaque;
	
	private DatePicker _datePicker;
	private TimePicker _timePicker;
	private Button _okButton;
	private Button _cancelButton;
	
	private static final int FORMAT_DATE_TIME = 0;
	private static final int FORMAT_DATE = 1;
	private static final int FORMAT_TIME = 2;
	
	private long _min_time;
	private long _max_time;
	private Calendar _min_Date = null;
	private Calendar _max_Date = null;
	
	
	public static String mStateID = "DateTimePickerScreenViewState";

	public class DateTimePickerScreenViewState extends Object implements Serializable {
		
		private String title;
		private long initial_time;
		private long saved_time;
		private String callback;
		private int fmt;
		private byte[] opaque;
		private long min_time;
		private long max_time;
		
		

		private void writeObject(java.io.ObjectOutputStream out) throws IOException {
	        out.writeUTF(callback);
			out.writeLong(initial_time);
			out.writeLong(saved_time);
			out.writeInt(fmt);
	        out.writeUTF(title);
			out.writeLong(min_time);
			out.writeLong(max_time);
			if (opaque != null) {
				out.writeInt(opaque.length);
				out.write(opaque);
			}
			else {
				out.writeInt(-1);
			}
		}
		
		private void readObject(java.io.ObjectInputStream in) throws IOException, ClassNotFoundException {
	        callback = in.readUTF();
	        initial_time = in.readLong();
	        saved_time = in.readLong();
	        fmt = in.readInt();
	        title = in.readUTF();
	        min_time = in.readLong();
	        max_time = in.readLong();
	        int size = in.readInt();
			if (size != -1) {
				opaque = new byte[size];
				in.read(opaque, 0, size);
			}
			else {
				opaque = null;
			}
		}
		
		public void save(DateTimePickerScreen p) {
			title = p._title;
			initial_time = p._initial_time;
			saved_time = p._saved_time;
			callback = p._callback;
			fmt = p._fmt;
			opaque = p._opaque;
			min_time = p._min_time;
			max_time = p._max_time;
			
		}
		public void restore(DateTimePickerScreen p) {
			p._title = title;
			p._initial_time = initial_time;
			p._saved_time = saved_time;
			p._callback = callback;
			p._fmt = fmt;
			p._opaque = opaque;
			p._min_time = min_time;
			p._max_time = max_time;
		}
	}
	
	
	
	
	
	
	
	
	
	private OnClickListener mOkListener = new OnClickListener() {
		public void onClick(View arg0) {
			_init.set(Calendar.YEAR, _datePicker.getYear());
			_init.set(Calendar.MONTH, _datePicker.getMonth());
			_init.set(Calendar.DAY_OF_MONTH, _datePicker.getDayOfMonth());
			
			_init.set(Calendar.HOUR_OF_DAY, _timePicker.getCurrentHour());
			_init.set(Calendar.MINUTE, _timePicker.getCurrentMinute());
			_init.set(Calendar.SECOND, 0);
			_init.set(Calendar.MILLISECOND, 0);
			sendResult(_callback, _init, _opaque);
		}
	};
	
	private OnClickListener mCancelListener = new OnClickListener() {
		public void onClick(View v) {
			sendResult(_callback, null, _opaque);
		}
	};

/*	
	private static void convertMillisecondsToDate(long millis, Date date, boolean only_date) {
		Calendar c = new GregorianCalendar();
		c.setTimeInMillis(millis);

		//date.setTime(0);
		
		int year = c.get(Calendar.YEAR);
		int month = c.get(Calendar.MONTH);
		int day = c.get(Calendar.DAY_OF_MONTH);
		int hours = c.get(Calendar.HOUR_OF_DAY);
		int minutes = c.get(Calendar.MINUTE);
		int seconds = c.get(Calendar.SECOND);
		
		date.setYear(year);
		date.setMonth(month);
		date.setDate(day);
		if (only_date) {
			date.setHours(0);
			date.setMinutes(0);
			date.setSeconds(0);
		}
		else {
			date.setHours(hours);
			date.setMinutes(minutes);
			date.setSeconds(seconds);
		}
	}
*/	
	
	
	
	private void initialization() {
		_init = new GregorianCalendar();

		_init.setTimeInMillis(_saved_time);
		
		if (_min_time != 0) {
			_min_Date = new GregorianCalendar();
			_min_Date.setTimeInMillis(_min_time);
			_min_Date.set(Calendar.HOUR_OF_DAY, 0);
			_min_Date.set(Calendar.MINUTE, 0);
			_min_Date.set(Calendar.SECOND, 0);
			_min_Date.set(Calendar.MILLISECOND, 0);
		}

		if (_max_time != 0) {
			_max_Date = new GregorianCalendar();
			_max_Date.setTimeInMillis(_max_time);
			_max_Date.set(Calendar.HOUR_OF_DAY, 0);
			_max_Date.set(Calendar.MINUTE, 0);
			_max_Date.set(Calendar.SECOND, 0);
			_max_Date.set(Calendar.MILLISECOND, 0);
		}
		
		this.setTitle(_title);
		
		_datePicker = (DatePicker)findViewById(AndroidR.id.datePicker);
		_timePicker = (TimePicker)findViewById(AndroidR.id.timePicker);
		_okButton = (Button)findViewById(AndroidR.id.okButton);
		_cancelButton = (Button)findViewById(AndroidR.id.cancelButton);
		
		_okButton.setOnClickListener(mOkListener);
		_cancelButton.setOnClickListener(mCancelListener);
		
		_datePicker.init(_init.get(Calendar.YEAR), _init.get(Calendar.MONTH), _init.get(Calendar.DAY_OF_MONTH), new DatePicker.OnDateChangedListener() {

				public void	onDateChanged(DatePicker view, int year, int monthOfYear, int dayOfMonth) {
					//Logger.D(TAG, "onDateChanged( year="+String.valueOf(year)+", month="+String.valueOf(monthOfYear)+", day="+dayOfMonth);
					Calendar new_date = new GregorianCalendar();
					Calendar set_to = null;

					new_date.set(Calendar.YEAR, year);
					new_date.set(Calendar.MONTH, monthOfYear);
					new_date.set(Calendar.DAY_OF_MONTH, dayOfMonth);
					new_date.set(Calendar.HOUR_OF_DAY, 0);
					new_date.set(Calendar.MINUTE, 0);
					new_date.set(Calendar.SECOND, 0);
					new_date.set(Calendar.MILLISECOND, 0);

					//Logger.D(TAG, "            new_date: year="+String.valueOf(new_date.getYear())+", month="+String.valueOf(new_date.getMonth())+", day="+new_date.getDay());
					
					if (_min_time != 0) {
						//Logger.D(TAG, "            min_date: year="+String.valueOf(_min_Date.getYear())+", month="+String.valueOf(_min_Date.getMonth())+", day="+_min_Date.getDay());
						if (new_date.before(_min_Date)) {
							set_to = _min_Date;
						}
					}
					if (_max_time != 0) {
						//Logger.D(TAG, "            max_date: year="+String.valueOf(_max_Date.getYear())+", month="+String.valueOf(_max_Date.getMonth())+", day="+_max_Date.getDay());
						if (new_date.after(_max_Date)) {
							set_to = _max_Date;
						}
					}
					if (set_to != null) {
						view.updateDate(set_to.get(Calendar.YEAR), set_to.get(Calendar.MONTH), set_to.get(Calendar.DAY_OF_MONTH));
					}
					{
						Calendar s_date = new GregorianCalendar();
						s_date.setTimeInMillis(_saved_time);
						if (set_to != null) {
							s_date.set(Calendar.YEAR, set_to.get(Calendar.YEAR));
							s_date.set(Calendar.MONTH, set_to.get(Calendar.MONTH));
							s_date.set(Calendar.DAY_OF_MONTH, set_to.get(Calendar.DAY_OF_MONTH));
						}
						else {
							s_date.set(Calendar.YEAR, year);
							s_date.set(Calendar.MONTH, monthOfYear);
							s_date.set(Calendar.DAY_OF_MONTH, dayOfMonth);
						}
						_saved_time = s_date.getTimeInMillis();
					}
				}
			}
		);
		
		_timePicker.setCurrentHour(_init.get(Calendar.HOUR_OF_DAY));
		_timePicker.setCurrentMinute(_init.get(Calendar.MINUTE));
		_timePicker.setOnTimeChangedListener( new TimePicker.OnTimeChangedListener() {
				public void onTimeChanged (TimePicker view, int hourOfDay, int minute) {
					{
						Calendar s_date = new GregorianCalendar();
						s_date.setTimeInMillis(_saved_time);
						s_date.set(Calendar.HOUR_OF_DAY, hourOfDay);
						s_date.set(Calendar.MINUTE, minute);
						s_date.set(Calendar.SECOND, 0);
						s_date.set(Calendar.MILLISECOND, 0);
						_saved_time = s_date.getTimeInMillis();
					}
				}
			}
		);

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
	
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		Logger.D(TAG, "onCreate");
		
		getWindow().setFlags(RhodesService.WINDOW_FLAGS, RhodesService.WINDOW_MASK);
		setContentView(AndroidR.layout.datetime);
		
		Bundle extras = this.getIntent().getExtras();
		
		if (extras.containsKey(INTENT_EXTRA_PREFIX + "callback")) 
			_callback = extras.getString(INTENT_EXTRA_PREFIX + "callback");
		
		if (extras.containsKey(INTENT_EXTRA_PREFIX + "init")) { 
			_initial_time = extras.getLong(INTENT_EXTRA_PREFIX + "init")*((long)1000);
			_saved_time = _initial_time;		
		}
		
		if (extras.containsKey(INTENT_EXTRA_PREFIX + "fmt")) 
			_fmt = extras.getInt(INTENT_EXTRA_PREFIX + "fmt");
		if (extras.containsKey(INTENT_EXTRA_PREFIX + "opaque")) 
			_opaque = extras.getByteArray(INTENT_EXTRA_PREFIX + "opaque");
		
		if (extras.containsKey(INTENT_EXTRA_PREFIX + "min_time")) 
			_min_time = extras.getLong(INTENT_EXTRA_PREFIX + "min_time")*((long)1000);
		if (extras.containsKey(INTENT_EXTRA_PREFIX + "max_time")) 
			_max_time = extras.getLong(INTENT_EXTRA_PREFIX + "max_time")*((long)1000);
		
		
		if (extras.containsKey(INTENT_EXTRA_PREFIX + "title")) 
			_title = extras.getString(INTENT_EXTRA_PREFIX + "title");

		initialization();
	}
	
	private void setFieldsEnabled(boolean v) {
		_datePicker.setEnabled(v);
		_timePicker.setEnabled(v);
		_okButton.setEnabled(v);
		_cancelButton.setEnabled(v);
	}
	
	private void sendResult(String callback, Calendar result, byte[] opaque) {
		this.setFieldsEnabled(false);
		long res = result == null ? 0 : result.getTimeInMillis()/((long)1000);
		Logger.D(TAG, "Return result: " + res);
		DateTimePicker.callback(callback, res, opaque, result == null);
		finish();
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}
	
	public void onRestoreInstanceState(Bundle savedInstanceState) {
		DateTimePickerScreenViewState s = (DateTimePickerScreenViewState)savedInstanceState.getSerializable(mStateID);
		s.restore(this);
		initialization();
	}

	public void onSaveInstanceState(Bundle outState) {
		DateTimePickerScreenViewState s = new DateTimePickerScreenViewState();
		s.save(this);
		outState.putSerializable(mStateID, s);
	}
	
	
}
