/* sriptvm.i */
%module RhoScriptVM
%{
#include "ruby/ext/rho/rhoruby.h"

extern VALUE rho_javascriptvm_call_js_function(const char* function);

#define call_js_function rho_javascriptvm_call_js_function
%}

extern VALUE call_js_function(const char* function);