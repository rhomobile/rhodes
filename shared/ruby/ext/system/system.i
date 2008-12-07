/* system.i */
%module System
%{
extern char* get_property(char* property);
%}

extern char* get_property(char* property);
