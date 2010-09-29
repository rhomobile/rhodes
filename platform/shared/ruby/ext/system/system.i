/* system.i */
%module System
%{
extern VALUE rho_sys_makephonecall(const char* callname, int nparams, char** param_names, char** param_values);
#define syscall rho_sys_makephonecall

extern VALUE rho_sys_get_property(char* property);
#define get_property rho_sys_get_property

extern VALUE rho_sys_has_network();
#define has_network rho_sys_has_network

extern VALUE rho_sys_get_locale();
#define get_locale rho_sys_get_locale

extern int rho_sys_get_screen_width();
#define get_screen_width rho_sys_get_screen_width
extern int rho_sys_get_screen_height();
#define get_screen_height rho_sys_get_screen_height

#define set_push_notification rho_sys_set_push_notification
extern void rho_sys_set_push_notification( const char *url, const char* params);

#define set_screen_rotation_notification rho_sys_set_screen_rotation_notification
extern void rho_sys_set_screen_rotation_notification(const char *url, const char* params);

#define exit rho_sys_app_exit
extern void rho_sys_app_exit();

#define unzip_file rho_sys_unzip_file
extern int rho_sys_unzip_file(const char *path);

#define set_sleeping rho_sys_set_sleeping
extern int rho_sys_set_sleeping(int sleeping);

%}

%typemap(in) (int nparams, char** param_names, char** param_values)
 (VALUE keys_arr, int i, VALUE key, VALUE val) {
 Check_Type($input, T_HASH);
 $1 = NUM2INT(rb_funcall($input, rb_intern("size"), 0, NULL));
 $2 = NULL;
 $3 = NULL;
 if ($1 > 0) {
 $2 = (char **) malloc($1*sizeof(char *));
 $3 = (char **) malloc($1*sizeof(char *));
 keys_arr = rb_funcall($input, rb_intern("keys"), 0, NULL);
 for (i = 0; i < $1; i++) {
 key = rb_ary_entry(keys_arr, i);
 val = rb_hash_aref($input, key);
 Check_Type(key, T_STRING);
 Check_Type(val, T_STRING);
 $2[i] = STR2CSTR(key);
 $3[i] = STR2CSTR(val);
}
}
}
%typemap(freearg) (int nparams, char** param_names, char** param_values) {
 free((void *) $2);
 free((void *) $3);
}
extern VALUE syscall(const char* callname, int nparams, char** param_names, char** param_values);
extern VALUE get_property(char* property);
extern VALUE has_network();
extern VALUE get_locale();
extern int get_screen_width();
extern int get_screen_height();
extern void set_push_notification( const char *url, const char* params);
extern void set_screen_rotation_notification(const char *url, const char* params);
extern void exit();
extern void unzip_file( const char *path );
extern int set_sleeping( int sleeping );