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

#define DEFINE_LOGCLASS static rho::LogCategory __rhoCurrentCategory;\
    static rho::LogCategory getLogCategory(){return __rhoCurrentCategory;}

#define IMPLEMENT_LOGCLASS(classname, name) \
    rho::LogCategory classname::__rhoCurrentCategory = name

#define DEFINE_BASELOGCLASS rho::LogCategory __rhoCurrentCategory;\
    rho::LogCategory getLogCategory(){return __rhoCurrentCategory;}\
    void setLogCategory(const rho::LogCategory& cat){__rhoCurrentCategory = cat;}

#endif //_RHOLOGCAT_H_
