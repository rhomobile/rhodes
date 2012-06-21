/**********************************************************************

  main.c -

  $Author: akr $
  created at: Fri Aug 19 13:19:58 JST 1994

  Copyright (C) 1993-2007 Yukihiro Matsumoto

**********************************************************************/

#undef RUBY_EXPORT
#define RHO_MEMORY__
#include "ruby.h"
#include "debug.h"

#include "ruby/ext/rho/rhoruby.h"
//#include <sqlite3.h>
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif
//#include <crtdbg.h>

extern void Init_SyncEngine(void);
extern void Init_strscan(void);
extern void Init_System(void);
#define COMPILER 1
/*
 if (rb_safe_level() == 0) {
 ruby_incpush(getenv("RUBYLIB"));
    }
[04.01.2009 11:44:19] evgeny vovchenko says: ˝ÚÓ Ì‡‰Ó Û·‡Ú¸
[04.01.2009 11:44:35] evgeny vovchenko says: ruby_init_gems(!(opt->disable & DISABLE_BIT(gems)));
*/
//RUBY_GLOBAL_SETUP

#ifndef COMPILER
int
main(int argc, char **argv)
{
//    MessageBox(0,"","",MB_OK);
    ruby_sysinit(&argc, &argv);

    RhoRubyStart();
    RhoRubyInitApp();
    RhoRubyStop();
}

/*const char* RhoGetRootPath()
{
    return "D:/Projects/rhodes1/rhodes/win32/bin/RhoBundle/";
}*/

#else 
#include "vm_core.h"
static VALUE
__rho_compile( VALUE obj, VALUE src);

int
main(int argc, char **argv)
{
    int nRes = 0;

    //_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF);

#ifdef RUBY_DEBUG_ENV
    ruby_set_debug_option(getenv("RUBY_DEBUG"));
#endif
#ifdef HAVE_LOCALE_H
    setlocale(LC_CTYPE, "");
#endif

#ifdef WIN32
    SetEnvironmentVariable("RUBYOPT","");
    SetEnvironmentVariable("RUBYLIB","");
#endif //WIN32

//    MessageBox(0,"","",MB_OK);
    ruby_sysinit(&argc, &argv);
    {
	RUBY_INIT_STACK;
	ruby_init();
    Init_strscan();
	//Init_sqlite3_api();
    Init_SyncEngine();
    Init_System();
    //Init_prelude();

    //rb_gc_disable();

    rb_define_global_function("__rho_compile", __rho_compile, 1);

	nRes = ruby_run_node(ruby_options(argc, argv));

    }

    return nRes;
}

int rho_rhodesapp_isrubycompiler()
{
    return 1;
}

static VALUE
__rho_compile( VALUE obj, VALUE src)
{
    VALUE result;
    rb_thread_t *th = GET_THREAD();

    rb_secure(1);

    th->parse_in_eval++;
    result = rb_iseq_compile(src, rb_str_new2("(eval)"), INT2FIX(1));

    //result = rb_iseq_compile_with_option(src, rb_str_new2("(eval)"), Qnil, INT2FIX(1), Qtrue );
    th->parse_in_eval--;

    return result;
}
//#ifdef __APPLE__
const char* rho_native_rhopath() 
{
    return "";
}

const char* rho_rhodesapp_getblobsdirpath()
{
    return "";
}

VALUE rho_sys_makephonecall(const char* callname, int nparams, char** param_names, char** param_values) {
	return 0;
}

//#endif //__APPLE__

#endif //!COMPILER
/*
void setup_delete_db_callback(sqlite3 * db) {
}

void lock_sync_mutex() {
}

void unlock_sync_mutex() {
}

void dosync() {
}*/

VALUE rho_json_parse(VALUE v,VALUE str)
{
    return 0;
}

unsigned long rho_rhodesapp_GetCallbackObject(int nIndex)
{
    return 0;
}

void Init_GeoLocation(){
}

void Init_SyncEngine(void) {
}

void Init_Phonebook(){
}

void Init_Camera(){
}

void Init_WebView(){
}

void Init_sqlite3_api()
{
}

void Init_RhoConf()
{
}

void Init_Alert()
{
}

void Init_stringio()
{
}

void Init_DateTimePicker()
{
}

void Init_NativeBar()
{
}

void Init_RhoEvent()
{
}

void Init_Calendar()
{
}

void Init_MapView()
{
}

void Init_RingtoneManager()
{
}

void Init_socket()
{
}

void Init_Extensions()
{
}

void Init_System()
{
}

void Init_AsyncHttp()
{
}

void Init_SignatureTool()
{
}

void Init_RhoBluetooth()
{
}

void Init_RhodesNativeViewManager()
{
}

void Init_NavBar()
{
}

void init_rhoext_Signature()
{
}

const char* rho_native_reruntimepath(){ return ""; }
const char* rho_native_rhouserpath(){ return ""; }
const char* rho_native_rhodbpath(){ return ""; }

VALUE has_network(){
    return Qfalse;
}

void rhoPlainLogArg(const char* file, int line, int severity, const char* szCategory,
                    const char* format, va_list ap ){
}

void rhoPlainLogVar(const char* file, int line, int severity, const char* szCategory,
                 const char* format, ... ){
}

int rhoPlainLog(const char* file, int line, int severity, const char* szCategory,
                  const char* msg ){
    return 1;
}

VALUE rho_conf_get_conflicts()
{
    return 0;
}

rho_param* rho_param_array(int size)
{
    return 0;
}

rho_param *rho_param_str(char *s)
{
    return 0;
}

rho_param *rho_param_hash(int size)
{
    return 0;
}

int rho_conf_getBool(const char* szName) {
    return  0;
}

void rho_file_recursive_createdir(const char* szPath, const char* szBasePath)
{
}

VALUE rho_json_quote_value(VALUE v,VALUE str)
{
    return 0;
}

void rho_param_free(rho_param *p)
{
}

void create_navbar(rho_param *p)
{
}

void remove_navbar()
{
}

VALUE navbar_started()
{
    return rho_ruby_create_boolean(0);
}

int rho_native_view_manager_create_native_view(const char* viewtype, int tab_index, VALUE params) {
	return -1;
}

void rho_native_view_manager_navigate_native_view(int native_view_id, const char* url) {
}

void rho_native_view_manager_destroy_native_view(int native_view_id) {
}

int rho_bluetooth_is_bluetooth_available(){return 0;}

void rho_bluetooth_off_bluetooth(){}

void rho_bluetooth_set_device_name(const char* device_name){}

VALUE rho_bluetooth_get_device_name(){return 0;}

const char* rho_bluetooth_get_last_error(){return 0;}

void rho_signature_take_signature(char* callback_url, char* image_format){}

const char* rho_bluetooth_create_session(const char* role, const char* callback_url){return 0;}

const char* rho_bluetooth_create_custom_server_session(const char* client_name, const char* callback_url, int accept_any_device){return 0;}

const char* rho_bluetooth_create_custom_client_session(const char* server_name, const char* callback_url){return 0;}

const char* rho_bluetooth_stop_current_connection_process(){return 0;}

void rho_bluetooth_session_set_callback(const char* connected_device_name, const char* callback_url){}

void rho_bluetooth_session_disconnect(const char* connected_device_name){}

int rho_bluetooth_session_get_status(const char* connected_device_name){return 0;}

VALUE rho_bluetooth_session_read_string(const char* connected_device_name){return 0;}

void rho_bluetooth_session_write_string(const char* connected_device_name, const char* str){}

VALUE rho_bluetooth_session_read_data(const char* connected_device_name){return 0;}

void rho_bluetooth_session_write_data(const char* connected_device_name, VALUE data){}

VALUE rho_asynchttp_request(const char* command, rho_param *p){return 0;}

void rho_asynchttp_cancel(const char* cancel_callback){}
