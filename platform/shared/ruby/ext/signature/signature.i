/* signature.i */
%module SignatureTool
%{
extern void take_signature(char* callback_url, char* image_format);
#define take_signature rho_signature_take_signature 
%}

extern void take_signature(char* callback_url, char* image_format);
