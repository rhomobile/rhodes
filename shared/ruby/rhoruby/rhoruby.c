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
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif
#include "vm_core.h"

extern void Init_strscan();
extern void Init_sqlite3_api();
extern void Init_GeoLocation(void);
extern void Init_SyncEngine(void);
extern void print_profile_report();
extern void enable_gc_profile(void);

static VALUE  framework;
static ID framework_mid;

static char* rb_type_to_s(VALUE obj);

void RhoRubyStart(const char* szAppPath)
{
#ifdef HAVE_LOCALE_H
    setlocale(LC_CTYPE, "");
#endif
    //ruby_sysinit(&argc, &argv);
    {
		RUBY_INIT_STACK;
		ruby_init();

#if defined(DEBUG)
		enable_gc_profile();
#endif

		ruby_init_loadpath(szAppPath);
		Init_strscan();
		Init_sqlite3_api();
		Init_GeoLocation();
		Init_SyncEngine();
    {
		const char* szFramework = 
		"begin\r\n"
		"require 'rho'\r\n"
		"puts 'RHO loaded'\r\n"
		"Rho::RHO.new\r\n"
		"rescue Exception => e\r\n"
		"puts e.message\r\n"
		"end\r\n";
    
		VALUE iseq  = rb_iseq_compile(rb_str_new2(szFramework), rb_str_new2(""), INT2FIX(0));
		framework = rb_iseq_eval(iseq);
		rb_gc_register_mark_object(framework);
    }
		CONST_ID(framework_mid, "serve");
	}	
}

void RhoRubyStop()
{
	//TBD: clenup framework, etc.
	ruby_cleanup(0);
}

char* makeControllerCall(char* classname, char* methodname);

VALUE createHash() {
	return rb_hash_new();
}

VALUE addTimeToHash(VALUE hash, const char* key, time_t val) {
	return rb_hash_aset(hash, rb_str_new2(key), rb_time_new(val,0));
}

VALUE addIntToHash(VALUE hash, const char* key, int val) {
	return rb_hash_aset(hash, rb_str_new2(key), INT2FIX(val));
}

VALUE addStrToHash(VALUE hash, const char* key, const char* val, int len) {
	return rb_hash_aset(hash, rb_str_new2(key), rb_str_new(val, len));
}

VALUE addHashToHash(VALUE hash, const char* key, VALUE val) {
	return rb_hash_aset(hash, rb_str_new2(key), val);	
}

char* callFramework(VALUE hashReq) {
	char* szRes;
  VALUE callres = rb_funcall(framework, framework_mid, 1, hashReq);
	
	if (TYPE(callres)!=T_STRING) {
		printf("Method call result type = %s\n", rb_type_to_s(callres));
		return "Error";//TBD: Supply html description of the error
	}
	
	//TBD: need to cleanup memory
	rb_gc();
#if defined(DEBUG)
	print_profile_report();
#endif

	szRes = RSTRING_PTR(callres);
	return szRes;
}

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

