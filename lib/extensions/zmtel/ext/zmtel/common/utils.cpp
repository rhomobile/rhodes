
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"


void VideoCodecDescSetParam(tVideoCodecDescSettings* pDesc,uint32 key,uint32 value)
{
  for (size_t i =0; i <pDesc->count; i++ ) {
    if (pDesc->pParam[i].key ==key) {
      pDesc->pParam[i].u.value = value;
      return;
    }
  }
  pDesc->pParam[pDesc->count].key = key;
  pDesc->pParam[pDesc->count++].u.value = value;


}

bool FillAddrFromString( tIpAddr& addr, const char *strIPPort )
{
  int         index;
  int         step;
  const char* p = strIPPort;

  if ( !p ) return false;

#ifdef SPIRIT_ENDIANESS_BE
  index = 0; step = 1;
#else
  index = 3; step = -1;
#endif

  memset( &addr, 0, sizeof( addr ) );
  addr.ip.i8[index] = (uint8)atoi(p);
  index += step;

  while (*p && *p!='.') p++;
  if (!p) return false;
  p++;
  addr.ip.i8[index] = (uint8) atoi(p);
  index += step;

  while (*p && *p!='.') p++;
  if (!p) return false;
  p++;
  addr.ip.i8[index] = (uint8)atoi(p);
  index += step;

  while (*p && *p!='.') p++;
  if (!p) return false;
  p++;
  addr.ip.i8[index] = atoi(p);

  while (*p && *p!=':') p++;
  if (!p) return false;
  p++;

  addr.port = (uint16) atoi(p);

  return true;
}


bool LoadRectFrom_cString(const char * szRect,sSpiritVideoRect& rect)
{
  rect.X = atoi(szRect);
  const char *p = szRect;
  while (*p && *p!= ',') p++;
  if (!*p || !*(p+1)) {
    return false;
  }
  p++;
  rect.Y = atoi(p);
  while (*p && *p!= ':') p++;
  if (!*p || !*(p+1)) {
    return false;
  }
  p++;
  rect.Width = atoi(p);

  while (*p && *p!= 'x') p++;
  if (!*p || !*(p+1)) {
    return false;
  }
  p++;
  rect.Height = atoi(p);
  if (!rect.Width || !rect.Height) return false;

  return true;
}

bool LoadRectFrom_tString(TCHAR * szRect,sSpiritVideoRect& rect)
{
  rect.X = _ttoi(szRect);
  TCHAR *p = szRect;
  while (*p && *p!= ',') p++;
  if (!*p || !*(p+1)) {
    return false;
  }
  p++;
  rect.Y = _ttoi(p);
  while (*p && *p!= ':') p++;
  if (!*p || !*(p+1)) {
    return false;
  }
  p++;
  rect.Width = _ttoi(p);

  while (*p && *p!= 'x') p++;
  if (!*p || !*(p+1)) {
    return false;
  }
  p++;
  rect.Height = _ttoi(p);
  if (!rect.Width || !rect.Height) return false;

  return true;
}


void* SpiritMalloc(size_t Size)
{
  return malloc(Size);
}
void  SpiritFree(void* pMem)
{
  free(pMem);
}

char *tChar2Char(const TCHAR* ptStr,char *pCharBuff,int nBuffSize)
{
  int k;
  for (k =0; k < _tcslen(ptStr)&& k <nBuffSize-1; k++) {
    pCharBuff[k] = (char) ptStr[k];
  }
  pCharBuff[k] = 0;
  return pCharBuff;
}