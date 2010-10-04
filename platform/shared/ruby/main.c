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
