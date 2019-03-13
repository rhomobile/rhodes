#pragma once


#include "../api/IMutableString.h"
#include "ObjectImpl.h"


namespace rho {
namespace ruby {



class MutableStringImpl : public IMutableString, ObjectImpl  {

public:
    MutableStringImpl(bool is_mutable);
    virtual ~MutableStringImpl();

    virtual const char* getClassName();

    virtual const char* getUTF8();
    virtual void setUTF8(const char* utf8_string);

private:
    char* mString;
};

}
}
