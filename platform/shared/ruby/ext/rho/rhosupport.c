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

#include "common/RhodesApp.h"
#include "common/RhoMutexLock.h"
#include "logging/RhoLog.h"

#ifdef RHODES_EMULATOR
#include "common/RhoSimConf.h"
#endif
#include "statistic/RhoProfiler.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoRuby"

static VALUE rb_RhoStdoutClass;
static VALUE rb_RhoLogModule;
static VALUE rb_RhoProfilerModule;

extern /*RHO static*/ VALUE eval_string_with_cref(VALUE self, VALUE src, VALUE scope, NODE *cref, const char *file, int line);
static VALUE loadISeqFromFile(VALUE path);
VALUE require_compiled(VALUE fname, VALUE* result, int bLoad);
VALUE RhoPreparePath(VALUE path);
VALUE rb_iseq_eval(VALUE iseqval);
static void Init_RhoJSON();

VALUE __rhoGetCurrentDir(void)
{
    return rb_str_new2(rho_native_rhopath());
}

VALUE __rhoGetRuntimeDir(void)
{
    return rb_str_new2(rho_native_reruntimepath());
}

VALUE __rhoGetUserDir(void)
{
    return rb_str_new2(rho_native_rhouserpath());
}

VALUE __rhoGetDBDir(void)
{
    return rb_str_new2(rho_native_rhodbpath());
}

#ifdef RHODES_EMULATOR
VALUE __rhoGetRhodesDir(void)
{
    return rb_str_new2(rho_simconf_getRhodesPath());
}
#endif// RHODES_EMULATOR

VALUE
rb_f_eval_compiled(int argc, VALUE *argv, VALUE self)
{
    VALUE scope, fname, iseqval, res;
    const char *file = 0;

    //rb_gc_disable();
    rb_scan_args(argc, argv, "11", &fname, &scope);

    //RAWLOG_INFO1("eval_compiled: %s", RSTRING_PTR(fname));
    
    iseqval = loadISeqFromFile(RhoPreparePath(fname));
    res = eval_string_with_cref( self, iseqval, scope, 0, file, 1 );
    //rb_gc_enable();
    
    return res;
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
__rhoGetCallbackObject(VALUE obj, VALUE valIndex)
{
    VALUE result = rho_rhodesapp_GetCallbackObject( NUM2INT(valIndex) );
    if (result == 0) {
        rb_raise(rb_eArgError, "__rhoGetCallbackObject invalid index -- %d", NUM2INT(valIndex) );
    }

    return result;
}

VALUE rb_require_compiled_safe(VALUE obj, VALUE fname)
{
    VALUE result;
    VALUE res;
    result = require_compiled(fname, &res, 0);
    return result;
}

VALUE
rb_require_compiled(VALUE obj, VALUE fname)
{
    VALUE result;
    VALUE res;
    result = require_compiled(fname, &res, 0);
    if (NIL_P(result)) {
        rb_raise(rb_eLoadError, "no such file to load -- %s",
            RSTRING_PTR(fname));
    }

    return result;
}

VALUE
rb_load_compiled(VALUE obj, VALUE fname)
{
    VALUE result;
    VALUE res;
    result = require_compiled(fname, &res, 1);
    if (NIL_P(result)) {
        rb_raise(rb_eLoadError, "no such file to load -- %s",
            RSTRING_PTR(fname));
    }

    return result;
}

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
/*
static VALUE check_extension(VALUE res, VALUE fname, int nAddExtName)
{
    if ( nAddExtName )
    {
        char* szSlash = strchr(RSTRING_PTR(fname), '/');
        if (!szSlash)
            szSlash = strchr(RSTRING_PTR(fname), '\\');
        if (!szSlash)
            rb_str_append(res,fname);
        else
            rb_str_cat(res, RSTRING_PTR(fname), szSlash-RSTRING_PTR(fname));
    }

    rb_str_cat2(res,"/");
    rb_str_append(res,fname);
    rb_str_cat2(res,RHO_RB_EXT);

    return eaccess(RSTRING_PTR(res), R_OK) == 0 ? res : 0;
}
*/

static VALUE check_app_file_exist(VALUE dir, VALUE fname1, const char* szPlatform)
{
    VALUE res = rb_str_dup(dir);
    //RAWLOG_INFO1("find_file: check dir %s", RSTRING_PTR(dir));

    #ifdef __SYMBIAN32__
        if(*RSTRING_PTR(res) == '/')
            res = rb_str_substr(res,1,RSTRING_LEN(res) - 1);
    #endif

    rb_str_cat(res,"/",1);
    rb_str_cat(res,RSTRING_PTR(fname1),RSTRING_LEN(fname1));
    if (szPlatform)
    {
        rb_str_cat(res,".",1);
        rb_str_cat(res,szPlatform,strlen(szPlatform));
    }

    rb_str_cat(res,RHO_RB_EXT,strlen(RHO_RB_EXT));
    //RAWLOG_INFO1("find_file: check file: %s", RSTRING_PTR(res));

    return eaccess(RSTRING_PTR(res), R_OK) == 0 ? res : 0;
}

static VALUE find_file_in_load_paths(VALUE fname)
{
	int   i = 0;
	VALUE res = 0;
	VALUE load_path = GET_VM()->load_path;
	VALUE fname1    = checkRhoBundleInPath(fname);

	//TODO: support document relative require in case of multiple apps
	if (RARRAY_LEN(load_path)>1)
	{
		for(; i < RARRAY_LEN(load_path); i++ )
		{
			VALUE dir = RARRAY_PTR(load_path)[i];

#ifdef RHODES_EMULATOR
            res = check_app_file_exist(dir, fname1, rho_simconf_getString("platform"));
#endif
           if (!res)
               res = check_app_file_exist(dir, fname1, 0);
		   		   
           if (res)
           {
               break;
           }
		}

		if ( !res )
        {
#ifdef RHODES_EMULATOR
            //check for extensions
/*                res = rb_str_new2(rho_simconf_getRhodesPath() );
            rb_str_cat2(res,"/lib/extensions/");

            res = check_extension(res, fname, 1);
            if ( !res )
            {
                res = rb_str_new2(rho_native_rhopath() );
                rb_str_cat2(res,"/extensions/");

                res = check_extension(res, fname,1);
            }

            if ( !res )
            {
                res = rb_str_new2( rho_simconf_getString("ext_path") );
                res = check_extension(res, fname, 0);
            }
*/
            const char* szPaths = rho_simconf_getString("ext_path");
            const char* szPath = szPaths;
            const char* szSep = strchr(szPath, ';');
            res = 0;
            for( ; szSep; szSep = strchr(szPath, ';') )
            {
                res = rb_str_new( szPath, szSep-szPath);
                rb_str_cat2(res,"/");
                rb_str_append(res,fname);
                rb_str_cat2(res,RHO_RB_EXT);

                if ( eaccess(RSTRING_PTR(res), R_OK) == 0 )
                    break;

                res = rb_str_new( szPath, szSep-szPath);
                rb_str_cat2(res,"/app/");
                rb_str_append(res,fname);
                rb_str_cat2(res,RHO_RB_EXT);

                if ( eaccess(RSTRING_PTR(res), R_OK) == 0 )
                    break;

                res = 0;
                szPath = szSep+1;
            }

            if( res )
                return res;
            else
                return 0;
#else
            return 0;
#endif                
		}	
	} /*else {
            dir = RARRAY_PTR(load_path)[RARRAY_LEN(load_path)-1];

            res = rb_str_dup(dir);
            rb_str_cat(res,"/",1);
            rb_str_cat(res,RSTRING_PTR(fname),RSTRING_LEN(fname));
            rb_str_cat(res,RHO_RB_EXT,strlen(RHO_RB_EXT));

            if ( g_curAppPath != 0 && eaccess(RSTRING_PTR(res), R_OK) != 0 ){
                res = rb_str_new2(g_curAppPath);
                rb_str_cat(res,"/",1);
                rb_str_cat(res,RSTRING_PTR(fname),RSTRING_LEN(fname));
                rb_str_cat(res,RHO_RB_EXT,strlen(RHO_RB_EXT));
            }
    } */

	return res;
}

static VALUE find_file(VALUE fname)
{
    VALUE res = 0;
    int nOK = 0;

    //RAWLOG_INFO1("find_file: fname: %s", RSTRING_PTR(fname));

#ifdef RHODES_EMULATOR
    if ( strncmp(RSTRING_PTR(fname), rho_simconf_getRhodesPath(), strlen(rho_simconf_getRhodesPath())) == 0 )
        res = fname;
    else
#endif

#ifdef OS_WP8
	res = find_file_in_load_paths(fname);

	if (res)
		return res;
	else
#endif
    if ( strncmp(RSTRING_PTR(fname), rho_native_rhopath(), strlen(rho_native_rhopath())) == 0 ){
        res = rb_str_dup(fname);
        rb_str_cat(res,RHO_RB_EXT,strlen(RHO_RB_EXT));
        //RAWLOG_INFO1("find_file: res: %s", RSTRING_PTR(res));
    } else if ( strncmp(RSTRING_PTR(fname), rho_native_reruntimepath(), strlen(rho_native_reruntimepath())) == 0 ){
        res = rb_str_dup(fname);
        rb_str_cat(res,RHO_RB_EXT,strlen(RHO_RB_EXT));
        //RAWLOG_INFO1("find_file: res: %s", RSTRING_PTR(res));
    } else {
		res = find_file_in_load_paths(fname);

        if (res)
        {
			nOK = 1;
		}
    }

    //RAWLOG_INFO1("find_file: RhoPreparePath: %s", RSTRING_PTR(res));
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

RHO_INIT_LOCK(require_lock);

static int String_endsWith(const char* str, const char* szSuffix)
{
    int nOff = 0;
    if ( !szSuffix || !*szSuffix )
        return 0;

    nOff = strlen(str) - strlen(szSuffix);
    if ( nOff < 0 )
        return 0;

    return strcmp(str+nOff, szSuffix) == 0;
}

VALUE require_compiled(VALUE fname, VALUE* result, int bLoad)
{
    VALUE path;
    char* szName1 = 0;
	char* la = 0;
    VALUE retval = Qtrue;
    
    if (TYPE(fname) != T_STRING)
        rb_raise(rb_eLoadError, "can not load non-string");

    szName1 = RSTRING_PTR(fname);

    if ( String_endsWith(szName1,".rb") ) 
    {
        rb_str_chop_bang(fname); rb_str_chop_bang(fname); rb_str_chop_bang(fname);
    }
    //rb_funcall(fname, rb_intern("sub!"), 2, rb_str_new2(".rb"), rb_str_new2("") );
    szName1 = RSTRING_PTR(fname);

    if ( strcmp("strscan",szName1)==0 || strcmp("enumerator",szName1)==0 ||
        strcmp("stringio",szName1)==0 || strcmp("socket",szName1)==0 )
        return Qtrue;

    RHO_LOCK(require_lock);

    if ( !bLoad && isAlreadyLoaded(fname) == Qtrue )
        goto RCompExit;

    path = find_file(fname);	

    if ( path != 0 )
    {
        VALUE seq;

        RAWLOG_INFO1("require_compiled: %s", szName1);

        //optimize require
        //rb_ary_push(GET_VM()->loaded_features, path);
        rb_ary_push(GET_VM()->loaded_features, fname);

#ifdef RHODES_EMULATOR
        if ( strstr( RSTRING_PTR(path), ".rb") == 0 )
            rb_str_cat(path,".rb",3);

        GET_VM()->src_encoding_index = rb_utf8_encindex();
        rb_load(path, 0);

        if( rho_simconf_getBool("reload_app_changes") )
        {
            if ( strncmp( RSTRING_PTR(path), rho_native_rhopath(), strlen(rho_native_rhopath()) ) == 0 )
                rb_ary_delete(GET_VM()->loaded_features, fname);
        }
#else
        //rb_gc_disable();
        seq = loadISeqFromFile(path);
        //*result = rb_funcall(seq, rb_intern("eval"), 0 );
        *result = rb_iseq_eval(seq);
        
        //rb_gc_enable();
#endif
        goto RCompExit;
    }

    RAWLOG_ERROR1("require_compiled: error: can not find %s", RSTRING_PTR(fname));
    retval = Qnil;

RCompExit:
    RHO_UNLOCK(require_lock);
    return retval;
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

static VALUE rb_RhoModule;
static VALUE rb_RhoJSON;
static VALUE rb_RhoMessages;
static VALUE rb_RhoError;
static ID get_message_mid;
static ID err_message_mid;

void Init_RhoSupport()
{
    rb_RhoModule = 0;
    rb_RhoJSON = 0;
    rb_RhoLogModule = 0;   
    rb_RhoStdoutClass = 0;   
    rb_RhoProfilerModule = 0;   
    rb_RhoMessages = 0;
	rb_RhoError = 0;
    get_message_mid = 0;
    err_message_mid = 0;
    
    rb_define_global_function("require", rb_require_compiled, 1);
#ifndef RHODES_EMULATOR
	rb_define_global_function("eval_compiled_file", rb_f_eval_compiled, -1);
#else
    rb_define_global_function("__rhoGetRhodesDir", __rhoGetRhodesDir, 0);
#endif
	rb_define_global_function("__rhoGetCurrentDir", __rhoGetCurrentDir, 0);
	rb_define_global_function("__rhoGetRuntimeDir", __rhoGetRuntimeDir, 0);
	rb_define_global_function("__rhoGetUserDir", __rhoGetUserDir, 0);
	rb_define_global_function("__rhoGetDBDir", __rhoGetDBDir, 0);
	rb_define_global_function("load", rb_load_compiled, 1);
	rb_define_global_function("__rhoGetCallbackObject", __rhoGetCallbackObject, 1);

    rb_define_method(rb_mKernel, "rhom_init", rb_obj_rhom_init, 1);	
	
	Init_RhoBlobs();

#if !defined(RHO_NO_RUBY)
    Init_RhoLog();
    Init_RhoJSON();
#endif
}

static void Init_RhoBlobs()
{
    const char* szBlobPath = rho_rhodesapp_getblobsdirpath();
    const char* szUserPath = rho_native_rhodbpath();
    RAWLOG_INFO1("Init_RhoBlobs: %s", szBlobPath );

    rho_file_recursive_createdir(szBlobPath, szUserPath);
}

void rhoRubyLogWithSeverity(int severity, VALUE category, VALUE str) {
    char* szStr = 0;
    char* catStr = StringValuePtr(category);

    if(catStr == NULL) {
        catStr = "APP";
    }

    str = rb_obj_as_string(str);

    szStr = RSTRING_PTR(str);
    //Skip just newline string
    if ( strcmp(szStr,"\r\n") != 0 && strcmp(szStr,"\n") != 0 )
        rhoPlainLog("",0,severity,catStr,RSTRING_PTR(str));

}

static VALUE rb_RhoLogInfo(VALUE rhoLog, VALUE category, VALUE str)
{
#if RHO_STRIP_LOG <= L_INFO
    rhoRubyLogWithSeverity(L_INFO, category, str);
#else
#endif

    return Qnil;
}

static VALUE rb_RhoLogError(VALUE rhoLog, VALUE category, VALUE str)
{
#if RHO_STRIP_LOG <= L_ERROR
   rhoRubyLogWithSeverity(L_ERROR, category, str);
#else
#endif

    return Qnil;
}

static VALUE rb_RhoStdoutWrite(VALUE rhoLog, VALUE str)
{
    return rb_RhoLogInfo(rhoLog,rb_str_new2("APP"),str);
}

static VALUE rb_RhoStdoutFlush(void)
{
    return Qnil;
}

static VALUE rb_RhoStdoutFileno(void)
{
    return ULONG2NUM(1);
}

void rhoRubyFatalError(const char* szError)
{
    rhoPlainLog("",0,L_FATAL,"RubyVM",szError);
}

int rhoRubyVFPrintf(FILE *file, const char *format, va_list ap)
{
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

int rhoRubyPrintf(const char *format, ...){
    int nRes = 1;
    va_list ap;
    va_start(ap, format);
    nRes = rhoRubyVFPrintf(stdout,format,ap);
    va_end(ap);

    return nRes;
}

static VALUE rb_RhoProfilerCreate(VALUE rhoProfiler, VALUE strName)
{
    PROF_CREATE_COUNTER(RSTRING_PTR(strName));
    return Qnil;
}

static VALUE rb_RhoProfilerDestroy(VALUE rhoProfiler, VALUE strName)
{
    PROF_DESTROY_COUNTER(RSTRING_PTR(strName));
    return Qnil;
}

static VALUE rb_RhoProfilerStart(VALUE rhoProfiler, VALUE strName)
{
    PROF_START(RSTRING_PTR(strName));
    return Qnil;
}

static VALUE rb_RhoProfilerStop(VALUE rhoProfiler, VALUE strName)
{
    PROF_STOP(RSTRING_PTR(strName));
    return Qnil;
}

static VALUE rb_RhoProfilerFlush(VALUE rhoProfiler, VALUE strName, VALUE strMsg )
{
    PROF_FLUSH_COUNTER( RSTRING_PTR(strName), RSTRING_PTR(strMsg) );
    return Qnil;
}

static VALUE rb_RhoProfilerStartCreated(VALUE rhoProfiler, VALUE strName)
{
    PROF_START_CREATED(RSTRING_PTR(strName));
    return Qnil;
}

static void Init_RhoLog(){

    VALUE appStdout;

    rb_RhoStdoutClass = rb_define_class("RhoStdout", rb_cObject);
    rb_define_method(rb_RhoStdoutClass, "write", rb_RhoStdoutWrite, 1);
    rb_define_method(rb_RhoStdoutClass, "print", rb_RhoStdoutWrite, 1);
    rb_define_method(rb_RhoStdoutClass, "flush", rb_RhoStdoutFlush, 0);
    rb_define_method(rb_RhoStdoutClass, "fileno", rb_RhoStdoutFileno, 0);
    rb_define_method(rb_RhoStdoutClass, "to_i", rb_RhoStdoutFileno, 0);

    appStdout = rb_funcall(rb_RhoStdoutClass, rb_intern("new"), 0);
    
    rb_gv_set("$stdout", appStdout);
    rb_gv_set("$stderr", appStdout);

#if !defined( OS_WINDOWS_DESKTOP ) && !defined(OS_WINCE) && !defined(OS_WP8)
    rb_RhoLogModule = rb_define_module("RhoLog");
    rb_define_module_function(rb_RhoLogModule, "info", rb_RhoLogInfo, 2);
    rb_define_module_function(rb_RhoLogModule, "error", rb_RhoLogError, 2);
#endif

    rb_RhoProfilerModule = rb_define_module("RhoProfiler");
    rb_define_module_function(rb_RhoProfilerModule, "create_counter", rb_RhoProfilerCreate, 1);
    rb_define_module_function(rb_RhoProfilerModule, "destroy_counter", rb_RhoProfilerDestroy, 1);
    rb_define_module_function(rb_RhoProfilerModule, "start_counter", rb_RhoProfilerStart, 1);
    rb_define_module_function(rb_RhoProfilerModule, "stop_counter", rb_RhoProfilerStop, 1);
    rb_define_module_function(rb_RhoProfilerModule, "flush_counter", rb_RhoProfilerFlush, 2);
    rb_define_module_function(rb_RhoProfilerModule, "start_created_counter", rb_RhoProfilerStartCreated, 1);
}

static VALUE rb_RhoModule;
static VALUE rb_RhoJSON;
extern VALUE rho_json_parse(VALUE,VALUE);
extern VALUE rho_json_quote_value(VALUE,VALUE);
static void Init_RhoJSON()
{

    //VALUE appLog; //, appErrLog;

    rb_RhoModule = rb_define_module("Rho");
    rb_RhoJSON = rb_define_class_under(rb_RhoModule, "JSON", rb_cObject);

    rb_define_singleton_method(rb_RhoJSON, "parse", rho_json_parse, 1);
    rb_define_singleton_method(rb_RhoJSON, "quote_value", rho_json_quote_value, 1);
}

static VALUE rb_RhoMessages;
static ID get_message_mid;
const char* rho_ruby_internal_getMessageText(const char* szName)
{
    VALUE callres, strVal;
    const char* szValue;
    if ( !rb_RhoMessages )
    {
        rb_RhoMessages = rb_const_get(rb_RhoModule,rb_intern("RhoMessages"));
        CONST_ID(get_message_mid, "get_message");
    }

    callres = rb_funcall(rb_RhoMessages, get_message_mid, 1, rb_str_new2(szName) );
    strVal = rb_funcall(callres, rb_intern("to_s"), 0);
    szValue = RSTRING_PTR(strVal);

    return szValue;
}

static VALUE rb_RhoError;
static ID err_message_mid;
const char* rho_ruby_internal_getErrorText(int nError)
{
    VALUE callres, strVal;
    const char* szValue;
    if ( !rb_RhoError )
    {
        rb_RhoError = rb_const_get(rb_RhoModule,rb_intern("RhoError"));
        CONST_ID(err_message_mid, "err_message");
    }

    callres = rb_funcall(rb_RhoError, err_message_mid, 1, rb_int_new(nError) );
    strVal = rb_funcall(callres, rb_intern("to_s"), 0);
    szValue = RSTRING_PTR(strVal);

    return szValue;
}
/*
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
}*/

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
