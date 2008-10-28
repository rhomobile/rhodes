//Utility macros used for error-checking, etc.

#pragma once


#define _ErrorLabel Error


#define CHR(hResult) \
    if(FAILED(hResult)) { hr = (hResult); goto _ErrorLabel;} 


#define CPR(pPointer) \
    if(NULL == (pPointer)) { hr = (E_OUTOFMEMORY); goto _ErrorLabel;} 


#define CBR(fBool) \
    if(!(fBool)) { hr = (E_FAIL); goto _ErrorLabel;} 


#define ARRAYSIZE(s) (sizeof(s) / sizeof(s[0]))


#define RELEASE_OBJ(s)  \
    if (s != NULL)      \
    {                   \
        s->Release();   \
        s = NULL;       \
    }


  

