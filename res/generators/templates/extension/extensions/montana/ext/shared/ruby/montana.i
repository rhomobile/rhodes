/* <%= name.downcase %>.i */
%module <%= name.camel_case %>
%{
#include "ruby/ext/rho/rhoruby.h"

extern VALUE <%= name.downcase %>_native_process_string(const char* str);
extern int <%= name.downcase %>_calc_summ(int x, int y);

#define native_process_string <%= name.downcase %>_native_process_string 
#define calc_summ <%= name.downcase %>_calc_summ 

%}

extern VALUE native_process_string(const char* str);
extern int calc_summ(int x, int y);
