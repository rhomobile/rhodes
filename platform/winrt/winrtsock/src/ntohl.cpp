
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

u_long __stdcall ntohl(u_long x)
{
    u_char *s = (u_char *) &x;
    return (uint32_t) (s[0] << 24 | s[1] << 16 | s[2] << 8 | s[3]);
}