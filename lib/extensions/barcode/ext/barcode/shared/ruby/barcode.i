/* barcode.i */
%module Barcode
%{
#include "ruby/ext/rho/rhoruby.h"
extern const char* rho_barcode_barcode_recognize(const char* filename);
extern void rho_barcode_take_barcode(const char* callback, VALUE options);

#define barcode_recognize rho_barcode_barcode_recognize 
#define take_barcode rho_barcode_take_barcode 


// additional methods for Moto compatibility

extern void rho_scanner_enumerate(const char* callback);
extern void  rho_motobarcode_enable(const char* callback, rho_param* p);
extern void  rho_motobarcode_disable();
extern void  rho_motobarcode_start();
extern void  rho_motobarcode_stop();
            
#define enumerate    rho_motobarcode_enumerate
#define enable       rho_motobarcode_enable
#define disable      rho_motobarcode_disable
#define start        rho_motobarcode_start
#define stop         rho_motobarcode_stop

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


extern const char* barcode_recognize(const char* filename);
extern void take_barcode(const char* callback, VALUE options);

extern void enumerate(const char* callback);
extern void  enable(const char* callback, rho_param* p);
extern void  disable();
extern void  start();
extern void  stop();