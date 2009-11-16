package j2me.sql;

import java.util.Calendar;

public class Timestamp extends java.util.Date
{
    private int nanos;
    
    public Timestamp(int year, int month, int date,
            int hour, int minute, int second, int nano) {
        Calendar myCal = Calendar.getInstance();
        myCal.set(java.util.Calendar.YEAR, year);
        myCal.set(java.util.Calendar.MONTH, month);
        myCal.set(java.util.Calendar.DATE, date);
        myCal.set(java.util.Calendar.HOUR_OF_DAY, hour);
        myCal.set(java.util.Calendar.MINUTE, minute);
        myCal.set(java.util.Calendar.SECOND, second);
        this.setTime(myCal.getTime().getTime());
	if (nano > 999999999 || nano < 0)
	    throw new IllegalArgumentException("nanos > 999999999 or < 0");
	nanos = nano;
    }
    
    public Timestamp(long time) {
	super(time / 1000L * 1000L);
	nanos = (int) (time % 1000L * 1000000L);
	if (nanos < 0) {
	    nanos = 1000000000 + nanos;
	    this.setTime((time / 1000L - 1L) * 1000L);
	}
    }
    
    public static Timestamp valueOf(String s) {
        String date_s;
	String time_s;
	String nanos_s;
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int a_nanos = 0;
	int firstDash;
	int secondDash;
	int dividingSpace;
	int firstColon = 0;
	int secondColon = 0;
	int period = 0;
	String formatError = "Timestamp format must be yyyy-mm-dd hh:mm:ss.fffffffff";
	String zeros = "000000000";

	if (s == null) throw new java.lang.IllegalArgumentException("null string");

	// Split the string into date and time components
	s = s.trim();
	dividingSpace = s.indexOf(' ');
	if (dividingSpace > 0) {
	    date_s = s.substring(0,dividingSpace);
	    time_s = s.substring(dividingSpace+1);
	} else {
	    throw new java.lang.IllegalArgumentException(formatError);
	}


	// Parse the date
	firstDash = date_s.indexOf('-');
	secondDash = date_s.indexOf('-', firstDash+1);

	// Parse the time
	if (time_s == null) 
	    throw new java.lang.IllegalArgumentException(formatError);
	firstColon = time_s.indexOf(':');
	secondColon = time_s.indexOf(':', firstColon+1);
	period = time_s.indexOf('.', secondColon+1);

	// Convert the date
	if ((firstDash > 0) & (secondDash > 0) & 
	    (secondDash < date_s.length()-1)) {
	    year = Integer.parseInt(date_s.substring(0, firstDash)) - 1900;
	    month = 
		Integer.parseInt(date_s.substring
				 (firstDash+1, secondDash)) - 1;
	    day = Integer.parseInt(date_s.substring(secondDash+1));
	} else {		
	    throw new java.lang.IllegalArgumentException(formatError);
	}

	// Convert the time; default missing nanos
	if ((firstColon > 0) & (secondColon > 0) & 
	    (secondColon < time_s.length()-1)) {
	    hour = Integer.parseInt(time_s.substring(0, firstColon));
	    minute = 
		Integer.parseInt(time_s.substring(firstColon+1, secondColon));
	    if ((period > 0) & (period < time_s.length()-1)) {
		second = 
		    Integer.parseInt(time_s.substring(secondColon+1, period));
		nanos_s = time_s.substring(period+1);
		if (nanos_s.length() > 9) 
		    throw new java.lang.IllegalArgumentException(formatError);
		if (!Character.isDigit(nanos_s.charAt(0)))
		    throw new java.lang.IllegalArgumentException(formatError);
		nanos_s = nanos_s + zeros.substring(0,9-nanos_s.length());
		a_nanos = Integer.parseInt(nanos_s);
	    } else if (period > 0) {
		throw new java.lang.IllegalArgumentException(formatError);
	    } else {
		second = Integer.parseInt(time_s.substring(secondColon+1));
	    }
	} else {
	    throw new java.lang.IllegalArgumentException();
	}

	return new Timestamp(year, month, day, hour, minute, second, a_nanos);
    }
    
    public String toString() {
        Calendar myCal = Calendar.getInstance();
        myCal.setTime(this);
        int year = myCal.get(java.util.Calendar.YEAR) + 1900;
        int month = myCal.get(java.util.Calendar.MONTH) + 1;
        int date = myCal.get(java.util.Calendar.DATE);
        int hour = myCal.get(java.util.Calendar.HOUR_OF_DAY);
        int minute = myCal.get(java.util.Calendar.MINUTE);
        int second = myCal.get(java.util.Calendar.SECOND);
	String string = "000000000";
	String year_s = "" + year;
	String month_s;
	if (month < 10)
	    month_s = "0" + month;
	else
	    month_s = Integer.toString(month);
	String date_s;
	if (date < 10)
	    date_s = "0" + date;
	else
	    date_s = Integer.toString(date);
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
	String nano_s;
	if (nanos == 0)
	    nano_s = "0";
	else {
	    nano_s = Integer.toString(nanos);
	    nano_s
		= string.substring(0, 9 - nano_s.length()) + nano_s;
	    char[] nanosChar = new char[nano_s.length()];
	    nano_s.getChars(0, nano_s.length(), nanosChar, 0);
	    int truncIndex = 8;
	    while (nanosChar[truncIndex] == '0') {
		truncIndex--;
	    }
	    nano_s = new String(nanosChar, 0, truncIndex + 1);
	}
	return (year_s + "-" + month_s + "-" + date_s + " "
		+ hour_s + ":" + minute_s + ":" + second_s + "."
		+ nano_s);
    }
    
    public int getNanos() {
	return nanos;
    }
    
    public void setNanos(int n) {
	if (n > 999999999 || n < 0)
	    throw new IllegalArgumentException("nanos > 999999999 or < 0");
	nanos = n;
    }
    
    public boolean equals(Timestamp ts) {
	if (super.equals(ts)) {
	    if (nanos == ts.nanos)
		return true;
	    return false;
	}
	return false;
    }
    
    public boolean equals(Object ts) {
	if (ts instanceof Timestamp)
	    return equals((Timestamp) ts);
	return false;
    }
    
    public boolean before(Timestamp ts) {
        Calendar myself = Calendar.getInstance();
        myself.setTime(this);
        Calendar when = Calendar.getInstance();
        when.setTime(ts);
        if (myself.before(when))
	    return true;
        if (myself.equals(when)) {
	    if (nanos < ts.nanos)
		return true;
	    return false;
	}
	return false;
    }
    
    public boolean after(Timestamp ts) {
        Calendar myself = Calendar.getInstance();
        myself.setTime(this);
        Calendar when = Calendar.getInstance();
        when.setTime(ts);
        if (myself.after(when))
	    return true;
        if (myself.equals(when)) {
	    if (nanos > ts.nanos)
		return true;
	    return false;
	}
	return false;
    }
}
