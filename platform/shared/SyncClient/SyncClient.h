#pragma once

#include "common/RhoError.h"

typedef enum _RHOM_MODEL_TYPE { RMT_PROPERTY_BAG=1, RMT_PROPERTY_FIXEDSCHEMA=2 } RHOM_MODEL_TYPE;
typedef enum _RHOM_SYNC_TYPE { RST_NONE = 0, RST_INCREMENTAL=1, RST_BULK_ONLY=2 } RHOM_SYNC_TYPE;
typedef struct _RHOM_MODEL
{
    const char* name;
    RHOM_MODEL_TYPE type;

    RHOM_SYNC_TYPE sync_type;
    int  sync_priority;

    const char* partition;
    //TODO: scema model parameters

}RHOM_MODEL;

typedef struct _RHO_SYNC_NOTIFY
{
    int total_count;
    int processed_count;
    int cumulative_count;
    int source_id;
    char* source_name;
    char* status;
    char* sync_type;
    int error_code;
    char* error_message;
    char* callback_params;
}RHO_SYNC_NOTIFY;


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void rho_syncclient_initmodel(RHOM_MODEL* model);
void rho_syncclient_init(RHOM_MODEL* pModels, int nModels);
void rho_syncclient_destroy();
void rho_syncclient_database_full_reset_and_logout();

void rho_syncclient_parsenotify(const char* msg, RHO_SYNC_NOTIFY* pNotify);
void rho_syncclient_free_syncnotify(RHO_SYNC_NOTIFY* pNotify);

unsigned long rho_syncclient_strarray_create();
void rho_syncclient_strarray_add(unsigned long ar, const char* szStr);
void rho_syncclient_strarray_delete(unsigned long ar);

unsigned long rho_syncclient_strhasharray_create();
void rho_syncclient_strhasharray_add(unsigned long ar, unsigned long hash);
void rho_syncclient_strhasharray_delete(unsigned long ar);
int rho_syncclient_strhasharray_size(unsigned long ar);
unsigned long rho_syncclient_strhasharray_get(unsigned long ar, int nIndex);

void rho_syncclient_create_object(const char* szModel, unsigned long hash);
unsigned long rho_syncclient_find(const char* szModel,const char* szObject );
unsigned long rho_syncclient_find_first(const char* szModel, unsigned long hashCond );
unsigned long rho_syncclient_find_all(const char* szModel, unsigned long hashCond );
void rho_syncclient_save( const char* szModel, unsigned long hash );
void rho_syncclient_itemdestroy( const char* szModel, unsigned long hash );

unsigned long rho_syncclient_hash_create();
void rho_syncclient_hash_put(unsigned long hash, const char* szKey, const char* szValue);
void rho_syncclient_hash_delete(unsigned long hash);
const char* rho_syncclient_hash_get(unsigned long hash, const char* szKey);
int rho_syncclient_hash_equal(unsigned long hash1, unsigned long hash2);
int rho_syncclient_hash_size(unsigned long hash);
void rho_syncclient_hash_enumerate(unsigned long hash, int (*enum_func)(const char* szKey, const char* szValue, void* pThis), void* pThis );
	
#ifdef __cplusplus
};
#endif //__cplusplus
