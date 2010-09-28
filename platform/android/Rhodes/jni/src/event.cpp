#include "rhodes/JNIRhodes.h"

#include <ruby.h>
#include <ruby/ext/event/event.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Event"

static jobject dateFromRuby(VALUE rDate)
{
    RAWLOG_INFO("dateFromRuby (1)");
    if (TYPE(rDate) == T_STRING)
    {
        RAWLOG_INFO("dateFromRuby (1.1)");
        rDate = rb_funcall(rb_cTime, rb_intern("parse"), 1, rDate);
        RAWLOG_INFO("dateFromRuby (1.2)");
    }
    VALUE cDate = rb_class_of(rDate);
    if (!rb_equal(cDate, rb_cTime))
        rb_raise(rb_eArgError, "Wrong type of parameter: %s (Time expected)", rb_class2name(cDate));
    
    RAWLOG_INFO("dateFromRuby (2)");
    int year = NUM2INT(rb_funcall(rDate, rb_intern("year"), 0)) - 1900;
    RAWLOG_INFO("dateFromRuby (3)");
    int month = NUM2INT(rb_funcall(rDate, rb_intern("month"), 0)) - 1;
    RAWLOG_INFO("dateFromRuby (4)");
    int day = NUM2INT(rb_funcall(rDate, rb_intern("day"), 0));
    RAWLOG_INFO("dateFromRuby (5)");
    int hour = NUM2INT(rb_funcall(rDate, rb_intern("hour"), 0));
    RAWLOG_INFO("dateFromRuby (6)");
    int minute = NUM2INT(rb_funcall(rDate, rb_intern("min"), 0));
    RAWLOG_INFO("dateFromRuby (7)");
    int second = NUM2INT(rb_funcall(rDate, rb_intern("sec"), 0));
    RAWLOG_INFO("dateFromRuby (8)");

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_DATE);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassMethod(env, cls, "<init>", "(IIIIII)V");
    if (!mid) return NULL;

    RAWLOG_INFO("dateFromRuby (9)");
    jobject jDate = env->NewObject(cls, mid, year, month, day, hour, minute, second);
    return jDate;
}

static VALUE dateToRuby(jobject jDate)
{
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

    int year = env->CallIntMethod(jDate, midYear) + 1900;
    int month = env->CallIntMethod(jDate, midMonth) + 1;
    int day = env->CallIntMethod(jDate, midDay);
    int hour = env->CallIntMethod(jDate, midHours);
    int minute = env->CallIntMethod(jDate, midMinutes);
    int second = env->CallIntMethod(jDate, midSeconds);

    VALUE rDate = rb_funcall(rb_cTime, rb_intern("mktime"), 7, INT2FIX(year), INT2FIX(month), INT2FIX(day),
            INT2FIX(hour), INT2FIX(minute), INT2FIX(second));
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

static bool initEventStuff(JNIEnv *env)
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

    initialized = true;
    return true;
}

static jobject eventFromRuby(VALUE rEvent)
{
    JNIEnv *env = jnienv();
    if (!initEventStuff(env))
        return NULL;

    jobject jEvent = env->NewObject(clsEvent, getJNIClassMethod(env, clsEvent, "<init>", "()V"));

    VALUE rId = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_ID));
    if (!NIL_P(rId))
    {
        Check_Type(rId, T_STRING);
        env->SetObjectField(jEvent, fidId, env->NewStringUTF(RSTRING_PTR(rId)));
    }
    
    VALUE rTitle = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_TITLE));
    if (!NIL_P(rTitle))
    {
        Check_Type(rTitle, T_STRING);
        env->SetObjectField(jEvent, fidTitle, env->NewStringUTF(RSTRING_PTR(rTitle)));
    }

    VALUE rStartDate = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_START_DATE));
    if (!NIL_P(rStartDate))
        env->SetObjectField(jEvent, fidStartDate, dateFromRuby(rStartDate));

    VALUE rEndDate = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_END_DATE));
    if (!NIL_P(rEndDate))
        env->SetObjectField(jEvent, fidEndDate, dateFromRuby(rEndDate));

    VALUE rLastModified = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_LAST_MODIFIED));
    if (!NIL_P(rLastModified))
        env->SetObjectField(jEvent, fidLastModified, dateFromRuby(rLastModified));

    VALUE rLocation = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_LOCATION));
    if (!NIL_P(rLocation))
    {
        Check_Type(rLocation, T_STRING);
        env->SetObjectField(jEvent, fidLocation, env->NewStringUTF(RSTRING_PTR(rLocation)));
    }

    VALUE rNotes = rb_hash_aref(rEvent, rb_str_new2(RUBY_EV_NOTES));
    if (!NIL_P(rNotes))
    {
        Check_Type(rNotes, T_STRING);
        env->SetObjectField(jEvent, fidNotes, env->NewStringUTF(RSTRING_PTR(rNotes)));
    }

    return jEvent;
}

static VALUE eventToRuby(jobject jEvent)
{
    JNIEnv *env = jnienv();
    if (!initEventStuff(env))
        return Qnil;

    VALUE rEvent = rb_hash_new();

    jstring jId = (jstring)env->GetObjectField(jEvent, fidId);
    std::string s = rho_cast<std::string>(env, jId);
    env->DeleteLocalRef(jId);
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_ID), rb_str_new2(s.c_str()));

    jstring jTitle = (jstring)env->GetObjectField(jEvent, fidTitle);
    s = rho_cast<std::string>(env, jTitle);
    env->DeleteLocalRef(jTitle);
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_TITLE), rb_str_new2(s.c_str()));

    jobject jStartDate = env->GetObjectField(jEvent, fidStartDate);
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_START_DATE), dateToRuby(jStartDate));
    env->DeleteLocalRef(jStartDate);

    jobject jEndDate = env->GetObjectField(jEvent, fidEndDate);
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_END_DATE), dateToRuby(jEndDate));
    env->DeleteLocalRef(jEndDate);

    jobject jLastModified = env->GetObjectField(jEvent, fidLastModified);
    rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_LAST_MODIFIED), dateToRuby(jLastModified));
    env->DeleteLocalRef(jLastModified);

    jstring jLocation = (jstring)env->GetObjectField(jEvent, fidLocation);
    if (jLocation)
    {
        s = rho_cast<std::string>(env, jLocation);
        env->DeleteLocalRef(jLocation);
        rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_LOCATION), rb_str_new2(s.c_str()));
    }

    jstring jNotes = (jstring)env->GetObjectField(jEvent, fidNotes);
    if (jNotes)
    {
        s = rho_cast<std::string>(env, jNotes);
        env->DeleteLocalRef(jNotes);
        rb_hash_aset(rEvent, rb_str_new2(RUBY_EV_NOTES), rb_str_new2(s.c_str()));
    }

    return rEvent;
}

RHO_GLOBAL VALUE event_fetch(VALUE start_date, VALUE end_date)
{
    RHO_LOG_CALLBACK;

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_EVENT_STORE);
    if (!cls) return Qnil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "fetch", "(Ljava/util/Date;Ljava/util/Date;)Ljava/util/Vector;");
    if (!mid) return Qnil;

    RAWLOG_INFO("event_fetch (1)");
    jobject jStartDate = dateFromRuby(start_date);
    RAWLOG_INFO("event_fetch (2)");
    jobject jEndDate = dateFromRuby(end_date);
    RAWLOG_INFO("event_fetch (3)");
    jobject jEvents = env->CallStaticObjectMethod(cls, mid, jStartDate, jEndDate);
    RAWLOG_INFO("event_fetch (4)");
    env->DeleteLocalRef(jStartDate);
    env->DeleteLocalRef(jEndDate);
    if (!jEvents) return Qnil;

    jclass clsVector = getJNIClass(RHODES_JAVA_CLASS_VECTOR);
    if (!clsVector) return Qnil;
    jmethodID midSize = getJNIClassMethod(env, clsVector, "size", "()I");
    if (!midSize) return Qnil;
    jmethodID midGet = getJNIClassMethod(env, clsVector, "get", "(I)Ljava/lang/Object;");
    if (!midGet) return Qnil;

    RAWLOG_INFO("event_fetch (5)");
    VALUE ret = rb_ary_new();

    RAWLOG_INFO("event_fetch (6)");
    for (int i = 0, lim = env->CallIntMethod(jEvents, midSize); i != lim; ++i)
    {
        RAWLOG_INFO("event_fetch (6.1)");
        jobject jEvent = env->CallObjectMethod(jEvents, midGet, i);
        RAWLOG_INFO("event_fetch (6.2)");
        VALUE rEvent = eventToRuby(jEvent);
        RAWLOG_INFO("event_fetch (6.3)");
        env->DeleteLocalRef(jEvent);
        RAWLOG_INFO("event_fetch (6.4)");
        rb_ary_push(ret, rEvent);
        RAWLOG_INFO("event_fetch (6.5)");
    }

    RAWLOG_INFO("event_fetch (7)");
    env->DeleteLocalRef(jEvents);

    return ret;
}

RHO_GLOBAL VALUE event_fetch_by_id(const char *id)
{
    RHO_LOG_CALLBACK;

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_EVENT_STORE);
    if (!cls) return Qnil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "fetch", "(Ljava/lang/String;)Lcom/rhomobile/rhodes/event/Event;");
    if (!mid) return Qnil;

    jstring jId = rho_cast<jstring>(env, id);
    jobject jEvent = env->CallStaticObjectMethod(cls, mid, jId);
    VALUE rEvent = eventToRuby(jEvent);
    env->DeleteLocalRef(jId);
    env->DeleteLocalRef(jEvent);
    return rEvent;
}

RHO_GLOBAL void event_save(VALUE rEvent)
{
    RHO_LOG_CALLBACK;

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_EVENT_STORE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "save", "(Lcom/rhomobile/rhodes/event/Event;)V");
    if (!mid) return;

    jobject jEvent = eventFromRuby(rEvent);
    env->CallStaticVoidMethod(cls, mid, jEvent);
    env->DeleteLocalRef(jEvent);
}

RHO_GLOBAL void event_delete(const char *id)
{
    RHO_LOG_CALLBACK;

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_EVENT_STORE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "delete", "(Ljava/lang/String;)V");
    if (!mid) return;

    jstring jId = rho_cast<jstring>(env, id);
    env->CallStaticVoidMethod(cls, mid, jId);
    env->DeleteLocalRef(jId);
}
