#pragma once

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

#ifdef __cplusplus
};
#endif //__cplusplus
