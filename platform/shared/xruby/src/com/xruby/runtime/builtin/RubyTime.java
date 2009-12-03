/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Locale;
import java.util.TimeZone;

import com.xruby.runtime.lang.*;
import com.xruby.runtime.lang.annotation.RubyAllocMethod;
import com.xruby.runtime.lang.annotation.RubyLevelClass;
import com.xruby.runtime.lang.annotation.RubyLevelMethod;

@RubyLevelClass(name="Time")
public class RubyTime extends RubyBasic {
    private final Calendar date_;

    private RubyTime(Calendar date){
        super(RubyRuntime.TimeClass);
        date_ = date;
    }

    RubyTime(long time){
        super(RubyRuntime.TimeClass);
        date_ = Calendar.getInstance();
        date_.setTimeInMillis(time);
    }

    RubyTime() {
        super(RubyRuntime.TimeClass);
        date_ = Calendar.getInstance();
    }

    @RubyAllocMethod
    public static RubyTime alloc(RubyValue receiver) {
        return ObjectFactory.createTime();
    }

    public final long getTime() {
        return date_.getTimeInMillis();
    }

    private final int getUsec() {
        long t = getTime();

        if (t > 0 && t < 1000) {
            return (int) (t * 1000);
        }

        float t1 = ((float)t / 1000);
        float t2 = (long)t1;
        return (int)((t1 - t2) * 1000000);
    }

    public String toString() {
        SimpleDateFormat sdf = new SimpleDateFormat("EEE MMM dd HH:mm:ss z yyyy", Locale.US);
        sdf.setCalendar(date_);
        return sdf.format(date_.getTime()).replace("GMT", "UTC");
    }

    @RubyLevelMethod(name="year")
    public RubyValue year() {
        return ObjectFactory.createFixnum(date_.get(Calendar.YEAR));
    }

    @RubyLevelMethod(name="month")
    public RubyValue month() {
        return ObjectFactory.createFixnum(date_.get(Calendar.MONTH) + 1);
    }

    @RubyLevelMethod(name="day")
    public RubyValue day() {
        return ObjectFactory.createFixnum(date_.get(Calendar.DATE));
    }

    @RubyLevelMethod(name="to_f")
    public RubyFloat to_f() {
        return ObjectFactory.createFloat((double)getTime() / 1000);
    }

    @RubyLevelMethod(name="to_i", alias="tv_sec")
    public RubyFixnum to_i() {
        return ObjectFactory.createFixnum((int) (getTime() / 1000));
    }

    @RubyLevelMethod(name="usec", alias="tv_usec")
    public RubyFixnum usec() {
        return ObjectFactory.createFixnum(this.getUsec());
    }

    @RubyLevelMethod(name="to_s")
    public RubyString to_s() {
        return ObjectFactory.createString(this.toString());
    }

    @RubyLevelMethod(name="+")
    public RubyTime plus(RubyValue value) {
        double timeAdd = value.toFloat();
        return ObjectFactory.createTime(getTime() + (long)(timeAdd * 1000));
    }

    @RubyLevelMethod(name="-")
    public RubyValue minus(RubyValue value) {
        if (value instanceof RubyTime) {
            RubyTime time2 = (RubyTime)value;
            long timeInteval = getTime() - time2.getTime();
            if (timeInteval % 1000 == 0) {
                return ObjectFactory.createFixnum((int) (timeInteval / 1000));
            }
            return ObjectFactory.createFloat((double) timeInteval / 1000);
        }

        double time = value.toFloat();
        return ObjectFactory.createTime((getTime() - (long)(time * 1000)));
    }

    @RubyLevelMethod(name="<=>")
    public RubyFixnum cmp(RubyValue value) {
        long time1 = getTime();
        long time2 = RubyTypesUtil.convertToTime(value).getTime();
        if (time1 < time2) {
            return ObjectFactory.FIXNUM_NEGATIVE_ONE;
        } else if (time1 > time2) {
            return ObjectFactory.FIXNUM1;
        }
        return ObjectFactory.FIXNUM0;
    }

    @RubyLevelMethod(name="zone")
    public RubyString zone() {
        String name = date_.getTimeZone().getDisplayName(false, TimeZone.SHORT, Locale.US);
        if (name.equals("GMT")) {
            name = "UTC";
        }
        return ObjectFactory.createString(name);
    }

    @RubyLevelMethod(name="utc", alias="gm")
    public static RubyTime utc(RubyValue receiver, RubyArray args) {
        return createTime(args, TimeZone.getTimeZone("GMT"));
    }

    @RubyLevelMethod(name="gmt?")
    public RubyValue gmt_question() {
        return ObjectFactory.createBoolean(date_.getTimeZone().hasSameRules(TimeZone.getTimeZone("GMT")));
    }

    @RubyLevelMethod(name="localtime")
    public RubyValue localtime() {
        date_.setTimeZone(TimeZone.getDefault());
        return this;
    }

    @RubyLevelMethod(name="mktime", alias="local")
    public static RubyTime local(RubyValue receiver, RubyArray args) {
        return createTime(args, TimeZone.getDefault());
    }

    private static RubyTime createTime(RubyArray args, TimeZone zone) {
        if (null == args || args.size() == 0) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong number of arguments (0 for 1)");
        }

        int i = 0;
        int year = ((RubyFixnum) args.get(i++)).toInt();
        int month = (args.size() <= i) ? 0 : ((RubyFixnum) args.get(i++)).toInt();
        int day = (args.size() <= i) ? 0 : ((RubyFixnum) args.get(i++)).toInt();
        int hour = (args.size() <= i) ? 0 : ((RubyFixnum) args.get(i++)).toInt();
        int min = (args.size() <= i) ? 0 : ((RubyFixnum) args.get(i++)).toInt();
        int sec = (args.size() <= i) ? 0 : ((RubyFixnum) args.get(i++)).toInt();
        Calendar calendar = Calendar.getInstance(zone);
        calendar.set(year, month - 1, day, hour, min, sec);
        return new RubyTime(calendar);
    }

    @RubyLevelMethod(name="at")
    public static RubyTime at(RubyValue receiver, RubyValue value) {
        double time = 0;
        if (value instanceof RubyFixnum) {
            time = value.toInt();
        } else if (value instanceof RubyBignum) {
            time = value.toFloat();
        } else if (value instanceof RubyFloat) {
            time = value.toFloat();
        } else {
            throw new RubyException(RubyRuntime.TypeErrorClass, "can't convert " + value.getRubyClass().getName() + " into Time");
        }

        return ObjectFactory.createTime((long) (time*1000));
    }

    @RubyLevelMethod(name="now")
    public static RubyValue now(RubyValue receiver, RubyArray args, RubyBlock block) {
        RubyClass r = (RubyClass) receiver;
        return r.newInstance(args, block);
    }
}
