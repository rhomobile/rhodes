
#include "..\include\winrtsock.h"
#include <string>
#include <ppltasks.h>
#include <memory>
#include "include\MiscUtils.h"
#include "include\InternalSocketManagement.h"

using namespace std;
using namespace concurrency;
using namespace Windows::Networking;
using namespace Platform;
using namespace Windows::Networking::Sockets;
using namespace Windows::Foundation::Collections;
using namespace Windows::Networking;

struct hostent * __stdcall gethostbyname(const char * name)
{
    return gethostbyaddr(name, strlen(name), AF_INET);
}