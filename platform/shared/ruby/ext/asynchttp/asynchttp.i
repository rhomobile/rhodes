/* asynchttp.i */
%module "Rho::AsyncHttp"
%{
    #include "ext/rho/rhoruby.h"
    
	extern VALUE rho_asynchttp_request(const char* command, rho_param *p);
	#define do_request rho_asynchttp_request

	extern void rho_asynchttp_cancel(const char* cancel_callback);
	#define cancel rho_asynchttp_cancel
%}

%typemap(default) const char* cancel_callback {
 $1 = "*";
}

%typemap(in) (rho_param *p) {
    $1 = rho_param_fromvalue($input);
}

extern VALUE do_request(const char* command, rho_param *p);
extern void cancel(const char* cancel_callback);

