#include "vm_core.h"

#include "ruby/ruby.h"
#include "ruby/io.h"
#include "missing/file.h"
#ifdef ENABLE_RUBY_VM_STAT
#include "../../stat/stat.h"
#endif
#ifdef _WIN32
#include "missing/file.h"
#endif

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoRuby"

extern /*RHO static*/ VALUE
eval_string_with_cref(VALUE self, VALUE src, VALUE scope, NODE *cref, const char *file, int line);
extern const char* RhoGetRootPath();
extern const char* RhoGetRelativeBlobsPath();
static VALUE loadISeqFromFile(VALUE path);
VALUE require_compiled(VALUE fname, VALUE* result);
VALUE RhoPreparePath(VALUE path);
VALUE rb_iseq_eval(VALUE iseqval);

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
    VALUE seq;
//        fiseq = File.open(fName)
#ifdef ENABLE_RUBY_VM_STAT
    struct timeval  start;
    struct timeval  end;
#endif    

        VALUE fiseq = rb_funcall(rb_cFile, rb_intern("binread"), 1, path);
        //VALUE fiseq = rb_funcall(rb_cFile, rb_intern("open"), 2, path, rb_str_new2("rb"));

#ifdef ENABLE_RUBY_VM_STAT
    gettimeofday (&start, NULL); 
#endif    


//        arr = Marshal.load(fiseq)
        VALUE arr = rb_funcall(rb_const_get(rb_cObject,rb_intern("Marshal")), rb_intern("load"), 1, fiseq);

#ifdef ENABLE_RUBY_VM_STAT
    gettimeofday (&end, NULL);
    
    if ( g_collect_stat )
	{
		if ( end.tv_sec > 0 )
			g_iseq_marshal_load_msec += (end.tv_sec  - start.tv_sec) * 1000;
		else
			g_iseq_marshal_load_msec += (end.tv_usec - start.tv_usec)/1000; 
	    
	}
    gettimeofday (&start, NULL);
#endif    
        
//        fiseq.close
        //rb_funcall(fiseq, rb_intern("close"), 0 );
//        seq = VM::InstructionSequence.load(arr)
        seq = rb_funcall(rb_cISeq, rb_intern("load"), 1, arr);

#ifdef ENABLE_RUBY_VM_STAT
    gettimeofday (&end, NULL);
    
    if ( g_collect_stat )
	{
		if ( end.tv_sec > 0 )
			g_iseq_binread_msec += (end.tv_sec  - start.tv_sec) * 1000;
		else
			g_iseq_binread_msec += ( end.tv_usec - start.tv_usec )/1000; 
	}
#endif    

        return seq;
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
/*
static char* g_curAppPath = 0;
void RhoSetCurAppPath(char* path){
    if ( g_curAppPath ){
        free(g_curAppPath);
        g_curAppPath = 0;
    }

    if ( path && *path ){
        char* appPath = strdup(path);
        int i = 0;
        char* szApp;
        for( ; i < strlen(appPath); i++ )
            if ( appPath[i] == '\\' )
                appPath[i] = '/';

        szApp = strstr( appPath, "/apps/");
        if ( szApp ){
            char* szAppEnd = strchr( szApp+6, '/');
            if ( szAppEnd ){
                int nLen = szAppEnd-appPath;
                g_curAppPath = malloc(nLen+1);
                g_curAppPath[0] = 0;
                strncpy(g_curAppPath,path,nLen);
                g_curAppPath[nLen] = 0;
            }
        }
        
        free(appPath);
    }
}*/

static VALUE checkRhoBundleInPath(VALUE fname)
{
    VALUE res = fname;
    const char* szName = RSTRING_PTR(fname);
    char* pRhoBundle = strstr(szName, "RhoBundle");
    char* slash = 0;
    char* slash1 = 0;
    if ( !pRhoBundle )
        return res;

    slash = strchr(pRhoBundle,'/');
    if ( !slash )
        slash = strchr(pRhoBundle,'\\');
    if ( !slash )
        return res;

    slash1 = strchr(slash+1,'/');
    if ( !slash1 )
        slash1 = strchr(slash+1,'\\');
    if ( !slash1 )
        return res;

    if ( strncmp(slash1+1,"app",3)==0 )
        slash1 += 4;

    return rb_str_new2(slash1+1);
}

static VALUE find_file(VALUE fname)
{
    VALUE res;
    int nOK = 0;

    if ( strncmp(RSTRING_PTR(fname), RhoGetRootPath(), strlen(RhoGetRootPath())) == 0 ){
        res = rb_str_dup(fname);
        rb_str_cat(res,".iseq",5);
    }else{
        int i = 0;
        VALUE load_path = GET_VM()->load_path;
        //VALUE dir;
        fname = checkRhoBundleInPath(fname);

        //TODO: support document relative require in case of multiple apps
        if (RARRAY_LEN(load_path)>1){
            for( ; i < RARRAY_LEN(load_path); i++ ){
                VALUE dir = RARRAY_PTR(load_path)[i];
                res = rb_str_dup(dir);
                rb_str_cat(res,"/",1);
                rb_str_cat(res,RSTRING_PTR(fname),RSTRING_LEN(fname));
                rb_str_cat(res,".iseq",5);

                if( eaccess(RSTRING_PTR(res), R_OK) == 0 ){
                    nOK = 1;
                    break;
                }
            }
            if ( !nOK )
                return 0;
        } /*else {
            dir = RARRAY_PTR(load_path)[RARRAY_LEN(load_path)-1];

            res = rb_str_dup(dir);
            rb_str_cat(res,"/",1);
            rb_str_cat(res,RSTRING_PTR(fname),RSTRING_LEN(fname));
            rb_str_cat(res,".iseq",5);

            if ( g_curAppPath != 0 && eaccess(RSTRING_PTR(res), R_OK) != 0 ){
                res = rb_str_new2(g_curAppPath);
                rb_str_cat(res,"/",1);
                rb_str_cat(res,RSTRING_PTR(fname),RSTRING_LEN(fname));
                rb_str_cat(res,".iseq",5);
            }
        } */
    }

    res = RhoPreparePath(res);
    if ( !nOK )
        nOK = 1;//eaccess(RSTRING_PTR(res), R_OK) == 0 ? 1 : 0;

    return nOK ? res : 0;
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
    char* szName = 0;
//    FilePathValue(fname);

	szName = RSTRING_PTR(fname);
    RAWTRACE1("require_compiled: %s", szName);

    rb_funcall(fname, rb_intern("sub!"), 2, rb_str_new2(".rb"), rb_str_new2("") );

    if ( strcmp("strscan",szName)==0 || strcmp("enumerator",szName)==0 )
        return Qtrue;

    path = find_file(fname);

    if ( path != 0 )
    {
        VALUE seq;
		
        if ( isAlreadyLoaded(path) == Qtrue )
            return Qtrue;

        rb_ary_push(GET_VM()->loaded_features, path);

        seq = loadISeqFromFile(path);

        //*result = rb_funcall(seq, rb_intern("eval"), 0 );
        *result = rb_iseq_eval(seq);

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

static void Init_RhoLog();
static void Init_RhoBlobs();

static VALUE
rb_obj_rhom_init(VALUE obj, VALUE iv)
{
    //TODO: if render will slow implement this function to fasten rhom member access(see BB)
    return Qnil;
}

void Init_RhoSupport()
{
	rb_define_global_function("require", rb_require_compiled, 1);
	rb_define_global_function("eval_compiled_file", rb_f_eval_compiled, -1);
	rb_define_global_function("__rhoGetCurrentDir", __rhoGetCurrentDir, 0);
	rb_define_global_function("load", rb_require_compiled, 1);

    rb_define_method(rb_mKernel, "rhom_init", rb_obj_rhom_init, 1);	

	Init_RhoLog();
	Init_RhoBlobs();
}

static void Init_RhoBlobs()
{
  VALUE path = __rhoGetCurrentDir();
  rb_funcall(path, rb_intern("concat"), 1, rb_str_new2(RhoGetRelativeBlobsPath()));

  RAWLOG_INFO1("Init_RhoBlobs: %s", RSTRING_PTR(path) );

  if ( rb_funcall(rb_cDir, rb_intern("exist?"), 1, path)==Qfalse )
    rb_funcall(rb_cDir, rb_intern("mkdir"), 1, path);

}

static VALUE
rb_RhoLogWrite(VALUE rhoLog, VALUE str)
{
#if RHO_STRIP_LOG <= L_INFO
    char* szStr = 0;
    str = rb_obj_as_string(str);

    szStr = RSTRING_PTR(str);
    //Skip just newline string
    if ( strcmp(szStr,"\r\n") != 0 && strcmp(szStr,"\n") != 0 )
        rhoPlainLog("",0,L_INFO,"APP",RSTRING_PTR(str));
#else
#endif

    return Qnil;
}

void rhoRubyFatalError(const char* szError){
    rhoPlainLog("",0,L_FATAL,"RubyVM",szError);
}

int rhoRubyVFPrintf(FILE *file, const char *format, va_list ap){
    int nRes = 1;
    if ( file == stderr || file == stdout ){
        int severity = L_INFO;
        if ( file == stderr ){
            if ( strstr(format,"[FATAL]") )
                severity = L_FATAL;
            else
                severity = L_ERROR;
        }

        if ( severity == L_INFO )
#if RHO_STRIP_LOG <= L_INFO
            rhoPlainLogArg("",0,severity,"RubyVM",format,ap);
#else
            ;
#endif
        else if ( severity == L_ERROR )
#if RHO_STRIP_LOG <= L_ERROR
            rhoPlainLogArg("",0,severity,"RubyVM",format,ap);
#else
            ;
#endif
        else
            rhoPlainLogArg("",0,severity,"RubyVM",format,ap);

    }
    else
        nRes = vfprintf(file,format,ap);

    return nRes;
}

int rhoRubyFPrintf(FILE *file, const char *format, ...){
    int nRes = 1;
    va_list ap;
    va_start(ap, format);
    nRes = rhoRubyVFPrintf(file,format,ap);
    va_end(ap);

    return nRes;
}

static VALUE rb_RhoLogClass;
static void Init_RhoLog(){

    VALUE appLog; //, appErrLog;

    rb_RhoLogClass = rb_define_class("RhoLog", rb_cObject);
    rb_define_method(rb_RhoLogClass, "write", rb_RhoLogWrite, 1);

    appLog = rb_funcall(rb_RhoLogClass, rb_intern("new"), 0);
    
    rb_gv_set("$stdout", appLog);
    rb_gv_set("$stderr", appLog);
}

static void Init_RhoLog2()
{
  VALUE path = __rhoGetCurrentDir();
  VALUE stdioPath, exist, logio;
  rb_funcall(path, rb_intern("concat"), 1, rb_str_new2("rhologpath.txt"));
  exist = rb_funcall(rb_cFile, rb_intern("exist?"), 1, path);
  if ( exist == Qtrue ){
    stdioPath = rb_funcall(rb_cIO, rb_intern("read"), 1, path);
    if ( stdioPath != 0 && stdioPath != Qnil && RSTRING_LEN(stdioPath)>0 )
    {
      //char* szPath = RSTRING_PTR(stdioPath);
      //int len = RSTRING_LEN(stdioPath);
#if defined(WIN32)
	  freopen( RSTRING_PTR(stdioPath), "w", stdout );
#endif
	  logio = rb_funcall(rb_cFile, rb_intern("new"), 2, stdioPath, rb_str_new2("w+"));
      if ( logio != 0 && logio != Qnil ){
		  rb_gv_set("$stdout", logio);
		  rb_gv_set("$stderr", logio);
      }
    }
  }
}
void rhoGCReport(VALUE valReport)
{
    char *report = RSTRING_PTR(valReport);
    char *p, *e = report+strlen(report);
    RAWTRACEC("RubyGC","GC Started.Profile report:");

    while(report < e) {
        p = strchr(report,'\n');
        if (p!=NULL) {
            *p = '\0';
        }
        RAWTRACEC("RubyGC",report);
        report += strlen(report)+1;
    }
    RAWTRACEC("RubyGC","--profile eof--");
}
