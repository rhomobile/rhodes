
#include "cfgfile.h"

tConfigFile :: tConfigFile(tUI &rUI, const char *pszFileName)
 : m_UI(rUI)
{
  if (!pszFileName)
    m_strFileName = "vve.cfg";
  else
    m_strFileName = pszFileName;
}

bool tConfigFile :: Exists()
{
  FILE *ifp = fopen(m_strFileName.c_str(),"r");
  if (!ifp) return false;

  fclose(ifp);
  return true;
}
char * trim(char *buff)
{
  char *pIn = buff;
  char *pOut = buff;
  int quote_mode = 0;

  while (*pIn) {
    if (*pIn == '"') {
      quote_mode = quote_mode ? 0:1;
    }
    else if (quote_mode || (*pIn != ' ' && *pIn != '\t' && *pIn != '\n' && *pIn != '\r')) {
      *pOut++ = *pIn;
    }
    pIn++;
  }
  *pOut = 0;
  return buff;
}

util_string tConfigFile :: GetConfigValueStr(const char *pszValueName,const char *pszDefaultValue)
{

  util_string strResult("");
  if (pszDefaultValue) strResult = pszDefaultValue;
  FILE *ifp = fopen(m_strFileName.c_str(),"r");
  if (ifp) {
    char sbuff[256];
    while (1) {
      if (!fgets(sbuff,256,ifp)) break;
      //lets skip comment
      if (*sbuff == '#') continue; 
      //removes space, tabas and line feeds
      trim(sbuff);
      //check value name
      if (_strnicmp(pszValueName,sbuff,strlen(pszValueName))) continue;
      //check '=' delimiter
      if (sbuff[strlen(pszValueName)] != '=') continue;
      //we have found required value lets copy the result and terminate loop
      strResult = sbuff + strlen(pszValueName) + 1;
      break;
    }
    fclose(ifp);
  }

  return strResult;
}

unsigned int tConfigFile :: GetConfigValueInt(const char *pszValueName,const char *pszDefaultValue)
{
  return atoi(GetConfigValueStr(pszValueName,pszDefaultValue).c_str());
}
