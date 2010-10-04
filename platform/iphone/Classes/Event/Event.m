//
//  Event.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 16.09.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "Event.h"
#import "Rhodes.h"

#include "ruby.h"
#include "ruby/ext/rho/rhoruby.h"
#include "ruby/ext/calendar/event.h"

#include "logging/RhoLogConf.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Event"

static void calendar_check()
{
#if defined(__IPHONE_4_0)
    NSString *version = [[UIDevice currentDevice] systemVersion];
    if ([version hasPrefix:@"3."] || [version hasPrefix:@"2."])
#endif
        rb_raise(rb_eRuntimeError, "No calendar support on this device");
}

#ifdef __IPHONE_4_0
static VALUE dateToRuby(NSDate *date)
{
    NSCalendar *gregorian = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar];
    unsigned unitFlags = NSYearCalendarUnit|NSMonthCalendarUnit|NSDayCalendarUnit;
    NSDateComponents *dateComponents = [gregorian components:unitFlags fromDate:date];
    unitFlags = NSHourCalendarUnit|NSMinuteCalendarUnit|NSSecondCalendarUnit;
    NSDateComponents *timeComponents = [gregorian components:unitFlags fromDate:date];
    
    int year = [dateComponents year];
    int month = [dateComponents month];
    int day = [dateComponents day] - 1;
    int hour = [timeComponents hour];
    int minute = [timeComponents minute];
    int second = [timeComponents second];
    
    [gregorian release];
    
    VALUE rDate = rb_funcall(rb_cTime, rb_intern("utc"), 7, INT2FIX(year), INT2FIX(month), INT2FIX(day),
                             INT2FIX(hour), INT2FIX(minute), INT2FIX(second), INT2FIX(0));
    return rDate;
}

static NSDate *dateFromRuby(VALUE rDate)
{
    if (TYPE(rDate) == T_STRING) {
        // Convert to time
        ID id_parse = rb_intern("parse");
        rDate = rb_funcall(rb_cTime, id_parse, 1, rDate);
    }
    
    VALUE cDate = rb_class_of(rDate);
    if (!rb_equal(cDate, rb_cTime))
        rb_raise(rb_eArgError, "Wrong type of parameter: %s (Time expected)", rb_class2name(cDate));
    
    ID id_gmtime = rb_intern("gmtime");
    ID id_year = rb_intern("year");
    ID id_month = rb_intern("month");
    ID id_day = rb_intern("day");
    ID id_hour = rb_intern("hour");
    ID id_min = rb_intern("min");
    ID id_sec = rb_intern("sec");
    
    // Get GM time
    rDate = rb_funcall(rDate, id_gmtime, 0);
    
    int year = FIX2INT(rb_funcall(rDate, id_year, 0));
    int month = FIX2INT(rb_funcall(rDate, id_month, 0));
    int day = FIX2INT(rb_funcall(rDate, id_day, 0)) + 1;
    int hour = FIX2INT(rb_funcall(rDate, id_hour, 0));
    int minute = FIX2INT(rb_funcall(rDate, id_min, 0));
    int second = FIX2INT(rb_funcall(rDate, id_sec, 0));
    
    NSDateComponents *dateComponents = [[NSDateComponents alloc] init];
    [dateComponents setYear:year];
    [dateComponents setMonth:month];
    [dateComponents setDay:day];
    [dateComponents setHour:hour];
    [dateComponents setMinute:minute];
    [dateComponents setSecond:second];
    
    NSCalendar *gregorian = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar];
    
    NSDate *date = [gregorian dateFromComponents:dateComponents];
    
    [gregorian release];
    [dateComponents release];
    
    return date;
}

static VALUE eventToRuby(EKEvent *event)
{
    if (!event)
        return Qnil;
    
    VALUE rEvent = rho_ruby_createHash();
    const char *eid = [event.eventIdentifier UTF8String];
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_ID), rb_str_new2(eid));
    if (event.title) {
        const char *title = [event.title UTF8String];
        rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_TITLE), rb_str_new2(title));
    }
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_CANCELED), rho_ruby_create_boolean(event.status == EKEventStatusCanceled));
    if (event.organizer) {
        const char *oname = [event.organizer.name UTF8String];
        rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_ORGANIZER), rb_str_new2(oname));
    }
    
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_START_DATE), dateToRuby(event.startDate));
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_END_DATE), dateToRuby(event.endDate));
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_LAST_MODIFIED), dateToRuby(event.lastModifiedDate));
    if (event.location) {
        const char *location = [event.location UTF8String];
        rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_LOCATION), rb_str_new2(location));
    }
    if (event.notes) {
        const char *notes = [event.notes UTF8String];
        rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_NOTES), rb_str_new2(notes));
    }
    
    if (event.recurrenceRule) {
        VALUE rRecurrence = rb_hash_new();
        const char *s;
        switch (event.recurrenceRule.frequency) {
            case EKRecurrenceFrequencyDaily: s = RUBY_EV_RECURRENCE_FREQUENCY_DAILY; break;
            case EKRecurrenceFrequencyWeekly: s = RUBY_EV_RECURRENCE_FREQUENCY_WEEKLY; break;
            case EKRecurrenceFrequencyMonthly: s = RUBY_EV_RECURRENCE_FREQUENCY_MONTHLY; break;
            case EKRecurrenceFrequencyYearly: s = RUBY_EV_RECURRENCE_FREQUENCY_YEARLY; break;
            default: s = "undefined";
        }
        rb_hash_aset(rRecurrence, rb_str_new2(RUBY_EV_RECURRENCE_FREQUENCY), rb_str_new2(s));
        
        int interval = event.recurrenceRule.interval;
        rb_hash_aset(rRecurrence, rb_str_new2(RUBY_EV_RECURRENCE_INTERVAL), INT2FIX(interval));
        
        rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_RECURRENCE), rRecurrence);
    }
    
    return rEvent;
}

static EKEvent *eventFromRuby(EKEventStore *eventStore, VALUE rEvent)
{
    VALUE rId = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_ID));
    if (!NIL_P(rId))
        Check_Type(rId, T_STRING);
    
    EKEvent *event = nil;
    if (NIL_P(rId) || strlen(RSTRING_PTR(rId)) == 0) {
        // New event
        event = [EKEvent eventWithEventStore:eventStore];
        event.calendar = [eventStore defaultCalendarForNewEvents];
    }
    else {
        // Existing event
        const char *sid = RSTRING_PTR(rId);
        event = [eventStore eventWithIdentifier:[NSString stringWithUTF8String:sid]];
    }
    
    VALUE rTitle = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_TITLE));
    if (!NIL_P(rTitle)) {
        Check_Type(rTitle, T_STRING);
        event.title = [NSString stringWithUTF8String:RSTRING_PTR(rTitle)];
    }
    
    VALUE rStartDate = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_START_DATE));
    if (!NIL_P(rStartDate))
        event.startDate = dateFromRuby(rStartDate);
    VALUE rEndDate = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_END_DATE));
    if (!NIL_P(rEndDate))
        event.endDate = dateFromRuby(rEndDate);
    
    VALUE rLocation = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_LOCATION));
    if (!NIL_P(rLocation)) {
        Check_Type(rLocation, T_STRING);
        event.location = [NSString stringWithUTF8String:RSTRING_PTR(rLocation)];
    }
    
    VALUE rNotes = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_NOTES));
    if (!NIL_P(rNotes)) {
        Check_Type(rNotes, T_STRING);
        event.notes = [NSString stringWithUTF8String:RSTRING_PTR(rNotes)];
    }
    
    VALUE rRecurrence = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_RECURRENCE));
    if (!NIL_P(rRecurrence)) {
        Check_Type(rRecurrence, T_HASH);
        VALUE rFrequency = rb_hash_aref(rRecurrence, rb_str_new2(RUBY_EV_RECURRENCE_FREQUENCY));
        Check_Type(rFrequency, T_STRING);
        const char *frequency = RSTRING_PTR(rFrequency);
        
        EKRecurrenceFrequency freq;
        if (strcasecmp(frequency, RUBY_EV_RECURRENCE_FREQUENCY_DAILY) == 0)
            freq = EKRecurrenceFrequencyDaily;
        else if (strcasecmp(frequency, RUBY_EV_RECURRENCE_FREQUENCY_WEEKLY) == 0)
            freq = EKRecurrenceFrequencyWeekly;
        else if (strcasecmp(frequency, RUBY_EV_RECURRENCE_FREQUENCY_MONTHLY) == 0)
            freq = EKRecurrenceFrequencyMonthly;
        else if (strcasecmp(frequency, RUBY_EV_RECURRENCE_FREQUENCY_YEARLY) == 0)
            freq = EKRecurrenceFrequencyYearly;
        else
            rb_raise(rb_eArgError, "Wrong recurrence frequency: %s", frequency);
        
        VALUE rInterval = rb_hash_aref(rRecurrence, rb_str_new2(RUBY_EV_RECURRENCE_INTERVAL));
        rInterval = rb_funcall(rInterval, rb_intern("to_i"), 0);
        int interval = NUM2INT(rInterval);
        
        EKRecurrenceRule *rule = [[EKRecurrenceRule alloc] initRecurrenceWithFrequency:freq interval:interval end:nil];
        event.recurrenceRule = rule;
        [rule release];
    }
    
    return event;
}
#endif // __IPHONE_4_0

VALUE event_fetch(VALUE start_date, VALUE end_date)
{
    calendar_check();
    
#if defined(__IPHONE_4_0)
    NSDate *start = dateFromRuby(start_date);
    NSDate *finish = dateFromRuby(end_date);
    
    EKEventStore *eventStore = [[Rhodes sharedInstance] eventStore];
    NSPredicate *pred = [eventStore predicateForEventsWithStartDate:start endDate:finish calendars:nil];
    NSArray *events = [eventStore eventsMatchingPredicate:pred];
    
    VALUE ret = rho_ruby_create_array();
    
    for (int i = 0, lim = [events count]; i != lim; ++i) {
        EKEvent *event = [events objectAtIndex:i];
        VALUE rEvent = eventToRuby(event);
        rho_ruby_add_to_array(ret, rEvent);
    }
    
    return ret;
#endif
}

VALUE event_fetch_by_id(const char *eid)
{
    calendar_check();
    
#if defined(__IPHONE_4_0)
    EKEventStore *eventStore = [[Rhodes sharedInstance] eventStore];
    EKEvent *event = [eventStore eventWithIdentifier:[NSString stringWithUTF8String:eid]];
    
    return eventToRuby(event);
#endif
}

void event_save(VALUE rEvent)
{
    calendar_check();
    
#if defined(__IPHONE_4_0)
    EKEventStore *eventStore = [[Rhodes sharedInstance] eventStore];

    EKEvent *event = eventFromRuby(eventStore, rEvent);
    
    NSError *err;
    BOOL saved = [eventStore saveEvent:event span:EKSpanFutureEvents error:&err];
    
    if (!saved)
        rb_raise(rb_eRuntimeError, "Event save failed: %s", [[err localizedDescription] UTF8String]);
#endif
}

void event_delete(const char *eid)
{
    calendar_check();
    
#if defined(__IPHONE_4_0)
    EKEventStore *eventStore = [[Rhodes sharedInstance] eventStore];
    EKEvent *event = [eventStore eventWithIdentifier:[NSString stringWithUTF8String:eid]];
    NSError *err;
    BOOL removed = [eventStore removeEvent:event span:EKSpanFutureEvents error:&err];
    
    if (!removed)
        rb_raise(rb_eRuntimeError, "Event was not removed: %s", [[err localizedDescription] UTF8String]);
#endif
}
