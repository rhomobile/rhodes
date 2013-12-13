#ifndef __INTERPROCESS_H_
#define __INTERPROCESS_H_

#define COPYDATA_INTERPROCESSMESSAGE 100

namespace rho
{

struct InterprocessMessage
{
    char params[1024];
    char appName[1024];
};

}

#endif