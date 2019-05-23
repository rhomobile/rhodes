#pragma once
#include "../api/IRhoRuby.h"

namespace rho {
namespace ruby {

class RubyServerResponceImpl : public IRubyServerResponce 
{
public:
    virtual ~RubyServerResponceImpl();
    RubyServerResponceImpl(const char* body);
    virtual IString* getResponceBody();
    
private:
    char* mBody;
};
    

        
}
}
