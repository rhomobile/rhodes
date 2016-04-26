/* rhoelementsext.i */
%module RhoElementsExt
%{
#include "ruby/ext/rho/rhoruby.h"

extern VALUE emmlapi_meta_proc(const char* module_name, const char* func_name, const char* params);
#define meta_proc emmlapi_meta_proc 


%}

extern VALUE meta_proc( const char* module_name, const char* func_name, const char* params);

