%module "Rho::SignatureCapture"
%{
#include "ext/rho/rhoruby.h"

#if !defined(bool)
#define bool int
#define true  1
#define false 0
#endif

extern void rho_signature_take(const char* callback_url, rho_param* p);
#define take  rho_signature_take

extern void rho_signature_visible(bool b, rho_param* p);
#define visible  rho_signature_visible

extern void rho_signature_capture(const char* callback_url);
#define capture  rho_signature_capture

extern void rho_signature_clear();
#define clear    rho_signature_clear

%}

%typemap(in) (rho_param *p) {
    $1 = rho_param_fromvalue($input);
}

%typemap(freearg) (rho_param *p) {
    rho_param_free($1);
}

%typemap(default) rho_param* p {
 $1 = 0;
}

extern void take(const char* callback_url, rho_param* p);
extern void visible(bool b, rho_param* p);
extern void capture(const char* callback_url);
extern void clear();
