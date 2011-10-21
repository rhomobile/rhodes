/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

package j2me.sql;

import java.util.Calendar;

public class Date extends java.util.Date
{
    public Date(int year, int month, int day) {
        super();
        Calendar myCal = Calendar.getInstance();
        myCal.set(java.util.Calendar.YEAR,year);
        myCal.set(java.util.Calendar.MONTH,month);
        myCal.set(java.util.Calendar.DATE,day);
        this.setTime(myCal.getTime().getTime());
    }

    public Date(long date) {
        // If the millisecond date value contains time info, mask it out.
	super(date);
    }
    public void setTime(long date) {
        // If the millisecond date value contains time info, mask it out.
	super.setTime(date);
    }

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
    public int getHours() {
	throw new IllegalArgumentException();
    }

    public int getMinutes() {
	throw new IllegalArgumentException();
    }

    public int getSeconds() {
	throw new IllegalArgumentException();
    }

    public void setHours(int i) {
	throw new IllegalArgumentException();
    }

    public void setMinutes(int i) {
	throw new IllegalArgumentException();
    }

    public void setSeconds(int i) {
	throw new IllegalArgumentException();
    }
}
