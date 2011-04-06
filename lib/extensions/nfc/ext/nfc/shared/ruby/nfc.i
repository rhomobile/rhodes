/* nfc.i */
%module Nfc
%{

extern void rho_nfc_set_callback(const char* callback_url);
#define set_callback rho_nfc_set_callback

extern void rho_nfc_enable(int enable);
#define enable rho_nfc_enable

extern int rho_nfc_is_enabled(void);
#define is_enabled rho_nfc_is_enabled

extern int rho_nfc_is_supported(void);
#define is_supported rho_nfc_is_supported

%}

extern void set_callback(const char* callback_url);
extern void enable(int enable);
extern int is_enabled(void);
extern int is_supported(void);
