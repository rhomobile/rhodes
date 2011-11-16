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

package com.rhomobile.rhodes.event;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.http.impl.cookie.DateUtils;

import android.text.format.Time;

import com.rhomobile.rhodes.Logger;

public class Event {
    private final static String TAG = Event.class.getSimpleName();
	
	public static final String FREQUENCY_DAILY = "daily";
	public static final String FREQUENCY_WEEKLY = "weekly";
	public static final String FREQUENCY_MONTHLY = "monthly";
	public static final String FREQUENCY_YEARLY = "yearly";
	
//    private static final Pattern DATE_PATTERN_RRULE = Pattern.compile(
//            "(\\d{4,})(\\d\\d)(\\d\\d)(?:T([0-1]\\d|2[0-3])([0-5]\\d)([0-5]\\d)(Z)?)?");

    private static final Pattern TIME_PATTERN_DURATION = Pattern.compile(
            "^P(?:(?:\\d+?)D)(?:T(?:(?:\\d+?)H)(?:(?:\\d+?)M)(?:(?:\\d+?)S))$");
    private static final int DURATION_DAYS = 1;
    private static final int DURATION_HOURS = 4;
    private static final int DURATION_MINUTES = 6;
    private static final int DURATION_SECONDS = 8;
    
    private static final int MINUTE_SEC = 60;
    private static final int HOUR_SEC = 3600;
    private static final int DAY_SEC = 3600 * 24;

    private static final String RRULE_FREQ = "FREQ";
    private static final String RRULE_INTERVAL = "INTERVAL";
    private static final String RRULE_UNTIL = "UNTIL";
    private static final String RRULE_COUNT = "COUNT";

    private static final String FORMAT_RRULE = "yyyyMMdd'T'HHmmss";

    public Event(String eid) {
		id = eid;
	}
	
	public String id;
	public String title;
	public Date startDate;
	public Date endDate;
	public Date lastModified;
	public String location;
	public String notes;
	public String privacy;
	public String frequency;
	public int interval;
	public Date recurrenceEnd;
	public int recurrenceTimes;
	
    public void parseDuration(String str) {
        if (str == null || str.length() == 0) {
            endDate = null;
            return;
        }

        try {
            Matcher match = TIME_PATTERN_DURATION.matcher(str);
            String strDays = match.group(DURATION_DAYS);
            String strHours = match.group(DURATION_HOURS);
            String strMinutes = match.group(DURATION_MINUTES);
            String strSeconds = match.group(DURATION_SECONDS);
            
            long days = 0;
            long hours = 0;
            long minutes = 0;
            long seconds = 0;
            
            if (strDays != null) {
                days = Long.parseLong(strDays);
            }
            if (strHours != null) {
                hours = Long.parseLong(strHours);
            }
            if (strMinutes != null) {
                minutes = Long.parseLong(strMinutes);
            }
            if (strSeconds != null) {
                seconds = Long.parseLong(strSeconds);
            }
            endDate = new Date(startDate.getTime() + (days * DAY_SEC + hours * HOUR_SEC + minutes * MINUTE_SEC + seconds) * 1000);
        } catch (IllegalStateException e) {
            Logger.T(TAG, e);
            endDate = null;
        }
    }
    public void parseRrule(String rrule) {
        if (rrule == null || rrule.length() == 0) {
            frequency = null;
            interval = 0;
            recurrenceTimes = 0;
            recurrenceEnd = null;
            return;
        }

        try {
            String[] rparams = rrule.split(";");

            for (String rparam: rparams) {
                String[] nameVal = rparam.split("=");
                if (0 == nameVal[0].compareToIgnoreCase(RRULE_FREQ)) {
                    frequency = nameVal[1].toLowerCase();
                    continue;
                }
                if (0 == nameVal[0].compareToIgnoreCase(RRULE_INTERVAL)) {
                    interval = Integer.parseInt(nameVal[1]);
                }
                if (0 == nameVal[0].compareToIgnoreCase(RRULE_COUNT)) {
                    recurrenceTimes = Integer.parseInt(nameVal[1]);
                }
                else if (0 == nameVal[0].compareToIgnoreCase(RRULE_UNTIL)) {
                    Logger.D(TAG, "Parsing 'until' date: " + nameVal[1]);
                    recurrenceEnd = DateUtils.parseDate(nameVal[1], new String[] {FORMAT_RRULE});
                    Logger.D(TAG, "Parsed 'until' date: " + recurrenceEnd.toString());
                }
            }
        } catch (Exception ex) {
            Logger.E(TAG, ex);
            frequency = null;
            interval = 0;
            recurrenceTimes = 0;
            recurrenceEnd = null;
        }
    }

    //Format event duration according to RFC2445 duration specification
    public String getDuration() {
        if (endDate == null || startDate == null) {
            return null;
        }
        StringBuilder duration = new StringBuilder("P");

        long sec = (endDate.getTime() - startDate.getTime()) / 1000;

        long mins = sec / 60;
        sec = sec % 60;
        
        long hours = mins / 60;
        mins = mins % 60;
        
        long days = hours / 24;
        hours = hours % 24;
        
        if (days != 0) {
            duration.append(days).append('D');
        }
        
        if (hours != 0 || mins != 0 || sec != 0) {
            duration.append('T');
            if (hours != 0) {
                duration.append(hours).append('H');
            }
            if (mins != 0) {
                duration.append(mins).append('M');
            }
            if (sec != 0) {
                duration.append(sec).append('S');
            }
        }
        
        return duration.toString();
    }
    
    private Time getRecurrenceEnd() {
        Time until = new Time();
        //Calendar calendar = new GregorianCalendar();
        //calendar.setTime(recurrenceEnd);
        //int monthDay = calendar.get(Calendar.DAY_OF_MONTH);
        //int month = calendar.get(Calendar.MONTH);
        //int year = calendar.get(Calendar.YEAR);
        //calendar.setTime(endDate);
        //int second = calendar.get(Calendar.SECOND);
        //int minute = calendar.get(Calendar.MINUTE);
        //int hour = calendar.get(Calendar.HOUR);
        //until.set(second, minute, hour, monthDay, month, year);
        //until.second++;
        //until.normalize(false);
        until.set(recurrenceEnd.getTime());
        until.switchTimezone(Time.TIMEZONE_UTC);
        return until;
    }
    
    public String getRrule() {
        if (frequency == null || frequency.length() == 0) {
            return null;
        }
        StringBuilder rrule = new StringBuilder(RRULE_FREQ);
        rrule.append("=").append(frequency.toUpperCase());
        if (null != recurrenceEnd) {
            rrule.append(';').append(RRULE_UNTIL).append('=').append(getRecurrenceEnd().format2445());
        } else if (0 < recurrenceTimes) {
            rrule.append(';').append(RRULE_COUNT).append('=').append(recurrenceTimes);
        }
        if (0 < interval) {
            rrule.append(';').append(RRULE_INTERVAL).append('=').append(Integer.toString(interval));
        }
        return rrule.toString();
    }

    public String toString() {
        StringBuilder buffer = new StringBuilder(Event.class.getSimpleName());
        buffer.append(" { ");
        buffer.append("id: ").append(id)
              .append(", title: ").append(title);
        if (startDate != null)
          buffer.append(", begin: ").append(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss z").format(startDate));
        else
            buffer.append(", begin: null");
        if (endDate != null)
          buffer.append(", end: ").append(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss z").format(endDate));
        else
            buffer.append(", end: null");
        buffer.append(", freq: ").append(frequency)
              .append(", interval: ").append(Integer.toString(interval));
        if (recurrenceEnd != null)
          buffer.append(", recurrence end: ").append(recurrenceEnd);
        else
            buffer.append(", recurrence end: null");
        buffer.append(", recurrence count: ").append(recurrenceTimes);
        buffer.append(" }");
        return buffer.toString();
    }
}

