/*
 * @(#)Date.java	1.33 04/05/18
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
 * <P>A thin wrapper around a millisecond value that allows
 * JDBC to identify this as an SQL <code>DATE</code> value.  A 
 * milliseconds value represents the number of milliseconds that 
 * have passed since January 1, 1970 00:00:00.000 GMT.
 * <p>
 * To conform with the definition of SQL <code>DATE</code>, the 
 * millisecond values wrapped by a <code>java.sql.Date</code> instance 
 * must be 'normalized' by setting the 
 * hours, minutes, seconds, and milliseconds to zero in the particular
 * time zone with which the instance is associated.
 */
public class Date extends java.util.Date
{
    /**
     * Constructs a <code>Date</code> object initialized with the given
     * year, month, and day.
     * <P>
     * The result is undefined if a given argument is out of bounds.
     *
     * @param year the year minus 1900; must be 0 to 8099. (Note that
     *        8099 is 9999 minus 1900.)
     * @param month 0 to 11
     * @param day 1 to 31
     * @deprecated instead use the constructor <code>Date(long date)</code>
     */
    public Date(int year, int month, int day) {
        super();
        Calendar myCal = Calendar.getInstance();
        myCal.set(java.util.Calendar.YEAR,year);
        myCal.set(java.util.Calendar.MONTH,month);
        myCal.set(java.util.Calendar.DATE,day);
        this.setTime(myCal.getTime().getTime());
    }
    
    /**
     * Constructs a <code>Date</code> object using the given milliseconds 
     * time value.  If the given milliseconds value contains time 
     * information, the driver will set the time components to the
     * time in the default time zone (the time zone of the Java virtual
     * machine running the application) that corresponds to zero GMT.
     *
     * @param date milliseconds since January 1, 1970, 00:00:00 GMT not
     *        to exceed the milliseconds representation for the year 8099.
     *        A negative number indicates the number of milliseconds
     *        before January 1, 1970, 00:00:00 GMT.
     */
    public Date(long date) {
        // If the millisecond date value contains time info, mask it out.
	super(date);
    }
    
    /**
     * Sets an existing <code>Date</code> object 
     * using the given milliseconds time value. 
     * If the given milliseconds value contains time information, 
     * the driver will set the time components to the
     * time in the default time zone (the time zone of the Java virtual
     * machine running the application) that corresponds to zero GMT.
     *
     * @param date milliseconds since January 1, 1970, 00:00:00 GMT not
     *        to exceed the milliseconds representation for the year 8099.
     *        A negative number indicates the number of milliseconds
     *        before January 1, 1970, 00:00:00 GMT.
     */
    public void setTime(long date) {
        // If the millisecond date value contains time info, mask it out.
	super.setTime(date);
    }
    
    /**
     * Converts a string in JDBC date escape format to
     * a <code>Date</code> value.
     *
     * @param s a <code>String</code> object representing a date in 
     *        in the format "yyyy-mm-dd"
     * @return a <code>java.sql.Date</code> object representing the
     *         given date
     * @throws IllegalArgumentException if the date given is not in the
     *         JDBC date escape format (yyyy-mm-dd)
     */
    public static Date valueOf(String s) {
        int year;
	int month;
	int day;
	int firstDash;
	int secondDash;
        
	if (s == null) throw new IllegalArgumentException();

        firstDash = s.indexOf('-');
	secondDash = s.indexOf('-', firstDash+1);
	if ((firstDash > 0) & (secondDash > 0) & (secondDash < s.length()-1)) {
	    year = Integer.parseInt(s.substring(0, firstDash)) - 1900;
	    month = Integer.parseInt(s.substring(firstDash+1, secondDash)) - 1;
	    day = Integer.parseInt(s.substring(secondDash+1));	 
	} else {
	    throw new java.lang.IllegalArgumentException();
	}
			
	return new Date(year, month, day);
    }
    
    /**
     * Formats a date in the date escape format yyyy-mm-dd.  
     * <P>
     * NOTE:  To specify a date format for the class
     * <code>SimpleDateFormat</code>, use "yyyy.MM.dd" rather than
     * "yyyy-mm-dd".  In the context of <code>SimpleDateFormat</code>,
     * "mm" indicates minutes rather than the month.  
     * For example:
     * <PRE>
     *
     *  Format Pattern                         Result
     *  --------------                         -------
     *	"yyyy.MM.dd G 'at' hh:mm:ss z"    ->>  1996.07.10 AD at 15:08:56 PDT
     * </PRE>
     * @return a String in yyyy-mm-dd format
     */
    public String toString() {
        Calendar myCal = Calendar.getInstance();
        myCal.setTime(this);
        int year = myCal.get(java.util.Calendar.YEAR) + 1900;
        int month = myCal.get(java.util.Calendar.MONTH) + 1;
        int date = myCal.get(java.util.Calendar.DATE);
	String year_s = Integer.toString(year);
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
	return year_s + "-" + month_s + "-" + date_s;
    }
    
   // Override all the time operations inherited from java.util.Date;

   /**
    * This method is deprecated and should not be used because SQL Date 
    * values do not have a time component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this method is invoked
    * @see #setHours
    */
    public int getHours() {
	throw new IllegalArgumentException();
    }
    
   /**
    * This method is deprecated and should not be used because SQL Date 
    * values do not have a time component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this method is invoked
    * @see #setMinutes
    */
    public int getMinutes() {
	throw new IllegalArgumentException();
    }
    
   /**
    * This method is deprecated and should not be used because SQL Date 
    * values do not have a time component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this method is invoked
    * @see #setSeconds
    */
    public int getSeconds() {
	throw new IllegalArgumentException();
    }
    
   /**
    * This method is deprecated and should not be used because SQL Date 
    * values do not have a time component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this method is invoked
    * @see #getHours
    */
    public void setHours(int i) {
	throw new IllegalArgumentException();
    }
    
   /**
    * This method is deprecated and should not be used because SQL Date 
    * values do not have a time component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this method is invoked
    * @see #getMinutes
    */
    public void setMinutes(int i) {
	throw new IllegalArgumentException();
    }
    
   /**
    * This method is deprecated and should not be used because SQL Date 
    * values do not have a time component.
    *
    * @deprecated
    * @exception java.lang.IllegalArgumentException if this method is invoked
    * @see #getSeconds
    */
    public void setSeconds(int i) {
	throw new IllegalArgumentException();
    }
}
