/* barcode.i */
%module Barcode
%{
extern const char* rho_barcode_barcode_recognize(const char* filename);
extern void rho_barcode_take_barcode(const char* callback);

#define barcode_recognize rho_barcode_barcode_recognize
#define take_barcode rho_barcode_take_barcode

%}

extern const char* barcode_recognize(const char* filename);
extern void take_barcode(const char* callback);