
#include "..\include\winrtsock.h"

u_short __stdcall ntohs(u_short n)
{
    return  (((((u_short) (n) & 0xFF)) << 8) | (((u_short) (n) & 0xFF00) >> 8));
}