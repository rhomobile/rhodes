#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED


#include "common/spirit_comtypes.h"
#include "logging/SpiritLogType.h"
#include "media_manager_api/spirit_media_manager.h"
#include "common.h"

//parses X.X.X.X:Port string and fills sSpiritRtpPort object
bool FillAddrFromString( tIpAddr& Port, const char *strIPPort );

//parses %i,%i:%ix%i string
//used to parse command line arguments
bool LoadRectFrom_tString(TCHAR * szRect,sSpiritVideoRect& rect);
//used to parse config string
bool LoadRectFrom_cString(const char * szRect,sSpiritVideoRect& rect);

void VideoCodecDescSetParam(tVideoCodecDescSettings* pCodecDesc,uint32 key,uint32 value);

char *tChar2Char(const TCHAR* ptStr,char *pCharBuff,int nBuffSize);
#ifndef UNICODE
#define PRINTF printf
#else // UNICODE
#define PRINTF wprintf
#endif // UNICODE

#endif // UTILS_H_INCLUDED
