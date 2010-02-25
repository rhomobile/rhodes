/* mapview.i */
%module MapView
%{
#include "ext/rho/rhoruby.h"
extern void mapview_create(rho_param *p);
#define create mapview_create
%}

%typemap(in) (rho_param *p) {
    $1 = valueToRhoParam($input);
}

%typemap(freearg) (rho_param *p) {
    rho_param_free($1);
}

extern void create(rho_param *p);

