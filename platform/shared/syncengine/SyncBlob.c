#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "SyncBlob.h"
#include "Constants.h"
#include "SyncObject.h"

//unsigned char* fetch_remote_rawdata(char* url, unsigned long* pnSize);
extern const char* RhoGetRootPath();
int push_remote_data(char* url, char* data, size_t data_size,char* contentType);
/*
static int sqlite3OsWrite(sqlite3_file *id, const void *pBuf, int amt, int offset){
  return id->pMethods->xWrite(id, pBuf, amt, offset);
}*/

#ifndef __APPLE__
#ifndef __SYMBIAN32__
static int sqlite3OsOpen(
						 sqlite3_vfs *pVfs, 
						 const char *zPath, 
						 sqlite3_file *pFile, 
						 int flags, 
						 int *pFlagsOut
){
	return pVfs->xOpen(pVfs, zPath, pFile, flags, pFlagsOut);
}

static int sqlite3OsRead(sqlite3_file *id, void *pBuf, int amt, int offset){
	return id->pMethods->xRead(id, pBuf, amt, offset);
}

static int sqlite3OsFileSize(sqlite3_file *id, sqlite_int64 *pSize){
	return id->pMethods->xFileSize(id, pSize);
}

static int sqlite3OsDelete(sqlite3_vfs *pVfs, const char *zPath, int dirSync){
	return pVfs->xDelete(pVfs, zPath, dirSync);
}

static int sqlite3OsClose(sqlite3_file *pId){
	int rc = SQLITE_OK;
	if( pId->pMethods ){
		rc = pId->pMethods->xClose(pId);
		pId->pMethods = 0;
	}
	return rc;
}
#else
int sqlite3OsOpen(
				  sqlite3_vfs *pVfs, 
				  const char *zPath, 
				  sqlite3_file *pFile, 
				  int flags, 
				  int *pFlagsOut
);
int sqlite3OsRead(sqlite3_file *id, void *pBuf, int amt, int offset);
int sqlite3OsFileSize(sqlite3_file *id, sqlite_int64 *pSize);
int sqlite3OsDelete(sqlite3_vfs *pVfs, const char *zPath, int dirSync);
int sqlite3OsClose(sqlite3_file *pId);
#endif

static int rhoReadFile( char* fName, char** resbuffer, int* resnFileSize, int nOffset ){
    sqlite3_vfs* pVfs = sqlite3_vfs_find(0);
    int nFlagsOut = 0;
    sqlite3_file *pFile = (sqlite3_file *)calloc(pVfs->szOsFile,1);

    int rc = sqlite3OsOpen(pVfs,fName,pFile,SQLITE_OPEN_READONLY,&nFlagsOut);
    if ( rc == SQLITE_OK ){
        sqlite_int64 nFileSize = 0;
        rc = sqlite3OsFileSize(pFile,&nFileSize);
        if ( rc == SQLITE_OK && nFileSize > 0 ){
            char* buffer = (char*)malloc((int)nFileSize+*resnFileSize);

            rc = sqlite3OsRead(pFile,buffer+nOffset,(int)nFileSize,0);
            if ( rc == SQLITE_OK ){
                *resbuffer = buffer;
                *resnFileSize += (int)nFileSize;
            }
            else
                free(buffer);
        }

        sqlite3OsClose(pFile);
    }

    free(pFile);

    return rc == SQLITE_OK ? 1 : 0;
}

static int rhoDeleteFile( char* pFilePath ){
    int rc = SQLITE_OK;
    if ( pFilePath && *pFilePath ){
        sqlite3_vfs* pVfs = sqlite3_vfs_find(0);
        rc = sqlite3OsDelete(pVfs, pFilePath, 0);
    }

    return rc == SQLITE_OK ? 1 : 0;
}
#else

static int rhoDeleteFile( char* pFilePath ){
    int rc = remove(pFilePath);
    return rc == 0 ? 1 : 0;
}

//const char* RhoGetRootPath() ;

static int rhoReadFile( char* pFilePath, char** resbuffer, int* resnFileSize, int nOffset ){
    int rc = 0;
    int retCode = 0;
	char path[1024];
	path[0]=0;
	sprintf(path,"%sapps%s", RhoGetRootPath(), pFilePath);
	printf("FULL PATH: %s", path);
    //FILE* file = fopen(pFilePath,"r");
	FILE* file = fopen(path,"r");
    if (file){
        struct stat st;
        memset(&st,0, sizeof(st));
        rc = stat(path, &st);
        if ( rc == 0 && st.st_size > 0 ){
            char* buffer = (char*)malloc(st.st_size + *resnFileSize);

            rc = fread(buffer+nOffset, sizeof(char), st.st_size, file );
            if ( rc == st.st_size ){
                *resbuffer = buffer;
                *resnFileSize += st.st_size;
                retCode = 1;
            }
        }

        fclose(file);
    }

    return retCode;
}
#endif //__APPLE__

/*
static char* makeFileName(pSyncObject ref, char* buffer)
{
    char szExt[20];
    const char* szRoot = RhoGetRootPath();
    char* url = ref->_value;

    char* quest = strchr(url,'?');
    szExt[0] = 0;
    if (quest){
        char* extStart = strstr(quest,"extension=");
        if ( extStart ){
            char* extEnd = strstr(extStart,"&");
            if (extEnd){
                int nExtLen = extEnd-(extStart+10);
                strncpy(szExt,extStart+10,nExtLen);
                szExt[nExtLen] = 0;
            }
            else
                strcpy(szExt,extStart+10);
        }
    }

    if ( !szExt[0] ){
        char* dot = strrchr(url,'.');
        //TODO: process :http://img.lenta.ru/news/2009/03/11/acid/picture.jpg?test=.img
        if (dot){
            if (quest){
                if(quest>dot){
                    strncpy(szExt,dot,quest-dot);
                    szExt[quest-dot] = 0;
                }
            }
            else
                strcpy(szExt,dot);
        }
    }

    if ( !szExt[0] )
        strcpy(szExt,".bin");

    sprintf(buffer, "%s%s/id_%x%s%s", szRoot, "blobs", ref->_primary_key, (szExt[0]=='.'?"":"."), szExt );
                                                  
    return buffer;
}

//pSyncObject syncObj = SyncObjectCopy(list[j]);
//SyncBlob_insertOp( syncObj, client_id, SyncBlob_SYNC_STAGE);

void SyncBlob_insertOp(pSyncObject ref, char *client_id, int stage)
{
    char* fName = NULL;
    char url_string[4096];
    char connectChar = '&';
    unsigned char* data = NULL;
    unsigned long  nDataSize = 0;

    if ( !ref->_type || strncmp(ref->_type,"blob.url", 8) != 0 )
        return;

    if ( strchr(ref->_value,'?') == 0 )
        connectChar = '?';

    sprintf(url_string, "%s%cclient_id=%s", ref->_value, connectChar, client_id );

    data = fetch_remote_rawdata(url_string, &nDataSize);
    if ( !data )
        return;

    fName = makeFileName(ref, url_string);

    {
        sqlite3_vfs* pVfs = sqlite3_vfs_find(0);
        int nFlagsOut = 0;
        sqlite3_file *pFile = (sqlite3_file *)calloc(pVfs->szOsFile,1);

        int rc = sqlite3OsOpen(pVfs,fName,pFile,SQLITE_OPEN_CREATE|SQLITE_OPEN_READWRITE,&nFlagsOut);
        if ( rc == SQLITE_OK ){
            rc = sqlite3OsWrite(pFile,data,nDataSize,0);
            if ( rc == SQLITE_OK )
            {
                free(ref->_type);
                ref->_type = str_assign("blob.file");
                free(ref->_value);
                ref->_value = str_assign(fName);
            }

            sqlite3OsClose(pFile);
        }

        free(pFile);
    }

    free(data);

}*/

int SyncBlob_extractBlobs(pSyncOperation* op_list, int op_list_count, pSyncOperation** popblob_list)
{
    int opblob_list_count = 0;
    pSyncOperation* opblob_list = NULL;
    int i = 0;

    for( ; i < op_list_count;  )
    {
        char* type = op_list[i]->_sync_object->_type;
        if ( op_list[i]->_operation && strcmp(op_list[i]->_operation, UPDATE_TYPE_DELETE ) != 0 &&
             type && strncmp(type, "blob.file", 9 ) == 0 )
        {
            if ( !opblob_list )
    	        opblob_list = calloc(1,op_list_count*sizeof(pSyncOperation));

            opblob_list[opblob_list_count] = op_list[i];
            opblob_list_count++;

            memmove( op_list+i, op_list+i+1, sizeof(pSyncOperation)*(op_list_count-i-1) );
            op_list_count--;
            continue;
        }

        i++;
    }

    *popblob_list = opblob_list;
    return opblob_list_count;
}

// We don't care about the filename (it is sent in the uri params).
// However, multipart-form post to rails seems to fail if we don't provide it.
static const char* szMultipartPrefix = 
   "------------A6174410D6AD474183FDE48F5662FCC5\r\n"
   "Content-Disposition: form-data; name=\"blob\"; filename=\"doesnotmatter.png\"\r\n"
   "Content-Type: application/octet-stream\r\n\r\n";
static const char* szMultipartPostfix = 
    "\r\n------------A6174410D6AD474183FDE48F5662FCC5--";

static char* szMultipartContType = 
    "multipart/form-data; boundary=----------A6174410D6AD474183FDE48F5662FCC5";

int SyncBlob_pushRemoteBlobs(pSyncOperation *list, int size)
{
    int result = SYNC_PUSH_CHANGES_OK;
    int i = 0;
    char url_string[4096];

    for( i = 0; i < size && result == SYNC_PUSH_CHANGES_OK; i++ )
    {
        char* fName = list[i]->_sync_object->_value;
        char* buffer = 0;
        int nFileSize = strlen(szMultipartPrefix) + strlen(szMultipartPostfix);
        int rc = rhoReadFile(fName, &buffer, &nFileSize, strlen(szMultipartPrefix) );

        if ( rc == 1 && nFileSize && buffer){

            sprintf(url_string, "%s&%s", list[i]->_uri, list[i]->_post_body );

            memcpy(buffer, szMultipartPrefix, strlen(szMultipartPrefix) );
            memcpy(buffer+nFileSize-strlen(szMultipartPostfix), szMultipartPostfix, strlen(szMultipartPostfix) );
/*
            strcpy(buffer,
                "--A6174410D6AD474183FDE48F5662FCC5\r\n"
                "Content-Disposition: form-data; name=\"files\"; filename=\"file1.txt\" \r\n"
                "Content-Type: application/octet-stream\r\n"
                "TEST"
                "----A6174410D6AD474183FDE48F5662FCC5--\r\n" );
            nFileSize = strlen(buffer);*/
            result = push_remote_data( url_string, buffer, nFileSize, szMultipartContType );
            //push_remote_data( url_string, buffer, nFileSize, "application/octet-stream" );

        }

        if ( buffer )
            free(buffer);
    }

    return result;
}

//"SELECT rhoOnDeleteObjectRecord(OLD.value,OLD.type,OLD.update_type);\n"
void SyncBlob_DeleteCallback(sqlite3_context* dbContext, int nArgs, sqlite3_value** ppArgs)
{
    int rc = 0;
    char* type = NULL;
    if ( nArgs < 2 )
        return;

    type = (char*)sqlite3_value_text(*(ppArgs+1));
    if ( type && strcmp(type,"blob.file") == 0 ){
        char* pFilePath = (char*)sqlite3_value_text(*(ppArgs));
        rc = rhoDeleteFile(pFilePath);
    }
}
