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

extern int rho_nfc_tech_MifareUltralight_get_type();    
#define tech_MifareUltralight_get_type rho_nfc_tech_MifareUltralight_get_type
    
extern VALUE rho_nfc_tech_MifareUltralight_transceive(VALUE data);    
#define tech_MifareUltralight_transceive rho_nfc_tech_MifareUltralight_transceive
    
extern int rho_nfc_tech_Ndef_get_max_size();    
#define tech_Ndef_get_max_size rho_nfc_tech_Ndef_get_max_size    

extern VALUE rho_nfc_tech_Ndef_get_type();    
#define tech_Ndef_get_type rho_nfc_tech_Ndef_get_type    
    
extern int rho_nfc_tech_Ndef_is_writable();    
#define tech_Ndef_is_writable rho_nfc_tech_Ndef_is_writable    
    
extern int rho_nfc_tech_Ndef_can_make_read_only();    
#define tech_Ndef_can_make_read_only rho_nfc_tech_Ndef_can_make_read_only    
    
extern int rho_nfc_tech_Ndef_make_read_only();    
#define tech_Ndef_make_read_only rho_nfc_tech_Ndef_make_read_only    
    
extern void rho_nfc_tech_Ndef_write_Nde_message(VALUE message_in_byte_array);    
#define tech_Ndef_write_Nde_message rho_nfc_tech_Ndef_write_Nde_message
    
extern VALUE rho_nfc_tech_Ndef_read_Nde_message();    
#define tech_Ndef_read_Nde_message rho_nfc_tech_Ndef_read_Nde_message
    
extern VALUE rho_nfc_tech_NfcA_get_Atqa();    
#define tech_NfcA_get_Atqa rho_nfc_tech_NfcA_get_Atqa
    
extern int rho_nfc_tech_NfcA_get_Sak();    
#define tech_NfcA_get_Sak rho_nfc_tech_NfcA_get_Sak

extern VALUE rho_nfc_tech_NfcA_transceive(VALUE data);    
#define tech_NfcA_transceive rho_nfc_tech_NfcA_transceive
    
extern VALUE rho_nfc_convert_byte_array_to_NdeRecord_hash(VALUE array);    
#define convert_byte_array_to_NdeRecord_hash rho_nfc_convert_byte_array_to_NdeRecord_hash
    
extern VALUE rho_nfc_convert_NdeRecord_hash_to_byte_array(VALUE array);    
#define convert_NdeRecord_hash_to_byte_array rho_nfc_convert_NdeRecord_hash_to_byte_array

extern VALUE rho_nfc_convert_NdeMessage_byte_array_to_NdeRecords_array(VALUE array);    
#define convert_NdeMessage_byte_array_to_NdeRecords_array rho_nfc_convert_NdeMessage_byte_array_to_NdeRecords_array
    
extern VALUE rho_nfc_convert_NdeRecords_array_to_NdeMessage_byte_array(VALUE array);    
#define convert_NdeRecords_array_to_NdeMessage_byte_array rho_nfc_convert_NdeRecords_array_to_NdeMessage_byte_array
    
extern VALUE rho_nfc_make_string_from_payload(VALUE payload, int tnf, VALUE type);
#define make_string_from_payload rho_nfc_make_string_from_payload
    
extern VALUE rho_nfc_make_payload_with_absolute_uri(const char* str);
#define make_payload_with_absolute_uri rho_nfc_make_payload_with_absolute_uri
    
extern VALUE rho_nfc_make_payload_with_well_known_text(const char* language, const char* str);
#define make_payload_with_well_known_text rho_nfc_make_payload_with_well_known_text

extern VALUE rho_nfc_make_payload_with_well_known_uri(int prefix, const char* str);
#define make_payload_with_well_known_uri rho_nfc_make_payload_with_well_known_uri

extern void rho_nfc_p2p_enable_foreground_nde_push(VALUE nde_message_byte_array);    
#define p2p_enable_foreground_nde_push rho_nfc_p2p_enable_foreground_nde_push
    
extern void rho_nfc_p2p_disable_foreground_nde_push();    
#define p2p_disable_foreground_nde_push rho_nfc_p2p_disable_foreground_nde_push
    
extern VALUE rho_nfc_tech_IsoDep_get_hi_layer_responce();
#define tech_IsoDep_get_hi_layer_responce rho_nfc_tech_IsoDep_get_hi_layer_responce
    
extern VALUE rho_nfc_tech_IsoDep_get_historical_bytes();
#define tech_IsoDep_get_historical_bytes rho_nfc_tech_IsoDep_get_historical_bytes
    
extern void rho_nfc_tech_IsoDep_set_timeout(int timeout);
#define tech_IsoDep_set_timeout rho_nfc_tech_IsoDep_set_timeout
    
extern VALUE rho_nfc_tech_IsoDep_transceive(VALUE data);    
#define tech_IsoDep_transceive rho_nfc_tech_IsoDep_transceive

extern void rho_nfc_tech_NdefFormatable_format(VALUE ndef_message_byte_array);
#define tech_NdefFormatable_format rho_nfc_tech_NdefFormatable_format

extern void rho_nfc_tech_NdefFormatable_format_read_only(VALUE ndef_message_byte_array);
#define tech_NdefFormatable_format_read_only rho_nfc_tech_NdefFormatable_format_read_only
    
// return byte[]
extern VALUE rho_nfc_tech_NfcB_get_application_data();
#define tech_NfcB_get_application_data rho_nfc_tech_NfcB_get_application_data
    
// return byte[]
extern VALUE rho_nfc_tech_NfcB_get_protocol_info();
#define tech_NfcB_get_protocol_info rho_nfc_tech_NfcB_get_protocol_info
    
// byte[] -> byte[]
extern VALUE rho_nfc_tech_NfcB_transceive(VALUE data);    
#define tech_NfcB_transceive rho_nfc_tech_NfcB_transceive
    
// return byte[]
extern VALUE rho_nfc_tech_NfcF_get_manufacturer();
#define tech_NfcF_get_manufacturer rho_nfc_tech_NfcF_get_manufacturer
    
// return byte[]
extern VALUE rho_nfc_tech_NfcF_get_system_code();
#define tech_NfcF_get_system_code rho_nfc_tech_NfcF_get_system_code
    
// byte[] -> byte[]
extern VALUE rho_nfc_tech_NfcF_transceive(VALUE data);    
#define tech_NfcF_transceive rho_nfc_tech_NfcF_transceive
    
extern int rho_nfc_tech_NfcV_get_dsf_id();
#define tech_NfcV_get_dsf_id rho_nfc_tech_NfcV_get_dsf_id
    
extern int rho_nfc_tech_NfcV_get_responce_flags();
#define tech_NfcV_get_responce_flags rho_nfc_tech_NfcV_get_responce_flags
    
// byte[] -> byte[]
extern VALUE rho_nfc_tech_NfcV_transceive(VALUE data);    
#define tech_NfcV_transceive rho_nfc_tech_NfcV_transceive
    
extern void rho_nfc_perform_open_application_event();    
#define perform_open_application_event rho_nfc_perform_open_application_event    
    
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
extern int tech_MifareUltralight_get_type();    
extern VALUE tech_MifareUltralight_transceive(VALUE data);    
extern int tech_Ndef_get_max_size();    
extern VALUE tech_Ndef_get_type();    
extern int tech_Ndef_is_writable();    
extern int tech_Ndef_can_make_read_only();    
extern int tech_Ndef_make_read_only();    
extern void tech_Ndef_write_Nde_message(VALUE message_in_byte_array);    
extern VALUE tech_Ndef_read_Nde_message();   
extern VALUE tech_NfcA_get_Atqa();    
extern int tech_NfcA_get_Sak();    
extern VALUE tech_NfcA_transceive(VALUE data);    
extern VALUE convert_byte_array_to_NdeRecord_hash(VALUE array);    
extern VALUE convert_NdeRecord_hash_to_byte_array(VALUE array);    
extern VALUE convert_NdeMessage_byte_array_to_NdeRecords_array(VALUE array);    
extern VALUE convert_NdeRecords_array_to_NdeMessage_byte_array(VALUE array);    
extern VALUE make_string_from_payload(VALUE payload, int tnf, VALUE type);
extern VALUE make_payload_with_absolute_uri(const char* str);
extern VALUE make_payload_with_well_known_text(const char* language, const char* str);
extern VALUE make_payload_with_well_known_uri(int prefix, const char* str);
extern void p2p_enable_foreground_nde_push(VALUE nde_message_byte_array);    
extern void p2p_disable_foreground_nde_push();    
extern VALUE tech_IsoDep_get_hi_layer_responce();
extern VALUE tech_IsoDep_get_historical_bytes();
extern void tech_IsoDep_set_timeout(int timeout);
extern VALUE tech_IsoDep_transceive(VALUE data);    
extern void tech_NdefFormatable_format(VALUE ndef_message_byte_array);
extern void tech_NdefFormatable_format_read_only(VALUE ndef_message_byte_array);
extern VALUE tech_NfcB_get_application_data();
extern VALUE tech_NfcB_get_protocol_info();
extern VALUE tech_NfcB_transceive(VALUE data);    
extern VALUE tech_NfcF_get_manufacturer();
extern VALUE tech_NfcF_get_system_code();
extern VALUE tech_NfcF_transceive(VALUE data);    
extern int tech_NfcV_get_dsf_id();
extern int tech_NfcV_get_responce_flags();
extern VALUE tech_NfcV_transceive(VALUE data);    
extern void perform_open_application_event();    











