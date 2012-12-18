#include "ruby.h"

static void
obj_free(void *p)
{
    ruby_xfree(p);
}

static VALUE obj_allocate(VALUE klass)
{
	return Data_Wrap_Struct(klass, 0, obj_free, ALLOC(void*));
}

VALUE rho_create_object_with_id( VALUE klass, const char* szID )
{
    const char ** ppString = NULL;

    VALUE valObj = obj_allocate(klass);
    Data_Get_Struct(valObj, void *, ppString);
    *ppString = xmalloc(strlen(szID)+1);
    strcpy(*ppString, szID);

    return valObj;
}

int rho_ruby_is_string(VALUE val)
{
    return (TYPE(val) == T_STRING) ? 1 : 0;
}

int rho_ruby_is_array(VALUE val)
{
    return (TYPE(val) == T_ARRAY) ? 1 : 0;
}

const char* rho_get_object_id( VALUE valObj )
{
    const char ** ppString = NULL;

    Data_Get_Struct(valObj, void *, ppString);

    return *ppString;
}
