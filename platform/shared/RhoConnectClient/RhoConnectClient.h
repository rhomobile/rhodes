/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#pragma once

#include "RhoError.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
typedef enum _RHOM_MODEL_TYPE { RMT_PROPERTY_BAG=1, RMT_PROPERTY_FIXEDSCHEMA=2 } RHOM_MODEL_TYPE;
typedef enum _RHOM_SYNC_TYPE { RST_NONE = 0, RST_INCREMENTAL=1, RST_BULK_ONLY=2 } RHOM_SYNC_TYPE;
typedef struct _RHOM_MODEL
{
    const char* name;
    RHOM_MODEL_TYPE type;
    int source_id; //generated when insert to database
    
    RHOM_SYNC_TYPE sync_type;
    int  sync_priority;

    const char* partition;

    unsigned long associations; //hash string to string: rho_connectclient_hash_create

    const char* blob_attribs;
}RHOM_MODEL;

typedef struct _RHO_CONNECT_NOTIFY
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
}RHO_CONNECT_NOTIFY;

typedef struct _RHO_CONNECT_OBJECT_NOTIFY
{
    int deleted_count;
    int updated_count;
    int created_count;
    int* deleted_source_ids;
    int* updated_source_ids;
    int* created_source_ids;
    
    char** deleted_objects;
    char** updated_objects;
    char** created_objects;
}RHO_CONNECT_OBJECT_NOTIFY;
    
void rho_connectclient_initmodel(RHOM_MODEL* model);
void rho_connectclient_destroymodel(RHOM_MODEL* model);

void rho_connectclient_init(RHOM_MODEL* pModels, int nModels);
void rho_connectclient_destroy();
void rho_connectclient_database_full_reset_and_logout();
void rho_connectclient_database_fullclient_reset_and_logout();
void rho_connectclient_database_full_reset(bool bClientReset);
	
void rho_connectclient_parsenotify(const char* msg, RHO_CONNECT_NOTIFY* pNotify);
void rho_connectclient_free_syncnotify(RHO_CONNECT_NOTIFY* pNotify);

void rho_connectclient_parse_objectnotify(const char* msg, RHO_CONNECT_OBJECT_NOTIFY* pNotify);
void rho_connectclient_free_sync_objectnotify(RHO_CONNECT_OBJECT_NOTIFY* pNotify);
    
unsigned long rho_connectclient_strarray_create();
void rho_connectclient_strarray_add(unsigned long ar, const char* szStr);
void rho_connectclient_strarray_delete(unsigned long ar);

unsigned long rho_connectclient_strhasharray_create();
void rho_connectclient_strhasharray_add(unsigned long ar, unsigned long hash);
void rho_connectclient_strhasharray_delete(unsigned long ar);
int rho_connectclient_strhasharray_size(unsigned long ar);
unsigned long rho_connectclient_strhasharray_get(unsigned long ar, int nIndex);

void rho_connectclient_create_object(const char* szModel, unsigned long hash);
unsigned long rho_connectclient_find(const char* szModel,const char* szObject );
unsigned long rho_connectclient_find_first(const char* szModel, unsigned long hashCond );
unsigned long rho_connectclient_find_all(const char* szModel, unsigned long hashCond );
unsigned long rho_connectclient_findbysql(const char* szModel, const char* szSql, unsigned long arParams );

void rho_connectclient_save( const char* szModel, unsigned long hash );
void rho_connectclient_itemdestroy( const char* szModel, unsigned long hash );

unsigned long rho_connectclient_hash_create();
void rho_connectclient_hash_put(unsigned long hash, const char* szKey, const char* szValue);
void rho_connectclient_hash_delete(unsigned long hash);
const char* rho_connectclient_hash_get(unsigned long hash, const char* szKey);
int rho_connectclient_hash_equal(unsigned long hash1, unsigned long hash2);
int rho_connectclient_hash_size(unsigned long hash);
void rho_connectclient_hash_enumerate(unsigned long hash, int (*enum_func)(const char* szKey, const char* szValue, void* pThis), void* pThis );

void rho_connectclient_start_bulkupdate(const char* szModel);
void rho_connectclient_stop_bulkupdate(const char* szModel);
	
#ifdef __cplusplus
};
#endif //__cplusplus
