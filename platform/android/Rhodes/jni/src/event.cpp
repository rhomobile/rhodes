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

#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoRuby.h"

#include <ruby.h>
#include <ruby/ext/calendar/event.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "EventJNI"

#ifdef RHO_TRACE
#undef RHO_TRACE
#endif

#ifdef RHO_EVENT_ENABLE_TRACE
#include <android/log.h>
#define RHO_TRACE(fmt, ...) __android_log_print(ANDROID_LOG_INFO, "Event", "%s:%d: " fmt, ##__VA_ARGS__)
#else
#define RHO_TRACE(...)
#endif

template <typename Out, typename In>
Out date_cast(In value);

template <>
jobject date_cast<jobject, VALUE>(VALUE rDate)
{
    if (NIL_P(rDate))
        return NULL;

    RHO_TRACE("dateFromRuby (1)");
    if (TYPE(rDate) == T_STRING)
    {
        RHO_TRACE("dateFromRuby (1.1)");
        rDate = rb_funcall(rb_cTime, rb_intern("parse"), 1, rDate);
        RHO_TRACE("dateFromRuby (1.2)");
    }
    VALUE cDate = rb_class_of(rDate);
    if (!rb_equal(cDate, rb_cTime))
        rb_raise(rb_eArgError, "Wrong type of parameter: %s (Time expected)", rb_class2name(cDate));

    RHO_TRACE("dateFromRuby (2)");
    int year = NUM2INT(rb_funcall(rDate, rb_intern("year"), 0)) - 1900;
    RHO_TRACE("dateFromRuby (3)");
    int month = NUM2INT(rb_funcall(rDate, rb_intern("month"), 0)) - 1;
    RHO_TRACE("dateFromRuby (4)");
    int day = NUM2INT(rb_funcall(rDate, rb_intern("day"), 0));
    RHO_TRACE("dateFromRuby (5)");
    int hour = NUM2INT(rb_funcall(rDate, rb_intern("hour"), 0));
    RHO_TRACE("dateFromRuby (6)");
    int minute = NUM2INT(rb_funcall(rDate, rb_intern("min"), 0));
    RHO_TRACE("dateFromRuby (7)");
    int second = NUM2INT(rb_funcall(rDate, rb_intern("sec"), 0));
    RHO_TRACE("dateFromRuby (8)");

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_DATE);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassMethod(env, cls, "<init>", "(IIIIII)V");
    if (!mid) return NULL;

    RHO_TRACE("dateFromRuby (9)");
    jobject jDate = env->NewObject(cls, mid, year, month, day, hour, minute, second);
    return jDate;
}

template <>
VALUE date_cast<VALUE, jobject>(jobject jDate)
{
    if (!jDate)
        return Qnil;

    RHO_TRACE("dateToRuby (1)");

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_DATE);
    if (!cls) return Qnil;
    jmethodID midYear = getJNIClassMethod(env, cls, "getYear", "()I");
    if (!midYear) return Qnil;
    jmethodID midMonth = getJNIClassMethod(env, cls, "getMonth", "()I");
    if (!midMonth) return Qnil;
    jmethodID midDay = getJNIClassMethod(env, cls, "getDate", "()I");
    if (!midDay) return Qnil;
    jmethodID midHours = getJNIClassMethod(env, cls, "getHours", "()I");
    if (!midHours) return Qnil;
    jmethodID midMinutes = getJNIClassMethod(env, cls, "getMinutes", "()I");
    if (!midMinutes) return Qnil;
    jmethodID midSeconds = getJNIClassMethod(env, cls, "getSeconds", "()I");
    if (!midSeconds) return Qnil;

    RHO_TRACE("dateToRuby (2)");

    int year = env->CallIntMethod(jDate, midYear) + 1900;
    int month = env->CallIntMethod(jDate, midMonth) + 1;
    int day = env->CallIntMethod(jDate, midDay);
    int hour = env->CallIntMethod(jDate, midHours);
    int minute = env->CallIntMethod(jDate, midMinutes);
    int second = env->CallIntMethod(jDate, midSeconds);

    RHO_TRACE("dateToRuby (3)");
    VALUE rDate = rb_funcall(rb_cTime, rb_intern("mktime"), 7, INT2FIX(year), INT2FIX(month), INT2FIX(day),
            INT2FIX(hour), INT2FIX(minute), INT2FIX(second), INT2FIX(0));

    RHO_TRACE("dateToRuby (4)");
    return rDate;
}

static jclass clsEvent;
static jfieldID fidId;
static jfieldID fidTitle;
static jfieldID fidStartDate;
static jfieldID fidEndDate;
static jfieldID fidLastModified;
static jfieldID fidLocation;
static jfieldID fidNotes;
static jfieldID fidPrivacy;
static jfieldID fidFrequency;
static jfieldID fidInterval;

static bool init_event_stuff(JNIEnv *env)
{
    static bool initialized = false;
    if (initialized)
        return true;

    clsEvent = getJNIClass(RHODES_JAVA_CLASS_EVENT);
    if (!clsEvent) return false;
    fidId = getJNIClassField(env, clsEvent, "id", "Ljava/lang/String;");
    if (!fidId) return false;
    fidTitle = getJNIClassField(env, clsEvent, "title", "Ljava/lang/String;");
    if (!fidTitle) return false;
    fidStartDate = getJNIClassField(env, clsEvent, "startDate", "Ljava/util/Date;");
    if (!fidStartDate) return false;
    fidEndDate = getJNIClassField(env, clsEvent, "endDate", "Ljava/util/Date;");
    if (!fidEndDate) return false;
    fidLastModified = getJNIClassField(env, clsEvent, "lastModified", "Ljava/util/Date;");
    if (!fidLastModified) return false;
    fidLocation = getJNIClassField(env, clsEvent, "location", "Ljava/lang/String;");
    if (!fidLocation) return false;
    fidNotes = getJNIClassField(env, clsEvent, "notes", "Ljava/lang/String;");
    if (!fidNotes) return false;
    fidPrivacy = getJNIClassField(env, clsEvent, "privacy", "Ljava/lang/String;");
    if (!fidPrivacy) return false;
    fidFrequency = getJNIClassField(env, clsEvent, "frequency", "Ljava/lang/String;");
    if (!fidFrequency) return false;
    fidInterval = getJNIClassField(env, clsEvent, "interval", "I");
    if (!fidInterval) return false;

    initialized = true;
    return true;
}

template <typename Out, typename In>
Out event_cast(In value);

template <>
jobject event_cast<jobject, VALUE>(VALUE rEvent)
{
    if (NIL_P(rEvent))
        return NULL;

    RHO_TRACE("eventFromRuby (1)");
    JNIEnv *env = jnienv();
    if (!init_event_stuff(env))
        return NULL;

    RHO_TRACE("eventFromRuby (2)");
    VALUE rId = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_ID));
    if (NIL_P(rId))
        rId = rb_str_new2("");
    Check_Type(rId, T_STRING);

    RHO_TRACE("eventFromRuby (3)");
    jmethodID mid = getJNIClassMethod(env, clsEvent, "<init>", "(Ljava/lang/String;)V");
    if (!mid) return NULL;
    jobject jEvent = env->NewObject(clsEvent, mid, rho_cast<jhstring>(RSTRING_PTR(rId)).get());
    if (!jEvent) return NULL;

    RHO_TRACE("eventFromRuby (4)");
    VALUE rTitle = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_TITLE));
    if (!NIL_P(rTitle))
    {
        Check_Type(rTitle, T_STRING);
        env->SetObjectField(jEvent, fidTitle, rho_cast<jhstring>(RSTRING_PTR(rTitle)).get());
    }

    RHO_TRACE("eventFromRuby (5)");
    VALUE rStartDate = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_START_DATE));
    if (!NIL_P(rStartDate))
        env->SetObjectField(jEvent, fidStartDate, date_cast<jobject>(rStartDate));

    RHO_TRACE("eventFromRuby (6)");
    VALUE rEndDate = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_END_DATE));
    if (!NIL_P(rEndDate))
        env->SetObjectField(jEvent, fidEndDate, date_cast<jobject>(rEndDate));

    RHO_TRACE("eventFromRuby (7)");
    VALUE rLastModified = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_LAST_MODIFIED));
    if (!NIL_P(rLastModified))
        env->SetObjectField(jEvent, fidLastModified, date_cast<jobject>(rLastModified));

    RHO_TRACE("eventFromRuby (8)");
    VALUE rLocation = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_LOCATION));
    if (!NIL_P(rLocation))
    {
        Check_Type(rLocation, T_STRING);
        env->SetObjectField(jEvent, fidLocation, rho_cast<jhstring>(RSTRING_PTR(rLocation)).get());
    }

    RHO_TRACE("eventFromRuby (9)");
    VALUE rNotes = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_NOTES));
    if (!NIL_P(rNotes))
    {
        Check_Type(rNotes, T_STRING);
        env->SetObjectField(jEvent, fidNotes, rho_cast<jhstring>(RSTRING_PTR(rNotes)).get());
    }

    RHO_TRACE("eventFromRuby (10)");
    VALUE rPrivacy = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_PRIVACY));
    if (!NIL_P(rPrivacy))
    {
        Check_Type(rPrivacy, T_STRING);
        env->SetObjectField(jEvent, fidPrivacy, rho_cast<jhstring>(RSTRING_PTR(rPrivacy)).get());
    }

    RHO_TRACE("eventFromRuby (11)");
    VALUE rRecurrence = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_RECURRENCE));
    if (!NIL_P(rRecurrence)) {
        Check_Type(rRecurrence, T_HASH);

        VALUE rFrequency = rb_hash_aref(rRecurrence, rb_str_new2(RUBY_EV_RECURRENCE_FREQUENCY));
        Check_Type(rFrequency, T_STRING);
        const char *frequency = RSTRING_PTR(rFrequency);
        if (   strcasecmp(frequency, RUBY_EV_RECURRENCE_FREQUENCY_DAILY) != 0
        	&& strcasecmp(frequency, RUBY_EV_RECURRENCE_FREQUENCY_WEEKLY) != 0
        	&& strcasecmp(frequency, RUBY_EV_RECURRENCE_FREQUENCY_MONTHLY) != 0
        	&& strcasecmp(frequency, RUBY_EV_RECURRENCE_FREQUENCY_YEARLY) != 0)
        {
        	rb_raise(rb_eArgError, "Wrong recurrence frequency: %s", frequency);
        }
        env->SetObjectField(jEvent, fidFrequency, rho_cast<jhstring>(RSTRING_PTR(rFrequency)).get());

        VALUE rInterval = rb_hash_aref(rRecurrence, rb_str_new2(RUBY_EV_RECURRENCE_INTERVAL));
        rInterval = rb_funcall(rInterval, rb_intern("to_i"), 0);
        int interval = NUM2INT(rInterval);
        env->SetIntField(jEvent, fidInterval, interval);
    }

    RHO_TRACE("eventFromRuby: return");
    return jEvent;
}

template <>
VALUE event_cast<VALUE, jobject>(jobject jEvent)
{
    if (!jEvent)
        return Qnil;

    RHO_TRACE("eventToRuby (1)");
    JNIEnv *env = jnienv();
    if (!init_event_stuff(env))
    {
        RHO_TRACE("eventToRuby (1.1)");
        return Qnil;
    }

    RHO_TRACE("eventToRuby (2)");
    VALUE rEvent = rb_hash_new();

    RHO_TRACE("eventToRuby (3)");
    jstring jId = (jstring)env->GetObjectField(jEvent, fidId);
    std::string s = rho_cast<std::string>(env, jId);
    env->DeleteLocalRef(jId);
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_ID), rb_str_new2(s.c_str()));

    RHO_TRACE("eventToRuby (4)");
    jstring jTitle = (jstring)env->GetObjectField(jEvent, fidTitle);
    s = rho_cast<std::string>(env, jTitle);
    env->DeleteLocalRef(jTitle);
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_TITLE), rb_str_new2(s.c_str()));

    RHO_TRACE("eventToRuby (5)");
    jobject jStartDate = env->GetObjectField(jEvent, fidStartDate);
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_START_DATE), date_cast<VALUE>(jStartDate));
    env->DeleteLocalRef(jStartDate);

    RHO_TRACE("eventToRuby (6)");
    jobject jEndDate = env->GetObjectField(jEvent, fidEndDate);
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_END_DATE), date_cast<VALUE>(jEndDate));
    env->DeleteLocalRef(jEndDate);

    RHO_TRACE("eventToRuby (7)");
    jobject jLastModified = env->GetObjectField(jEvent, fidLastModified);
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_LAST_MODIFIED), date_cast<VALUE>(jLastModified));
    env->DeleteLocalRef(jLastModified);

    RHO_TRACE("eventToRuby (8)");
    jstring jLocation = (jstring)env->GetObjectField(jEvent, fidLocation);
    if (jLocation)
    {
        s = rho_cast<std::string>(env, jLocation);
        env->DeleteLocalRef(jLocation);
        rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_LOCATION), rb_str_new2(s.c_str()));
    }

    RHO_TRACE("eventToRuby (9)");
    jstring jNotes = (jstring)env->GetObjectField(jEvent, fidNotes);
    if (jNotes)
    {
        s = rho_cast<std::string>(env, jNotes);
        env->DeleteLocalRef(jNotes);
        rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_NOTES), rb_str_new2(s.c_str()));
    }

    RHO_TRACE("eventToRuby (10)");
    jstring jPrivacy = (jstring)env->GetObjectField(jEvent, fidPrivacy);
    if (jPrivacy)
    {
        s = rho_cast<std::string>(env, jPrivacy);
        env->DeleteLocalRef(jPrivacy);
        rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_PRIVACY), rb_str_new2(s.c_str()));
    }

    RHO_TRACE("eventToRuby (11)");
    jstring jFrequency = (jstring)env->GetObjectField(jEvent, fidFrequency);
    if (jFrequency)
    {
        VALUE rRecurrence = rb_hash_new();

        jboolean isCopy;
        const char* str = env->GetStringUTFChars (jFrequency, &isCopy);
        if (   strcasecmp(str, RUBY_EV_RECURRENCE_FREQUENCY_DAILY) == 0
        	|| strcasecmp(str, RUBY_EV_RECURRENCE_FREQUENCY_WEEKLY) == 0
        	|| strcasecmp(str, RUBY_EV_RECURRENCE_FREQUENCY_MONTHLY) == 0
        	|| strcasecmp(str, RUBY_EV_RECURRENCE_FREQUENCY_YEARLY) == 0)
        {
        	s = rho_cast<std::string>(env, jFrequency);
        } else {
        	s = "undefined";
            // rb_raise(rb_eArgError, "Wrong recurrence frequency: %s", frequency);
        }
        if (isCopy == JNI_TRUE) {
           env->ReleaseStringUTFChars (jFrequency, str);
        }
        rb_hash_aset(rRecurrence, rb_str_new2(RUBY_EV_RECURRENCE_FREQUENCY), rb_str_new2(s.c_str()));
        env->DeleteLocalRef(jFrequency);

        jint jInterval = (jint)env->GetIntField(jEvent, fidInterval);
        rb_hash_aset(rRecurrence, rb_str_new2(RUBY_EV_RECURRENCE_INTERVAL), INT2FIX((int)jInterval));

        rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_RECURRENCE), rRecurrence);
    }

    RHO_TRACE("eventToRuby: return");
    return rEvent;
}

RHO_GLOBAL VALUE event_fetch(VALUE rParams)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_EVENT_STORE);
    if (!cls) return Qnil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "fetch", "(Ljava/util/Date;Ljava/util/Date;Z)Ljava/lang/Object;");
    if (!mid) return Qnil;

    VALUE start_date = rb_hash_aref(rParams, rb_str_new2(RUBY_EV_START_DATE));
    VALUE end_date = rb_hash_aref(rParams, rb_str_new2(RUBY_EV_END_DATE));
    VALUE include_repeating = rb_hash_aref(rParams, rb_str_new2(RUBY_FETCH_include_repeating));

    RHO_TRACE("event_fetch (1)");
    jobject jStartDate = date_cast<jobject>(start_date);
    RHO_TRACE("event_fetch (2)");
    jobject jEndDate = date_cast<jobject>(end_date);
    RHO_TRACE("event_fetch (3)");
    jobject jRet = env->CallStaticObjectMethod(cls, mid, jStartDate, jEndDate, (jboolean)rho_ruby_get_bool(include_repeating));
    RHO_TRACE("event_fetch (4)");
    env->DeleteLocalRef(jStartDate);
    env->DeleteLocalRef(jEndDate);

    jclass clsString = getJNIClass(RHODES_JAVA_CLASS_STRING);
    if (!clsString) return Qnil;
    if (env->IsInstanceOf(jRet, clsString))
    {
        std::string error = rho_cast<std::string>(env, (jstring)jRet);
        env->DeleteLocalRef(jRet);
        rb_raise(rb_eRuntimeError, "Can't fetch events: %s", error.c_str());
        return Qnil;
    }

    jclass clsVector = getJNIClass(RHODES_JAVA_CLASS_VECTOR);
    if (!clsVector) return Qnil;
    jmethodID midSize = getJNIClassMethod(env, clsVector, "size", "()I");
    if (!midSize) return Qnil;
    jmethodID midGet = getJNIClassMethod(env, clsVector, "get", "(I)Ljava/lang/Object;");
    if (!midGet) return Qnil;

    RHO_TRACE("event_fetch (5)");
    VALUE ret = rb_ary_new();

    RHO_TRACE("event_fetch (6)");
    for (int i = 0, lim = env->CallIntMethod(jRet, midSize); i != lim; ++i)
    {
        RHO_TRACE("event_fetch (6.1)");
        jobject jEvent = env->CallObjectMethod(jRet, midGet, i);
        RHO_TRACE("event_fetch (6.2)");
        VALUE rEvent = event_cast<VALUE>(jEvent);
        RHO_TRACE("event_fetch (6.3)");
        env->DeleteLocalRef(jEvent);
        RHO_TRACE("event_fetch (6.4)");
        rb_ary_push(ret, rEvent);
        RHO_TRACE("event_fetch (6.5)");
    }

    RHO_TRACE("event_fetch (7)");
    env->DeleteLocalRef(jRet);

    return ret;
}

RHO_GLOBAL VALUE event_fetch_by_id(const char *id)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_EVENT_STORE);
    if (!cls) return Qnil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "fetch", "(Ljava/lang/String;)Ljava/lang/Object;");
    if (!mid) return Qnil;
    jclass clsString = getJNIClass(RHODES_JAVA_CLASS_STRING);
    if (!clsString) return Qnil;

    RHO_TRACE("event_fetch_by_id (1)");
    jhstring jId = rho_cast<jhstring>(env, id);
    RHO_TRACE("event_fetch_by_id (2)");
    jhobject jRet = jhobject(env->CallStaticObjectMethod(cls, mid, jId.get()));
    RHO_TRACE("event_fetch_by_id (3)");
    if (env->IsInstanceOf(jRet.get(), clsString))
    {
        RHO_TRACE("event_fetch_by_id (3.1)");
        std::string error = rho_cast<std::string>(env, (jstring)jRet.get());
        RHO_TRACE("event_fetch_by_id (3.2)");
        rb_raise(rb_eRuntimeError, "Can't fetch event with id %s: %s", id, error.c_str());
        return Qnil;
    }
    VALUE rEvent = event_cast<VALUE>(jRet.get());
    RHO_TRACE("event_fetch_by_id (4)");
    return rEvent;
}

static std::string return_string;

RHO_GLOBAL const char* event_save(VALUE rEvent)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_EVENT_STORE);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "save", "(Lcom/rhomobile/rhodes/event/Event;)Ljava/lang/String;");
    if (!mid) return NULL;

    jobject jEvent = event_cast<jobject>(rEvent);
    jstring new_id = (jstring)env->CallStaticObjectMethod(cls, mid, jEvent);
    env->DeleteLocalRef(jEvent);

    //if (jError)
    //{
    //    std::string error = rho_cast<std::string>(env, jError);
    //   env->DeleteLocalRef(jError);
    //    rb_raise(rb_eRuntimeError, "Event save failed: %s", error.c_str());
    //}
    if (new_id) {
        return_string = rho_cast<std::string>(env, new_id);
        env->DeleteLocalRef(new_id);
        return return_string.c_str();
    }
    return NULL;
}

RHO_GLOBAL void event_delete(const char *id)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_EVENT_STORE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "delete", "(Ljava/lang/String;)Ljava/lang/String;");
    if (!mid) return;

    jhstring jId = rho_cast<jhstring>(env, id);
    jhstring jError = jhstring((jstring)env->CallStaticObjectMethod(cls, mid, jId.get()));

    if (!!jError)
    {
        std::string error = rho_cast<std::string>(env, jError.get());
        rb_raise(rb_eRuntimeError, "Event delete failed: %s", error.c_str());
    }
}
