/* system.i */
%module RhoConf
%{
	extern void rho_conf_set_property_by_name(char* name, char* value);
	#define set_property_by_name rho_conf_set_property_by_name
%}

extern void set_property_by_name(char* name, char* value);