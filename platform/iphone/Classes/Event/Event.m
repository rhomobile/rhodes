//
//  Event.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 16.09.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "Event.h"

#include "ruby.h"
#include "ruby/ext/rho/rhoruby.h"
#include "ruby/ext/event/event.h"

#include "logging/RhoLogConf.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Event"

@implementation Event

@end

static VALUE dateToRuby(NSDate *date)
{
    ID id_mktime = rb_intern("mktime");
    NSTimeInterval ti = [date timeIntervalSinceReferenceDate];
    CFGregorianDate gDate = CFAbsoluteTimeGetGregorianDate(ti, NULL);
    VALUE rDate = rb_funcall(rb_cTime, id_mktime, 7, INT2FIX(gDate.year), INT2FIX(gDate.month), INT2FIX(gDate.day),
                             INT2FIX(gDate.hour), INT2FIX(gDate.minute), INT2FIX(gDate.second), INT2FIX(0));
    return rDate;
}

static NSDate *dateFromRuby(VALUE rDate)
{
    VALUE cDate = rb_class_of(rDate);
    if (!rb_equal(cDate, rb_cTime))
        rb_raise(rb_eArgError, "Wrong type of parameter: %s (Time expected)", rb_class2name(cDate));
    
    CFGregorianDate gDate;
    
    ID id_gmtime = rb_intern("gmtime");
    ID id_year = rb_intern("year");
    ID id_month = rb_intern("month");
    ID id_day = rb_intern("day");
    ID id_hour = rb_intern("hour");
    ID id_min = rb_intern("min");
    ID id_sec = rb_intern("sec");
    
    // Get GM time
    rDate = rb_funcall(rDate, id_gmtime, 0);
    
    VALUE year = rb_funcall(rDate, id_year, 0);
    VALUE month = rb_funcall(rDate, id_month, 0);
    VALUE day = rb_funcall(rDate, id_day, 0);
    VALUE hour = rb_funcall(rDate, id_hour, 0);
    VALUE minute = rb_funcall(rDate, id_min, 0);
    VALUE second = rb_funcall(rDate, id_sec, 0);
    
    gDate.year = FIX2INT(year);
    gDate.month = FIX2INT(month);
    gDate.day = FIX2INT(day);
    gDate.hour = FIX2INT(hour);
    gDate.minute = FIX2INT(minute);
    gDate.second = FIX2INT(second);
    
    NSDate *date = [NSDate dateWithTimeIntervalSinceReferenceDate:CFGregorianDateGetAbsoluteTime(gDate, NULL)];
    return date;
}

static VALUE event2ruby(EKEvent *event)
{
    VALUE rEvent = rho_ruby_createHash();
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_ID), rb_str_new2([event.eventIdentifier UTF8String]));
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_TITLE), rb_str_new2([event.title UTF8String]));
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_CANCELED), rho_ruby_create_boolean(event.status == EKEventStatusCanceled));
    if (event.organizer) {
        rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_ORGANIZER), rb_str_new2([event.organizer.name UTF8String]));
    }
    
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_START_DATE), dateToRuby(event.startDate));
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_END_DATE), dateToRuby(event.endDate));
    
    return rEvent;
}

VALUE event_fetch(VALUE start_date, VALUE end_date)
{
#if !defined(__IPHONE_4_0)
    return rho_ruby_get_NIL();
#else
    NSDate *start = dateFromRuby(start_date);
    NSDate *finish = dateFromRuby(end_date);
    
    EKEventStore *eventStore = [[EKEventStore alloc] init];
    NSPredicate *pred = [eventStore predicateForEventsWithStartDate:start endDate:finish calendars:nil];
    NSArray *events = [eventStore eventsMatchingPredicate:pred];
    [eventStore release];
    
    VALUE ret = rho_ruby_create_array();
    
    for (int i = 0, lim = [events count]; i != lim; ++i) {
        EKEvent *event = [events objectAtIndex:i];
        VALUE rEvent = event2ruby(event);
        rho_ruby_add_to_array(ret, rEvent);
    }
    
    return ret;
#endif
}

VALUE event_fetch_by_id(const char *eid)
{
#if !defined(__IPHONE_4_0)
    return rho_ruby_get_NIL();
#else
    EKEventStore *eventStore = [[EKEventStore alloc] init];
    EKEvent *event = [eventStore eventWithIdentifier:[NSString stringWithUTF8String:eid]];
    [eventStore release];
    
    return event2ruby(event);
#endif
}

void event_save(VALUE rEvent)
{
    VALUE rId = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_ID));
    if (!NIL_P(rId))
        Check_Type(rId, T_STRING);

    EKEventStore *eventStore = [[EKEventStore alloc] init];
    
    EKEvent *event = nil;
    if (NIL_P(rId)) {
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
    
    NSError *err;
    BOOL saved = [eventStore saveEvent:event span:EKSpanThisEvent error:&err];
    [eventStore release];
    
    if (!saved)
        rb_raise(rb_eRuntimeError, "Event save failed: %s", [[err localizedDescription] UTF8String]);
}
