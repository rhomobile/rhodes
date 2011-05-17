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

extern int rho_nfc_tech_MifareClassic_get_block_count();
#define tech_MifareClassic_get_block_count rho_nfc_tech_MifareClassic_get_block_count

extern int rho_nfc_tech_MifareClassic_get_sector_count();
#define tech_MifareClassic_get_sector_count rho_nfc_tech_MifareClassic_get_sector_count

extern int rho_nfc_tech_MifareClassic_get_blocks_in_sector_count(int index);
#define tech_MifareClassic_get_blocks_in_sector_count rho_nfc_tech_MifareClassic_get_blocks_in_sector_count

extern int rho_nfc_tech_MifareClassic_sector_to_block(int index);
#define tech_MifareClassic_sector_to_block rho_nfc_tech_MifareClassic_sector_to_block

extern int rho_nfc_tech_MifareClassic_authenticate_sector_with_key_A(int index, VALUE key);
#define tech_MifareClassic_authenticate_sector_with_key_A rho_nfc_tech_MifareClassic_authenticate_sector_with_key_A

extern int rho_nfc_tech_MifareClassic_authenticate_sector_with_key_B(int index, VALUE key);
#define tech_MifareClassic_authenticate_sector_with_key_B rho_nfc_tech_MifareClassic_authenticate_sector_with_key_B

extern int rho_nfc_tech_MifareClassic_get_type();    
#define tech_MifareClassic_get_type rho_nfc_tech_MifareClassic_get_type

extern VALUE rho_nfc_tech_MifareClassic_transceive(VALUE data);    
#define tech_MifareClassic_transceive rho_nfc_tech_MifareClassic_transceive
    
extern VALUE rho_nfc_tag_get_id();
#define tag_get_id rho_nfc_tag_get_id
    
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
extern int tech_MifareClassic_get_block_count();
extern int tech_MifareClassic_get_sector_count();
extern int tech_MifareClassic_get_blocks_in_sector_count(int index);
extern int tech_MifareClassic_sector_to_block(int index);
extern int tech_MifareClassic_authenticate_sector_with_key_A(int index, VALUE key);
extern int tech_MifareClassic_authenticate_sector_with_key_B(int index, VALUE key);
extern int tech_MifareClassic_get_type();    
extern VALUE tag_get_id();
extern VALUE tech_MifareClassic_transceive(VALUE data);    
extern void tech_MifareUltralight_write_page(int index, VALUE block);
extern VALUE tech_MifareUltralight_read_pages(int index);

