/* barcode1.i */
%module Barcode1
%{
#include "ruby/ext/rho/rhoruby.h"

extern VALUE barcode1_native_process_string(const char* str);
extern int barcode1_calc_summ(int x, int y);

#define native_process_string barcode1_native_process_string 
#define calc_summ barcode1_calc_summ 

%}

extern VALUE native_process_string(const char* str);
extern int calc_summ(int x, int y);
