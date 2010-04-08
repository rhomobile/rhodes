//-----------------------------------------------------------------------------
// Spirit Storage Manager
//
// Copyright (C) SPIRIT Corp. 2007

//-----------------------------------------------------------------------------
#include "../stdafx.h"
#include "storage_manager.h"
#include <ctype.h>

#define STRING_LENGTH 512
#define EMPTY_STRING(str) (!str || str[0]=='\0')

namespace Spirit
{
  //=====================================================================================
  // storage reader
  //=====================================================================================
  tStorageReader::tStorageReader()
    :pFile(NULL)
  {
    m_LocalStorage.clear();
    currentSection=m_LocalStorage.end();
  }

  //-----------------------------------------------------------------------------
  tStorageReader::~tStorageReader()
  {
  }

  //-----------------------------------------------------------------------------
  void tStorageReader::ParseValue(std::string &value)
  {
    TrimLine(value);
  }

  //-----------------------------------------------------------------------------
  void tStorageReader::ParseLine(std::string &line,
                                                      std::string &section,
                                                      int ine_number)
  {
    int equal_pos = (int)line.find('=');
    if (equal_pos == (int)std::string::npos)
      return;

    std::string name(line, 0, equal_pos);
    TrimLine(name);

    std::string value(line, equal_pos+1, std::string::npos);
    ParseValue(value);

    {
      tLock lock(&m_CritSec);
      m_LocalStorage[section][name] = value;
    }
  }

  //-----------------------------------------------------------------------------
  bool tStorageReader::IsSectionName(std::string &line,
                                                          std::string &section)
  {
    int first=line.find('[');
    int last=line.find(']');
    int npos=std::string::npos;
    if (first==npos || last==npos || last<first || line.find('=')!=npos || line.find('#')!=npos || line.find(';')!=npos)
      return false;

    section.assign(line, first+1, last-1);
    return true;
  }

  //-----------------------------------------------------------------------------
  void tStorageReader::TrimLine(std::string& line)
  {
    int begin = 0;
    int end = (int)line.size()-1;

    if (end<=0)
      return;

    while (begin<end && isspace(line[begin]))
      begin++;

    while (end>begin && isspace(line[end]))
      end--;
    line.assign(line, begin, end-begin+1);
  }

  //-----------------------------------------------------------------------------
  void tStorageReader::FilterLine(std::string& line)
  {
    int begin = 0;

    teLineStates state = els_DEFAULT;
    bool bFinish = false;
    char symbol;
    int len, end;
    len = (int)line.size();

    for (end=begin; end<len; end++)
    {
      symbol = line[end];
      switch (symbol)
      {
      case '\"':
        if (state==els_IN_QUOTE)
          state = els_DEFAULT;
        else
          state = els_IN_QUOTE;
        break;
      case '#':
        if (state!=els_IN_QUOTE)
          bFinish = true;
        break;
      case '/':
        if (state==els_IN_SLASH)
        {
          end--;
          bFinish = true;
        }
        else if (state!=els_IN_QUOTE)
          state = els_IN_SLASH;
        break;
      default:
        if (state!=els_IN_QUOTE)
          state = els_DEFAULT;
        break;
      }
      if (bFinish)
        break;
    }
    line.assign(line, begin, end-begin);
    TrimLine(line);
  }

  //-----------------------------------------------------------------------------
  tResult tStorageReader::LoadFile(const char *pFileName)
  {
    if (!pFileName)
      return SPIRIT_RESULT_BAD_PARAM;

    tLock lock(&m_CritSec);

    currentSection=m_LocalStorage.end();
    m_LocalStorage.clear();

    FILE *f=fopen(pFileName, "r");
    if (!f)
      return SPIRIT_RESULT_OK; //no file, it's OK we will create it later

    //read params
    int line_number = 0;
    bool bSectionStarted = false;
    std::string section;

    char lineA[512];
    while (fgets(lineA, sizeof(lineA), f))
    {
      std::string line=lineA;
      line_number++;
      FilterLine(line);
      if (line.empty())
        continue;

      if (IsSectionName(line, section))
        bSectionStarted = true;
      else
      {
        if (bSectionStarted)
          ParseLine(line, section, line_number);
      }
    }

    fclose(f);

    return SPIRIT_RESULT_OK;
  }

  //-----------------------------------------------------------------------------
  tResult tStorageReader::GetSection(char *pParamSection)
  {
    if (!pParamSection)
      return SPIRIT_RESULT_BAD_PARAM;

    std::string section = pParamSection;

    tLock lock(&m_CritSec);
    currentSection = m_LocalStorage.find(section);
    if (currentSection == m_LocalStorage.end())
      return SPIRIT_RESULT_NOT_READY;

    return SPIRIT_RESULT_OK;
  }

  //-----------------------------------------------------------------------------
  std::string tStorageReader::GetString(const char *pParamName, const char *pDefault)
  {
    if (currentSection == m_LocalStorage.end())
      return pDefault;

    if (!pParamName)
      return pDefault;

    //get param here
    std::string param = pParamName;

    tLock lock(&m_CritSec);
    tSMLocalStorage::const_iterator section_it = currentSection;

    std::map<std::string, std::string>::const_iterator param_it =
      section_it->second.find(param);
    if (param_it == section_it->second.end())
      return pDefault;

    return param_it->second.c_str();
  }

  //-----------------------------------------------------------------------------
  tResult tStorageReader::GetString(int paramId, std::string *pParamName, std::string *strParamValue)
  {
    if (!pParamName || paramId<0 || !strParamValue)
      return SPIRIT_RESULT_BAD_PARAM;

    if (currentSection == m_LocalStorage.end())
      return SPIRIT_RESULT_NOT_READY;

    *strParamValue="";

    tLock lock(&m_CritSec);
    tSMLocalStorage::const_iterator section_it = currentSection;

    for (std::map<std::string, std::string>::const_iterator param_it=section_it->second.begin(); param_it!=section_it->second.end(); param_it++, paramId--)
      if (paramId==0)
      {
        *pParamName=param_it->first.c_str();
        *strParamValue=param_it->second.c_str();
        return SPIRIT_RESULT_OK;
      }

      return SPIRIT_RESULT_NOT_READY;
  }


  //-----------------------------------------------------------------------------
  int tStorageReader::GetInt(const char *pParamName, int Default)
  {
    std::string result=GetString(pParamName, "");
    if (result=="")
      return Default;
    return atoi(result.c_str());
  }

  //-----------------------------------------------------------------------------
  bool tStorageReader::GetBool(const char *pParamName, bool Default)
  {
    std::string result=GetString(pParamName, "");
    if (result=="")
      return Default;
    return  _stricmp(result.c_str(), "true")==0
      ||    _stricmp(result.c_str(), "yes")==0
      ||    _stricmp(result.c_str(), "ok")==0
      ||    _stricmp(result.c_str(), "1")==0
      ;
  }




  //=====================================================================================
  // storage writer
  //=====================================================================================
  tStorageWriter::tStorageWriter()
    :pFile(NULL)
  {
    m_LocalStorage.clear();
    currentSection=m_LocalStorage.end();
  }

  //-----------------------------------------------------------------------------
  tResult tStorageWriter::SaveFile(const char *filename)
  {
    FILE *f=fopen(filename, "w");
    if (!f)
      return SPIRIT_RESULT_BAD_PARAM;

    //write params
    tSMLocalStorage::const_iterator section_it;
    std::map<std::string, std::string>::const_iterator param_it;

    {
      tLock lock(&m_CritSec);
      for (section_it=m_LocalStorage.begin(); section_it!=m_LocalStorage.end(); section_it++)
      {
        fprintf(f, "[%s]\n", section_it->first.c_str());
        for (param_it=section_it->second.begin(); param_it!=section_it->second.end(); param_it++)
          fprintf(f, "%s = %s\n", param_it->first.c_str(), param_it->second.c_str());
        fprintf(f, "\n");
      }
    }

    fclose(f);

    return SPIRIT_RESULT_OK;
  }


  //-----------------------------------------------------------------------------
  tResult tStorageWriter::SetSection(const char *pParamSection)
  {
    if (!pParamSection)
      return SPIRIT_RESULT_BAD_PARAM;

    //set param here
    std::string section = pParamSection;

    {
      tLock lock(&m_CritSec);
      m_LocalStorage[section];
      currentSection=m_LocalStorage.find(section);
      if (currentSection==m_LocalStorage.end())
        return SPIRIT_RESULT_NO_MEM;
    }
    return SPIRIT_RESULT_OK;
  }


  //-----------------------------------------------------------------------------
  tResult tStorageWriter::SetString(const char *pParamName, const char *strParamValue)
  {
    if (!pParamName)
      return SPIRIT_RESULT_BAD_PARAM;

    if (currentSection == m_LocalStorage.end())
      return SPIRIT_RESULT_NOT_READY;

    //set param here
    std::string param = pParamName;

    {
      tLock lock(&m_CritSec);
      currentSection->second[param] = strParamValue;
    }
    return SPIRIT_RESULT_OK;
  }

  //-----------------------------------------------------------------------------
  tResult tStorageWriter::SetInt(const char *pParamName, int paramValue)
  {
    char str[STRING_LENGTH];
    sprintf(str, "%d", paramValue);
    return SetString(pParamName, str);
  }

  //-----------------------------------------------------------------------------
  tResult tStorageWriter::SetBool(const char *pParamName, bool paramValue)
  {
    return SetString(pParamName, paramValue ? "true" : "false");
  }
}
