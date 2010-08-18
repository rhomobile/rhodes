package com.rho;

import java.util.Hashtable;

public class DateTimeTokenizer {
    protected int year;
    protected int month;
    protected int day;
    protected int hour;
    protected int minute;
    protected int second;
    protected int milli;
    protected int tzoffset;
    protected static int local_tz;

    DateTimeTokenizer(int inp_year, int inp_month, int inp_day, 
               int inp_hour, int inp_minute, int inp_second) {
        if (inp_year < 1583
            || inp_month < 0 || inp_month > 11
            || inp_day < 0 || (inp_day > days_in_month[inp_month] 
                   && !(inp_month == 1 && inp_day == 29 && inp_year % 4 == 0))
            || inp_hour < 0 || inp_hour > 23
            || inp_minute < 0 || inp_minute > 59
            || inp_second < 0 || inp_second > 59) {
            throw new IllegalArgumentException();
        }
        
        year   = inp_year;
        month  = inp_month;
        day    = inp_day;
        hour   = inp_hour;
        minute = inp_minute;
        second = inp_second;
        milli = 0;
    }
    
    DateTimeTokenizer(String s) {
        internalParse(s);
    }
    
    DateTimeTokenizer(String s, boolean iso) {
        if (iso == false) {
            internalParse(s);
        } else {
            internalParseISO(s);
        }
    }
    
    static void setTimeZone(String tz) {
        if (timezones.get(tz) == null) {
            return;
        }
        local_tz = ((Integer)timezones.get(tz)).intValue();
    }
    
    public static long parse(String s) {
        return (new DateTimeTokenizer(s)).getTime();
    }
    
    public static long parseISO(String date) {
        return (new DateTimeTokenizer(date, true)).getTime();
    }

    
    
    private void internalParseISO(String date) {
        int field[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        int field_ptr = 0;
        boolean field_ok;
        
        int c = -1;
        int i = 0;
        int num_dig = 4;
        int n = -1;
        int prevc = 0;

        if (date == null) {
            throw new IllegalArgumentException();
        }

        int limit = date.length();
        while (i < limit) {
            c = date.charAt(i);
            i++;
            if (c == '+' || c == '-' || c == 'Z' || 
                    c == ' ' || c == 'T' || c == ':') {
                prevc = c;
                continue;
            }
            
            if (c < '0' || '9' < c) {
                throw new IllegalArgumentException();
            } else {
                n = c - '0';
                num_dig--;
                while (i < limit && num_dig > 0) {
                    if ('0' <= (c = date.charAt(i)) && c <= '9')  {
                        n = n * 10 + c - '0';
                        i++;
                        num_dig--;
                    } else
                        break;
                }
                num_dig = 2;
                
                field_ok = false;
                switch (field_ptr) {
                    case 0: /* year */
                        if (prevc == 0)
                            field_ok = true;
                        break;
                    case 1: /* month */
                        if ((prevc == '-') || (prevc == 0))
                            field_ok = true;
                        break;
                    case 2: /* day */
                        if ((prevc == '-') || (prevc == 0))
                            field_ok = true;
                        break;
                    case 3: /* hour */
                        if ((prevc == ' ') || (prevc == 'T'))
                            field_ok = true;
                        break;
                    case 4: /* min */
                        if ((prevc == ':') || (prevc == 0))
                            field_ok = true;
                        break;
                    case 5: /* sec */
                        if ((prevc == ':') || (prevc == 0)) {
                            field_ok = true;
                        } else if ((prevc == '+') || (prevc == '-')) {
                            field[field_ptr++] = 0;
                            field[field_ptr++] = (prevc == '-')?-1:+1;
                            field_ok = true;
                        }
                        break;
                    case 7: /* tz_hour */
                        if ((prevc == '+') || (prevc == '-') || (prevc == 0)) {
                            field[field_ptr++] = (prevc == '-')?-1:+1;
                            field_ok = true;
                        }
                        break;
                    case 8: /* tz_min */
                        if ((prevc == ':') || (prevc == 0)) {
                            field_ok = true;
                        }
                        break;
                }
                if (field_ok) {
                    field[field_ptr++] = n;
                } else {
                    throw new IllegalArgumentException();
                }
                prevc = 0;
            }   
        }
    
        if ((field_ptr >= 5) || (prevc == 'Z')) {
            field_ptr = 9;
        }
        if (field_ptr < 8) {
            throw new IllegalArgumentException();
        }

        year = field[0];
        month = field[1]-1;
        day = field[2];
        hour = field[3];
        minute = field[4];
        second = field[5];
        tzoffset = field[6] * ((((field[7]*60) + field[8]) * 60) * 1000);
        milli = 0;
    }

    int getYear() {
        return year;
    }
    
    int getMonth() {
        return month;
    }
    
    int getDay() {
        return day;
    }
    
    int getHour() {
        return hour;
    }
    
    int getMinute() {
        return minute;
    }

    int getSecond() {
        return second;
    }

    long getTime() {
        long julianDay = computeJulianDay(year, month, day);
        long millis = julianDayToMillis(julianDay);
        
        int millisInDay = 0;
        millisInDay += hour;
        millisInDay *= 60;
        millisInDay += minute; // now have minutes
        millisInDay *= 60;
        millisInDay += second; // now have seconds
        millisInDay *= 1000;
        millisInDay += milli; // now have millis
        
        return millis + millisInDay - tzoffset;
    }
    
    private final long computeJulianDay(int inp_year, 
                                        int inp_month, 
                                        int inp_day) {
        int y;
        
        boolean isLeap = inp_year%4 == 0;
        y = inp_year - 1;
        long julianDay = 365L*y + floorDivide(y, 4) + (JAN_1_1_JULIAN_DAY - 3);
        
        isLeap = isLeap && ((inp_year%100 != 0) || (inp_year%400 == 0));
        julianDay += floorDivide(y, 400) - floorDivide(y, 100) + 2;
        julianDay += isLeap ? LEAP_NUM_DAYS[inp_month] : NUM_DAYS[inp_month];
        julianDay += inp_day;

        return julianDay;
    }

    private static final long floorDivide(long numerator, long denominator) {
        return (numerator >= 0) ?
            numerator / denominator :
            ((numerator + 1) / denominator) - 1;
    }
    
    private long julianDayToMillis(long julian) {
        return (julian - julianDayOffset) * millisPerDay;
    }
    
    private void internalParse(String s) {
        int inp_year = -1;
        int mon = -1;
        int mday = -1;
        int inp_hour = -1;
        int min = -1;
        int sec = -1;
        int c = -1;
        int i = 0;
        int n = -1;
        int inp_tzoffset = -1;
        int prevc = 0;

        syntax: {
            if (s == null)
                break syntax;
            int limit = s.length();
            while (i < limit) {
                c = s.charAt(i);
                i++;
                if (c <= ' ' || c == ',')
                    continue;
                if (c == '(') { // skip comments
                    int depth = 1;
                    while (i < limit) {
                        c = s.charAt(i);
                        i++;
                        if (c == '(') depth++;
                        else if (c == ')')
                            if (--depth <= 0)
                                break;
                    }
                    continue;
                }
                if ('0' <= c && c <= '9') {
                    n = c - '0';
                    while (i < limit && '0' <= (c = s.charAt(i)) && c <= '9') {
                        n = n * 10 + c - '0';
                        i++;
                    }
                    if (prevc == '+' || prevc == '-' && inp_year >= 0) {
                        // timezone offset
                        if (n < 24)
                            n = n * 60; // EG. "GMT-3"
                        else
                            n = n % 100 + n / 100 * 60; // eg "GMT-0430"
                        if (prevc == '+')       // plus means east of GMT
                            n = -n;
                        if (inp_tzoffset != 0 && inp_tzoffset != -1)
                            break syntax;
                        inp_tzoffset = n;
                    } else if (n >= 70)
                        if (inp_year >= 0)
                            break syntax;
                        else if (c <= ' ' || c == ',' || c == '/' || i >= limit)
                            // year = n < 1900 ? n : n - 1900;
                            inp_year = n < 100 ? n + 1900 : n;
                        else
                            break syntax;
                    else if (c == ':')
                        if (inp_hour < 0)
                            inp_hour = (byte) n;
                        else if (min < 0)
                            min = (byte) n;
                        else
                            break syntax;
                    else if (c == '/')
                        if (mon < 0)
                            mon = (byte) (n - 1);
                        else if (mday < 0)
                            mday = (byte) n;
                        else
                            break syntax;
                    else if (i < limit && c != ',' && c > ' ' && c != '-')
                        break syntax;
                    else if (inp_hour >= 0 && min < 0)
                        min = (byte) n;
                    else if (min >= 0 && sec < 0)
                        sec = (byte) n;
                    else if (mday < 0)
                        mday = (byte) n;
                    else
                        break syntax;
                    prevc = 0;
                } else if (c == '/' || c == ':' || c == '+' || c == '-')
                    prevc = c;
                else {
                    int st = i - 1;
                    while (i < limit) {
                        c = s.charAt(i);
                        if (!('A' <= c && c <= 'Z' || 'a' <= c && c <= 'z'))
                            break;
                        i++;
                    }
                    if (i <= st + 1)
                        break syntax;
                    int k;
                    for (k = wtb.length; --k >= 0; )
                        if (wtb[k].regionMatches(true, 0, s, st, i - st)) {
                            int action = ttb[k];
                            if (action != 0) {
                                if (action == 1) {      // pm
                                    if (inp_hour > 12 || inp_hour < 1)
                                        break syntax;
                                    else if (inp_hour < 12)
                                        inp_hour += 12;
                                } else if (action == 14) {  // am
                                    if (inp_hour > 12 || inp_hour < 1)
                                        break syntax;
                                    else if (inp_hour == 12)
                                        inp_hour = 0;
                                } else if (action <= 13) {      // month!
                                    if (mon < 0)
                                        mon = (byte) (action - 2);
                                    else
                                        break syntax;
                                } else {
                                    inp_tzoffset = action - 10000;
                                }
                            }
                            break;
                        }
                    if (k < 0)
                        break syntax;
                    prevc = 0;
                }
            }
            if (inp_year < 1583 || mon < 0 || mday < 0)
                break syntax;
            if (sec < 0)
                sec = 0;
            if (min < 0)
                min = 0;
            if (inp_hour < 0)
                inp_hour = 0;
            
            year = inp_year;
            month = mon;
            day = mday;
            hour = inp_hour;
            tzoffset = -inp_tzoffset * 60 * 1000;
            minute = min;
            second = sec;
            milli = 0;
            return;
        }
        // syntax error
        throw new IllegalArgumentException();
    }
    
    private static Hashtable timezones;

    private int[] days_in_month = {31, 28, 31, 30, 31, 30, 31,
                                   31, 30, 31, 30, 31};
    //private String[] month_shorts = {"Jan", "Feb", "Mar", "Apr", 
    //                                 "May", "Jun", "Jul", "Aug", 
    //                                 "Sep", "Oct", "Nov", "Dec"};
    //private String[] weekday_shorts = {"Mon", "Tue", "Wed", "Thu",
    //                                   "Fri", "Sat", "Sun"};
    
    private static long julianDayOffset = 2440588;
    private static int millisPerHour = 60 * 60 * 1000;
    private static int millisPerDay = 24 * millisPerHour;
    private static final int JAN_1_1_JULIAN_DAY = 1721426;
    
    private final static String wtb[] = {
        "am", "pm",
        "monday", "tuesday", "wednesday", "thursday", "friday",
        "saturday", "sunday",
        "january", "february", "march", "april", "may", "june",
        "july", "august", "september", "october", "november", "december",
        "gmt", "ut", "utc", "est", "edt", "cst", "cdt",
        "mst", "mdt", "pst", "pdt"
        // this time zone table needs to be expanded
    };

    private final static int ttb[] = {
        14, 1, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
        10000 + 0, 10000 + 0, 10000 + 0,        // GMT/UT/UTC
        10000 + 5 * 60, 10000 + 4 * 60, // EST/EDT
        10000 + 6 * 60, 10000 + 5 * 60,
        10000 + 7 * 60, 10000 + 6 * 60,
        10000 + 8 * 60, 10000 + 7 * 60
    };
    
    private static final int NUM_DAYS[]      = {  0,  31,  59,  90, 120, 151, 
                                                 181, 212, 243, 273, 304, 334};

    private static final int LEAP_NUM_DAYS[] = {  0,  31,  60,  91, 121, 152,
                                                182, 213, 244, 274, 305, 335};
    
    static {
        timezones = new Hashtable();
        timezones.put("GMT", new Integer(0 * millisPerHour));
        timezones.put("UT", new Integer(0 * millisPerHour));
        timezones.put("UTC", new Integer(0 * millisPerHour));
        timezones.put("PST", new Integer(-8 * millisPerHour));
        timezones.put("PDT", new Integer(-7 * millisPerHour));
        timezones.put("JST", new Integer(9 * millisPerHour));
        
        local_tz = ((Integer)timezones.get("PST")).intValue();
    }
}
