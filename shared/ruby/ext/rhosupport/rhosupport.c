#include "vm_core.h"

#include "ruby/ruby.h"

#ifdef ENABLE_RUBY_VM_STAT
#include "../../stat/stat.h"
#endif

extern /*RHO static*/ VALUE
eval_string_with_cref(VALUE self, VALUE src, VALUE scope, NODE *cref, const char *file, int line);
extern const char* RhoGetRootPath();
static VALUE loadISeqFromFile(VALUE path);
VALUE require_compiled(VALUE fname, VALUE* result);
VALUE RhoPreparePath(VALUE path);

VALUE __rhoGetCurrentDir(void)
{
    return rb_str_new2(RhoGetRootPath());
}

VALUE
rb_f_eval_compiled(int argc, VALUE *argv, VALUE self)
{
    VALUE scope, fname, iseqval;
    const char *file = 0;

    rb_scan_args(argc, argv, "11", &fname, &scope);

    iseqval = loadISeqFromFile(RhoPreparePath(fname));
    return eval_string_with_cref( self, iseqval, scope, 0, file, 1 );
    //return eval_iseq_with_scope(self, scope, iseqval );
}   

static VALUE loadISeqFromFile(VALUE path)
{
//        fiseq = File.open(fName)
#ifdef ENABLE_RUBY_VM_STAT
    struct timeval  start;
    struct timeval  end;
    gettimeofday (&start, NULL); 
#endif    

        VALUE fiseq = rb_funcall(rb_cFile, rb_intern("binread"), 1, path);

#ifdef ENABLE_RUBY_VM_STAT
    gettimeofday (&end, NULL);
    
    g_iseq_binread_usec += end.tv_usec - start.tv_usec; 
    g_iseq_binread_sec += end.tv_sec  - start.tv_sec; 
    
    gettimeofday (&start, NULL);
#endif    

//        arr = Marshal.load(fiseq)
        VALUE arr = rb_funcall(rb_const_get(rb_cObject,rb_intern("Marshal")), rb_intern("load"), 1, fiseq);

#ifdef ENABLE_RUBY_VM_STAT
    gettimeofday (&end, NULL);
    
    g_iseq_marshal_load_usec += end.tv_usec - start.tv_usec; 
    g_iseq_marshal_load_sec += end.tv_sec  - start.tv_sec; 
#endif    
        
//        fiseq.close
        //rb_funcall(fiseq, rb_intern("close"), 0 );
//        seq = VM::InstructionSequence.load(arr)
        return rb_funcall(rb_cISeq, rb_intern("load"), 1, arr);
}

VALUE
rb_require_compiled(VALUE obj, VALUE fname)
{
    VALUE result;
    VALUE res;
    result = require_compiled(fname, &res);
    if (NIL_P(result)) {
        rb_raise(rb_eLoadError, "no such file to load -- %s",
            RSTRING_PTR(fname));
    }

    return result;
}

static const char *const loadable_ext[] = {
    ".iseq",
    0
};

static VALUE find_file(VALUE fname)
{
    VALUE tmp;
    int type;
    FilePathValue(fname);

    tmp = rb_str_dup(fname);
    type = rb_find_file_ext(&tmp, loadable_ext);
    tmp = rb_file_expand_path(tmp, Qnil);

    if ( type == 1 )
        return RhoPreparePath(tmp);

    return 0;
}

VALUE isAlreadyLoaded(VALUE path)
{
    VALUE v, features;
    long i;
    const char *f;

    features = GET_VM()->loaded_features;
    for (i = 0; i < RARRAY_LEN(features); ++i) 
    {
        v = RARRAY_PTR(features)[i];
        f = StringValuePtr(v);
        if ( RSTRING_LEN(v) != RSTRING_LEN(path))
            continue;
        if (strcmp(f, RSTRING_PTR(path)) == 0) {
            return Qtrue;
        }
    }

    return Qfalse;
}

VALUE require_compiled(VALUE fname, VALUE* result)
{
    VALUE path;
	
#ifdef ENABLE_RUBY_VM_STAT
    struct timeval  start;
    struct timeval  end;
    struct timeval  req_start;
    
    gettimeofday (&req_start, NULL);
#endif    
    
    rb_funcall(fname, rb_intern("sub!"), 2, rb_str_new2(".rb"), rb_str_new2("") );

    if ( strcmp("strscan",RSTRING_PTR(fname))==0 || strcmp("enumerator",RSTRING_PTR(fname))==0 )
        return Qtrue;

    path = find_file(fname);

    if ( path != 0 )
    {
        VALUE seq;
		
        if ( isAlreadyLoaded(path) == Qtrue )
            return Qtrue;

        rb_ary_push(GET_VM()->loaded_features, path);

        seq = loadISeqFromFile(path);

#ifdef ENABLE_RUBY_VM_STAT
    gettimeofday (&start, NULL); 
#endif    

        *result = rb_funcall(seq, rb_intern("eval"), 0 );

#ifdef ENABLE_RUBY_VM_STAT
    gettimeofday (&end, NULL);
    
    g_iseq_eval_usec += end.tv_usec - start.tv_usec; 
    g_iseq_eval_sec += end.tv_sec  - start.tv_sec;
    
    g_require_compiled_usec += end.tv_usec - req_start.tv_usec; 
    g_require_compiled_sec += end.tv_sec  - req_start.tv_sec;
#endif    

        return Qtrue;
    }

    return Qnil;
}

#ifndef CharNext		/* defined as CharNext[AW] on Windows. */
#define CharNext(p) ((p) + mblen(p, RUBY_MBCHAR_MAXSIZE))
#endif

static void
translate_char(char *p, int from, int to)
{
    while (*p) {
	    if ((unsigned char)*p == from)
	      *p = to;
	    p = CharNext(p);
    }
}

VALUE RhoPreparePath(VALUE path){
#ifdef __SYMBIAN32__

	VALUE fname2 = rb_str_dup(path);
	
	translate_char(RSTRING_PTR(fname2),'/', '\\');
	
	return fname2;
#endif //__SYMBIAN32__
	
	return path;
}

void Init_RhoSupport()
{
    rb_define_global_function("require", rb_require_compiled, 1);
    rb_define_global_function("eval_compiled_file", rb_f_eval_compiled, -1);
    rb_define_global_function("__rhoGetCurrentDir", __rhoGetCurrentDir, 0);
}
