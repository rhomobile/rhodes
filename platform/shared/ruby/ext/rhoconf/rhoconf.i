/* system.i */
%module RhoConf
%{
	extern void rho_conf_set_property_by_name(char* name, char* value);
	#define set_property_by_name rho_conf_set_property_by_name
	
	extern void rho_conf_show_log();
	#define show_log rho_conf_show_log

	extern int rho_conf_send_log();
	#define send_log rho_conf_send_log

	extern int rho_conf_is_property_exists(char* name);
	#define is_property_exists rho_conf_is_property_exists
	
%}

%typemap(out) int is_property_exists 
 "$result = ($1 != 0) ? Qtrue : Qfalse;";
 
extern void set_property_by_name(char* name, char* value);
extern void show_log();
extern int send_log();
extern int is_property_exists(char* name);
