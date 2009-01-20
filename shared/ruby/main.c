/**********************************************************************

  main.c -

  $Author: akr $
  created at: Fri Aug 19 13:19:58 JST 1994

  Copyright (C) 1993-2007 Yukihiro Matsumoto

**********************************************************************/

#undef RUBY_EXPORT
#include "ruby.h"
#include "debug.h"
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

extern void Init_SyncEngine(void);

#define COMPILER 1
/*
 if (rb_safe_level() == 0) {
 ruby_incpush(getenv("RUBYLIB"));
    }
[04.01.2009 11:44:19] evgeny vovchenko says: это надо убрать
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
    RhoRubyStop();
}

const char* RhoGetRootPath()
{
    return "D:/Projects/rhodes1/rhodes/win32/bin/RhoBundle/";
}

void Init_Phonebook()
{
}

#else 
#include "vm_core.h"
static VALUE
__rho_compile( VALUE obj, VALUE src);

int
main(int argc, char **argv)
{
    int nRes = 0;
#ifdef RUBY_DEBUG_ENV
    ruby_set_debug_option(getenv("RUBY_DEBUG"));
#endif
#ifdef HAVE_LOCALE_H
    setlocale(LC_CTYPE, "");
#endif

    SetEnvironmentVariable("RUBYOPT","");
    SetEnvironmentVariable("RUBYLIB","");

//    MessageBox(0,"","",MB_OK);
    ruby_sysinit(&argc, &argv);
    {
	RUBY_INIT_STACK;
	ruby_init();
    Init_strscan();
	Init_sqlite3_api();
    Init_SyncEngine();
    Init_System();
    //Init_prelude();

    rb_define_global_function("__rho_compile", __rho_compile, 1);

	nRes = ruby_run_node(ruby_options(argc, argv));

    }

    return nRes;
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

const char* RhoGetRootPath()
{
    return 0;
}

#endif //!COMPILER

void lock_sync_mutex() {
}

void unlock_sync_mutex() {
}

void dosync() {
}

void Init_GeoLocation(){
}

void Init_SyncEngine(void) {
}
