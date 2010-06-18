/* nativebar.i */
%module NavBar
%{
#include "ext/rho/rhoruby.h"
extern void create_navbar(rho_param *p);
#define create create_navbar
extern void remove_navbar();
#define remove remove_navbar
extern VALUE navbar_started();
#define started navbar_started
%}

%typemap(in) (rho_param *p) {
  $1 = valueToRhoParam($input);
}

%typemap(freearg) (rho_param *p) {
  rho_param_free($1);
}

extern void create(rho_param *p);
extern void remove();
extern VALUE started();
