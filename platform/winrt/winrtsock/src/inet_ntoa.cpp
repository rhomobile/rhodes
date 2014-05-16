
#include "..\include\winrtsock.h"
#include "include\InternalSocketManagement.h"
#include <string>

char * __stdcall inet_ntoa(struct in_addr in)
{
    auto buf = GlobalSettings::Get_inet_ntoa_String();
    
    auto& ucp = in.S_un.S_un_b;

    sprintf_s(buf, MAX_IP_ADDRESS_CHAR_LEN, "%d.%d.%d.%d", ucp.s_b1 & 0xff, ucp.s_b2 & 0xff, ucp.s_b3 & 0xff, ucp.s_b4 & 0xff);

    return buf;
}