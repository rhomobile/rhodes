#define VALUE unsigned long 
#ifndef _TIME_T_DEFINED
typedef unsigned long  time_t;
#define _TIME_T_DEFINED 	/* avoid multiple def's of time_t */
#endif
typedef long off_t;

void rho_ruby_holdValue(VALUE val)
{

}

void rho_ruby_releaseValue(VALUE val)
{
}

int rho_ruby_is_string(VALUE val)
{
    return  0;
}

int rho_ruby_is_integer(VALUE val)
{
    return  0;
}

int rho_ruby_is_boolean(VALUE val)
{
    return  0;
}

int rho_ruby_is_double(VALUE val)
{
    return  0;
}

int rho_ruby_is_array(VALUE val)
{
    return  0;
}

int rho_ruby_is_hash(VALUE val)
{
    return  0;
}

int rho_ruby_to_int(VALUE val, int* dest)
{
    return 0;
}

int rho_ruby_to_double(VALUE val, double* dest)
{
    return 0;
}

int rho_ruby_to_bool(VALUE val, int* dest)
{
    return 0;
}

int rho_ruby_to_str_ex(VALUE val, const char** dest, int* len)
{
    return 0;
}


VALUE rho_ruby_createHash() {
    return 0;
}

VALUE rho_ruby_get_NIL()
{
    return 0;
}

int rho_ruby_is_NIL(VALUE val)
{
    return 0;
}

int rho_ruby_get_bool(VALUE val)
{
    return 0;
}

long rho_ruby_get_int(VALUE val)
{
    return 0;
}

double rho_ruby_get_double(VALUE val)
{
    return 0;
}

int  rho_ruby_is_started()
{
    return 0;
}

VALUE rho_ruby_create_boolean(unsigned char b)
{
    return 0;
}

VALUE rho_ruby_create_integer(__int64 i)
{
    return 0;
}

VALUE rho_ruby_create_double(double d)
{
    return 0;
}

void rho_ruby_add_to_array(VALUE ar, VALUE val)
{
   
}

void rho_ruby_add_to_hash(VALUE hash, VALUE key, VALUE item) {
	
}

VALUE rho_ruby_create_array()
{
    return 0;
}

VALUE rho_ruby_create_string(const char* szVal)
{
    return 0;
}

VALUE rho_ruby_create_string_withlen(int len)
{
    return 0;
}

VALUE rho_ruby_create_string_withlen2(const char* szVal, int len)
{
    return 0;
}

VALUE addStrToHash(VALUE hash, const char* key, const char* val) {
    return 0;
}

VALUE addHashToHash(VALUE hash, const char* key, VALUE val) {
    return 0;
}

VALUE rho_ruby_create_object_with_id( VALUE klass, const char* szID )
{
    
    return 0;
}

VALUE rho_ruby_get_class_byname(const char* szName)
{
    return 0;
}

VALUE rho_ruby_callmethod_arg(VALUE classValue, const char *name, VALUE arg)
{
    return 0;
}

void rho_ruby_raise_runtime(const char* szText)
{
    ;
}

void rho_ruby_raise_argerror(const char *fmt, ...)
{
}

VALUE addTimeToHash(VALUE hash, const char* key, time_t val) {
    return 0;
}

unsigned long rho_ruby_get_time_part(VALUE rDate, const char* szPart)
{
    return 0;
}

VALUE rho_ruby_get_time(VALUE rDate)
{
    return 0;
}
VALUE addIntToHash(VALUE hash, const char* key, int val) {
    return 0;
}

VALUE addBoolToHash(VALUE hash, const char* key, int val)
{
    return 0;
}

VALUE rho_ruby_array_get(VALUE ar, int nIndex)
{
    return 0;
}

char* getStringFromValue(VALUE val){
   
    return 0;
}

VALUE rho_ruby_hash_aref(VALUE hash, const char* key)
{
    return 0;
}

VALUE rho_ruby_disable_gc()
{
    return 0;
}

void  rho_ruby_enable_gc(VALUE val)
{
    
}

VALUE rho_ruby_create_byte_array(unsigned char* buf, int length) {
	return 0;
}

int rho_ruby_unpack_byte_array(VALUE array_value, unsigned char* buf, int max_length)
{
    return 0;
}
