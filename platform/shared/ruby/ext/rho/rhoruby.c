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

#undef RUBY_EXPORT
#include "ruby.h"
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif
#include "rhoruby.h"
#include "vm_core.h"
#include "transcode_data.h"

#ifdef ENABLE_RUBY_VM_STAT
#include "../stat/stat.h"
#endif

#ifndef RHO_RUBY_COMPILER
#include "common/app_build_capabilities.h"
#endif

#include "logging/RhoLog.h"

#ifdef RHODES_EMULATOR
#include "common/RhoSimConf.h"
#endif

#include "sync/RhoconnectClientManager.h"


#include "posixnames.h"

int rho_is_remote_debug();

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoRuby"
extern void Init_strscan();
extern void Init_sqlite3_api();
extern void Init_GeoLocation(void);
extern void print_profile_report();
extern void Init_System(void);
extern void Init_Phonebook(void);
extern void Init_WebView(void);
extern void Init_RhoConf(void);
extern void Init_Alert(void);
extern void Init_Camera(void);
extern void Init_SignatureCapture(void);
extern void Init_RhoBluetooth(void);
extern void Init_RhodesNativeViewManager(void);
extern void Init_stringio(void);
extern void Init_DateTimePicker(void);
extern void Init_NativeBar(void);
extern void Init_MapView(void);
extern void Init_RingtoneManager(void);
extern void Init_socket(void);
extern void Init_AsyncHttp(void);
extern void Init_NavBar(void);
extern void Init_RhoEvent(void);
extern void Init_Calendar(void);
extern void Init_Extensions(void);
extern void init_rhoext_Signature();
extern void Init_encdb(void);
extern void ruby_init_prelude(void);
extern void Init_transcode(void);


//RhoSupport extension
extern void Init_RhoSupport(void);
extern VALUE require_compiled(VALUE fname, VALUE* result);
extern VALUE RhoPreparePath(VALUE path);
extern const char* rho_native_rhopath();
extern const char* rho_native_reruntimepath();
extern const char* rho_native_rhouserpath();
extern const char* rho_get_remote_debug_host();
extern const char* rho_native_rhopath();

static VALUE  framework;
static ID framework_mid;
static ID framework_mid2;
static ID initApp_mid;
static ID onConfigConflicts_mid;
static ID activateApp_mid;
static ID deactivateApp_mid;
static ID uiCreated_mid;
static ID uiDestroyed_mid;
static ID loadServerSources_mid;
static ID loadAllSyncSources_mid;
static ID resetDBOnSyncUserChanged_mid;

static char* rb_type_to_s(VALUE obj);
//extern int ruby_thread_set_native(rb_thread_t *th);
//extern int native_mutex_lock(rb_thread_lock_t *);
//extern void Init_native_thread2(rb_thread_t *th);
extern int rho_conf_getBool(const char* szName);

static int extensions_loaded = 0;

#if defined(WIN32)
extern void rb_w32_sysinit(int *argc, char ***argv);
#endif

/*
void RhoRubyThreadStart()
{
    rb_thread_t *th;

    VALUE self = rb_thread_alloc(rb_cThread);
    GetThreadPtr(self, th);

#if defined( WINDOWS_PLATFORM )
    ruby_thread_init_stack(th);
#endif
    ruby_thread_set_native(th);

    rb_gc_register_mark_object(self);

    Init_native_thread2(th);

    //st_insert(th->vm->living_threads, 0, (st_data_t) th->thread_id);
//	RhoRuby_RhomAttribManager_add_attrib(0, "test");
    //native_mutex_lock(&th->vm->global_vm_lock);
} */
/*
void RhoRubyThreadStop()
{
    //rb_thread_t *th = GET_THREAD();
    //st_delete(th->vm->living_threads, &(th->self), 0);
    //native_mutex_unlock(&th->vm->global_vm_lock);

    //native_mutex_destroy(&th->interrupt_lock);
} */


#if !defined(OS_SYMBIAN) && (defined(RHO_SYMBIAN))// || defined (RHODES_EMULATOR))
int   daylight;
char *tzname[2];
#endif

void RhoModifyRubyLoadPath( const char* );

#if defined(OS_MACOSX) || defined(OS_ANDROID)

void Init_trans_utf_16_32();
void Init_trans_single_byte();

void Init_transcoders() {
    //TRANS_INIT(utf_16_32);
    //TRANS_INIT(single_byte);
    Init_trans_utf_16_32();
    Init_trans_single_byte();
}
#endif


void RhoRubyStart()
{
    const char* szRoot = rho_native_rhopath();
//RHO
#if defined(_WIN32)
    char* av[] = { "" };
#endif
#ifdef HAVE_LOCALE_H
    setlocale(LC_CTYPE, "");
#endif
    {
#ifdef ENABLE_RUBY_VM_STAT
    g_collect_stat = 1;
#endif

    RUBY_INIT_STACK;

#if defined(_WIN32)
    rb_w32_sysinit(0,av);
#endif

    ruby_init();
    Init_encdb();
#if defined(OS_MACOSX) || defined(OS_ANDROID)
    Init_transcode();
    Init_transcoders();
#endif

#if defined(DEBUG)
    //enable_gc_profile();
#endif
    //rb_funcall(rb_mGC, rb_intern("stress="), 1, Qtrue);

    ruby_init_loadpath(szRoot);

    RhoModifyRubyLoadPath( szRoot );

    ruby_init_prelude();
    

#if !defined(OS_WP8) && !defined(OS_UWP)

    Init_strscan(); //+
    Init_GeoLocation(); //+

    Init_Phonebook();
#if !defined(OS_MACOSX) && !defined( OS_WINDOWS_DESKTOP ) && !defined(OS_WINCE) && !defined(OS_ANDROID)
    Init_WebView(); //+
#endif

#if !defined(RHO_NO_RUBY)
    Init_RhoConf(); //+
#endif

    Init_RhoBluetooth();
	Init_RhodesNativeViewManager();
#if !defined(OS_MACOSX) && !defined(OS_ANDROID)
    Init_Camera();
#endif
    Init_stringio(); //+
    Init_DateTimePicker();

    Init_RhoSupport(); //+
    Init_MapView();                         
    Init_RingtoneManager();
    Init_socket(); //+
    Init_RhoEvent();
    Init_Calendar();

    Init_Extensions();

#else // OS_WP8 is set
	Init_strscan();
	
    Init_GeoLocation();
	Init_NavBar();
	Init_RhoSupport();
	Init_RhoConf();
	Init_Alert();

	Init_socket();
	Init_stringio();

	Init_Extensions();
#endif //OS_WP8
 
	extensions_loaded = 1;

    if ( rho_rcclient_have_rhoconnect_client() ) {
        rb_const_set(rb_cObject, rb_intern("RHOCONNECT_CLIENT_PRESENT"), Qtrue);
    }

    if (rho_is_remote_debug())
    {
        rb_const_set(rb_cObject, rb_intern("RHOSTUDIO_REMOTE_DEBUG"), Qtrue);
        rb_const_set(rb_cObject, rb_intern("RHOSTUDIO_REMOTE_HOST"), rho_ruby_create_string(rho_get_remote_debug_host()));
        rb_const_set(rb_cObject, rb_intern("RHOSTUDIO_REMOTE_APPPATH"), rho_ruby_create_string(rho_native_rhopath()));
    }
    else
    {
        rb_const_set(rb_cObject, rb_intern("RHOSTUDIO_REMOTE_DEBUG"), Qfalse);
    }

#ifdef ENABLE_RUBY_VM_STAT
    struct timeval  start;
    struct timeval  end;

    gettimeofday (&start, NULL);
#endif

#ifdef RHODES_EMULATOR
    require_compiled(rb_str_new2("rhoframework"), &framework );
    framework = rb_const_get(rb_cObject,rb_intern("RHO_FRAMEWORK"));
#else
    {
		VALUE res = rho_ruby_disable_gc();

        require_compiled(rb_str_new2("rhoframework"), &framework );
        rho_ruby_enable_gc(res);
    }
#endif //RHODES_EMULATOR

    if ( framework == 0 || framework == Qnil )
    {
        RAWLOG_FATAL("RHO framework creating failed. Application will exit.");
    }

#ifdef ENABLE_RUBY_VM_STAT
    gettimeofday (&end, NULL);

    if ( end.tv_sec > 0 )
        g_require_compiled_msec += (end.tv_sec  - start.tv_sec) * 1000;
    else
        g_require_compiled_msec += (end.tv_usec - start.tv_usec)/1000;

#endif

    rb_gc_register_mark_object(framework);

    CONST_ID(framework_mid, "serve");
    CONST_ID(framework_mid2, "serve_index");
    CONST_ID(initApp_mid, "init_app");
    CONST_ID(onConfigConflicts_mid, "on_config_conflicts");
    CONST_ID(activateApp_mid, "activate_app");
    CONST_ID(deactivateApp_mid, "deactivate_app");
    CONST_ID(uiCreated_mid, "ui_created");
    CONST_ID(uiDestroyed_mid, "ui_destroyed");
    CONST_ID(loadServerSources_mid,"load_server_sources");
    CONST_ID(loadAllSyncSources_mid,"load_all_sync_sources");
    CONST_ID(resetDBOnSyncUserChanged_mid, "reset_db_on_sync_user_changed");

    //moduleRhom = rb_const_get(rb_cObject, rb_intern("Rhom"));

#ifdef ENABLE_RUBY_VM_STAT
    g_collect_stat = 0;
#endif

    }
}



void RhoModifyRubyLoadPath( const char* szRoot ) {
#if defined(RHODES_EMULATOR) || defined(APP_BUILD_CAPABILITY_SYMBOL) || defined(OS_WP8) || defined(OS_UWP)
        VALUE load_path = GET_VM()->load_path;

        const int app_path_len = strlen(szRoot)+100;
        char* app_path = malloc(app_path_len);

        rb_ary_clear(load_path);

        strcpy(app_path, szRoot);
#if defined(RHODES_EMULATOR)
        strcat(app_path, "app");
#elif defined(OS_WP8) //|| defined(OS_UWP)
        strcat(app_path, "/apps/app");
#else
        strcat(app_path, "apps/app");
#endif

        rb_ary_push(load_path, rb_str_new2(app_path) );

#if defined(APP_BUILD_CAPABILITY_SYMBOL)
        strcpy(app_path, rho_native_reruntimepath());
        strcat(app_path, "lib");
#elif defined(OS_WP8)
        strcpy(app_path, szRoot);
        strcat(app_path, "/lib");
#elif defined(OS_UWP)
		strcpy(app_path, szRoot);
		strcat(app_path, "lib");
#else
        strcpy(app_path, rho_simconf_getRhodesPath());
        strcat(app_path, "/lib/framework");
#endif

        rb_ary_push(load_path, rb_str_new2(app_path) );

/*
#ifdef RHODES_EMULATOR
        const char*p0, *p1;

        p0=p1=rho_simconf_getString( "ext_path" );

        while ((*p0 != 0)) {

            //find next token
            for ( ; (*p1!=0 && *p1!=';'); ++p1 ) ;

            int len = p1-p0;

            if ( len > (app_path_len-1) ) {
                RAWLOG_FATAL( "Buffer length for ext path is too short" );
                return;
            }

            strncpy(app_path,p0,len);
            app_path[len]=0;
            rb_ary_push(load_path, rb_str_new2(app_path) );

            ++p1;
            p0=p1;
        }


        strcpy(app_path, rho_simconf_getRhodesPath());
        strcat(app_path, "/lib/framework");
        rb_ary_push(load_path, rb_str_new2(app_path) );

#endif
*/
#endif
}

int  rho_ruby_is_started()
{
    return rb_mKernel != 0 ? 1 : 0;
}

int  rho_extensions_are_loaded()
{
	return extensions_loaded;
}

int rho_ruby_is_enabled_eval()
{
#ifdef RHODES_EMULATOR
    return 1;
#else
    return 1;
#endif
}

void RhoRubyInitApp()
{
    rb_funcall(framework, initApp_mid, 0);
}

#ifndef RHO_NO_RUBY

VALUE rho_conf_get_conflicts();
void rho_ruby_call_config_conflicts()
{
    VALUE hashConflicts = rho_conf_get_conflicts();
    if ( RHASH_EMPTY_P(hashConflicts) )
        return;

    rb_funcall(framework, onConfigConflicts_mid, 1, hashConflicts);
}
#else
void rho_ruby_call_config_conflicts() {
}
#endif

void rho_ruby_activateApp()
{
    rb_funcall(framework, activateApp_mid, 0);
}

void rho_ruby_deactivateApp()
{
    rb_funcall(framework, deactivateApp_mid, 0);
}

void rho_ruby_uiCreated()
{
    rb_funcall(framework, uiCreated_mid, 0);
}

void rho_ruby_uiDestroyed()
{
    rb_funcall(framework, uiDestroyed_mid, 0);
}

void rho_ruby_loadserversources(const char* szData)
{
    rb_funcall(framework, loadServerSources_mid, 1, rb_str_new2(szData) );
}

void rho_ruby_loadallsyncsources()
{
    rb_funcall(framework, loadAllSyncSources_mid, 0 );
}

void rho_ruby_callmethod(const char* szMethodPath)
{
    ID rbMethodCall;
    CONST_ID(rbMethodCall, szMethodPath);
    rb_funcall(Qnil, rbMethodCall, 0);
}

VALUE rho_ruby_callmethod_arg(VALUE classValue, const char *name, VALUE arg)
{
    return rb_funcall(classValue, rb_intern(name), 1, arg);
}

void rho_ruby_reset_db_on_sync_user_changed()
{
    rb_funcall(framework, resetDBOnSyncUserChanged_mid, 0);
}

const char* rho_ruby_getRhoDBVersion()
{
    VALUE moduleRhodes = rb_const_get(rb_cObject, rb_intern("Rhodes"));
    VALUE valVer = rb_const_get(moduleRhodes, rb_intern("DBVERSION"));
    return RSTRING_PTR(valVer);
}

void RhoRubyStop()
{
    //TBD: clenup framework, etc.
    ruby_cleanup(0);

    rb_mKernel = 0;
}

char* makeControllerCall(char* classname, char* methodname);

//typedef void rho_eachstr_func(const char*, const char*, void*);
struct CHashEnumStrData
{
    void* data;
    rho_hash_eachstr_func *func;
};

struct CHashEnumData
{
    void* data;
    rho_hash_each_func *func;
};

static int
hash_each_str(VALUE key, VALUE value, struct CHashEnumStrData* pEnumData)
{
    const char* szValue = "";
    int valueLen = 0;
    const char* szKey = "";

    if ( value != 0 && value != Qnil )
    {
        VALUE strVal = rb_funcall(value, rb_intern("to_s"), 0);
        szValue = RSTRING_PTR(strVal);
        valueLen = RSTRING_LEN(strVal);
    }
    if ( key != 0 && key != Qnil )
    {
        VALUE strKey = rb_funcall(key, rb_intern("to_s"), 0);
        szKey = RSTRING_PTR(strKey);
    }

    (*pEnumData->func)(szKey, szValue, valueLen, pEnumData->data );
    return ST_CONTINUE;
}

void rho_ruby_enum_strhash(VALUE hash, rho_hash_eachstr_func * func, void* data)
{
    struct CHashEnumStrData enumData;

    if ( !hash || hash ==Qnil )
        return;

    enumData.data = data;
    enumData.func = func;

    rb_hash_foreach(hash, hash_each_str, (VALUE)(&enumData));
}

static int
hash_each_json(VALUE key, VALUE value, struct CHashEnumStrData* pEnumData)
{
    const char* szValue = "";
    int valueLen = 0;
    const char* szKey = "";

    if ( value != 0 && value != Qnil )
    {
        VALUE strVal;
        if ( TYPE(value) != T_STRING ) {
            if (TYPE(value) == T_FIXNUM || TYPE(value) == T_TRUE || TYPE(value) == T_FALSE ||
                TYPE(value) == T_FLOAT || TYPE(value) == T_BIGNUM || TYPE(value) == T_SYMBOL || TYPE(value) == T_RATIONAL || TYPE(value) == T_COMPLEX )
                strVal = rb_funcall(value, rb_intern("to_s"), 0);
            else
                strVal = rb_funcall(value, rb_intern("to_json"), 0);

        } else
            strVal = value;

        szValue = RSTRING_PTR(strVal);
        valueLen = RSTRING_LEN(strVal);
    }
    if ( key != 0 && key != Qnil )
    {
        VALUE strKey = rb_funcall(key, rb_intern("to_s"), 0);
        szKey = RSTRING_PTR(strKey);
    }

    (*pEnumData->func)(szKey, szValue, valueLen, pEnumData->data );
    return ST_CONTINUE;
}

void rho_ruby_enum_strhash_json(VALUE hash, rho_hash_eachstr_func *func, void* data)
{
    struct CHashEnumStrData enumData;

    if ( !hash || hash ==Qnil )
        return;

    enumData.data = data;
    enumData.func = func;

    rb_hash_foreach(hash, hash_each_json, (VALUE)(&enumData));
}

static int
hash_each(VALUE key, VALUE value, struct CHashEnumData* pEnumData)
{
    const char* szKey = "";

    if ( key != 0 && key != Qnil )
    {
        VALUE strKey = rb_funcall(key, rb_intern("to_s"), 0);
        szKey = RSTRING_PTR(strKey);
    }

    (*pEnumData->func)(szKey, value, pEnumData->data);
    return ST_CONTINUE;
}

void rho_ruby_enum_hash(VALUE hash, rho_hash_each_func * func, void* data)
{
    struct CHashEnumData enumData;

    if ( !hash || hash == Qnil )
        return;

    enumData.data = data;
    enumData.func = func;

    rb_hash_foreach(hash, hash_each, (VALUE)(&enumData));
}

void rho_ruby_enum_strary(VALUE ary, rho_ary_eachstr_func * func, void* data)
{
    int i = 0;

    if ( ary ==0 || ary == Qnil )
        return;

    for (i=0; i<RARRAY_LEN(ary); i++)
    {
        VALUE value = RARRAY_PTR(ary)[i];
        const char* szValue = "";
        int valueLen = 0;
        if ( value != 0 && value != Qnil )
        {
            VALUE strVal = rb_funcall(value, rb_intern("to_s"), 0);
            szValue = RSTRING_PTR(strVal);
            valueLen = RSTRING_LEN(strVal);
        }

        (*func)(szValue, valueLen, data );
    }
}

void rho_ruby_enum_strary_json(VALUE ary, rho_ary_eachstr_func * func, void* data)
{
    int i = 0;

    if ( ary ==0 || ary == Qnil )
        return;

    for (i=0; i<RARRAY_LEN(ary); i++)
    {
        VALUE value = RARRAY_PTR(ary)[i];
        const char* szValue = "";
        int valueLen = 0;
        if ( value != 0 && value != Qnil )
        {
            VALUE strVal;
            if ( TYPE(value) == T_STRING || TYPE(value) == T_FIXNUM || TYPE(value) == T_TRUE || TYPE(value) == T_FALSE ||
                 TYPE(value) == T_FLOAT || TYPE(value) == T_BIGNUM || TYPE(value) == T_SYMBOL || TYPE(value) == T_RATIONAL || TYPE(value) == T_COMPLEX )
                strVal = rb_funcall(value, rb_intern("to_s"), 0);
            else
                strVal = rb_funcall(value, rb_intern("to_json"), 0);

            szValue = RSTRING_PTR(strVal);
            valueLen = RSTRING_LEN(strVal);
        }

        (*func)(szValue, valueLen, data );
    }
}

void rho_ruby_enum_ary(VALUE ary, rho_ary_each_func * func, void* data)
{
    int i = 0;

    if ( ary ==0 || ary == Qnil )
        return;

    for (i=0; i<RARRAY_LEN(ary); i++)
    {
        VALUE value = RARRAY_PTR(ary)[i];
        (*func)(value, data );
    }
}

VALUE rho_ruby_get_NIL()
{
    return Qnil;
}

int rho_ruby_is_NIL(VALUE val)
{
    return NIL_P(val);
}

int rho_ruby_get_bool(VALUE val)
{
    return val == Qtrue;
}

long rho_ruby_get_int(VALUE val)
{
    return NUM2LONG(val);
}

double rho_ruby_get_double(VALUE val)
{
    return RFLOAT_VALUE(val);
}

VALUE rho_ruby_get_time(VALUE rDate)
{
    VALUE res, cDate;
    if (TYPE(rDate) == T_STRING) {
        rDate = rb_funcall(rb_cTime, rb_intern("parse"), 1, rDate);
    }

    cDate = rb_class_of(rDate);
    if (!rb_equal(cDate, rb_cTime))
        rb_raise(rb_eArgError, "Wrong type of parameter: %s (Time expected)", rb_class2name(cDate));

    return rDate;
}

unsigned long rho_ruby_get_time_part(VALUE rDate, const char* szPart)
{
    return NUM2LONG(rb_funcall(rDate, rb_intern(szPart), 0));
}

VALUE rho_ruby_create_array()
{
    return rb_ary_new();
}

VALUE rho_ruby_create_string(const char* szVal)
{
    return rb_utf8_str_new_cstr(szVal);
    //return rb_str_new2(szVal);
}

VALUE rho_ruby_create_string_withlen(int len)
{
    return rb_utf8_str_new("", len);
    //return rb_str_new("", len);
}

VALUE rho_ruby_create_string_withlen2(const char* szVal, int len)
{
    return rb_utf8_str_new(szVal, len);
    //return rb_str_new(szVal, len);
}

VALUE rho_ruby_create_time(long t)
{
    return rb_time_new(t,0);
}

VALUE rho_ruby_create_boolean(unsigned char b)
{
    return b ? Qtrue : Qfalse;
}

VALUE rho_ruby_create_integer(__int64 i)
{
    return rb_ll2inum(i);
}

VALUE rho_ruby_create_double(double d)
{
    return rb_float_new(d);
}

void rho_ruby_add_to_array(VALUE ar, VALUE val)
{
    rb_ary_push(ar,val);
}

void rho_ruby_add_to_hash(VALUE hash, VALUE key, VALUE item) {
	rb_hash_aset(hash, key, item);
}

VALUE rho_ruby_create_byte_array(unsigned char* buf, int length) {
	VALUE ar = rb_ary_new2(length);
	int i;
	for (i = 0 ; i < length; i++) {
		rb_ary_push(ar,rb_ll2inum(buf[i]));
	}
	return ar;
}

int rho_ruby_unpack_byte_array(VALUE array_value, unsigned char* buf, int max_length)
{
    int size = 0;
	int i = 0;

	if (TYPE(array_value) != T_ARRAY) {
		return -1;
	}
	size = RARRAY_LEN(array_value);
	if (buf == NULL) {
		return size;
	}
	if (size > max_length) {
		size = max_length;
	}
	for (i = 0; i < size; i++) {
		VALUE item = rb_ary_entry( array_value, i);
		long n = rb_num2ll(item);
		buf[i] = (unsigned char)n;
	}
	return size;
}

int rho_ruby_array_get_size(VALUE ar) {
    int size = 0;
	if (TYPE(ar) != T_ARRAY) {
		return -1;
	}
	size = RARRAY_LEN(ar);
    return size;
}




VALUE rho_ruby_createHash() {
    return rb_hash_new();
}

VALUE addTimeToHash(VALUE hash, const char* key, time_t val) {
    return rb_hash_aset(hash, rb_str_new2(key), rb_time_new(val,0));
}

VALUE addIntToHash(VALUE hash, const char* key, int val) {
    return rb_hash_aset(hash, rb_str_new2(key), INT2FIX(val));
}

VALUE addBoolToHash(VALUE hash, const char* key, int val)
{
    return rb_hash_aset(hash, rb_str_new2(key), (val ? Qtrue : Qfalse) );
}

VALUE addStrToHash(VALUE hash, const char* key, const char* val) {
    return rb_hash_aset(hash, rb_str_new2(key), rb_str_new2(val));
}

VALUE addStrToHashLen(VALUE hash, const char* key, const char* val, int len) {
    return rb_hash_aset(hash, rb_str_new2(key), rb_str_new(val, len));
}

VALUE addHashToHash(VALUE hash, const char* key, VALUE val) {
    return rb_hash_aset(hash, rb_str_new2(key), val);
}

VALUE rho_ruby_hash_aref(VALUE hash, const char* key)
{
    return rb_hash_aref( hash, rb_str_new2(key));
}

VALUE rho_ruby_hash_get(VALUE hash, VALUE key)
{
    return rb_hash_aref(hash, key);
}

VALUE rho_ruby_array_get(VALUE ar, int nIndex)
{
    return rb_ary_entry( ar, nIndex);
}

char* getStringFromValue(VALUE val){
    if (val == 0) {
        return NULL;
    }
    if (TYPE(val) != T_STRING) {
        return NULL;
    }
    return RSTRING_PTR(val);
}

int getStringLenFromValue(VALUE val){
    return RSTRING_LEN(val);
}

rho_param *rho_param_fromvalue(VALUE v)
{
    int i, size;
/*
    if (v == NULL) {
         return NULL;
    }

    if (rho_ruby_is_NIL(v)) {
         return NULL;
    }
*/
    switch(TYPE(v)) {
    case T_ARRAY:
        {
            rho_param *p = 0;
            size = RARRAY_LEN(v);
            p = rho_param_array(size);
            for (i = 0; i < size; ++i) {
                VALUE item = rb_ary_entry(v, i);
                p->v.array->value[i] = rho_param_fromvalue(item);
            }
            return p;
        }
        break;
    case T_HASH:
        {
            VALUE keys = rb_funcall(v, rb_intern("keys"), 0, NULL);
            rho_param *p = 0;
            size = RARRAY_LEN(keys);
            p = rho_param_hash(size);
            for (i = 0; i < size; ++i) {
                VALUE key = rb_ary_entry(keys, i);
                VALUE value = rb_hash_aref(v, key);
                p->v.hash->name[i] = fpstrdup(RSTRING_PTR(rb_String(key)));
                p->v.hash->value[i] = rho_param_fromvalue(value);
            }
            return p;
        }
        break;
    default:
        {
            VALUE value = v;
            if (TYPE(v) != T_STRING)
                value = rb_funcall(value, rb_intern("to_s"), 0, NULL);
            return rho_param_str(RSTRING_PTR(value));
        }
    }
}

struct CRhoRubyStringOrInt rho_ruby_getstringorint(VALUE val)
{
    struct CRhoRubyStringOrInt oRes;
    oRes.m_szStr = "";
    oRes.m_nInt = 0;

    if (TYPE(val) == T_FIXNUM || TYPE(val) == T_BIGNUM)
        oRes.m_nInt = rb_num2ll(val);
    else
    {
        VALUE strVal = rb_funcall(val, rb_intern("to_s"), 0);
        oRes.m_szStr = RSTRING_PTR(strVal);
    }

    return oRes;
}

VALUE rho_ruby_disable_gc()
{
    return rb_gc_disable() == Qtrue ? Qfalse : Qtrue;
}

void  rho_ruby_enable_gc(VALUE val)
{
    if ( val == Qtrue )
        rb_gc_enable();
    else
        rb_gc_disable();
}

void rho_ruby_holdValue(VALUE val)
{
    rb_gc_register_mark_object(val);
}

void rho_ruby_releaseValue(VALUE val)
{
    VALUE ary = GET_THREAD()->vm->mark_object_ary;
    int i = RARRAY_LEN(ary)-1;
    for ( ; i >= 0; i-- ) {
        if ( RARRAY_PTR(ary)[i]== val )
            break;
    }
    if ( i >= 0 )
        rb_ary_delete_at(ary,i);
}

extern VALUE rb_cMutex;
VALUE rho_ruby_create_mutex()
{
    VALUE val = rb_funcall(rb_cMutex, rb_intern("new"), 0 );
    rho_ruby_holdValue(val);

    return val;
}

void rho_ruby_destroy_mutex(VALUE val)
{
    rho_ruby_releaseValue(val);
}

void rho_ruby_lock_mutex(VALUE val)
{
    rb_mutex_lock(val);
}

void rho_ruby_unlock_mutex(VALUE val)
{
    rb_mutex_unlock(val);
}

void rho_ruby_raise_runtime(const char* szText)
{
    rb_raise(rb_eRuntimeError, szText );
}

void rho_ruby_raise_argerror(const char *fmt, ...)
{
	va_list args;
	VALUE mesg;

	va_start(args, fmt);
	mesg = rb_vsprintf(fmt, args);
	va_end(args);
	rb_exc_raise(rb_exc_new3(rb_eArgError, mesg));
}

VALUE rho_ruby_main_thread()
{
    if (!rho_ruby_is_started())
        return Qnil;

    return rb_thread_main();
}

VALUE rho_ruby_current_thread()
{
    if (!rho_ruby_is_started())
        return Qnil;

    if ( ruby_native_thread_p() != 1 )
        return Qnil;

    return rb_thread_current();
}

VALUE callFramework(VALUE hashReq) {
    VALUE callres = rb_funcall(framework, framework_mid, 1, hashReq);

    if (TYPE(callres)!=T_STRING) {
        RAWLOG_INFO1("Method call result type = %s", rb_type_to_s(callres));
        return rb_str_new2("Error");//TBD: Supply html description of the error
    }
    if ( !rho_conf_getBool("log_skip_post") )
        RAWTRACE(RSTRING_PTR(callres));

    rb_gc_register_mark_object(callres);
    //TBD: need to cleanup memory
    //rb_gc();

    return callres;
}

VALUE callServeIndex(char* index_name, VALUE hashReq) {
    VALUE callres;
    //RhoSetCurAppPath(index_name);
    callres = rb_funcall(framework, framework_mid2, 2, RhoPreparePath(rb_str_new2(index_name)), hashReq);

    if (TYPE(callres)!=T_STRING) {
        RAWLOG_INFO1("Method call result type = %s", rb_type_to_s(callres));
        return rb_str_new2("Error");//TBD: Supply html description of the error
    }

    if ( !rho_conf_getBool("log_skip_post") )
        RAWTRACE(RSTRING_PTR(callres));

    rb_gc_register_mark_object(callres);

    //TBD: need to cleanup memory
    //rb_gc();

    return callres;
}

/*static int s_gcWasDisabled;
//extern void native_sleep(rb_thread_t *th, struct timeval *tv);
extern void sleep_for_polling(rb_thread_t *th);
static void start_ruby_call()
{
    rb_thread_t *th = GET_THREAD();

    while( rb_during_gc() )
    {
        sleep_for_polling(th);
    }

    s_gcWasDisabled = rb_gc_disable();
}
static void end_ruby_call()
{
    if ( !s_gcWasDisabled )
        rb_gc_enable();
}*/

static char*
rb_type_to_s(VALUE obj)
{
  VALUE type;

  if (IMMEDIATE_P(obj)) {
        if (FIXNUM_P(obj)) return "T_FIXNUM";
        if (obj == Qtrue) return "T_TRUE";
        if (SYMBOL_P(obj)) return "T_SYMBOL";
        if (obj == Qundef) return "T_UNDEF";
  } else if (!RTEST(obj)) {
        if (obj == Qnil) return "T_NIL";
        if (obj == Qfalse) return "T_FALSE";
  }

  type = BUILTIN_TYPE(obj);
    switch(type) {
        case T_NONE:
            return "RUBY_T_NONE";
        case T_NIL:
            return "RUBY_T_NIL";
        case T_OBJECT:
            return "RUBY_T_OBJECT";
        case T_CLASS:
            return "RUBY_T_CLASS";
        case T_ICLASS:
            return "RUBY_T_ICLASS";
        case T_MODULE:
            return "RUBY_T_MODULE";
        case T_FLOAT:
            return "RUBY_T_FLOAT";
        case T_STRING:
            return "RUBY_T_STRING";
        case T_REGEXP:
            return "RUBY_T_REGEXP";
        case T_ARRAY:
            return "RUBY_T_ARRAY";
        case T_HASH:
            return "RUBY_T_HASH";
        case T_STRUCT:
            return "RUBY_T_STRUCT";
        case T_BIGNUM:
            return "RUBY_T_BIGNUM";
        case T_FILE:
            return "RUBY_T_FILE";
        case T_FIXNUM:
            return "RUBY_T_FIXNUM";
        case T_TRUE:
            return "RUBY_T_TRUE";
        case T_FALSE:
            return "RUBY_T_FALSE";
        case T_DATA:
            return "RUBY_T_DATA";
        case T_MATCH:
            return "RUBY_T_MATCH";
        case T_SYMBOL:
            return "RUBY_T_SYMBOL";
        case T_RATIONAL:
            return "RUBY_T_RATIONAL";
        case T_COMPLEX:
            return "RUBY_T_COMPLEX";
        case T_UNDEF:
            return "RUBY_T_UNDEF";
        case T_NODE:
            return "RUBY_T_NODE";
        case T_ZOMBIE:
            return "RUBY_T_ZOMBIE";
        default:
            return "Unknown";
    }
}

int rho_is_eval_disabled()
{
    return !rho_conf_getBool("enable_eval");
}

static void
obj_free(void *p)
{
    ruby_xfree(p);
}

static VALUE obj_allocate(VALUE klass)
{
	return Data_Wrap_Struct(klass, 0, obj_free, ALLOC(void*));
}

VALUE rho_ruby_create_object_with_id( VALUE klass, const char* szID )
{
    char ** ppString = NULL;

    VALUE valObj = obj_allocate(klass);
    Data_Get_Struct(valObj, char *, ppString);
    *ppString = xmalloc(strlen(szID)+1);
    strcpy(*ppString, szID);

    return valObj;
}

int rho_ruby_is_string(VALUE val)
{
    return (TYPE(val) == T_STRING) ? 1 : 0;
}

int rho_ruby_is_integer(VALUE val)
{
    return (TYPE(val) == T_FIXNUM) ? 1 : 0;
}

int rho_ruby_is_boolean(VALUE val)
{
    return (TYPE(val) == T_TRUE || TYPE(val) == T_FALSE) ? 1 : 0;
}

int rho_ruby_is_double(VALUE val)
{
    return (TYPE(val) == T_FLOAT) ? 1 : 0;
}

int rho_ruby_is_array(VALUE val)
{
    return (TYPE(val) == T_ARRAY) ? 1 : 0;
}

int rho_ruby_is_hash(VALUE val)
{
    return (TYPE(val) == T_HASH) ? 1 : 0;
}

int isNil(VALUE value) {
    return (NIL_P(value) || value == 0);
}

int rho_ruby_to_int(VALUE val, int* dest) {
    if (isNil(val) && dest == 0)
        return 0;
    *dest = 0;
    if (TYPE(val) == RUBY_T_FLOAT ||
        TYPE(val) == RUBY_T_FIXNUM ||
        TYPE(val) == RUBY_T_BIGNUM ||
        TYPE(val) == RUBY_T_RATIONAL) {
        *dest = NUM2INT(val);
        return 1;
    }
    return 0;

}
int rho_ruby_to_double(VALUE val, double* dest) {
    if (isNil(val) && dest == 0)
        return 0;
    *dest = 0.0;
    if (TYPE(val) == RUBY_T_FLOAT ||
        TYPE(val) == RUBY_T_FIXNUM ||
        TYPE(val) == RUBY_T_BIGNUM ||
        TYPE(val) == RUBY_T_RATIONAL) {
        *dest = NUM2DBL(val);
        return 1;
    }
    return 0;
}

int rho_ruby_to_bool(VALUE val, int* dest) {
    if (isNil(val) && dest == 0)
        return 0;
    *dest = 0;
    switch (TYPE(val)) {
        case RUBY_T_TRUE:
        case RUBY_T_FALSE:
            *dest = TYPE(val) == RUBY_T_TRUE;
            return 1;
            break;
        case RUBY_T_FLOAT:
        case RUBY_T_BIGNUM:
        case RUBY_T_FIXNUM:
        case RUBY_T_RATIONAL:
            *dest = NUM2INT(val) != 0;
            return 1;
            break;

        default:
            break;
    }

    return 0;
}

int rho_ruby_to_str(VALUE val, const char** dest, int* len) {
    if (rb_type(val) != T_STRING)
    {
        val = rb_funcall(val, rb_intern("to_s"), 0, NULL);
    }
    *dest = getStringFromValue(val);
    *len = getStringLenFromValue(val);
    return 1;
}

static const char* empty_str = "";

int rho_ruby_to_str_ex(VALUE val, const char** dest, int* len) {

    if (0 == dest || 0 == len) {
        return 0;
    }

    *dest = empty_str;
    *len = 0;

    if ( val != 0 && val != Qnil )
    {
        VALUE strVal;
        if ( TYPE(val) == T_STRING || TYPE(val) == T_FIXNUM || TYPE(val) == T_TRUE || TYPE(val) == T_FALSE ||
            TYPE(val) == T_FLOAT || TYPE(val) == T_BIGNUM || TYPE(val) == T_SYMBOL || TYPE(val) == T_RATIONAL || TYPE(val) == T_COMPLEX )
            strVal = rb_funcall(val, rb_intern("to_s"), 0);
        else
            strVal = rb_funcall(val, rb_intern("to_json"), 0);

        *dest = RSTRING_PTR(strVal);
        *len = RSTRING_LEN(strVal);
    }

    return 1;
}

VALUE rho_ruby_hash_keys(VALUE val) {
    return rb_funcall(val, rb_intern("keys"), 0);
}

VALUE rho_ruby_each_key(VALUE val) {
    return rb_each(val);
}

VALUE rb_require_compiled_safe(VALUE obj, VALUE fname);
VALUE rho_ruby_safe_require(const char *fname)
{
    VALUE fn = rb_str_new2(fname);
    OBJ_FREEZE(fn);

    return rb_require_compiled_safe( Qnil, fn);
}

const char* rho_ruby_get_object_id( VALUE valObj )
{
    char ** ppString = NULL;

    Data_Get_Struct(valObj, char *, ppString);

    return *ppString;
}

void rho_ruby_clear_object_id( VALUE valObj )
{
    char ** ppString = NULL;

    Data_Get_Struct(valObj, char *, ppString);

    *ppString = 0;
}

int rho_ruby_is_proc(VALUE val)
{
    return CLASS_OF(val) == rb_cProc ? 1 : 0;
}

int rho_ruby_is_method(VALUE val)
{
    return CLASS_OF(val) == rb_cMethod ? 1 : 0;
}

int rho_ruby_is_object_of_class(VALUE val, VALUE valClass)
{
    return CLASS_OF(val) == valClass ? 1 : 0;
}

VALUE rho_ruby_get_object_class(VALUE val)
{
    return CLASS_OF(val);
}

VALUE rho_ruby_get_class_byname(const char* szName)
{
    return rb_path_to_class(rb_str_new2(szName));
}

VALUE rho_ruby_getProcBinding(VALUE proc)
{
    static ID binding_mid;
    CONST_ID(binding_mid, "binding");
    return rb_funcall(proc, binding_mid, 0);
}
