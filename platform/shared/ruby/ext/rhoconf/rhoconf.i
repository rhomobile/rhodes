/* system.i */
%module RhoConf
%{
	extern void rho_conf_set_property_by_name(char* name, char* value);
	#define set_property_by_name rho_conf_set_property_by_name

    extern VALUE rho_conf_get_property_by_name(char* property);
    #define get_property_by_name rho_conf_get_property_by_name
	
	extern void rho_conf_show_log();
	#define show_log rho_conf_show_log

	extern int rho_conf_send_log();
	#define send_log rho_conf_send_log

	extern void rho_conf_clean_log();
	#define clean_log rho_conf_clean_log

	extern VALUE rho_conf_read_log(int limit);
	#define read_log rho_conf_read_log

	extern int rho_conf_is_property_exists(char* name);
	#define is_property_exists rho_conf_is_property_exists
	
%}

%typemap(out) int is_property_exists 
 "$result = ($1 != 0) ? Qtrue : Qfalse;";

%typemap(default) int limit {
 $1 = 0;
}
 
extern void set_property_by_name(char* name, char* value);
extern VALUE get_property_by_name(char* name);
extern void show_log();
extern int send_log();
extern void clean_log();
extern VALUE read_log(int limit);
extern int is_property_exists(char* name);
