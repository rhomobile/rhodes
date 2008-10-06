/*
 *  rhoruby.c
 *  rhorubylib
 *
 *  Created by evgeny vovchenko on 10/1/08.
 *  Copyright 2008 RhoMobile. All rights reserved.
 *
 */
#undef RUBY_EXPORT
#include "ruby.h"
#include "debug.h"
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#include "vm_core.h"

void RhoRubyStart(const char* szAppPath)
{
#ifdef HAVE_LOCALE_H
    setlocale(LC_CTYPE, "");
#endif
	
    //ruby_sysinit(&argc, &argv);
    {
		RUBY_INIT_STACK;
		ruby_init();
		
		ruby_init_loadpath(szAppPath);
		Init_strscan();
	}	
}

void RhoRubyStop()
{
	ruby_cleanup(0);
}

char* makeControllerCall(char* classname, char* methodname);

char* RhoProcessRequest( char* szRequest)
{
	return makeControllerCall("RhoTest", "respond");
	
	
}

char* makeControllerCall(char* classname, char* methodname)
{
	const char* szRubyCall=
	"require '%s'\r\n"
	"ctrl = %s.new\r\n"
	"ctrl.%s";
	char src[1000];
	VALUE iseq, res, string;
	sprintf(src, szRubyCall, classname, classname, methodname);
	iseq  = rb_iseq_compile(rb_str_new2(src), rb_str_new2("test.rb"), INT2FIX(0));
	res = rb_iseq_eval(iseq);
	
	//string = rb_funcall(res, rb_intern("to_s"), 0, 0);
	
	char* szRes = RSTRING_PTR(res);
	
	return szRes;
}

/*		{
 const char * src = 
 "startTime = Time.new.to_f\r\n"
 "puts (startTime).to_s \r\n"
 "sum = \"\" \r\n"
 "50000.times{|e| sum += e.to_s} \r\n"
 "endTime = Time.new.to_f \r\n"
 "puts (endTime).to_s \r\n"
 "puts (endTime - startTime).to_s + ' sec'";
 
 VALUE iseq = rb_funcall(rb_cISeq,rb_intern("compile_file"), 1, rb_str_new2("/Users/evgeny/Ruby/test/erb/test_erb2.rb"));
 
 //	VALUE iseq  = rb_iseq_compile(rb_str_new2(src), rb_str_new2("test.rb"), INT2FIX(0));
 //VALUE arr = rb_funcall(iseq, rb_intern("to_a"), 0, 0);
 //VALUE string = rb_funcall(arr, rb_intern("to_s"), 0, 0);
 
 
 
 //VALUE iseqRes = iseq_load(Qnil, arr, Qnil, Qnil);			
 
 VALUE res = rb_iseq_eval(iseq);
 int i = 0;
 }*/

