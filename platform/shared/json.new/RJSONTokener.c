/*
 * $Id: json_tokener.c,v 1.20 2006/07/25 03:24:50 mclark Exp $
 *
 * Copyright (c) 2004, 2005 Metaparadigm Pte. Ltd.
 * Michael Clark <michael@metaparadigm.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>

#include <common/RhoDefs.h>
#ifndef RHO_NO_RUBY_API

#if defined(_WIN32_WCE) || defined(WIN32) || defined(OS_WP8) || defined(OS_UWP)
//#define strdup _strdup
//extern int _shttpd_strncasecmp(register const char *,register const char *, size_t);
//#define strncasecmp _shttpd_strncasecmp
# define strncasecmp _strnicmp
#endif

#ifdef POSIXNAME
#define fpstrdup _strdup
#else
#define fpstrdup strdup
#endif // POSIXNAME


#ifdef _MSC_VER
// warning C4018: '<' : signed/unsigned mismatch
#pragma warning ( disable : 4018 )
#endif

#include "bits.h"
#include "printbuf.h"
#include "arraylist.h"
#include "json_object.h"
#include "json_tokener.h"
#include "json.h"

#include "ruby/ext/rho/rhoruby.h"

#if !HAVE_STRNCASECMP && defined(_MSC_VER)
  /* MSC has the version as _strnicmp */
# define strncasecmp _strnicmp
#elif !HAVE_STRNCASECMP
# error You do not have strncasecmp on your system.
#endif /* HAVE_STRNCASECMP */

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RJSON"

static const char* json_null_str = "null";
static const char* json_true_str = "true";
static const char* json_false_str = "false";





VALUE json_convert_to_ruby(json_object * jobj) {
    VALUE robj = 0;
    enum json_type type = json_object_get_type(jobj);
    int count = 0;
    int i;
    if (jobj == NULL) {
        return rho_ruby_get_NIL();
    }
    switch (type) {
        case json_type_boolean:
            return rho_ruby_create_boolean(json_object_get_boolean(jobj));
            break;
        case json_type_double:
            return rho_ruby_create_double(json_object_get_double(jobj));
            break;
        case json_type_int:
            return rho_ruby_create_integer(json_object_get_int(jobj));
            break;
        case json_type_null:
            return rho_ruby_get_NIL();
            break;
        case json_type_string:
            return rho_ruby_create_string(json_object_get_string(jobj));
            break;
        case json_type_array:
            robj = rho_ruby_create_array();
            count = (int)json_object_array_length(jobj);
            for (i = 0; i < count; i++) {
                rho_ruby_add_to_array(robj, json_convert_to_ruby(json_object_array_get_idx(jobj, i)));
            }
            return robj;
            break;
        case json_type_object:
            robj = rho_ruby_createHash();
            json_object_object_foreach(jobj, key, val) {
                rho_ruby_add_to_hash(robj, rho_ruby_create_string(key), json_convert_to_ruby(val));
            }
            return robj;
            break;
    }
    return rho_ruby_get_NIL();
}



VALUE rjson_tokener_parse(const char *str, char** pszError)
{
    struct json_tokener* tok = NULL;
    struct json_object* obj = NULL;
    VALUE result = NULL;
    
    tok = json_tokener_new();
    obj = json_tokener_parse_ex(tok, str, -1);
    if(tok->err != json_tokener_success)
    {
        if ( pszError )
        {
            *pszError = malloc(100);
            sprintf(*pszError,"JSON error code: %d; Offset: %d", tok->err, tok->char_offset);
        }
        
        obj = NULL;
    }
    result = json_convert_to_ruby(obj);
    json_tokener_free(tok);
    json_object_put(obj);
    
    return result;
}




VALUE rb_gc_enable(void);
VALUE rb_gc_disable(void);

VALUE rho_json_parse(VALUE v,VALUE str)
{
    char* szError = 0;
    VALUE res = 0;

    VALUE oldGC = rho_ruby_disable_gc();
    res = rjson_tokener_parse(getStringFromValue(str), &szError);
    rho_ruby_enable_gc(oldGC);

    if ( res != 0 )
        return res;

    RAWLOG_ERROR1("Incorrect json body.Error:%s", (szError ? szError:""));

    rho_ruby_raise_runtime(szError);

    if ( szError )
        free(szError);

    return rho_ruby_get_NIL();
}
#else // RHO_NO_RUBY_API
unsigned long rjson_tokener_parse(const char *str, char** pszError)
{
    return 0;
}
#endif
