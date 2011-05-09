/* nfc.i */
%module Nfc
%{
#include "ruby/ext/rho/rhoruby.h"

extern void rho_nfc_set_callback(const char* callback_url);
#define set_callback rho_nfc_set_callback

extern void rho_nfc_enable(int enable);
#define enable rho_nfc_enable

extern int rho_nfc_is_enabled(void);
#define is_enabled rho_nfc_is_enabled

extern int rho_nfc_is_supported(void);
#define is_supported rho_nfc_is_supported

extern void rho_nfc_set_tech_callback(const char* callback_url);
#define set_tech_callback rho_nfc_set_tech_callback

extern void rho_nfc_set_listen_tech_list(VALUE tech_list);
#define set_listen_tech_list rho_nfc_set_listen_tech_list

extern VALUE rho_nfc_get_tech_list();
#define get_tech_list rho_nfc_get_tech_list

extern void rho_nfc_tech_connect(const char* name);
#define tech_connect rho_nfc_tech_connect

extern void rho_nfc_tech_close(const char* name);
#define tech_close rho_nfc_tech_close

extern int rho_nfc_tech_is_connected(const char* name);
#define tech_is_connected rho_nfc_tech_is_connected

extern int rho_nfc_tech_MifareClassic_get_size();
#define tech_MifareClassic_get_size rho_nfc_tech_MifareClassic_get_size

extern void rho_nfc_tech_MifareClassic_write_block(int index, VALUE block);
#define tech_MifareClassic_write_block rho_nfc_tech_MifareClassic_write_block

extern VALUE rho_nfc_tech_MifareClassic_read_block(int index);
#define tech_MifareClassic_read_block rho_nfc_tech_MifareClassic_read_block

extern int rho_nfc_tech_MifareUltralight_get_size();
#define tech_MifareUltralight_get_size rho_nfc_tech_MifareUltralight_get_size

extern void rho_nfc_tech_MifareUltralight_write_page(int index, VALUE block);
#define tech_MifareUltralight_write_page rho_nfc_tech_MifareUltralight_write_page

extern VALUE rho_nfc_tech_MifareUltralight_read_pages(int index);
#define tech_MifareUltralight_read_pages rho_nfc_tech_MifareUltralight_read_pages

%}

extern void set_callback(const char* callback_url);
extern void enable(int enable);
extern int is_enabled(void);
extern int is_supported(void);
extern void set_tech_callback(const char* callback_url);
extern void set_listen_tech_list(VALUE tech_list);
extern VALUE get_tech_list();
extern void tech_connect(const char* name);
extern void tech_close(const char* name);
extern int tech_is_connected(const char* name);
extern int tech_MifareClassic_get_size();
extern void tech_MifareClassic_write_block(int index, VALUE block);
extern VALUE tech_MifareClassic_read_block(int index);
extern int tech_MifareUltralight_get_size();
extern void tech_MifareUltralight_write_page(int index, VALUE block);
extern VALUE tech_MifareUltralight_read_pages(int index);

