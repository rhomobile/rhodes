#ifndef RHO_PARAMSWRAP_H
#define RHO_PARAMSWRAP_H

#include "common/RhoStd.h"
#include "ruby/ext/rho/rhoruby.h"

namespace rho
{

class CRhoParams
{
protected:
    rho_param * m_pParams;

    rho_param * findHashParam(const char* name);

public:
    CRhoParams(rho_param *p);
    CRhoParams(const CRhoParams& copy) : m_pParams(copy.m_pParams){} 

    String getString(const char* name);
    String getString(const char* szName, const char* szDefValue);

    void getHash(const char* name, Hashtable<String,String>& mapHeaders);
    boolean getBool(const char* name);

    void free();
};

class CRhoParamArray : public CRhoParams
{
    rho_array * m_array;
public:
    CRhoParamArray(CRhoParams& oParams, const char* name);
    int size();

    CRhoParams getItem(int nIndex);
};

} // namespace rho

#endif /* RHO_PARAMSWRAP_H */
