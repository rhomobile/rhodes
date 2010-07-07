/* barcode.i */
%module Barcode
%{
extern const char* rho_barcode_barcode_recognize(const char* filename);
#define barcode_recognize rho_barcode_barcode_recognize 
%}

extern const char* barcode_recognize(const char* filename);
