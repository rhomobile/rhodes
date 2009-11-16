package j2me.sql;

import java.util.Calendar;

public class Time extends java.util.Date
{
    public Time(int hour, int minute, int second) {
        Calendar myCal = Calendar.getInstance();
        myCal.set(java.util.Calendar.YEAR, 70);
        myCal.set(java.util.Calendar.MONTH, 0);
        myCal.set(java.util.Calendar.DATE, 1);
        myCal.set(java.util.Calendar.HOUR_OF_DAY, hour);
        myCal.set(java.util.Calendar.MINUTE, minute);
        myCal.set(java.util.Calendar.SECOND, second);
        this.setTime(myCal.getTime().getTime());
    }
    
    public Time(long time) {
	super(time);
    }
    
    public void setTime(long time) {
	super.setTime(time);
    }
    
    public static Time valueOf(String s) {
        int hour;
	int minute;
	int second;
	int firstColon;
	int secondColon;

	if (s == null) throw new java.lang.IllegalArgumentException();

	firstColon = s.indexOf(':');
	secondColon = s.indexOf(':', firstColon+1);
	if ((firstColon > 0) & (secondColon > 0) & 
	    (secondColon < s.length()-1)) {
	    hour = Integer.parseInt(s.substring(0, firstColon));
	    minute = 
		Integer.parseInt(s.substring(firstColon+1, secondColon));
	    second = Integer.parseInt(s.substring(secondColon+1));	    
	} else {
	    throw new java.lang.IllegalArgumentException();
	}

	return new Time(hour, minute, second);
    }
    
    public String toString() {
        Calendar myCal = Calendar.getInstance();
        myCal.setTime(this);
        int hour = myCal.get(java.util.Calendar.HOUR_OF_DAY);
        int minute = myCal.get(java.util.Calendar.MINUTE);
        int second = myCal.get(java.util.Calendar.SECOND);
	String hour_s;
	if (hour < 10)
	    hour_s = "0" + hour;
	else
	    hour_s = Integer.toString(hour);
	String minute_s;
	if (minute < 10)
	    minute_s = "0" + minute;
	else
	    minute_s = Integer.toString(minute);
	String second_s;
	if (second < 10)
	    second_s = "0" + second;
	else
	    second_s = Integer.toString(second);
	return hour_s + ":" + minute_s + ":" + second_s;
    }
    
   // Override all the date operations inherited from java.util.Date;

    public int getYear() {
	throw new IllegalArgumentException();
    }
    
    public int getMonth() {
	throw new IllegalArgumentException();
    }
    
    public int getDay() {
	throw new IllegalArgumentException();
    }
    
    public int getDate() {
	throw new IllegalArgumentException();
    }
    
    public void setYear(int i) {
	throw new IllegalArgumentException();
    }
    
    public void setMonth(int i) {
	throw new IllegalArgumentException();
    }
    
    public void setDate(int i) {
	throw new IllegalArgumentException();
    }
}

