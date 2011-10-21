/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
