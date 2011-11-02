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

#include "common/phonebook/PhonebookParams.h"
#include "common/rhoparams.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Phonebook"

const PbPageParam* rho_phonebook_getpageparams(rho_param* param, PbPageParam* page)
{
    bool offset = false;
    bool per_page = false;
    if(param == 0 || page == 0)
    {
        RAWTRACE2("Param(0x%.8X) or page(0x%.8X) is NULL, skipping", param, page);
        return 0;
    }
    if (param->type == RHO_PARAM_HASH)
    {
        for (int i = 0, lim = param->v.hash->size; i < lim; ++i)
        {
            const char* key = param->v.hash->name[i];
            rho_param* value = param->v.hash->value[i];
            
            if (strcasecmp(key, "offset") == 0)
            {
                RAWTRACE("'offset' parameter found");
                if (value->type == RHO_PARAM_STRING)
                {
                    RAWTRACE1("'offset' parameter is string: %s", value->v.string);
                    sscanf(value->v.string, "%d", &page->offset);
                    offset = true;
                }
            } else if (strcasecmp(key, "per_page") == 0)
            {
                RAWTRACE("'per_page' parameter found");
                if (value->type == RHO_PARAM_STRING)
                {
                    RAWTRACE1("'per_page' parameter is string: %s", value->v.string);
                    sscanf(value->v.string, "%d", &page->per_page);
                    per_page = true;
                }
            }
            if(offset && per_page) break;
        }
    }
    else
    {
        RAWTRACE("rho_param is not a hash!");
    }
    if(!offset)
        page->offset = 0;
    if(!per_page)
        page->per_page = -1;

    return page;
}
//----------------------------------------------------------------------------------------------------------------------
const rho_param* rho_phonebook_getselectparams(rho_param* param)
{
    if(param == 0)
        return 0;
    else
        return rho::CRhoParams(param).findHashParam("select");
}
//----------------------------------------------------------------------------------------------------------------------
const rho_param* rho_phonebook_getconditions(rho_param* param)
{
    if(param == 0)
        return 0;
    else
        return rho::CRhoParams(param).findHashParam("conditions");
}
//----------------------------------------------------------------------------------------------------------------------
