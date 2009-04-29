/*
 * @(#)Time.java	1.32 04/05/18
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
 * <P>A thin wrapper around the <code>java.util.Date</code> class that allows the JDBC
 * API to identify this as an SQL <code>TIME</code> value. The <code>Time</code>
 * class adds formatting and
 * parsing operations to support the JDBC escape syntax for time
 * values. 
 * <p>The date components should be set to the "zero epoch"
 * value of January 1, 1970 and should not be accessed. 
 */
public class Time extends java.util.Date
{
    /**
     * Constructs a <code>Time</code> object initialized with the 
     * given values for the hour, minute, and second.
     * The driver sets the date components to January 1, 1970.
     * Any method that attempts to access the date components of a
     * <code>Time</code> object will throw a
     * <code>java.lang.IllegalArgumentException</code>.
     * <P>
     * The result is undefined if a given argument is out of bounds.
     *
     * @param hour 0 to 23
     * @param minute 0 to 59
     * @param second 0 to 59
     *
     * @deprecated Use the constructor that takes a milliseconds value
     *             in place of this constructor
     */
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
    
    /**
     * Constructs a <code>Time</code> object using a milliseconds time value.
     *
     * @param time milliseconds since January 1, 1970, 00:00:00 GMT;
     *             a negative number is milliseconds before
     *               January 1, 1970, 00:00:00 GMT
     */
    public Time(long time) {
	super(time);
    }
    
    /**
     * Sets a <code>Time</code> object using a milliseconds time value.
     *
     * @param time milliseconds since January 1, 1970, 00:00:00 GMT;
     *             a negative number is milliseconds before
     *               January 1, 1970, 00:00:00 GMT
     */
    public void setTime(long time) {
	super.setTime(time);
    }
    
    /**
     * Converts a string in JDBC time escape format to a <code>Time</code> value.
     *
     * @param s time in format "hh:mm:ss"
     * @return a corresponding <code>Time</code> object
     */
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
    
    /**
     * Formats a time in JDBC time escape format.  
     *
     * @return a <code>String</code> in hh:mm:ss format
     */
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

   /**
    * This method is deprecated and should not be used because SQL <code>TIME</code> 
    * values do not have a year component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this
    *           method is invoked
    * @see #setYear
    */
    public int getYear() {
	throw new IllegalArgumentException();
    }
    
   /**
    * This method is deprecated and should not be used because SQL <code>TIME</code> 
    * values do not have a month component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this
    *           method is invoked
    * @see #setMonth
    */
    public int getMonth() {
	throw new IllegalArgumentException();
    }
    
   /**
    * This method is deprecated and should not be used because SQL <code>TIME</code> 
    * values do not have a day component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this
    *           method is invoked
    */
    public int getDay() {
	throw new IllegalArgumentException();
    }
    
   /**
    * This method is deprecated and should not be used because SQL <code>TIME</code> 
    * values do not have a date component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this
    *           method is invoked
    * @see #setDate
    */
    public int getDate() {
	throw new IllegalArgumentException();
    }
    
   /**
    * This method is deprecated and should not be used because SQL <code>TIME</code> 
    * values do not have a year component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this
    *           method is invoked
    * @see #getYear
    */
    public void setYear(int i) {
	throw new IllegalArgumentException();
    }
    
   /**
    * This method is deprecated and should not be used because SQL <code>TIME</code> 
    * values do not have a month component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this
    *           method is invoked
    * @see #getMonth
    */
    public void setMonth(int i) {
	throw new IllegalArgumentException();
    }
    
   /**
    * This method is deprecated and should not be used because SQL <code>TIME</code> 
    * values do not have a date component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this
    *           method is invoked
    * @see #getDate
    */
    public void setDate(int i) {
	throw new IllegalArgumentException();
    }
}

