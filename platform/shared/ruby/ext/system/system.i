/* system.i */
%module System
%{
extern char* get_property(char* property);
extern VALUE has_network();
%}

extern char* get_property(char* property);
extern VALUE has_network();
