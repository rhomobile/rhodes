#ifndef __INTERPROCESS_H_
#define __INTERPROCESS_H_

#define COPYDATA_INTERPROCESSMESSAGE 100
#define EVENTNAME_PREFIX L".Intent"

namespace rho
{

struct InterprocessMessage
{
    char params[1024];
    char appName[1024];
};

//
void waitIntentEvent(const rho::String& appName);
//
bool createIntentEvent();
//
void fireIntentEvent();

}

#endif