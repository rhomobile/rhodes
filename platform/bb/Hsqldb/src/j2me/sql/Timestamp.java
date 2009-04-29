/*
 * @(#)Timestamp.java	1.58 04/05/18
 *
 * Copyright 2004 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/*
 * Modified by Jérôme Paschoud to compile on J2ME CLDC
 */

package j2me.sql;

import java.util.Calendar;

/**
 * <P>A thin wrapper around <code>java.util.Date</code> that allows
 * the JDBC API to identify this as an SQL <code>TIMESTAMP</code> value.
 * It adds the ability
 * to hold the SQL <code>TIMESTAMP</code> nanos value and provides formatting and
 * parsing operations to support the JDBC escape syntax for timestamp values.
 *
 * <P><B>Note:</B> This type is a composite of a <code>java.util.Date</code> and a
 * separate nanoseconds value. Only integral seconds are stored in the
 * <code>java.util.Date</code> component. The fractional seconds - the nanos - are
 * separate.  The <code>Timestamp.equals(Object)</code> method never returns 
 * <code>true</code> when passed a value of type <code>java.util.Date</code>
 * because the nanos component of a date is unknown.
 * As a result, the <code>Timestamp.equals(Object)</code>
 * method is not symmetric with respect to the
 * <code>java.util.Date.equals(Object)</code>
 * method.  Also, the <code>hashcode</code> method uses the underlying 
 * <code>java.util.Date</code> 
 * implementation and therefore does not include nanos in its computation.  
 * <P>
 * Due to the differences between the <code>Timestamp</code> class
 * and the <code>java.util.Date</code>
 * class mentioned above, it is recommended that code not view
 * <code>Timestamp</code> values generically as an instance of
 * <code>java.util.Date</code>.  The
 * inheritance relationship between <code>Timestamp</code>
 * and <code>java.util.Date</code> really 
 * denotes implementation inheritance, and not type inheritance.  
 */
public class Timestamp extends java.util.Date
{
    /**
     * @serial
     */
    private int nanos;
    
    /**
     * Constructs a <code>Timestamp</code> object initialized
     * with the given values.
     *
     * @param year the year minus 1900
     * @param month 0 to 11 
     * @param date 1 to 31
     * @param hour 0 to 23
     * @param minute 0 to 59
     * @param second 0 to 59
     * @param nano 0 to 999,999,999
     * @deprecated instead use the constructor <code>Timestamp(long millis)</code>
     * @exception IllegalArgumentException if the nano argument is out of bounds
     */
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
    
    /**
     * Constructs a <code>Timestamp</code> object 
     * using a milliseconds time value. The
     * integral seconds are stored in the underlying date value; the
     * fractional seconds are stored in the <code>nanos</code> field of
     * the <code>Timestamp</code> object.
     *
     * @param time milliseconds since January 1, 1970, 00:00:00 GMT.
     *        A negative number is the number of milliseconds before
     *         January 1, 1970, 00:00:00 GMT.
     * @see java.util.Calendar for more information
     */
    public Timestamp(long time) {
	super(time / 1000L * 1000L);
	nanos = (int) (time % 1000L * 1000000L);
	if (nanos < 0) {
	    nanos = 1000000000 + nanos;
	    this.setTime((time / 1000L - 1L) * 1000L);
	}
    }
    
    /**
     * Converts a <code>String</code> object in JDBC timestamp escape format to a
     * <code>Timestamp</code> value.
     *
     * @param s timestamp in format <code>yyyy-mm-dd hh:mm:ss.fffffffff</code>
     * @return corresponding <code>Timestamp</code> value
     * @exception java.lang.IllegalArgumentException if the given argument
     * does not have the format <code>yyyy-mm-dd hh:mm:ss.fffffffff</code>
     */
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
    
    /**
     * Formats a timestamp in JDBC timestamp escape format.
     *         <code>yyyy-mm-dd hh:mm:ss.fffffffff</code>,
     * where <code>ffffffffff</code> indicates nanoseconds.
     * <P>
     * NOTE: To specify a timestamp for the class 
     * <code>java.text.SimpleDateFormat</code>, use "yyyy.MM.dd" rather than
     * "yyyy-mm-dd". In the context of <code>java.text.SimpleDateFormat</code>,
     * "mm" indicates minutes rather than the month. Note that 
     * <code>java.text.SimpleDateFormat</code> does not allow for the
     * nanoseconds component of a <code>Timestamp</code> object.
     * For Example:
     * <PRE>
     *
     * Format Pattern				Result
     * --------------				------
     * "yyyy.MM.dd G 'at' hh:mm:ss z"	-->	2002.07.10 AD at 15:08:56 PDT
     *
     * </PRE>
     * @return a <code>String</code> object in
     *           <code>yyyy-mm-dd hh:mm:ss.fffffffff</code> format
     */
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
    
    /**
     * Gets this <code>Timestamp</code> object's <code>nanos</code> value.
     *
     * @return this <code>Timestamp</code> object's fractional seconds component
     * @see #setNanos
     */
    public int getNanos() {
	return nanos;
    }
    
    /**
     * Sets this <code>Timestamp</code> object's <code>nanos</code> field
     * to the given value.
     *
     * @param n the new fractional seconds component
     * @exception java.lang.IllegalArgumentException if the given argument
     *            is greater than 999999999 or less than 0
     * @see #getNanos
     */
    public void setNanos(int n) {
	if (n > 999999999 || n < 0)
	    throw new IllegalArgumentException("nanos > 999999999 or < 0");
	nanos = n;
    }
    
    /**
     * Tests to see if this <code>Timestamp</code> object is
     * equal to the given <code>Timestamp</code> object.
     *
     * @param ts the <code>Timestamp</code> value to compare with
     * @return <code>true</code> if the given <code>Timestamp</code>
     *         object is equal to this <code>Timestamp</code> object;
     *         <code>false</code> otherwise
     */
    public boolean equals(Timestamp ts) {
	if (super.equals(ts)) {
	    if (nanos == ts.nanos)
		return true;
	    return false;
	}
	return false;
    }
    
    /**
     * Tests to see if this <code>Timestamp</code> object is
     * equal to the given object.
     *
     * This version of the method <code>equals</code> has been added
     * to fix the incorrect 
     * signature of <code>Timestamp.equals(Timestamp)</code> and to preserve backward 
     * compatibility with existing class files.
     *
     * Note: This method is not symmetric with respect to the 
     * <code>equals(Object)</code> method in the base class.
     *
     * @param ts the <code>Object</code> value to compare with
     * @return <code>true</code> if the given <code>Object</code>
     *         instance is equal to this <code>Timestamp</code> object;
     *         <code>false</code> otherwise
     */
    public boolean equals(Object ts) {
	if (ts instanceof Timestamp)
	    return equals((Timestamp) ts);
	return false;
    }
    
    /**
     * Indicates whether this <code>Timestamp</code> object is
     * earlier than the given <code>Timestamp</code> object.
     *
     * @param ts the <code>Timestamp</code> value to compare with
     * @return <code>true</code> if this <code>Timestamp</code> object is earlier;
     *        <code>false</code> otherwise
     */
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
    
    /**
     * Indicates whether this <code>Timestamp</code> object is
     * later than the given <code>Timestamp</code> object.
     *
     * @param ts the <code>Timestamp</code> value to compare with
     * @return <code>true</code> if this <code>Timestamp</code> object is later;
     *        <code>false</code> otherwise
     */
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
