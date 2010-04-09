#ifndef PUBLIC_SPIRIT_STORAGE_MANAGER_H
#define PUBLIC_SPIRIT_STORAGE_MANAGER_H

#ifdef SPIRIT_USE_STL_PORT
  #include <stlport/map>
  #include <stlport/string>
#else
  #include <map>
  #include <string>
#endif

#include "common/spirit_comtypes.h"
#include "system/spirit_system.h"

namespace Spirit
{
  //=====================================================================================
  // storage reader
  //=====================================================================================
  class tStorageReader
  {
  public:
    //ctor/dtor
    tStorageReader();
    ~tStorageReader();

    tResult LoadFile(const char *pFileName);

    tResult     GetSection(char *pParamSection);
    std::string GetString(const char *pParamName,
                      const char *pDefault);
    int GetInt(const char *pParamName, int Default);
    bool GetBool(const char *pParamName, bool Default);
    tResult     GetString(int  paramId,
                     std::string *pParamName,
                     std::string *strParamValue);

  private:
    typedef std::map<std::string, std::map<std::string, std::string> > tSMLocalStorage;
     tSMLocalStorage m_LocalStorage;
     void *pFile;
     tCritSec m_CritSec;

     tSMLocalStorage::iterator currentSection;

     typedef enum
     {
        els_DEFAULT = 0,
        els_IN_QUOTE,
        els_IN_SLASH
     } teLineStates;

     void FilterLine(std::string& line);
     void TrimLine(std::string& line);
     bool IsSectionName(std::string &line, std::string &section);
     void ParseLine(std::string &line, std::string &section, int ine_number);
     void ParseValue(std::string &value);
  };


  //=====================================================================================
  // storage writer
  //=====================================================================================
  class tStorageWriter
  {
  public:
     //ctor/dtor
     tStorageWriter();
     tResult SaveFile(const char *pFileName);

     tResult SetSection(const char *pParamSection);
     tResult SetString(const char *pParamName,
                      const char *strParamValue);
     tResult SetInt(const char *pParamName, int paramValue);
     tResult SetBool(const char *pParamName, bool paramValue);

  private:
     typedef std::map<std::string, std::map<std::string, std::string> > tSMLocalStorage;
     tSMLocalStorage m_LocalStorage;
     void *pFile;
     tCritSec m_CritSec;

     tSMLocalStorage::iterator currentSection;
  };
}

#endif // PUBLIC_SPIRIT_STORAGE_MANAGER_H
