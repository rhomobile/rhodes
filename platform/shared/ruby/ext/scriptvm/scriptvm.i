/* sriptvm.i */
%module RhoScriptVM
%{
#include "ruby/ext/rho/rhoruby.h"

#if !defined(bool)
#define bool int
#define true  1
#define false 0
#endif

extern bool rho_javascriptvm_load_script(const char* buffer, const char* tag);
extern bool rho_javascriptvm_unload_script(const char* tag);
extern void rho_javascriptvm_perform_action(const char* actionName, const char* params);

#define load_script rho_javascriptvm_load_script
#define unload_script rho_javascriptvm_unload_script
#define perform_action rho_javascriptvm_perform_action
%}

extern bool load_script(const char* buffer, const char* tag);
extern bool unload_script(const char* tag);
extern void perform_action(const char* actionName, const char* params);