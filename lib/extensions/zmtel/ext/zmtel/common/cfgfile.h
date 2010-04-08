
#ifndef CONFIG_FILE_H_INCLUDED
#define CONFIG_FILE_H_INCLUDED

#include "custom_string.h"
#include "../ui/ui.h"

class tConfigFile 
{
public:
  //mm.cfg file is used when pszFileName==NULL
  tConfigFile(tUI &rUI, const char *pszFileName=0);
  ~tConfigFile() { };

  util_string GetConfigValueStr(const char *pszValueName,const char *pszDefaultValue=0);
  unsigned int GetConfigValueInt(const char *pszValueName,const char *pszDefaultValue=0);

  bool Exists();
private:
  tUI& m_UI;

  util_string m_strFileName;
  
};

#endif

