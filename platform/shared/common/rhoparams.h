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

    const rho_param * findHashParam(const char* name) const;

public:
    CRhoParams(rho_param *p);
    CRhoParams(const CRhoParams& copy) : m_pParams(copy.m_pParams){}

    boolean has(const char* name) const;

    String getString(const char* name) const;
    String getString(const char* szName, const char* szDefValue) const;

    void getHash(const char* name, Hashtable<String,String>& mapHeaders) const;
    boolean getBool(const char* name) const;

    void free_params();
};

class CRhoParamArray : public CRhoParams
{
    rho_array * m_array;
public:
    CRhoParamArray(CRhoParams& oParams, const char* name);
    int size() const;

    CRhoParams getItem(int nIndex) const;
};

} // namespace rho

#endif /* RHO_PARAMSWRAP_H */
