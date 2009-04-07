#ifndef _RHOLOGCAT_H_
#define _RHOLOGCAT_H_

#include "common/RhoPort.h"
#include "RhoLogConf.h"

namespace rho {

class LogCategory{
    String m_strName;
public:
    LogCategory(const char* szName = "") : m_strName(szName){}

    const String& getName()const{ return m_strName; }
    bool          isEmpty()const{ return m_strName.length() == 0; }
};

}

extern rho::LogCategory __rhoCurrentCategory;

#define DEFINE_LOGCLASS static rho::LogCategory __rhoCurrentCategory
#define IMPLEMENT_LOGCLASS(classname, name) \
    rho::LogCategory classname::__rhoCurrentCategory = name

#endif //_RHOLOGCAT_H_
